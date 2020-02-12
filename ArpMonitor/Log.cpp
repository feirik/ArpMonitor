#define _CRT_SECURE_NO_WARNINGS

#include "Log.h"

#include <chrono>
#include <ctime>
#include <algorithm>

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

std::string LogArpEvent(const std::string& description, const IPAddressInfo& entry)
{
	return GetCurrentTimeAsString() + " " + description + ": " + entry.MACAddress + " " + IP::GetIPAddressAsString(entry);
}

void LogToFile(std::string input, std::string path)
{
	std::ofstream outFile;

	outFile.open(path, std::ios_base::app);

	if (!outFile.is_open())
	{
		std::cout << "Could not open output file " << path << std::endl;
	}
	else
	{
		outFile << input << "\n";
	}
}