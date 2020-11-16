#include <iostream>

#include "../parsertl14/include/parsertl/generator.hpp"
#include <iterator>
#include "../parsertl14/include/parsertl/match.hpp"
#include "../lexertl14/include/lexertl/memory_file.hpp"
#include <sstream>

using memory = std::vector<uint8_t>;
using token = parsertl::token<lexertl::citerator>;

struct data
{
	parsertl::state_machine _gsm;
	lexertl::state_machine _lsm;
	std::map<std::uint16_t, void (*)(data& data)> _actions;
	parsertl::match_results _results;
	token::token_vector _productions;
	uint16_t _org = 23296;
	std::map<std::string, uint16_t> _labels;
	// Relative jumps
	std::map<std::string, std::set<std::size_t>> _rel_set;
	// absolute jumps
	std::map<std::string, std::set<std::size_t>> _set;
	uint8_t _cc = ~0;
	uint8_t _dd = ~0;
	uint8_t _pp = ~0;
	uint8_t _qq = ~0;
	uint8_t _r = ~0;
	uint8_t _r2 = ~0;
	uint8_t _rr = ~0;
	uint8_t _ss = ~0;
	uint16_t _integer = ~0;
	memory _memory;

	auto dollar(const std::size_t index)
	{
		return _results.dollar(_gsm, index, _productions);
	}

	void push_byte()
	{
		if (_integer > 255)
			throw std::out_of_range("Value " + std::to_string(_integer) + " is greater than 255");

		_memory.push_back(_integer & 0xff);
	}

	void push_byte(const uint8_t by)
	{
		_memory.push_back(by);
	}

	void push_word()
	{
		if (_integer > 65535)
			throw std::out_of_range("Value " + std::to_string(_integer) + " is greater than 65535");

		_memory.push_back(_integer & 0xff);
		_memory.push_back(_integer >> 8);
	}

	void rel_label(const std::size_t idx)
	{
		const std::string name = dollar(idx).str();
		auto iter = _labels.find(name);

		if (iter == _labels.end())
		{
			_rel_set[name].insert(_memory.size() - 1);
		}
		else
		{
			int i = static_cast<int>(iter->second - _memory.size());

			if (i < -128 || i > 127)
				throw std::out_of_range("Out of range relative call to '" + name + '\'');

			_memory.back() = static_cast<unsigned int>(i);
		}
	}

	void wlabel(const int32_t idx)
	{
		const std::string name = dollar(idx).str();
		auto iter = _labels.find(name);

		if (iter == _labels.end())
		{
			_set[name].insert(_memory.size() - 2);
		}
		else
		{
			uint8_t* ptr = &_memory.back() - 1;
			uint16_t val = _org + iter->second;

			*ptr++ = val & 0xff;
			*ptr = static_cast<uint8_t>(val >> 8);
		}
	}

	int eight_bits(const std::size_t idx)
	{
		const int n = atoi(dollar(idx).first);

		if (n > 255)
		{
			const std::string str(dollar(0).first,
				dollar(_results.production_size(_gsm,
					_results.entry.param)).second);

			throw std::out_of_range(str + ": Integer out of range");
		}

		return n;
	}

	int sixteen_bits(const std::size_t idx)
	{
		const int nn = atoi(dollar(idx).first);

		if (nn > 65535)
		{
			const std::string str(dollar(0).first,
				dollar(_results.production_size(_gsm,
					_results.entry.param)).second);

			throw std::out_of_range(str + ": Integer out of range");
		}

		return nn;
	}

