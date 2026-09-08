#pragma once

#include <stdexcept>
#include <string>

class z80_error : public std::runtime_error
{
public:
	using std::runtime_error::runtime_error;
};
