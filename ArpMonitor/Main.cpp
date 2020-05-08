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
	std::cout << "ERROR. Enter integer delay " << MIN_DELAY << "-" << MAX_DELAY << " seconds as an agrument to -d or --delay." << std::endl;
}

int main(int argc, char* argv[])
{
	userInput inputs;

	CLI::App app("ARP monitor for tracking changes to the ARP cache.");

	app.add_option("-i,--interface", inputs.interfaceIn, "Set specific interface IP address to monitor.");

	int delay = DEFAULT_DELAY;
	app.add_option("-d,--delay", inputs.delay, "Set delay in seconds between ARP cache checks.");

	bool logOnlyFlag = false;
	app.add_flag("-l,--logonly", inputs.logOnlyFlag, "Only output events to logfile when set.");

	bool passiveFlag = false;
	app.add_flag("-p,--passive", inputs.passiveFlag, "Passively monitor ARP cache if set. (No DNS lookup)");

	CLI11_PARSE(app, argc, argv);

	// Checking user delay input, if entered
	if (inputs.delay != DEFAULT_DELAY)
	{
		if (MIN_DELAY > inputs.delay || inputs.delay > MAX_DELAY)
		{
			PrintDelayError();
			return 0;
		}
	}

	// Checking user interface input, if entered
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

	Monitor monitor(inputs);

	return 0;
}