	void build_parser()
	{
		parsertl::rules grules;
		lexertl::rules lrules(lexertl::dot_not_cr_lf | lexertl::icase);
		std::string warnings;

		grules.token("A ADD ADC AF AND B BC Binary BIT C CALL CCF Char CP CPD "
			"CPDR CPI CPIR CPL D DAA DB DW DE DEC DI DJNZ E EI EX EXX H HALT "
			"Hex HL Integer I IM IN INC IND INDR INI INIR IX IY JP JR L LD "
			"LDD LDDR LDI LDIR M Name NC NEG NL NOP NZ OR ORG OTDR OTIR OUT "
			"OUTD OUTI P PE PO PUSH POP R RES RET RETI RETN RL RLA RLC RLCA "
			"RLD RR RRA RRC RRCA RRD RST SBC SCF SET SLA SP SRA SRL String "
			"SUB XOR Z");
		grules.push("opcodes", "%empty "
			"| opcodes opcode NL");
		grules.push("opcode", "");
		_actions[grules.push("opcode", "ORG integer")] = [](data& data)
		{
			data._org = data._integer;
		};
		grules.push("opcode", "label opt_colon opcode");
		_actions[grules.push("label", "Name")] = [](data& data)
		{
			std::string name = data.dollar(0).str();
			auto iter = data._labels.find(name);

			if (iter != data._labels.end())
			{
				throw std::runtime_error(name + " already exists");
			}

			data._labels[name] = static_cast<uint16_t>(data._memory.size());
		};
		grules.push("opt_colon", "%empty | ':'");
		grules.push("opcode", "DB db_list");
		grules.push("opcode", "DW dw_list");
		_actions[grules.push("db_list", "integer")] = [](data& data)
		{
			data.push_byte();
		};
		_actions[grules.push("db_list", "String")] = [](data& data)
		{
			auto t = data.dollar(0);

			++t.first;
			--t.second;

			for (const char* iter = t.first; iter != t.second; ++iter)
			{
				data.push_byte(static_cast<uint8_t>(*iter));
			}
		};
		_actions[grules.push("db_list", "db_list ',' integer")] = [](data& data)
		{
			data.push_byte();
		};
		_actions[grules.push("db_list", "db_list ',' String")] = [](data& data)
		{
			auto t = data.dollar(2);

			++t.first;
			--t.second;

			for (const char* iter = t.first; iter != t.second; ++iter)
			{
				data.push_byte(static_cast<uint8_t>(*iter));
			}
		};
		_actions[grules.push("dw_list", "integer")] = [](data& data)
		{
			data.push_word();
		};
		_actions[grules.push("dw_list", "Name")] = [](data& data)
		{
			data.push_byte(0);
			data.push_byte(0);
			data.wlabel(0);
		};
		_actions[grules.push("dw_list", "db_list ',' integer")] = [](data& data)
		{
			data.push_word();
		};
		_actions[grules.push("dw_list", "db_list ',' Name")] = [](data& data)
		{
			data.push_byte(0);
			data.push_byte(0);
			data.wlabel(2);
		};
		_actions[grules.push("opcode", "LD r ',' r2")] = [](data& data)
		{
			// Made local var to prevent VC++ warning
			const uint8_t by = 0b01000000 | data._r << 3 | data._r2;

			data.push_byte(by);
		};
		_actions[grules.push("opcode", "LD r ',' integer")] = [](data& data)
		{
			data.push_byte(0b00000110 | data._r << 3);
			data.push_byte();
		};
		_actions[grules.push("opcode", "LD r ',' '(' HL ')'")] = [](data& data)
		{
			data.push_byte(0b01000110 | data._r << 3);
		};
		_actions[grules.push("opcode", "LD r ',' '(' IX '+' integer ')'")] = [](data& data)
		{
			data.push_byte(0xDD);
			data.push_byte(0b01000110 | data._r << 3);
			data.push_byte();
		};
		_actions[grules.push("opcode", "LD r ',' '(' IY '+' integer ')'")] = [](data& data)
		{
			data.push_byte(0xFD);
			data.push_byte(0b01000110 | data._r << 3);
			data.push_byte();
		};
		_actions[grules.push("opcode", "LD '(' HL ')' ',' r")] = [](data& data)
		{
			data.push_byte(0b01110000 | data._r);
		};
		_actions[grules.push("opcode", "LD '(' IX '+' integer ')' ',' r")] = [](data& data)
		{
			data.push_byte(0xDD);
			data.push_byte(0b01110000 | data._r);
			data.push_byte();
		};
		_actions[grules.push("opcode", "LD '(' IY '+' integer ')' ',' r")] = [](data& data)
		{
			data.push_byte(0xFD);
			data.push_byte(0b01110000 | data._r);
			data.push_byte();
		};
		_actions[grules.push("opcode", "LD '(' HL ')' ',' integer")] = [](data& data)
		{
			data.push_byte(0x36);
			data.push_byte();
		};
		_actions[grules.push("opcode", "LD '(' IX '+' integer ')' ',' Integer")] = [](data& data)
		{
			data.push_byte(0xDD);
			data.push_byte(0x36);
			data.push_byte();
			data.push_byte(data.eight_bits(7));
		};
		_actions[grules.push("opcode", "LD '(' IY '+' integer ')' ',' Integer")] = [](data& data)
		{
			data.push_byte(0xFD);
			data.push_byte(0x36);
			data.push_byte();
			data.push_byte(data.eight_bits(7));
		};
		// Only A register is legal
		_actions[grules.push("opcode", "LD r ',' '(' BC ')'")] = [](data& data)
		{
			if (data._r != 0b111)
			{
				const std::string str(data.dollar(0).first,
					data.dollar(5).second);

				throw std::runtime_error(str + ": Only register A valid");
			}

			data.push_byte(0x0A);
		};
		// Only A register is legal
		_actions[grules.push("opcode", "LD r ',' '(' DE ')'")] = [](data& data)
		{
			if (data._r != 0b111)
			{
				const std::string str(data.dollar(0).first,
					data.dollar(5).second);

				throw std::runtime_error(str + ": Only register A valid");
			}

			data.push_byte(0x1A);
		};
		// Only A register is legal
		_actions[grules.push("opcode", "LD r ',' '(' integer ')'")] = [](data& data)
		{
			if (data._r != 0b111)
			{
				const std::string str(data.dollar(0).first,
					data.dollar(5).second);

				throw std::runtime_error(str + ": Only register A valid");
			}

			data.push_byte(0x3A);
			data.push_word();
		};
		// Only A register is legal
		_actions[grules.push("opcode", "LD r ',' '(' Name ')'")] = [](data& data)
		{
			if (data._r != 0b111)
			{
				const std::string str(data.dollar(0).first,
					data.dollar(5).second);

				throw std::runtime_error(str + ": Only register A valid");
			}

			data.push_byte(0x3A);
			data.push_byte(0);
			data.push_byte(0);
			data.wlabel(4);
		};
		_actions[grules.push("opcode", "LD '(' BC ')' ',' A")] = [](data& data)
		{
			data.push_byte(0x02);
		};
		_actions[grules.push("opcode", "LD '(' DE ')' ',' A")] = [](data& data)
		{
			data.push_byte(0x12);
		};
		_actions[grules.push("opcode", "LD '(' integer ')' ',' A")] = [](data& data)
		{
			data.push_byte(0x32);
			data.push_word();
		};
		_actions[grules.push("opcode", "LD '(' Name ')' ',' A")] = [](data& data)
		{
			data.push_byte(0x32);
			data.push_byte(0);
			data.push_byte(0);
			data.wlabel(2);
		};
		// Only A register is legal
		_actions[grules.push("opcode", "LD r ',' I")] = [](data& data)
		{
			if (data._r != 0b111)
			{
				const std::string str(data.dollar(0).first,
					data.dollar(5).second);

				throw std::runtime_error(str + ": Only register A valid");
			}

			data.push_byte(0xED);
			data.push_byte(0x57);
		};
		// Only A register is legal
		_actions[grules.push("opcode", "LD r ',' R")] = [](data& data)
		{
			if (data._r != 0b111)
			{
				const std::string str(data.dollar(0).first,
					data.dollar(5).second);

				throw std::runtime_error(str + ": Only register A valid");
			}

			data.push_byte(0xED);
			data.push_byte(0x5F);
		};
		_actions[grules.push("opcode", "LD I ',' A")] = [](data& data)
		{
			data.push_byte(0xED);
			data.push_byte(0x47);
		};
		_actions[grules.push("opcode", "LD R ',' A")] = [](data& data)
		{
			data.push_byte(0xED);
			data.push_byte(0x4F);
		};
		_actions[grules.push("opcode", "LD dd ',' integer")] = [](data& data)
		{
			data.push_byte(0b00000001 | data._dd << 4);
			data.push_word();
		};
		_actions[grules.push("opcode", "LD dd ',' Name")] = [](data& data)
		{
			data.push_byte(0b00000001 | data._dd << 4);
			data.push_byte(0);
			data.push_byte(0);
			data.wlabel(3);
		};
		_actions[grules.push("opcode", "LD IX ',' integer")] = [](data& data)
		{
			data.push_byte(0xDD);
			data.push_byte(0x21);
			data.push_word();
		};
		_actions[grules.push("opcode", "LD IY ',' integer")] = [](data& data)
		{
			data.push_byte(0xFD);
			data.push_byte(0x21);
			data.push_word();
		};
		_actions[grules.push("opcode", "LD dd ',' '(' integer ')'")] = [](data& data)
		{
			if (data._dd == 0b10)
			{
				data.push_byte(0x2A);
			}
			else
			{
				data.push_byte(0xED);
				data.push_byte(0b01001011 | data._dd << 4);
			}

			data.push_word();
		};
		_actions[grules.push("opcode", "LD dd ',' '(' Name ')'")] = [](data& data)
		{
			if (data._dd == 0b10)
			{
				data.push_byte(0x2A);
			}
			else
			{
				data.push_byte(0xED);
				data.push_byte(0b01001011 | data._dd << 4);
			}

			data.push_byte(0);
			data.push_byte(0);
			data.wlabel(4);
		};
		_actions[grules.push("opcode", "LD IX ',' '(' integer ')'")] = [](data& data)
		{
			data.push_byte(0xDD);
			data.push_byte(0x2A);
			data.push_word();
		};
		_actions[grules.push("opcode", "LD IY ',' '(' integer ')'")] = [](data& data)
		{
			data.push_byte(0xFD);
			data.push_byte(0x2A);
			data.push_word();
		};
		_actions[grules.push("opcode", "LD '(' integer ')' ',' dd")] = [](data& data)
		{
			data.push_byte(0xED);
			data.push_byte(0b01000011 | data._dd << 4);
			data.push_word();
		};
		_actions[grules.push("opcode", "LD '(' integer ')' ',' IX")] = [](data& data)
		{
			data.push_byte(0xDD);
			data.push_byte(0x22);
			data.push_word();
		};
		_actions[grules.push("opcode", "LD '(' integer ')' ',' IY")] = [](data& data)
		{
			data.push_byte(0xFD);
			data.push_byte(0x22);
			data.push_word();
		};
		// Only SP register is legal
		_actions[grules.push("opcode", "LD dd ',' HL")] = [](data& data)
		{
			if (data._dd != 0b11)
			{
				const std::string str(data.dollar(0).first,
					data.dollar(5).second);

				throw std::runtime_error(str + ": Only register SP valid");
			}

			data.push_byte(0xF9);
		};
		// Only SP register is legal
		_actions[grules.push("opcode", "LD dd ',' IX")] = [](data& data)
		{
			if (data._dd != 0b11)
			{
				const std::string str(data.dollar(0).first,
					data.dollar(5).second);

				throw std::runtime_error(str + ": Only register SP valid");
			}

			data.push_byte(0xDD);
			data.push_byte(0xF9);
		};
		// Only SP register is legal
		_actions[grules.push("opcode", "LD dd ',' IY")] = [](data& data)
		{
			if (data._dd != 0b11)
			{
				const std::string str(data.dollar(0).first,
					data.dollar(5).second);

				throw std::runtime_error(str + ": Only register SP valid");
			}

			data.push_byte(0xFD);
			data.push_byte(0xF9);
		};
		_actions[grules.push("opcode", "PUSH qq")] = [](data& data)
		{
			data.push_byte(0b11000101 | data._qq << 4);
		};
		_actions[grules.push("opcode", "PUSH IX")] = [](data& data)
		{
			data.push_byte(0xDD);
			data.push_byte(0xE5);
		};
		_actions[grules.push("opcode", "PUSH IY")] = [](data& data)
		{
			data.push_byte(0xFD);
			data.push_byte(0xE5);
		};
		_actions[grules.push("opcode", "POP qq")] = [](data& data)
		{
			data.push_byte(0b11000001 | data._qq << 4);
		};
		_actions[grules.push("opcode", "POP IX")] = [](data& data)
		{
			data.push_byte(0xDD);
			data.push_byte(0xE1);
		};
		_actions[grules.push("opcode", "POP IY")] = [](data& data)
		{
			data.push_byte(0xFD);
			data.push_byte(0xE1);
		};
		_actions[grules.push("opcode", "EX DE ',' HL")] = [](data& data)
		{
			data.push_byte(0xEB);
		};
		_actions[grules.push("opcode", R"(EX AF ',' AF '\'')")] = [](data& data)
		{
			data.push_byte(0x08);
		};
		_actions[grules.push("opcode", "EXX")] = [](data& data)
		{
			data.push_byte(0xD9);
		};
		_actions[grules.push("opcode", "EX '(' SP ')' ',' HL")] = [](data& data)
		{
			data.push_byte(0xE3);
		};
		_actions[grules.push("opcode", "EX '(' SP ')' ',' IX")] = [](data& data)
		{
			data.push_byte(0xDD);
			data.push_byte(0xE3);
		};
		_actions[grules.push("opcode", "EX '(' SP ')' ',' IY")] = [](data& data)
		{
			data.push_byte(0xFD);
			data.push_byte(0xE3);
		};
		_actions[grules.push("opcode", "LDI")] = [](data& data)
		{
			data.push_byte(0xED);
			data.push_byte(0xA0);
		};
		_actions[grules.push("opcode", "LDIR")] = [](data& data)
		{
			data.push_byte(0xED);
			data.push_byte(0xB0);
		};
		_actions[grules.push("opcode", "LDD")] = [](data& data)
		{
			data.push_byte(0xED);
			data.push_byte(0xA8);
		};
		_actions[grules.push("opcode", "LDDR")] = [](data& data)
		{
			data.push_byte(0xED);
			data.push_byte(0xB8);
		};
		_actions[grules.push("opcode", "CPI")] = [](data& data)
		{
			data.push_byte(0xED);
			data.push_byte(0xA1);
		};
		_actions[grules.push("opcode", "CPIR")] = [](data& data)
		{
			data.push_byte(0xED);
			data.push_byte(0xB1);
		};
		_actions[grules.push("opcode", "CPD")] = [](data& data)
		{
			data.push_byte(0xED);
			data.push_byte(0xA9);
		};
		_actions[grules.push("opcode", "CPDR")] = [](data& data)
		{
			data.push_byte(0xED);
			data.push_byte(0xB9);
		};
		_actions[grules.push("opcode", "ADD A ',' r")] = [](data& data)
		{
			data.push_byte(0b10000000 | data._r);
		};
		_actions[grules.push("opcode", "ADD A ',' integer")] = [](data& data)
		{
			data.push_byte(0b11000110);
			data.push_byte();
		};
		_actions[grules.push("opcode", "ADD A ',' '(' HL ')'")] = [](data& data)
		{
			data.push_byte(0b10000110);
		};
		_actions[grules.push("opcode", "ADD A ',' '(' IX '+' integer ')'")] = [](data& data)
		{
			data.push_byte(0xDD);
			data.push_byte(0x86);
			data.push_byte();
		};
		_actions[grules.push("opcode", "ADD A ',' '(' IY '+' integer ')'")] = [](data& data)
		{
			data.push_byte(0xFD);
			data.push_byte(0x86);
			data.push_byte();
		};
		_actions[grules.push("opcode", "ADC A ',' r")] = [](data& data)
		{
			data.push_byte(0b10001000 | data._r);
		};
		_actions[grules.push("opcode", "ADC A ',' integer")] = [](data& data)
		{
			data.push_byte(0xCE);
			data.push_byte();
		};
		_actions[grules.push("opcode", "ADC A ',' '(' HL ')'")] = [](data& data)
		{
			data.push_byte(0x8E);
		};
		_actions[grules.push("opcode", "ADC A ',' '(' IX '+' integer ')'")] = [](data& data)
		{
			data.push_byte(0xDD);
			data.push_byte(0x8E);
			data.push_byte();
		};
		_actions[grules.push("opcode", "SUB r")] = [](data& data)
		{
			data.push_byte(0b10010000 | data._r);
		};
		_actions[grules.push("opcode", "SUB integer")] = [](data& data)
		{
			data.push_byte(0xd6);
			data.push_byte();
		};
		_actions[grules.push("opcode", "SUB '(' HL ')'")] = [](data& data)
		{
			data.push_byte(0x96);
		};
		_actions[grules.push("opcode", "SUB '(' IX '+' integer ')'")] = [](data& data)
		{
			data.push_byte(0xDD);
			data.push_byte(0x96);
			data.push_byte();
		};
		_actions[grules.push("opcode", "SUB '(' IY '+' integer ')'")] = [](data& data)
		{
			data.push_byte(0xFD);
			data.push_byte(0x96);
			data.push_byte();
		};
		_actions[grules.push("opcode", "SBC A ',' r")] = [](data& data)
		{
			data.push_byte(0b10011000 | data._r);
		};
		_actions[grules.push("opcode", "SBC A ',' integer")] = [](data& data)
		{
			data.push_byte(0xDE);
			data.push_byte();
		};
		_actions[grules.push("opcode", "SBC A ',' '(' HL ')'")] = [](data& data)
		{
			data.push_byte(0b10011110);
		};
		_actions[grules.push("opcode", "SBC A ',' '(' IX '+' integer ')'")] = [](data& data)
		{
			data.push_byte(0xDD);
			data.push_byte(0x9E);
			data.push_byte();
		};
		_actions[grules.push("opcode", "SBC A ',' '(' IY '+' integer ')'")] = [](data& data)
		{
			data.push_byte(0xFD);
			data.push_byte(0x9E);
			data.push_byte();
		};
		_actions[grules.push("opcode", "AND r")] = [](data& data)
		{
			data.push_byte(0b10100000 | data._r);
		};
		_actions[grules.push("opcode", "AND integer")] = [](data& data)
		{
			data.push_byte(0xE6);
			data.push_byte();
		};
		_actions[grules.push("opcode", "AND '(' HL ')'")] = [](data& data)
		{
			data.push_byte(0xA6);
		};
		_actions[grules.push("opcode", "AND '(' IX '+' integer ')'")] = [](data& data)
		{
			data.push_byte(0xDD);
			data.push_byte(0xA6);
			data.push_byte();
		};
		_actions[grules.push("opcode", "AND '(' IY '+' integer ')'")] = [](data& data)
		{
			data.push_byte(0xFD);
			data.push_byte(0xA6);
			data.push_byte();
		};
		_actions[grules.push("opcode", "OR r")] = [](data& data)
		{
			data.push_byte(0b10110000 | data._r);
		};
		_actions[grules.push("opcode", "OR integer")] = [](data& data)
		{
			data.push_byte(0xF6);
			data.push_byte();
		};
		_actions[grules.push("opcode", "OR '(' HL ')'")] = [](data& data)
		{
			data.push_byte(0xB6);
		};
		_actions[grules.push("opcode", "OR '(' IX '+' integer ')'")] = [](data& data)
		{
			data.push_byte(0xDD);
			data.push_byte(0xB6);
			data.push_byte();
		};
		_actions[grules.push("opcode", "OR '(' IY '+' integer ')'")] = [](data& data)
		{
			data.push_byte(0xFD);
			data.push_byte(0xB6);
			data.push_byte();
		};
		_actions[grules.push("opcode", "XOR r")] = [](data& data)
		{
			data.push_byte(0b10101000 | data._r);
		};
		_actions[grules.push("opcode", "XOR integer")] = [](data& data)
		{
			data.push_byte(0xEE);
			data.push_byte();
		};
		_actions[grules.push("opcode", "XOR '(' HL ')'")] = [](data& data)
		{
			data.push_byte(0xAE);
		};
		_actions[grules.push("opcode", "XOR '(' IX '+' integer ')'")] = [](data& data)
		{
			data.push_byte(0xDD);
			data.push_byte(0xAE);
			data.push_byte();
		};
		_actions[grules.push("opcode", "XOR '(' IY '+' integer ')'")] = [](data& data)
		{
			data.push_byte(0xFD);
			data.push_byte(0xAE);
			data.push_byte();
		};
		_actions[grules.push("opcode", "CP r")] = [](data& data)
		{
			data.push_byte(0b10111000 | data._r);
		};
		_actions[grules.push("opcode", "CP integer")] = [](data& data)
		{
			data.push_byte(0xFE);
			data.push_byte();
		};
		_actions[grules.push("opcode", "CP '(' HL ')'")] = [](data& data)
		{
			data.push_byte(0xBE);
		};
		_actions[grules.push("opcode", "CP '(' IX '+' integer ')'")] = [](data& data)
		{
			data.push_byte(0xDD);
			data.push_byte(0xBE);
			data.push_byte();
		};
		_actions[grules.push("opcode", "CP '(' IY '+' integer ')'")] = [](data& data)
		{
			data.push_byte(0xFD);
			data.push_byte(0xBE);
			data.push_byte();
		};
		_actions[grules.push("opcode", "INC r")] = [](data& data)
		{
			data.push_byte(0b00000100 | data._r << 3);
		};
		_actions[grules.push("opcode", "INC '(' HL ')'")] = [](data& data)
		{
			data.push_byte(0x34);
		};
		_actions[grules.push("opcode", "INC '(' IX '+' integer ')'")] = [](data& data)
		{
			data.push_byte(0xDD);
			data.push_byte(0x34);
			data.push_byte();
		};
		_actions[grules.push("opcode", "INC '(' IY '+' integer ')'")] = [](data& data)
		{
			data.push_byte(0xFD);
			data.push_byte(0x34);
			data.push_byte();
		};
		_actions[grules.push("opcode", "DEC r")] = [](data& data)
		{
			data.push_byte(0b00000101 | data._r << 3);
		};
		_actions[grules.push("opcode", "DEC '(' HL ')'")] = [](data& data)
		{
			data.push_byte(0x35);
		};
		_actions[grules.push("opcode", "DEC '(' IX '+' integer ')'")] = [](data& data)
		{
			data.push_byte(0xDD);
			data.push_byte(0x35);
			data.push_byte();
		};
		_actions[grules.push("opcode", "DEC '(' IY '+' integer ')'")] = [](data& data)
		{
			data.push_byte(0xFD);
			data.push_byte(0x35);
			data.push_byte();
		};
		_actions[grules.push("opcode", "DAA")] = [](data& data)
		{
			data.push_byte(0x27);
		};
		_actions[grules.push("opcode", "CPL")] = [](data& data)
		{
			data.push_byte(0x2F);
		};
		_actions[grules.push("opcode", "NEG")] = [](data& data)
		{
			data.push_byte(0xED);
			data.push_byte(0x44);
		};
		_actions[grules.push("opcode", "CCF")] = [](data& data)
		{
			data.push_byte(0x3F);
		};
		_actions[grules.push("opcode", "SCF")] = [](data& data)
		{
			data.push_byte(0x37);
		};
		_actions[grules.push("opcode", "NOP")] = [](data& data)
		{
			data.push_byte(0);
		};
		_actions[grules.push("opcode", "HALT")] = [](data& data)
		{
			data.push_byte(0x76);
		};
		_actions[grules.push("opcode", "DI")] = [](data& data)
		{
			data.push_byte(0xF3);
		};
		_actions[grules.push("opcode", "EI")] = [](data& data)
		{
			data.push_byte(0xFB);
		};
		// Integer can be 0, 1, 2
		_actions[grules.push("opcode", "IM Integer")] = [](data& data)
		{
			const int i = atoi(data.dollar(1).first);

			switch (i)
			{
			case 0:
				data.push_byte(0xED);
				data.push_byte(0x46);
				break;
			case 1:
				data.push_byte(0xED);
				data.push_byte(0x56);
				break;
			case 2:
				data.push_byte(0xED);
				data.push_byte(0x5E);
				break;
			default:
				throw std::out_of_range(std::string(data.dollar(0).first,
					data.dollar(1).second) + ": Integer out of range");
			}
		};
		_actions[grules.push("opcode", "ADD HL ',' ss")] = [](data& data)
		{
			data.push_byte(0b00001001 | data._ss << 4);
		};
		_actions[grules.push("opcode", "ADC HL ',' dd")] = [](data& data)
		{
			data.push_byte(0xED);
			data.push_byte(0b01001010 | data._dd << 4);
		};
		_actions[grules.push("opcode", "SBC HL ',' dd")] = [](data& data)
		{
			data.push_byte(0xED);
			data.push_byte(0b01000010 | data._dd << 4);
		};
		_actions[grules.push("opcode", "ADD IX ',' pp")] = [](data& data)
		{
			data.push_byte(0xDD);
			data.push_byte(0b00001001 | data._pp << 4);
		};
		_actions[grules.push("opcode", "ADD IY ',' rr")] = [](data& data)
		{
			data.push_byte(0xFD);
			data.push_byte(0b00001001 | data._rr << 4);
		};
		_actions[grules.push("opcode", "INC ss")] = [](data& data)
		{
			data.push_byte(0b00000011 | data._ss << 4);
		};
		_actions[grules.push("opcode", "INC IX")] = [](data& data)
		{
			data.push_byte(0xDD);
			data.push_byte(0x23);
		};
		_actions[grules.push("opcode", "INC IY")] = [](data& data)
		{
			data.push_byte(0xFD);
			data.push_byte(0x23);
		};
		_actions[grules.push("opcode", "DEC ss")] = [](data& data)
		{
			data.push_byte(0b00001011 | data._ss << 4);
		};
		_actions[grules.push("opcode", "DEC IX")] = [](data& data)
		{
			data.push_byte(0xDD);
			data.push_byte(0x2B);
		};
		_actions[grules.push("opcode", "DEC IY")] = [](data& data)
		{
			data.push_byte(0xFD);
			data.push_byte(0x2B);
		};
		_actions[grules.push("opcode", "RLCA")] = [](data& data)
		{
			data.push_byte(0x07);
		};
		_actions[grules.push("opcode", "RLA")] = [](data& data)
		{
			data.push_byte(0x17);
		};
		_actions[grules.push("opcode", "RRCA")] = [](data& data)
		{
			data.push_byte(0x0F);
		};
		_actions[grules.push("opcode", "RRA")] = [](data& data)
		{
			data.push_byte(0x1F);
		};
		_actions[grules.push("opcode", "RLC r")] = [](data& data)
		{
			data.push_byte(0xCB);
			data.push_byte(data._r);
		};
		_actions[grules.push("opcode", "RLC '(' HL ')'")] = [](data& data)
		{
			data.push_byte(0xCB);
			data.push_byte(0x06);
		};
		_actions[grules.push("opcode", "RLC '(' IX '+' integer ')'")] = [](data& data)
		{
			data.push_byte(0xDD);
			data.push_byte(0xCB);
			data.push_byte();
			data.push_byte(0x06);
		};
		_actions[grules.push("opcode", "RLC '(' IY '+' integer ')'")] = [](data& data)
		{
			data.push_byte(0xFD);
			data.push_byte(0xCB);
			data.push_byte();
			data.push_byte(0x06);
		};
		_actions[grules.push("opcode", "RL r")] = [](data& data)
		{
			data.push_byte(0xCB);
			data.push_byte(0b00010000 | data._r);
		};
		_actions[grules.push("opcode", "RL '(' HL ')'")] = [](data& data)
		{
			data.push_byte(0xCB);
			data.push_byte(0x16);
		};
		_actions[grules.push("opcode", "RL '(' IX '+' integer ')'")] = [](data& data)
		{
			data.push_byte(0xDD);
			data.push_byte(0xCB);
			data.push_byte();
			data.push_byte(0x16);
		};
		_actions[grules.push("opcode", "RL '(' IY '+' integer ')'")] = [](data& data)
		{
			data.push_byte(0xFD);
			data.push_byte(0xCB);
			data.push_byte();
			data.push_byte(0x16);
		};
		_actions[grules.push("opcode", "RRC r")] = [](data& data)
		{
			data.push_byte(0xCB);
			data.push_byte(0b00001000 | data._r);
		};
		_actions[grules.push("opcode", "RRC '(' HL ')'")] = [](data& data)
		{
			data.push_byte(0xCB);
			data.push_byte(0x0E);
		};
		_actions[grules.push("opcode", "RRC '(' IX '+' integer ')'")] = [](data& data)
		{
			data.push_byte(0xDD);
			data.push_byte(0xCB);
			data.push_byte();
			data.push_byte(0x0E);
		};
		_actions[grules.push("opcode", "RRC '(' IY '+' integer ')'")] = [](data& data)
		{
			data.push_byte(0xFD);
			data.push_byte(0xCB);
			data.push_byte();
			data.push_byte(0x0E);
		};
		_actions[grules.push("opcode", "RR r")] = [](data& data)
		{
			data.push_byte(0xCB);
			data.push_byte(0b00001000 | data._r);
		};
		_actions[grules.push("opcode", "RR '(' HL ')'")] = [](data& data)
		{
			data.push_byte(0xCB);
			data.push_byte(0x1E);
		};
		_actions[grules.push("opcode", "RR '(' IX '+' integer ')'")] = [](data& data)
		{
			data.push_byte(0xDD);
			data.push_byte(0xCB);
			data.push_byte();
			data.push_byte(0x1E);
		};
		_actions[grules.push("opcode", "RR '(' IY '+' integer ')'")] = [](data& data)
		{
			data.push_byte(0xFD);
			data.push_byte(0xCB);
			data.push_byte();
			data.push_byte(0x1E);
		};
		_actions[grules.push("opcode", "SLA r")] = [](data& data)
		{
			data.push_byte(0xCB);
			data.push_byte(0b00100000 | data._r);
		};
		_actions[grules.push("opcode", "SLA '(' HL ')'")] = [](data& data)
		{
			data.push_byte(0xCB);
			data.push_byte(0x26);
		};
		_actions[grules.push("opcode", "SLA '(' IX '+' integer ')'")] = [](data& data)
		{
			data.push_byte(0xDD);
			data.push_byte(0xCB);
			data.push_byte();
			data.push_byte(0x26);
		};
		_actions[grules.push("opcode", "SLA '(' IY '+' integer ')'")] = [](data& data)
		{
			data.push_byte(0xFD);
			data.push_byte(0xCB);
			data.push_byte();
			data.push_byte(0x26);
		};
		_actions[grules.push("opcode", "SRA r")] = [](data& data)
		{
			data.push_byte(0xCB);
			data.push_byte(0b00101000 | data._r);
		};
		_actions[grules.push("opcode", "SRA '(' HL ')'")] = [](data& data)
		{
			data.push_byte(0xCB);
			data.push_byte(0x2E);
		};
		_actions[grules.push("opcode", "SRA '(' IX '+' integer ')'")] = [](data& data)
		{
			data.push_byte(0xDD);
			data.push_byte(0xCB);
			data.push_byte();
			data.push_byte(0x2E);
		};
		_actions[grules.push("opcode", "SRA '(' IY '+' integer ')'")] = [](data& data)
		{
			data.push_byte(0xFD);
			data.push_byte(0xCB);
			data.push_byte();
			data.push_byte(0x2E);
		};
		_actions[grules.push("opcode", "SRL r")] = [](data& data)
		{
			data.push_byte(0xCB);
			data.push_byte(0b00111000 | data._r);
		};
		_actions[grules.push("opcode", "SRL '(' HL ')'")] = [](data& data)
		{
			data.push_byte(0xCB);
			data.push_byte(0x3E);
		};
		_actions[grules.push("opcode", "SRL '(' IX '+' integer ')'")] = [](data& data)
		{
			data.push_byte(0xDD);
			data.push_byte(0xCB);
			data.push_byte();
			data.push_byte(0x3E);
		};
		_actions[grules.push("opcode", "SRL '(' IY '+' integer ')'")] = [](data& data)
		{
			data.push_byte(0xFD);
			data.push_byte(0xCB);
			data.push_byte();
			data.push_byte(0x3E);
		};
		_actions[grules.push("opcode", "RLD")] = [](data& data)
		{
			data.push_byte(0xED);
			data.push_byte(0x6F);
		};
		_actions[grules.push("opcode", "RRD")] = [](data& data)
		{
			data.push_byte(0xED);
			data.push_byte(0x67);
		};
		// Integer is 0-7
		_actions[grules.push("opcode", "BIT Integer ',' r")] = [](data& data)
		{
			const int bit = atoi(data.dollar(1).first);

			if (bit > 7)
			{
				const std::string str(data.dollar(0).first,
					data.dollar(3).second);

				throw std::out_of_range(str + ": Integer out of range");
			}

			// Made local var to prevent VC++ warning
			const uint8_t by = 0b01000000 | bit << 3 | data._r;

			data.push_byte(0xCB);
			data.push_byte(by);
		};
		// Integer is 0-7
		_actions[grules.push("opcode", "BIT Integer ',' '(' HL ')'")] = [](data& data)
		{
			const int bit = atoi(data.dollar(1).first);

			if (bit > 7)
			{
				const std::string str(data.dollar(0).first,
					data.dollar(5).second);

				throw std::out_of_range(str + ": Integer out of range");
			}

			data.push_byte(0xCB);
			data.push_byte(0b01000110 | bit << 3);
		};
		// Integer is 0-7
		_actions[grules.push("opcode", "BIT Integer ',' '(' IX '+' Integer ')'")] = [](data& data)
		{
			const int bit =
				atoi(data.dollar(1).first);

			if (bit > 7)
			{
				const std::string str(data.dollar(0).first,
					data.dollar(7).second);

				throw std::out_of_range(str + ": Integer out of range");
			}

			data.push_byte(0xDD);
			data.push_byte(0xCB);
			data.push_byte(data.eight_bits(6));
			data.push_byte(0b01000110 | bit << 3);
		};
		// Integer is 0-7
		_actions[grules.push("opcode", "BIT Integer ',' '(' IY '+' Integer ')'")] = [](data& data)
		{
			const int bit =
				atoi(data.dollar(1).first);

			if (bit > 7)
			{
				const std::string str(data.dollar(0).first,
					data.dollar(7).second);

				throw std::out_of_range(str + ": Integer out of range");
			}

			data.push_byte(0xFD);
			data.push_byte(0xCB);
			data.push_byte(data.eight_bits(6));
			data.push_byte(0b01000110 | bit << 3);
		};
		// Integer is 0-7
		_actions[grules.push("opcode", "SET Integer ',' r")] = [](data& data)
		{
			const int bit = atoi(data.dollar(1).first);

			if (bit > 7)
			{
				const std::string str(data.dollar(0).first,
					data.dollar(3).second);

				throw std::out_of_range(str + ": Integer out of range");
			}

			// Made local var to prevent VC++ warning
			const uint8_t by = 0b11000000 | bit << 3 | data._r;

			data.push_byte(0xCB);
			data.push_byte(by);
		};
		// Integer is 0-7
		_actions[grules.push("opcode", "SET Integer ',' '(' HL ')'")] = [](data& data)
		{
			const int bit = atoi(data.dollar(1).first);

			if (bit > 7)
			{
				const std::string str(data.dollar(0).first,
					data.dollar(5).second);

				throw std::out_of_range(str + ": Integer out of range");
			}

			data.push_byte(0xCB);
			data.push_byte(0b11000110 | bit << 3);
		};
		// Integer is 0-7
		_actions[grules.push("opcode", "SET Integer ',' '(' IX '+' Integer ')'")] = [](data& data)
		{
			const int bit = atoi(data.dollar(1).first);

			if (bit > 7)
			{
				const std::string str(data.dollar(0).first,
					data.dollar(7).second);

				throw std::out_of_range(str + ": Integer out of range");
			}

			data.push_byte(0xDD);
			data.push_byte(0xCB);
			data.push_byte(data.eight_bits(1));
			data.push_byte(0b11000110 | bit << 3);
		};
		// Integer is 0-7
		_actions[grules.push("opcode", "SET Integer ',' '(' IY '+' Integer ')'")] = [](data& data)
		{
			const int bit = atoi(data.dollar(1).first);

			if (bit > 7)
			{
				const std::string str(data.dollar(0).first,
					data.dollar(7).second);

				throw std::out_of_range(str + ": Integer out of range");
			}

			data.push_byte(0xFD);
			data.push_byte(0xCB);
			data.push_byte(data.eight_bits(1));
			data.push_byte(0b11000110 | bit << 3);
		};
		// Integer is 0-7
		_actions[grules.push("opcode", "RES Integer ',' r")] = [](data& data)
		{
			const int bit = atoi(data.dollar(1).first);

			if (bit > 7)
			{
				const std::string str(data.dollar(0).first,
					data.dollar(3).second);

				throw std::out_of_range(str + ": Integer out of range");
			}

			// Made local var to prevent VC++ warning
			const uint8_t by = 0b10000000 | bit << 3 | data._r;

			data.push_byte(0xCB);
			data.push_byte(by);
		};
		_actions[grules.push("opcode", "RES Integer ',' '(' HL ')'")] = [](data& data)
		{
			const int bit =
				atoi(data.dollar(1).first);

			if (bit > 7)
			{
				const std::string str(data.dollar(0).first,
					data.dollar(5).second);

				throw std::out_of_range(str + ": Integer out of range");
			}

			data.push_byte(0xCB);
			data.push_byte(0b10000110 | bit << 3);
		};
		_actions[grules.push("opcode", "RES Integer ',' '(' IX '+' Integer ')'")] = [](data& data)
		{
			const int bit =
				atoi(data.dollar(1).first);

			if (bit > 7)
			{
				const std::string str(data.dollar(0).first,
					data.dollar(7).second);

				throw std::out_of_range(str + ": Integer out of range");
			}

			data.push_byte(0xDD);
			data.push_byte(0xCB);
			data.push_byte(data.eight_bits(6));
			data.push_byte(0b10000110 | bit << 3);
		};
		_actions[grules.push("opcode", "RES Integer ',' '(' IY '+' Integer ')'")] = [](data& data)
		{
			const int bit = atoi(data.dollar(1).first);

			if (bit > 7)
			{
				const std::string str(data.dollar(0).first,
					data.dollar(7).second);

				throw std::out_of_range(str + ": Integer out of range");
			}

			data.push_byte(0xFD);
			data.push_byte(0xCB);
			data.push_byte(data.eight_bits(6));
			data.push_byte(0b10000110 | bit << 3);
		};
		_actions[grules.push("opcode", "JP integer")] = [](data& data)
		{
			data.push_byte(0xC3);
			data.push_word();
		};
		_actions[grules.push("opcode", "JP Name")] = [](data& data)
		{
			data.push_byte(0xC3);
			data.push_byte(0);
			data.push_byte(0);
			data.wlabel(1);
		};
		_actions[grules.push("opcode", "JP cc ',' integer")] = [](data& data)
		{
			data.push_byte(0b11000010 | data._cc << 3);
			data.push_word();
		};
		_actions[grules.push("opcode", "JP cc ',' Name")] = [](data& data)
		{
			data.push_byte(0b11000010 | data._cc << 3);
			data.push_byte(0);
			data.push_byte(0);
			data.wlabel(3);
		};
		_actions[grules.push("opcode", "JR Name")] = [](data& data)
		{
			data.push_byte(0x18);
			data.push_byte(0);
			data.rel_label(1);
		};
		_actions[grules.push("opcode", "JR C ',' Name")] = [](data& data)
		{
			data.push_byte(0x38);
			data.push_byte(0);
			data.rel_label(3);
		};
		_actions[grules.push("opcode", "JR NC ',' Name")] = [](data& data)
		{
			data.push_byte(0x30);
			data.push_byte(0);
			data.rel_label(3);
		};
		_actions[grules.push("opcode", "JR Z ',' Name")] = [](data& data)
		{
			data.push_byte(0x28);
			data.push_byte(0);
			data.rel_label(3);
		};
		_actions[grules.push("opcode", "JR NZ ',' Name")] = [](data& data)
		{
			data.push_byte(0x20);
			data.push_byte(0);
			data.rel_label(3);
		};
		_actions[grules.push("opcode", "JP '(' HL ')'")] = [](data& data)
		{
			data.push_byte(0xE9);
		};
		_actions[grules.push("opcode", "JP '(' IX ')'")] = [](data& data)
		{
			data.push_byte(0xDD);
			data.push_byte(0xE9);
		};
		_actions[grules.push("opcode", "JP '(' IY ')'")] = [](data& data)
		{
			data.push_byte(0xFD);
			data.push_byte(0xE9);
		};
		_actions[grules.push("opcode", "DJNZ Name")] = [](data& data)
		{
			data.push_byte(0x10);
			data.push_byte(0);
			data.rel_label(1);
		};
		_actions[grules.push("opcode", "CALL integer")] = [](data& data)
		{
			data.push_byte(0xCD);
			data.push_word();
		};
		_actions[grules.push("opcode", "CALL Name")] = [](data& data)
		{
			data.push_byte(0xCD);
			data.push_byte(0);
			data.push_byte(0);
			data.wlabel(1);
		};
		_actions[grules.push("opcode", "CALL cc ',' integer")] = [](data& data)
		{
			data.push_byte(0b11000100 | data._cc << 3);
			data.push_word();
		};
		_actions[grules.push("opcode", "CALL cc ',' Name")] = [](data& data)
		{
			data.push_byte(0b11000100 | data._cc << 3);
			data.push_byte(0);
			data.push_byte(0);
			data.wlabel(3);
		};
		_actions[grules.push("opcode", "RET")] = [](data& data)
		{
			data.push_byte(0xC9);
		};
		_actions[grules.push("opcode", "RET cc")] = [](data& data)
		{
			data.push_byte(0b11000000 | data._cc << 3);
		};
		_actions[grules.push("opcode", "RETI")] = [](data& data)
		{
			data.push_byte(0xED);
			data.push_byte(0x4D);
		};
		_actions[grules.push("opcode", "RETN")] = [](data& data)
		{
			data.push_byte(0xED);
			data.push_byte(0x45);
		};
		// Integer is 0-7
		_actions[grules.push("opcode", "RST Integer")] = [](data& data)
		{
			int t = atoi(data.dollar(1).first);
			uint8_t by = 0;

			switch (t)
			{
			case 0x00:
				by = 0b000;
				break;
			case 0x08:
				by = 0b001;
				break;
			case 0x10:
				by = 0b010;
				break;
			case 0x18:
				by = 0b011;
				break;
			case 0x20:
				by = 0b100;
				break;
			case 0x28:
				by = 0b101;
				break;
			case 0x30:
				by = 0b110;
				break;
			case 0x38:
				by = 0b111;
				break;
			default:
				throw std::out_of_range("Invalid value for RST");
			}

			data.push_byte(0b11000111 | by << 3);
		};
		_actions[grules.push("opcode", "IN r '(' C ')'")] = [](data& data)
		{
			data.push_byte(0xED);
			data.push_byte(0b01000000 | data._r << 3);
		};
		_actions[grules.push("opcode", "INI")] = [](data& data)
		{
			data.push_byte(0xED);
			data.push_byte(0xA2);
		};
		_actions[grules.push("opcode", "INIR")] = [](data& data)
		{
			data.push_byte(0xED);
			data.push_byte(0xB2);
		};
		_actions[grules.push("opcode", "IND")] = [](data& data)
		{
			data.push_byte(0xED);
			data.push_byte(0xAA);
		};
		_actions[grules.push("opcode", "INDR")] = [](data& data)
		{
			data.push_byte(0xED);
			data.push_byte(0xBA);
		};
		_actions[grules.push("opcode", "OUT '(' integer ')' ',' A")] = [](data& data)
		{
			data.push_byte(0xD3);
			data.push_byte();
		};
		_actions[grules.push("opcode", "OUT '(' C ')' ',' r")] = [](data& data)
		{
			data.push_byte(0xED);
			data.push_byte(0b01000001 | data._r << 3);
		};
		_actions[grules.push("opcode", "OUTI")] = [](data& data)
		{
			data.push_byte(0xED);
			data.push_byte(0xA3);
		};
		_actions[grules.push("opcode", "OTIR")] = [](data& data)
		{
			data.push_byte(0xED);
			data.push_byte(0xB3);
		};
		_actions[grules.push("opcode", "OUTD")] = [](data& data)
		{
			data.push_byte(0xED);
			data.push_byte(0xAB);
		};
		_actions[grules.push("opcode", "OTDR")] = [](data& data)
		{
			data.push_byte(0xED);
			data.push_byte(0xBB);
		};

		_actions[grules.push("r", "A")] = [](data& data)
		{
			data._r = 0b111;
		};
		_actions[grules.push("r", "B")] = [](data& data)
		{
			data._r = 0b000;
		};
		_actions[grules.push("r", "C")] = [](data& data)
		{
			data._r = 0b001;
		};
		_actions[grules.push("r", "D")] = [](data& data)
		{
			data._r = 0b010;
		};
		_actions[grules.push("r", "E")] = [](data& data)
		{
			data._r = 0b011;
		};
		_actions[grules.push("r", "H")] = [](data& data)
		{
			data._r = 0b100;
		};
		_actions[grules.push("r", "L")] = [](data& data)
		{
			data._r = 0b101;
		};
		_actions[grules.push("r2", "A")] = [](data& data)
		{
			data._r2 = 0b111;
		};
		_actions[grules.push("r2", "B")] = [](data& data)
		{
			data._r2 = 0b000;
		};
		_actions[grules.push("r2", "C")] = [](data& data)
		{
			data._r2 = 0b001;
		};
		_actions[grules.push("r2", "D")] = [](data& data)
		{
			data._r2 = 0b010;
		};
		_actions[grules.push("r2", "E")] = [](data& data)
		{
			data._r2 = 0b011;
		};
		_actions[grules.push("r2", "H")] = [](data& data)
		{
			data._r2 = 0b100;
		};
		_actions[grules.push("r2", "L")] = [](data& data)
		{
			data._r2 = 0b101;
		};
		_actions[grules.push("dd", "BC")] = [](data& data)
		{
			data._dd = 0b00;
		};
		_actions[grules.push("dd", "DE")] = [](data& data)
		{
			data._dd = 0b01;
		};
		_actions[grules.push("dd", "HL")] = [](data& data)
		{
			data._dd = 0b10;
		};
		_actions[grules.push("dd", "SP")] = [](data& data)
		{
			data._dd = 0b11;
		};
		_actions[grules.push("qq", "BC")] = [](data& data)
		{
			data._qq = 0b00;
		};
		_actions[grules.push("qq", "DE")] = [](data& data)
		{
			data._qq = 0b01;
		};
		_actions[grules.push("qq", "HL")] = [](data& data)
		{
			data._qq = 0b10;
		};
		_actions[grules.push("qq", "AF")] = [](data& data)
		{
			data._qq = 0b11;
		};
		_actions[grules.push("ss", "BC")] = [](data& data)
		{
			data._ss = 0b00;
		};
		_actions[grules.push("ss", "DE")] = [](data& data)
		{
			data._ss = 0b01;
		};
		_actions[grules.push("ss", "HL")] = [](data& data)
		{
			data._ss = 0b10;
		};
		_actions[grules.push("ss", "SP")] = [](data& data)
		{
			data._ss = 0b11;
		};
		_actions[grules.push("pp", "BC")] = [](data& data)
		{
			data._pp = 0b11;
		};
		_actions[grules.push("pp", "DE")] = [](data& data)
		{
			data._pp = 0b01;
		};
		_actions[grules.push("pp", "IX")] = [](data& data)
		{
			data._pp = 0b10;
		};
		_actions[grules.push("pp", "SP")] = [](data& data)
		{
			data._pp = 0b11;
		};
		_actions[grules.push("rr", "BC")] = [](data& data)
		{
			data._rr = 0b00;
		};
		_actions[grules.push("rr", "DE")] = [](data& data)
		{
			data._rr = 0b01;
		};
		_actions[grules.push("rr", "IY")] = [](data& data)
		{
			data._rr = 0b10;
		};
		_actions[grules.push("rr", "SP")] = [](data& data)
		{
			data._rr = 0b11;
		};
		_actions[grules.push("cc", "NZ")] = [](data& data)
		{
			data._cc = 0b000;
		};
		_actions[grules.push("cc", "Z")] = [](data& data)
		{
			data._cc = 0b001;
		};
		_actions[grules.push("cc", "NC")] = [](data& data)
		{
			data._cc = 0b010;
		};
		_actions[grules.push("cc", "C")] = [](data& data)
		{
			data._cc = 0b011;
		};
		_actions[grules.push("cc", "PO")] = [](data& data)
		{
			data._cc = 0b100;
		};
		_actions[grules.push("cc", "PE")] = [](data& data)
		{
			data._cc = 0b101;
		};
		_actions[grules.push("cc", "P")] = [](data& data)
		{
			data._cc = 0b110;
		};
		_actions[grules.push("cc", "M")] = [](data& data)
		{
			data._cc = 0b111;
		};

		_actions[grules.push("integer", "Binary")] = [](data& data)
		{
			const auto& t = data.dollar(0);
			char* end = nullptr;

			if (*t.first == '%')
				data._integer = static_cast<uint8_t>(strtol(t.first + 1, &end, 2));
			else
				data._integer = static_cast<uint8_t>(strtol(t.first, &end, 2));
		};
		_actions[grules.push("integer", "Hex")] = [](data& data)
		{
			const auto& t = data.dollar(0);
			char* end = nullptr;

			if (*t.first == '&' || *t.first == '$')
				data._integer = static_cast<uint16_t>(strtol(t.first + 1, &end, 16));
			else
				data._integer = static_cast<uint16_t>(strtol(t.first, &end, 16));
		};
		_actions[grules.push("integer", "Char")] = [](data& data)
		{
			const auto& t = data.dollar(0);

			data._integer = *(t.first + 1);
		};
		_actions[grules.push("integer", "Integer")] = [](data& data)
		{
			const auto& t = data.dollar(0);

			data._integer = atoi(t.first);
		};

		parsertl::generator::build(grules, _gsm, &warnings);

		if (!warnings.empty())
			throw std::runtime_error(warnings);

		lrules.push("[(]", grules.token_id("'('"));
		lrules.push("[)]", grules.token_id("')'"));
		lrules.push("[+]", grules.token_id("'+'"));
		lrules.push(",", grules.token_id("','"));
		lrules.push(":", grules.token_id("':'"));
		lrules.push("[.]?ORG", grules.token_id("ORG"));
		lrules.push("A", grules.token_id("A"));
		lrules.push("ADC", grules.token_id("ADC"));
		lrules.push("ADD", grules.token_id("ADD"));
		lrules.push("AF", grules.token_id("AF"));
		lrules.push("AND", grules.token_id("AND"));
		lrules.push("B", grules.token_id("B"));
		lrules.push("BC", grules.token_id("BC"));
		lrules.push("BIT", grules.token_id("BIT"));
		lrules.push("C", grules.token_id("C"));
		lrules.push("CALL", grules.token_id("CALL"));
		lrules.push("CCF", grules.token_id("CCF"));
		lrules.push("CP", grules.token_id("CP"));
		lrules.push("CPD", grules.token_id("CPD"));
		lrules.push("CPDR", grules.token_id("CPDR"));
		lrules.push("CPI", grules.token_id("CPI"));
		lrules.push("CPIR", grules.token_id("CPIR"));
		lrules.push("CPL", grules.token_id("CPL"));
		lrules.push("D", grules.token_id("D"));
		lrules.push("DB|DEFB", grules.token_id("DB"));
		lrules.push("DW|DEFW", grules.token_id("DW"));
		lrules.push("DAA", grules.token_id("DAA"));
		lrules.push("DE", grules.token_id("DE"));
		lrules.push("DEC", grules.token_id("DEC"));
		lrules.push("DI", grules.token_id("DI"));
		lrules.push("DJNZ", grules.token_id("DJNZ"));
		lrules.push("E", grules.token_id("E"));
		lrules.push("EI", grules.token_id("EI"));
		lrules.push("EX", grules.token_id("EX"));
		lrules.push("EXX", grules.token_id("EXX"));
		lrules.push("H", grules.token_id("H"));
		lrules.push("HALT", grules.token_id("HALT"));
		lrules.push("HL", grules.token_id("HL"));
		lrules.push("I", grules.token_id("I"));
		lrules.push("IM", grules.token_id("IM"));
		lrules.push("IN", grules.token_id("IN"));
		lrules.push("INC", grules.token_id("INC"));
		lrules.push("IND", grules.token_id("IND"));
		lrules.push("INDR", grules.token_id("INDR"));
		lrules.push("INI", grules.token_id("INI"));
		lrules.push("INIR", grules.token_id("INIR"));
		lrules.push("IX", grules.token_id("IX"));
		lrules.push("IY", grules.token_id("IY"));
		lrules.push("JP", grules.token_id("JP"));
		lrules.push("JR", grules.token_id("JR"));
		lrules.push("L", grules.token_id("L"));
		lrules.push("LD", grules.token_id("LD"));
		lrules.push("LDD", grules.token_id("LDD"));
		lrules.push("LDDR", grules.token_id("LDDR"));
		lrules.push("LDI", grules.token_id("LDI"));
		lrules.push("LDIR", grules.token_id("LDIR"));
		lrules.push("M", grules.token_id("M"));
		lrules.push("NC", grules.token_id("NC"));
		lrules.push("NEG", grules.token_id("NEG"));
		lrules.push("NOP", grules.token_id("NOP"));
		lrules.push("NZ", grules.token_id("NZ"));
		lrules.push("OR", grules.token_id("OR"));
		lrules.push("OTDR", grules.token_id("OTDR"));
		lrules.push("OTIR", grules.token_id("OTIR"));
		lrules.push("OUT", grules.token_id("OUT"));
		lrules.push("OUTD", grules.token_id("OUTD"));
		lrules.push("OUTI", grules.token_id("OUTI"));
		lrules.push("P", grules.token_id("P"));
		lrules.push("PE", grules.token_id("PE"));
		lrules.push("PO", grules.token_id("PO"));
		lrules.push("POP", grules.token_id("POP"));
		lrules.push("PUSH", grules.token_id("PUSH"));
		lrules.push("R", grules.token_id("R"));
		lrules.push("RES", grules.token_id("RES"));
		lrules.push("RET", grules.token_id("RET"));
		lrules.push("RETI", grules.token_id("RETI"));
		lrules.push("RETN", grules.token_id("RETN"));
		lrules.push("RL", grules.token_id("RL"));
		lrules.push("RLA", grules.token_id("RLA"));
		lrules.push("RLC", grules.token_id("RLC"));
		lrules.push("RLCA", grules.token_id("RLCA"));
		lrules.push("RLD", grules.token_id("RLD"));
		lrules.push("RR", grules.token_id("RR"));
		lrules.push("RRA", grules.token_id("RRA"));
		lrules.push("RRC", grules.token_id("RRC"));
		lrules.push("RRCA", grules.token_id("RRCA"));
		lrules.push("RRD", grules.token_id("RRD"));
		lrules.push("RST", grules.token_id("RST"));
		lrules.push("SBC", grules.token_id("SBC"));
		lrules.push("SCF", grules.token_id("SCF"));
		lrules.push("SET", grules.token_id("SET"));
		lrules.push("SLA", grules.token_id("SLA"));
		lrules.push("SP", grules.token_id("SP"));
		lrules.push("SRA", grules.token_id("SRA"));
		lrules.push("SRL", grules.token_id("SRL"));
		lrules.push("SUB", grules.token_id("SUB"));
		lrules.push("XOR", grules.token_id("XOR"));
		lrules.push("Z", grules.token_id("Z"));
		lrules.push("'", grules.token_id(R"('\'')"));
		lrules.push("%[01]{8}|[01]{8}b", grules.token_id("Binary"));
		lrules.push("[&$][0-9A-Fa-f]{1,4}|[0-9A-Fa-f]{1,4}h", grules.token_id("Hex"));
		lrules.push("'[^']'", grules.token_id("Char"));
		lrules.push(R"(\d+)", grules.token_id("Integer"));
		lrules.push("'[^']{2,}'", grules.token_id("String"));
		lrules.push("[_A-Z][0-9_A-Z]+", grules.token_id("Name"));
		lrules.push("[ \t]+|;.*|[/][*](?s:.)*?[*][/]", lrules.skip());
		lrules.push("\r?\n", grules.token_id("NL"));
		lexertl::generator::build(lrules, _lsm);
	}

