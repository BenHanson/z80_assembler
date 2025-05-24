#include <algorithm>
#include <charconv>
#include <lexertl/generator.hpp>
#include "skool.hpp"

void parse_skool(const char* first, const char* second, data& data)
{
	// Enabled the lexertl '*' enum class operator
	using namespace lexertl;
	enum class token { decimal = 1, hex };
	rules rules;
	state_machine sm;

	rules.push(R"(\d{5})", *token::decimal);
	rules.push(R"(\$[0-9A-Fa-f]{4})", *token::hex);
	rules.push("[@;i].*\r?\n", rules::skip());
	rules.push(R"("/*"(?s:.)*?"*/")", rules::skip());
	rules.push("(?s:.)", rules::skip());
	generator::build(rules, sm);

	citerator iter(first, second, sm);

	data._program._org = 0;

	while (iter->id != 0)
	{
		uint16_t addr = 0;
		const auto eol = std::find(iter->first, second, '\n');

		std::from_chars(iter->first + (iter->id == *token::decimal ? 0 :1),
			iter->first + (iter->id == *token::decimal ? 6 : 5),
			addr, iter->id == *token::decimal ? 10 : 16);

		if (addr == 0)
			continue;

		if (data._program._org == 0)
		{
			data._program._org = addr;
			data._program._org_set = true;
		}
		else if (data._program._org + data._program._memory.size() != addr)
		{
			uint16_t next_addr = data._program._org + (data._program._memory.size() & 0xffff);

			data._program._memory.resize(data._program._memory.size() + addr - next_addr);
		}

		data.parse(first, iter->first + 6, *eol == '\0' ? eol : eol + 1);
		iter = citerator(*eol == '\0' ? eol : eol + 1, second, sm);
	}
}
