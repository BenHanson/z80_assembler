#include "disassem.h"
#include <iomanip>
#include <sstream>

[[nodiscard]] std::string dump_IX_IY_bits(const uint8_t*& curr, const char xy, const base base)
{
	std::ostringstream ret;
	const uint16_t data = static_cast<uint16_t>(*curr);

	++curr;

	if (base == base::hex)
	{
		ret << std::uppercase << std::hex << std::setfill('0') << std::setw(2);
	}

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
	{
		ret << std::uppercase << std::hex;
	}

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

		if (base == base::hex)
		{
			ret << std::setw(4) << std::setfill('0');
		}

		ret << *reinterpret_cast<const uint16_t*>(++curr);
		curr += 2;
		break;
	case 0x22:
		ret << "LD (";

		if (base == base::hex)
		{
			ret << std::setw(4) << std::setfill('0');
		}

		ret << *reinterpret_cast<const uint16_t*>(++curr);
		ret << "), I" << xy;
		curr += 2;
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

		if (base == base::hex)
			ret << std::setw(2) << std::setfill('0');

		ret << static_cast<uint16_t>(*++curr);
		++curr;
		break;
	case 0x29:
		ret << "ADD I" << xy << ", I" << xy;
		++curr;
		break;
	case 0x2a:
		ret << "LD I" << xy << ", (";

		if (base == base::hex)
			ret << std::setw(4) << std::setfill('0');

		ret << *reinterpret_cast<const uint16_t*>(++curr);
		ret << ')';
		curr += 2;
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

		if (base == base::hex)
			ret << std::setw(2) << std::setfill('0');

		ret << static_cast<uint16_t>(*++curr);
		++curr;
		break;
	case 0x34:
		ret << "INC (I" << xy << " + ";

		if (base == base::hex)
			ret << std::setw(2) << std::setfill('0');

		ret << static_cast<uint16_t>(*++curr);
		ret << ')';
		++curr;
		break;
	case 0x35:
		ret << "DEC (I" << xy << " + ";

		if (base == base::hex)
			ret << std::setw(2) << std::setfill('0');

		ret << static_cast<uint16_t>(*++curr);
		ret << ')';
		++curr;
		break;
	case 0x36:
		ret << "LD (I" << xy << " + ";

		if (base == base::hex)
			ret << std::setw(2) << std::setfill('0');

		ret << static_cast<uint16_t>(*++curr);
		ret << "), ";

		if (base == base::hex)
			ret << std::setw(2) << std::setfill('0');

		ret << static_cast<uint16_t>(*++curr);
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

		if (base == base::hex)
			ret << std::setw(2) << std::setfill('0');

		ret << static_cast<uint16_t>(*++curr);
		ret << ')';
		++curr;
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

		if (base == base::hex)
			ret << std::setw(2) << std::setfill('0');

		ret << static_cast<uint16_t>(*++curr);
		ret << ')';
		++curr;
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

		if (base == base::hex)
			ret << std::setw(2) << std::setfill('0');

		ret << static_cast<uint16_t>(*++curr);
		ret << ')';
		++curr;
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

		if (base == base::hex)
			ret << std::setw(2) << std::setfill('0');

		ret << static_cast<uint16_t>(*++curr);
		ret << ')';
		++curr;
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

		if (base == base::hex)
			ret << std::setw(2) << std::setfill('0');

		ret << static_cast<uint16_t>(*++curr);
		ret << ')';
		++curr;
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

		if (base == base::hex)
			ret << std::setw(2) << std::setfill('0');

		ret << static_cast<uint16_t>(*++curr);
		ret << ')';
		++curr;
		break;
	case 0x6f:
		ret << "LD I" << xy << "L, A";
		++curr;
		break;
	case 0x70:
		ret << "LD (I" << xy << " + ";

		if (base == base::hex)
			ret << std::setw(2) << std::setfill('0');

		ret << static_cast<uint16_t>(*++curr);
		ret << "), B";
		++curr;
		break;
	case 0x71:
		ret << "LD (I" << xy << " + ";

		if (base == base::hex)
			ret << std::setw(2) << std::setfill('0');

		ret << static_cast<uint16_t>(*++curr);
		ret << "), C";
		++curr;
		break;
	case 0x72:
		ret << "LD (I" << xy << " + ";

		if (base == base::hex)
			ret << std::setw(2) << std::setfill('0');

		ret << static_cast<uint16_t>(*++curr);
		ret << "), D";
		++curr;
		break;
	case 0x73:
		ret << "LD (I" << xy << " + ";

		if (base == base::hex)
			ret << std::setw(2) << std::setfill('0');

		ret << static_cast<uint16_t>(*++curr);
		ret << "), E";
		++curr;
		break;
	case 0x74:
		ret << "LD (I" << xy << " + ";

		if (base == base::hex)
			ret << std::setw(2) << std::setfill('0');

		ret << static_cast<uint16_t>(*++curr);
		ret << "), H";
		++curr;
		break;
	case 0x75:
		ret << "LD (I" << xy << " + ";

		if (base == base::hex)
			ret << std::setw(2) << std::setfill('0');

		ret << static_cast<uint16_t>(*++curr);
		ret << "), L";
		++curr;
		break;
	case 0x77:
		ret << "LD (I" << xy << " + ";

		if (base == base::hex)
			ret << std::setw(2) << std::setfill('0');

		ret << static_cast<uint16_t>(*++curr);
		ret << "), A";
		++curr;
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

		if (base == base::hex)
			ret << std::setw(2) << std::setfill('0');

		ret << static_cast<uint16_t>(*++curr);
		ret << ')';
		++curr;
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
		ret << "ADD A, (I" << xy << " + ";

		if (base == base::hex)
			ret << std::setw(2) << std::setfill('0');

		ret << static_cast<uint16_t>(*++curr) << ')';
		++curr;
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
		ret << "ADC A, (I" << xy << " + ";

		if (base == base::hex)
			ret << std::setw(2) << std::setfill('0');

		ret << static_cast<uint16_t>(*++curr) << ')';
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
		ret << "SUB (I" << xy << " + ";

		if (base == base::hex)
			ret << std::setw(2) << std::setfill('0');

		ret << static_cast<uint16_t>(*++curr) << ')';
		++curr;
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
		ret << "SBC A, (I" << xy << " + ";

		if (base == base::hex)
			ret << std::setw(2) << std::setfill('0');

		ret << static_cast<uint16_t>(*++curr) << ')';
		++curr;
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
		ret << "AND (I" << xy << " + ";

		if (base == base::hex)
			ret << std::setw(2) << std::setfill('0');

		ret << static_cast<uint16_t>(*++curr) << ')';
		++curr;
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
		ret << "XOR (I" << xy << " + ";

		if (base == base::hex)
			ret << std::setw(2) << std::setfill('0');

		ret << static_cast<uint16_t>(*++curr) << ')';
		++curr;
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
		ret << "OR (I" << xy << " + ";

		if (base == base::hex)
			ret << std::setw(2) << std::setfill('0');

		ret << static_cast<uint16_t>(*++curr) << ')';
		++curr;
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
		ret << "CP (I" << xy << " + ";

		if (base == base::hex)
			ret << std::setw(2) << std::setfill('0');

		ret << static_cast<uint16_t>(*++curr) << ')';
		++curr;
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
		ret << "db ";

		if (base == base::hex)
			ret << std::setw(2) << std::setfill('0');

		ret << static_cast<uint16_t>(*curr);
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
		ret << std::uppercase << std::hex;

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

		if (base == base::hex)
			ret << std::setw(4) << std::setfill('0');

		ret << *reinterpret_cast<const uint16_t*>(++curr);
		ret << "), BC";
		curr += 2;
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

		if (base == base::hex)
			ret << std::setw(4) << std::setfill('0');

		ret << *reinterpret_cast<const uint16_t*>(++curr);
		ret << ')';
		curr += 2;
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

		if (base == base::hex)
			ret << std::setw(4) << std::setfill('0');

		ret << *reinterpret_cast<const uint16_t*>(++curr);
		ret << "), DE";
		curr += 2;
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

		if (base == base::hex)
			ret << std::setw(4) << std::setfill('0');

		ret << *reinterpret_cast<const uint16_t*>(++curr);
		ret << ')';
		curr += 2;
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

		if (base == base::hex)
			ret << std::setw(4) << std::setfill('0');

		ret << *reinterpret_cast<const uint16_t*>(++curr);
		ret << "), HL";
		curr += 2;
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

		if (base == base::hex)
			ret << std::setw(4) << std::setfill('0');

		ret << *reinterpret_cast<const uint16_t*>(++curr);
		ret << ')';
		curr += 2;
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
		ret << "IN F, (C)";
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

		if (base == base::hex)
			ret << std::setw(4) << std::setfill('0');

		ret << *reinterpret_cast<const uint16_t*>(++curr);
		ret << "), SP";
		curr += 2;
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

		if (base == base::hex)
			ret << std::setw(4) << std::setfill('0');

		ret << *reinterpret_cast<const uint16_t*>(++curr);
		ret << ')';
		curr += 2;
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
		ret << "db ";

		if (base == base::hex)
			ret << std::setw(2) << std::setfill('0');

		ret << 237 << ", ";

		if (base == base::hex)
			ret << std::setw(2) << std::setfill('0');

		ret << static_cast<uint16_t>(*curr);
		break;
	}

	return ret.str();
}