	void parse(const char* first, const char* second)
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
			auto start = std::reverse_iterator<const char *>(iter->first);
			auto bol = std::find(start, std::reverse_iterator<const char *>(first), '\n');

			std::cout << "Parser error, line " <<
				std::count(first, iter->first, '\n') + 1 <<
				" column " << bol - start + 1 << '\n';
		}
		else
		{
			for (const auto& pair : _rel_set)
			{
				for (const auto idx : pair.second)
				{
					auto iter = _labels.find(pair.first);

					if (iter == _labels.end())
					{
						throw std::runtime_error("Cannot find label '" + pair.first + '\'');
					}
					else
					{
						const int i = static_cast<int>(iter->second - (idx + 1));

						if (i < -128 || i > 127)
							throw std::runtime_error("Out of range relative call to '" +
								pair.first + '\'');

						_memory[idx] = static_cast<uint8_t>(i);
					}
				}
			}

			for (const auto& pair : _set)
			{
				for (const auto idx : pair.second)
				{
					auto iter = _labels.find(pair.first);

					if (iter == _labels.end())
					{
						throw std::runtime_error("Cannot find label '" + pair.first + '\'');
					}
					else
					{
						uint16_t address = _org + iter->second;

						_memory[idx] = address & 0xff;
						_memory[idx + 1] = static_cast<uint8_t>(address >> 8);
					}
				}
			}
		}
	}
};

