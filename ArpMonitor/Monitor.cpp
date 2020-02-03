#include "Monitor.h"

#include "Command.h"

#include <string>

Monitor::Monitor(int delay)
	: m_Delay(delay)
{
	std::cout << "Created monitor with delay: " << GetDelay() << std::endl;

	std::string test = cmd::GetCommandOutput("arp -a");
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
