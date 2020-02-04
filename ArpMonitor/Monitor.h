#pragma once

#include <iostream>
#include <vector>

struct IPAddressInfo
{
	int a, b, c, d; 
	std::string MACAddress;
	bool dynamic;
};

class Monitor
{
public:
	Monitor(int delay);
	~Monitor();

	void SetDelay(int delay);

	int GetDelay();

	int GetOctetIt();
	void IncrementOctetIt();

	int GetIPOctetAsInt(const char& it0, const char& it1, const char& it2);

private:
	int m_Delay;

	short m_OctetIt = 0;

	std::vector<IPAddressInfo> IPAddressArray;
};