std::ostringstream dump_IX_IY_bits(const uint8_t*& curr, const char xy)
{
	std::ostringstream ret;
	const uint16_t data = static_cast<uint16_t>(*curr);

	++curr;

	switch (*curr)
	{
	case 0x00:
		ret << "RLC (I" << xy << " + " << data << "), B";
		break;
	case 0x01:
		ret << "RLC (I" << xy << " + " << data << "), C";
		break;
	case 0x02:
		ret << "RLC (I" << xy << " + " << data << "), D";
		break;
	case 0x03:
		ret << "RLC (I" << xy << " + " << data << "), E";
		break;
	case 0x04:
		ret << "RLC (I" << xy << " + " << data << "), H";
		break;
	case 0x05:
		ret << "RLC (I" << xy << " + " << data << "), L";
		break;
	case 0x06:
		ret << "RLC (I" << xy << " + " << data << ")";
		break;
	case 0x07:
		ret << "RLC (I" << xy << " + " << data << "), A";
		break;
	case 0x08:
		ret << "RRC (I" << xy << " + " << data << "), B";
		break;
	case 0x09:
		ret << "RRC (I" << xy << " + " << data << "), C";
		break;
	case 0x0a:
		ret << "RRC (I" << xy << " + " << data << "), D";
		break;
	case 0x0b:
		ret << "RRC (I" << xy << " + " << data << "), E";
		break;
	case 0x0c:
		ret << "RRC (I" << xy << " + " << data << "), H";
		break;
	case 0x0d:
		ret << "RRC (I" << xy << " + " << data << "), L";
		break;
	case 0x0e:
		ret << "RRC (I" << xy << " + " << data << ")";
		break;
	case 0x0f:
		ret << "RRC (I" << xy << " + " << data << "), A";
		break;
	case 0x10:
		ret << "RL (I" << xy << " + " << data << "), B";
		break;
	case 0x11:
		ret << "RL (I" << xy << " + " << data << "), C";
		break;
	case 0x12:
		ret << "RL (I" << xy << " + " << data << "), D";
		break;
	case 0x13:
		ret << "RL (I" << xy << " + " << data << "), E";
		break;
	case 0x14:
		ret << "RL (I" << xy << " + " << data << "), H";
		break;
	case 0x15:
		ret << "RL (I" << xy << " + " << data << "), L";
		break;
	case 0x16:
		ret << "RL (I" << xy << " + " << data << ")";
		break;
	case 0x17:
		ret << "RL (I" << xy << " + " << data << "), A";
		break;
	case 0x18:
		ret << "RR (I" << xy << " + " << data << "), B";
		break;
	case 0x19:
		ret << "RR (I" << xy << " + " << data << "), C";
		break;
	case 0x1a:
		ret << "RR (I" << xy << " + " << data << "), D";
		break;
	case 0x1b:
		ret << "RR (I" << xy << " + " << data << "), E";
		break;
	case 0x1c:
		ret << "RR (I" << xy << " + " << data << "), H";
		break;
	case 0x1d:
		ret << "RR (I" << xy << " + " << data << "), L";
		break;
	case 0x1e:
		ret << "RR (I" << xy << " + " << data << ")";
		break;
	case 0x1f:
		ret << "RR (I" << xy << " + " << data << "), A";
		break;
	case 0x20:
		ret << "SLA (I" << xy << " + " << data << "), B";
		break;
	case 0x21:
		ret << "SLA (I" << xy << " + " << data << "), C";
		break;
	case 0x22:
		ret << "SLA (I" << xy << " + " << data << "), D";
		break;
	case 0x23:
		ret << "SLA (I" << xy << " + " << data << "), E";
		break;
	case 0x24:
		ret << "SLA (I" << xy << " + " << data << "), H";
		break;
	case 0x25:
		ret << "SLA (I" << xy << " + " << data << "), L";
		break;
	case 0x26:
		ret << "SLA (I" << xy << " + " << data << ")";
		break;
	case 0x27:
		ret << "SLA (I" << xy << " + " << data << "), A";
		break;
	case 0x28:
		ret << "SRA (I" << xy << " + " << data << "), B";
		break;
	case 0x29:
		ret << "SRA (I" << xy << " + " << data << "), C";
		break;
	case 0x2a:
		ret << "SRA (I" << xy << " + " << data << "), D";
		break;
	case 0x2b:
		ret << "SRA (I" << xy << " + " << data << "), E";
		break;
	case 0x2c:
		ret << "SRA (I" << xy << " + " << data << "), H";
		break;
	case 0x2d:
		ret << "SRA (I" << xy << " + " << data << "), L";
		break;
	case 0x2e:
		ret << "SRA (I" << xy << " + " << data << ")";
		break;
	case 0x2f:
		ret << "SRA (I" << xy << " + " << data << "), A";
		break;
	case 0x30:
		ret << "SLL (I" << xy << " + " << data << "), B";
		break;
	case 0x31:
		ret << "SLL (I" << xy << " + " << data << "), C";
		break;
	case 0x32:
		ret << "SLL (I" << xy << " + " << data << "), D";
		break;
	case 0x33:
		ret << "SLL (I" << xy << " + " << data << "), E";
		break;
	case 0x34:
		ret << "SLL (I" << xy << " + " << data << "), H";
		break;
	case 0x35:
		ret << "SLL (I" << xy << " + " << data << "), L";
		break;
	case 0x36:
		ret << "SLL (I" << xy << " + " << data << ")";
		break;
	case 0x37:
		ret << "SLL (I" << xy << " + " << data << "), A";
		break;
	case 0x38:
		ret << "SRL (I" << xy << " + " << data << "), B";
		break;
	case 0x39:
		ret << "SRL (I" << xy << " + " << data << "), C";
		break;
	case 0x3a:
		ret << "SRL (I" << xy << " + " << data << "), D";
		break;
	case 0x3b:
		ret << "SRL (I" << xy << " + " << data << "), E";
		break;
	case 0x3c:
		ret << "SRL (I" << xy << " + " << data << "), H";
		break;
	case 0x3d:
		ret << "SRL (I" << xy << " + " << data << "), L";
		break;
	case 0x3e:
		ret << "SRL (I" << xy << " + " << data << ")";
		break;
	case 0x3f:
		ret << "SRL (I" << xy << " + " << data << "), A";
		break;
	case 0x40:
	case 0x41:
	case 0x42:
	case 0x43:
	case 0x44:
	case 0x45:
	case 0x46:
	case 0x47:
		ret << "BIT 0, (I" << xy << " + " << data << ")";
		break;
	case 0x48:
	case 0x49:
	case 0x4a:
	case 0x4b:
	case 0x4c:
	case 0x4d:
	case 0x4e:
	case 0x4f:
		ret << "BIT 1, (I" << xy << " + " << data << ")";
		break;
	case 0x50:
	case 0x51:
	case 0x52:
	case 0x53:
	case 0x54:
	case 0x55:
	case 0x56:
	case 0x57:
		ret << "BIT 2, (I" << xy << " + " << data << ")";
		break;
	case 0x58:
	case 0x59:
	case 0x5a:
	case 0x5b:
	case 0x5c:
	case 0x5d:
	case 0x5e:
	case 0x5f:
		ret << "BIT 3, (I" << xy << " + " << data << ")";
		break;
	case 0x60:
	case 0x61:
	case 0x62:
	case 0x63:
	case 0x64:
	case 0x65:
	case 0x66:
	case 0x67:
		ret << "BIT 4, (I" << xy << " + " << data << ")";
		break;
	case 0x68:
	case 0x69:
	case 0x6a:
	case 0x6b:
	case 0x6c:
	case 0x6d:
	case 0x6e:
	case 0x6f:
		ret << "BIT 5, (I" << xy << " + " << data << ")";
		break;
	case 0x70:
	case 0x71:
	case 0x72:
	case 0x73:
	case 0x74:
	case 0x75:
	case 0x76:
	case 0x77:
		ret << "BIT 6, (I" << xy << " + " << data << ")";
		break;
	case 0x78:
	case 0x79:
	case 0x7a:
	case 0x7b:
	case 0x7c:
	case 0x7d:
	case 0x7e:
	case 0x7f:
		ret << "BIT 7, (I" << xy << " + " << data << ")";
		break;
	case 0x80:
		ret << "RES 0, (I" << xy << " + " << data << "), B";
		break;
	case 0x81:
		ret << "RES 0, (I" << xy << " + " << data << "), C";
		break;
	case 0x82:
		ret << "RES 0, (I" << xy << " + " << data << "), D";
		break;
	case 0x83:
		ret << "RES 0, (I" << xy << " + " << data << "), E";
		break;
	case 0x84:
		ret << "RES 0, (I" << xy << " + " << data << "), H";
		break;
	case 0x85:
		ret << "RES 0, (I" << xy << " + " << data << "), L";
		break;
	case 0x86:
		ret << "RES 0, (I" << xy << " + " << data << ")";
		break;
	case 0x87:
		ret << "RES 0, (I" << xy << " + " << data << "), A";
		break;
	case 0x88:
		ret << "RES 1, (I" << xy << " + " << data << "), B";
		break;
	case 0x89:
		ret << "RES 1, (I" << xy << " + " << data << "), C";
		break;
	case 0x8a:
		ret << "RES 1, (I" << xy << " + " << data << "), D";
		break;
	case 0x8b:
		ret << "RES 1, (I" << xy << " + " << data << "), E";
		break;
	case 0x8c:
		ret << "RES 1, (I" << xy << " + " << data << "), H";
		break;
	case 0x8d:
		ret << "RES 1, (I" << xy << " + " << data << "), L";
		break;
	case 0x8e:
		ret << "RES 1, (I" << xy << " + " << data << ")";
		break;
	case 0x8f:
		ret << "RES 1, (I" << xy << " + " << data << "), A";
		break;
	case 0x90:
		ret << "RES 2, (I" << xy << " + " << data << "), B";
		break;
	case 0x91:
		ret << "RES 2, (I" << xy << " + " << data << "), C";
		break;
	case 0x92:
		ret << "RES 2, (I" << xy << " + " << data << "), D";
		break;
	case 0x93:
		ret << "RES 2, (I" << xy << " + " << data << "), E";
		break;
	case 0x94:
		ret << "RES 2, (I" << xy << " + " << data << "), H";
		break;
	case 0x95:
		ret << "RES 2, (I" << xy << " + " << data << "), L";
		break;
	case 0x96:
		ret << "RES 2, (I" << xy << " + " << data << ")";
		break;
	case 0x97:
		ret << "RES 2, (I" << xy << " + " << data << "), A";
		break;
	case 0x98:
		ret << "RES 3, (I" << xy << " + " << data << "), B";
		break;
	case 0x99:
		ret << "RES 3, (I" << xy << " + " << data << "), C";
		break;
	case 0x9a:
		ret << "RES 3, (I" << xy << " + " << data << "), D";
		break;
	case 0x9b:
		ret << "RES 3, (I" << xy << " + " << data << "), E";
		break;
	case 0x9c:
		ret << "RES 3, (I" << xy << " + " << data << "), H";
		break;
	case 0x9d:
		ret << "RES 3, (I" << xy << " + " << data << "), L";
		break;
	case 0x9e:
		ret << "RES 3, (I" << xy << " + " << data << ")";
		break;
	case 0x9f:
		ret << "RES 3, (I" << xy << " + " << data << "), A";
		break;
	case 0xa0:
		ret << "RES 4, (I" << xy << " + " << data << "), B";
		break;
	case 0xa1:
		ret << "RES 4, (I" << xy << " + " << data << "), C";
		break;
	case 0xa2:
		ret << "RES 4, (I" << xy << " + " << data << "), D";
		break;
	case 0xa3:
		ret << "RES 4, (I" << xy << " + " << data << "), E";
		break;
	case 0xa4:
		ret << "RES 4, (I" << xy << " + " << data << "), H";
		break;
	case 0xa5:
		ret << "RES 4, (I" << xy << " + " << data << "), L";
		break;
	case 0xa6:
		ret << "RES 4, (I" << xy << " + " << data << ")";
		break;
	case 0xa7:
		ret << "RES 4, (I" << xy << " + " << data << "), A";
		break;
	case 0xa8:
		ret << "RES 5, (I" << xy << " + " << data << "), B";
		break;
	case 0xa9:
		ret << "RES 5, (I" << xy << " + " << data << "), C";
		break;
	case 0xaa:
		ret << "RES 5, (I" << xy << " + " << data << "), D";
		break;
	case 0xab:
		ret << "RES 5, (I" << xy << " + " << data << "), E";
		break;
	case 0xac:
		ret << "RES 5, (I" << xy << " + " << data << "), H";
		break;
	case 0xad:
		ret << "RES 5, (I" << xy << " + " << data << "), L";
		break;
	case 0xae:
		ret << "RES 5, (I" << xy << " + " << data << ")";
		break;
	case 0xaf:
		ret << "RES 5, (I" << xy << " + " << data << "), A";
		break;
	case 0xb0:
		ret << "RES 6, (I" << xy << " + " << data << "), B";
		break;
	case 0xb1:
		ret << "RES 6, (I" << xy << " + " << data << "), C";
		break;
	case 0xb2:
		ret << "RES 6, (I" << xy << " + " << data << "), D";
		break;
	case 0xb3:
		ret << "RES 6, (I" << xy << " + " << data << "), E";
		break;
	case 0xb4:
		ret << "RES 6, (I" << xy << " + " << data << "), H";
		break;
	case 0xb5:
		ret << "RES 6, (I" << xy << " + " << data << "), L";
		break;
	case 0xb6:
		ret << "RES 6, (I" << xy << " + " << data << ")";
		break;
	case 0xb7:
		ret << "RES 6, (I" << xy << " + " << data << "), A";
		break;
	case 0xb8:
		ret << "RES 7, (I" << xy << " + " << data << "), B";
		break;
	case 0xb9:
		ret << "RES 7, (I" << xy << " + " << data << "), C";
		break;
	case 0xba:
		ret << "RES 7, (I" << xy << " + " << data << "), D";
		break;
	case 0xbb:
		ret << "RES 7, (I" << xy << " + " << data << "), E";
		break;
	case 0xbc:
		ret << "RES 7, (I" << xy << " + " << data << "), H";
		break;
	case 0xbd:
		ret << "RES 7, (I" << xy << " + " << data << "), L";
		break;
	case 0xbe:
		ret << "RES 7, (I" << xy << " + " << data << ")";
		break;
	case 0xbf:
		ret << "RES 7, (I" << xy << " + " << data << "), A";
		break;
	case 0xc0:
		ret << "SET 0, (I" << xy << " + " << data << "), B";
		break;
	case 0xc1:
		ret << "SET 0, (I" << xy << " + " << data << "), C";
		break;
	case 0xc2:
		ret << "SET 0, (I" << xy << " + " << data << "), D";
		break;
	case 0xc3:
		ret << "SET 0, (I" << xy << " + " << data << "), E";
		break;
	case 0xc4:
		ret << "SET 0, (I" << xy << " + " << data << "), H";
		break;
	case 0xc5:
		ret << "SET 0, (I" << xy << " + " << data << "), L";
		break;
	case 0xc6:
		ret << "SET 0, (I" << xy << " + " << data << ")";
		break;
	case 0xc7:
		ret << "SET 0, (I" << xy << " + " << data << "), A";
		break;
	case 0xc8:
		ret << "SET 1, (I" << xy << " + " << data << "), B";
		break;
	case 0xc9:
		ret << "SET 1, (I" << xy << " + " << data << "), C";
		break;
	case 0xca:
		ret << "SET 1, (I" << xy << " + " << data << "), D";
		break;
	case 0xcb:
		ret << "SET 1, (I" << xy << " + " << data << "), E";
		break;
	case 0xcc:
		ret << "SET 1, (I" << xy << " + " << data << "), H";
		break;
	case 0xcd:
		ret << "SET 1, (I" << xy << " + " << data << "), L";
		break;
	case 0xce:
		ret << "SET 1, (I" << xy << " + " << data << ")";
		break;
	case 0xcf:
		ret << "SET 1, (I" << xy << " + " << data << "), A";
		break;
	case 0xd0:
		ret << "SET 2, (I" << xy << " + " << data << "), B";
		break;
	case 0xd1:
		ret << "SET 2, (I" << xy << " + " << data << "), C";
		break;
	case 0xd2:
		ret << "SET 2, (I" << xy << " + " << data << "), D";
		break;
	case 0xd3:
		ret << "SET 2, (I" << xy << " + " << data << "), E";
		break;
	case 0xd4:
		ret << "SET 2, (I" << xy << " + " << data << "), H";
		break;
	case 0xd5:
		ret << "SET 2, (I" << xy << " + " << data << "), L";
		break;
	case 0xd6:
		ret << "SET 2, (I" << xy << " + " << data << ")";
		break;
	case 0xd7:
		ret << "SET 2, (I" << xy << " + " << data << "), A";
		break;
	case 0xd8:
		ret << "SET 3, (I" << xy << " + " << data << "), B";
		break;
	case 0xd9:
		ret << "SET 3, (I" << xy << " + " << data << "), C";
		break;
	case 0xda:
		ret << "SET 3, (I" << xy << " + " << data << "), D";
		break;
	case 0xdb:
		ret << "SET 3, (I" << xy << " + " << data << "), E";
		break;
	case 0xdc:
		ret << "SET 3, (I" << xy << " + " << data << "), H";
		break;
	case 0xdd:
		ret << "SET 3, (I" << xy << " + " << data << "), L";
		break;
	case 0xde:
		ret << "SET 3, (I" << xy << " + " << data << ")";
		break;
	case 0xdf:
		ret << "SET 3, (I" << xy << " + " << data << "), A";
		break;
	case 0xe0:
		ret << "SET 4, (I" << xy << " + " << data << "), B";
		break;
	case 0xe1:
		ret << "SET 4, (I" << xy << " + " << data << "), C";
		break;
	case 0xe2:
		ret << "SET 4, (I" << xy << " + " << data << "), D";
		break;
	case 0xe3:
		ret << "SET 4, (I" << xy << " + " << data << "), E";
		break;
	case 0xe4:
		ret << "SET 4, (I" << xy << " + " << data << "), H";
		break;
	case 0xe5:
		ret << "SET 4, (I" << xy << " + " << data << "), L";
		break;
	case 0xe6:
		ret << "SET 4, (I" << xy << " + " << data << ")";
		break;
	case 0xe7:
		ret << "SET 4, (I" << xy << " + " << data << "), A";
		break;
	case 0xe8:
		ret << "SET 5, (I" << xy << " + " << data << "), B";
		break;
	case 0xe9:
		ret << "SET 5, (I" << xy << " + " << data << "), C";
		break;
	case 0xea:
		ret << "SET 5, (I" << xy << " + " << data << "), D";
		break;
	case 0xeb:
		ret << "SET 5, (I" << xy << " + " << data << "), E";
		break;
	case 0xec:
		ret << "SET 5, (I" << xy << " + " << data << "), H";
		break;
	case 0xed:
		ret << "SET 5, (I" << xy << " + " << data << "), L";
		break;
	case 0xee:
		ret << "SET 5, (I" << xy << " + " << data << ")";
		break;
	case 0xef:
		ret << "SET 5, (I" << xy << " + " << data << "), A";
		break;
	case 0xf0:
		ret << "SET 6, (I" << xy << " + " << data << "), B";
		break;
	case 0xf1:
		ret << "SET 6, (I" << xy << " + " << data << "), C";
		break;
	case 0xf2:
		ret << "SET 6, (I" << xy << " + " << data << "), D";
		break;
	case 0xf3:
		ret << "SET 6, (I" << xy << " + " << data << "), E";
		break;
	case 0xf4:
		ret << "SET 6, (I" << xy << " + " << data << "), H";
		break;
	case 0xf5:
		ret << "SET 6, (I" << xy << " + " << data << "), L";
		break;
	case 0xf6:
		ret << "SET 6, (I" << xy << " + " << data << ")";
		break;
	case 0xf7:
		ret << "SET 6, (I" << xy << " + " << data << "), A";
		break;
	case 0xf8:
		ret << "SET 7, (I" << xy << " + " << data << "), B";
		break;
	case 0xf9:
		ret << "SET 7, (I" << xy << " + " << data << "), C";
		break;
	case 0xfa:
		ret << "SET 7, (I" << xy << " + " << data << "), D";
		break;
	case 0xfb:
		ret << "SET 7, (I" << xy << " + " << data << "), E";
		break;
	case 0xfc:
		ret << "SET 7, (I" << xy << " + " << data << "), H";
		break;
	case 0xfd:
		ret << "SET 7, (I" << xy << " + " << data << "), L";
		break;
	case 0xfe:
		ret << "SET 7, (I" << xy << " + " << data << ")";
		break;
	case 0xff:
		ret << "SET 7, (I" << xy << " + " << data << "), A";
		break;
	}

	++curr;
	return ret;
}

