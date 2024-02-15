#pragma once

#include "data.hpp"
#include <lexertl/enums.hpp>

void build_parsers(data& data, const std::size_t flags = *lexertl::regex_flags::icase);
