#pragma once

#include <string>
#include <array>

struct oui
{
	std::string MACprefix;
	std::string vendor;

	// Comparison for use in binary search to find location of vendor in array
	struct less_than
	{
		bool operator()(const oui& a, const oui& b)
		{
			return a.MACprefix < b.MACprefix;
		}
	};
};

extern const std::array<oui, 27948> ouiArray;