std::string dump_IX_IY(const uint8_t*& curr, const char xy)
{
	std::ostringstream ret;

	switch (*curr)
	{
	case 0x09:
		ret << "ADD I" << xy << ", BC";
		++curr;
		break;
	case 0x19:
		ret << "ADD I" << xy << ", DE";
		++curr;
		break;
	case 0x21:
		ret << "LD I" << xy << ", ";
		++curr;
		ret << *reinterpret_cast<const uint16_t*>(curr);
		curr += 2;
		break;
	case 0x22:
		ret << "LD (";
		++curr;
		ret << *reinterpret_cast<const uint16_t*>(curr);
		curr += 2;
		ret << "), I" << xy;
		break;
	case 0x23:
		ret << "INC I" << xy;
		++curr;
		break;
	case 0x24:
		ret << "INC I" << xy << 'H';
		++curr;
		break;
	case 0x25:
		ret << "DEC I" << xy << 'H';
		++curr;
		break;
	case 0x26:
		ret << "LD I" << xy << "H, ";
		++curr;
		ret << static_cast<uint16_t>(*curr);
		++curr;
		break;
	case 0x29:
		ret << "ADD I" << xy << ", I" << xy;
		++curr;
		break;
	case 0x2a:
		ret << "LD I" << xy << ", (";
		++curr;
		ret << *reinterpret_cast<const uint16_t*>(curr);
		curr += 2;
		ret << ')';
		break;
	case 0x2b:
		ret << "DEC I" << xy;
		++curr;
		break;
	case 0x2c:
		ret << "INC I" << xy << 'L';
		++curr;
		break;
	case 0x2d:
		ret << "DEC I" << xy << 'L';
		++curr;
		break;
	case 0x2e:
		ret << "LD I" << xy << "L, ";
		++curr;
		ret << static_cast<uint16_t>(*curr);
		++curr;
		break;
	case 0x34:
		ret << "INC (I" << xy << " + ";
		++curr;
		ret << static_cast<uint16_t>(*curr);
		++curr;
		ret << ')';
		break;
	case 0x35:
		ret << "DEC (I" << xy << " + ";
		++curr;
		ret << static_cast<uint16_t>(*curr);
		++curr;
		ret << ')';
		break;
	case 0x36:
		ret << "LD (I" << xy << " + ";
		++curr;
		ret << static_cast<uint16_t>(*curr);
		ret << "), ";
		++curr;
		ret << static_cast<uint16_t>(*curr);
		++curr;
		break;
	case 0x39:
		ret << "ADD I" << xy << ", SP";
		++curr;
		break;
	case 0x44:
		ret << "LD B, I" << xy << 'H';
		++curr;
		break;
	case 0x45:
		ret << "LD B, I" << xy << 'L';
		++curr;
		break;
	case 0x46:
		ret << "LD B, (I" << xy << " + ";
		++curr;
		ret << static_cast<uint16_t>(*curr);
		ret << ')';
		break;
	case 0x4c:
		ret << "LD C, I" << xy << 'H';
		++curr;
		break;
	case 0x4d:
		ret << "LD C, I" << xy << 'L';
		++curr;
		break;
	case 0x4e:
		ret << "LD C, (I" << xy << " + ";
		++curr;
		ret << static_cast<uint16_t>(*curr);
		ret << ')';
		break;
	case 0x54:
		ret << "LD D, I" << xy << 'H';
		++curr;
		break;
	case 0x55:
		ret << "LD D, I" << xy << 'L';
		++curr;
		break;
	case 0x56:
		ret << "LD D, (I" << xy << " + ";
		++curr;
		ret << static_cast<uint16_t>(*curr);
		ret << ')';
		break;
	case 0x5c:
		ret << "LD E, I" << xy << 'H';
		++curr;
		break;
	case 0x5d:
		ret << "LD E, I" << xy << 'L';
		++curr;
		break;
	case 0x5e:
		ret << "LD E, (I" << xy << " + ";
		++curr;
		ret << static_cast<uint16_t>(*curr);
		++curr;
		ret << ')';
		break;
	case 0x60:
		ret << "LD I" << xy << "H, B";
		++curr;
		break;
	case 0x61:
		ret << "LD I" << xy << "H, C";
		++curr;
		break;
	case 0x62:
		ret << "LD I" << xy << "H, D";
		++curr;
		break;
	case 0x63:
		ret << "LD I" << xy << "H, E";
		++curr;
		break;
	case 0x64:
		ret << "LD I" << xy << "H, I" << xy << 'H';
		++curr;
		break;
	case 0x65:
		ret << "LD I" << xy << "H, I" << xy << 'L';
		++curr;
		break;
	case 0x66:
		ret << "LD H, (I" << xy << " + ";
		++curr;
		ret << static_cast<uint16_t>(*curr);
		++curr;
		ret << ')';
		break;
	case 0x67:
		ret << "LD I" << xy << "H, A";
		++curr;
		break;
	case 0x68:
		ret << "LD I" << xy << "L, B";
		++curr;
		break;
	case 0x69:
		ret << "LD I" << xy << "L, C";
		++curr;
		break;
	case 0x6a:
		ret << "LD I" << xy << "L, D";
		++curr;
		break;
	case 0x6b:
		ret << "LD I" << xy << "L, E";
		++curr;
		break;
	case 0x6c:
		ret << "LD I" << xy << "L, I" << xy << 'H';
		++curr;
		break;
	case 0x6d:
		ret << "LD I" << xy << "L, I" << xy << 'L';
		++curr;
		break;
	case 0x6e:
		ret << "LD L, (I" << xy << " + ";
		++curr;
		ret << static_cast<uint16_t>(*curr);
		++curr;
		ret << ')';
		break;
	case 0x6f:
		ret << "LD I" << xy << "L, A";
		++curr;
		break;
	case 0x70:
		ret << "LD (I" << xy << " + ";
		++curr;
		ret << static_cast<uint16_t>(*curr);
		++curr;
		ret << "), B";
		break;
	case 0x71:
		ret << "LD (I" << xy << " + ";
		++curr;
		ret << static_cast<uint16_t>(*curr);
		++curr;
		ret << "), C";
		break;
	case 0x72:
		ret << "LD (I" << xy << " + ";
		++curr;
		ret << static_cast<uint16_t>(*curr);
		++curr;
		ret << "), D";
		break;
	case 0x73:
		ret << "LD (I" << xy << " + ";
		++curr;
		ret << static_cast<uint16_t>(*curr);
		++curr;
		ret << "), E";
		break;
	case 0x74:
		ret << "LD (I" << xy << " + ";
		++curr;
		ret << static_cast<uint16_t>(*curr);
		++curr;
		ret << "), H";
		break;
	case 0x75:
		ret << "LD (I" << xy << " + ";
		++curr;
		ret << static_cast<uint16_t>(*curr);
		++curr;
		ret << "), L";
		break;
	case 0x77:
		ret << "LD (I" << xy << " + ";
		++curr;
		ret << static_cast<uint16_t>(*curr);
		++curr;
		ret << "), A";
		break;
	case 0x7c:
		ret << "LD A, I" << xy << 'H';
		++curr;
		break;
	case 0x7d:
		ret << "LD A, I" << xy << 'L';
		++curr;
		break;
	case 0x7e:
		ret << "LD A, (I" << xy << " + ";
		++curr;
		ret << static_cast<uint16_t>(*curr);
		++curr;
		ret << ')';
		break;
	case 0x84:
		ret << "ADD A, I" << xy << 'H';
		++curr;
		break;
	case 0x85:
		ret << "ADD A, I" << xy << 'L';
		++curr;
		break;
	case 0x86:
		ret << "ADD A, (I" << xy << " + " <<
			static_cast<uint16_t>(*curr) << ')';
		curr += 2;
		break;
	case 0x8c:
		ret << "ADC A, I" << xy << 'H';
		++curr;
		break;
	case 0x8d:
		ret << "ADC A, I" << xy << 'L';
		++curr;
		break;
	case 0x8e:
		ret << "ADC A, (I" << xy << " + " <<
			static_cast<uint16_t>(*curr) << ')';
		++curr;
		break;
	case 0x94:
		ret << "SUB I" << xy << 'H';
		++curr;
		break;
	case 0x95:
		ret << "SUB I" << xy << 'L';
		++curr;
		break;
	case 0x96:
		ret << "SUB (I" << xy << " + " <<
			static_cast<uint16_t>(*curr) << ')';
		curr += 2;
		break;
	case 0x9c:
		ret << "SBC A, I" << xy << 'H';
		++curr;
		break;
	case 0x9d:
		ret << "SBC A, I" << xy << 'L';
		++curr;
		break;
	case 0x9e:
		ret << "SBC A, (I" << xy << " + " <<
			static_cast<uint16_t>(*curr) << ')';
		curr += 2;
		break;
	case 0xa4:
		ret << "AND I" << xy << 'H';
		++curr;
		break;
	case 0xa5:
		ret << "AND I" << xy << 'L';
		++curr;
		break;
	case 0xa6:
		ret << "AND (I" << xy << " + " <<
			static_cast<uint16_t>(*curr) << ')';
		curr += 2;
		break;
	case 0xac:
		ret << "XOR I" << xy << 'H';
		++curr;
		break;
	case 0xad:
		ret << "XOR I" << xy << 'L';
		++curr;
		break;
	case 0xae:
		ret << "XOR (I" << xy << " + " <<
			static_cast<uint16_t>(*curr) << ')';
		curr += 2;
		break;
	case 0xb4:
		ret << "OR I" << xy << 'H';
		++curr;
		break;
	case 0xb5:
		ret << "OR I" << xy << 'L';
		++curr;
		break;
	case 0xb6:
		ret << "OR (I" << xy << " + " <<
			static_cast<uint16_t>(*curr) << ')';
		curr += 2;
		break;
	case 0xbc:
		ret << "CP I" << xy << 'H';
		++curr;
		break;
	case 0xbd:
		ret << "CP I" << xy << 'L';
		++curr;
		break;
	case 0xbe:
		ret << "CP (I" << xy << " + " <<
			static_cast<uint16_t>(*curr) << ')';
		curr += 2;
		break;
	case 0xcb:
		dump_IX_IY_bits(++curr, xy);
		break;
	case 0xe1:
		ret << "POP I" << xy;
		++curr;
		break;
	case 0xe3:
		ret << "EX (SP), I" << xy;
		++curr;
		break;
	case 0xe5:
		ret << "PUSH I" << xy;
		++curr;
		break;
	case 0xe9:
		ret << "JP (I" << xy << ')';
		++curr;
		break;
	case 0xf9:
		ret << "LD SP, I" << xy;
		++curr;
		break;
	default:
		ret << "db " << static_cast<uint16_t>(*curr);
		break;
	}

	return ret.str();
}

