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

#include "GPIO.h"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>

using namespace std;

class GPIOTest : public GPIODelegate
{
public:
	virtual void triggered(GPIO *gpio, GPIOInputTriggerEdge inputTriggerEdge)
	{
		printf("[GPIOTest::triggered] number=%d inputTriggerEdge=%d\n", gpio->number(), inputTriggerEdge);
	}
};

int main(int argc, char **argv)
{
	if(argc < 2)
	{
		printf("[Usage] %s <GPIO number>\n", argv[0]);
		return -1;
	}

	GPIOTest gpioTest;
	GPIO gpio(atoi(argv[1]), true, GPIO_DIRECTION_OUTPUT, GPIO_STATE_LOW, GPIO_ACTIVE_STATE_HIGH, GPIO_INPUT_TRIGGER_EDGE_NONE, gpioTest);

	if(!gpio.isActive())
	{
		fprintf(stderr, "[Error] Failed to activate GPIO %d.\n", atoi(argv[1]));
		return -2;
	}

	const char help[] = {
			"[Help]\n"
			" <enter> to show all GPIO information\n"
			" \"n\" to show the GPIO number\n"
			" \"d\" to show the current direction\n"
			" \"d <0 for output, 1 for input>\" to set the direction\n"
			" \"s\" to show the current state\n"
			" \"s <0 for low, 1 for high>\" to set the state\n"
			" \"a\" to show the current active state\n"
			" \"a <0 for active high, 1 for active low>\" to set the active state\n"
			" \"t\" to show the current input trigger edge\n"
			" \"t <0 for none, 1 for rising, 2 for falling, 3 for both>\" to set the input trigger edge\n"
			" \"r\" to show the current input polling rate\n"
			" \"r <polling rate greater than zero>\" to set the input polling rate\n"
			" \"q\" to quit\n"
	};

	char buffer[100];
	char cmd[5];
	double arg;
	int items;
	int value;
	stringstream info;

	while(1)
	{
		printf("%s\n>> ", info.str().c_str());
		fgets(buffer, 99, stdin);
		cmd[0] = '\0';
		items = sscanf(buffer, "%3s %lf", cmd, &arg);

		if(items < 3 && (cmd[0] == '\0' || cmd[1] == '\0'))
		{
			if(cmd[0] == 'q')
			{
				if(items == 1)
					break;
			}
			else
			{
				info.str("");

				switch(cmd[0])
				{
				case '\0':
				case 'n':
					if(cmd[0] == 'n' && items != 1)
						break;

					info << "number: " << gpio.number() << endl;

					if(cmd[0] == 'n')
						continue;

				case 'd':
					if(cmd[0] == 'd' && items == 2)
						gpio.setDirection((GPIODirection)arg);

					value = (int)gpio.direction();
					info << "direction: [" << value << "] ";

					switch(value)
					{
					case GPIO_DIRECTION_OUTPUT:
						info << "output";
						break;

					case GPIO_DIRECTION_INPUT:
						info << "input";
						break;

					default:
						break;
					}

					info << endl;

					if(cmd[0] == 'd')
						continue;

				case 's':
					if(cmd[0] == 's' && items == 2)
						gpio.setState((GPIOState)arg);

					value = (int)gpio.state();
					info << "state: [" << value << "] ";

					switch(value)
					{
					case GPIO_STATE_LOW:
						info << "low";
						break;

					case GPIO_STATE_HIGH:
						info << "high";
						break;

					default:
						break;
					}

					info << endl;

					if(cmd[0] == 's')
						continue;

				case 'a':
					if(cmd[0] == 'a' && items == 2)
						gpio.setActiveState((GPIOActiveState)arg);

					value = (int)gpio.activeState();
					info << "active state: [" << value << "] ";

					switch(value)
					{
					case GPIO_ACTIVE_STATE_LOW:
						info << "active low";
						break;

					case GPIO_ACTIVE_STATE_HIGH:
						info << "active high";
						break;

					default:
						break;
					}

					info << endl;

					if(cmd[0] == 'a')
						continue;

				case 't':
					if(cmd[0] == 't' && items == 2)
						gpio.setInputTriggerEdge((GPIOInputTriggerEdge)arg);

					value = (int)gpio.inputTriggerEdge();
					info << "input trigger edge: [" << value << "] ";

					switch(value)
					{
					case GPIO_INPUT_TRIGGER_EDGE_NONE:
						info << "none";
						break;

					case GPIO_INPUT_TRIGGER_EDGE_RISING:
						info << "rising";
						break;

					case GPIO_INPUT_TRIGGER_EDGE_FALLING:
						info << "falling";
						break;

					case GPIO_INPUT_TRIGGER_EDGE_BOTH:
						info << "rising and falling";
						break;

					default:
						break;
					}

					info << endl;

					if(cmd[0] == 't')
						continue;

				case 'r':
					if(cmd[0] == 'r' && items == 2)
						gpio.setInputPollingRate(arg);

					info << "input polling rate: " << gpio.inputPollingRate() << " Hz" << endl;
					continue;

				default:
					break;
				}
			}
		}

		printf("%s", help);
		info.str("");
	}

	return 0;
}
