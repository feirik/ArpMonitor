#pragma once

#include "IPAddress.h"

#define DEFAULT_DELAY_IN_SECONDS 5
#define DEFAULT_LOG_PATH "ArpMonitor.log"

struct userInput
{
	std::string interfaceIn = "";
	std::string logPath = DEFAULT_LOG_PATH;
	int delay = DEFAULT_DELAY_IN_SECONDS;
	bool logOnlyFlag = false;
	bool passiveFlag = false;
};

class Monitor
{
public:
	Monitor(userInput inputs);
	~Monitor();

	void SetDelay(int delay);
	int GetDelay() const;

	void PopulateArpInfo(std::vector<IPAddressInfo>* IPAddressArray, const std::string& ArpOutput);

	void CompareIPAddressArrays(std::vector<IPAddressInfo>* Old, std::vector<IPAddressInfo>* New);

	void LogArpEvents(const std::vector<IPAddressInfo>& Old, const std::vector<IPAddressInfo>& New);

	bool GetWriteToConsole() const;

	int GetVectorCapacity() const;
	void SetVectorCapacity(const int capacity);

	std::string GetLogPath() const;

	std::string GetInterface() const;
	std::string GetInterfaceInfo(const std::string& ArpOutput);

	bool GetPassiveFlag() const;

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

