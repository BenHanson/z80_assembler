#include <algorithm>
#include "data.h"
#include "../parsertl14/include/parsertl/lookup.hpp"
#include <stdexcept>
#include <sstream>
#include <string>

data::token data::dollar(const std::size_t index)
{
	return _results.dollar(_gsm, index, _productions);
}

void data::push_byte()
{
	if (_integer > 255)
		throw std::out_of_range("Value " + std::to_string(_integer) + " is greater than 255");

	_memory.push_back(_integer & 0xff);
}

void data::push_byte(const uint8_t by)
{
	_memory.push_back(by);
}

void data::push_word(const uint16_t w)
{
	_memory.push_back(w & 0xff);
	_memory.push_back(w >> 8);
}

void data::rel_label(const std::size_t idx)
{
	const std::string name = dollar(idx).str();
	auto iter = _label.find(name);

	_rel_addr[_memory.size() - 1] = name;
}

void data::bexpr(const int32_t idx)
{
	const std::string name = dollar(idx).str();

	_byte_expr[_memory.size() - 1] = name;
}

void data::wexpr(const int32_t idx)
{
	const std::string name = dollar(idx).str();

	_word_expr[_memory.size() - 2] = name;
}

void data::parse(const char* first, const char* second)
{
	lexertl::citerator iter(first, second, _lsm);

	_results.reset(iter->id, _gsm);

	while (_results.entry.action != parsertl::action::error &&
		_results.entry.action != parsertl::action::accept)
	{
		if (_results.entry.action == parsertl::action::reduce)
		{
			auto i = _actions.find(_results.entry.param);

			if (i != _actions.end())
			{
				try
				{
					i->second(*this);
				}
				catch (const std::exception& e)
				{
					std::ostringstream ss;

					ss << e.what() << " at line " <<
						std::count(first, iter->first, '\n') + 1;
					throw std::runtime_error(ss.str());
				}
			}
		}

		parsertl::lookup(_gsm, iter, _results, _productions);
	}

	if (_results.entry.action == parsertl::action::error)
	{
		auto start = std::reverse_iterator<const char*>(iter->first);
		auto bol = std::find(start, std::reverse_iterator<const char*>(first), '\n');
		auto eol = std::find(iter->first, iter->eoi, '\n');
		std::string cmd((&*bol) + 1, eol[-1] == '\r' ? eol - 1 : eol);
		std::ostringstream ss;
		std::size_t count = 0;

		for (auto i = (&*bol) + 1, s = i, e = iter->first; i != e; ++i)
			if (*i == '\t')
				count += 8 - count % 8;
			else
				++count;

		ss << "Parser error, line " <<
			std::count(first, iter->first, '\n') + 1 << '\n' <<
			cmd << '\n' << std::string(count, ' ') << '^';
		throw std::runtime_error(ss.str());
	}
	else
	{
		for (const auto& pair : _rel_addr)
		{
			const uint16_t val = parse_expr(pair.second.c_str(),
				pair.second.c_str() + pair.second.size());
			const int off = static_cast<int>(val - (_org + pair.first + 1));

			if (off < -128 || off > 127)
				throw std::runtime_error("Out of range relative call to '" +
					pair.first + '\'');

			_memory[pair.first] = static_cast<uint8_t>(off);
		}

		for (const auto& pair : _byte_expr)
		{
			const uint16_t val = parse_expr(pair.second.c_str(),
				pair.second.c_str() + pair.second.size());

			if (val > 255)
				throw std::runtime_error(pair.second + " gives a value > 255");

			_memory[pair.first] = val & 0xff;
		}

		for (const auto& pair : _word_expr)
		{
			const uint16_t val = parse_expr(pair.second.c_str(),
				pair.second.c_str() + pair.second.size());

			_memory[pair.first] = val & 0xff;
			_memory[pair.first + 1] = static_cast<uint8_t>(val >> 8);
		}
	}
}

uint16_t data::parse_expr(const char* first, const char* second)
{
	uint16_t ret = 0;
	lexertl::citerator iter(first, second, _expr_lsm);
	parsertl::match_results results;
	token::token_vector productions;

	std::swap(results, _results);
	std::swap(productions, _productions);
	_results.reset(iter->id, _expr_gsm);
	_productions.clear();

	while (_results.entry.action != parsertl::action::error &&
		_results.entry.action != parsertl::action::accept)
	{
		if (_results.entry.action == parsertl::action::reduce)
		{
			auto i = _expr_actions.find(_results.entry.param);

			if (i != _expr_actions.end())
			{
				i->second(*this);
			}
		}

		parsertl::lookup(_expr_gsm, iter, _results, _productions);
	}

	std::swap(results, _results);
	std::swap(productions, _productions);
	ret = _acc.top();
	_acc.pop();
	return ret;
}
