#pragma once

#include "data.h"

enum class base
{
	decimal,
	hex
};

void dump(const data& data, const base base);
