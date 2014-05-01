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

#include "Input.h"
#include <cstdio>
#include <fcntl.h>

/**
 * the watcher thread which polls for input events and sends them as
 *   notifications to the delegate
 */
static void * watcher(InputInfo *info)
{
	ssize_t size;
	struct input_event event;

	/**
	 * make this thread cancellable anytime
	 */
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

	/**
	 * input device is now active
	 */
	info->isActive = true;

	/**
	 * send 'activated' notification to the delegate
	 */
	info->delegate->activated(info->input);

	/**
	 * repeatedly read input data from a file
	 */
	while(1)
	{
		/**
		 * read input data from a file
		 */
		size = read(info->fd, &event, sizeof(struct input_event));

		/**
		 * check for fatal error
		 */
		if(size < 0)
		{
			perror("read");
			break;
		}

		/**
		 * check if we have a valid event data
		 */
		if(size != sizeof(event))
			continue;

		/**
		 * send the 'received event' notification to the delegate
		 */
		info->delegate->event(info->input, &event);

		/**
		 * send the 'input changed' notification to the delegate
		 */
		if(event.code != KEY_RESERVED)
			info->delegate->changed(info->input, event.value);
	}

	/**
	 * input device is now inactive
	 */
	info->isActive = false;

	/**
	 * send 'deactivated' notification to the delegate
	 */
	info->delegate->deactivated(info->input);

	return NULL;
}

/**
 * initialize and activate the input device watcher thread
 */
Input::Input(InputDelegate &delegate, string name)
{
	string path = string(INPUT_PATH) + name;

	threadStatus = -1;
    info.input = this;
	info.isActive = false;
	info.name = name;
    info.delegate = &delegate;

    if((info.fd = open(path.data(), O_RDONLY)) == -1)
    {
    	perror("open");
    	return;
    }

    if((threadStatus = pthread_create(&threadID, NULL, (void * (*)(void *))watcher, &info)) != 0)
    	perror("pthread_create");
}

/**
 * clean-up
 */
Input::~Input()
{
	if(threadStatus == 0)
	{
		pthread_cancel(threadID);
		pthread_join(threadID, NULL);
		threadStatus = -1;
	}

	info.isActive = false;

	if(info.fd != -1)
	{
		close(info.fd);
		info.fd = -1;
	}
}

/**
 * indicates whether the input device is active or not
 */
bool Input::isActive()
{
	return info.isActive;
}

/**
 * provides the name of the input device
 */
string Input::name()
{
	return info.name;
}
