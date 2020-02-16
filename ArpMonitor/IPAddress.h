#pragma once

#include <string>
#include <vector>
#include <iostream>

struct IPAddressInfo
{
	int a, b, c, d;
	std::string MACAddress;
	bool dynamic;

	bool newMAC, newIP, checked, multiIP;
};

namespace IP {

int GetIPOctetAsInt(const char& it0, const char& it1, const char& it2);

int GetNumberOfOctetDigits(int octet);

void PrintIPAddressArray(const std::vector<IPAddressInfo>& IPAddressArray);

bool IsSameIP(const IPAddressInfo& Old, const IPAddressInfo& New);

std::string GetIPAddressAsString(const IPAddressInfo& address);

} // Namespace

