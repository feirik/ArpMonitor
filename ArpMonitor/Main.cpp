#include "Log.h"
#include "Math.h"
#include "Monitor.h"
#include "Command.h"
#include "CLI11.hpp"

#include <sstream>

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
	userInput inputs;

	CLI::App app("ARP monitor for tracking changes to the ARP cache.");

	//std::string interfaceIn = "";
	app.add_option("-i,--interface", inputs.interfaceIn, "Set specific interface IP address to monitor.");

	int delay = DEFAULT_DELAY;
	app.add_option("-d,--delay", inputs.delay, "Set delay in seconds between ARP cache checks.");

	bool logOnlyFlag = false;
	app.add_flag("-l,--logonly", inputs.logOnlyFlag, "Only output events to logfile when set.");

	/*bool passiveFlag = false;
	app.add_flag("-p,--passive", inputs.passiveFlag, "Passively monitor ARP cache if set.");*/

	CLI11_PARSE(app, argc, argv);

	//std::cout << "Interface: " << inputs.interfaceIn << std::endl;

	//std::cout << "Passiveflag: " << passiveFlag << std::endl;

	//std::cout << "Delay: " << inputs.delay << std::endl;

	//Checking user input
	if (inputs.delay != DEFAULT_DELAY)
	{
		if (MIN_DELAY > inputs.delay || inputs.delay > MAX_DELAY)
		{
			PrintDelayError();
			return 0;
		}
	}

	if (inputs.interfaceIn != "")
	{
		if (inputs.interfaceIn.length() < 16)
		{
			std::string ArpOutput = cmd::GetCommandOutput("arp -a");

			std::string target = "Interface: " + inputs.interfaceIn;

			std::size_t charPos = ArpOutput.find(target);

			if (charPos == std::string::npos)
			{
				std::cout << "Interface " << inputs.interfaceIn << " is an invalid interface." << std::endl;
				return 0;
			}
		}
		else
		{
			std::cout << "Interface " << inputs.interfaceIn << " is an invalid interface." << std::endl;
			return 0;
		}
	}
	else
	{
		std::cout << "Interface is blank" << std::endl;
	}

	Monitor monitor(inputs);

	return 0;
}