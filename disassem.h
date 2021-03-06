#pragma once

#include "data.h"

enum class base
{
	decimal,
	hexadecimal
};

enum class relative
{
	as_is,
	expand
};

void dump(const data& data, const base base, const relative relative);
std::string mnemonic(const data& data, const base base, const uint8_t*& end,
	const relative relative);
