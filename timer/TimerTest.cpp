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

#include <cstdio>
#include <cstdlib>
#include <sys/time.h>
#include "Timer.h"

/**
 * reference time (time 0)
 */
static struct timeval referenceTime;

/**
 * test class conforming to the TimerDelegate protocol
 */
class TimerTest : public TimerDelegate
{
public:

	virtual void started(Timer *timer)
	{
		printf("[Started]\n");
	}

	virtual void timeout(Timer *timer)
	{
		struct timeval now;
		gettimeofday(&now, NULL);
		printf("[Timeout] %f seconds elapsed\n", (double)((now.tv_sec * 1000000 + now.tv_usec) - (referenceTime.tv_sec * 1000000 + referenceTime.tv_usec)) / 1000000);
	}

	virtual void finished(Timer *timer)
	{
		printf("[Finished]\n");
		exit(0);
	}
};

int main(int argc, char **argv)
{
	TimerTest timerTest;
	Timer timer;
	double interval;
	int repetitions;

	/**
	 * check
	 */
	if(argc < 2)
	{
		printf("[Usage] %s <duration in seconds> <repetitions>\n", argv[0]);
		exit(-1);
	}

	/**
	 * get the test parameters
	 */
	interval = strtod(argv[1], NULL);
	repetitions = (argc > 2) ? atoi(argv[2]) : 0;

	/**
	 * print the test parameters
	 */
	printf("TimerTest: interval=%f, repetitions=%d\n", interval, repetitions);

	/**
	 * set the reference time (time 0)
	 */
	gettimeofday(&referenceTime, NULL);

	/**
	 * print some information to the user
	 */
	printf("Press 'Enter' to stop...\n\n");

	/**
	 * start the timer
	 */
	if(timer.start(timerTest, interval, repetitions) != 0)
	{
		printf("Failed to start the timer!\n");
		exit(-2);
	}

	/**
	 * block until user presses the enter key
	 */
	while(getchar() != '\n');

	/**
	 * stop the timer
	 */
	timer.stop();

	/**
	 * done
	 */
	return 0;
}
