#include <sstream>

#include "Monitor.h"
#include "Math.h"
#include "Log.h"

#define MIN_DELAY 1
#define MAX_DELAY 30

void PrintDelayError()
{
	std::cout << "ERROR. Usage: Enter integer delay " << MIN_DELAY << "-" << MAX_DELAY << " seconds as an agrument." << std::endl;
}

/*
Input of a int pointer delay, const char* argument
Stores argument in integer pointer
Returns 1 if delay value is within limits, else false and prints error message
*/
bool ProcessDelay(int* delay, const char* argv)
{
	std::istringstream ss(argv);
	ss >> *delay;

	if (MIN_DELAY > *delay || *delay > MAX_DELAY)
	{
		PrintDelayError();
		return 0;
	}
	else
	{
		return 1;
	}
}

int main(int argc, char* argv[])
{
	std::cout << GetCurrentTimeAsString() << " ARP monitor started..." << std::endl;

	int delay = 0;
	bool writeToConsole = 1;

	if (argc == 2 && math::IsInteger(argv[1]) == true)
	{
		bool result = ProcessDelay(&delay, argv[1]);

		if (!result)
		{
			return 0;
		}
	}
	else if (argc == 3 && math::IsInteger(argv[1]) == true)
	{
		std::string argv2 = argv[2];

		if (argv2 == "-logonly")
		{
			bool result = ProcessDelay(&delay, argv[1]);

			if (!result)
			{
				return 0;
			}

			writeToConsole = 0;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		PrintDelayError();
	}

	Monitor monitor(delay, writeToConsole);

	return 0;
}