#define _CRT_SECURE_NO_WARNINGS

#include "Command.h"
#include "Log.h"
#include "Oui.h"

#include <algorithm>
#include <chrono>
#include <ctime>
#include <string>

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
std::string LogArpEvent(const std::string& description, const IPAddressInfo& entry, const bool passive, const std::string& logPath)
{
	std::string returnStr;

	// Include DNS lookup check if passive flag is not set
	if (!passive)
	{
		returnStr = GetCurrentTimeAsString() + " " + description + ": " + entry.MACAddress + " " +
					oui::GetVendor(entry.MACAddress) + " " + IP::GetIPAddressAsString(entry) + " " +
					cmd::GetNetworkName(entry, logPath);
	}
	else
	{
		returnStr = GetCurrentTimeAsString() + " " + description + ": " + entry.MACAddress + 
					oui::GetVendor(entry.MACAddress) + " " +
					IP::GetIPAddressAsString(entry);
	}

	return returnStr;
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
/*
Takes a const reference vector of IPAddressInfo and bool write to console flag as input
Logs the IP address, MAC address and domain name for the IPAddressInfo entries in the array
Prints the same info if flag is set to true
No return
*/
void LogInitialArpStatus(const std::vector<IPAddressInfo>& Array, const bool writeToConsole, const bool passive, const std::string& logPath)
{
	for (size_t i = 0; i < Array.size(); ++i)
	{
		std::string log = LogArpEvent("ARP entry at startup", Array.at(i), passive, logPath);

		if (writeToConsole == true)
		{
			std::cout << log << std::endl;
		}
		LogToFile(log, logPath);
	}
}