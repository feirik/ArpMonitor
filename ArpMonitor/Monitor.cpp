#include "Monitor.h"
#include "Command.h"
#include "Math.h"
#include "Log.h"

#define MAC_LENGTH 17
#define STATIC_WORDLEN 6
#define DYNAMIC_WORDLEN 7
#define START_CAPACITY 5

Monitor::Monitor(userInput inputs)
	: m_delay(inputs.delay), m_writeToConsole(!inputs.logOnlyFlag), m_vectorCapacity(START_CAPACITY),
	  m_interface(inputs.interfaceIn), m_passiveFlag(inputs.passiveFlag)
{
	// Initializing vector capacity to avoid unnecessary vector copies
	m_IPAddressArrayA.reserve(GetVectorCapacity());
	m_IPAddressArrayB.reserve(GetVectorCapacity());

	std::string startMsg = GetCurrentTimeAsString() + " Started ArpMonitor successfully with delay " + 
						   std::to_string(GetDelay()) + " seconds...";

	if (WriteToConsole() == true)
	{
		std::cout << startMsg << std::endl;
	}

	LogToFile(startMsg, LOG_PATH);

	// Getting initidal arp -a output to create a comparsion entry
	std::string ArpOutput = cmd::GetCommandOutput("arp -a");

	PrintSelectedInterface(ArpOutput);

	PopulateArpInfo(&m_IPAddressArrayA, ArpOutput);

	LogInitialArpStatus(m_IPAddressArrayA, WriteToConsole(), GetPassiveFlag());

	// Main loop, A/B swaps for comparing the arrays
	while (true)
	{
		math::Delay(GetDelay());

		// Updating B array
		std::string ArpOutput = cmd::GetCommandOutput("arp -a");

		PopulateArpInfo(&m_IPAddressArrayB, ArpOutput);

		CompareIPAddressArrays(&m_IPAddressArrayA, &m_IPAddressArrayB);

		LogArpEvents(m_IPAddressArrayA, m_IPAddressArrayB);

		math::Delay(GetDelay());

		// Updating A array
		ArpOutput = cmd::GetCommandOutput("arp -a");

		PopulateArpInfo(&m_IPAddressArrayA, ArpOutput);

		CompareIPAddressArrays(&m_IPAddressArrayB, &m_IPAddressArrayA);

		LogArpEvents(m_IPAddressArrayB, m_IPAddressArrayA);
	}
}

Monitor::~Monitor()
{
	std::string endMsg = GetCurrentTimeAsString() + " Shut down ArpMonitor.";

	if (WriteToConsole() == true)
	{
		std::cout << endMsg << std::endl;
	}

	LogToFile(endMsg, LOG_PATH);
}

void Monitor::SetDelay(int delay)
{
	m_delay = delay;
}

int Monitor::GetDelay()
{
	return m_delay;
}

