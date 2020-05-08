#pragma once

#include "IPAddress.h"

#define DEFAULT_DELAY 5
#define DEFAULT_LOG_PATH "ArpMonitor.log"

struct userInput
{
	std::string interfaceIn = "";
	std::string logPath = DEFAULT_LOG_PATH;
	int delay = DEFAULT_DELAY;
	bool logOnlyFlag = false;
	bool passiveFlag = false;
};

class Monitor
{
public:
	//Monitor(int delay, bool logOnly);
	Monitor(userInput inputs);
	~Monitor();

	void SetDelay(int delay);
	int GetDelay();

	void PopulateArpInfo(std::vector<IPAddressInfo>* IPAddressArray, const std::string& ArpOutput);

	void CompareIPAddressArrays(std::vector<IPAddressInfo>* Old, std::vector<IPAddressInfo>* New);

	void LogArpEvents(const std::vector<IPAddressInfo>& Old, const std::vector<IPAddressInfo>& New);

	bool WriteToConsole();

	int GetVectorCapacity();
	void SetVectorCapacity(int capacity);

	std::string GetLogPath();

	std::string GetInterface();
	std::string GetInterfaceInfo(const std::string& ArpOutput);

	bool GetPassiveFlag();

private:
	int m_delay;
	int m_vectorCapacity;

	bool m_writeToConsole;
	bool m_passiveFlag;

	std::string m_interface;
	std::string m_logPath;

	std::vector<IPAddressInfo> m_IPAddressArrayA;
	std::vector<IPAddressInfo> m_IPAddressArrayB;
};

