#pragma once

#include <string>
#include <iostream>
#include <fstream>

#include "IPAddress.h"

#define LOG_PATH "ArpMonitor.log"

std::string GetCurrentTimeAsString();

std::string LogArpEvent(const std::string& description, const IPAddressInfo& entry);

void LogToFile(std::string input, std::string path);

void LogInitialArpStatus(const std::vector<IPAddressInfo>& Array, bool writeToConsole);


