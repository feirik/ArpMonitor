#pragma once

#include <string>
#include <iostream>
#include <fstream>

#include "IPAddress.h"

#define LOG_PATH "log.txt"

std::string GetCurrentTimeAsString();

std::string LogArpEvent(const std::string& description, const IPAddressInfo& entry);

void LogToFile(std::string input, std::string path);


