CC=g++
LIBS=-lpthread
ifeq ($(shell uname),Linux)
LIBS+=-lrt
endif

default: TimerTest

clean:
	rm -f TimerTest
	
TimerTest:
	$(CC) -o $@ Timer.cpp TimerTest.cpp $(LIBS)
