#include "disassem.h"
#include <iomanip>
#include <sstream>

[[nodiscard]] std::string dump_IX_IY_bits(const uint8_t*& curr, const char xy, const base base)
{
	std::ostringstream ret;
	const uint16_t data = static_cast<uint16_t>(*curr);

	++curr;

	if (base == base::hex)
		ret << std::hex;

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
		ret << "BIT 0, (I" << xy << " + " << data << "), B";
		break;
	case 0x41:
		ret << "BIT 0, (I" << xy << " + " << data << "), C";
		break;
	case 0x42:
		ret << "BIT 0, (I" << xy << " + " << data << "), D";
		break;
	case 0x43:
		ret << "BIT 0, (I" << xy << " + " << data << "), E";
		break;
	case 0x44:
		ret << "BIT 0, (I" << xy << " + " << data << "), H";
		break;
	case 0x45:
		ret << "BIT 0, (I" << xy << " + " << data << "), L";
		break;
	case 0x47:
		ret << "BIT 0, (I" << xy << " + " << data << "), A";
		break;
	case 0x46:
		ret << "BIT 0, (I" << xy << " + " << data << ")";
		break;
	case 0x48:
		ret << "BIT 1, (I" << xy << " + " << data << "), B";
		break;
	case 0x49:
		ret << "BIT 1, (I" << xy << " + " << data << "), C";
		break;
	case 0x4a:
		ret << "BIT 1, (I" << xy << " + " << data << "), D";
		break;
	case 0x4b:
		ret << "BIT 1, (I" << xy << " + " << data << "), E";
		break;
	case 0x4c:
		ret << "BIT 1, (I" << xy << " + " << data << "), H";
		break;
	case 0x4d:
		ret << "BIT 1, (I" << xy << " + " << data << "), L";
		break;
	case 0x4e:
		ret << "BIT 1, (I" << xy << " + " << data << ")";
		break;
	case 0x4f:
		ret << "BIT 1, (I" << xy << " + " << data << "), A";
		break;
	case 0x50:
		ret << "BIT 2, (I" << xy << " + " << data << "), B";
		break;
	case 0x51:
		ret << "BIT 2, (I" << xy << " + " << data << "), C";
		break;
	case 0x52:
		ret << "BIT 2, (I" << xy << " + " << data << "), D";
		break;
	case 0x53:
		ret << "BIT 2, (I" << xy << " + " << data << "), E";
		break;
	case 0x54:
		ret << "BIT 2, (I" << xy << " + " << data << "), H";
		break;
	case 0x55:
		ret << "BIT 2, (I" << xy << " + " << data << "), L";
		break;
	case 0x56:
		ret << "BIT 2, (I" << xy << " + " << data << ")";
		break;
	case 0x57:
		ret << "BIT 2, (I" << xy << " + " << data << "), A";
		break;
	case 0x58:
		ret << "BIT 3, (I" << xy << " + " << data << "), B";
		break;
	case 0x59:
		ret << "BIT 3, (I" << xy << " + " << data << "), C";
		break;
	case 0x5a:
		ret << "BIT 3, (I" << xy << " + " << data << "), D";
		break;
	case 0x5b:
		ret << "BIT 3, (I" << xy << " + " << data << "), E";
		break;
	case 0x5c:
		ret << "BIT 3, (I" << xy << " + " << data << "), H";
		break;
	case 0x5d:
		ret << "BIT 3, (I" << xy << " + " << data << "), L";
		break;
	case 0x5e:
		ret << "BIT 3, (I" << xy << " + " << data << ")";
		break;
	case 0x5f:
		ret << "BIT 3, (I" << xy << " + " << data << "), A";
		break;
	case 0x60:
		ret << "BIT 4, (I" << xy << " + " << data << "), B";
		break;
	case 0x61:
		ret << "BIT 4, (I" << xy << " + " << data << "), C";
		break;
	case 0x62:
		ret << "BIT 4, (I" << xy << " + " << data << "), D";
		break;
	case 0x63:
		ret << "BIT 4, (I" << xy << " + " << data << "), E";
		break;
	case 0x64:
		ret << "BIT 4, (I" << xy << " + " << data << "), H";
		break;
	case 0x65:
		ret << "BIT 4, (I" << xy << " + " << data << "), L";
		break;
	case 0x66:
		ret << "BIT 4, (I" << xy << " + " << data << ")";
		break;
	case 0x67:
		ret << "BIT 4, (I" << xy << " + " << data << "), A";
		break;
	case 0x68:
		ret << "BIT 5, (I" << xy << " + " << data << "), B";
		break;
	case 0x69:
		ret << "BIT 5, (I" << xy << " + " << data << "), C";
		break;
	case 0x6a:
		ret << "BIT 5, (I" << xy << " + " << data << "), D";
		break;
	case 0x6b:
		ret << "BIT 5, (I" << xy << " + " << data << "), E";
		break;
	case 0x6c:
		ret << "BIT 5, (I" << xy << " + " << data << "), H";
		break;
	case 0x6d:
		ret << "BIT 5, (I" << xy << " + " << data << "), L";
		break;
	case 0x6e:
		ret << "BIT 5, (I" << xy << " + " << data << ")";
		break;
	case 0x6f:
		ret << "BIT 5, (I" << xy << " + " << data << "), A";
		break;
	case 0x70:
		ret << "BIT 6, (I" << xy << " + " << data << "), B";
		break;
	case 0x71:
		ret << "BIT 6, (I" << xy << " + " << data << "), C";
		break;
	case 0x72:
		ret << "BIT 6, (I" << xy << " + " << data << "), D";
		break;
	case 0x73:
		ret << "BIT 6, (I" << xy << " + " << data << "), E";
		break;
	case 0x74:
		ret << "BIT 6, (I" << xy << " + " << data << "), H";
		break;
	case 0x75:
		ret << "BIT 6, (I" << xy << " + " << data << "), L";
		break;
	case 0x76:
		ret << "BIT 6, (I" << xy << " + " << data << ")";
		break;
	case 0x77:
		ret << "BIT 6, (I" << xy << " + " << data << "), A";
		break;
	case 0x78:
		ret << "BIT 7, (I" << xy << " + " << data << "), B";
		break;
	case 0x79:
		ret << "BIT 7, (I" << xy << " + " << data << "), C";
		break;
	case 0x7a:
		ret << "BIT 7, (I" << xy << " + " << data << "), D";
		break;
	case 0x7b:
		ret << "BIT 7, (I" << xy << " + " << data << "), E";
		break;
	case 0x7c:
		ret << "BIT 7, (I" << xy << " + " << data << "), H";
		break;
	case 0x7d:
		ret << "BIT 7, (I" << xy << " + " << data << "), L";
		break;
	case 0x7e:
		ret << "BIT 7, (I" << xy << " + " << data << ")";
		break;
	case 0x7f:
		ret << "BIT 7, (I" << xy << " + " << data << "), A";
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
	return ret.str();
}

