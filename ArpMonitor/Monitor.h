#pragma once

#include <iostream>
#include <vector>

struct IPAddressInfo
{
	int a, b, c, d; 
	std::string MACAddress;
	bool dynamic;

	bool newEntry, elapsedEntry;
};

class Monitor
{
public:
	Monitor(int delay);
	~Monitor();

	void SetDelay(int delay);

	int GetDelay();

	int GetIPOctetAsInt(const char& it0, const char& it1, const char& it2);

	int GetNumberOfOctetDigits(int octet);

	void PopulateArpInfo(std::vector<IPAddressInfo>* IPAddressArray, const std::string& ArpOutput);

	void PrintIPAddressArray(const std::vector<IPAddressInfo>& IPAddressArray);

private:
	int m_Delay;

	std::vector<IPAddressInfo> m_IPAddressArrayA;
	std::vector<IPAddressInfo> m_IPAddressArrayB;
};

