#include "IPAddress.h"

#include <cctype>

namespace IP {

/*
Input of 3 const char references
Checks to see how many consecutive chars of the inputs are digits.
Starts at three digit check due to the logic of checking NNN.NNN.NNN.NNN format of IP addresses from string input
Adds up the individual char numbers into an int based on digit position.
Returns the integer representing an IP address octet with range 1-256.
*/
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

/*
Input of 2 const reference IPAddressInfo entries
Checks the IP address octets of the entries to see if all matches
Returns true if match, false if no match 
*/
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

/*
Input of a const reference IPAddressInfo entry
Returns IP address of the entry in IP format as a string
*/
std::string GetIPAddressAsString(const IPAddressInfo& address)
{
	using namespace std;

	return to_string(address.a) + "." + to_string(address.b) + "." + to_string(address.c) + "." + to_string(address.d);
}
} // Namespace
