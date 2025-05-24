#include "dump.hpp"

void dump_blocks(const program& program)
{
	std::cout << program._org << '\n';

	for (const auto& block : program._mem_type)
	{
		using type = program::block::type;

		switch (block._type)
		{
			case type::code:
				std::cout << "code";
				break;
			case type::db:
				std::cout << "db";
				break;
			case type::ds:
				std::cout << "ds";
				break;
			case type::dw:
				std::cout << "dw";
				break;
		}

		std::cout << ' ' << block._count << '\n';
	}
}
