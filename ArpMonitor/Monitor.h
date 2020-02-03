#pragma once

#include <iostream>

class Monitor
{
public:
	Monitor();
	~Monitor();

	Monitor(int delay);

	void SetDelay(int delay);

	int GetDelay();

private:
	int m_Delay;
};

