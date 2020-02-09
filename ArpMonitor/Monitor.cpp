#include "Monitor.h"

#include "Command.h"
#include "Math.h"

#include <string>
#include <cctype>

#define MAC_LENGTH 17
#define STATIC_WORDLEN 6
#define DYNAMIC_WORDLEN 7


Monitor::Monitor(int delay)
	: m_Delay(delay)
{
	std::cout << "Created monitor with delay: " << GetDelay() << std::endl;

	std::string ArpOutput = cmd::GetCommandOutput("arp -a");

	PopulateArpInfo(&m_IPAddressArrayA, ArpOutput);

	PrintIPAddressArray(m_IPAddressArrayA);

	bool switchFlag = 0;

	math::Delay(GetDelay());

	while (true)
	{
		std::string ArpOutput = cmd::GetCommandOutput("arp -a");

		if (switchFlag == 0)
		{
			PopulateArpInfo(&m_IPAddressArrayB, ArpOutput);

			CompareIPAddressArrays(&m_IPAddressArrayA, &m_IPAddressArrayB);

			/*std::cout << "\n\nPRINTING OLD: \n" << std::endl;
			PrintIPAddressArray(m_IPAddressArrayA);

			std::cout << "\n\nPRINTING NEW: \n" << std::endl;
			PrintIPAddressArray(m_IPAddressArrayB);*/

			switchFlag = 1;

		}

		math::Delay(GetDelay());

		if (switchFlag == 1)
		{
			PopulateArpInfo(&m_IPAddressArrayA, ArpOutput);

			CompareIPAddressArrays(&m_IPAddressArrayB, &m_IPAddressArrayA);

			/*std::cout << "\n\nPRINTING OLD: \n" << std::endl;
			PrintIPAddressArray(m_IPAddressArrayB);

			std::cout << "\n\nPRINTING NEW: \n" << std::endl;
			PrintIPAddressArray(m_IPAddressArrayA);*/

			switchFlag = 0;

		}

		math::Delay(GetDelay());
	}

	//Compare two IPAddressArrays, print/log out important changes

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

int Monitor::GetIPOctetAsInt(const char& it0, const char& it1, const char& it2)
{
	if (isdigit(it0) && isdigit(it1) && isdigit(it2))
	{
		// Converting 3 chars into int
		int result = (it0 - '0') * 100 + (it1 - '0') * 10 + (it2 - '0');

		return result;
	}
	// 2 digit check
	else if (isdigit(it0) && isdigit(it1))
	{
		// Converting 2 chars into int
		int result = (it0 - '0') * 10 + (it1 - '0');

		return result;
	}
	// 1 digit check
	else if (isdigit(it0))
	{
		// Converting char into int
		int result = (it0 - '0');

		return result;
	}

	return 0;
}

int Monitor::GetNumberOfOctetDigits(int octet)
{
	if (octet > 99)
	{
		return 3;
	}
	else if (octet > 9)
	{
		return 2;
	}
	else
	{
		return 1;
	}
}

void Monitor::PopulateArpInfo(std::vector<IPAddressInfo>* IPAddressArray, const std::string& ArpOutput)
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

		// Find start of IP address (192.168.86.1)
		if (!isspace(*it))
		{
			IPAddressArray->emplace_back();

			for (int i = 0; i < 4; ++i)
			{
				int octet = GetIPOctetAsInt(*it, *(it + 1), *(it + 2));

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

					it += GetNumberOfOctetDigits(octet) + 1;
				}
				if (i == 3)
				{
					IPAddressArray->at(IPAddressRow).d = octet;
					it += GetNumberOfOctetDigits(octet);
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

void Monitor::PrintIPAddressArray(const std::vector<IPAddressInfo>& Array)
{
	std::cout << "Printing IP address array: " << " Size: " << Array.size() << std::endl;

	for (size_t i = 0; i < Array.size(); ++i)
	{
		std::cout <<  Array.at(i).a << ":" << Array.at(i).b << ":" << Array.at(i).c << ":" << Array.at(i).d 
				  << " " << Array.at(i).MACAddress << " Dyn:" << Array.at(i).dynamic
				<< " Checked:" << Array.at(i).checked << " Elapsed:" << Array.at(i).elapsedEntry 
				<< " New IP: " << Array.at(i).newIP << " New MAC: " << Array.at(i).newMAC << std::endl;
	}
}

void Monitor::CompareIPAddressArrays(std::vector<IPAddressInfo>* Old, std::vector<IPAddressInfo>* New)
{
	// Reset flags from previous comparison
	for (size_t i = 0; i < New->size(); ++i)
	{
		New->at(i).checked = false;
		New->at(i).elapsedEntry = false;
		New->at(i).newIP = false;
		New->at(i).newMAC = false;
	}

	for (size_t j = 0; j < Old->size(); ++j)
	{
		Old->at(j).checked = false;
		Old->at(j).elapsedEntry = false;
		Old->at(j).newIP = false;
		Old->at(j).newMAC = false;
	}

	for (size_t i = 0; i < New->size(); ++i)
	{
		for (size_t j = 0; j < Old->size(); ++j)
		{
			// Check if new entry MAC address matches with old entry
			if (New->at(i).MACAddress == Old->at(j).MACAddress)
			{
				New->at(i).checked = true;
				Old->at(j).checked = true;

				// If MAC matches, but IP doesnt match, set flag
				if (New->at(i).a != Old->at(j).a || New->at(i).b != Old->at(j).b ||
					New->at(i).c != Old->at(j).c || New->at(i).d != Old->at(j).d)
				{
					New->at(i).newIP = true;
				}
			}
		}
	}

	// Checking if a new MAC address is using a previous IP address
	for (size_t j = 0; j < Old->size(); ++j)
	{
		// If no MAC match was found in loop section
		if (Old->at(j).checked == false)
		{
			for (size_t i = 0; i < New->size(); ++i)
			{
				// Check if unchecked old IP addresses matches with any new
				if (New->at(i).a == Old->at(j).a && New->at(i).b == Old->at(j).b &&
					New->at(i).c == Old->at(j).c && New->at(i).d == Old->at(j).d)
				{
					// Would mean that an old IP address was reassigned to a new MAC
					New->at(i).newMAC = true;
					New->at(i).checked = true;

					Old->at(j).checked = true;
				}
			}
		}
	}

	for (size_t i = 0; i < New->size(); ++i)
	{
		// If new array hasn't been checked by previous loops, must be a new entry of MAC and IP
		if (New->at(i).checked == false)
		{
			New->at(i).newIP = true;
			New->at(i).newMAC = true;
		}
	}
	
	for (size_t j = 0; j < Old->size(); ++j)
	{
		// No match has been found when comparing old entry to new entry
		if (Old->at(j).checked == false)
		{
			// Entry must have expired
			Old->at(j).elapsedEntry = true;
		}
	}

	for (size_t i = 0; i < New->size(); ++i)
	{
		if (New->at(i).newIP == true || New->at(i).newMAC == true)
		{
			std::cout << "New ARP entry: " << New->at(i).MACAddress << std::endl;
		}
	}

	/*std::cout << "\n\nPRINTING OLD: \n" << std::endl;
	PrintIPAddressArray(*Old);

	std::cout << "\n\nPRINTING NEW: \n" << std::endl;
	PrintIPAddressArray(*New);*/
}