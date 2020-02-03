#include "Math.h"

#include <cctype>

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
} // Namespace
