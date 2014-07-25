bitsNpieces
===========

algo
----
Some algorithms.

### binheap.c
A binary heap implementation in linked list data structure.

fconv
-----
File converters.

### intelhex

A file converter between Intel's hexadecimal object file and a simple binary file.

io
--
In and out.

### Input.cpp, Input.h, InputTest.cpp

A convenience class to interface with Linux's input devices, `/dev/input`. This is a
delegate-based implementation with the delegate receiving notifications from the class.

This uses the `pthread` library and is usable only in Linux.

### GPIO.cpp, GPIO.h, GPIOTest.cpp

A convenience class to interface with Linux's GPIO Sysfs, `/sys/class/gpio`. This is a
delegate-based implementation with the delegate receiving  notifications from the class.

This uses the `pthread` library and is usable only in Linux.

tc-bfs
------
Set of bash scripts to build a GNU toolchain from scratch. Good as reference only.

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

This uses the `pthread` library, as well as the `rt` library in Linux. This also uses
some platform-specific time functions. This should be useable in Linux and OS X; not
tested on other Unix platforms.
