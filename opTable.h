#include <string>
#include <map>
using namespace std;

int opcode(string mnemonic)
{
	map<string, int> opTable;
	opTable["ADD"] = 0x18;
	opTable["ADDF"] = 0x58;
	opTable["ADDR"] = 0x90;
	opTable["AND"] = 0x40;
	opTable["CLEAR"] = 0xB4;
	opTable["COMP"] = 0x28;
	opTable["COMPF"] = 0x88;
	opTable["COMPR"] = 0xA0;
	opTable["DIV"] = 0x24;
	opTable["DIVF"] = 0x9C;
	opTable["FIX"] = 0xC4;
	opTable["FLOAT"] = 0xC0;
	opTable["HIO"] = 0xF4;
	opTable["J"] = 0x3C;
	opTable["JEQ"] = 0x30;
	opTable["JGT"] = 0x34;
	opTable["JLT"] = 0x38;
	opTable["JSUB"] = 0x48;
	opTable["LDA"] = 0x00;
	opTable["LDB"] = 0x68;
	opTable["LDCH"] = 0x50;
	opTable["LDF"] = 0x70;
	opTable["LDL"] = 0x08;
	opTable["LDS"] = 0x6C;
	opTable["LDT"] = 0x74;
	opTable["LDX"] = 0x04;
	opTable["LPS"] = 0xD0;
	opTable["MULF"] = 0x60;
	opTable["MULR"] = 0x98;
	opTable["NORM"] = 0xC8;
	opTable["OR"] = 0x44;
	opTable["RD"] = 0xD8;
	opTable["RMO"] = 0xAC;
	opTable["RSUB"] = 0x4C;
	opTable["SHIFTL"] = 0xA4;
	opTable["SHIFTR"] = 0xA8;
	opTable["SIO"] = 0xF0;
	opTable["SSK"] = 0xEC;
	opTable["STA"] = 0x0C;
	opTable["STB"] = 0x78;
	opTable["STCH"] = 0x54;
	opTable["STF"] = 0x80;
	opTable["STI"] = 0xD4;
	opTable["STL"] = 0x14;
	opTable["STS"] = 0x7C;
	opTable["STSW"] = 0xE8;
	opTable["STT"] = 0x84;
	opTable["STX"] = 0x10;
	opTable["SUB"] = 0x1C;
	opTable["SUBF"] = 0x5C;
	opTable["SUBR"] = 0x94;
	opTable["SVC"] = 0xB0;
	opTable["TD"] = 0xE0;
	opTable["TIO"] = 0xF8;
	opTable["TIX"] = 0x2C;
	opTable["TIXR"] = 0xB8;
	opTable["WD"] = 0xDC;
	return opTable.count(mnemonic) ? opTable[mnemonic] : -1;
}