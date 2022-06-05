#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <map>
// #include <vector>
#include "complie.h"
#include "symbolTable.h"
using namespace std;

bool is_start = false;
int line_number = 1;
int loc = 0;
map<string, int> symTable;
map<int, string> error_log;

// void store_symbol(string label)
// {
// 	if (!label.empty())
// 	{
// 		if (opcode(label) != -1)
// 			error_log[line_number] = "Cannot use mnemonic as label: " + label;
// 		else if (symTable.count(label) != 0)
// 			error_log[line_number] = "Redefined symbol: " + label;
// 		else
// 			symTable[label] = loc;
// 	}
// }

// int check_operand(string mnemoic, string operand)
// {
// 	if (operand.empty() && mnemoic != "RSUB")
// 	{
// 		error_log[line_number] = "Need operand.";
// 		return 0;
// 	}
// 	else
// 		return 1;
// }

// int check_mnemoic(string mnemoic)
// {
// 	if (
// 		opcode(mnemoic) == -1
// 		&& mnemoic != "START"
// 		&& mnemoic != "END"
// 		&& mnemoic != "RESB"
// 		&& mnemoic != "RESW"
// 		&& mnemoic != "WORD"
// 		&& mnemoic != "BYTE"
// 		)
// 	{
// 		error_log[line_number] = "Invalid mnemoic: " + mnemoic;
// 		return 0;
// 	}
// 	else
// 		return 1;
// }

// compile and ingnore empty line
int read_code(string input_line)
{
	// [ Label, Mnemoic, Operand ]
	map<string, string> statement = compile(input_line);
	string label = statement["Label"];
	string mnemoic = statement["Mnemoic"];
	string operand = statement["Operand"];
	string addressing = statement["Addressing"];

	// Output Label, Mnemoic, Operand
	if (!(label.empty() && mnemoic.empty() && operand.empty()))
	{
		cout << hex << loc << "\t";
		cout << label << "\t";
		cout << mnemoic << "\t";
		cout << setfill(' ') << setw(9) << left
				 << operand << "\t";
		// Output Opcode
		if (opcode(mnemoic) != -1)
			cout << setfill('0') << setw(2) << opcode(mnemoic) << "\t";
		else
			cout << "\t";
		// Output Addressing Mode
		if (!statement["Addressing"].empty())
			cout << statement["Addressing"];
		cout << "\n";

		// END Program ?
		if (mnemoic == "END")
		{
			is_start = false;
			return 0;
		}
		// START from this line
		else if (mnemoic == "START")
			loc = stoi(operand, 0, 16); // hex string to int, set start loc
		// RESB
		else if (mnemoic == "RESB")
		{
			int byte = stoi(operand);
			loc += byte;
		}
		// RESW
		else if (mnemoic == "RESW")
		{
			int word = stoi(operand);
			loc += word * 3;
		}
		else
		{
			loc += 3;
		}
	}
	return 1;
}

// Main
int main(int argc, char *argv[])
{
	string input_line;
	while (cin)
	{
		getline(cin, input_line);
		// START
		if (input_line.find("START") != string::npos)
			is_start = true;
		// Read Line
		if (is_start && input_line.length() > 1)
		{
			if (!read_code(input_line))
				break;
		}
		line_number++;
	}

	// // Output Symble Table
	// cout << "\nSymbol Table" << endl;

	// for (auto iter = symTable.begin(); iter != symTable.end(); iter++)
	// {
	// 	// cout << setfill(' ') << setw(10) << left
	// 	// 		 << operand << "\t";
	// 	cout << iter->first << ":\t" 
	// 			 << left << iter->second << "\n";
	// }
	// cout << "\n";

	// // Output Error log
	// cout << "Error log: " << endl;

	// for (auto iter = error_log.begin(); iter != error_log.end(); iter++)
	// {
	// 	cout << "[ #" << to_string(iter->first)
	// 			 << " " << iter->second << " ]\n";
	// }
	// cout << "\n\n";

	// End Main
	return 0;
}
