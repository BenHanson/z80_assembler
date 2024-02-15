#pragma once

#include <stdexcept>

class z80_error : public std::runtime_error
{
public:
	z80_error(const std::string& msg) :
		std::runtime_error(msg)
	{
	}
};
