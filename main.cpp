#include "disassem.h"
#include <iostream>
#include "../lexertl14/include/lexertl/memory_file.hpp"
#include "parsers.h"

void save(program& program, const char* src, const char* dest)
{
	lexertl::memory_file mf(src);

	if (mf.data())
	{
		std::vector<uint8_t> sna(mf.data(), mf.data() + mf.size());
		uint8_t* ptr = &sna.front() + 27 + (program._org - 16384);
		FILE* fp = nullptr;

		mf.close();
		memcpy(ptr, &program._memory.front(), program._memory.size());
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

		build_parsers(data);
		data.parse(mf.data(), mf.data() + mf.size());
		mf.close();

		if (data._program._org + data._program._memory.size() - 1 > 65535)
			throw std::runtime_error("Code exceeds memory limit (65535)");

		if (argc == 4)
			save(data._program, argv[2], argv[3]);
		else
			dump(data._program, base::decimal, relative::expand);

		return 0;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << '\n';
		return 1;
	}
}
