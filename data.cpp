#include <algorithm>
#include "data.hpp"
#include <format>
#include <parsertl/lookup.hpp>
#include <stdexcept>
#include <sstream>
#include <string>
#include "z80_error.hpp"

data::token data::dollar(const std::size_t index) const
{
	return _results.dollar(index, _gsm, _productions);
}

void data::push_byte()
{
	if (_integer > 255)
		throw std::out_of_range(std::format("Value {} is greater than 255", _integer));

	_program._memory.push_back(_integer & 0xff);
}

void data::push_byte(const uint8_t by)
{
	_program._memory.push_back(by);
}

void data::push_word(const uint16_t w)
{
	_program._memory.push_back(w & 0xff);
	_program._memory.push_back(w >> 8);
}

void data::rel_label(const std::size_t idx)
{
	const std::string name = dollar(idx).str();

	_rel_addr[_program._memory.size() - 1] = name;
}

void data::bexpr(const int32_t idx)
{
	const std::string name = dollar(idx).str();

	_byte_expr[_program._memory.size() - 1] = std::make_pair(name, '+');
}

void data::bexpr(const int32_t idx, const char op)
{
	const std::string name = dollar(idx).str();

	_byte_expr[_program._memory.size() - 1] = std::make_pair(name, op);
}

void data::wexpr(const int32_t idx)
{
	const std::string name = dollar(idx).str();

	_word_expr[_program._memory.size() - 2] = name;
}

void data::parse(const char* first, const char* second, const relative relative)
{
	lexertl::citerator iter(first, second, _lsm);

	// In case you are parsing one line at a time
	_productions.clear();
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
					throw z80_error(std::format("{} at line {}",
						e.what(),
						std::count(first, iter->first, '\n') + 1));
				}
			}
		}

		parsertl::lookup(iter, _gsm, _results, _productions);
	}

	if (_results.entry.action == parsertl::action::error)
	{
		auto start = std::reverse_iterator<const char*>(iter->first);
		auto bol = std::find(start, std::reverse_iterator<const char*>(first), '\n');
		auto eol = std::find(iter->first, iter->eoi, '\n');
		std::string cmd(std::to_address(bol) + 1, eol[-1] == '\r' ? eol - 1 : eol);
		std::size_t count = 0;

		for (auto i = std::to_address(bol) + 1, e = iter->first; i != e; ++i)
			if (*i == '\t')
				count += 8 - count % 8;
			else
				++count;

		throw z80_error(std::format("Parser error, line {}\n"
			"{}\n{}^",
			std::count(first, iter->first, '\n') + 1,
			cmd,
			std::string(count, ' ')));
	}
	else
	{
		for (const auto& [address, expr] : _rel_addr)
		{
			int off = 0;

			if (relative == relative::absolute)
			{
				const uint16_t val = parse_expr(expr.c_str(),
					expr.c_str() + expr.size());

				off = static_cast<int>(val - (_program._org + address + 1));
			}
			else
			{
				off = static_cast<int8_t>(parse_expr(expr.c_str(),
					expr.c_str() + expr.size()));
			}

			if (off < -128 || off > 127)
			{
				throw z80_error(std::format("Out of range relative call to '{}'", expr));
			}

			_program._memory[address] = static_cast<uint8_t>(off);
		}

		for (const auto& [address, pair] : _byte_expr)
		{
			int16_t val = parse_expr(pair.first.c_str(),
				pair.first.c_str() + pair.first.size());

			if (pair.second == '-')
				val *= -1;

			_program._memory[address] = val & 0xff;
		}

		for (const auto& [address, expr] : _word_expr)
		{
			const uint16_t val = parse_expr(expr.c_str(),
				expr.c_str() + expr.size());

			_program._memory[address] = val & 0xff;
			_program._memory[address + 1] = static_cast<uint8_t>(val >> 8);
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

		parsertl::lookup(iter, _expr_gsm, _results, _productions);
	}

	std::swap(results, _results);
	std::swap(productions, _productions);
	ret = static_cast<uint16_t>(_acc.top());
	_acc.pop();
	return ret;
}

void data::clear()
{
	_program.clear();
	_label.clear();
	_equ.clear();
	_rel_addr.clear();
	_byte_expr.clear();
	_word_expr.clear();
	_cc = static_cast<uint8_t>(~0);
	_c = static_cast<uint8_t>(~0);
	_dd = static_cast<uint8_t>(~0);
	_pp = static_cast<uint8_t>(~0);
	_qq = static_cast<uint8_t>(~0);
	_r = static_cast<uint8_t>(~0);
	_r2 = static_cast<uint8_t>(~0);
	_rr = static_cast<uint8_t>(~0);
	_integer = static_cast<uint16_t>(~0);
	_plus_minus = '+';

	while (!_acc.empty())
		_acc.pop();
}