/*
Input of a vector of IPAddressArray pointer, const reference arp output string
Checks to see if arp output resulted in an error, if error, logs and prints error info.
If no error, populates IP address, MAC address and static/dynamic info into array until end of output.
No output from function
*/
void Monitor::PopulateArpInfo(std::vector<IPAddressInfo>* IPAddressArray, const std::string& ArpOutput)
{
	if (ArpOutput.substr(0, 7).find("No ARP") != std::string::npos)
	{
		std::string error = GetCurrentTimeAsString() + " Command line error, ARP output not available.";

		LogToFile(error, LOG_PATH);

		if (WriteToConsole() == true)
		{
			std::cout << error << std::endl;
		}
	}
	else
	{
		std::size_t charPos;
		std::string target;
		std::size_t IPPos;

		// Find start of selected interface, if provided
		if (m_interface != "")
		{
			// Find start of selected interface in arp -a output
			target = "Interface: " + GetInterface();
			charPos = ArpOutput.find(target);
			IPPos = charPos + target.length();

			// Iterate until 'Type' is found
			for (auto it = ArpOutput.begin() + IPPos; it != ArpOutput.end(); ++it)
			{
				if (*it == 'T' && *(it + 1) == 'y' && *(it + 2) == 'p')
				{
					// Skip chars before IP addresses begin
					IPPos += 4;
					break;
				}
				++IPPos;
			}
		}
		else
		{
			// Find last word before IP addresses begin
			target = "Type";
			charPos = ArpOutput.find(target);
			IPPos = charPos + target.length();
		}

		//int IPAddressRow = 0;

		IPAddressArray->clear();

		// Iterate over output starting at IP addresses
		for (auto it = ArpOutput.begin() + IPPos; it != ArpOutput.end(); ++it)
		{
			// Find start of IP address
			if (!isspace(*it))
			{
				// If 'Interface' is found after finding 'Type', stop populating as output must show a new adapter
				if (*it == 'I' && *(it+1) == 'n' && *(it + 2) == 't')
				{
					break;
				}

				// Define new row entry
				IPAddressInfo newEntry;

				// Populating the 4 IP octets
				for (int i = 0; i < 4; ++i)
				{
					int octet = IP::GetIPOctetAsInt(*it, *(it + 1), *(it + 2));

					if (i < 3)
					{
						switch (i)
						{
						case 0: newEntry.a = octet; break;
						case 1: newEntry.b = octet; break;
						case 2: newEntry.c = octet; break;
						default:
							break;
						}

						it += IP::GetNumberOfOctetDigits(octet) + 1;
					}
					if (i == 3)
					{
						newEntry.d = octet;
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
				newEntry.MACAddress = ArpOutput.substr(itPos, MAC_LENGTH);

				it += MAC_LENGTH;

				// Find start of dynamic/static description
				while (isspace(*it))
				{
					++it;
				}

				if (*it == 'd')
				{
					newEntry.dynamic = true;
					it += DYNAMIC_WORDLEN;
				}
				else if (*it == 's')
				{
					newEntry.dynamic = false;
					it += STATIC_WORDLEN;
				}

				// Construct new entry at the back of array
				try
				{
					IPAddressArray->emplace_back(newEntry);
				}
				// Output error if memory allocation fails
				catch (const std::bad_alloc&)
				{
					std::string log = GetCurrentTimeAsString() + "FATAL ERROR. Unable to allocate enough memory.";

					if (WriteToConsole() == true)
					{
						std::cout << log << std::endl;
					}
					LogToFile(log, LOG_PATH);
				}
			}
		}
	}
}

/*
Input of two vectors of IPAddressInfo array pointers
Resets flags from previous comparisons.
Sets flags based on comparison logic between the old and new array describing changes to arp output.
No output
*/
void Monitor::CompareIPAddressArrays(std::vector<IPAddressInfo>* Old, std::vector<IPAddressInfo>* New)
{
	size_t newSize = New->size();
	size_t oldSize = Old->size();

	// Reset flags from previous comparison
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
			// If duplicate flag has not been set yet
			if (i != k && New->at(k).multiIP == false)
			{
				// Flag both entries
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
		// If new array hasn't been checked in previous loops, must be a new entry of both MAC and IP
		if (New->at(i).checked == false)
		{
			New->at(i).newIP = true;
			New->at(i).newMAC = true;
		}
	}
}

/*
Input of two const reference vectors of IPAddressInfo
Logs and outputs text based on flags in vectors-
Old vector flags describe elapsed changes, new vector flags describe new entries.
No output from function
*/
void Monitor::LogArpEvents(const std::vector<IPAddressInfo>& Old, const std::vector<IPAddressInfo>& New)
{
	size_t newSize = New.size();
	size_t oldSize = Old.size();

	// Checking elapsed entries
	for (size_t j = 0; j < oldSize; ++j)
	{
		// If no match has been found when comparing old entry to new entry
		if (Old.at(j).checked == false)
		{
			// Multi-IP entry must have elapsed
			if (Old.at(j).multiIP == true)
			{
				std::string log = LogArpEvent("Multi-IP entry elapsed", Old.at(j), GetPassiveFlag());

				LogToFile(log, LOG_PATH);

				if (WriteToConsole() == true)
				{
					std::cout << log << std::endl;
				}
			}
			// Regular entry must have elapsed
			else
			{
				std::string log = LogArpEvent("ARP entry elapsed", Old.at(j), GetPassiveFlag());

				LogToFile(log, LOG_PATH);

				if (WriteToConsole() == true)
				{
					std::cout << log << std::endl;
				}
			}
		}
	}

	// Checking new entries
	for (size_t i = 0; i < newSize; ++i)
	{
		// If new IP and MAC address is found, must be a new entry
		if (New.at(i).newIP == true && New.at(i).newMAC == true)
		{
			std::string log = LogArpEvent("New ARP entry", New.at(i), GetPassiveFlag());

			LogToFile(log, LOG_PATH);

			if (WriteToConsole() == true)
			{
				std::cout << log << std::endl;
			}
		}
		else if (New.at(i).newIP == true || New.at(i).newMAC == true)
		{
			if (New.at(i).multiIP)
			{
				std::string log = LogArpEvent("New multi-IP ARP entry", New.at(i), GetPassiveFlag());

				LogToFile(log, LOG_PATH);

				if (WriteToConsole() == true)
				{
					std::cout << log << std::endl;
				}
			}
			else if(New.at(i).newMAC == true)
			{
				std::string log = LogArpEvent("New MAC address broadcasting old IP", New.at(i), GetPassiveFlag());

				LogToFile(log, LOG_PATH);

				if (WriteToConsole() == true)
				{
					std::cout << log << std::endl;
				}
			}
			else
			{
				std::string log = LogArpEvent("Old MAC address broadcasting new IP", New.at(i), GetPassiveFlag());

				LogToFile(log, LOG_PATH);

				if (WriteToConsole() == true)
				{
					std::cout << log << std::endl;
				}
			}
		}
	}
}

bool Monitor::WriteToConsole()
{
	return m_writeToConsole;
}

int Monitor::GetVectorCapacity()
{
	return m_vectorCapacity;
}

void Monitor::SetVectorCapacity(int capacity)
{
	m_vectorCapacity = capacity;
}

std::string Monitor::GetInterface()
{
	return m_interface;
}

bool Monitor::GetPassiveFlag()
{
	return m_passiveFlag;
}

/*
Prints the selected interface if provided as command line argument
Else prints the first interface in the ARP output
*/
void Monitor::PrintSelectedInterface(const std::string& ArpOutput)
{
	if (GetInterface() != "")
	{
		std::cout << GetCurrentTimeAsString() << " Monitoring interface " << GetInterface() << std::endl;
	}
	else
	{
		// Search for interface
		std::string target = "Interface: ";
		std::size_t charPos = ArpOutput.find(target);
		std::size_t ItPos = charPos + target.length();

		std::size_t interfaceLen = 0;

		// Find length of interface IP address
		for (auto it = ArpOutput.begin() + ItPos; it != ArpOutput.end(); ++it)
		{
			if (isspace(*it))
			{
				break;
			}
			++interfaceLen;
		}

		std::string defaultInterface = ArpOutput.substr(ItPos, interfaceLen);

		std::cout << GetCurrentTimeAsString() << " Monitoring interface " << defaultInterface << std::endl;
	}	
}