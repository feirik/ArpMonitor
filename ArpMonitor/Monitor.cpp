#include "Monitor.h"

#include "Command.h"
#include "Math.h"

#include <string>

Monitor::Monitor(int delay)
	: m_Delay(delay)
{
	std::cout << "Created monitor with delay: " << GetDelay() << std::endl;

	std::string ArpOutput = cmd::GetCommandOutput("arp -a");

	std::string target = "Type";

	std::size_t charPos = ArpOutput.find(target);

	if (charPos != std::string::npos)
		std::cout << "Found type at location: " << charPos << std::endl;

	std::size_t IPPos = charPos + target.length();

	//std::cout << "OUTPUT: " << ArpOutput[81] << std::endl;

	for (std::string::iterator it = ArpOutput.begin() + IPPos ; it != ArpOutput.end(); ++it) 
	{
		std::cout << *it;
	}

	// Iterator til end of string, lete etter 3 digits, så 2 digits, så 1 digit, store i a,b,c,d. Finne MAC address først non-blank etter int d.
	//while()




}

Monitor::~Monitor()
{
}


void Monitor::SetDelay(int delay)
{
	m_Delay = delay;
}

int Monitor::GetDelay()
{
	return m_Delay;
}
