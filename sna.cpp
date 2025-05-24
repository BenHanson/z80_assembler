#include <bit>
#include <charconv>
#include <format>
#include <lexertl/generator.hpp>
#include <lexertl/memory_file.hpp>
#include <stdlib.h>
#include "sna.hpp"
#include "z80_error.hpp"

void read_sna(std::string pathname, const char* first,
	const char* second, data& data)
{
	lexertl::memory_file mf;
	/*uint8_t I = *first;
	uint16_t HL_alt = *std::bit_cast<uint16_t*>(first + 1);
	uint16_t DE_alt = *std::bit_cast<uint16_t*>(first + 3);
	uint16_t BC_alt = *std::bit_cast<uint16_t*>(first + 5);
	uint16_t AF_alt = *std::bit_cast<uint16_t*>(first + 7);
	uint16_t HL = *std::bit_cast<uint16_t*>(first + 9);
	uint16_t DE = *std::bit_cast<uint16_t*>(first + 11);
	uint16_t BC = *std::bit_cast<uint16_t*>(first + 13);
	uint16_t IY = *std::bit_cast<uint16_t*>(first + 15);
	uint16_t IX = *std::bit_cast<uint16_t*>(first + 17);
	uint8_t Interrupt = first[19];
	uint8_t R = first[20];
	uint16_t AF = *std::bit_cast<uint16_t*>(first + 21);
	uint16_t SP = *std::bit_cast<uint16_t*>(first + 23);
	uint8_t IntMode = first[25];
	uint8_t BorderColor = first[26];*/

	//27       49152  bytes  RAM dump 16384..65535
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
		if (iter->id == *token::nl)
			continue;

		auto sv = iter->view();
		program::block::type type = program::block::type::code;
		std::size_t count = 0;

		if (sv == "db")
			type = program::block::type::db;
		else if (sv == "ds")
			type = program::block::type::ds;
		else if (sv == "dw")
			type = program::block::type::dw;

		++iter;
		std::from_chars(iter->first, iter->second, count);
		data._program._mem_type.emplace_back(type, count);
	}
}
