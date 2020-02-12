#include "IPAddress.h"

#include <cctype>

namespace IP {

int GetIPOctetAsInt(const char& it0, const char& it1, const char& it2)
{
	// 3 digit check
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

int GetNumberOfOctetDigits(int octet)
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

void PrintIPAddressArray(const std::vector<IPAddressInfo>& Array)
{
	std::cout << "Printing IP address array: " << " Size: " << Array.size() << std::endl;

	for (size_t i = 0; i < Array.size(); ++i)
	{
		std::cout << Array.at(i).a << "." << Array.at(i).b << "." << Array.at(i).c << "." << Array.at(i).d
			<< " " << Array.at(i).MACAddress << " Dyn:" << Array.at(i).dynamic
			<< " Checked:" << Array.at(i).checked
			<< " New IP: " << Array.at(i).newIP << " New MAC: " << Array.at(i).newMAC << std::endl;
	}
}

bool IsSameIP(const IPAddressInfo& Old, const IPAddressInfo& New)
{

	if (New.a == Old.a && New.b == Old.b && New.c == Old.c && New.d == Old.d)
	{
		return true;
	}
	else
	{
		return false;
	}
}

std::string GetIPAddressAsString(const IPAddressInfo& address)
{
	using namespace std;

	return to_string(address.a) + "." + to_string(address.b) + "." + to_string(address.c) + "." + to_string(address.d);
}

} // Namespace
