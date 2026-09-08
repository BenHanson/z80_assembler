#pragma once

#include "data.hpp"

#include <string>

void read_sna(std::string pathname, const char* first,
	const char* second, data& data);