std::string dump_bits(const uint8_t*& curr)
{
	std::ostringstream ret;

	switch (*curr)
	{
	case 0x00:
		ret << "RLC B";
		break;
	case 0x01:
		ret << "RLC C";
		break;
	case 0x02:
		ret << "RLC D";
		break;
	case 0x03:
		ret << "RLC E";
		break;
	case 0x04:
		ret << "RLC H";
		break;
	case 0x05:
		ret << "RLC L";
		break;
	case 0x06:
		ret << "RLC (HL)";
		break;
	case 0x07:
		ret << "RLC A";
		break;
	case 0x08:
		ret << "RRC B";
		break;
	case 0x09:
		ret << "RRC C";
		break;
	case 0x0a:
		ret << "RRC D";
		break;
	case 0x0b:
		ret << "RRC E";
		break;
	case 0x0c:
		ret << "RRC H";
		break;
	case 0x0d:
		ret << "RRC L";
		break;
	case 0x0e:
		ret << "RRC (HL)";
		break;
	case 0x0f:
		ret << "RRC A";
		break;
	case 0x10:
		ret << "RL B";
		break;
	case 0x11:
		ret << "RL C";
		break;
	case 0x12:
		ret << "RL D";
		break;
	case 0x13:
		ret << "RL E";
		break;
	case 0x14:
		ret << "RL H";
		break;
	case 0x15:
		ret << "RL L";
		break;
	case 0x16:
		ret << "RL (HL)";
		break;
	case 0x17:
		ret << "RL A";
		break;
	case 0x18:
		ret << "RR B";
		break;
	case 0x19:
		ret << "RR C";
		break;
	case 0x1a:
		ret << "RR D";
		break;
	case 0x1b:
		ret << "RR E";
		break;
	case 0x1c:
		ret << "RR H";
		break;
	case 0x1d:
		ret << "RR L";
		break;
	case 0x1e:
		ret << "RR (HL)";
		break;
	case 0x1f:
		ret << "RR A";
		break;
	case 0x20:
		ret << "SLA B";
		break;
	case 0x21:
		ret << "SLA C";
		break;
	case 0x22:
		ret << "SLA D";
		break;
	case 0x23:
		ret << "SLA E";
		break;
	case 0x24:
		ret << "SLA H";
		break;
	case 0x25:
		ret << "SLA L";
		break;
	case 0x26:
		ret << "SLA (HL)";
		break;
	case 0x27:
		ret << "SLA A";
		break;
	case 0x28:
		ret << "SRA B";
		break;
	case 0x29:
		ret << "SRA C";
		break;
	case 0x2a:
		ret << "SRA D";
		break;
	case 0x2b:
		ret << "SRA E";
		break;
	case 0x2c:
		ret << "SRA H";
		break;
	case 0x2d:
		ret << "SRA L";
		break;
	case 0x2e:
		ret << "SRA (HL)";
		break;
	case 0x2f:
		ret << "SRA A";
		break;
	case 0x30:
		ret << "SLL B";
		break;
	case 0x31:
		ret << "SLL C";
		break;
	case 0x32:
		ret << "SLL D";
		break;
	case 0x33:
		ret << "SLL E";
		break;
	case 0x34:
		ret << "SLL H";
		break;
	case 0x35:
		ret << "SLL L";
		break;
	case 0x36:
		ret << "SLL (HL)";
		break;
	case 0x37:
		ret << "SLL A";
		break;
	case 0x38:
		ret << "SRL B";
		break;
	case 0x39:
		ret << "SRL C";
		break;
	case 0x3a:
		ret << "SRL D";
		break;
	case 0x3b:
		ret << "SRL E";
		break;
	case 0x3c:
		ret << "SRL H";
		break;
	case 0x3d:
		ret << "SRL L";
		break;
	case 0x3e:
		ret << "SRL (HL)";
		break;
	case 0x3f:
		ret << "SRL A";
		break;
	case 0x40:
		ret << "BIT 0, B";
		break;
	case 0x41:
		ret << "BIT 0, C";
		break;
	case 0x42:
		ret << "BIT 0, D";
		break;
	case 0x43:
		ret << "BIT 0, E";
		break;
	case 0x44:
		ret << "BIT 0, H";
		break;
	case 0x45:
		ret << "BIT 0, L";
		break;
	case 0x46:
		ret << "BIT 0, (HL)";
		break;
	case 0x47:
		ret << "BIT 0, A";
		break;
	case 0x48:
		ret << "BIT 1, B";
		break;
	case 0x49:
		ret << "BIT 1, C";
		break;
	case 0x4a:
		ret << "BIT 1, D";
		break;
	case 0x4b:
		ret << "BIT 1, E";
		break;
	case 0x4c:
		ret << "BIT 1, H";
		break;
	case 0x4d:
		ret << "BIT 1, L";
		break;
	case 0x4e:
		ret << "BIT 1, (HL)";
		break;
	case 0x4f:
		ret << "BIT 1, A";
		break;
	case 0x50:
		ret << "BIT 2, B";
		break;
	case 0x51:
		ret << "BIT 2, C";
		break;
	case 0x52:
		ret << "BIT 2, D";
		break;
	case 0x53:
		ret << "BIT 2, E";
		break;
	case 0x54:
		ret << "BIT 2, H";
		break;
	case 0x55:
		ret << "BIT 2, L";
		break;
	case 0x56:
		ret << "BIT 2, (HL)";
		break;
	case 0x57:
		ret << "BIT 2, A";
		break;
	case 0x58:
		ret << "BIT 3, B";
		break;
	case 0x59:
		ret << "BIT 3, C";
		break;
	case 0x5a:
		ret << "BIT 3, D";
		break;
	case 0x5b:
		ret << "BIT 3, E";
		break;
	case 0x5c:
		ret << "BIT 3, H";
		break;
	case 0x5d:
		ret << "BIT 3, L";
		break;
	case 0x5e:
		ret << "BIT 3, (HL)";
		break;
	case 0x5f:
		ret << "BIT 3, A";
		break;
	case 0x60:
		ret << "BIT 4, B";
		break;
	case 0x61:
		ret << "BIT 4, C";
		break;
	case 0x62:
		ret << "BIT 4, D";
		break;
	case 0x63:
		ret << "BIT 4, E";
		break;
	case 0x64:
		ret << "BIT 4, H";
		break;
	case 0x65:
		ret << "BIT 4, L";
		break;
	case 0x66:
		ret << "BIT 4, (HL)";
		break;
	case 0x67:
		ret << "BIT 4, A";
		break;
	case 0x68:
		ret << "BIT 5, B";
		break;
	case 0x69:
		ret << "BIT 5, C";
		break;
	case 0x6a:
		ret << "BIT 5, D";
		break;
	case 0x6b:
		ret << "BIT 5, E";
		break;
	case 0x6c:
		ret << "BIT 5, H";
		break;
	case 0x6d:
		ret << "BIT 5, L";
		break;
	case 0x6e:
		ret << "BIT 5, (HL)";
		break;
	case 0x6f:
		ret << "BIT 5, A";
		break;
	case 0x70:
		ret << "BIT 6, B";
		break;
	case 0x71:
		ret << "BIT 6, C";
		break;
	case 0x72:
		ret << "BIT 6, D";
		break;
	case 0x73:
		ret << "BIT 6, E";
		break;
	case 0x74:
		ret << "BIT 6, H";
		break;
	case 0x75:
		ret << "BIT 6, L";
		break;
	case 0x76:
		ret << "BIT 6, (HL)";
		break;
	case 0x77:
		ret << "BIT 6, A";
		break;
	case 0x78:
		ret << "BIT 7, B";
		break;
	case 0x79:
		ret << "BIT 7, C";
		break;
	case 0x7a:
		ret << "BIT 7, D";
		break;
	case 0x7b:
		ret << "BIT 7, E";
		break;
	case 0x7c:
		ret << "BIT 7, H";
		break;
	case 0x7d:
		ret << "BIT 7, L";
		break;
	case 0x7e:
		ret << "BIT 7, (HL)";
		break;
	case 0x7f:
		ret << "BIT 7, A";
		break;
	case 0x80:
		ret << "RES 0, B";
		break;
	case 0x81:
		ret << "RES 0, C";
		break;
	case 0x82:
		ret << "RES 0, D";
		break;
	case 0x83:
		ret << "RES 0, E";
		break;
	case 0x84:
		ret << "RES 0, H";
		break;
	case 0x85:
		ret << "RES 0, L";
		break;
	case 0x86:
		ret << "RES 0, (HL)";
		break;
	case 0x87:
		ret << "RES 0, A";
		break;
	case 0x88:
		ret << "RES 1, B";
		break;
	case 0x89:
		ret << "RES 1, C";
		break;
	case 0x8a:
		ret << "RES 1, D";
		break;
	case 0x8b:
		ret << "RES 1, E";
		break;
	case 0x8c:
		ret << "RES 1, H";
		break;
	case 0x8d:
		ret << "RES 1, L";
		break;
	case 0x8e:
		ret << "RES 1, (HL)";
		break;
	case 0x8f:
		ret << "RES 1, A";
		break;
	case 0x90:
		ret << "RES 2, B";
		break;
	case 0x91:
		ret << "RES 2, C";
		break;
	case 0x92:
		ret << "RES 2, D";
		break;
	case 0x93:
		ret << "RES 2, E";
		break;
	case 0x94:
		ret << "RES 2, H";
		break;
	case 0x95:
		ret << "RES 2, L";
		break;
	case 0x96:
		ret << "RES 2, (HL)";
		break;
	case 0x97:
		ret << "RES 2, A";
		break;
	case 0x98:
		ret << "RES 3, B";
		break;
	case 0x99:
		ret << "RES 3, C";
		break;
	case 0x9a:
		ret << "RES 3, D";
		break;
	case 0x9b:
		ret << "RES 3, E";
		break;
	case 0x9c:
		ret << "RES 3, H";
		break;
	case 0x9d:
		ret << "RES 3, L";
		break;
	case 0x9e:
		ret << "RES 3, (HL)";
		break;
	case 0x9f:
		ret << "RES 3, A";
		break;
	case 0xa0:
		ret << "RES 4, B";
		break;
	case 0xa1:
		ret << "RES 4, C";
		break;
	case 0xa2:
		ret << "RES 4, D";
		break;
	case 0xa3:
		ret << "RES 4, E";
		break;
	case 0xa4:
		ret << "RES 4, H";
		break;
	case 0xa5:
		ret << "RES 4, L";
		break;
	case 0xa6:
		ret << "RES 4, (HL)";
		break;
	case 0xa7:
		ret << "RES 4, A";
		break;
	case 0xa8:
		ret << "RES 5, B";
		break;
	case 0xa9:
		ret << "RES 5, C";
		break;
	case 0xaa:
		ret << "RES 5, D";
		break;
	case 0xab:
		ret << "RES 5, E";
		break;
	case 0xac:
		ret << "RES 5, H";
		break;
	case 0xad:
		ret << "RES 5, L";
		break;
	case 0xae:
		ret << "RES 5, (HL)";
		break;
	case 0xaf:
		ret << "RES 5, A";
		break;
	case 0xb0:
		ret << "RES 6, B";
		break;
	case 0xb1:
		ret << "RES 6, C";
		break;
	case 0xb2:
		ret << "RES 6, D";
		break;
	case 0xb3:
		ret << "RES 6, E";
		break;
	case 0xb4:
		ret << "RES 6, H";
		break;
	case 0xb5:
		ret << "RES 6, L";
		break;
	case 0xb6:
		ret << "RES 6, (HL)";
		break;
	case 0xb7:
		ret << "RES 6, A";
		break;
	case 0xb8:
		ret << "RES 7, B";
		break;
	case 0xb9:
		ret << "RES 7, C";
		break;
	case 0xba:
		ret << "RES 7, D";
		break;
	case 0xbb:
		ret << "RES 7, E";
		break;
	case 0xbc:
		ret << "RES 7, H";
		break;
	case 0xbd:
		ret << "RES 7, L";
		break;
	case 0xbe:
		ret << "RES 7, (HL)";
		break;
	case 0xbf:
		ret << "RES 7, A";
		break;
	case 0xc0:
		ret << "SET 0, B";
		break;
	case 0xc1:
		ret << "SET 0, C";
		break;
	case 0xc2:
		ret << "SET 0, D";
		break;
	case 0xc3:
		ret << "SET 0, E";
		break;
	case 0xc4:
		ret << "SET 0, H";
		break;
	case 0xc5:
		ret << "SET 0, L";
		break;
	case 0xc6:
		ret << "SET 0, (HL)";
		break;
	case 0xc7:
		ret << "SET 0, A";
		break;
	case 0xc8:
		ret << "SET 1, B";
		break;
	case 0xc9:
		ret << "SET 1, C";
		break;
	case 0xca:
		ret << "SET 1, D";
		break;
	case 0xcb:
		ret << "SET 1, E";
		break;
	case 0xcc:
		ret << "SET 1, H";
		break;
	case 0xcd:
		ret << "SET 1, L";
		break;
	case 0xce:
		ret << "SET 1, (HL)";
		break;
	case 0xcf:
		ret << "SET 1, A";
		break;
	case 0xd0:
		ret << "SET 2, B";
		break;
	case 0xd1:
		ret << "SET 2, C";
		break;
	case 0xd2:
		ret << "SET 2, D";
		break;
	case 0xd3:
		ret << "SET 2, E";
		break;
	case 0xd4:
		ret << "SET 2, H";
		break;
	case 0xd5:
		ret << "SET 2, L";
		break;
	case 0xd6:
		ret << "SET 2, (HL)";
		break;
	case 0xd7:
		ret << "SET 2, A";
		break;
	case 0xd8:
		ret << "SET 3, B";
		break;
	case 0xd9:
		ret << "SET 3, C";
		break;
	case 0xda:
		ret << "SET 3, D";
		break;
	case 0xdb:
		ret << "SET 3, E";
		break;
	case 0xdc:
		ret << "SET 3, H";
		break;
	case 0xdd:
		ret << "SET 3, L";
		break;
	case 0xde:
		ret << "SET 3, (HL)";
		break;
	case 0xdf:
		ret << "SET 3, A";
		break;
	case 0xe0:
		ret << "SET 4, B";
		break;
	case 0xe1:
		ret << "SET 4, C";
		break;
	case 0xe2:
		ret << "SET 4, D";
		break;
	case 0xe3:
		ret << "SET 4, E";
		break;
	case 0xe4:
		ret << "SET 4, H";
		break;
	case 0xe5:
		ret << "SET 4, L";
		break;
	case 0xe6:
		ret << "SET 4, (HL)";
		break;
	case 0xe7:
		ret << "SET 4, A";
		break;
	case 0xe8:
		ret << "SET 5, B";
		break;
	case 0xe9:
		ret << "SET 5, C";
		break;
	case 0xea:
		ret << "SET 5, D";
		break;
	case 0xeb:
		ret << "SET 5, E";
		break;
	case 0xec:
		ret << "SET 5, H";
		break;
	case 0xed:
		ret << "SET 5, L";
		break;
	case 0xee:
		ret << "SET 5, (HL)";
		break;
	case 0xef:
		ret << "SET 5, A";
		break;
	case 0xf0:
		ret << "SET 6, B";
		break;
	case 0xf1:
		ret << "SET 6, C";
		break;
	case 0xf2:
		ret << "SET 6, D";
		break;
	case 0xf3:
		ret << "SET 6, E";
		break;
	case 0xf4:
		ret << "SET 6, H";
		break;
	case 0xf5:
		ret << "SET 6, L";
		break;
	case 0xf6:
		ret << "SET 6, (HL)";
		break;
	case 0xf7:
		ret << "SET 6, A";
		break;
	case 0xf8:
		ret << "SET 7, B";
		break;
	case 0xf9:
		ret << "SET 7, C";
		break;
	case 0xfa:
		ret << "SET 7, D";
		break;
	case 0xfb:
		ret << "SET 7, E";
		break;
	case 0xfc:
		ret << "SET 7, H";
		break;
	case 0xfd:
		ret << "SET 7, L";
		break;
	case 0xfe:
		ret << "SET 7, (HL)";
		break;
	case 0xff:
		ret << "SET 7, A";
		break;
	}

	++curr;
	return ret.str();
}

