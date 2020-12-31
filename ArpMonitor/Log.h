#pragma once

#include "IPAddress.h"

#include <fstream>
#include <iostream>
#include <string>

#define LOG_DNS_ERROR 0

std::string GetCurrentTimeAsString();

std::string LogArpEvent(const std::string& description, const IPAddressInfo& entry, const bool passive, const std::string& logPath);

void LogToFile(const std::string& input, const std::string& path);

void LogInitialArpStatus(const std::vector<IPAddressInfo>& Array, const bool writeToConsole, const bool passive, const std::string& logPath);


