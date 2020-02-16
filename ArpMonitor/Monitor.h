#pragma once

#include "IPAddress.h"

class Monitor
{
public:
	Monitor(int delay, bool logOnly);
	~Monitor();

	void SetDelay(int delay);

	int GetDelay();

	void PopulateArpInfo(std::vector<IPAddressInfo>* IPAddressArray, const std::string& ArpOutput);

	void CompareIPAddressArrays(std::vector<IPAddressInfo>* Old, std::vector<IPAddressInfo>* New);

	void LogArpEvents(const std::vector<IPAddressInfo>& Old, const std::vector<IPAddressInfo>& New);

	bool WriteToConsole();

	int GetVectorCapacity();

	void SetVectorCapacity(int capacity);

private:
	int m_Delay;

	bool m_writeToConsole;

	int m_vectorCapacity;

	std::vector<IPAddressInfo> m_IPAddressArrayA;
	std::vector<IPAddressInfo> m_IPAddressArrayB;
};

