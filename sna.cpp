#include "data.hpp"
#include "sna.hpp"
#include "z80_error.hpp"

#include <lexertl/generator.hpp>
#include <lexertl/iterator.hpp>
#include <lexertl/memory_file.hpp>
#include <lexertl/rules.hpp>
#include <lexertl/state_machine.hpp>

#include <charconv>
#include <cstdlib>
#include <format>
#include <string>

void read_sna(std::string pathname, const char* first,
	const char* second, data& data)
{
	lexertl::memory_file mf;
	// 8 bits I [0];
	// 16 bits HL_alt [1]
	// 16 bits DE_alt [3]
	// 16 bits BC_alt [5]
	// 16 bits AF_alt [7]
	// 16 bits HL [9]
	// 16 bits DE [11]
	// 16 bits BC [13]
	// 16 bits IY [15]
	// 16 bits IX [17]
	// 8 bits Interrupt [19]
	// 8 bits R = [20]
	// 16 bits AF [21]
	// 16 bits SP [23]
	// 8 bits IntMode [25]
	// 8 bits BorderColor [26]
	//49152  bytes  RAM dump 16384..65535 [27]
	data._program._memory.assign(first + 27, second);

	pathname.erase(pathname.size() - 4);
	pathname += ".blocks";
	mf.open(pathname.c_str());

	if (!mf.data())
		throw z80_error(std::format("Unable to open {}", pathname));

	enum class token { integer = 1, name, nl };
	using namespace lexertl;
	rules rules;
	state_machine sm;

	rules.push(R"(\d+)", *token::integer);
	rules.push("[a-z]+", *token::name);
	rules.push(" ", rules::skip());
	rules.push("\r?\n", *token::nl);
	generator::build(rules, sm);

	citerator iter(mf.data(), mf.data() + mf.size(), sm);

	data._program._org = atoi(iter->first) & 0xffff;
	++iter;

	for (; iter->id != 0; ++iter)
	{
		using enum program::block::type;

		if (iter->id == *token::nl)
			continue;

		auto sv = iter->view();
		auto type = code;
		std::size_t count = 0;

		if (sv == "db")
			type = db;
		else if (sv == "ds")
			type = ds;
		else if (sv == "dw")
			type = dw;

		++iter;
		std::from_chars(iter->first, iter->second, count);
		data._program._mem_type.emplace_back(type, count);
	}
}
