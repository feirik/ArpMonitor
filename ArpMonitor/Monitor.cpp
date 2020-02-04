#include "Monitor.h"

#include "Command.h"

#include <string>
#include <cctype>

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

	for (auto it = ArpOutput.begin() + IPPos ; it != ArpOutput.end(); ++it) 
	{
		int i = 0;

		// Find start of IP address
		if (!isspace(*it))
		{
			IPAddressArray.emplace_back();

			int octet = GetIPOctetAsInt(*it, *(it + 1), *(it + 2));

			//FOR LOOP WITH 4 LOOPS ISTEDENFOR OCTET ITERATOR??????
			switch (GetOctetIt())
			{
				case 0: IPAddressArray.at(i).a = octet; break;
				case 1: IPAddressArray.at(i).b = octet; break;
				case 2: IPAddressArray.at(i).c = octet; break;
				case 3: IPAddressArray.at(i).b = octet; break;
			default:
				break;
			}



			std::cout << "OCTET TEST:" << IPAddressArray.at(i).a << std::endl;

			break;

			//Enumeration to check if octet is to be saved in a, b, c or d of vector?


			/*IPAddressArray.emplace_back();

			// Check if first part of IP address consists of 3 digits
			if (isdigit(*it) && isdigit(*(it + 1)) && isdigit(*(it + 2)))
			{
				// Converting chars into int
				int result = (*it - '0')*100 + (*(it + 1) - '0')*10 + (*(it + 2) - '0');

				std::cout << "CALC: " << result << std::endl;

				IPAddressArray.at(0).a = result;

				std::cout << "ARRAY: " << IPAddressArray.at(0).a << std::endl;


				it += 4;
			}
			// 2 digit check
			else if (isdigit(*it) && isdigit(*(it + 1)))
			{
				// Converting chars into int
				int result = (*it - '0') * 10 + (*(it + 1) - '0');

				std::cout << "CALC: " << result << std::endl;

				IPAddressArray.at(0).a = result;

				std::cout << "ARRAY: " << IPAddressArray.at(0).a << std::endl;


				it += 3;
			}
			// 1 digit check
			else if (isdigit(*it))
			{
				// Converting chars into int
				int result = (*it - '0');

				std::cout << "CALC: " << result << std::endl;

				IPAddressArray.at(0).a = result;

				std::cout << "ARRAY: " << IPAddressArray.at(0).a << std::endl;


				it += 2;
			}*/

			std::cout << *it << std::endl;
			std::cout << *(it+1) << std::endl;
			break;
		}

		//std::cout << *it;
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

int Monitor::GetIPOctetAsInt(const char& it0, const char& it1, const char& it2)
{
	if (isdigit(it0) && isdigit(it1) && isdigit(it2))
	{
		// Converting chars into int
		int result = (it0 - '0') * 100 + (it1 - '0') * 10 + (it2 - '0');

		return result;
	}
	// 2 digit check
	else if (isdigit(it0) && isdigit(it1))
	{
		// Converting chars into int
		int result = (it0 - '0') * 10 + (it1 - '0');

		return result;
	}
	// 1 digit check
	else if (isdigit(it0))
	{
		// Converting chars into int
		int result = (it0 - '0');

		return result;
	}

	return 0;
}

void Monitor::IncrementOctetIt()
{
	++m_OctetIt;

	if (m_OctetIt > 3)
	{
		m_OctetIt = 0;
	}
}

int Monitor::GetOctetIt()
{
	return m_OctetIt;
}
