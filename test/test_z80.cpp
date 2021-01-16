#include <charconv>
#include "../disassem.h"
#include "../../lexertl14/include/lexertl/generator.hpp"
#include <iostream>
#include "../../lexertl14/include/lexertl/iterator.hpp"
#include "../../lexertl14/include/lexertl/memory_file.hpp"
#include "../parsers.h"

enum
{
	eString = 1,
	eNum,
	eNL
};

void build_opcodes_parser(lexertl::state_machine& sm)
{
	lexertl::rules rules;

	rules.push_state("NUM");
	rules.push("[/][/].*\r\n", rules.skip());
	rules.push("INITIAL", R"(\".*?\")", eString, "NUM");
	rules.push("NUM", " ", rules.skip(), ".");
	rules.push("NUM", "[den]", eNum, ".");
	rules.push("NUM", "[0-9A-F]{2}", eNum, ".");
	rules.push("NUM", "[0-9A-F]{4}", eNum, ".");
	rules.push("NUM", "\r\n", eNL, "INITIAL");
	lexertl::generator::build(rules, sm);
}

void test_opcodes(data& data)
{
	lexertl::memory_file mf("opcodes.txt");
	lexertl::state_machine sm;

	build_opcodes_parser(sm);

	lexertl::citerator iter(mf.data(), mf.data() + mf.size(), sm);
	lexertl::citerator end;
	std::string mnemonics;
	std::vector<uint8_t> opcodes;

	for (; iter != end; ++iter)
	{
		switch (iter->id)
		{
		case eString:
			if (*(iter->second - 2) == '*')
				mnemonics = iter->substr(1, 2) + "\r\n";
			else
				mnemonics = iter->substr(1, 1) + "\r\n";

			break;
		case eNum:
		{
			switch (iter->second - iter->first)
			{
			case 1:
				opcodes.push_back(1);
				break;
			case 2:
			{
				int val = 0;

				std::from_chars(iter->first, iter->second, val, 16);
				opcodes.push_back(val & 0xff);
				break;
			}
			case 4:
				int val = 0;

				std::from_chars(iter->first, iter->first + 2, val, 16);
				opcodes.push_back(val & 0xff);
				std::from_chars(iter->first + 2, iter->second, val, 16);
				opcodes.push_back(val & 0xff);
				break;
			}

			break;
		}
		case eNL:
		{
			try
			{
				data._org = 16384;
				mnemonics = "d EQU 01h\r\n"
					"n EQU 01h\r\n"
					"nn EQU 0101h\r\n"
					"e:\r\n"+ mnemonics;
				data.parse(mnemonics.data(), mnemonics.data() + mnemonics.size());
				dump(data, base::hex);

				if (opcodes != data._memory)
					throw std::runtime_error(mnemonics + " did not match.\n");
			}
			catch (const std::exception& e)
			{
				std::cout << e.what() << '\n';
			}

			opcodes.clear();
			data.clear();
			break;
		}
		default:
			throw std::runtime_error("Parse error in " + mnemonics);
		}
	}
}

int main()
{
	try
	{
		data data;

		build_parsers(data, 0);
		test_opcodes(data);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << '\n';
	}
}
