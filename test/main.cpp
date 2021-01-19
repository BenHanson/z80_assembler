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

void build_opcodes_parser2(lexertl::state_machine& sm)
{
	lexertl::rules rules;

	rules.push_state("NL");
	rules.push("[/][/].*\r\n", rules.skip());
	rules.push("INITIAL", R"(\".*?\")", eString, "NL");
	rules.push(" ", rules.skip());
	rules.push("[den]", eNum);
	rules.push("[0-9A-F]{2}", eNum);
	rules.push("[0-9A-F]{4}", eNum);
	rules.push("NL", "\r\n", eNL, "INITIAL");
	lexertl::generator::build(rules, sm);
}

void replace_vars(std::string& str, const base base)
{
	for (std::size_t pos = str.size(); pos != 0; --pos)
	{
		const std::size_t idx = pos - 1;

		switch (str[idx])
		{
		case 'd':
			//01h
			str[idx] = '1';

			if (base == base::hex)
				str.insert(idx, 1, '0');

			break;
		case 'n':
			//01h
			//0101h
			str[idx] = '1';

			if (base == base::hex)
				str.insert(idx, 1, '0');

			break;
		case 'e':
			if (base == base::hex)
			{
				//FEh
				str[idx] = 'E';
				str.insert(idx, 1, 'F');
			}
			else
			{
				//-2
				str[idx] = '2';
				str.insert(idx, 1, '-');
			}

			break;
		}
	}
}

void test_opcodes(const char* pathname, data& data, const lexertl::state_machine& sm)
{
	lexertl::memory_file mf(pathname);
	lexertl::citerator iter(mf.data(), mf.data() + mf.size(), sm);
	lexertl::citerator end;
	std::string text;
	std::vector<uint8_t> opcodes;

	for (; iter != end; ++iter)
	{
		switch (iter->id)
		{
		case eString:
			if (*(iter->second - 2) == '*')
				text = iter->substr(1, 2);
			else
				text = iter->substr(1, 1);

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
				std::string cmd = "d EQU 01h\r\n"
					"n EQU 01h\r\n"
					"nn EQU 0101h\r\n"
					"e:\r\n" + text + "\r\n";

				data._org = 16384;
				data.parse(cmd.data(), cmd.data() + cmd.size());
				cmd = mnemonic(data, base::hex);
				replace_vars(text, base::hex);

				if (opcodes != data._memory || cmd != text)
					throw std::runtime_error("mismatch:\n" + text + '\n' + cmd + '\n');
				else
				{
					std::cout << cmd;

					if (cmd.size() < 8)
						std::cout << "\t\t";
					else if (cmd.size() < 16)
						std::cout << '\t';

					std::cout << "\tPassed\n";
				}
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
			throw std::runtime_error("Parse error in " + text);
		}
	}
}

int main()
{
	try
	{
		data data;
		lexertl::state_machine sm;

		build_parsers(data, 0);
		build_opcodes_parser(sm);
		test_opcodes("opcodes.txt", data, sm);
		build_opcodes_parser2(sm);
		test_opcodes("opcodes2.txt", data, sm);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << '\n';
	}
}
