#include "data.h"
#include "disassem.h"
#include <iostream>
#include <iterator>
#include "../lexertl14/include/lexertl/memory_file.hpp"
#include <sstream>

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
#ifdef WIN32
		fopen_s(&fp, dest, "wb");
#else
		fp = fopen(dest, "wb");
#endif

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
		data.build_expr_parser();
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
