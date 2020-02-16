#define _CRT_SECURE_NO_WARNINGS

#include "Log.h"
#include "Command.h"

#include <chrono>
#include <ctime>
#include <algorithm>

/*
Takes no input
Returns a time stamp [Day Mon dd hh:mm:ss yyyy] as a string
*/
std::string GetCurrentTimeAsString()
{
	auto now = std::chrono::system_clock::now();

	std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

	std::string result = std::ctime(&currentTime);

	// Stripping new line '\n' char at end of string
	if (!result.empty() && result[result.length() - 1] == '\n')
	{
		result.erase(result.length() - 1);
	}

	return "[" + result + "]";
}

/*
Input of a string description and a const reference IPAddressInfo
Returns current time, the description, MAC address, IP address and network name as a string.
*/
std::string LogArpEvent(const std::string& description, const IPAddressInfo& entry)
{
	return GetCurrentTimeAsString() + " " + description + ": " + entry.MACAddress + " "
		 + IP::GetIPAddressAsString(entry) + " " + cmd::GetNetworkName(entry);
}

/*
Input of a string input and a string path
If file opens successfully, write input string to file.
Else output an error.
*/
void LogToFile(std::string input, std::string path)
{
	std::ofstream outFile;

	outFile.open(path, std::ios_base::app);

	if (outFile.is_open())
	{
		outFile << input << "\n";
	}
	else
	{
		std::cout << "Could not open output file " << path << "." << std::endl;
	}
}