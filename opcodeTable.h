#include <iostream>
#include <string>
#include <map>
using namespace std;

int opcode(string mnemonic)
{
	map<string, int> opcodeTable;

	opcodeTable["ADD"] = 0x18;
	opcodeTable["ADDF"] = 0x58;
	opcodeTable["ADDR"] = 0x90;
	opcodeTable["AND"] = 0x40;
	opcodeTable["CLEAR"] = 0xB4;
	opcodeTable["COMP"] = 0x28;
	opcodeTable["COMPF"] = 0x88;
	opcodeTable["COMPR"] = 0xA0;
	opcodeTable["DIV"] = 0x24;
	opcodeTable["DIVF"] = 0x9C;
	opcodeTable["FIX"] = 0xC4;
	opcodeTable["FLOAT"] = 0xC0;
	opcodeTable["HIO"] = 0xF4;
	opcodeTable["J"] = 0x3C;
	opcodeTable["JEQ"] = 0x30;
	opcodeTable["JGT"] = 0x34;
	opcodeTable["JLT"] = 0x38;
	opcodeTable["JSUB"] = 0x48;
	opcodeTable["LDA"] = 0x00;
	opcodeTable["LDB"] = 0x68;
	opcodeTable["LDCH"] = 0x50;
	opcodeTable["LDF"] = 0x70;
	opcodeTable["LDL"] = 0x08;
	opcodeTable["LDS"] = 0x6C;
	opcodeTable["LDT"] = 0x74;
	opcodeTable["LDX"] = 0x04;
	opcodeTable["LPS"] = 0xD0;
	opcodeTable["MULF"] = 0x60;
	opcodeTable["MULR"] = 0x98;
	opcodeTable["NORM"] = 0xC8;
	opcodeTable["OR"] = 0x44;
	opcodeTable["RD"] = 0xD8;
	opcodeTable["RMO"] = 0xAC;
	opcodeTable["RSUB"] = 0x4C;
	opcodeTable["SHIFTL"] = 0xA4;
	opcodeTable["SHIFTR"] = 0xA8;
	opcodeTable["SIO"] = 0xF0;
	opcodeTable["SSK"] = 0xEC;
	opcodeTable["STA"] = 0x0C;
	opcodeTable["STB"] = 0x78;
	opcodeTable["STCH"] = 0x54;
	opcodeTable["STF"] = 0x80;
	opcodeTable["STI"] = 0xD4;
	opcodeTable["STL"] = 0x14;
	opcodeTable["STS"] = 0x7C;
	opcodeTable["STSW"] = 0xE8;
	opcodeTable["STT"] = 0x84;
	opcodeTable["STX"] = 0x10;
	opcodeTable["SUB"] = 0x1C;
	opcodeTable["SUBF"] = 0x5C;
	opcodeTable["SUBR"] = 0x94;
	opcodeTable["SVC"] = 0xB0;
	opcodeTable["TD"] = 0xE0;
	opcodeTable["TIO"] = 0xF8;
	opcodeTable["TIX"] = 0x2C;
	opcodeTable["TIXR"] = 0xB8;
	opcodeTable["WD"] = 0xDC;

	if (opcodeTable.count(mnemonic))
	{
		return opcodeTable[mnemonic];
	}
	else
	{
		return -1;
	}
}