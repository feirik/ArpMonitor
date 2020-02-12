#include <sstream>

#include "Monitor.h"
#include "Math.h"
#include "Log.h"

#define MIN_DELAY 1
#define MAX_DELAY 30

int main(int argc, char* argv[])
{
	std::cout << GetCurrentTimeAsString() << " ARP monitor started..." << std::endl;

	int delay = 0;

	if (argc == 2 && math::IsInteger(argv[1]) == 1)
	{
		std::istringstream ss(argv[1]);
		ss >> delay;

		if (1 > delay || delay > 30)
		{
			std::cout << "ERROR. Usage: Enter integer delay " << MIN_DELAY << "-" << MAX_DELAY << " seconds as an agrument." << std::endl;
			return 0;
		}

		std::cout << "Delay: " << argv[1] << std::endl;
	}
	else
	{
		std::cout << "ERROR. Usage: Enter integer delay " << MIN_DELAY << "-" << MAX_DELAY << " seconds as an agrument." << std::endl;
	}

	Monitor monitor(delay);

	return 0;
}