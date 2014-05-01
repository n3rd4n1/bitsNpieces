bitsNpieces
===========

tc-bfs
------
Set of bash scripts to build a GNU toolchain from scratch. Good as reference only.

algo
----
Some algorithms.

### binheap.c
A binary heap implementation in linked list data structure.

timer
-----
Tick-tock.

### Timer.cpp, Timer.h, TimerTest.cpp

An interval timer class that can notify a delegate about timer events i.e. when
the timer started, when an interval timed-out, and when the timer is finished. This
should provide functionality similar to alarm() and setitimer() which both send
signal upon timer expiration, but instead of signal, it sends notifications to its
delegate. This is done to eliminate the side effects of signals to functions such as
sem_wait() which can be interrupted by a signal handler.

Internally, the timer is implemented by maintaining two threads: first is the notifier
thread which processes a queue of notifications and dispatches all notifications to
the delegate, and the second is the timer thread which employs nanosleep() to repeatedly
block for the interval's duration and queues notifications to be later processed by the
notifier thread.

This uses the `libpthread` library, and the `librt` library on `Linux`, and some platform
specific time functions. This should be useable in `Linux` and `OS X`. Not tested on
other Unix platforms.
