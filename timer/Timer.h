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

/**
 * an interval timer class that can notify a delegate about timer events i.e. when
 *   the timer started, when an interval timed-out, and when the timer is finished;
 *   this should provide functionality similar to alarm() and setitimer() which
 *   both send signal upon timer expiration, but instead of signal, it sends
 *   notifications to its delegate; this is done to eliminate the side effects of
 *   signals to functions such as sem_wait() which can be interrupted by a signal
 *   handler
 *
 * internally, the timer is implemented by maintaining two threads: first is the
 *   notifier thread which processes a queue of notifications and dispatches all
 *   notifications to the delegate, and the second is the timer thread which
 *   employs nanosleep() to repeatedly block for the interval's duration and
 *   queues notifications to be later processed by the notifier thread
 */

#ifndef TIMER_H
#define TIMER_H

#include <pthread.h>
#include <queue>
#if __APPLE__
#include <mach/mach.h>
#include <mach/mach_time.h>
#endif

class Timer;

/**
 * timer delegate protocol
 */
class TimerDelegate
{
public:
	virtual ~TimerDelegate() { }
	virtual void started(Timer *timer) { }
    virtual void timeout(Timer *timer) { }
    virtual void finished(Timer *timer) { }
};

/**
 * timer information (used internally)
 */
typedef struct TimerInfo {
	Timer *timer;
	TimerDelegate *delegate;
	long double seconds;
	int repetitions;
	std::queue<int> queue;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
#if __APPLE__
	mach_timebase_info_data_t timebase;
#endif
} TimerInfo;

/**
 * timer class
 */
class Timer
{
public:
	/**
	 * constructor
	 */
	Timer();

	/**
	 * destructor
	 */
	~Timer();

	/**
	 * start the timer
	 *
	 * delegate - the delegate conforming to the TimerDelegate protocol
	 * seconds - interval duration in seconds
	 * repetitions - number of interval repetitions
	 *
	 * returns zero if successful, non-zero otherwise
	 */
	int start(TimerDelegate &delegate, long double seconds, int repetitions);

	/**
	 * stop the timer
	 */
	void stop();

	/**
	 * indicates whether the timer is running or not
	 *
	 * returns 'true' if running, 'false' otherwise
	 */
	bool isRunning();

private:
	bool running;
	int timerThread;
	int notifierThread;
	pthread_t timerThreadID;
	pthread_t notifierThreadID;
	TimerInfo info;
};

#endif // TIMER_H
