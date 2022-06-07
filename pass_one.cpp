#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include "complie.h"
using namespace std;

int line_number = 1;
int loc = 0;
// compile and ingnore empty line
void read_code(string input)
{
	try
	{ // statement [Label, Mnemoic, Operand, Addressing]
		map<string, string> statement = compile(input);
		string label = statement["Label"];
		string mnemoic = statement["Mnemoic"];
		string operand = statement["Operand"];
		string addressing = statement["Addressing"];
		// Output Label, Mnemoic, Operand
		if (!(label.empty() && mnemoic.empty() && operand.empty()))
		{
			cout << hex << loc << '\t';
			cout << label << '\t';
			cout << mnemoic << '\t';
			cout << setfill(' ') << setw(9) << left
					 << operand << '\t';
			// Output Opcode
			if (opcode(mnemoic) != -1)
				cout << setfill('0') << setw(2) << opcode(mnemoic) << '\t';
			else
				cout << '\t';
			// Output Addressing Mode
			if (!addressing.empty())
				cout << addressing;

			cout << "\n";
			// Check Mnemoic, Operand and set location counter
			if (mnemoic == "START") // START from this line
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
				if (opcode(mnemoic) == -1 && mnemoic != "WORD" && mnemoic != "END")
					throw "Invalid mnemoic.";
				else
				{
					if (operand.empty())
						throw "Operand cannot be empty.";
					else if (
							operand.find(',') != string::npos &&
							operand.substr(operand.find(','), operand.back()) != ",X")
					{
						throw "Invalid syntax of index addressing.";
					}
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
	}
	catch (const char *err_massage)
	{
		cout << "\033[0;31m"
				 << "Line " << dec << line_number
				 << ": " << err_massage << "\033[0m\n";
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
		if (input.length() > 1)
		{
			read_code(input);
		}
		line_number++;
	}
	// End Main
	return 0;
}
