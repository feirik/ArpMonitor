#include "Command.h"

#include <iostream>

#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>


namespace cmd {

std::string GetCommandOutput(const char* cmd)
{
	std::array<char, 128> buffer;
	std::string result;

	std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);

	if (!pipe)
	{
		throw std::runtime_error("_popen() failed");
	}
	while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
	{
		result += buffer.data();

		std::cout << buffer.data() << std::endl;
	}

	return result;
}

} // Namespace
