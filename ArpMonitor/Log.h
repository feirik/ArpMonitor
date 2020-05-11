#pragma once

#include "IPAddress.h"
#include "Oui.h"

#include <fstream>
#include <iostream>
#include <string>

#define LOG_DNS_ERROR 0

std::string GetCurrentTimeAsString();

std::string LogArpEvent(const std::string& description, const IPAddressInfo& entry, const bool passive, const std::string& logPath);

void LogToFile(std::string input, std::string path);

void LogInitialArpStatus(const std::vector<IPAddressInfo>& Array, const bool writeToConsole, const bool passive, const std::string& logPath);

std::string GetVendor(const std::string& MACaddress);