std::string fetch_opcode(const uint8_t*& curr, const base base)
{
	std::ostringstream ret;

	if (base == base::hex)
		ret << std::uppercase << std::hex;

	switch (*curr)
	{
	case 0x00:
		ret << "NOP";
		++curr;
		break;
	case 0x01:
		ret << "LD BC, ";

		if (base == base::hex)
			ret << std::setw(4) << std::setfill('0');

		ret << *reinterpret_cast<const uint16_t*>(++curr);
		curr += 2;
		break;
	case 0x02:
		ret << "LD (BC), A";
		++curr;
		break;
	case 0x03:
		ret << "INC BC";
		++curr;
		break;
	case 0x04:
		ret << "INC B";
		++curr;
		break;
	case 0x05:
		ret << "DEC B";
		++curr;
		break;
	case 0x06:
		ret << "LD B, ";

		if (base == base::hex)
			ret << std::setw(2) << std::setfill('0');

		ret << static_cast<uint16_t>(*++curr);
		++curr;
		break;
	case 0x07:
		ret << "RLCA";
		++curr;
		break;
	case 0x08:
		ret << "EX AF, AF'";
		++curr;
		break;
	case 0x09:
		ret << "ADD HL, BC";
		++curr;
		break;
	case 0x0a:
		ret << "LD A, (BC)";
		++curr;
		break;
	case 0x0b:
		ret << "DEC BC";
		++curr;
		break;
	case 0x0c:
		ret << "INC C";
		++curr;
		break;
	case 0x0d:
		ret << "DEC C";
		++curr;
		break;
	case 0x0e:
		ret << "LD C, ";

		if (base == base::hex)
			ret << std::setw(2) << std::setfill('0');

		ret << static_cast<uint16_t>(*++curr);
		++curr;
		break;
	case 0x0f:
		ret << "RRCA";
		++curr;
		break;
	case 0x10:
		ret << "DJNZ ";
		++curr;

		if (base == base::hex)
		{
			ret << std::setw(2) << std::setfill('0');
			ret << static_cast<uint16_t>(*curr);
		}
		else
			ret << static_cast<int16_t>(static_cast<char>(*curr));

		++curr;
		break;
	case 0x11:
		ret << "LD DE, ";

		if (base == base::hex)
			ret << std::setw(4) << std::setfill('0');

		ret << *reinterpret_cast<const uint16_t*>(++curr);
		curr += 2;
		break;
	case 0x12:
		ret << "LD (DE), A";
		++curr;
		break;
	case 0x13:
		ret << "INC DE";
		++curr;
		break;
	case 0x14:
		ret << "INC D";
		++curr;
		break;
	case 0x15:
		ret << "DEC D";
		++curr;
		break;
	case 0x16:
		ret << "LD D, ";

		if (base == base::hex)
			ret << std::setw(2) << std::setfill('0');

		ret << static_cast<uint16_t>(*++curr);
		++curr;
		break;
	case 0x17:
		ret << "RLA";
		++curr;
		break;
	case 0x18:
		ret << "JR ";
		++curr;

		if (base == base::hex)
		{
			ret << std::setw(2) << std::setfill('0');
			ret << static_cast<uint16_t>(*curr);
		}
		else
			ret << static_cast<int16_t>(static_cast<char>(*curr));

		++curr;
		break;
	case 0x19:
		ret << "ADD HL, DE";
		++curr;
		break;
	case 0x1a:
		ret << "LD A, (DE)";
		++curr;
		break;
	case 0x1b:
		ret << "DEC DE";
		++curr;
		break;
	case 0x1c:
		ret << "INC E";
		++curr;
		break;
	case 0x1d:
		ret << "DEC E";
		++curr;
		break;
	case 0x1e:
		ret << "LD E, ";

		if (base == base::hex)
			ret << std::setw(2) << std::setfill('0');

		ret << static_cast<uint16_t>(*++curr);
		++curr;
		break;
	case 0x1f:
		ret << "RRA";
		++curr;
		break;
	case 0x20:
		ret << "JR NZ, ";
		++curr;

		if (base == base::hex)
		{
			ret << std::setw(2) << std::setfill('0');
			ret << static_cast<uint16_t>(*curr);
		}
		else
			ret << static_cast<int16_t>(static_cast<char>(*curr));

		++curr;
		break;
	case 0x21:
		ret << "LD HL, ";

		if (base == base::hex)
			ret << std::setw(4) << std::setfill('0');

		ret << *reinterpret_cast<const uint16_t*>(++curr);
		curr += 2;
		break;
	case 0x22:
		ret << "LD (";

		if (base == base::hex)
			ret << std::setw(4) << std::setfill('0');

		ret << *reinterpret_cast<const uint16_t*>(++curr);
		curr += 2;
		ret << "), HL";
		break;
	case 0x23:
		ret << "INC HL";
		++curr;
		break;
	case 0x24:
		ret << "INC H";
		++curr;
		break;
	case 0x25:
		ret << "DEC H";
		++curr;
		break;
	case 0x26:
		ret << "LD H, ";

		if (base == base::hex)
			ret << std::setw(2) << std::setfill('0');

		ret << static_cast<uint16_t>(*++curr);
		++curr;
		break;
	case 0x27:
		ret << "DAA";
		++curr;
		break;
	case 0x28:
		ret << "JR Z, ";
		++curr;

		if (base == base::hex)
		{
			ret << std::setw(2) << std::setfill('0');
			ret << static_cast<uint16_t>(*curr);
		}
		else
			ret << static_cast<int16_t>(static_cast<char>(*curr));

		++curr;
		break;
	case 0x29:
		ret << "ADD HL, HL";
		++curr;
		break;
	case 0x2a:
		ret << "LD HL, (";

		if (base == base::hex)
			ret << std::setw(4) << std::setfill('0');

		ret << *reinterpret_cast<const uint16_t*>(++curr);
		ret << ')';
		curr += 2;
		break;
	case 0x2b:
		ret << "DEC HL";
		++curr;
		break;
	case 0x2c:
		ret << "INC L";
		++curr;
		break;
	case 0x2d:
		ret << "DEC L";
		++curr;
		break;
	case 0x2e:
		ret << "LD L, ";

		if (base == base::hex)
			ret << std::setw(2) << std::setfill('0');

		ret << static_cast<uint16_t>(*++curr);
		++curr;
		break;
	case 0x2f:
		ret << "CPL";
		++curr;
		break;
	case 0x30:
		ret << "JR NC, ";
		++curr;

		if (base == base::hex)
		{
			ret << std::setw(2) << std::setfill('0');
			ret << static_cast<uint16_t>(*curr);
		}
		else
			ret << static_cast<int16_t>(static_cast<char>(*curr));

		++curr;
		break;
	case 0x31:
		ret << "LD SP, ";

		if (base == base::hex)
			ret << std::setw(4) << std::setfill('0');

		ret << *reinterpret_cast<const uint16_t*>(++curr);
		curr += 2;
		break;
	case 0x32:
		ret << "LD (";

		if (base == base::hex)
			ret << std::setw(4) << std::setfill('0');

		ret << *reinterpret_cast<const uint16_t*>(++curr);
		ret << "), A";
		curr += 2;
		break;
	case 0x33:
		ret << "INC SP";
		++curr;
		break;
	case 0x34:
		ret << "INC (HL)";
		++curr;
		break;
	case 0x35:
		ret << "DEC (HL)";
		++curr;
		break;
	case 0x36:
		ret << "LD (HL), ";

		if (base == base::hex)
			ret << std::setw(2) << std::setfill('0');

		ret << static_cast<uint16_t>(*++curr);
		++curr;
		break;
	case 0x37:
		ret << "SCF";
		++curr;
		break;
	case 0x38:
		ret << "JR C, ";
		++curr;

		if (base == base::hex)
		{
			ret << std::setw(2) << std::setfill('0');
			ret << static_cast<uint16_t>(*curr);
		}
		else
			ret << static_cast<int16_t>(static_cast<char>(*curr));

		++curr;
		break;
	case 0x39:
		ret << "ADD HL, SP";
		++curr;
		break;
	case 0x3a:
		ret << "LD A, (";

		if (base == base::hex)
		{
			ret << std::setw(4) << std::setfill('0');
		}

		ret << *reinterpret_cast<const uint16_t*>(++curr);
		ret << ')';
		curr += 2;
		break;
	case 0x3b:
		ret << "DEC SP";
		++curr;
		break;
	case 0x3c:
		ret << "INC A";
		++curr;
		break;
	case 0x3d:
		ret << "DEC A";
		++curr;
		break;
	case 0x3e:
		ret << "LD A, ";

		if (base == base::hex)
		{
			ret << std::setw(2) << std::setfill('0');
		}

		ret << static_cast<uint16_t>(*++curr);
		++curr;
		break;
	case 0x3f:
		ret << "CCF";
		++curr;
		break;
	case 0x40:
		ret << "LD B, B";
		++curr;
		break;
	case 0x41:
		ret << "LD B, C";
		++curr;
		break;
	case 0x42:
		ret << "LD B, D";
		++curr;
		break;
	case 0x43:
		ret << "LD B, E";
		++curr;
		break;
	case 0x44:
		ret << "LD B, H";
		++curr;
		break;
	case 0x45:
		ret << "LD B, L";
		++curr;
		break;
	case 0x46:
		ret << "LD B, (HL)";
		++curr;
		break;
	case 0x47:
		ret << "LD B, A";
		++curr;
		break;
	case 0x48:
		ret << "LD C, B";
		++curr;
		break;
	case 0x49:
		ret << "LD C, C";
		++curr;
		break;
	case 0x4a:
		ret << "LD C, D";
		++curr;
		break;
	case 0x4b:
		ret << "LD C, E";
		++curr;
		break;
	case 0x4c:
		ret << "LD C, H";
		++curr;
		break;
	case 0x4d:
		ret << "LD C, L";
		++curr;
		break;
	case 0x4e:
		ret << "LD C, (HL)";
		++curr;
		break;
	case 0x4f:
		ret << "LD C, A";
		++curr;
		break;
	case 0x50:
		ret << "LD D, B";
		++curr;
		break;
	case 0x51:
		ret << "LD D, C";
		++curr;
		break;
	case 0x52:
		ret << "LD D, D";
		++curr;
		break;
	case 0x53:
		ret << "LD D, E";
		++curr;
		break;
	case 0x54:
		ret << "LD D, H";
		++curr;
		break;
	case 0x55:
		ret << "LD D, L";
		++curr;
		break;
	case 0x56:
		ret << "LD D, (HL)";
		++curr;
		break;
	case 0x57:
		ret << "LD D, A";
		++curr;
		break;
	case 0x58:
		ret << "LD E, B";
		++curr;
		break;
	case 0x59:
		ret << "LD E, C";
		++curr;
		break;
	case 0x5a:
		ret << "LD E, D";
		++curr;
		break;
	case 0x5b:
		ret << "LD E, E";
		++curr;
		break;
	case 0x5c:
		ret << "LD E, H";
		++curr;
		break;
	case 0x5d:
		ret << "LD E, L";
		++curr;
		break;
	case 0x5e:
		ret << "LD E, (HL)";
		++curr;
		break;
	case 0x5f:
		ret << "LD E, A";
		++curr;
		break;
	case 0x60:
		ret << "LD H, B";
		++curr;
		break;
	case 0x61:
		ret << "LD H, C";
		++curr;
		break;
	case 0x62:
		ret << "LD H, D";
		++curr;
		break;
	case 0x63:
		ret << "LD H, E";
		++curr;
		break;
	case 0x64:
		ret << "LD H, H";
		++curr;
		break;
	case 0x65:
		ret << "LD H, L";
		++curr;
		break;
	case 0x66:
		ret << "LD H, (HL)";
		++curr;
		break;
	case 0x67:
		ret << "LD H, A";
		++curr;
		break;
	case 0x68:
		ret << "LD L, B";
		++curr;
		break;
	case 0x69:
		ret << "LD L, C";
		++curr;
		break;
	case 0x6a:
		ret << "LD L, D";
		++curr;
		break;
	case 0x6b:
		ret << "LD L, E";
		++curr;
		break;
	case 0x6c:
		ret << "LD L, H";
		++curr;
		break;
	case 0x6d:
		ret << "LD L, L";
		++curr;
		break;
	case 0x6e:
		ret << "LD L, (HL)";
		++curr;
		break;
	case 0x6f:
		ret << "LD L, A";
		++curr;
		break;
	case 0x70:
		ret << "LD (HL), B";
		++curr;
		break;
	case 0x71:
		ret << "LD (HL), C";
		++curr;
		break;
	case 0x72:
		ret << "LD (HL), D";
		++curr;
		break;
	case 0x73:
		ret << "LD (HL), E";
		++curr;
		break;
	case 0x74:
		ret << "LD (HL), H";
		++curr;
		break;
	case 0x75:
		ret << "LD (HL), L";
		++curr;
		break;
	case 0x76:
		ret << "HALT";
		++curr;
		break;
	case 0x77:
		ret << "LD (HL), A";
		++curr;
		break;
	case 0x78:
		ret << "LD A, B";
		++curr;
		break;
	case 0x79:
		ret << "LD A, C";
		++curr;
		break;
	case 0x7a:
		ret << "LD A, D";
		++curr;
		break;
	case 0x7b:
		ret << "LD A, E";
		++curr;
		break;
	case 0x7c:
		ret << "LD A, H";
		++curr;
		break;
	case 0x7d:
		ret << "LD A, L";
		++curr;
		break;
	case 0x7e:
		ret << "LD A, (HL)";
		++curr;
		break;
	case 0x7f:
		ret << "LD A, A";
		++curr;
		break;
	case 0x80:
		ret << "ADD A, B";
		++curr;
		break;
	case 0x81:
		ret << "ADD A, C";
		++curr;
		break;
	case 0x82:
		ret << "ADD A, D";
		++curr;
		break;
	case 0x83:
		ret << "ADD A, E";
		++curr;
		break;
	case 0x84:
		ret << "ADD A, H";
		++curr;
		break;
	case 0x85:
		ret << "ADD A, L";
		++curr;
		break;
	case 0x86:
		ret << "ADD A, (HL)";
		++curr;
		break;
	case 0x87:
		ret << "ADD A, A";
		++curr;
		break;
	case 0x88:
		ret << "ADC A, B";
		++curr;
		break;
	case 0x89:
		ret << "ADC A, C";
		++curr;
		break;
	case 0x8a:
		ret << "ADC A, D";
		++curr;
		break;
	case 0x8b:
		ret << "ADC A, E";
		++curr;
		break;
	case 0x8c:
		ret << "ADC A, H";
		++curr;
		break;
	case 0x8d:
		ret << "ADC A, L";
		++curr;
		break;
	case 0x8e:
		ret << "ADC A, (HL)";
		++curr;
		break;
	case 0x8f:
		ret << "ADC A, A";
		++curr;
		break;
	case 0x90:
		ret << "SUB B";
		++curr;
		break;
	case 0x91:
		ret << "SUB C";
		++curr;
		break;
	case 0x92:
		ret << "SUB D";
		++curr;
		break;
	case 0x93:
		ret << "SUB E";
		++curr;
		break;
	case 0x94:
		ret << "SUB H";
		++curr;
		break;
	case 0x95:
		ret << "SUB L";
		++curr;
		break;
	case 0x96:
		ret << "SUB (HL)";
		++curr;
		break;
	case 0x97:
		ret << "SUB A";
		++curr;
		break;
	case 0x98:
		ret << "SBC A, B";
		++curr;
		break;
	case 0x99:
		ret << "SBC A, C";
		++curr;
		break;
	case 0x9a:
		ret << "SBC A, D";
		++curr;
		break;
	case 0x9b:
		ret << "SBC A, E";
		++curr;
		break;
	case 0x9c:
		ret << "SBC A, H";
		++curr;
		break;
	case 0x9d:
		ret << "SBC A, L";
		++curr;
		break;
	case 0x9e:
		ret << "SBC A, (HL)";
		++curr;
		break;
	case 0x9f:
		ret << "SBC A, A";
		++curr;
		break;
	case 0xa0:
		ret << "AND B";
		++curr;
		break;
	case 0xa1:
		ret << "AND C";
		++curr;
		break;
	case 0xa2:
		ret << "AND D";
		++curr;
		break;
	case 0xa3:
		ret << "AND E";
		++curr;
		break;
	case 0xa4:
		ret << "AND H";
		++curr;
		break;
	case 0xa5:
		ret << "AND L";
		++curr;
		break;
	case 0xa6:
		ret << "AND (HL)";
		++curr;
		break;
	case 0xa7:
		ret << "AND A";
		++curr;
		break;
	case 0xa8:
		ret << "XOR B";
		++curr;
		break;
	case 0xa9:
		ret << "XOR C";
		++curr;
		break;
	case 0xaa:
		ret << "XOR D";
		++curr;
		break;
	case 0xab:
		ret << "XOR E";
		++curr;
		break;
	case 0xac:
		ret << "XOR H";
		++curr;
		break;
	case 0xad:
		ret << "XOR L";
		++curr;
		break;
	case 0xae:
		ret << "XOR (HL)";
		++curr;
		break;
	case 0xaf:
		ret << "XOR A";
		++curr;
		break;
	case 0xb0:
		ret << "OR B";
		++curr;
		break;
	case 0xb1:
		ret << "OR C";
		++curr;
		break;
	case 0xb2:
		ret << "OR D";
		++curr;
		break;
	case 0xb3:
		ret << "OR E";
		++curr;
		break;
	case 0xb4:
		ret << "OR H";
		++curr;
		break;
	case 0xb5:
		ret << "OR L";
		++curr;
		break;
	case 0xb6:
		ret << "OR (HL)";
		++curr;
		break;
	case 0xb7:
		ret << "OR A";
		++curr;
		break;
	case 0xb8:
		ret << "CP B";
		++curr;
		break;
	case 0xb9:
		ret << "CP C";
		++curr;
		break;
	case 0xba:
		ret << "CP D";
		++curr;
		break;
	case 0xbb:
		ret << "CP E";
		++curr;
		break;
	case 0xbc:
		ret << "CP H";
		++curr;
		break;
	case 0xbd:
		ret << "CP L";
		++curr;
		break;
	case 0xbe:
		ret << "CP (HL)";
		++curr;
		break;
	case 0xbf:
		ret << "CP A";
		++curr;
		break;
	case 0xc0:
		ret << "RET NZ";
		++curr;
		break;
	case 0xc1:
		ret << "POP BC";
		++curr;
		break;
	case 0xc2:
		ret << "JP NZ, ";

		if (base == base::hex)
		{
			ret << std::setw(4) << std::setfill('0');
		}

		ret << *reinterpret_cast<const uint16_t*>(++curr);
		curr += 2;
		break;
	case 0xc3:
		ret << "JP ";

		if (base == base::hex)
		{
			ret << std::setw(4) << std::setfill('0');
		}

		ret << *reinterpret_cast<const uint16_t*>(++curr);
		curr += 2;
		break;
	case 0xc4:
		ret << "CALL NZ, ";

		if (base == base::hex)
		{
			ret << std::setw(4) << std::setfill('0');
		}

		ret << *reinterpret_cast<const uint16_t*>(++curr);
		curr += 2;
		break;
	case 0xc5:
		ret << "PUSH BC";
		++curr;
		break;
	case 0xc6:
		ret << "ADD A, ";

		if (base == base::hex)
		{
			ret << std::setw(2) << std::setfill('0');
		}

		ret << static_cast<uint16_t>(*++curr);
		++curr;
		break;
	case 0xc7:
		ret << "RST 00H";
		++curr;
		break;
	case 0xc8:
		ret << "RET Z";
		++curr;
		break;
	case 0xc9:
		ret << "RET";
		++curr;
		break;
	case 0xca:
		ret << "JP Z, ";

		if (base == base::hex)
		{
			ret << std::setw(4) << std::setfill('0');
		}

		ret << *reinterpret_cast<const uint16_t*>(++curr);
		curr += 2;
		break;
	case 0xcb:
		ret << dump_bits(++curr);
		break;
	case 0xcc:
		ret << "CALL Z, ";

		if (base == base::hex)
		{
			ret << std::setw(4) << std::setfill('0');
		}

		ret << *reinterpret_cast<const uint16_t*>(++curr);
		curr += 2;
		break;
	case 0xcd:
		ret << "CALL ";

		if (base == base::hex)
		{
			ret << std::setw(4) << std::setfill('0');
		}

		ret << *reinterpret_cast<const uint16_t*>(++curr);
		curr += 2;
		break;
	case 0xce:
		ret << "ADC A, ";

		if (base == base::hex)
		{
			ret << std::setw(2) << std::setfill('0');
		}

		ret << static_cast<uint16_t>(*++curr);
		++curr;
		break;
	case 0xcf:
		ret << "RST 08H";
		++curr;
		break;
	case 0xd0:
		ret << "RET NC";
		++curr;
		break;
	case 0xd1:
		ret << "POP DE";
		++curr;
		break;
	case 0xd2:
		ret << "JP NC, ";

		if (base == base::hex)
		{
			ret << std::setw(4) << std::setfill('0');
		}

		ret << *reinterpret_cast<const uint16_t*>(++curr);
		curr += 2;
		break;
	case 0xd3:
		ret << "OUT (";

		if (base == base::hex)
		{
			ret << std::setw(2) << std::setfill('0');
		}

		ret << static_cast<uint16_t>(*++curr);
		ret << "), A";
		++curr;
		break;
	case 0xd4:
		ret << "CALL NC, ";

		if (base == base::hex)
		{
			ret << std::setw(4) << std::setfill('0');
		}

		ret << *reinterpret_cast<const uint16_t*>(++curr);
		curr += 2;
		break;
	case 0xd5:
		ret << "PUSH DE";
		++curr;
		break;
	case 0xd6:
		ret << "SUB ";

		if (base == base::hex)
		{
			ret << std::setw(2) << std::setfill('0');
		}

		ret << static_cast<uint16_t>(*++curr);
		++curr;
		break;
	case 0xd7:
		ret << "RST 10H";
		++curr;
		break;
	case 0xd8:
		ret << "RET C";
		++curr;
		break;
	case 0xd9:
		ret << "EXX";
		++curr;
		break;
	case 0xda:
		ret << "JP C, ";

		if (base == base::hex)
		{
			ret << std::setw(4) << std::setfill('0');
		}

		ret << *reinterpret_cast<const uint16_t*>(++curr);
		curr += 2;
		break;
	case 0xdb:
		ret << "IN A, (";

		if (base == base::hex)
		{
			ret << std::setw(2) << std::setfill('0');
		}

		ret << static_cast<uint16_t>(*++curr);
		ret << ')';
		++curr;
		break;
	case 0xdc:
		ret << "CALL C, ";

		if (base == base::hex)
		{
			ret << std::setw(4) << std::setfill('0');
		}

		ret << *reinterpret_cast<const uint16_t*>(++curr);
		curr += 2;
		break;
	case 0xdd:
		ret << dump_IX_IY(++curr, 'X', base);
		break;
	case 0xde:
		ret << "SBC A, ";

		if (base == base::hex)
		{
			ret << std::setw(2) << std::setfill('0');
		}

		ret << static_cast<uint16_t>(*++curr);
		++curr;
		break;
	case 0xdf:
		ret << "RST 18H";
		++curr;
		break;
	case 0xe0:
		ret << "RET PO";
		++curr;
		break;
	case 0xe1:
		ret << "POP HL";
		++curr;
		break;
	case 0xe2:
		ret << "JP PO, ";

		if (base == base::hex)
		{
			ret << std::setw(4) << std::setfill('0');
		}

		ret << *reinterpret_cast<const uint16_t*>(++curr);
		curr += 2;
		break;
	case 0xe3:
		ret << "EX (SP), HL";
		++curr;
		break;
	case 0xe4:
		ret << "CALL PO, ";

		if (base == base::hex)
		{
			ret << std::setw(4) << std::setfill('0');
		}

		ret << *reinterpret_cast<const uint16_t*>(++curr);
		curr += 2;
		break;
	case 0xe5:
		ret << "PUSH HL";
		++curr;
		break;
	case 0xe6:
		ret << "AND ";

		if (base == base::hex)
		{
			ret << std::setw(2) << std::setfill('0');
		}

		ret << static_cast<uint16_t>(*++curr);
		++curr;
		break;
	case 0xe7:
		ret << "RST 20H";
		++curr;
		break;
	case 0xe8:
		ret << "RET PE";
		++curr;
		break;
	case 0xe9:
		ret << "JP (HL)";
		++curr;
		break;
	case 0xea:
		ret << "JP PE, ";

		if (base == base::hex)
		{
			ret << std::setw(4) << std::setfill('0');
		}

		ret << *reinterpret_cast<const uint16_t*>(++curr);
		curr += 2;
		break;
	case 0xeb:
		ret << "EX DE, HL";
		++curr;
		break;
	case 0xec:
		ret << "CALL PE, ";

		if (base == base::hex)
		{
			ret << std::setw(4) << std::setfill('0');
		}

		ret << *reinterpret_cast<const uint16_t*>(++curr);
		curr += 2;
		break;
	case 0xed:
		ret << dump_ext(++curr, base);
		break;
	case 0xee:
		ret << "XOR ";

		if (base == base::hex)
		{
			ret << std::setw(2) << std::setfill('0');
		}

		ret << static_cast<uint16_t>(*++curr);
		++curr;
		break;
	case 0xef:
		ret << "RST 28H";
		++curr;
		break;
	case 0xf0:
		ret << "RET P";
		++curr;
		break;
	case 0xf1:
		ret << "POP AF";
		++curr;
		break;
	case 0xf2:
		ret << "JP P, ";

		if (base == base::hex)
		{
			ret << std::setw(4) << std::setfill('0');
		}

		ret << *reinterpret_cast<const uint16_t*>(++curr);
		curr += 2;
		break;
	case 0xf3:
		ret << "DI";
		++curr;
		break;
	case 0xf4:
		ret << "CALL P, ";

		if (base == base::hex)
		{
			ret << std::setw(4) << std::setfill('0');
		}

		ret << *reinterpret_cast<const uint16_t*>(++curr);
		curr += 2;
		break;
	case 0xf5:
		ret << "PUSH AF";
		++curr;
		break;
	case 0xf6:
		ret << "OR ";

		if (base == base::hex)
		{
			ret << std::setw(2) << std::setfill('0');
		}

		ret << static_cast<uint16_t>(*++curr);
		++curr;
		break;
	case 0xf7:
		ret << "RST 30H";
		++curr;
		break;
	case 0xf8:
		ret << "RET M";
		++curr;
		break;
	case 0xf9:
		ret << "LD SP, HL";
		++curr;
		break;
	case 0xfa:
		ret << "JP M, ";

		if (base == base::hex)
		{
			ret << std::setw(4) << std::setfill('0');
		}

		ret << *reinterpret_cast<const uint16_t*>(++curr);
		curr += 2;
		break;
	case 0xfb:
		ret << "EI";
		++curr;
		break;
	case 0xfc:
		ret << "CALL M, ";

		if (base == base::hex)
		{
			ret << std::setw(4) << std::setfill('0');
		}

		ret << *reinterpret_cast<const uint16_t*>(++curr);
		curr += 2;
		break;
	case 0xfd:
		ret << dump_IX_IY(++curr, 'Y', base);
		break;
	case 0xfe:
		ret << "CP ";

		if (base == base::hex)
		{
			ret << std::setw(2) << std::setfill('0');
		}

		ret << static_cast<uint16_t>(*++curr);
		++curr;
		break;
	case 0xff:
		ret << "RST 38H";
		++curr;
		break;
	}

	ret << '\n';
	return ret.str();
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
		const std::string line = fetch_opcode(first, base);

		offset = first - curr;

		for (; curr < first; ++curr)
		{
			if (base == base::hex)
				ss << std::uppercase << std::hex <<
					std::setfill('0') << std::setw(2);
			else
				ss << std::setw(3);

			ss << static_cast<int>(*curr) << ' ';
		}

		bytes = ss.str();
		bytes.resize(static_cast<std::size_t>(5) *
			static_cast<std::size_t>(base == base::hex ? 3 : 4), ' ');

		if (base == base::hex)
		{
			std::cout << std::uppercase << std::hex <<
				std::setfill('0') << std::setw(4);
		}
		else
			std::cout << std::setw(5);

		std::cout << addr << "   ";
		std::cout << bytes << line;
		addr += offset;
	}
}
