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
 * a convenience class to interface with Linux's input devices (/dev/input); this
 *   is a delegate-based implementation with the delegate receiving notifications
 *   from the class
 */

#ifndef INPUT_H
#define INPUT_H

#include <linux/input.h>
#include <pthread.h>
#include <string>

using namespace std;

/**
 * system path to input devices
 */
#define INPUT_PATH	"/dev/input/"

/**
 * input class
 */
class Input;

/**
 * input delegate protocol
 */
class InputDelegate
{
public:
	virtual ~InputDelegate() { }
	virtual void activated(Input *input) { }
	virtual void changed(Input *input, int value) { }
	virtual void event(Input *input, struct input_event *event) { }
	virtual void deactivated(Input *input) { }
};

/**
 * input information (internally used)
 */
typedef struct {
	Input *input;
	bool isActive;
	string name;
	int fd;
	InputDelegate *delegate;
} InputInfo;

/**
 * input class
 */
class Input
{
public:
	/**
	 * constructor
	 *
	 * delegate - the delegate conforming to the InputDelegate protocol
	 * name - the name of the input device as seen on INPUT_PATH
	 */
	Input(InputDelegate &delegate, string name);

	/**
	 * destructor
	 */
	~Input();

	/**
	 * indicates whether the input device is active or not
	 *
	 * returns 'true' if active, 'false' otherwise
	 */
	bool isActive();

	/**
	 * get the name of the input device
	 *
	 * returns the name in string representing the input device
	 */
	string name();

private:
	int threadStatus;
	pthread_t threadID;
	InputInfo info;
};

#endif // INPUT_H
