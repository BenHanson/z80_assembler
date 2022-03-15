#pragma once

#include "data.h"
#include "enums.h"

void dump(const program& program, const base base, const relative relative);
std::string mnemonic(const program& program, const base base, const uint8_t*& end,
	const relative relative);
