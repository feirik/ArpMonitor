#include "Monitor.h"
#include "Command.h"
#include "Math.h"
#include "Log.h"

#define MAC_LENGTH 17
#define STATIC_WORDLEN 6
#define DYNAMIC_WORDLEN 7

Monitor::Monitor(int delay)
	: m_Delay(delay)
{
	std::string startMsg = GetCurrentTimeAsString() + " Started ArpMonitor successfully with delay " + 
						   std::to_string(GetDelay()) + " seconds...";

	std::cout << startMsg << std::endl;

	LogToFile(startMsg, LOG_PATH);

	std::string ArpOutput = cmd::GetCommandOutput("arp -a");

	PopulateArpInfo(&m_IPAddressArrayA, ArpOutput);

	IP::PrintIPAddressArray(m_IPAddressArrayA);

	bool switchFlag = 0;

	while (true)
	{
		math::Delay(GetDelay());

		std::string ArpOutput = cmd::GetCommandOutput("arp -a");

		if (switchFlag == 0)
		{
			PopulateArpInfo(&m_IPAddressArrayB, ArpOutput);

			CompareIPAddressArrays(&m_IPAddressArrayA, &m_IPAddressArrayB);

			LogArpEvents(m_IPAddressArrayA, m_IPAddressArrayB);

			switchFlag = 1;
		}

		math::Delay(GetDelay());

		if (switchFlag == 1)
		{
			PopulateArpInfo(&m_IPAddressArrayA, ArpOutput);

			CompareIPAddressArrays(&m_IPAddressArrayB, &m_IPAddressArrayA);

			LogArpEvents(m_IPAddressArrayB, m_IPAddressArrayA);

			switchFlag = 0;

		}

		math::Delay(GetDelay());
	}
}

Monitor::~Monitor()
{
	std::string endMsg = GetCurrentTimeAsString() + " Shut down ArpMonitor.";

	std::cout << endMsg << std::endl;

	LogToFile(endMsg, LOG_PATH);
}

void Monitor::SetDelay(int delay)
{
	m_Delay = delay;
}

int Monitor::GetDelay()
{
	return m_Delay;
}

void Monitor::PopulateArpInfo(std::vector<IPAddressInfo>* IPAddressArray, const std::string& ArpOutput)
{
	if (ArpOutput.substr(0, 7).find("No ARP") != std::string::npos)
	{
		std::string error = GetCurrentTimeAsString() + " Command line error, ARP output not available.";

		LogToFile(error, LOG_PATH);
		std::cout << error << std::endl;
	}
	else
	{
		// Find last word before IP addresses begin
		std::string target = "Type";

		std::size_t charPos = ArpOutput.find(target);

		std::size_t IPPos = charPos + target.length();

		int IPAddressRow = 0;

		IPAddressArray->clear();

		// Iterate over output starting at IP addresses
		for (auto it = ArpOutput.begin() + IPPos; it != ArpOutput.end(); ++it)
		{
			// Find start of IP address
			if (!isspace(*it))
			{
				IPAddressArray->emplace_back();

				for (int i = 0; i < 4; ++i)
				{
					int octet = IP::GetIPOctetAsInt(*it, *(it + 1), *(it + 2));

					if (i < 3)
					{
						switch (i)
						{
						case 0: IPAddressArray->at(IPAddressRow).a = octet; break;
						case 1: IPAddressArray->at(IPAddressRow).b = octet; break;
						case 2: IPAddressArray->at(IPAddressRow).c = octet; break;
						default:
							break;
						}

						it += IP::GetNumberOfOctetDigits(octet) + 1;
					}
					if (i == 3)
					{
						IPAddressArray->at(IPAddressRow).d = octet;
						it += IP::GetNumberOfOctetDigits(octet);
					}
				}

				// Find start of MAC address
				while (isspace(*it))
				{
					++it;
				}

				size_t itPos = it - ArpOutput.begin();

				// Store MAC address sub-string
				IPAddressArray->at(IPAddressRow).MACAddress = ArpOutput.substr(itPos, MAC_LENGTH);

				it += MAC_LENGTH;

				// Find start of dynamic/static description
				while (isspace(*it))
				{
					++it;
				}

				if (*it == 'd')
				{
					IPAddressArray->at(IPAddressRow).dynamic = true;
					it += DYNAMIC_WORDLEN;
				}
				else if (*it == 's')
				{
					IPAddressArray->at(IPAddressRow).dynamic = false;
					it += STATIC_WORDLEN;
				}

				++IPAddressRow;
			}
		}
	}
}

