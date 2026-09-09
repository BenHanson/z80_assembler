# Z80 Assembler

A complete Z80 assembler and disassembler.

## Building

A C++20 compatible compiler is required.

```shell
git clone https://github.com/BenHanson/lexertl17
git clone https://github.com/BenHanson/parsertl17
git clone https://github.com/BenHanson/z80_assembler
```

* Use the `Makefile` when building on Linux
* Use the `.sln` file when building with Visual Studio

## Usage

`z80_assembler <pathname> [<source .sna> <dest .sna>] [--blocks] [--(dec|hex)] [--(jr_offset|jr_address)]`

## Switches

| Switch       | Description |
| ------------ | ----------- |
| --blocks     | Output a list of blocks showing a byte count for code and data |
| --dec        | Output numbers in decimal |
| --hex        | Output numbers in hex |
| --jr_offset  | Treat realtive jump parameters as offsets |
| --jr_address | Treat relative jump parameters as addresses (default) |
