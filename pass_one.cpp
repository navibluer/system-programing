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
// map<int, string> error_log;

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

// compile and ingnore empty line
int read_code(string input)
{
	// [ Label, Mnemoic, Operand ]
	try
	{
		map<string, string> statement = compile(input);
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
			if (!addressing.empty())
				cout << addressing;

			cout << "\n";
			// Check Mnemoic and set location counter
			// END Program ?
			if (mnemoic == "END")
			{
				is_start = false;
				return 0;
			}
			// START from this line
			else if (mnemoic == "START")
			{
				try
				{
					// hex string to int, set start loc
					loc = stoi(operand, 0, 16);
				}
				catch (invalid_argument &e)
				{
					throw "Invalid operand, needs a heximal integer.";
				}
			}
			// RESB
			else if (mnemoic == "RESB")
			{
				try
				{
					int byte = stoi(operand);
					loc += byte;
				}
				catch (invalid_argument &e)
				{
					throw "Invalid operand, needs a decimal integer.";
				}
			}
			// RESW
			else if (mnemoic == "RESW")
			{
				try
				{
					int word = stoi(operand);
					loc += word * 3;
				}
				catch (invalid_argument &e)
				{
					throw "Invalid operand, needs a decimal integer.";
				}
			}
			// BYTE
			else if (mnemoic == "BYTE")
			{
				try
				{
					int content_len = operand.length() - 3; // type and quotes
					if (operand.front() != 'C' && operand.front() != 'X')
					{
						throw "Invalid operand, the content type must be 'C' or 'X'.";
					}
					else if (operand[1] != '\'' || operand.back() != '\'')
					{
						throw "Invalid operand, content needs single quotations.";
					}
					else if (content_len <= 0)
					{
						throw "Invalid operand, needs the content.";
					}
					else
					{
						switch (operand.front())
						{
						case 'C':
							loc += content_len;
							break;

						case 'X':
							if (content_len % 2 != 0)
								throw "Invalid operand, the content length of 'BYTE X' must be even.";
							else
								loc += content_len / 2;
							break;

						default:
							break;
						}
					}
				}
				catch (invalid_argument &e)
				{
					throw "Invalid operand, needs a decimal integer.";
				}
			}
			// RSUB
			else if (mnemoic == "RSUB")
			{
				if (!operand.empty())
					throw "'RSUB' cannot have operand.";
				else
					loc += 3;
			}
			else
			{
				// Not in Opcode Tabel
				if (opcode(mnemoic) == -1 && mnemoic != "WORD")
					throw "Invalid mnemoic.";
				else
				{
					if (operand.empty())
						throw "Operand cannot be empty.";
					else
						loc += 3;
				}
			}

			// Check Symbol Name
			if (!label.empty() && label == mnemoic)
				throw "Symbol cannot be same with mnemoic.";
			if (!label.empty() && label == operand)
				throw "Symbol cannot be same with operand.";
			if (opcode(operand) != -1)
				throw "Cannot use opcode to be operand.";
		}
		return 1;
	}
	catch (const char *err_massage)
	{
		cout << "\033[0;31m"
				 << "Line " << dec << line_number
				 << ": " << err_massage << "\033[0m\n";
		return 1;
	}
}

// Main
int main(int argc, char *argv[])
{
	string input;
	while (cin)
	{
		getline(cin, input);
		transform(input.begin(), input.end(), input.begin(), ::toupper);
		// START
		if (input.find("START") != string::npos)
			is_start = true;
		// Read Line
		if (is_start && input.length() > 1)
		{
			if (!read_code(input))
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
	// 	cout << "[ #" << dec << iter->first)
	// 			 << " " << iter->second << " ]\n";
	// }
	// cout << "\n\n";

	// End Main
	return 0;
}
