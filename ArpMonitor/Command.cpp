#include "Command.h"
#include "Log.h"

#include <iostream>

#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>


namespace cmd {

/*
Input of a char pointer, command to be executing in windows command line.
Opens a pipe to the command line output and returns the buffer as a string.
*/
std::string GetCommandOutput(const char* cmd)
{
	std::array<char, 64> buffer;
	std::string result;

	std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);

	if (!pipe)
	{
		throw std::runtime_error("_popen() failed");
	}
	while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
	{
		result += buffer.data();
	}

	return result;
}

/*
Input of an IPAddressInfo const reference
Executes a nslookup command with the provided IP address.
Output from the command is checked for a DNS name. The name is returned as a string if found.
If a name is not found, 'Unknown' is returned as a string and an error is logged to file.
*/
std::string GetNetworkName(const IPAddressInfo& address)
{
	// Running script with a filter for DNS name to avoid cerr output of default nslookup
	std::string command = "DNSname.bat " + IP::GetIPAddressAsString(address);
	std::string DNSname = GetCommandOutput(command.c_str());

	// Stripping new line '\n' char at end of string
	if (!DNSname.empty() && DNSname[DNSname.length() - 1] == '\n')
	{
		DNSname.erase(DNSname.length() - 1);
	}

	// DNS name not found in script, log error
	if (DNSname == "Unknown")
	{
		if (LOG_DNS_ERROR == true)
		{
			std::string log = GetCurrentTimeAsString() + " " + "Unable to resolve DNS name for: " + address.MACAddress + " " +
				IP::GetIPAddressAsString(address);

			LogToFile(log, LOG_PATH);
		}

		return DNSname;
	}
	// DNS name found in script
	else
	{
		return DNSname;
	}
}
} // Namespace
