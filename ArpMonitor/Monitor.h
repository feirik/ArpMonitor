#pragma once

#include <iostream>
#include <vector>

struct IPAddressInfo
{
	int a, b, c, d; 
	std::string macAddress;
};

class Monitor
{
public:
	Monitor(int delay);
	~Monitor();

	void SetDelay(int delay);

	int GetDelay();

private:
	int m_Delay;



	std::vector<std::vector<IPAddressInfo>> IPAddressArray;
};

