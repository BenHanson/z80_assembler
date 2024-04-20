#include "data.hpp"
//#include <parsertl/debug.hpp>
#include "parsers.hpp"
#include <parsertl/generator.hpp>
#include "z80_error.hpp"

static void build_parser(data& d, const std::size_t flags)
{
	parsertl::rules grules;
	lexertl::rules lrules(*lexertl::regex_flags::dot_not_cr_lf | flags);
	std::string warnings;

	grules.token("A ADD ADC AF AF_PRIME AND B BC Binary BIT C CALL CCF Char CP CPD "
		"CPDR CPI CPIR CPL D DAA DB DS DW DE DEC DI DJNZ E EI EQU EX EXX F H "
		"HALT Hex HL Integer I IM IN INC IND INDR INI INIR IX IXh IXl "
		"IY IYh IYl JP JR L LD LDD LDDR LDI LDIR M Name NC NEG NL NOP NZ OR "
		"ORG OTDR OTIR OUT OUTD OUTI P PE PO PUSH POP R RES RET RETI RETN "
		"RL RLA RLC RLCA RLD RR RRA RRC RRCA RRD RST SBC SCF SET SLA SLL SP "
		"SRA SRL String SUB XOR Z");
	grules.left("'|' '&'");
	grules.left("'+' '-'");
	grules.left("'*' '/'");
	grules.precedence("UMINUS");

	grules.push("mnemonics", "line NL "
		"| mnemonics line NL");
	grules.push("line", "%empty | data");
	d._actions[grules.push("line", "Name EQU full_expr")] = [](data& data)
	{
		const auto& t = data.dollar(2);
		const uint16_t val = data.parse_expr(t.first, t.second);

		data._equ[data.dollar(0).str()] = val;
	};
	d._actions[grules.push("line", "ORG integer")] = [](data& data)
	{
		if (data._program._org_set)
			throw z80_error("ORG can only be set once");

		data._program._org = data._integer;
		data._program._org_set = true;
	};
	d._actions[grules.push("line", "opcode")] = [](data& data)
	{
		if (data._program._mem_type.empty() ||
			data._program._mem_type.back()._type != program::block::type::code)
		{
			data._program._mem_type.emplace_back(program::
				block(program::block::type::code,
				data._program._memory.size()));
		}
		else
			data._program._mem_type.back()._end = data._program._memory.size();
	};
	grules.push("line", "label opt_colon opt_opcode_data");
	d._actions[grules.push("label", "Name")] = [](data& data)
	{
		std::string name = data.dollar(0).str();

		if (auto iter = data._label.find(name);
			iter != data._label.end())
		{
			throw z80_error(name + " already exists");
		}

		data._label[name] = static_cast<uint16_t>(data._program._memory.size());
	};
	grules.push("opt_colon", "%empty | ':'");
	grules.push("opt_opcode_data", "%empty | data");
	d._actions[grules.push("opt_opcode_data", "opcode")] = [](data& data)
	{
		if (data._program._mem_type.empty() ||
			data._program._mem_type.back()._type !=
			program::block::type::code)
		{
			data._program._mem_type.emplace_back(program::
				block(program::block::type::code, data._program._memory.size()));
		}
		else
			data._program._mem_type.back()._end = data._program._memory.size();
	};
	d._actions[grules.push("data", "DB db_list")] = [](data& data)
	{
		if (data._program._mem_type.empty() ||
			data._program._mem_type.back()._type != program::block::type::db)
		{
			data._program._mem_type.emplace_back(program::
				block(program::block::type::db, data._program._memory.size()));
		}
		else
			data._program._mem_type.back()._end = data._program._memory.size();
	};
	d._actions[grules.push("data", "DS integer")] = [](data& data)
	{
		data._program._memory.insert(data._program._memory.end(), data._integer, 0);

		if (data._program._mem_type.empty() ||
			data._program._mem_type.back()._type != program::block::type::ds)
		{
			data._program._mem_type.emplace_back(program::
				block(program::block::type::ds, data._program._memory.size()));
		}
		else
			data._program._mem_type.back()._end = data._program._memory.size();
	};
	d._actions[grules.push("data", "DW dw_list")] = [](data& data)
	{
		if (data._program._mem_type.empty() ||
			data._program._mem_type.back()._type != program::block::type::dw)
		{
			data._program._mem_type.emplace_back(program::
				block(program::block::type::dw,
				data._program._memory.size()));
		}
		else
			data._program._mem_type.back()._end = data._program._memory.size();
	};
	d._actions[grules.push("db_list", "full_expr")] = [](data& data)
	{
		data.push_byte(0);
		data.bexpr(0);
	};
	d._actions[grules.push("db_list", "String")] = [](data& data)
	{
		std::string str = data.dollar(0).substr(1, 1);
		std::size_t idx = str.find('\\');

		while (idx != std::string::npos)
		{
			str.erase(idx, 1);
			idx = str.find('\\', idx + 1);
		}

		for (const char* iter = str.c_str(), *end = iter + str.size();
			iter != end; ++iter)
		{
			data.push_byte(static_cast<uint8_t>(*iter));
		}
	};
	d._actions[grules.push("db_list", "db_list ',' full_expr")] = [](data& data)
	{
		data.push_byte(0);
		data.bexpr(2);
	};
	d._actions[grules.push("db_list", "db_list ',' String")] = [](data& data)
	{
		std::string str = data.dollar(2).substr(1, 1);
		std::size_t idx = str.find('\\');

		while (idx != std::string::npos)
		{
			str.erase(idx, 1);
			idx = str.find('\\', idx + 1);
		}

		for (const char* iter = str.c_str(), *end = iter + str.size();
			iter != end; ++iter)
		{
			data.push_byte(static_cast<uint8_t>(*iter));
		}
	};
	d._actions[grules.push("dw_list", "full_expr")] = [](data& data)
	{
		data.push_word(0);
		data.wexpr(2, 0);
	};
	d._actions[grules.push("dw_list", "dw_list ',' full_expr")] = [](data& data)
	{
		data.push_word(0);
		data.wexpr(2, 2);
	};
	d._actions[grules.push("opcode", R"(LD r ',' r2)")] = [](data& data)
	{
		// Made local var to prevent VC++ warning
		const uint8_t by = (0b01000000 | data._r << 3 | data._r2) & 0xff;

		data.push_byte(by);
	};
	d._actions[grules.push("opcode", R"(LD r ',' IXh)")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte((0b01000100 | data._r << 3) & 0xff);
	};
	d._actions[grules.push("opcode", R"(LD r ',' IXl)")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte((0b01000101 | data._r << 3) & 0xff);
	};
	d._actions[grules.push("opcode", R"(LD r ',' IYh)")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte((0b01000100 | data._r << 3) & 0xff);
	};
	d._actions[grules.push("opcode", R"(LD r ',' IYl)")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte((0b01000101 | data._r << 3) & 0xff);
	};
	d._actions[grules.push("opcode", "LD r ',' expr")] = [](data& data)
	{
		data.push_byte((0b00000110 | data._r << 3) & 0xff);
		data.push_byte(0);
		data.bexpr(3);
	};
	d._actions[grules.push("opcode", "LD IXh ',' full_expr")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0x26);
		data.push_byte(0);
		data.bexpr(3);
	};
	d._actions[grules.push("opcode", "LD IXl ',' full_expr")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0x2E);
		data.push_byte(0);
		data.bexpr(3);
	};
	d._actions[grules.push("opcode", "LD IYh ',' full_expr")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0x26);
		data.push_byte(0);
		data.bexpr(3);
	};
	d._actions[grules.push("opcode", "LD IYl ',' full_expr")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0x2E);
		data.push_byte(0);
		data.bexpr(3);
	};
	d._actions[grules.push("opcode", "LD r ',' '(' HL ')'")] = [](data& data)
	{
		data.push_byte((0b01000110 | data._r << 3) & 0xff);
	};
	d._actions[grules.push("opcode", "LD r ',' '(' IX plus_minus expr ')'")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte((0b01000110 | data._r << 3) & 0xff);
		data.push_byte(0);
		data.bexpr(6, data._plus_minus);
	};
	d._actions[grules.push("opcode", "LD r ',' '(' IY plus_minus expr ')'")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte((0b01000110 | data._r << 3) & 0xff);
		data.push_byte(0);
		data.bexpr(6, data._plus_minus);
	};
	d._actions[grules.push("opcode", "LD '(' HL ')' ',' r")] = [](data& data)
	{
		data.push_byte(0b01110000 | data._r);
	};
	d._actions[grules.push("opcode", "LD '(' IX plus_minus expr ')' ',' r")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0b01110000 | data._r);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
	};
	d._actions[grules.push("opcode", "LD '(' IY plus_minus expr ')' ',' r")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0b01110000 | data._r);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
	};
	d._actions[grules.push("opcode", "LD '(' HL ')' ',' full_expr")] = [](data& data)
	{
		data.push_byte(0x36);
		data.push_byte(0);
		data.bexpr(5);
	};
	d._actions[grules.push("opcode", "LD '(' IX plus_minus expr ')' ',' full_expr")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0x36);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
		data.push_byte(0);
		data.bexpr(7);
	};
	d._actions[grules.push("opcode", "LD '(' IY plus_minus expr ')' ',' full_expr")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0x36);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
		data.push_byte(0);
		data.bexpr(7);
	};
	// Only A register is legal
	d._actions[grules.push("opcode", "LD r ',' '(' BC ')'")] = [](data& data)
	{
		if (data._r != 0b111)
		{
			const std::string str(data.dollar(0).first,
				data.dollar(5).second);

			throw z80_error(str + ": Only register A is valid");
		}

		data.push_byte(0x0A);
	};
	// Only A register is legal
	d._actions[grules.push("opcode", "LD r ',' '(' DE ')'")] = [](data& data)
	{
		if (data._r != 0b111)
		{
			const std::string str(data.dollar(0).first,
				data.dollar(5).second);

			throw z80_error(str + ": Only register A is valid");
		}

		data.push_byte(0x1A);
	};
	// Only A register is legal
	d._actions[grules.push("opcode", "LD r ',' '(' expr ')'")] = [](data& data)
	{
		if (data._r != 0b111)
		{
			const std::string str(data.dollar(0).first,
				data.dollar(5).second);

			throw z80_error(str + ": Only register A is valid");
		}

		data.push_byte(0x3A);
		data.push_word(0);
		data.wexpr(3, 4);
	};
	d._actions[grules.push("opcode", "LD '(' BC ')' ',' A")] = [](data& data)
	{
		data.push_byte(0x02);
	};
	d._actions[grules.push("opcode", "LD '(' DE ')' ',' A")] = [](data& data)
	{
		data.push_byte(0x12);
	};
	d._actions[grules.push("opcode", "LD '(' expr ')' ',' A")] = [](data& data)
	{
		data.push_byte(0x32);
		data.push_word(0);
		data.wexpr(3, 2);
	};
	// Only A register is legal
	d._actions[grules.push("opcode", "LD r ',' I")] = [](data& data)
	{
		if (data._r != 0b111)
		{
			const std::string str(data.dollar(0).first,
				data.dollar(3).second);

			throw z80_error(str + ": Only register A is valid");
		}

		data.push_byte(0xED);
		data.push_byte(0x57);
	};
	// Only A register is legal
	d._actions[grules.push("opcode", "LD r ',' R")] = [](data& data)
	{
		if (data._r != 0b111)
		{
			const std::string str(data.dollar(0).first,
				data.dollar(3).second);

			throw z80_error(str + ": Only register A is valid");
		}

		data.push_byte(0xED);
		data.push_byte(0x5F);
	};
	d._actions[grules.push("opcode", "LD I ',' A")] = [](data& data)
	{
		data.push_byte(0xED);
		data.push_byte(0x47);
	};
	d._actions[grules.push("opcode", "LD R ',' A")] = [](data& data)
	{
		data.push_byte(0xED);
		data.push_byte(0x4F);
	};
	d._actions[grules.push("opcode", "LD dd ',' expr")] = [](data& data)
	{
		data.push_byte((0b00000001 | data._dd << 4) & 0xff);
		data.push_word(0);
		data.wexpr(3, 3);
	};
	d._actions[grules.push("opcode", "LD IX ',' expr")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0x21);
		data.push_word(0);
		data.wexpr(4, 3);
	};
	d._actions[grules.push("opcode", "LD IXh ',' r")] = [](data& data)
	{
		if (data._r == 0b100)
		{
			const std::string str(data.dollar(0).first,
				data.dollar(3).second);

			throw z80_error(str + ": Register H not allowed");
		}

		if (data._r == 0b101)
		{
			const std::string str(data.dollar(0).first,
				data.dollar(3).second);

			throw z80_error(str + ": Register L not allowed");
		}

		data.push_byte(0xDD);
		data.push_byte(0b01100000 | data._r);
	};
	d._actions[grules.push("opcode", "LD IXh ',' IXh")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0x64);
	};
	d._actions[grules.push("opcode", "LD IXh ',' IXl")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0x65);
	};
	d._actions[grules.push("opcode", "LD IXl ',' r")] = [](data& data)
	{
		if (data._r == 0b100)
		{
			const std::string str(data.dollar(0).first,
				data.dollar(3).second);

			throw z80_error(str + ": Register H not allowed");
		}

		if (data._r == 0b101)
		{
			const std::string str(data.dollar(0).first,
				data.dollar(3).second);

			throw z80_error(str + ": Register L not allowed");
		}

		data.push_byte(0xDD);
		data.push_byte(0b01101000 | data._r);
	};
	d._actions[grules.push("opcode", "LD IXl ',' IXh")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0x6C);
	};
	d._actions[grules.push("opcode", "LD IXl ',' IXl")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0x6D);
	};
	d._actions[grules.push("opcode", "LD IY ',' expr")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0x21);
		data.push_word(0);
		data.wexpr(4, 3);
	};
	d._actions[grules.push("opcode", "LD IYh ',' r")] = [](data& data)
	{
		if (data._r == 0b100)
		{
			const std::string str(data.dollar(0).first,
				data.dollar(3).second);

			throw z80_error(str + ": Register H not allowed");
		}

		if (data._r == 0b101)
		{
			const std::string str(data.dollar(0).first,
				data.dollar(3).second);

			throw z80_error(str + ": Register L not allowed");
		}

		data.push_byte(0xFD);
		data.push_byte(0b01100000 | data._r);
	};
	d._actions[grules.push("opcode", "LD IYh ',' IYh")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0x64);
	};
	d._actions[grules.push("opcode", "LD IYh ',' IYl")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0x65);
	};
	d._actions[grules.push("opcode", "LD IYl ',' r")] = [](data& data)
	{
		if (data._r == 0b100)
		{
			const std::string str(data.dollar(0).first,
				data.dollar(3).second);

			throw z80_error(str + ": Register H not allowed");
		}

		if (data._r == 0b101)
		{
			const std::string str(data.dollar(0).first,
				data.dollar(3).second);

			throw z80_error(str + ": Register L not allowed");
		}

		data.push_byte(0xFD);
		data.push_byte(0b01101000 | data._r);
	};
	d._actions[grules.push("opcode", "LD IYl ',' IYh")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0x6C);
	};
	d._actions[grules.push("opcode", "LD IYl ',' IYl")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0x6D);
	};
	d._actions[grules.push("opcode", "LD dd ',' '(' expr ')'")] = [](data& data)
	{
		if (data._dd == 0b10)
		{
			// HL
			data.push_byte(0x2A);
		}
		else
		{
			data.push_byte(0xED);
			data.push_byte((0b01001011 | data._dd << 4) & 0xff);
		}

		data.push_word(0);
		data.wexpr(data._dd == 0b10 ? 3 : 4, 4);
	};
	d._actions[grules.push("opcode", "LD IX ',' '(' expr ')'")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0x2A);
		data.push_word(0);
		data.wexpr(4, 4);
	};
	d._actions[grules.push("opcode", "LD IY ',' '(' expr ')'")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0x2A);
		data.push_word(0);
		data.wexpr(4, 4);
	};
	d._actions[grules.push("opcode", "LD '(' expr ')' ',' dd")] = [](data& data)
	{
		// HL
		if (data._dd == 0b10)
		{
			data.push_byte(0x22);
		}
		else
		{
			data.push_byte(0xED);
			data.push_byte((0b01000011 | data._dd << 4) & 0xff);
		}

		data.push_word(0);
		data.wexpr(data._dd == 0b10 ? 3 : 4, 2);
	};
	d._actions[grules.push("opcode", "LD '(' expr ')' ',' IX")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0x22);
		data.push_word(0);
		data.wexpr(4, 2);
	};
	d._actions[grules.push("opcode", "LD '(' expr ')' ',' IY")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0x22);
		data.push_word(0);
		data.wexpr(4, 2);
	};
	// Only SP register is legal
	d._actions[grules.push("opcode", "LD dd ',' HL")] = [](data& data)
	{
		if (data._dd != 0b11)
		{
			const std::string str(data.dollar(0).first,
				data.dollar(3).second);

			throw z80_error(str + ": Only register SP is valid");
		}

		data.push_byte(0xF9);
	};
	// Only SP register is legal
	d._actions[grules.push("opcode", "LD dd ',' IX")] = [](data& data)
	{
		if (data._dd != 0b11)
		{
			const std::string str(data.dollar(0).first,
				data.dollar(3).second);

			throw z80_error(str + ": Only register SP is valid");
		}

		data.push_byte(0xDD);
		data.push_byte(0xF9);
	};
	// Only SP register is legal
	d._actions[grules.push("opcode", "LD dd ',' IY")] = [](data& data)
	{
		if (data._dd != 0b11)
		{
			const std::string str(data.dollar(0).first,
				data.dollar(3).second);

			throw z80_error(str + ": Only register SP is valid");
		}

		data.push_byte(0xFD);
		data.push_byte(0xF9);
	};
	d._actions[grules.push("opcode", "PUSH qq")] = [](data& data)
	{
		data.push_byte((0b11000101 | data._qq << 4) & 0xff);
	};
	d._actions[grules.push("opcode", "PUSH IX")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0xE5);
	};
	d._actions[grules.push("opcode", "PUSH IY")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0xE5);
	};
	d._actions[grules.push("opcode", "POP qq")] = [](data& data)
	{
		data.push_byte((0b11000001 | data._qq << 4) & 0xff);
	};
	d._actions[grules.push("opcode", "POP IX")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0xE1);
	};
	d._actions[grules.push("opcode", "POP IY")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0xE1);
	};
	d._actions[grules.push("opcode", "EX DE ',' HL")] = [](data& data)
	{
		data.push_byte(0xEB);
	};
	d._actions[grules.push("opcode", "EX AF ',' AF_PRIME")] = [](data& data)
	{
		data.push_byte(0x08);
	};
	d._actions[grules.push("opcode", "EXX")] = [](data& data)
	{
		data.push_byte(0xD9);
	};
	d._actions[grules.push("opcode", "EX '(' SP ')' ',' HL")] = [](data& data)
	{
		data.push_byte(0xE3);
	};
	d._actions[grules.push("opcode", "EX '(' SP ')' ',' IX")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0xE3);
	};
	d._actions[grules.push("opcode", "EX '(' SP ')' ',' IY")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0xE3);
	};
	d._actions[grules.push("opcode", "LDI")] = [](data& data)
	{
		data.push_byte(0xED);
		data.push_byte(0xA0);
	};
	d._actions[grules.push("opcode", "LDIR")] = [](data& data)
	{
		data.push_byte(0xED);
		data.push_byte(0xB0);
	};
	d._actions[grules.push("opcode", "LDD")] = [](data& data)
	{
		data.push_byte(0xED);
		data.push_byte(0xA8);
	};
	d._actions[grules.push("opcode", "LDDR")] = [](data& data)
	{
		data.push_byte(0xED);
		data.push_byte(0xB8);
	};
	d._actions[grules.push("opcode", "CPI")] = [](data& data)
	{
		data.push_byte(0xED);
		data.push_byte(0xA1);
	};
	d._actions[grules.push("opcode", "CPIR")] = [](data& data)
	{
		data.push_byte(0xED);
		data.push_byte(0xB1);
	};
	d._actions[grules.push("opcode", "CPD")] = [](data& data)
	{
		data.push_byte(0xED);
		data.push_byte(0xA9);
	};
	d._actions[grules.push("opcode", "CPDR")] = [](data& data)
	{
		data.push_byte(0xED);
		data.push_byte(0xB9);
	};
	d._actions[grules.push("opcode", "ADD A ',' r")] = [](data& data)
	{
		data.push_byte(0b10000000 | data._r);
	};
	d._actions[grules.push("opcode", "ADD A ',' IXh")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0x84);
	};
	d._actions[grules.push("opcode", "ADD A ',' IXl")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0x85);
	};
	d._actions[grules.push("opcode", "ADD A ',' IYh")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0x84);
	};
	d._actions[grules.push("opcode", "ADD A ',' IYl")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0x85);
	};
	d._actions[grules.push("opcode", "ADD A ',' expr")] = [](data& data)
	{
		data.push_byte(0b11000110);
		data.push_byte(0);
		data.bexpr(3);
	};
	d._actions[grules.push("opcode", "ADD A ',' '(' HL ')'")] = [](data& data)
	{
		data.push_byte(0x86);
	};
	d._actions[grules.push("opcode", "ADD A ',' '(' IX plus_minus expr ')'")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0x86);
		data.push_byte(0);
		data.bexpr(6, data._plus_minus);
	};
	d._actions[grules.push("opcode", "ADD A ',' '(' IY plus_minus expr ')'")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0x86);
		data.push_byte(0);
		data.bexpr(6, data._plus_minus);
	};
	d._actions[grules.push("opcode", "ADC A ',' r")] = [](data& data)
	{
		data.push_byte(0b10001000 | data._r);
	};
	d._actions[grules.push("opcode", "ADC A ',' IXh")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0x8C);
	};
	d._actions[grules.push("opcode", "ADC A ',' IXl")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0x8D);
	};
	d._actions[grules.push("opcode", "ADC A ',' IYh")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0x8C);
	};
	d._actions[grules.push("opcode", "ADC A ',' IYl")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0x8D);
	};
	d._actions[grules.push("opcode", "ADC A ',' expr")] = [](data& data)
	{
		data.push_byte(0xCE);
		data.push_byte(0);
		data.bexpr(3);
	};
	d._actions[grules.push("opcode", "ADC A ',' '(' HL ')'")] = [](data& data)
	{
		data.push_byte(0x8E);
	};
	d._actions[grules.push("opcode", "ADC A ',' '(' IX plus_minus expr ')'")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0x8E);
		data.push_byte(0);
		data.bexpr(6, data._plus_minus);
	};
	d._actions[grules.push("opcode", "ADC A ',' '(' IY plus_minus expr ')'")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0x8E);
		data.push_byte(0);
		data.bexpr(6, data._plus_minus);
	};
	d._actions[grules.push("opcode", "SUB r")] = [](data& data)
	{
		data.push_byte(0b10010000 | data._r);
	};
	d._actions[grules.push("opcode", "SUB IXh")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0x94);
	};
	d._actions[grules.push("opcode", "SUB IXl")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0x95);
	};
	d._actions[grules.push("opcode", "SUB IYh")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0x94);
	};
	d._actions[grules.push("opcode", "SUB IYl")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0x95);
	};
	d._actions[grules.push("opcode", "SUB expr")] = [](data& data)
	{
		data.push_byte(0xD6);
		data.push_byte(0);
		data.bexpr(1);
	};
	d._actions[grules.push("opcode", "SUB '(' HL ')'")] = [](data& data)
	{
		data.push_byte(0x96);
	};
	d._actions[grules.push("opcode", "SUB '(' IX plus_minus expr ')'")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0x96);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
	};
	d._actions[grules.push("opcode", "SUB '(' IY plus_minus expr ')'")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0x96);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
	};
	d._actions[grules.push("opcode", "SBC A ',' r")] = [](data& data)
	{
		data.push_byte(0b10011000 | data._r);
	};
	d._actions[grules.push("opcode", "SBC A ',' IXh")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0x9C);
	};
	d._actions[grules.push("opcode", "SBC A ',' IXl")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0x9D);
	};
	d._actions[grules.push("opcode", "SBC A ',' IYh")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0x9C);
	};
	d._actions[grules.push("opcode", "SBC A ',' IYl")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0x9D);
	};
	d._actions[grules.push("opcode", "SBC A ',' expr")] = [](data& data)
	{
		data.push_byte(0xDE);
		data.push_byte(0);
		data.bexpr(3);
	};
	d._actions[grules.push("opcode", "SBC A ',' '(' HL ')'")] = [](data& data)
	{
		data.push_byte(0x9E);
	};
	d._actions[grules.push("opcode", "SBC A ',' '(' IX plus_minus expr ')'")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0x9E);
		data.push_byte(0);
		data.bexpr(6, data._plus_minus);
	};
	d._actions[grules.push("opcode", "SBC A ',' '(' IY plus_minus expr ')'")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0x9E);
		data.push_byte(0);
		data.bexpr(6, data._plus_minus);
	};
	d._actions[grules.push("opcode", "AND r")] = [](data& data)
	{
		data.push_byte(0b10100000 | data._r);
	};
	d._actions[grules.push("opcode", "AND IXh")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0xA4);
	};
	d._actions[grules.push("opcode", "AND IXl")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0xA5);
	};
	d._actions[grules.push("opcode", "AND IYh")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0xA4);
	};
	d._actions[grules.push("opcode", "AND IYl")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0xA5);
	};
	d._actions[grules.push("opcode", "AND expr")] = [](data& data)
	{
		data.push_byte(0xE6);
		data.push_byte(0);
		data.bexpr(1);
	};
	d._actions[grules.push("opcode", "AND '(' HL ')'")] = [](data& data)
	{
		data.push_byte(0xA6);
	};
	d._actions[grules.push("opcode", "AND '(' IX plus_minus expr ')'")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0xA6);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
	};
	d._actions[grules.push("opcode", "AND '(' IY plus_minus expr ')'")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0xA6);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
	};
	d._actions[grules.push("opcode", "OR r")] = [](data& data)
	{
		data.push_byte(0b10110000 | data._r);
	};
	d._actions[grules.push("opcode", "OR IXh")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0xB4);
	};
	d._actions[grules.push("opcode", "OR IXl")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0xB5);
	};
	d._actions[grules.push("opcode", "OR IYh")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0xB4);
	};
	d._actions[grules.push("opcode", "OR IYl")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0xB5);
	};
	d._actions[grules.push("opcode", "OR expr")] = [](data& data)
	{
		data.push_byte(0xF6);
		data.push_byte(0);
		data.bexpr(1);
	};
	d._actions[grules.push("opcode", "OR '(' HL ')'")] = [](data& data)
	{
		data.push_byte(0xB6);
	};
	d._actions[grules.push("opcode", "OR '(' IX plus_minus expr ')'")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0xB6);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
	};
	d._actions[grules.push("opcode", "OR '(' IY plus_minus expr ')'")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0xB6);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
	};
	d._actions[grules.push("opcode", "XOR r")] = [](data& data)
	{
		data.push_byte(0b10101000 | data._r);
	};
	d._actions[grules.push("opcode", "XOR IXh")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0xAC);
	};
	d._actions[grules.push("opcode", "XOR IXl")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0xAD);
	};
	d._actions[grules.push("opcode", "XOR IYh")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0xAC);
	};
	d._actions[grules.push("opcode", "XOR IYl")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0xAD);
	};
	d._actions[grules.push("opcode", "XOR expr")] = [](data& data)
	{
		data.push_byte(0xEE);
		data.push_byte(0);
		data.bexpr(1);
	};
	d._actions[grules.push("opcode", "XOR '(' HL ')'")] = [](data& data)
	{
		data.push_byte(0xAE);
	};
	d._actions[grules.push("opcode", "XOR '(' IX plus_minus expr ')'")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0xAE);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
	};
	d._actions[grules.push("opcode", "XOR '(' IY plus_minus expr ')'")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0xAE);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
	};
	d._actions[grules.push("opcode", "CP r")] = [](data& data)
	{
		data.push_byte(0b10111000 | data._r);
	};
	d._actions[grules.push("opcode", "CP IXh")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0xBC);
	};
	d._actions[grules.push("opcode", "CP IXl")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0xBD);
	};
	d._actions[grules.push("opcode", "CP IYh")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0xBC);
	};
	d._actions[grules.push("opcode", "CP IYl")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0xBD);
	};
	d._actions[grules.push("opcode", "CP full_expr")] = [](data& data)
	{
		data.push_byte(0xFE);
		data.push_byte(0);
		data.bexpr(1);
	};
	d._actions[grules.push("opcode", "CP '(' HL ')'")] = [](data& data)
	{
		data.push_byte(0xBE);
	};
	d._actions[grules.push("opcode", "CP '(' IX plus_minus expr ')'")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0xBE);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
	};
	d._actions[grules.push("opcode", "CP '(' IY plus_minus expr ')'")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0xBE);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
	};
	d._actions[grules.push("opcode", "INC r")] = [](data& data)
	{
		data.push_byte((0b00000100 | data._r << 3) & 0xff);
	};
	d._actions[grules.push("opcode", "INC IXh")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0x24);
	};
	d._actions[grules.push("opcode", "INC IXl")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0x2C);
	};
	d._actions[grules.push("opcode", "INC IYh")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0x24);
	};
	d._actions[grules.push("opcode", "INC IYl")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0x2C);
	};
	d._actions[grules.push("opcode", "INC '(' HL ')'")] = [](data& data)
	{
		data.push_byte(0x34);
	};
	d._actions[grules.push("opcode", "INC '(' IX plus_minus expr ')'")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0x34);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
	};
	d._actions[grules.push("opcode", "INC '(' IY plus_minus expr ')'")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0x34);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
	};
	d._actions[grules.push("opcode", "DEC r")] = [](data& data)
	{
		data.push_byte((0b00000101 | data._r << 3) & 0xff);
	};
	d._actions[grules.push("opcode", "DEC IXh")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0x25);
	};
	d._actions[grules.push("opcode", "DEC IXl")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0x2D);
	};
	d._actions[grules.push("opcode", "DEC IYh")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0x25);
	};
	d._actions[grules.push("opcode", "DEC IYl")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0x2D);
	};
	d._actions[grules.push("opcode", "DEC '(' HL ')'")] = [](data& data)
	{
		data.push_byte(0x35);
	};
	d._actions[grules.push("opcode", "DEC '(' IX plus_minus expr ')'")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0x35);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
	};
	d._actions[grules.push("opcode", "DEC '(' IY plus_minus expr ')'")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0x35);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
	};
	d._actions[grules.push("opcode", "DAA")] = [](data& data)
	{
		data.push_byte(0x27);
	};
	d._actions[grules.push("opcode", "CPL")] = [](data& data)
	{
		data.push_byte(0x2F);
	};
	d._actions[grules.push("opcode", "NEG")] = [](data& data)
	{
		data.push_byte(0xED);
		data.push_byte(0x44);
	};
	d._actions[grules.push("opcode", "CCF")] = [](data& data)
	{
		data.push_byte(0x3F);
	};
	d._actions[grules.push("opcode", "SCF")] = [](data& data)
	{
		data.push_byte(0x37);
	};
	d._actions[grules.push("opcode", "NOP")] = [](data& data)
	{
		data.push_byte(0);
	};
	d._actions[grules.push("opcode", "HALT")] = [](data& data)
	{
		data.push_byte(0x76);
	};
	d._actions[grules.push("opcode", "DI")] = [](data& data)
	{
		data.push_byte(0xF3);
	};
	d._actions[grules.push("opcode", "EI")] = [](data& data)
	{
		data.push_byte(0xFB);
	};
	// Integer can be 0, 1, 2
	d._actions[grules.push("opcode", "IM Integer")] = [](data& data)
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
	d._actions[grules.push("opcode", "ADD HL ',' dd")] = [](data& data)
	{
		data.push_byte((0b00001001 | data._dd << 4) & 0xff);
	};
	d._actions[grules.push("opcode", "ADC HL ',' dd")] = [](data& data)
	{
		data.push_byte(0xED);
		data.push_byte((0b01001010 | data._dd << 4) & 0xff);
	};
	d._actions[grules.push("opcode", "SBC HL ',' dd")] = [](data& data)
	{
		data.push_byte(0xED);
		data.push_byte((0b01000010 | data._dd << 4) & 0xff);
	};
	d._actions[grules.push("opcode", "ADD IX ',' pp")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte((0b00001001 | data._pp << 4) & 0xff);
	};
	d._actions[grules.push("opcode", "ADD IY ',' rr")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte((0b00001001 | data._rr << 4) & 0xff);
	};
	d._actions[grules.push("opcode", "INC dd")] = [](data& data)
	{
		data.push_byte((0b00000011 | data._dd << 4) & 0xff);
	};
	d._actions[grules.push("opcode", "INC IX")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0x23);
	};
	d._actions[grules.push("opcode", "INC IY")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0x23);
	};
	d._actions[grules.push("opcode", "DEC dd")] = [](data& data)
	{
		data.push_byte((0b00001011 | data._dd << 4) & 0xff);
	};
	d._actions[grules.push("opcode", "DEC IX")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0x2B);
	};
	d._actions[grules.push("opcode", "DEC IY")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0x2B);
	};
	d._actions[grules.push("opcode", "RLA")] = [](data& data)
	{
		data.push_byte(0x17);
	};
	d._actions[grules.push("opcode", "RLCA")] = [](data& data)
	{
		data.push_byte(0x07);
	};
	d._actions[grules.push("opcode", "RRCA")] = [](data& data)
	{
		data.push_byte(0x0F);
	};
	d._actions[grules.push("opcode", "RRC '(' IX plus_minus expr ')' ',' r")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
		data.push_byte(0b00001000 | data._r);
	};
	d._actions[grules.push("opcode", "RRC '(' IY plus_minus expr ')' ',' r")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
		data.push_byte(0b00001000 | data._r);
	};
	d._actions[grules.push("opcode", "RR '(' IX plus_minus expr ')' ',' r")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
		data.push_byte(0b00011000 | data._r);
	};
	d._actions[grules.push("opcode", "RR '(' IY plus_minus expr ')' ',' r")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
		data.push_byte(0b00011000 | data._r);
	};
	d._actions[grules.push("opcode", "RRA")] = [](data& data)
	{
		data.push_byte(0x1F);
	};
	d._actions[grules.push("opcode", "RLC r")] = [](data& data)
	{
		data.push_byte(0xCB);
		data.push_byte(data._r);
	};
	d._actions[grules.push("opcode", "RLC '(' HL ')'")] = [](data& data)
	{
		data.push_byte(0xCB);
		data.push_byte(0x06);
	};
	d._actions[grules.push("opcode", "RLC '(' IX plus_minus expr ')'")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
		data.push_byte(0x06);
	};
	d._actions[grules.push("opcode", "RLC '(' IX plus_minus expr ')' ',' r")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
		data.push_byte(data._r);
	};
	d._actions[grules.push("opcode", "RLC '(' IY plus_minus expr ')'")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
		data.push_byte(0x06);
	};
	d._actions[grules.push("opcode", "RLC '(' IY plus_minus expr ')' ',' r")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
		data.push_byte(data._r);
	};
	d._actions[grules.push("opcode", "RL r")] = [](data& data)
	{
		data.push_byte(0xCB);
		data.push_byte(0b00010000 | data._r);
	};
	d._actions[grules.push("opcode", "RL '(' HL ')'")] = [](data& data)
	{
		data.push_byte(0xCB);
		data.push_byte(0x16);
	};
	d._actions[grules.push("opcode", "RL '(' IX plus_minus expr ')'")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
		data.push_byte(0x16);
	};
	d._actions[grules.push("opcode", "RL '(' IX plus_minus expr ')' ',' r")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
		data.push_byte(0b00010000 | data._r);
	};
	d._actions[grules.push("opcode", "RL '(' IY plus_minus expr ')'")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
		data.push_byte(0x16);
	};
	d._actions[grules.push("opcode", "RL '(' IY plus_minus expr ')' ',' r")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
		data.push_byte(0b00010000 | data._r);
	};
	d._actions[grules.push("opcode", "RRC r")] = [](data& data)
	{
		data.push_byte(0xCB);
		data.push_byte(0b00001000 | data._r);
	};
	d._actions[grules.push("opcode", "RRC '(' HL ')'")] = [](data& data)
	{
		data.push_byte(0xCB);
		data.push_byte(0x0E);
	};
	d._actions[grules.push("opcode", "RRC '(' IX plus_minus expr ')'")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
		data.push_byte(0x0E);
	};
	d._actions[grules.push("opcode", "RRC '(' IY plus_minus expr ')'")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
		data.push_byte(0x0E);
	};
	d._actions[grules.push("opcode", "RR r")] = [](data& data)
	{
		data.push_byte(0xCB);
		data.push_byte(0b00011000 | data._r);
	};
	d._actions[grules.push("opcode", "RR '(' HL ')'")] = [](data& data)
	{
		data.push_byte(0xCB);
		data.push_byte(0x1E);
	};
	d._actions[grules.push("opcode", "RR '(' IX plus_minus expr ')'")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
		data.push_byte(0x1E);
	};
	d._actions[grules.push("opcode", "RR '(' IY plus_minus expr ')'")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
		data.push_byte(0x1E);
	};
	d._actions[grules.push("opcode", "SLA r")] = [](data& data)
	{
		data.push_byte(0xCB);
		data.push_byte(0b00100000 | data._r);
	};
	d._actions[grules.push("opcode", "SLA '(' HL ')'")] = [](data& data)
	{
		data.push_byte(0xCB);
		data.push_byte(0x26);
	};
	d._actions[grules.push("opcode", "SLA '(' IX plus_minus expr ')'")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
		data.push_byte(0x26);
	};
	d._actions[grules.push("opcode", "SLA '(' IX plus_minus expr ')' ',' r")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
		data.push_byte(0b00100000 | data._r);
	};
	d._actions[grules.push("opcode", "SLA '(' IY plus_minus expr ')'")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
		data.push_byte(0x26);
	};
	d._actions[grules.push("opcode", "SLA '(' IY plus_minus expr ')' ',' r")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
		data.push_byte(0b00100000 | data._r);
	};
	d._actions[grules.push("opcode", "SLL r")] = [](data& data)
	{
		data.push_byte(0xCB);
		data.push_byte(0b00110000 | data._r);
	};
	d._actions[grules.push("opcode", "SLL '(' HL ')'")] = [](data& data)
	{
		data.push_byte(0xCB);
		data.push_byte(0x36);
	};
	d._actions[grules.push("opcode", "SLL '(' IX plus_minus expr ')'")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
		data.push_byte(0x36);
	};
	d._actions[grules.push("opcode", "SLL '(' IX plus_minus expr ')' ',' r")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
		data.push_byte(0b00110000 | data._r);
	};
	d._actions[grules.push("opcode", "SLL '(' IY plus_minus expr ')'")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
		data.push_byte(0x36);
	};
	d._actions[grules.push("opcode", "SLL '(' IY plus_minus expr ')' ',' r")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
		data.push_byte(0b00110000 | data._r);
	};
	d._actions[grules.push("opcode", "SRA r")] = [](data& data)
	{
		data.push_byte(0xCB);
		data.push_byte(0b00101000 | data._r);
	};
	d._actions[grules.push("opcode", "SRA '(' HL ')'")] = [](data& data)
	{
		data.push_byte(0xCB);
		data.push_byte(0x2E);
	};
	d._actions[grules.push("opcode", "SRA '(' IX plus_minus expr ')'")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
		data.push_byte(0x2E);
	};
	d._actions[grules.push("opcode", "SRA '(' IX plus_minus expr ')' ',' r")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
		data.push_byte(0b00101000 | data._r);
	};
	d._actions[grules.push("opcode", "SRA '(' IY plus_minus expr ')'")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
		data.push_byte(0x2E);
	};
	d._actions[grules.push("opcode", "SRA '(' IY plus_minus expr ')' ',' r")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
		data.push_byte(0b00101000 | data._r);
	};
	d._actions[grules.push("opcode", "SRL r")] = [](data& data)
	{
		data.push_byte(0xCB);
		data.push_byte(0b00111000 | data._r);
	};
	d._actions[grules.push("opcode", "SRL '(' HL ')'")] = [](data& data)
	{
		data.push_byte(0xCB);
		data.push_byte(0x3E);
	};
	d._actions[grules.push("opcode", "SRL '(' IX plus_minus expr ')'")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
		data.push_byte(0x3E);
	};
	d._actions[grules.push("opcode", "SRL '(' IX plus_minus expr ')' ',' r")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
		data.push_byte(0b00111000 | data._r);
	};
	d._actions[grules.push("opcode", "SRL '(' IY plus_minus expr ')'")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
		data.push_byte(0x3E);
	};
	d._actions[grules.push("opcode", "SRL '(' IY plus_minus expr ')' ',' r")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0xCB);
		data.push_byte(0);
		data.bexpr(4, data._plus_minus);
		data.push_byte(0b00111000 | data._r);
	};
	d._actions[grules.push("opcode", "RLD")] = [](data& data)
	{
		data.push_byte(0xED);
		data.push_byte(0x6F);
	};
	d._actions[grules.push("opcode", "RRD")] = [](data& data)
	{
		data.push_byte(0xED);
		data.push_byte(0x67);
	};
	// Bit is 0-7
	d._actions[grules.push("opcode", "BIT integer ',' r")] = [](data& data)
	{
		const uint16_t bit = data._integer;

		if (bit > 7)
		{
			const std::string str(data.dollar(0).first,
				data.dollar(3).second);

			throw std::out_of_range(str + ": Bit out of range");
		}

		// Made local var to prevent VC++ warning
		const auto by = static_cast<uint8_t>(0b01000000 | bit << 3 | data._r);

		data.push_byte(0xCB);
		data.push_byte(by);
	};
	// Bit is 0-7
	d._actions[grules.push("opcode", "BIT integer ',' '(' HL ')'")] = [](data& data)
	{
		const uint16_t bit = data._integer;

		if (bit > 7)
		{
			const std::string str(data.dollar(0).first,
				data.dollar(5).second);

			throw std::out_of_range(str + ": Bit out of range");
		}

		data.push_byte(0xCB);
		data.push_byte(static_cast<uint8_t>(0b01000110 | bit << 3));
	};
	// Bit is 0-7
	d._actions[grules.push("opcode", "BIT integer ',' '(' IX plus_minus expr ')'")] = [](data& data)
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
		data.bexpr(6, data._plus_minus);
		data.push_byte(static_cast<uint8_t>(0b01000110 | bit << 3));
	};
	// Bit is 0-7
	d._actions[grules.push("opcode", "BIT integer ',' '(' IX plus_minus expr ')' ',' r")] = [](data& data)
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
		data.bexpr(6, data._plus_minus);

		const auto by = static_cast<uint8_t>(0b01000000 | bit << 3 | data._r);

		data.push_byte(by);
	};
	// Bit is 0-7
	d._actions[grules.push("opcode", "BIT integer ',' '(' IY plus_minus expr ')'")] = [](data& data)
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
		data.bexpr(6, data._plus_minus);
		data.push_byte(static_cast<uint8_t>(0b01000110 | bit << 3));
	};
	// Bit is 0-7
	d._actions[grules.push("opcode", "BIT integer ',' '(' IY plus_minus expr ')' ',' r")] = [](data& data)
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
		data.bexpr(6, data._plus_minus);

		const auto by = static_cast<uint8_t>(0b01000000 | bit << 3 | data._r);

		data.push_byte(by);
	};
	// Bit is 0-7
	d._actions[grules.push("opcode", "SET integer ',' r")] = [](data& data)
	{
		const uint16_t bit = data._integer;

		if (bit > 7)
		{
			const std::string str(data.dollar(0).first,
				data.dollar(3).second);

			throw std::out_of_range(str + ": Bit out of range");
		}

		// Made local var to prevent VC++ warning
		const auto by = static_cast<uint8_t>(0b11000000 | bit << 3 | data._r);

		data.push_byte(0xCB);
		data.push_byte(by);
	};
	// Bit is 0-7
	d._actions[grules.push("opcode", "SET integer ',' '(' HL ')'")] = [](data& data)
	{
		const int bit = data._integer;

		if (bit > 7)
		{
			const std::string str(data.dollar(0).first,
				data.dollar(5).second);

			throw std::out_of_range(str + ": Bit out of range");
		}

		data.push_byte(0xCB);
		data.push_byte(static_cast<uint8_t>(0b11000110 | bit << 3));
	};
	// Bit is 0-7
	d._actions[grules.push("opcode", "SET integer ',' '(' IX plus_minus expr ')'")] = [](data& data)
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
		data.bexpr(6, data._plus_minus);
		data.push_byte(static_cast<uint8_t>(0b11000110 | bit << 3));
	};
	// Bit is 0-7
	d._actions[grules.push("opcode", "SET integer ',' '(' IX plus_minus expr ')' ',' r")] = [](data& data)
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
		data.bexpr(6, data._plus_minus);

		// Made local var to prevent VC++ warning
		const auto by = static_cast<uint8_t>(0b11000000 | bit << 3 | data._r);

		data.push_byte(by);
	};
	// Bit is 0-7
	d._actions[grules.push("opcode", "SET integer ',' '(' IY plus_minus expr ')'")] = [](data& data)
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
		data.bexpr(6, data._plus_minus);
		data.push_byte(static_cast<uint8_t>(0b11000110 | bit << 3));
	};
	// Bit is 0-7
	d._actions[grules.push("opcode", "SET integer ',' '(' IY plus_minus expr ')' ',' r")] = [](data& data)
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
		data.bexpr(6, data._plus_minus);

		// Made local var to prevent VC++ warning
		const auto by = static_cast<uint8_t>(0b11000000 | bit << 3 | data._r);

		data.push_byte(by);
	};
	// Bit is 0-7
	d._actions[grules.push("opcode", "RES integer ',' r")] = [](data& data)
	{
		const uint16_t bit = data._integer;

		if (bit > 7)
		{
			const std::string str(data.dollar(0).first,
				data.dollar(3).second);

			throw std::out_of_range(str + ": Bit out of range");
		}

		// Made local var to prevent VC++ warning
		const auto by = static_cast<uint8_t>(0b10000000 | bit << 3 | data._r);

		data.push_byte(0xCB);
		data.push_byte(by);
	};
	// Bit is 0-7
	d._actions[grules.push("opcode", "RES integer ',' '(' HL ')'")] = [](data& data)
	{
		const uint16_t bit = data._integer;

		if (bit > 7)
		{
			const std::string str(data.dollar(0).first,
				data.dollar(5).second);

			throw std::out_of_range(str + ": Bit out of range");
		}

		data.push_byte(0xCB);
		data.push_byte(static_cast<uint8_t>(0b10000110 | bit << 3));
	};
	// Bit is 0-7
	d._actions[grules.push("opcode", "RES integer ',' '(' IX plus_minus expr ')'")] = [](data& data)
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
		data.bexpr(6, data._plus_minus);
		data.push_byte(static_cast<uint8_t>(0b10000110 | bit << 3));
	};
	// Bit is 0-7
	d._actions[grules.push("opcode", "RES integer ',' '(' IX plus_minus expr ')' ',' r")] = [](data& data)
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
		data.bexpr(6, data._plus_minus);

		const auto by = static_cast<uint8_t>(0b10000000 | bit << 3 | data._r);

		data.push_byte(by);
	};
	// Bit is 0-7
	d._actions[grules.push("opcode", "RES integer ',' '(' IY plus_minus expr ')'")] = [](data& data)
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
		data.bexpr(6, data._plus_minus);
		data.push_byte(static_cast<uint8_t>(0b10000110 | bit << 3));
	};
	// Bit is 0-7
	d._actions[grules.push("opcode", "RES integer ',' '(' IY plus_minus expr ')' ',' r")] = [](data& data)
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
		data.bexpr(6, data._plus_minus);

		const auto by = static_cast<uint8_t>(0b10000000 | bit << 3 | data._r);

		data.push_byte(by);
	};
	d._actions[grules.push("opcode", "JP expr")] = [](data& data)
	{
		data.push_byte(0xC3);
		data.push_word(0);
		data.wexpr(3, 1);
	};
	d._actions[grules.push("opcode", "JP cc ',' full_expr")] = [](data& data)
	{
		data.push_byte((0b11000010 | data._cc << 3) & 0xff);
		data.push_word(0);
		data.wexpr(3, 3);
	};
	d._actions[grules.push("opcode", "JR full_expr")] = [](data& data)
	{
		data.push_byte(0x18);
		data.push_byte(0);
		data.rel_label(1);
	};
	d._actions[grules.push("opcode", "JR c ',' full_expr")] = [](data& data)
	{
		data.push_byte((0b00100000 | data._c << 3) & 0xff);
		data.push_byte(0);
		data.rel_label(3);
	};
	d._actions[grules.push("opcode", "JP '(' HL ')'")] = [](data& data)
	{
		data.push_byte(0xE9);
	};
	d._actions[grules.push("opcode", "JP '(' IX ')'")] = [](data& data)
	{
		data.push_byte(0xDD);
		data.push_byte(0xE9);
	};
	d._actions[grules.push("opcode", "JP '(' IY ')'")] = [](data& data)
	{
		data.push_byte(0xFD);
		data.push_byte(0xE9);
	};
	d._actions[grules.push("opcode", "DJNZ full_expr")] = [](data& data)
	{
		data.push_byte(0x10);
		data.push_byte(0);
		data.rel_label(1);
	};
	d._actions[grules.push("opcode", "CALL full_expr")] = [](data& data)
	{
		data.push_byte(0xCD);
		data.push_word(0);
		data.wexpr(3, 1);
	};
	d._actions[grules.push("opcode", "CALL cc ',' full_expr")] = [](data& data)
	{
		data.push_byte((0b11000100 | data._cc << 3) & 0xff);
		data.push_word(0);
		data.wexpr(3, 3);
	};
	d._actions[grules.push("opcode", "RET")] = [](data& data)
	{
		data.push_byte(0xC9);
	};
	d._actions[grules.push("opcode", "RET cc")] = [](data& data)
	{
		data.push_byte((0b11000000 | data._cc << 3) & 0xff);
	};
	d._actions[grules.push("opcode", "RETI")] = [](data& data)
	{
		data.push_byte(0xED);
		data.push_byte(0x4D);
	};
	d._actions[grules.push("opcode", "RETN")] = [](data& data)
	{
		data.push_byte(0xED);
		data.push_byte(0x45);
	};
	// Integer is 0-7
	d._actions[grules.push("opcode", "RST integer")] = [](data& data)
	{
		uint8_t by = 0;

		switch (data._integer)
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

		data.push_byte((0b11000111 | by << 3) & 0xff);
	};
	// Only A register is legal
	d._actions[grules.push("opcode", "IN r ',' '(' expr ')'")] = [](data& data)
	{
		if (data._r != 0b111)
		{
			const std::string str(data.dollar(0).first,
				data.dollar(5).second);

			throw z80_error(str + ": Only register A is valid");
		}

		data.push_byte(0xDB);
		data.push_byte(0);
		data.bexpr(4);
	};
	d._actions[grules.push("opcode", "IN F ',' '(' C ')'")] = [](data& data)
	{
		data.push_byte(0xED);
		data.push_byte(0x70);
	};
	d._actions[grules.push("opcode", "IN '(' C ')'")] = [](data& data)
	{
		data.push_byte(0xED);
		data.push_byte(0x70);
	};
	d._actions[grules.push("opcode", "IN r ',' '(' C ')'")] = [](data& data)
	{
		data.push_byte(0xED);
		data.push_byte((0b01000000 | data._r << 3) & 0xff);
	};
	d._actions[grules.push("opcode", "INI")] = [](data& data)
	{
		data.push_byte(0xED);
		data.push_byte(0xA2);
	};
	d._actions[grules.push("opcode", "INIR")] = [](data& data)
	{
		data.push_byte(0xED);
		data.push_byte(0xB2);
	};
	d._actions[grules.push("opcode", "IND")] = [](data& data)
	{
		data.push_byte(0xED);
		data.push_byte(0xAA);
	};
	d._actions[grules.push("opcode", "INDR")] = [](data& data)
	{
		data.push_byte(0xED);
		data.push_byte(0xBA);
	};
	d._actions[grules.push("opcode", "OUT '(' expr ')' ',' A")] = [](data& data)
	{
		data.push_byte(0xD3);
		data.push_byte(0);
		data.bexpr(2);
	};
	d._actions[grules.push("opcode", "OUT '(' C ')' ',' integer")] = [](data& data)
	{
		if (data._integer != 0)
		{
			const std::string str(data.dollar(0).first,
				data.dollar(5).second);

			throw z80_error(str + ": Only 0 is valid");
		}

		data.push_byte(0xED);
		data.push_byte(0x71);
	};
	d._actions[grules.push("opcode", "OUT '(' C ')' ',' r")] = [](data& data)
	{
		data.push_byte(0xED);
		data.push_byte((0b01000001 | data._r << 3) & 0xff);
	};
	d._actions[grules.push("opcode", "OUTI")] = [](data& data)
	{
		data.push_byte(0xED);
		data.push_byte(0xA3);
	};
	d._actions[grules.push("opcode", "OTIR")] = [](data& data)
	{
		data.push_byte(0xED);
		data.push_byte(0xB3);
	};
	d._actions[grules.push("opcode", "OUTD")] = [](data& data)
	{
		data.push_byte(0xED);
		data.push_byte(0xAB);
	};
	d._actions[grules.push("opcode", "OTDR")] = [](data& data)
	{
		data.push_byte(0xED);
		data.push_byte(0xBB);
	};

	d._actions[grules.push("plus_minus", "'+'")] =  [](data& data)
	{
		data._plus_minus = *data.dollar(0).first;
	};
	d._actions[grules.push("plus_minus", "'-'")] = [](data& data)
	{
		data._plus_minus = *data.dollar(0).first;
	};

	d._actions[grules.push("r", "A")] = [](data& data)
	{
		data._r = 0b111;
	};
	d._actions[grules.push("r", "B")] = [](data& data)
	{
		data._r = 0b000;
	};
	d._actions[grules.push("r", "C")] = [](data& data)
	{
		data._r = 0b001;
	};
	d._actions[grules.push("r", "D")] = [](data& data)
	{
		data._r = 0b010;
	};
	d._actions[grules.push("r", "E")] = [](data& data)
	{
		data._r = 0b011;
	};
	d._actions[grules.push("r", "H")] = [](data& data)
	{
		data._r = 0b100;
	};
	d._actions[grules.push("r", "L")] = [](data& data)
	{
		data._r = 0b101;
	};
	d._actions[grules.push("r2", "A")] = [](data& data)
	{
		data._r2 = 0b111;
	};
	d._actions[grules.push("r2", "B")] = [](data& data)
	{
		data._r2 = 0b000;
	};
	d._actions[grules.push("r2", "C")] = [](data& data)
	{
		data._r2 = 0b001;
	};
	d._actions[grules.push("r2", "D")] = [](data& data)
	{
		data._r2 = 0b010;
	};
	d._actions[grules.push("r2", "E")] = [](data& data)
	{
		data._r2 = 0b011;
	};
	d._actions[grules.push("r2", "H")] = [](data& data)
	{
		data._r2 = 0b100;
	};
	d._actions[grules.push("r2", "L")] = [](data& data)
	{
		data._r2 = 0b101;
	};
	d._actions[grules.push("dd", "BC")] = [](data& data)
	{
		data._dd = 0b00;
	};
	d._actions[grules.push("dd", "DE")] = [](data& data)
	{
		data._dd = 0b01;
	};
	d._actions[grules.push("dd", "HL")] = [](data& data)
	{
		data._dd = 0b10;
	};
	d._actions[grules.push("dd", "SP")] = [](data& data)
	{
		data._dd = 0b11;
	};
	d._actions[grules.push("qq", "BC")] = [](data& data)
	{
		data._qq = 0b00;
	};
	d._actions[grules.push("qq", "DE")] = [](data& data)
	{
		data._qq = 0b01;
	};
	d._actions[grules.push("qq", "HL")] = [](data& data)
	{
		data._qq = 0b10;
	};
	d._actions[grules.push("qq", "AF")] = [](data& data)
	{
		data._qq = 0b11;
	};
	d._actions[grules.push("pp", "BC")] = [](data& data)
	{
		data._pp = 0b00;
	};
	d._actions[grules.push("pp", "DE")] = [](data& data)
	{
		data._pp = 0b01;
	};
	d._actions[grules.push("pp", "IX")] = [](data& data)
	{
		data._pp = 0b10;
	};
	d._actions[grules.push("pp", "SP")] = [](data& data)
	{
		data._pp = 0b11;
	};
	d._actions[grules.push("rr", "BC")] = [](data& data)
	{
		data._rr = 0b00;
	};
	d._actions[grules.push("rr", "DE")] = [](data& data)
	{
		data._rr = 0b01;
	};
	d._actions[grules.push("rr", "IY")] = [](data& data)
	{
		data._rr = 0b10;
	};
	d._actions[grules.push("rr", "SP")] = [](data& data)
	{
		data._rr = 0b11;
	};
	d._actions[grules.push("cc", "NZ")] = [](data& data)
	{
		data._cc = 0b000;
	};
	d._actions[grules.push("cc", "Z")] = [](data& data)
	{
		data._cc = 0b001;
	};
	d._actions[grules.push("cc", "NC")] = [](data& data)
	{
		data._cc = 0b010;
	};
	d._actions[grules.push("cc", "C")] = [](data& data)
	{
		data._cc = 0b011;
	};
	d._actions[grules.push("cc", "PO")] = [](data& data)
	{
		data._cc = 0b100;
	};
	d._actions[grules.push("cc", "PE")] = [](data& data)
	{
		data._cc = 0b101;
	};
	d._actions[grules.push("cc", "P")] = [](data& data)
	{
		data._cc = 0b110;
	};
	d._actions[grules.push("cc", "M")] = [](data& data)
	{
		data._cc = 0b111;
	};
	d._actions[grules.push("c", "C")] = [](data& data)
	{
		data._c = 0b11;
	};
	d._actions[grules.push("c", "NC")] = [](data& data)
	{
		data._c = 0b10;
	};
	d._actions[grules.push("c", "Z")] = [](data& data)
	{
		data._c = 0b01;
	};
	d._actions[grules.push("c", "NZ")] = [](data& data)
	{
		data._c = 0b00;
	};

	grules.push("expr", "expr '|' expr "
		"| expr '&' expr "
		"| expr '+' expr "
		"| expr '-' expr "
		"| expr '*' expr "
		"| expr '/' expr "
		"| '-' expr %prec UMINUS "
		"| item");
	grules.push("full_expr", "full_expr '|' full_expr "
		"| full_expr '&' full_expr "
		"| full_expr '+' full_expr "
		"| full_expr '-' full_expr "
		"| full_expr '*' full_expr "
		"| full_expr '/' full_expr "
		"| '(' full_expr ')' "
		"| '-' full_expr %prec UMINUS "
		"| item");
	grules.push("item", "Name "
		"| '$' "
		"| Binary "
		"| Hex "
		"| Char "
		"| Integer");

	d._actions[grules.push("integer", "Binary")] = [](data& data)
	{
		const auto& t = data.dollar(0);
		char* end = nullptr;

		if (*t.first == '%')
			data._integer = static_cast<uint8_t>(strtol(t.first + 1, &end, 2));
		else
			data._integer = static_cast<uint8_t>(strtol(t.first, &end, 2));
	};
	d._actions[grules.push("integer", "Hex")] = [](data& data)
	{
		const auto& t = data.dollar(0);
		char* end = nullptr;

		if (*t.first == '&' || *t.first == '$')
			data._integer = static_cast<uint16_t>(strtol(t.first + 1, &end, 16));
		else
			data._integer = static_cast<uint16_t>(strtol(t.first, &end, 16));
	};
	d._actions[grules.push("integer", "Char")] = [](data& data)
	{
		const auto& t = data.dollar(0);
		const char* first = t.first + 1;

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
			default:
				break;
			}
		}
		else
		{
			data._integer = *first;
		}
	};
	d._actions[grules.push("integer", "Integer")] = [](data& data)
	{
		const auto& t = data.dollar(0);

		data._integer = static_cast<uint16_t>(atoi(t.first));
	};

	//parsertl::debug::dump(grules, std::cout);
	parsertl::generator::build(grules, d._gsm, &warnings);

	if (!warnings.empty())
		throw z80_error(warnings);

	lrules.push("[(]", grules.token_id("'('"));
	lrules.push("[)]", grules.token_id("')'"));
	lrules.push("[|]", grules.token_id("'|'"));
	lrules.push("&", grules.token_id("'&'"));
	lrules.push("[+]", grules.token_id("'+'"));
	lrules.push("-", grules.token_id("'-'"));
	lrules.push("[*]", grules.token_id("'*'"));
	lrules.push("[/]", grules.token_id("'/'"));
	lrules.push(",", grules.token_id("','"));
	lrules.push(":", grules.token_id("':'"));
	lrules.push("[.]?ORG", grules.token_id("ORG"));
	lrules.push("A", grules.token_id("A"));
	lrules.push("ADC", grules.token_id("ADC"));
	lrules.push("ADD", grules.token_id("ADD"));
	lrules.push("AF", grules.token_id("AF"));
	lrules.push("AF'", grules.token_id("AF_PRIME"));
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
	lrules.push("DB|DEFB|DEFM", grules.token_id("DB"));
	lrules.push("DS|DEFS", grules.token_id("DS"));
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
	lrules.push("F", grules.token_id("F"));
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
	lrules.push("IXH", grules.token_id("IXh"));
	lrules.push("IXL", grules.token_id("IXl"));
	lrules.push("IY", grules.token_id("IY"));
	lrules.push("IYH", grules.token_id("IYh"));
	lrules.push("IYL", grules.token_id("IYl"));
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
	lrules.push("SLL", grules.token_id("SLL"));
	lrules.push("SP", grules.token_id("SP"));
	lrules.push("SRA", grules.token_id("SRA"));
	lrules.push("SRL", grules.token_id("SRL"));
	lrules.push("SUB", grules.token_id("SUB"));
	lrules.push("XOR", grules.token_id("XOR"));
	lrules.push("Z", grules.token_id("Z"));
	lrules.push(R"(\$)", grules.token_id("'$'"));
	lrules.push("%[01]{8}|[01]{8}b", grules.token_id("Binary"));
	lrules.push("[&$][0-9A-Fa-f]+|[0-9A-Fa-f]+[hH]", grules.token_id("Hex"));
	lrules.push(R"('(\\([abefnrtvx\\'"?]|\d{3}|x[\da-f]{2})|[^\\'])')", grules.token_id("Char"));
	lrules.push(R"(\d+)", grules.token_id("Integer"));
	lrules.push(R"('[^'\r\n]{2,}'|\"([^"\r\n]|\\.)*\")", grules.token_id("String"));
	lrules.push("[A-Z_a-z][0-9A-Z_a-z]*", grules.token_id("Name"));
	lrules.push("[ \t]+|;.*|[/][*](?s:.)*?[*][/]", lexertl::rules::skip());
	lrules.push("\r?\n", grules.token_id("NL"));
	lexertl::generator::build(lrules, d._lsm);
}

