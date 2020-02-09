#include "Math.h"

#include <cctype>
#include <chrono>

#include <iostream>

namespace math {

bool IsInteger(const std::string& input)
{
	std::string::const_iterator it = input.begin();

	// Interating over all positions in string, only progressing if position is an integer
	while (it != input.end() && std::isdigit(*it))
	{
		++it;
	}

	// If iterator made it til the end and string is not empty, return true
	return !input.empty() && it == input.end();
}

void Delay(int delayInSeconds)
{
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	while (std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() < delayInSeconds)
	{
		end = std::chrono::steady_clock::now();
	}
}
} // Namespace
