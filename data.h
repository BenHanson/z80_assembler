#pragma once

#include "../lexertl14/include/lexertl/iterator.hpp"
#include "../parsertl14/include/parsertl/token.hpp"
#include "../parsertl14/include/parsertl/match_results.hpp"

struct data
{
	using memory = std::vector<uint8_t>;
	using token = parsertl::token<lexertl::citerator>;

	parsertl::state_machine _gsm;
	lexertl::state_machine _lsm;
	parsertl::state_machine _expr_gsm;
	lexertl::state_machine _expr_lsm;
	std::map<std::uint16_t, void (*)(data& data)> _actions;
	std::map<std::uint16_t, void (*)(data& data)> _expr_actions;
	parsertl::match_results _results;
	token::token_vector _productions;

	uint16_t _org = 23296;
	std::map<std::string, uint16_t> _label;
	std::map<std::string, int> _equ;
	// Relative jumps
	std::map<std::size_t, std::string> _rel_addr;
	// 8 bit expressions
	std::map<std::size_t, std::pair<std::string, char>> _byte_expr;
	// 16 bit expressions
	std::map<std::size_t, std::string> _word_expr;
	uint8_t _cc = ~0;
	uint8_t _c = ~0;
	uint8_t _dd = ~0;
	uint8_t _pp = ~0;
	uint8_t _qq = ~0;
	uint8_t _r = ~0;
	uint8_t _r2 = ~0;
	uint8_t _rr = ~0;
	uint16_t _integer = ~0;
	char _plus_minus = '+';
	std::stack<int> _acc;
	memory _memory;

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
	std::vector<block> _mem_type;

	token dollar(const std::size_t index);
	void push_byte();
	void push_byte(const uint8_t by);
	void push_word(const uint16_t w);
	void rel_label(const std::size_t idx);
	void bexpr(const int32_t idx);
	void bexpr(const int32_t idx, const char op);
	void wexpr(const int32_t idx);

	void parse(const char* first, const char* second);
	uint16_t parse_expr(const char* first, const char* second);

	void clear();
};