void Monitor::CompareIPAddressArrays(std::vector<IPAddressInfo>* Old, std::vector<IPAddressInfo>* New)
{
	size_t newSize = New->size();
	size_t oldSize = Old->size();

	// Reset flags from previous comparison
	for (size_t i = 0; i < newSize; ++i)
	{
		New->at(i).checked = false;
		New->at(i).newIP = false;
		New->at(i).newMAC = false;
		New->at(i).multiIP = false;
	}

	for (size_t j = 0; j < oldSize; ++j)
	{
		Old->at(j).checked = false;
		Old->at(j).newIP = false;
		Old->at(j).newMAC = false;
	}

	// Checking for duplicate MAC addresses, avoiding sorting implementation to keep output similar to arp -a
	for (size_t i = 0; i < newSize; ++i)
	{
		for (size_t k = 0; k < newSize; ++k)
		{
			// Different iterator value is checked, and duplicate has not been flagged yet
			if (i != k && New->at(k).multiIP == false)
			{
				if (New->at(i).MACAddress == New->at(k).MACAddress)
				{
					New->at(i).multiIP = true;
					New->at(k).multiIP = true;
				}
			}
		}
	}

	for (size_t i = 0; i < newSize; ++i)
	{
		for (size_t j = 0; j < oldSize; ++j)
		{
			// Check if new entry MAC address matches with old entry
			if (New->at(i).MACAddress == Old->at(j).MACAddress)
			{
				New->at(i).checked = true;
				Old->at(j).checked = true;

				// If MAC matches, but IP doesnt match
				if (IP::IsSameIP(Old->at(j), New->at(i)) == false)
				{
					// Check if MAC address has multiple IPs
					if (New->at(i).multiIP == true)
					{
						bool found = 0;

						// Check if the IP address is found in the old array
						for (size_t j = 0; j < oldSize; ++j)
						{
							// If IP address is not new as it was in the old array
							if (Old->at(j).multiIP == true && IP::IsSameIP(Old->at(j), New->at(i)))
							{
								found = 1;
							}
						}

						// If not found, must be a multi IP MAC address with a new IP
						if (!found)
						{
							New->at(i).newIP = true;
						}
					}
					// Set flag
					else
					{
						New->at(i).newIP = true;
					}
				}
			}
		}
	}

	// Checking if a new MAC address is using a previously stored IP address
	for (size_t j = 0; j < oldSize; ++j)
	{
		// If no MAC match was found in previous loop section
		if (Old->at(j).checked == false)
		{
			for (size_t i = 0; i < newSize; ++i)
			{
				// Check if unchecked old IP addresses matches with any new
				if (IP::IsSameIP(Old->at(j), New->at(i)))
				{
					// Would mean that an old IP address was reassigned to a new MAC
					New->at(i).newMAC = true;
					New->at(i).checked = true;

					Old->at(j).checked = true;
				}
			}
		}
	}

	for (size_t i = 0; i < newSize; ++i)
	{
		// If new array hasn't been checked by previous loops, must be a new entry of both MAC and IP
		if (New->at(i).checked == false)
		{
			New->at(i).newIP = true;
			New->at(i).newMAC = true;
		}
	}
}

void Monitor::LogArpEvents(const std::vector<IPAddressInfo>& Old, const std::vector<IPAddressInfo>& New)
{
	size_t newSize = New.size();
	size_t oldSize = Old.size();

	for (size_t j = 0; j < oldSize; ++j)
	{
		// If no match has been found when comparing old entry to new entry
		if (Old.at(j).checked == false)
		{
			// Multi-IP entry must have elapsed
			if (Old.at(j).multiIP == true)
			{
				std::string log = LogArpEvent("Multi-IP entry elapsed", Old.at(j));

				LogToFile(log, LOG_PATH);
				std::cout << log << std::endl;
			}
			// Regular entry must have eleapsed
			else
			{
				std::string log = LogArpEvent("ARP entry elapsed", Old.at(j));

				LogToFile(log, LOG_PATH);
				std::cout << log << std::endl;
			}
		}
	}

	for (size_t i = 0; i < newSize; ++i)
	{
		if (New.at(i).newIP == true && New.at(i).newMAC == true)
		{
			std::string log = LogArpEvent("New ARP entry", New.at(i));

			LogToFile(log, LOG_PATH);
			std::cout << log << std::endl;
		}
		else if (New.at(i).newIP == true || New.at(i).newMAC == true)
		{
			if (New.at(i).multiIP)
			{
				std::string log = LogArpEvent("New multi-IP ARP entry", New.at(i));

				LogToFile(log, LOG_PATH);
				std::cout << log << std::endl;
			}
			else if(New.at(i).newMAC == true)
			{
				std::string log = LogArpEvent("New MAC address broadcasting old IP", New.at(i));

				LogToFile(log, LOG_PATH);
				std::cout << log << std::endl;
			}
			else
			{
				std::string log = LogArpEvent("Old MAC address broadcasting new IP", New.at(i));

				LogToFile(log, LOG_PATH);
				std::cout << log << std::endl;
			}
		}
	}
}