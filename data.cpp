#include "data.h"
#include "../parsertl14/include/parsertl/generator.hpp"
#include "../parsertl14/include/parsertl/lookup.hpp"
#include <stdexcept>
#include <string>

auto data::dollar(const std::size_t index)
{
	return _results.dollar(_gsm, index, _productions);
}

void data::push_byte()
{
	if (_integer > 255)
		throw std::out_of_range("Value " + std::to_string(_integer) + " is greater than 255");

	_memory.push_back(_integer & 0xff);
}

void data::push_byte(const uint8_t by)
{
	_memory.push_back(by);
}

void data::push_word(const uint16_t w)
{
	_memory.push_back(w & 0xff);
	_memory.push_back(w >> 8);
}

void data::rel_label(const std::size_t idx)
{
	const std::string name = dollar(idx).str();
	auto iter = _label.find(name);

	_rel_addr[_memory.size() - 1] = name;
}

void data::bexpr(const int32_t idx)
{
	const std::string name = dollar(idx).str();

	_byte_expr[_memory.size() - 1] = name;
}

void data::wexpr(const int32_t idx)
{
	const std::string name = dollar(idx).str();

	_word_expr[_memory.size() - 2] = name;
}

void data::build_parser()
{
	parsertl::rules grules;
	lexertl::rules lrules(lexertl::dot_not_cr_lf | lexertl::icase);
	std::string warnings;

	grules.token("A ADD ADC AF AND B BC Binary BIT C CALL CCF Char CP CPD "
		"CPDR CPI CPIR CPL D DAA DB DW DE DEC DI DJNZ E EI EQU EX EXX H "
		"HALT Hex HL Integer I IM IN INC IND INDR INI INIR IX IY JP JR L "
		"LD LDD LDDR LDI LDIR M Name NC NEG NL NOP NZ OR ORG OTDR OTIR "
		"OUT OUTD OUTI P PE PO PUSH POP R RES RET RETI RETN RL RLA RLC "
		"RLCA RLD RR RRA RRC RRCA RRD RST SBC SCF SET SLA SP SRA SRL "
		"String SUB XOR Z");

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
		auto iter = data._label.find(name);

		if (iter != data._label.end())
		{
			throw std::runtime_error(name + " already exists");
		}

		data._label[name] = static_cast<uint16_t>(data._memory.size());
	};
	grules.push("opt_colon", "%empty | ':'");
	_actions[grules.push("opcode", "Name EQU expr")] = [](data& data)
	{
		data._equ[data.dollar(0).str()] = data.dollar(2).str();
	};
	grules.push("opcode", "DB db_list");
	grules.push("opcode", "DW dw_list");
	_actions[grules.push("db_list", "expr")] = [](data& data)
	{
		data.push_byte(0);
		data.bexpr(0);
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
	_actions[grules.push("db_list", "db_list ',' expr")] = [](data& data)
	{
		data.push_byte(0);
		data.bexpr(2);
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
	_actions[grules.push("dw_list", "expr")] = [](data& data)
	{
		data.push_word(0);
		data.wexpr(0);
	};
	_actions[grules.push("dw_list", "dw_list ',' expr")] = [](data& data)
	{
		data.push_word(0);
		data.wexpr(2);
	};
	_actions[grules.push("opcode", R"(LD r ',' r2)")] = [](data& data)
	{
		// Made local var to prevent VC++ warning
		const uint8_t by = 0b01000000 | data._r << 3 | data._r2;

		data.push_byte(by);
	};
	_actions[grules.push("opcode", "LD r ',' expr")] = [](data& data)
	{
		data.push_byte(0b00000110 | data._r << 3);
		data.push_byte(0);
		data.bexpr(3);
	};
	_actions[grules.push("opcode", "LD r ',' '(' HL ')'")] = [](data& data)
	{
		data.push_byte(0b01000110 | data._r << 3);
	};
	_actions[grules.push("opcode", "LD r ',' '(' IX '+' expr ')'")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0b01000110 | data._r << 3);
		data.push_byte(0);
		data.bexpr(6);
	};
	_actions[grules.push("opcode", "LD r ',' '(' IY '+' expr ')'")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0b01000110 | data._r << 3);
		data.push_byte(0);
		data.bexpr(6);
	};
	_actions[grules.push("opcode", "LD '(' HL ')' ',' r")] = [](data& data)
	{
		data.push_byte(0b01110000 | data._r);
	};
	_actions[grules.push("opcode", "LD '(' IX '+' expr ')' ',' r")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0b01110000 | data._r);
		data.push_byte(0);
		data.bexpr(4);
	};
	_actions[grules.push("opcode", "LD '(' IY '+' expr ')' ',' r")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0b01110000 | data._r);
		data.push_byte(0);
		data.bexpr(4);
	};
	_actions[grules.push("opcode", "LD '(' HL ')' ',' expr")] = [](data& data)
	{
		data.push_byte(0x36);
		data.push_byte(0);
		data.bexpr(5);
	};
	_actions[grules.push("opcode", "LD '(' IX '+' expr ')' ',' expr")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0x36);
		data.push_byte(0);
		data.bexpr(4);
		data.push_byte(0);
		data.bexpr(7);
	};
	_actions[grules.push("opcode", "LD '(' IY '+' expr ')' ',' expr")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0x36);
		data.push_byte(0);
		data.bexpr(4);
		data.push_byte(0);
		data.bexpr(7);
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
	_actions[grules.push("opcode", "LD r ',' '(' expr ')'")] = [](data& data)
	{
		if (data._r != 0b111)
		{
			const std::string str(data.dollar(0).first,
				data.dollar(5).second);

			throw std::runtime_error(str + ": Only register A valid");
		}

		data.push_byte(0x3A);
		data.push_word(0);
		data.wexpr(4);
	};
	_actions[grules.push("opcode", "LD '(' BC ')' ',' A")] = [](data& data)
	{
		data.push_byte(0x02);
	};
	_actions[grules.push("opcode", "LD '(' DE ')' ',' A")] = [](data& data)
	{
		data.push_byte(0x12);
	};
	_actions[grules.push("opcode", "LD '(' expr ')' ',' A")] = [](data& data)
	{
		data.push_byte(0x32);
		data.push_word(0);
		data.wexpr(2);
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
	_actions[grules.push("opcode", "LD dd ',' expr")] = [](data& data)
	{
		data.push_byte(0b00000001 | data._dd << 4);
		data.push_word(0);
		data.wexpr(3);
	};
	_actions[grules.push("opcode", "LD IX ',' expr")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0x21);
		data.push_word(0);
		data.wexpr(3);
	};
	_actions[grules.push("opcode", "LD IY ',' expr")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0x21);
		data.push_word(0);
		data.wexpr(3);
	};
	_actions[grules.push("opcode", "LD dd ',' '(' expr ')'")] = [](data& data)
	{
		if (data._dd == 0b10)
		{
			// HL
			data.push_byte(0x2A);
		}
		else
		{
			data.push_byte(0xED);
			data.push_byte(0b01001011 | data._dd << 4);
		}

		data.push_word(0);
		data.wexpr(4);
	};
	_actions[grules.push("opcode", "LD IX ',' '(' expr ')'")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0x2A);
		data.push_word(0);
		data.wexpr(4);
	};
	_actions[grules.push("opcode", "LD IY ',' '(' expr ')'")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0x2A);
		data.push_word(0);
		data.wexpr(4);
	};
	_actions[grules.push("opcode", "LD '(' expr ')' ',' dd")] = [](data& data)
	{
		// HL
		if (data._dd == 0b10)
		{
			data.push_byte(0x22);
		}
		else
		{
			data.push_byte(0xED);
			data.push_byte(0b01000011 | data._dd << 4);
		}

		data.push_word(0);
		data.wexpr(2);
	};
	_actions[grules.push("opcode", "LD '(' expr ')' ',' IX")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0x22);
		data.push_word(0);
		data.wexpr(2);
	};
	_actions[grules.push("opcode", "LD '(' expr ')' ',' IY")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0x22);
		data.push_word(0);
		data.wexpr(2);
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
	_actions[grules.push("opcode", "ADD A ',' expr")] = [](data& data)
	{
		data.push_byte(0b11000110);
		data.push_byte(0);
		data.bexpr(3);
	};
	_actions[grules.push("opcode", "ADD A ',' '(' HL ')'")] = [](data& data)
	{
		data.push_byte(0x86);
	};
	_actions[grules.push("opcode", "ADD A ',' '(' IX '+' expr ')'")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0x86);
		data.push_byte(0);
		data.bexpr(6);
	};
	_actions[grules.push("opcode", "ADD A ',' '(' IY '+' expr ')'")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0x86);
		data.push_byte(0);
		data.bexpr(6);
	};
	_actions[grules.push("opcode", "ADC A ',' r")] = [](data& data)
	{
		data.push_byte(0b10001000 | data._r);
	};
	_actions[grules.push("opcode", "ADC A ',' expr")] = [](data& data)
	{
		data.push_byte(0xCE);
		data.push_byte(0);
		data.bexpr(3);
	};
	_actions[grules.push("opcode", "ADC A ',' '(' HL ')'")] = [](data& data)
	{
		data.push_byte(0x8E);
	};
	_actions[grules.push("opcode", "ADC A ',' '(' IX '+' expr ')'")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0x8E);
		data.push_byte(0);
		data.bexpr(6);
	};
	_actions[grules.push("opcode", "ADC A ',' '(' IY '+' expr ')'")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0x8E);
		data.push_byte(0);
		data.bexpr(6);
	};
	_actions[grules.push("opcode", "SUB r")] = [](data& data)
	{
		data.push_byte(0b10010000 | data._r);
	};
	_actions[grules.push("opcode", "SUB expr")] = [](data& data)
	{
		data.push_byte(0xD6);
		data.push_byte(0);
		data.bexpr(1);
	};
	_actions[grules.push("opcode", "SUB '(' HL ')'")] = [](data& data)
	{
		data.push_byte(0x96);
	};
	_actions[grules.push("opcode", "SUB '(' IX '+' expr ')'")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0x96);
		data.push_byte(0);
		data.bexpr(4);
	};
	_actions[grules.push("opcode", "SUB '(' IY '+' expr ')'")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0x96);
		data.push_byte(0);
		data.bexpr(4);
	};
	_actions[grules.push("opcode", "SBC A ',' r")] = [](data& data)
	{
		data.push_byte(0b10011000 | data._r);
	};
	_actions[grules.push("opcode", "SBC A ',' expr")] = [](data& data)
	{
		data.push_byte(0xDE);
		data.push_byte(0);
		data.bexpr(3);
	};
	_actions[grules.push("opcode", "SBC A ',' '(' HL ')'")] = [](data& data)
	{
		data.push_byte(0x9E);
	};
	_actions[grules.push("opcode", "SBC A ',' '(' IX '+' expr ')'")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0x9E);
		data.push_byte(0);
		data.bexpr(6);
	};
	_actions[grules.push("opcode", "SBC A ',' '(' IY '+' expr ')'")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0x9E);
		data.push_byte(0);
		data.bexpr(6);
	};
	_actions[grules.push("opcode", "AND r")] = [](data& data)
	{
		data.push_byte(0b10100000 | data._r);
	};
	_actions[grules.push("opcode", "AND expr")] = [](data& data)
	{
		data.push_byte(0xE6);
		data.push_byte(0);
		data.bexpr(1);
	};
	_actions[grules.push("opcode", "AND '(' HL ')'")] = [](data& data)
	{
		data.push_byte(0xA6);
	};
	_actions[grules.push("opcode", "AND '(' IX '+' expr ')'")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0xA6);
		data.push_byte(0);
		data.bexpr(4);
	};
	_actions[grules.push("opcode", "AND '(' IY '+' expr ')'")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0xA6);
		data.push_byte(0);
		data.bexpr(4);
	};
	_actions[grules.push("opcode", "OR r")] = [](data& data)
	{
		data.push_byte(0b10110000 | data._r);
	};
	_actions[grules.push("opcode", "OR expr")] = [](data& data)
	{
		data.push_byte(0xF6);
		data.push_byte(0);
		data.bexpr(1);
	};
	_actions[grules.push("opcode", "OR '(' HL ')'")] = [](data& data)
	{
		data.push_byte(0xB6);
	};
	_actions[grules.push("opcode", "OR '(' IX '+' expr ')'")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0xB6);
		data.push_byte(0);
		data.bexpr(4);
	};
	_actions[grules.push("opcode", "OR '(' IY '+' expr ')'")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0xB6);
		data.push_byte(0);
		data.bexpr(4);
	};
	_actions[grules.push("opcode", "XOR r")] = [](data& data)
	{
		data.push_byte(0b10101000 | data._r);
	};
	_actions[grules.push("opcode", "XOR expr")] = [](data& data)
	{
		data.push_byte(0xEE);
		data.push_byte(0);
		data.bexpr(1);
	};
	_actions[grules.push("opcode", "XOR '(' HL ')'")] = [](data& data)
	{
		data.push_byte(0xAE);
	};
	_actions[grules.push("opcode", "XOR '(' IX '+' expr ')'")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0xAE);
		data.push_byte(0);
		data.bexpr(4);
	};
	_actions[grules.push("opcode", "XOR '(' IY '+' expr ')'")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0xAE);
		data.push_byte(0);
		data.bexpr(4);
	};
	_actions[grules.push("opcode", "CP r")] = [](data& data)
	{
		data.push_byte(0b10111000 | data._r);
	};
	_actions[grules.push("opcode", "CP expr")] = [](data& data)
	{
		data.push_byte(0xFE);
		data.push_byte(0);
		data.bexpr(1);
	};
	_actions[grules.push("opcode", "CP '(' HL ')'")] = [](data& data)
	{
		data.push_byte(0xBE);
	};
	_actions[grules.push("opcode", "CP '(' IX '+' expr ')'")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0xBE);
		data.push_byte(0);
		data.bexpr(4);
	};
	_actions[grules.push("opcode", "CP '(' IY '+' expr ')'")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0xBE);
		data.push_byte(0);
		data.bexpr(4);
	};
	_actions[grules.push("opcode", "INC r")] = [](data& data)
	{
		data.push_byte(0b00000100 | data._r << 3);
	};
	_actions[grules.push("opcode", "INC '(' HL ')'")] = [](data& data)
	{
		data.push_byte(0x34);
	};
	_actions[grules.push("opcode", "INC '(' IX '+' expr ')'")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0x34);
		data.push_byte(0);
		data.bexpr(4);
	};
	_actions[grules.push("opcode", "INC '(' IY '+' expr ')'")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0x34);
		data.push_byte(0);
		data.bexpr(4);
	};
	_actions[grules.push("opcode", "DEC r")] = [](data& data)
	{
		data.push_byte(0b00000101 | data._r << 3);
	};
	_actions[grules.push("opcode", "DEC '(' HL ')'")] = [](data& data)
	{
		data.push_byte(0x35);
	};
	_actions[grules.push("opcode", "DEC '(' IX '+' expr ')'")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0x35);
		data.push_byte(0);
		data.bexpr(4);
	};
	_actions[grules.push("opcode", "DEC '(' IY '+' expr ')'")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0x35);
		data.push_byte(0);
		data.bexpr(4);
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
	_actions[grules.push("opcode", "RLC '(' IX '+' expr ')'")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(4);
		data.push_byte(0x06);
	};
	_actions[grules.push("opcode", "RLC '(' IY '+' expr ')'")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(4);
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
	_actions[grules.push("opcode", "RL '(' IX '+' expr ')'")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(4);
		data.push_byte(0x16);
	};
	_actions[grules.push("opcode", "RL '(' IY '+' expr ')'")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(4);
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
	_actions[grules.push("opcode", "RRC '(' IX '+' expr ')'")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(4);
		data.push_byte(0x0E);
	};
	_actions[grules.push("opcode", "RRC '(' IY '+' expr ')'")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(4);
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
	_actions[grules.push("opcode", "RR '(' IX '+' expr ')'")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(4);
		data.push_byte(0x1E);
	};
	_actions[grules.push("opcode", "RR '(' IY '+' expr ')'")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(4);
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
	_actions[grules.push("opcode", "SLA '(' IX '+' expr ')'")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(4);
		data.push_byte(0x26);
	};
	_actions[grules.push("opcode", "SLA '(' IY '+' expr ')'")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(4);
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
	_actions[grules.push("opcode", "SRA '(' IX '+' expr ')'")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(4);
		data.push_byte(0x2E);
	};
	_actions[grules.push("opcode", "SRA '(' IY '+' expr ')'")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(4);
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
	_actions[grules.push("opcode", "SRL '(' IX '+' expr ')'")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(4);
		data.push_byte(0x3E);
	};
	_actions[grules.push("opcode", "SRL '(' IY '+' expr ')'")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(4);
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
	// Bit is 0-7
	_actions[grules.push("opcode", "BIT integer ',' r")] = [](data& data)
	{
		const uint16_t bit = data._integer;

		if (bit > 7)
		{
			const std::string str(data.dollar(0).first,
				data.dollar(3).second);

			throw std::out_of_range(str + ": Bit out of range");
		}

		// Made local var to prevent VC++ warning
		const uint8_t by = 0b01000000 | bit << 3 | data._r;

		data.push_byte(0xCB);
		data.push_byte(by);
	};
	// Bit is 0-7
	_actions[grules.push("opcode", "BIT integer ',' '(' HL ')'")] = [](data& data)
	{
		const uint16_t bit = data._integer;

		if (bit > 7)
		{
			const std::string str(data.dollar(0).first,
				data.dollar(5).second);

			throw std::out_of_range(str + ": Bit out of range");
		}

		data.push_byte(0xCB);
		data.push_byte(0b01000110 | bit << 3);
	};
	// Bit is 0-7
	_actions[grules.push("opcode", "BIT integer ',' '(' IX '+' expr ')'")] = [](data& data)
	{
		const uint16_t bit = data._integer;

		if (bit > 7)
		{
			const std::string str(data.dollar(0).first,
				data.dollar(7).second);

			throw std::out_of_range(str + ": Bit out of range");
		}

		data.push_byte(0xDD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(6);
		data.push_byte(0b01000110 | bit << 3);
	};
	// Bit is 0-7
	_actions[grules.push("opcode", "BIT integer ',' '(' IY '+' expr ')'")] = [](data& data)
	{
		const uint16_t bit = data._integer;

		if (bit > 7)
		{
			const std::string str(data.dollar(0).first,
				data.dollar(7).second);

			throw std::out_of_range(str + ": Bit out of range");
		}

		data.push_byte(0xFD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(6);
		data.push_byte(0b01000110 | bit << 3);
	};
	// Bit is 0-7
	_actions[grules.push("opcode", "SET integer ',' r")] = [](data& data)
	{
		const uint16_t bit = data._integer;

		if (bit > 7)
		{
			const std::string str(data.dollar(0).first,
				data.dollar(3).second);

			throw std::out_of_range(str + ": Bit out of range");
		}

		// Made local var to prevent VC++ warning
		const uint8_t by = 0b11000000 | bit << 3 | data._r;

		data.push_byte(0xCB);
		data.push_byte(by);
	};
	// Bit is 0-7
	_actions[grules.push("opcode", "SET integer ',' '(' HL ')'")] = [](data& data)
	{
		const int bit = data._integer;

		if (bit > 7)
		{
			const std::string str(data.dollar(0).first,
				data.dollar(5).second);

			throw std::out_of_range(str + ": Bit out of range");
		}

		data.push_byte(0xCB);
		data.push_byte(0b11000110 | bit << 3);
	};
	// Bit is 0-7
	_actions[grules.push("opcode", "SET integer ',' '(' IX '+' expr ')'")] = [](data& data)
	{
		const int16_t bit = data._integer;

		if (bit > 7)
		{
			const std::string str(data.dollar(0).first,
				data.dollar(7).second);

			throw std::out_of_range(str + ": Bit out of range");
		}

		data.push_byte(0xDD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(6);
		data.push_byte(0b11000110 | bit << 3);
	};
	// Bit is 0-7
	_actions[grules.push("opcode", "SET integer ',' '(' IY '+' expr ')'")] = [](data& data)
	{
		const uint16_t bit = data._integer;

		if (bit > 7)
		{
			const std::string str(data.dollar(0).first,
				data.dollar(7).second);

			throw std::out_of_range(str + ": Bit out of range");
		}

		data.push_byte(0xFD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(6);
		data.push_byte(0b11000110 | bit << 3);
	};
	// Bit is 0-7
	_actions[grules.push("opcode", "RES integer ',' r")] = [](data& data)
	{
		const uint16_t bit = data._integer;

		if (bit > 7)
		{
			const std::string str(data.dollar(0).first,
				data.dollar(3).second);

			throw std::out_of_range(str + ": Bit out of range");
		}

		// Made local var to prevent VC++ warning
		const uint8_t by = 0b10000000 | bit << 3 | data._r;

		data.push_byte(0xCB);
		data.push_byte(by);
	};
	// Bit is 0-7
	_actions[grules.push("opcode", "RES integer ',' '(' HL ')'")] = [](data& data)
	{
		const uint16_t bit = data._integer;

		if (bit > 7)
		{
			const std::string str(data.dollar(0).first,
				data.dollar(5).second);

			throw std::out_of_range(str + ": Bit out of range");
		}

		data.push_byte(0xCB);
		data.push_byte(0b10000110 | bit << 3);
	};
	// Bit is 0-7
	_actions[grules.push("opcode", "RES integer ',' '(' IX '+' expr ')'")] = [](data& data)
	{
		const uint16_t bit = data._integer;

		if (bit > 7)
		{
			const std::string str(data.dollar(0).first,
				data.dollar(7).second);

			throw std::out_of_range(str + ": Bit out of range");
		}

		data.push_byte(0xDD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(6);
		data.push_byte(0b10000110 | bit << 3);
	};
	// Bit is 0-7
	_actions[grules.push("opcode", "RES integer ',' '(' IY '+' expr ')'")] = [](data& data)
	{
		const uint16_t bit = data._integer;

		if (bit > 7)
		{
			const std::string str(data.dollar(0).first,
				data.dollar(7).second);

			throw std::out_of_range(str + ": Bit out of range");
		}

		data.push_byte(0xFD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(6);
		data.push_byte(0b10000110 | bit << 3);
	};
	_actions[grules.push("opcode", "JP expr")] = [](data& data)
	{
		data.push_byte(0xC3);
		data.push_word(0);
		data.wexpr(1);
	};
	_actions[grules.push("opcode", "JP cc ',' expr")] = [](data& data)
	{
		data.push_byte(0b11000010 | data._cc << 3);
		data.push_word(0);
		data.wexpr(3);
	};
	_actions[grules.push("opcode", "JR expr")] = [](data& data)
	{
		data.push_byte(0x18);
		data.push_byte(0);
		data.rel_label(1);
	};
	_actions[grules.push("opcode", "JR C ',' expr")] = [](data& data)
	{
		data.push_byte(0x38);
		data.push_byte(0);
		data.rel_label(3);
	};
	_actions[grules.push("opcode", "JR NC ',' expr")] = [](data& data)
	{
		data.push_byte(0x30);
		data.push_byte(0);
		data.rel_label(3);
	};
	_actions[grules.push("opcode", "JR Z ',' expr")] = [](data& data)
	{
		data.push_byte(0x28);
		data.push_byte(0);
		data.rel_label(3);
	};
	_actions[grules.push("opcode", "JR NZ ',' expr")] = [](data& data)
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
	_actions[grules.push("opcode", "DJNZ expr")] = [](data& data)
	{
		data.push_byte(0x10);
		data.push_byte(0);
		data.rel_label(1);
	};
	_actions[grules.push("opcode", "CALL expr")] = [](data& data)
	{
		data.push_byte(0xCD);
		data.push_word(0);
		data.wexpr(1);
	};
	_actions[grules.push("opcode", "CALL cc ',' expr")] = [](data& data)
	{
		data.push_byte(0b11000100 | data._cc << 3);
		data.push_word(0);
		data.wexpr(3);
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
	// Only A register is legal
	_actions[grules.push("opcode", "IN r ',' '(' expr ')'")] = [](data& data)
	{
		if (data._r != 0b111)
		{
			const std::string str(data.dollar(0).first,
				data.dollar(5).second);

			throw std::runtime_error(str + ": Only register A valid");
		}

		data.push_byte(0xDB);
		data.push_byte(0);
		data.bexpr(4);
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
	_actions[grules.push("opcode", "OUT '(' expr ')' ',' A")] = [](data& data)
	{
		data.push_byte(0xD3);
		data.push_byte(0);
		data.bexpr(2);
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

	grules.push("expr", "item "
		"| expr '+' item "
		"| expr '-' item");
	grules.push("item", "Name "
		"| integer");

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
		const char* first = t.first + 1;
		uint8_t c = 0;

		if (*first == '\\')
		{
			++first;

			switch (*first)
			{
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				data._integer = 0;

				for (int i = 0; i < 3; ++i)
				{
					data._integer *= 8;
					data._integer += *first - '0';
				}

				break;
			case 'a':
				data._integer = '\a';
				break;
			case 'b':
				data._integer = '\b';
				break;
			case 'e':
				data._integer = 0x1b;
				break;
			case 'f':
				data._integer = '\f';
				break;
			case 'n':
				data._integer = '\n';
				break;
			case 'r':
				data._integer = '\r';
				break;
			case 't':
				data._integer = '\t';
				break;
			case 'v':
				data._integer = '\v';
				break;
			case 'x':
				data._integer = 0;

				for (int i = 0; i < 2; ++i)
				{
					++first;
					data._integer *= 10;

					if (*first >= 'a' && *first <= 'f')
						data._integer += *first - 'a' + 10;
					else if (*first >= 'A' && *first <= 'F')
						data._integer += *first - 'A' + 10;
					else
						data._integer += *first - '0';
				}

				break;
			case '\\':
				data._integer = '\\';
				break;
			case '\'':
				data._integer = '\'';
				break;
			case '"':
				data._integer = '"';
				break;
			case '?':
				data._integer = '?';
				break;
			}
		}
		else
		{
			data._integer = *first;
		}
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
	lrules.push("-", grules.token_id("'-'"));
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
	lrules.push("EQU", grules.token_id("EQU"));
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
	lrules.push(R"('(\\([abefnrtvx\\'"?]|\d{3}|x[\da-f]{2})|[^\\'])')", grules.token_id("Char"));
	lrules.push(R"(\d+)", grules.token_id("Integer"));
	lrules.push(R"('[^']{2,}'|\"[^"]{2,}\")", grules.token_id("String"));
	lrules.push("[_A-Z][0-9_A-Z]+", grules.token_id("Name"));
	lrules.push("[ \t]+|;.*|[/][*](?s:.)*?[*][/]", lrules.skip());
	lrules.push("\r?\n", grules.token_id("NL"));
	lexertl::generator::build(lrules, _lsm);
}

void data::build_expr_parser()
{
	parsertl::rules grules;
	lexertl::rules lrules(lexertl::dot_not_cr_lf | lexertl::icase);
	std::string warnings;

	grules.token("Binary Char Hex Integer Name");
	grules.left("'+' '-'");

	grules.push("expr", "item");
	_expr_actions[grules.push("expr", "expr '+' item")] = [](data& data)
	{
		auto rhs = data._acc.top();

		data._acc.pop();
		data._acc.top() += rhs;
	};
	_expr_actions[grules.push("expr", "expr '-' item")] = [](data& data)
	{
		auto rhs = data._acc.top();

		data._acc.pop();
		data._acc.top() -= rhs;
	};
	_expr_actions[grules.push("item", "Name")] = [](data& data)
	{
		const auto str = data._results.dollar(data._expr_gsm, 0, data._productions).str();
		auto lab = data._label.find(str);

		if (lab != data._label.end())
		{
			data._acc.push(data._org + lab->second);
			return;
		}

		auto equ = data._equ.find(str);

		if (equ == data._equ.end())
			throw std::runtime_error("Unknown label " + str);

		data._acc.push(atoi(equ->second.c_str()));
	};
	grules.push("item", "integer");
	_expr_actions[grules.push("integer", "Binary")] = [](data& data)
	{
		const auto& t = data._results.dollar(data._expr_gsm, 0, data._productions);
		char* end = nullptr;

		if (*t.first == '%')
			data._integer = static_cast<uint8_t>(strtol(t.first + 1, &end, 2));
		else
			data._integer = static_cast<uint8_t>(strtol(t.first, &end, 2));

		data._acc.push(data._integer);
	};
	_expr_actions[grules.push("integer", "Hex")] = [](data& data)
	{
		const auto& t = data._results.dollar(data._expr_gsm, 0, data._productions);
		char* end = nullptr;

		if (*t.first == '&' || *t.first == '$')
			data._integer = static_cast<uint16_t>(strtol(t.first + 1, &end, 16));
		else
			data._integer = static_cast<uint16_t>(strtol(t.first, &end, 16));

		data._acc.push(data._integer);
	};
	_expr_actions[grules.push("integer", "Char")] = [](data& data)
	{
		const auto& t = data._results.dollar(data._expr_gsm, 0, data._productions);
		const char* first = t.first + 1;
		uint8_t c = 0;

		if (*first == '\\')
		{
			++first;

			switch (*first)
			{
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				data._integer = 0;

				for (int i = 0; i < 3; ++i)
				{
					data._integer *= 8;
					data._integer += *first - '0';
				}

				break;
			case 'a':
				data._integer = '\a';
				break;
			case 'b':
				data._integer = '\b';
				break;
			case 'e':
				data._integer = 0x1b;
				break;
			case 'f':
				data._integer = '\f';
				break;
			case 'n':
				data._integer = '\n';
				break;
			case 'r':
				data._integer = '\r';
				break;
			case 't':
				data._integer = '\t';
				break;
			case 'v':
				data._integer = '\v';
				break;
			case 'x':
				data._integer = 0;

				for (int i = 0; i < 2; ++i)
				{
					++first;
					data._integer *= 10;

					if (*first >= 'a' && *first <= 'f')
						data._integer += *first - 'a' + 10;
					else if (*first >= 'A' && *first <= 'F')
						data._integer += *first - 'A' + 10;
					else
						data._integer += *first - '0';
				}

				break;
			case '\\':
				data._integer = '\\';
				break;
			case '\'':
				data._integer = '\'';
				break;
			case '"':
				data._integer = '"';
				break;
			case '?':
				data._integer = '?';
				break;
			}
		}
		else
		{
			data._integer = *first;
		}

		data._acc.push(data._integer);
	};
	_expr_actions[grules.push("integer", "Integer")] = [](data& data)
	{
		const auto& t = data._results.dollar(data._expr_gsm, 0, data._productions);

		data._integer = atoi(t.first);
		data._acc.push(data._integer);
	};
	parsertl::generator::build(grules, _expr_gsm, &warnings);

	if (!warnings.empty())
		throw std::runtime_error(warnings);

	lrules.push("[+]", grules.token_id("'+'"));
	lrules.push("-", grules.token_id("'-'"));
	lrules.push("%[01]{8}|[01]{8}b", grules.token_id("Binary"));
	lrules.push(R"('(\\([abefnrtvx\\'"?]|\d{3}|x[\da-f]{2})|[^\\'])')", grules.token_id("Char"));
	lrules.push("[&$][0-9A-Fa-f]{1,4}|[0-9A-Fa-f]{1,4}h", grules.token_id("Hex"));
	lrules.push(R"(\d+)", grules.token_id("Integer"));
	lrules.push("[_A-Z][0-9_A-Z]+", grules.token_id("Name"));
	lrules.push("[ \t]+|;.*|[/][*](?s:.)*?[*][/]", lrules.skip());
	lexertl::generator::build(lrules, _expr_lsm);
}

void data::parse(const char* first, const char* second)
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
		auto start = std::reverse_iterator<const char*>(iter->first);
		auto bol = std::find(start, std::reverse_iterator<const char*>(first), '\n');
		auto eol = std::find(iter->first, iter->eoi, '\n');
		std::string cmd((&*bol) + 1, eol[-1] == '\r' ? eol - 1 : eol);
		std::ostringstream ss;
		std::size_t count = 0;

		for (auto i = (&*bol) + 1, s = i, e = iter->first; i != e; ++i)
			if (*i == '\t')
				count += 8 - count % 8;
			else
				++count;

		ss << "Parser error, line " <<
			std::count(first, iter->first, '\n') + 1 <<
			" column " << bol - start + 1 << '\n' <<
			cmd << '\n' << std::string(count, ' ') << '^';
		throw std::runtime_error(ss.str());
	}
	else
	{
		for (auto& pair : _equ)
		{
			const uint16_t val = parse_expr(pair.second.c_str(),
				pair.second.c_str() + pair.second.size());

			pair.second = std::to_string(val);
		}

		for (const auto& pair : _rel_addr)
		{
			const uint16_t val = parse_expr(pair.second.c_str(),
				pair.second.c_str() + pair.second.size());
			const int off = static_cast<int>(val - (_org + pair.first + 1));

			if (off < -128 || off > 127)
				throw std::runtime_error("Out of range relative call to '" +
					pair.first + '\'');

			_memory[pair.first] = static_cast<uint8_t>(off);
		}

		for (const auto& pair : _byte_expr)
		{
			const uint16_t val = parse_expr(pair.second.c_str(),
				pair.second.c_str() + pair.second.size());

			if (val > 255)
				throw std::runtime_error(pair.second + " gives a value > 255");

			_memory[pair.first] = val & 0xff;
		}

		for (const auto& pair : _word_expr)
		{
			const uint16_t val = parse_expr(pair.second.c_str(),
				pair.second.c_str() + pair.second.size());

			_memory[pair.first] = val & 0xff;
			_memory[pair.first + 1] = static_cast<uint8_t>(val >> 8);
		}
	}
}

uint16_t data::parse_expr(const char* first, const char* second)
{
	uint16_t ret = 0;
	lexertl::citerator iter(first, second, _expr_lsm);

	_results.reset(iter->id, _expr_gsm);
	_productions.clear();

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

		parsertl::lookup(_expr_gsm, iter, _results, _productions);
	}

	ret = _acc.top();
	_acc.pop();
	return ret;
}
