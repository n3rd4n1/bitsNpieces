/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2014 Billy Millare
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "Timer.h"
#include <cstring>
#include <ctime>
#include <sched.h>

/**
 * notification identifiers
 */
enum {
	TimerNotificationStarted,
	TimerNotificationTimeout,
	TimerNotificationFinished
};

/**
 * the notifier thread which sends timer notifications to the delegate; this
 *   thread processes a queue of notifications and forwards each notification
 *   to the delegate; a 'timer started' notification is sent once the timer
 *   has started, 'interval timeout' notification for each timer interval
 *   timeout, and a 'timer finished' notification once the timer has ended
 *   without interruptions i.e. stop
 */
static void * notifier(TimerInfo *info)
{
	/**
	 * make this thread cancellable anytime
	 */
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

	while(1)
	{
		/**
		 * block until there's a notification to process
		 */
		pthread_mutex_lock(&info->mutex);

		while(info->queue.size() < 1)
			pthread_cond_wait(&info->cond, &info->mutex);

		pthread_mutex_unlock(&info->mutex);

		/**
		 * dispatch all notifications
		 */
		while(info->queue.size() > 0)
		{
			switch(info->queue.front())
			{
			case TimerNotificationStarted:
				info->delegate->started(info->timer);
				break;

			case TimerNotificationTimeout:
				info->delegate->timeout(info->timer);
				break;

			case TimerNotificationFinished:
				info->delegate->finished(info->timer);
				break;

			default:
				break;
			}

			info->queue.pop();
		}
	}

	return NULL;
}

/**
 * queue a notification for the notifier thread to process
 */
static void queueNotification(TimerInfo *info, int notification)
{
	pthread_mutex_lock(&info->mutex);
	info->queue.push(notification);
	pthread_cond_signal(&info->cond);
	pthread_mutex_unlock(&info->mutex);
}

/**
 * the timer thread which queues notifications for the notifier thread
 *   to process
 */
static void * timer(TimerInfo *info)
{
	unsigned long long intervalCount;
	struct timespec interval;
	struct timespec *currentInterval;
	struct timespec remainingInterval;
#if __APPLE__
	uint64_t startTime;
#else
	struct timespec startTime;
	struct timespec currentTime;
#endif
	long double adjustedInterval;

	/**
	 * make this thread cancellable anytime
	 */
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

	/**
	 * set the interval count
	 */
	intervalCount = 0;

	/**
	 * set the interval duration
	 */
	adjustedInterval = info->seconds;

	/**
	 * send the 'timer started' notification
	 */
	queueNotification(info, TimerNotificationStarted);

	/**
	 * mark the start time; this is used to compensate for drifts
	 */
#if __APPLE__
	startTime = mach_absolute_time();
#else
	clock_gettime(CLOCK_MONOTONIC, &startTime);
#endif

	/**
	 * repeat for maximum 'repetitions'
	 */
	while(1)
	{
		/**
		 * block for approximately interval duration
		 */
		if(adjustedInterval > 0)
		{
			interval.tv_sec = (time_t)adjustedInterval;
			interval.tv_nsec = (long int)((adjustedInterval - (long double)interval.tv_sec) * 1000000000);
			currentInterval = &interval;

			while(nanosleep(currentInterval, &remainingInterval) == -1 && (remainingInterval.tv_sec > 0 && remainingInterval.tv_nsec > 0))
				currentInterval = &remainingInterval;
		}

		/**
		 * send an 'interval timeout' notification
		 */
		queueNotification(info, TimerNotificationTimeout);

		/**
		 * check if this is the last interval
		 */
		if(info->repetitions > 0)
			--info->repetitions;
		else if(info->repetitions == 0)
		{
			/**
			 * send the 'timer finished' notification, then break
			 */
			queueNotification(info, TimerNotificationFinished);
			break;
		}

		/**
		 * increment interval count
		 */
		++intervalCount;

		/**
		 * compute the remaining time for the next interval; this compensates for drift
		 */
#if __APPLE__
		adjustedInterval = (((long double)(mach_absolute_time() - startTime) * (long double)info->timebase.numer) / (long double)info->timebase.denom) / (long double)1000000000;
#else
		clock_gettime(CLOCK_MONOTONIC, &currentTime);
		adjustedInterval = (long double)(currentTime.tv_sec - startTime.tv_sec);

		if(currentTime.tv_nsec < startTime.tv_nsec)
		{
			--adjustedInterval;
			currentTime.tv_nsec += 1000000000;
		}

		adjustedInterval += ((long double)(currentTime.tv_nsec - startTime.tv_nsec) / (long double)1000000000);
#endif

		adjustedInterval = info->seconds - (adjustedInterval - ((long double)intervalCount * info->seconds));
	}

	info->timer->stop();
	return NULL;
}

/**
 * spawn thread with heightened priority
 */
static int spawnThread(pthread_t *threadID, void *(*thread)(void *), void *data)
{
	struct sched_param schedParam;
	pthread_attr_t attr;
	schedParam.sched_priority = sched_get_priority_max(SCHED_FIFO);
	pthread_attr_init(&attr);
	pthread_attr_setschedparam(&attr, &schedParam);
	pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
	return pthread_create(threadID, &attr, thread, data);
}

/**
 * initialize and run the notifier thread
 */
Timer::Timer()
{
	running = false;
	info.timer = this;
	pthread_mutex_init(&info.mutex, NULL);
	pthread_cond_init(&info.cond, NULL);
#if __APPLE__
	mach_timebase_info(&info.timebase);
#endif
	timerThread = -1;
	notifierThread = spawnThread(&notifierThreadID, (void * (*)(void *))notifier, &info);
}

/**
 * clean-up
 */
Timer::~Timer()
{
	stop();
	pthread_cond_destroy(&info.cond);
	pthread_mutex_destroy(&info.mutex);

	if(notifierThread == 0)
	{
		pthread_cancel(notifierThreadID);
		pthread_join(notifierThreadID, NULL);
	}
}

/**
 * start the timer
 */
int Timer::start(TimerDelegate &delegate, long double seconds, int repetitions)
{
	stop();
	info.delegate = &delegate;
	info.seconds = seconds;
	info.repetitions = repetitions;
	timerThread = spawnThread(&timerThreadID, (void * (*)(void *))timer, &info);

	if(timerThread != 0 || notifierThread != 0)
	{
		stop();
		return -1;
	}

	running = true;
	return 0;
}

/**
 * stop the timer
 */
void Timer::stop()
{
	running = false;

	if(timerThread == 0)
	{
		pthread_cancel(timerThreadID);
		pthread_join(timerThreadID, NULL);
		timerThread = -1;
	}
}

/**
 * indicates whether timer is running or not
 */
bool Timer::isRunning()
{
	return running;
}
