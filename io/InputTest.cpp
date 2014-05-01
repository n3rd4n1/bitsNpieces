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
#include "Input.h"

/**
 * test class conforming to the InputDelegate protocol
 */
class InputTest : public InputDelegate
{
public:
	virtual void activated(Input *input)
	{
		printf("[Activated]\n");
	}

	virtual void changed(Input *input, int value)
	{
		printf("[Changed] %s %d\n", input->name().c_str(), value);
	}

	virtual void deactivated(Input *input)
	{
		printf("[Deactivated]\n");
	}
};

int main(int argc, char **argv)
{
	/**
	 * check
	 */
	if(argc < 2)
	{
		printf("[Usage] %s <Input name as seen in "INPUT_PATH">\n", argv[0]);
		exit(-1);
	}

	InputTest inputTest;
	Input input(inputTest, string(argv[1]));

	/**
	 * print some information for the user
	 */
	printf("Press 'Enter' to stop...\n");

	/**
	 * block until user presses the enter key
	 */
	while(getchar() != '\n');

	/**
	 * done
	 */
	return 0;
}
