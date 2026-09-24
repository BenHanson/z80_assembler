#pragma once

#include "data.hpp"
#include "enums.hpp"

#include <cstdint>
#include <string>

void dump(const program& program, const base base, const relative relative,
	const dump_mode mode = dump_mode::full);

std::string mnemonic(const program& program, const base base, const uint8_t*& end,
	const relative relative);