static void build_expr_parser(data& d)
{
	parsertl::rules grules;
	lexertl::rules lrules(*lexertl::regex_flags::dot_not_cr_lf | *lexertl::regex_flags::icase);
	std::string warnings;

	grules.token("'$' Binary Char Hex Integer Name");
	grules.left("'|' '&'");
	grules.left("'+' '-'");
	grules.left("'*' '/'");
	grules.precedence("UMINUS");

	d._expr_actions[grules.push("expr", "expr '|' expr")] = [](data& data)
	{
		auto rhs = data._acc.top();

		data._acc.pop();
		data._acc.top() |= rhs;
	};
	d._expr_actions[grules.push("expr", "expr '&' expr")] = [](data& data)
	{
		auto rhs = data._acc.top();

		data._acc.pop();
		data._acc.top() &= rhs;
	};
	d._expr_actions[grules.push("expr", "expr '+' expr")] = [](data& data)
	{
		auto rhs = data._acc.top();

		data._acc.pop();
		data._acc.top() += rhs;
	};
	d._expr_actions[grules.push("expr", "expr '-' expr")] = [](data& data)
	{
		auto rhs = data._acc.top();

		data._acc.pop();
		data._acc.top() -= rhs;
	};
	d._expr_actions[grules.push("expr", "expr '*' expr")] = [](data& data)
	{
		auto rhs = data._acc.top();

		data._acc.pop();
		data._acc.top() *= rhs;
	};
	d._expr_actions[grules.push("expr", "expr '/' expr")] = [](data& data)
	{
		auto rhs = data._acc.top();

		data._acc.pop();
		data._acc.top() /= rhs;
	};
	grules.push("expr", "'(' expr ')'");
	d._expr_actions[grules.push("expr", "'-' expr %prec UMINUS")] = [](data& data)
	{
		data._acc.top() *= -1;
	};
	d._expr_actions[grules.push("expr", "Name")] = [](data& data)
	{
		const auto str = data._results.dollar(0, data._expr_gsm, data._productions).str();

		if (auto lab = data._label.find(str);
			lab != data._label.end())
		{
			data._acc.push(data._program._org + lab->second);
			return;
		}

		auto equ = data._equ.find(str);

		if (equ == data._equ.end())
			throw z80_error("Unknown label " + str);

		data._acc.push(equ->second);
	};
	grules.push("expr", "integer");
	d._expr_actions[grules.push("integer", "Binary")] = [](data& data)
	{
		const auto& t = data._results.dollar(0, data._expr_gsm, data._productions);
		char* end = nullptr;

		if (*t.first == '%')
			data._integer = static_cast<uint8_t>(strtol(t.first + 1, &end, 2));
		else
			data._integer = static_cast<uint8_t>(strtol(t.first, &end, 2));

		data._acc.push(data._integer);
	};
	d._expr_actions[grules.push("integer", "Hex")] = [](data& data)
	{
		const auto& t = data._results.dollar(0, data._expr_gsm, data._productions);
		char* end = nullptr;

		if (*t.first == '&' || *t.first == '$')
			data._integer = static_cast<uint16_t>(strtol(t.first + 1, &end, 16));
		else
			data._integer = static_cast<uint16_t>(strtol(t.first, &end, 16));

		data._acc.push(data._integer);
	};
	d._expr_actions[grules.push("integer", "Char")] = [](data& data)
	{
		const auto& t = data._results.dollar(0, data._expr_gsm, data._productions);

		if (const char* first = t.first + 1;
			*first == '\\')
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
			default:
				break;
			}
		}
		else
		{
			data._integer = *first;
		}

		data._acc.push(data._integer);
	};
	d._expr_actions[grules.push("integer", "Integer")] = [](data& data)
	{
		const auto& t = data._results.dollar(0, data._expr_gsm, data._productions);

		data._integer = static_cast<uint16_t>(atoi(t.first));
		data._acc.push(data._integer);
	};
	parsertl::generator::build(grules, d._expr_gsm, &warnings);

	if (!warnings.empty())
		throw z80_error(warnings);

	d._pc_idx = grules.token_id("'$'");
	lrules.push(R"(\$)", d._pc_idx);
	lrules.push("[|]", grules.token_id("'|'"));
	lrules.push("&", grules.token_id("'&'"));
	lrules.push("[+]", grules.token_id("'+'"));
	lrules.push("-", grules.token_id("'-'"));
	lrules.push("[*]", grules.token_id("'*'"));
	lrules.push("[/]", grules.token_id("'/'"));
	lrules.push("[(]", grules.token_id("'('"));
	lrules.push("[)]", grules.token_id("')'"));
	lrules.push("%[01]{8}|[01]{8}b", grules.token_id("Binary"));
	lrules.push(R"('(\\([abefnrtvx\\'"?]|\d{3}|x[\da-f]{2})|[^\\'])')", grules.token_id("Char"));
	lrules.push("[&$][0-9A-Fa-f]+|[0-9A-Fa-f]+h", grules.token_id("Hex"));
	lrules.push(R"(\d+)", grules.token_id("Integer"));
	lrules.push("[_A-Z][0-9_A-Z]*", grules.token_id("Name"));
	lrules.push("[ \t]+|;.*|[/][*](?s:.)*?[*][/]", lexertl::rules::skip());
	lexertl::generator::build(lrules, d._expr_lsm);
}

void build_parsers(data& data, const std::size_t flags)
{
	build_parser(data, flags);
	build_expr_parser(data);
}
