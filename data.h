#pragma once

#include "enums.h"
#include "../lexertl14/include/lexertl/iterator.hpp"
#include "../parsertl14/include/parsertl/token.hpp"
#include "../parsertl14/include/parsertl/match_results.hpp"

struct program
{
	using memory = std::vector<uint8_t>;
	struct block
	{
		enum class type { code, ds, db, dw };
		type _type;
		std::size_t _end = 0;

		block(const type type, const std::size_t end) :
			_type(type),
			_end(end)
		{
		}
	};

	uint16_t _org = 23296;
	bool _org_set = false;
	memory _memory;
	std::vector<block> _mem_type;

	void clear()
	{
		_org = 23296;
		_org_set = false;
		_memory.clear();
		_mem_type.clear();
	}
};

struct data
{
	using token = parsertl::token<lexertl::citerator>;

	parsertl::state_machine _gsm;
	lexertl::state_machine _lsm;
	parsertl::state_machine _expr_gsm;
	lexertl::state_machine _expr_lsm;
	std::map<std::uint16_t, void (*)(data& data)> _actions;
	std::map<std::uint16_t, void (*)(data& data)> _expr_actions;
	parsertl::match_results _results;
	token::token_vector _productions;

	std::map<std::string, uint16_t> _label;
	std::map<std::string, int> _equ;
	// Relative jumps
	std::map<std::size_t, std::string> _rel_addr;
	// 8 bit expressions
	std::map<std::size_t, std::pair<std::string, char>> _byte_expr;
	// 16 bit expressions
	std::map<std::size_t, std::string> _word_expr;
	uint8_t _cc = static_cast<uint8_t>(~0);
	uint8_t _c = static_cast<uint8_t>(~0);
	uint8_t _dd = static_cast<uint8_t>(~0);
	uint8_t _pp = static_cast<uint8_t>(~0);
	uint8_t _qq = static_cast<uint8_t>(~0);
	uint8_t _r = static_cast<uint8_t>(~0);
	uint8_t _r2 = static_cast<uint8_t>(~0);
	uint8_t _rr = static_cast<uint8_t>(~0);
	uint16_t _integer = static_cast<uint16_t>(~0);
	char _plus_minus = '+';
	std::stack<int> _acc;
	program _program;

	token dollar(const std::size_t index);
	void push_byte();
	void push_byte(const uint8_t by);
	void push_word(const uint16_t w);
	void rel_label(const std::size_t idx);
	void bexpr(const int32_t idx);
	void bexpr(const int32_t idx, const char op);
	void wexpr(const int32_t idx);

	void parse(const char* first, const char* second, const relative relative);
	uint16_t parse_expr(const char* first, const char* second);

	void clear();
};
