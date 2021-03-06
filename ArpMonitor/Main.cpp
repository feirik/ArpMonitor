#include "Log.h"
#include "Math.h"
#include "Monitor.h"
#include "Command.h"

#include "CLI11.hpp"

#include <sstream>

#define MIN_DELAY_IN_SECONDS 1
#define MAX_DELAY_IN_SECONDS 30

#define MAX_INTERFACE_LENGTH 15

void PrintDelayError()
{
	std::cout << "ERROR. Enter integer delay " << MIN_DELAY_IN_SECONDS << "-" << MAX_DELAY_IN_SECONDS << " seconds as an agrument to -d or --delay." << std::endl;
}

int main(int argc, char* argv[])
{
	userInput inputs;

	CLI::App app("ARP monitor for tracking changes to the ARP cache.");

	app.add_option("-i,--interface", inputs.interfaceIn, "Set specific interface IP address to monitor.");
	app.add_option("-d,--delay", inputs.delay, "Set delay in seconds between ARP cache checks.");
	app.add_option("-o,--output", inputs.logPath, "Set path for logfile.");

	app.add_flag("-l,--logonly", inputs.logOnlyFlag, "Only output events to logfile when set.");
	app.add_flag("-p,--passive", inputs.passiveFlag, "Passively monitor ARP cache if set. (No DNS lookup)");

	CLI11_PARSE(app, argc, argv);

	// Checking user delay input, if entered
	if (inputs.delay != DEFAULT_DELAY_IN_SECONDS)
	{
		if (MIN_DELAY_IN_SECONDS > inputs.delay || inputs.delay > MAX_DELAY_IN_SECONDS)
		{
			PrintDelayError();
			return 0;
		}
	}

	// Checking user interface input, if entered
	if (inputs.interfaceIn != "")
	{
		if (inputs.interfaceIn.length() <= MAX_INTERFACE_LENGTH)
		{
			const std::string ArpOutput = cmd::GetCommandOutput("arp -a");

			const std::string target = "Interface: " + inputs.interfaceIn;

			// Searching an arp lookup output to check whether interface is valid
			const std::size_t charPos = ArpOutput.find(target);

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

	if (inputs.logPath != DEFAULT_LOG_PATH)
	{
		std::ofstream outFile;

		outFile.open(inputs.logPath, std::ios_base::app);

		if (!outFile.is_open())
		{
			std::cout << "Could not open output file with path " << inputs.logPath << ". Exiting." << std::endl;
			return 0;
		}

		outFile.close();
	}

	Monitor monitor(inputs);

	return 0;
}