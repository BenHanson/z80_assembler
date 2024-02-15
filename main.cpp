#include "disassem.hpp"
#include <format>
#include <iostream>
#include <lexertl/memory_file.hpp>
#include "parsers.hpp"
#include "z80_error.hpp"

static void save(program& program, const char* src, const char* dest)
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
			throw z80_error(std::format("Unable to open {}", dest));
		}
	}
	else
	{
		throw z80_error(std::format("Unable to open {}", src));
	}
}

static const char* usage()
{
	return "USAGE: z80_assembler <pathname> "
		"[<source .sna> <dest .sna>] [-(dec|hex)] [-(jr_off|jr_addr)]\n";
}

int main(int argc, const char* argv[])
{
	if (argc < 2)
	{
		std::cout << usage();
		return 1;
	}

	try
	{
		lexertl::memory_file mf(argv[1]);
		data data;
		std::vector<const char*> pathnames;
		base base = base::decimal;
		relative relative = relative::absolute;

		for (int i = 2; i < argc; ++i)
		{
			const char* arg = argv[i];

			if (::strcmp(arg, "-dec") == 0)
				base = base::decimal;
			else if (::strcmp(arg, "-hex") == 0)
				base = base::hexadecimal;
			else if (::strcmp(arg, "-jr_off") == 0)
				relative = relative::offset;
			else if (::strcmp(arg, "-jr_addr") == 0)
				relative = relative::absolute;
			else
				pathnames.push_back(arg);
		}

		if (!mf.data())
			throw z80_error(std::format("Unable to open {}", argv[1]));

		if (!(pathnames.empty() || pathnames.size() == 2))
			throw z80_error(usage());

		build_parsers(data);
		data.parse(mf.data(), mf.data() + mf.size(), relative);
		mf.close();

		if (data._program._org + data._program._memory.size() - 1 > 65535)
			throw z80_error("Code exceeds memory limit (65535)");

		if (pathnames.size() == 2)
			save(data._program, argv[2], argv[3]);
		else
			dump(data._program, base, relative);

		return 0;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << '\n';
		return 1;
	}
}
