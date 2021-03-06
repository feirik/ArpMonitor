#pragma once

#include <iostream>
#include <string>
#include <vector>

struct IPAddressInfo
{
	int a, b, c, d;
	std::string MACAddress;
	bool dynamic;

	bool newMAC = false, newIP = false, checked = false, multiIP = false;
};

namespace IP {

int GetIPOctetAsInt(const char& it0, const char& it1, const char& it2);

int GetNumberOfOctetDigits(int octet);

bool IsSameIP(const IPAddressInfo& Old, const IPAddressInfo& New);

std::string GetIPAddressAsString(const IPAddressInfo& address);

} // Namespace

