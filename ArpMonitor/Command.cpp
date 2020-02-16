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
	std::string command = "nslookup " + IP::GetIPAddressAsString(address);
	std::string nslookupOutput = GetCommandOutput(command.c_str());

	std::string target = "Name:";
	std::size_t targetPos = nslookupOutput.find(target);
	std::size_t namePos = targetPos + target.length();

	// If 'Name:' is part of command output
	if (targetPos != std::string::npos)
	{
		for (auto it = nslookupOutput.begin() + namePos; it != nslookupOutput.end(); ++it)
		{
			// Find start of name description
			if (!isspace(*it))
			{
				int nameStart = it - nslookupOutput.begin();

				// Iterate while characters in description is not space
				while (!isspace(*it))
				{
					++it;
				}

				int nameLength = it - nslookupOutput.begin() - nameStart;

				// Return substring
				return nslookupOutput.substr(nameStart, nameLength);
			}
		}
		return "";
	}
	// 'Name:' is not found as part of command output, log DNS error and return unknown
	else
	{
		std::string log = GetCurrentTimeAsString() + " " + "Unable to resolve DNS name for: " + address.MACAddress + 
						  IP::GetIPAddressAsString(address);

		LogToFile(log, LOG_PATH);

		return "Unknown";
	}
}
} // Namespace