[[nodiscard]] std::string dump_IX_IY(const uint8_t*& curr, const char xy, const base base)
{
	std::ostringstream ret;

	if (base == base::hex)
		ret << std::hex;

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
		ret << dump_IX_IY_bits(++curr, xy, base);
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

[[nodiscard]] std::string dump_bits(const uint8_t*& curr)
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

[[nodiscard]] std::string dump_ext(const uint8_t*& curr, const base base)
{
	std::ostringstream ret;

	if (base == base::hex)
		ret << std::hex;

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

void fetch_opcode(const uint8_t*& curr, std::string& line, const base base)
{
	std::ostringstream ss;

	if (base == base::hex)
		ss << std::hex;

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
		ss << dump_IX_IY(++curr, 'X', base);
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
		ss << dump_ext(++curr, base);
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
		ss << dump_IX_IY(++curr, 'Y', base);
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

void dump(const data& data, const base base)
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

		fetch_opcode(first, line, base);
		offset = first - curr;

		for (; curr < first; ++curr)
		{
			if (base == base::hex)
			{
				ss << std::hex << std::setfill('0');
				ss.width(2);
			}
			else
				ss.width(3);

			ss << static_cast<int>(*curr) << ' ';
		}

		bytes = ss.str();
		bytes.resize(static_cast<std::size_t>(5) *
			static_cast<std::size_t>(base == base::hex ? 3 : 4), ' ');

		if (base == base::hex)
		{
			std::cout << std::hex << std::setfill('0');
			std::cout.width(4);
		}
		else
			std::cout.width(5);

		std::cout << addr << "   ";
		std::cout << bytes << line;
		addr += offset;
	}
}
