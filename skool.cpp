#include <algorithm>
#include <charconv>
#include "enums.hpp"
#include <lexertl/generator.hpp>
#include "skool.hpp"
#include <sstream>

void parse_skool(const char* first, const char* second,
	data& data, const base base)
{
	lexertl::rules rules;
	lexertl::state_machine sm;

	if (base == base::hexadecimal)
		rules.push(R"(\$[0-9A-Fa-f]{4})", 1);
	else
		rules.push(R"(\d{5})", 1);

	rules.push("[@;i].*\r?\n", lexertl::rules::skip());
	rules.push(R"("/*"(?s:.)*?"*/")", lexertl::rules::skip());
	rules.push("(?s:.)", lexertl::rules::skip());
	lexertl::generator::build(rules, sm);

	lexertl::citerator iter(first, second, sm);

	data._program._org = 0;

	while (iter->id != 0)
	{
		uint16_t addr = 0;
		const char* eol = std::find(iter->first, second, '\n');

		std::from_chars(iter->first + (base == base::decimal ? 0 :1),
			iter->first + (base == base::decimal ? 6 : 5),
			addr, base == base::decimal ? 10 : 16);

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
		iter = lexertl::citerator(*eol == '\0' ? eol : eol + 1, second, sm);
	}

	std::cout << '\n';
}
