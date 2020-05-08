#pragma once

#include "IPAddress.h"

#include <string>

namespace cmd {

std::string GetCommandOutput(const char* cmd);

std::string GetNetworkName(const IPAddressInfo& address, const std::string& logPath);

} // Namespace