std::string dump_ext(const uint8_t*& curr)
{
	std::ostringstream ret;

	switch (*curr)
	{
	case 0x40:
		ret << "IN B, (C)";
		++curr;
		break;
	case 0x41:
		ret << "OUT (C), B";
		++curr;
		break;
	case 0x42:
		ret << "SBC HL, BC";
		++curr;
		break;
	case 0x43:
		ret << "LD (";
		++curr;
		ret << *reinterpret_cast<const uint16_t*>(curr);
		curr += 2;
		ret << "), BC";
		break;
	case 0x44:
		ret << "NEG";
		++curr;
		break;
	case 0x45:
		ret << "RETN";
		++curr;
		break;
	case 0x46:
		ret << "IM 0";
		++curr;
		break;
	case 0x47:
		ret << "LD I, A";
		++curr;
		break;
	case 0x48:
		ret << "IN C, (C)";
		++curr;
		break;
	case 0x49:
		ret << "OUT (C), C";
		++curr;
		break;
	case 0x4a:
		ret << "ADC HL, BC";
		++curr;
		break;
	case 0x4b:
		ret << "LD BC, (";
		++curr;
		ret << *reinterpret_cast<const uint16_t*>(curr);
		curr += 2;
		ret << ')';
		break;
	case 0x4c:
		ret << "NEG";
		++curr;
		break;
	case 0x4d:
		ret << "RETI";
		++curr;
		break;
	case 0x4e:
		ret << "IM 0/1";
		++curr;
		break;
	case 0x4f:
		ret << "LD R, A";
		++curr;
		break;
	case 0x50:
		ret << "IN D, (C)";
		++curr;
		break;
	case 0x51:
		ret << "OUT (C), D";
		++curr;
		break;
	case 0x52:
		ret << "SBC HL, DE";
		++curr;
		break;
	case 0x53:
		ret << "LD (";
		++curr;
		ret << *reinterpret_cast<const uint16_t*>(curr);
		curr += 2;
		ret << "), DE";
		break;
	case 0x54:
		ret << "NEG";
		++curr;
		break;
	case 0x55:
		ret << "RETN";
		++curr;
		break;
	case 0x56:
		ret << "IM 1";
		++curr;
		break;
	case 0x57:
		ret << "LD A, I";
		++curr;
		break;
	case 0x58:
		ret << "IN E, (C)";
		++curr;
		break;
	case 0x59:
		ret << "OUT (C), E";
		++curr;
		break;
	case 0x5a:
		ret << "ADC HL, DE";
		++curr;
		break;
	case 0x5b:
		ret << "LD DE, (";
		++curr;
		ret << *reinterpret_cast<const uint16_t*>(curr);
		curr += 2;
		ret << ')';
		break;
	case 0x5c:
		ret << "NEG";
		++curr;
		break;
	case 0x5d:
		ret << "RETN";
		++curr;
		break;
	case 0x5e:
		ret << "IM 2";
		++curr;
		break;
	case 0x5f:
		ret << "LD A, R";
		++curr;
		break;
	case 0x60:
		ret << "IN H, (C)";
		++curr;
		break;
	case 0x61:
		ret << "OUT (C), H";
		++curr;
		break;
	case 0x62:
		ret << "SBC HL, HL";
		++curr;
		break;
	case 0x63:
		ret << "LD (";
		++curr;
		ret << *reinterpret_cast<const uint16_t*>(curr);
		curr += 2;
		ret << "), HL";
		break;
	case 0x64:
		ret << "NEG";
		++curr;
		break;
	case 0x65:
		ret << "RETN";
		++curr;
		break;
	case 0x66:
		ret << "IM 0";
		++curr;
		break;
	case 0x67:
		ret << "RRD";
		++curr;
		break;
	case 0x68:
		ret << "IN L, (C)";
		++curr;
		break;
	case 0x69:
		ret << "OUT (C), L";
		++curr;
		break;
	case 0x6a:
		ret << "ADC HL, HL";
		++curr;
		break;
	case 0x6b:
		ret << "LD HL, (";
		++curr;
		ret << *reinterpret_cast<const uint16_t*>(curr);
		curr += 2;
		ret << ')';
		break;
	case 0x6c:
		ret << "NEG";
		++curr;
		break;
	case 0x6d:
		ret << "RETN";
		++curr;
		break;
	case 0x6e:
		ret << "IM 0/1";
		++curr;
		break;
	case 0x6f:
		ret << "RLD";
		++curr;
		break;
	case 0x70:
		ret << "IN (C)";
		++curr;
		break;
	case 0x71:
		ret << "OUT (C), 0";
		++curr;
		break;
	case 0x72:
		ret << "SBC HL, SP";
		++curr;
		break;
	case 0x73:
		ret << "LD (";
		++curr;
		ret << *reinterpret_cast<const uint16_t*>(curr);
		curr += 2;
		ret << "), SP";
		break;
	case 0x74:
		ret << "NEG";
		++curr;
		break;
	case 0x75:
		ret << "RETN";
		++curr;
		break;
	case 0x76:
		ret << "IM 1";
		++curr;
		break;
	case 0x78:
		ret << "IN A, (C)";
		++curr;
		break;
	case 0x79:
		ret << "OUT (C), A";
		++curr;
		break;
	case 0x7a:
		ret << "ADC HL, SP";
		++curr;
		break;
	case 0x7b:
		ret << "LD SP, (";
		++curr;
		ret << *reinterpret_cast<const uint16_t*>(curr);
		curr += 2;
		ret << ')';
		break;
	case 0x7c:
		ret << "NEG";
		++curr;
		break;
	case 0x7d:
		ret << "RETN";
		++curr;
		break;
	case 0x7e:
		ret << "IM 2";
		++curr;
		break;
	case 0xa0:
		ret << "LDI";
		++curr;
		break;
	case 0xa1:
		ret << "CPI";
		++curr;
		break;
	case 0xa2:
		ret << "INI";
		++curr;
		break;
	case 0xa3:
		ret << "OUTI";
		++curr;
		break;
	case 0xa8:
		ret << "LDD";
		++curr;
		break;
	case 0xa9:
		ret << "CPD";
		++curr;
		break;
	case 0xaa:
		ret << "IND";
		++curr;
		break;
	case 0xab:
		ret << "OUTD";
		++curr;
		break;
	case 0xb0:
		ret << "LDIR";
		++curr;
		break;
	case 0xb1:
		ret << "CPIR";
		++curr;
		break;
	case 0xb2:
		ret << "INIR";
		++curr;
		break;
	case 0xb3:
		ret << "OTIR";
		++curr;
		break;
	case 0xb8:
		ret << "LDDR";
		++curr;
		break;
	case 0xb9:
		ret << "CPDR";
		++curr;
		break;
	case 0xba:
		ret << "INDR";
		++curr;
		break;
	case 0xbb:
		ret << "OTDR";
		++curr;
		break;
	default:
		ret << "db 237, " << static_cast<uint16_t>(*curr);
		break;
	}

	return ret.str();
}

