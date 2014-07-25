CC=g++
CFLAGS=-Wall

default all: InputTest GPIOTest

clean:
	rm -f InputTest GPIOTest

InputTest:
	$(CC) $(CFLAGS) -o $@ Input.cpp InputTest.cpp -lpthread

GPIOTest:
	$(CC) $(CFLAGS) -o $@ GPIO.cpp GPIOTest.cpp -lpthread
