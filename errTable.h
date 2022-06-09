#include <string>
#include <map>
using namespace std;

string err_message(string err_type)
{
	map<string, string> errTable;
	errTable["invalid_syntax"] = "Invalid instruction syntax, possibly too long or wrong whitespaces.";
	// operand
	errTable["operand_START"] = "Program must start from a heximal location counter. -> ";
	errTable["operand_RESB"] = "'RESB' needs a decimal operand. -> ";
	errTable["operand_RESW"] = "'RESW' needs a decimal operand. -> ";
	errTable["operand_BYTE_type"] = "The content type of 'BYTE' must be 'C' or 'X'. -> ";
	errTable["operand_BYTE_quote"] = "The content of 'BYTE' needs single quotations.";
	errTable["operand_BYTE_empty"] = "The content of 'BYTE' cannot be empty.";
	errTable["operand_BYTE_len"] = "The content length of type 'X' must be even. -> ";
	errTable["operand_BYTE_hex"] = "The content of type 'X' must be heximal. -> ";
	errTable["operand_RSUB"] = "'RSUB' cannot have operand. -> ";
	errTable["operand_empty"] = "Operand cannot be empty.";
	errTable["operand_addressing"] = "Invalid syntax of index addressing. -> ";
	errTable["operand_opcode"] = "Cannot use opcode to be operand. -> ";
	// mnemoic
	errTable["mnemoic_invalid"] = "Invalid mnemoic. -> ";
	// label
	errTable["lable_mnemoic"] = "Symbol cannot be same with mnemoic. -> ";
	errTable["lable_operand"] = "Symbol cannot be same with operand. -> ";
	// pseudo
	errTable["START"] = "Program do not start from 'START'.";
	errTable["END"] = "Program do not end at 'END'.";
	// symbol
	errTable["symbol_redefined"] = "Redefined symbol. -> ";
	return errTable.count(err_type) ? errTable[err_type] : "Cannot find this error type.";
}

// FIXME: label use opcode?
// FIXME: all base 16