void fetch_opcode(const uint8_t*& curr, std::string& line)
{
	std::ostringstream ss;

	switch (*curr)
	{
	case 0x00:
		ss << "NOP";
		++curr;
		break;
	case 0x01:
		ss << "LD BC, ";
		++curr;
		ss << *reinterpret_cast<const uint16_t*>(curr);
		curr += 2;
		break;
	case 0x02:
		ss << "LD (BC), A";
		++curr;
		break;
	case 0x03:
		ss << "INC BC";
		++curr;
		break;
	case 0x04:
		ss << "INC B";
		++curr;
		break;
	case 0x05:
		ss << "DEC B";
		++curr;
		break;
	case 0x06:
		ss << "LD B, ";
		++curr;
		ss << static_cast<uint16_t>(*curr);
		++curr;
		break;
	case 0x07:
		ss << "RLCA";
		++curr;
		break;
	case 0x08:
		ss << "EX AF,AF'";
		++curr;
		break;
	case 0x09:
		ss << "ADD HL,BC";
		++curr;
		break;
	case 0x0a:
		ss << "LD A,(BC)";
		++curr;
		break;
	case 0x0b:
		ss << "DEC BC";
		++curr;
		break;
	case 0x0c:
		ss << "INC C";
		++curr;
		break;
	case 0x0d:
		ss << "DEC C";
		++curr;
		break;
	case 0x0e:
		ss << "LD C, ";
		++curr;
		ss << static_cast<uint16_t>(*curr);
		++curr;
		break;
	case 0x0f:
		ss << "RRCA";
		++curr;
		break;
	case 0x10:
		ss << "DJNZ ";
		++curr;
		ss << static_cast<int16_t>(static_cast<char>(*curr));
		++curr;
		break;
	case 0x11:
		ss << "LD DE, ";
		++curr;
		ss << *reinterpret_cast<const uint16_t*>(curr);
		curr += 2;
		break;
	case 0x12:
		ss << "LD (DE), A";
		++curr;
		break;
	case 0x13:
		ss << "INC DE";
		++curr;
		break;
	case 0x14:
		ss << "INC D";
		++curr;
		break;
	case 0x15:
		ss << "DEC D";
		++curr;
		break;
	case 0x16:
		ss << "LD D, ";
		++curr;
		ss << static_cast<uint16_t>(*curr);
		++curr;
		break;
	case 0x17:
		ss << "RLA";
		++curr;
		break;
	case 0x18:
		ss << "JR ";
		++curr;
		ss << static_cast<int16_t>(static_cast<char>(*curr));
		++curr;
		break;
	case 0x19:
		ss << "ADD HL, DE";
		++curr;
		break;
	case 0x1a:
		ss << "LD A, (DE)";
		++curr;
		break;
	case 0x1b:
		ss << "DEC DE";
		++curr;
		break;
	case 0x1c:
		ss << "INC E";
		++curr;
		break;
	case 0x1d:
		ss << "DEC E";
		++curr;
		break;
	case 0x1e:
		ss << "LD E, ";
		++curr;
		ss << static_cast<uint16_t>(*curr);
		++curr;
		break;
	case 0x1f:
		ss << "RRA";
		++curr;
		break;
	case 0x20:
		ss << "JR NZ, ";
		++curr;
		ss << static_cast<int16_t>(static_cast<char>(*curr));
		++curr;
		break;
	case 0x21:
		ss << "LD HL, ";
		++curr;
		ss << *reinterpret_cast<const uint16_t*>(curr);
		curr += 2;
		break;
	case 0x22:
		ss << "LD (";
		++curr;
		ss << *reinterpret_cast<const uint16_t*>(curr);
		curr += 2;
		ss << "), HL";
		break;
	case 0x23:
		ss << "INC HL";
		++curr;
		break;
	case 0x24:
		ss << "INC H";
		++curr;
		break;
	case 0x25:
		ss << "DEC H";
		++curr;
		break;
	case 0x26:
		ss << "LD H, ";
		++curr;
		ss << static_cast<uint16_t>(*curr);
		++curr;
		break;
	case 0x27:
		ss << "DAA";
		++curr;
		break;
	case 0x28:
		ss << "JR Z, ";
		++curr;
		ss << static_cast<int16_t>(static_cast<char>(*curr));
		++curr;
		break;
	case 0x29:
		ss << "ADD HL, HL";
		++curr;
		break;
	case 0x2a:
		ss << "LD HL, (";
		++curr;
		ss << *reinterpret_cast<const uint16_t*>(curr);
		curr += 2;
		ss << ')';
		break;
	case 0x2b:
		ss << "DEC HL";
		++curr;
		break;
	case 0x2c:
		ss << "INC L";
		++curr;
		break;
	case 0x2d:
		ss << "DEC L";
		++curr;
		break;
	case 0x2e:
		ss << "LD L, ";
		++curr;
		ss << static_cast<uint16_t>(*curr);
		++curr;
		break;
	case 0x2f:
		ss << "CPL";
		++curr;
		break;
	case 0x30:
		ss << "JR NC, ";
		++curr;
		ss << static_cast<int16_t>(static_cast<char>(*curr));
		++curr;
		break;
	case 0x31:
		ss << "LD SP, ";
		++curr;
		ss << *reinterpret_cast<const uint16_t*>(curr);
		curr += 2;
		break;
	case 0x32:
		ss << "LD (";
		++curr;
		ss << *reinterpret_cast<const uint16_t*>(curr);
		curr += 2;
		ss << "), A";
		break;
	case 0x33:
		ss << "INC SP";
		++curr;
		break;
	case 0x34:
		ss << "INC (HL)";
		++curr;
		break;
	case 0x35:
		ss << "DEC (HL)";
		++curr;
		break;
	case 0x36:
		ss << "LD (HL), ";
		++curr;
		ss << static_cast<uint16_t>(*curr);
		++curr;
		break;
	case 0x37:
		ss << "SCF";
		++curr;
		break;
	case 0x38:
		ss << "JR C, ";
		++curr;
		ss << static_cast<int16_t>(static_cast<char>(*curr));
		++curr;
		break;
	case 0x39:
		ss << "ADD HL, SP";
		++curr;
		break;
	case 0x3a:
		ss << "LD A, (";
		++curr;
		ss << *reinterpret_cast<const uint16_t*>(curr);
		curr += 2;
		ss << ')';
		break;
	case 0x3b:
		ss << "DEC SP";
		++curr;
		break;
	case 0x3c:
		ss << "INC A";
		++curr;
		break;
	case 0x3d:
		ss << "DEC A";
		++curr;
		break;
	case 0x3e:
		ss << "LD A, ";
		++curr;
		ss << static_cast<uint16_t>(*curr);
		++curr;
		break;
	case 0x3f:
		ss << "CCF";
		++curr;
		break;
	case 0x40:
		ss << "LD B, B";
		++curr;
		break;
	case 0x41:
		ss << "LD B, C";
		++curr;
		break;
	case 0x42:
		ss << "LD B, D";
		++curr;
		break;
	case 0x43:
		ss << "LD B, E";
		++curr;
		break;
	case 0x44:
		ss << "LD B, H";
		++curr;
		break;
	case 0x45:
		ss << "LD B, L";
		++curr;
		break;
	case 0x46:
		ss << "LD B, (HL)";
		++curr;
		break;
	case 0x47:
		ss << "LD B, A";
		++curr;
		break;
	case 0x48:
		ss << "LD C, B";
		++curr;
		break;
	case 0x49:
		ss << "LD C, C";
		++curr;
		break;
	case 0x4a:
		ss << "LD C, D";
		++curr;
		break;
	case 0x4b:
		ss << "LD C, E";
		++curr;
		break;
	case 0x4c:
		ss << "LD C, H";
		++curr;
		break;
	case 0x4d:
		ss << "LD C, L";
		++curr;
		break;
	case 0x4e:
		ss << "LD C, (HL)";
		++curr;
		break;
	case 0x4f:
		ss << "LD C, A";
		++curr;
		break;
	case 0x50:
		ss << "LD D, B";
		++curr;
		break;
	case 0x51:
		ss << "LD D, C";
		++curr;
		break;
	case 0x52:
		ss << "LD D, D";
		++curr;
		break;
	case 0x53:
		ss << "LD D, E";
		++curr;
		break;
	case 0x54:
		ss << "LD D, H";
		++curr;
		break;
	case 0x55:
		ss << "LD D, L";
		++curr;
		break;
	case 0x56:
		ss << "LD D, (HL)";
		++curr;
		break;
	case 0x57:
		ss << "LD D, A";
		++curr;
		break;
	case 0x58:
		ss << "LD E, B";
		++curr;
		break;
	case 0x59:
		ss << "LD E, C";
		++curr;
		break;
	case 0x5a:
		ss << "LD E, D";
		++curr;
		break;
	case 0x5b:
		ss << "LD E, E";
		++curr;
		break;
	case 0x5c:
		ss << "LD E, H";
		++curr;
		break;
	case 0x5d:
		ss << "LD E, L";
		++curr;
		break;
	case 0x5e:
		ss << "LD E, (HL)";
		++curr;
		break;
	case 0x5f:
		ss << "LD E, A";
		++curr;
		break;
	case 0x60:
		ss << "LD H, B";
		++curr;
		break;
	case 0x61:
		ss << "LD H, C";
		++curr;
		break;
	case 0x62:
		ss << "LD H, D";
		++curr;
		break;
	case 0x63:
		ss << "LD H, E";
		++curr;
		break;
	case 0x64:
		ss << "LD H, H";
		++curr;
		break;
	case 0x65:
		ss << "LD H, L";
		++curr;
		break;
	case 0x66:
		ss << "LD H, (HL)";
		++curr;
		break;
	case 0x67:
		ss << "LD H, A";
		++curr;
		break;
	case 0x68:
		ss << "LD L, B";
		++curr;
		break;
	case 0x69:
		ss << "LD L, C";
		++curr;
		break;
	case 0x6a:
		ss << "LD L, D";
		++curr;
		break;
	case 0x6b:
		ss << "LD L, E";
		++curr;
		break;
	case 0x6c:
		ss << "LD L, H";
		++curr;
		break;
	case 0x6d:
		ss << "LD L, L";
		++curr;
		break;
	case 0x6e:
		ss << "LD L, (HL)";
		++curr;
		break;
	case 0x6f:
		ss << "LD L, A";
		++curr;
		break;
	case 0x70:
		ss << "LD (HL), B";
		++curr;
		break;
	case 0x71:
		ss << "LD (HL), C";
		++curr;
		break;
	case 0x72:
		ss << "LD (HL), D";
		++curr;
		break;
	case 0x73:
		ss << "LD (HL), E";
		++curr;
		break;
	case 0x74:
		ss << "LD (HL), H";
		++curr;
		break;
	case 0x75:
		ss << "LD (HL), L";
		++curr;
		break;
	case 0x76:
		ss << "HALT";
		++curr;
		break;
	case 0x77:
		ss << "LD (HL), A";
		++curr;
		break;
	case 0x78:
		ss << "LD A, B";
		++curr;
		break;
	case 0x79:
		ss << "LD A, C";
		++curr;
		break;
	case 0x7a:
		ss << "LD A, D";
		++curr;
		break;
	case 0x7b:
		ss << "LD A, E";
		++curr;
		break;
	case 0x7c:
		ss << "LD A, H";
		++curr;
		break;
	case 0x7d:
		ss << "LD A, L";
		++curr;
		break;
	case 0x7e:
		ss << "LD A, (HL)";
		++curr;
		break;
	case 0x7f:
		ss << "LD A, A";
		++curr;
		break;
	case 0x80:
		ss << "ADD A, B";
		++curr;
		break;
	case 0x81:
		ss << "ADD A, C";
		++curr;
		break;
	case 0x82:
		ss << "ADD A, D";
		++curr;
		break;
	case 0x83:
		ss << "ADD A, E";
		++curr;
		break;
	case 0x84:
		ss << "ADD A, H";
		++curr;
		break;
	case 0x85:
		ss << "ADD A, L";
		++curr;
		break;
	case 0x86:
		ss << "ADD A, (HL)";
		++curr;
		break;
	case 0x87:
		ss << "ADD A, A";
		++curr;
		break;
	case 0x88:
		ss << "ADC A, B";
		++curr;
		break;
	case 0x89:
		ss << "ADC A, C";
		++curr;
		break;
	case 0x8a:
		ss << "ADC A, D";
		++curr;
		break;
	case 0x8b:
		ss << "ADC A, E";
		++curr;
		break;
	case 0x8c:
		ss << "ADC A, H";
		++curr;
		break;
	case 0x8d:
		ss << "ADC A, L";
		++curr;
		break;
	case 0x8e:
		ss << "ADC A, (HL)";
		++curr;
		break;
	case 0x8f:
		ss << "ADC A, A";
		++curr;
		break;
	case 0x90:
		ss << "SUB B";
		++curr;
		break;
	case 0x91:
		ss << "SUB C";
		++curr;
		break;
	case 0x92:
		ss << "SUB D";
		++curr;
		break;
	case 0x93:
		ss << "SUB E";
		++curr;
		break;
	case 0x94:
		ss << "SUB H";
		++curr;
		break;
	case 0x95:
		ss << "SUB L";
		++curr;
		break;
	case 0x96:
		ss << "SUB (HL)";
		++curr;
		break;
	case 0x97:
		ss << "SUB A";
		++curr;
		break;
	case 0x98:
		ss << "SBC A, B";
		++curr;
		break;
	case 0x99:
		ss << "SBC A, C";
		++curr;
		break;
	case 0x9a:
		ss << "SBC A, D";
		++curr;
		break;
	case 0x9b:
		ss << "SBC A, E";
		++curr;
		break;
	case 0x9c:
		ss << "SBC A, H";
		++curr;
		break;
	case 0x9d:
		ss << "SBC A, L";
		++curr;
		break;
	case 0x9e:
		ss << "SBC A, (HL)";
		++curr;
		break;
	case 0x9f:
		ss << "SBC A, A";
		++curr;
		break;
	case 0xa0:
		ss << "AND B";
		++curr;
		break;
	case 0xa1:
		ss << "AND C";
		++curr;
		break;
	case 0xa2:
		ss << "AND D";
		++curr;
		break;
	case 0xa3:
		ss << "AND E";
		++curr;
		break;
	case 0xa4:
		ss << "AND H";
		++curr;
		break;
	case 0xa5:
		ss << "AND L";
		++curr;
		break;
	case 0xa6:
		ss << "AND (HL)";
		++curr;
		break;
	case 0xa7:
		ss << "AND A";
		++curr;
		break;
	case 0xa8:
		ss << "XOR B";
		++curr;
		break;
	case 0xa9:
		ss << "XOR C";
		++curr;
		break;
	case 0xaa:
		ss << "XOR D";
		++curr;
		break;
	case 0xab:
		ss << "XOR E";
		++curr;
		break;
	case 0xac:
		ss << "XOR H";
		++curr;
		break;
	case 0xad:
		ss << "XOR L";
		++curr;
		break;
	case 0xae:
		ss << "XOR (HL)";
		++curr;
		break;
	case 0xaf:
		ss << "XOR A";
		++curr;
		break;
	case 0xb0:
		ss << "OR B";
		++curr;
		break;
	case 0xb1:
		ss << "OR C";
		++curr;
		break;
	case 0xb2:
		ss << "OR D";
		++curr;
		break;
	case 0xb3:
		ss << "OR E";
		++curr;
		break;
	case 0xb4:
		ss << "OR H";
		++curr;
		break;
	case 0xb5:
		ss << "OR L";
		++curr;
		break;
	case 0xb6:
		ss << "OR (HL)";
		++curr;
		break;
	case 0xb7:
		ss << "OR A";
		++curr;
		break;
	case 0xb8:
		ss << "CP B";
		++curr;
		break;
	case 0xb9:
		ss << "CP C";
		++curr;
		break;
	case 0xba:
		ss << "CP D";
		++curr;
		break;
	case 0xbb:
		ss << "CP E";
		++curr;
		break;
	case 0xbc:
		ss << "CP H";
		++curr;
		break;
	case 0xbd:
		ss << "CP L";
		++curr;
		break;
	case 0xbe:
		ss << "CP (HL)";
		++curr;
		break;
	case 0xbf:
		ss << "CP A";
		++curr;
		break;
	case 0xc0:
		ss << "RET NZ";
		++curr;
		break;
	case 0xc1:
		ss << "POP BC";
		++curr;
		break;
	case 0xc2:
		ss << "JP NZ, ";
		++curr;
		ss << *reinterpret_cast<const uint16_t*>(curr);
		curr += 2;
		break;
	case 0xc3:
		ss << "JP ";
		++curr;
		ss << *reinterpret_cast<const uint16_t*>(curr);
		curr += 2;
		break;
	case 0xc4:
		ss << "CALL NZ, ";
		++curr;
		ss << *reinterpret_cast<const uint16_t*>(curr);
		curr += 2;
		break;
	case 0xc5:
		ss << "PUSH BC";
		++curr;
		break;
	case 0xc6:
		ss << "ADD A, ";
		++curr;
		ss << static_cast<uint16_t>(*curr);
		++curr;
		break;
	case 0xc7:
		ss << "RST 00H";
		++curr;
		break;
	case 0xc8:
		ss << "RET Z";
		++curr;
		break;
	case 0xc9:
		ss << "RET";
		++curr;
		break;
	case 0xca:
		ss << "JP Z, ";
		++curr;
		ss << *reinterpret_cast<const uint16_t*>(curr);
		curr += 2;
		break;
	case 0xcb:
		ss << dump_bits(++curr);
		break;
	case 0xcc:
		ss << "CALL Z, ";
		++curr;
		ss << *reinterpret_cast<const uint16_t*>(curr);
		curr += 2;
		break;
	case 0xcd:
		ss << "CALL ";
		++curr;
		ss << *reinterpret_cast<const uint16_t*>(curr);
		curr += 2;
		break;
	case 0xce:
		ss << "ADC A, ";
		++curr;
		ss << static_cast<uint16_t>(*curr);
		++curr;
		break;
	case 0xcf:
		ss << "RST 08H";
		++curr;
		break;
	case 0xd0:
		ss << "RET NC";
		++curr;
		break;
	case 0xd1:
		ss << "POP DE";
		++curr;
		break;
	case 0xd2:
		ss << "JP NC, ";
		++curr;
		ss << *reinterpret_cast<const uint16_t*>(curr);
		curr += 2;
		break;
	case 0xd3:
		ss << "OUT (";
		++curr;
		ss << static_cast<uint16_t>(*curr);
		++curr;
		ss << "), A";
		break;
	case 0xd4:
		ss << "CALL NC, ";
		++curr;
		ss << *reinterpret_cast<const uint16_t*>(curr);
		curr += 2;
		break;
	case 0xd5:
		ss << "PUSH DE";
		++curr;
		break;
	case 0xd6:
		ss << "SUB ";
		++curr;
		ss << static_cast<uint16_t>(*curr);
		++curr;
		break;
	case 0xd7:
		ss << "RST 10H";
		++curr;
		break;
	case 0xd8:
		ss << "RET C";
		++curr;
		break;
	case 0xd9:
		ss << "EXX";
		++curr;
		break;
	case 0xda:
		ss << "JP C, ";
		++curr;
		ss << *reinterpret_cast<const uint16_t*>(curr);
		curr += 2;
		break;
	case 0xdb:
		ss << "IN A, (";
		++curr;
		ss << static_cast<uint16_t>(*curr);
		++curr;
		ss << ')';
		break;
	case 0xdc:
		ss << "CALL C, ";
		++curr;
		ss << *reinterpret_cast<const uint16_t*>(curr);
		curr += 2;
		break;
	case 0xdd:
		ss << dump_IX_IY(++curr, 'X');
		break;
	case 0xde:
		ss << "SBC A, ";
		++curr;
		ss << static_cast<uint16_t>(*curr);
		++curr;
		break;
	case 0xdf:
		ss << "RST 18H";
		++curr;
		break;
	case 0xe0:
		ss << "RET PO";
		++curr;
		break;
	case 0xe1:
		ss << "POP HL";
		++curr;
		break;
	case 0xe2:
		ss << "JP PO, ";
		++curr;
		ss << *reinterpret_cast<const uint16_t*>(curr);
		curr += 2;
		break;
	case 0xe3:
		ss << "EX (SP), HL";
		++curr;
		break;
	case 0xe4:
		ss << "CALL PO, ";
		++curr;
		ss << *reinterpret_cast<const uint16_t*>(curr);
		curr += 2;
		break;
	case 0xe5:
		ss << "PUSH HL";
		++curr;
		break;
	case 0xe6:
		ss << "AND ";
		++curr;
		ss << static_cast<uint16_t>(*curr);
		++curr;
		break;
	case 0xe7:
		ss << "RST 20H";
		++curr;
		break;
	case 0xe8:
		ss << "RET PE";
		++curr;
		break;
	case 0xe9:
		ss << "JP (HL)";
		++curr;
		break;
	case 0xea:
		ss << "JP PE, ";
		++curr;
		ss << *reinterpret_cast<const uint16_t*>(curr);
		curr += 2;
		break;
	case 0xeb:
		ss << "EX DE, HL";
		++curr;
		break;
	case 0xec:
		ss << "CALL PE, ";
		++curr;
		ss << *reinterpret_cast<const uint16_t*>(curr);
		curr += 2;
		break;
	case 0xed:
		ss << dump_ext(++curr);
		break;
	case 0xee:
		ss << "XOR ";
		++curr;
		ss << static_cast<uint16_t>(*curr);
		++curr;
		break;
	case 0xef:
		ss << "RST 28H";
		++curr;
		break;
	case 0xf0:
		ss << "RET P";
		++curr;
		break;
	case 0xf1:
		ss << "POP AF";
		++curr;
		break;
	case 0xf2:
		ss << "JP P, ";
		++curr;
		ss << *reinterpret_cast<const uint16_t*>(curr);
		curr += 2;
		break;
	case 0xf3:
		ss << "DI";
		++curr;
		break;
	case 0xf4:
		ss << "CALL P, ";
		++curr;
		ss << *reinterpret_cast<const uint16_t*>(curr);
		curr += 2;
		break;
	case 0xf5:
		ss << "PUSH AF";
		++curr;
		break;
	case 0xf6:
		ss << "OR ";
		++curr;
		ss << static_cast<uint16_t>(*curr);
		++curr;
		break;
	case 0xf7:
		ss << "RST 30H";
		++curr;
		break;
	case 0xf8:
		ss << "RET M";
		++curr;
		break;
	case 0xf9:
		ss << "LD SP, HL";
		++curr;
		break;
	case 0xfa:
		ss << "JP M, ";
		++curr;
		ss << *reinterpret_cast<const uint16_t*>(curr);
		curr += 2;
		break;
	case 0xfb:
		ss << "EI";
		++curr;
		break;
	case 0xfc:
		ss << "CALL M, ";
		++curr;
		ss << *reinterpret_cast<const uint16_t*>(curr);
		curr += 2;
		break;
	case 0xfd:
		dump_IX_IY(++curr, 'Y');
		break;
	case 0xfe:
		ss << "CP ";
		++curr;
		ss << static_cast<uint16_t>(*curr);
		++curr;
		break;
	case 0xff:
		ss << "RST 38H";
		++curr;
		break;
	}

	ss << '\n';
	line = ss.str();
}

void dump(const data& data)
{
	std::size_t addr = data._org;
	const uint8_t* first = &data._memory.front();
	const uint8_t* second = first + data._memory.size();
	std::string bytes;
	std::string line;

	while (first != second)
	{
		const uint8_t* curr = first;
		std::ostringstream ss;
		std::size_t offset = 0;

		fetch_opcode(first, line);
		offset = first - curr;

		for (; curr < first; ++curr)
		{
			ss.width(3);
			ss << static_cast<int>(*curr) << ' ';
		}

		bytes = ss.str();
		bytes.resize(5 * 4, ' ');
		std::cout.width(5);
		std::cout << addr << "   ";
		std::cout << bytes << line;
		addr += offset;
	}
}

void save(data& data, const char* src, const char* dest)
{
	lexertl::memory_file mf(src);

	if (mf.data())
	{
		std::vector<uint8_t> sna(mf.data(), mf.data() + mf.size());
		uint8_t* ptr = &sna.front() + 27 + (data._org - 16384);
		FILE* fp = nullptr;

		mf.close();
		memcpy(ptr, &data._memory.front(), data._memory.size());
		fopen_s(&fp, dest, "wb");

		if (fp)
		{
			fwrite(&sna.front(), 1, sna.size(), fp);
			fclose(fp);
		}
		else
		{
			throw std::runtime_error("Unable to open " + std::string(dest));
		}
	}
	else
	{
		throw std::runtime_error("Unable to open " + std::string(src));
	}
}

int main(int argc, const char* argv[])
{
	if (argc != 2 && argc != 4)
	{
		std::cout << "USAGE: z80_assembler <pathname> [<source .sna> <dest .sna>]\n";
		return 1;
	}

	try
	{
		lexertl::memory_file mf(argv[1]);
		data data;

		if (!mf.data())
			throw std::runtime_error("Unable to open " + std::string(argv[1]));

		data.build_parser();
		data.parse(mf.data(), mf.data() + mf.size());
		mf.close();

		if (data._org + data._memory.size() - 1 > 65535)
			throw std::runtime_error("Code exceeds memory limit (65535)");

		if (argc == 4)
			save(data, argv[2], argv[3]);
		else
			dump(data);

		return 0;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << '\n';
		return 1;
	}
}
