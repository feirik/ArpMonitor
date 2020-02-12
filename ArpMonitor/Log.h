#pragma once

#include <string>
#include <iostream>

#include "IPAddress.h"

std::string GetCurrentTimeAsString();

std::string LogArpEvent(const std::string& description, const IPAddressInfo& entry);


