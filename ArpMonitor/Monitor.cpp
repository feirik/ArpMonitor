#include "Monitor.h"

#include "Command.h"

#include <string>
#include <cctype>

#define MAC_LENGTH 17
#define STATIC_WORDLEN 6
#define DYNAMIC_WORDLEN 7

#define GET_VARIABLE_NAME(Variable) (#Variable)


Monitor::Monitor(int delay)
	: m_Delay(delay)
{
	std::cout << "Created monitor with delay: " << GetDelay() << std::endl;

	std::string ArpOutput = cmd::GetCommandOutput("arp -a");

	PopulateArpInfo(&m_IPAddressArrayA, ArpOutput);

	PrintIPAddressArray(m_IPAddressArrayA);

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

void Monitor::PrintIPAddressArray(const std::vector<IPAddressInfo>& IPAddressArray)
{
	std::cout << "Printing IP address array: " << std::endl;

	for (size_t i = 0; i < IPAddressArray.size(); ++i)
	{
		std::cout <<  IPAddressArray.at(i).a << ":" << IPAddressArray.at(i).b << ":" <<
					  IPAddressArray.at(i).c << ":" << IPAddressArray.at(i).d <<
			   " " << IPAddressArray.at(i).MACAddress << " Dyn:" << IPAddressArray.at(i).dynamic << std::endl;
	}
}