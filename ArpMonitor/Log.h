#pragma once

#include "IPAddress.h"

#include <fstream>
#include <iostream>
#include <string>

#define LOG_PATH "ArpMonitor.log"
#define LOG_DNS_ERROR 0

std::string GetCurrentTimeAsString();

std::string LogArpEvent(const std::string& description, const IPAddressInfo& entry);

void LogToFile(std::string input, std::string path);

void LogInitialArpStatus(const std::vector<IPAddressInfo>& Array, bool writeToConsole);


