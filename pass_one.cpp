#include <iostream>
#include <iomanip>
#include <string>
#include <string.h>
#include <map>
#include "complie_one.h"
using namespace std;

int line_number = 1;
int loc = 0;
// Compile and ingnore empty line
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
					throw err_message("operand_START").append(operand);
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
					throw err_message("operand_RESB").append(operand);
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
					throw err_message("operand_RESW").append(operand);
				}
			}
			// BYTE
			else if (mnemoic == "BYTE")
			{
				int content_len = operand.length() - 3; // type and quotes
				// Error Type
				if (operand.front() != 'C' && operand.front() != 'X')
					throw err_message("operand_BYTE_type") += operand.front();
				else if (operand[1] != '\'' || operand.back() != '\'')
					throw err_message("operand_BYTE_quote");
				else if (content_len <= 0)
					throw err_message("operand_BYTE_empty");
				else
				{
					switch (operand.front())
					{
					case 'C':
						loc += content_len;
						break;

					case 'X':
						if (content_len % 2 != 0)
						{
							throw err_message("operand_BYTE_len")
									.append(operand, 2, operand.length() - 3); //  X' '\n
						}
						else
						{
							try
							{
								stoi( //  X' '\n
										operand.substr(2, operand.length() - 3),
										0, 16);
							}
							catch (invalid_argument &e)
							{
								throw err_message("operand_BYTE_hex")
										.append(operand, 2, operand.length() - 3); //  X' '\n
							}

							loc += content_len / 2;
						}
						break;

					default:
						break;
					}
				}
			}
			// RSUB
			else if (mnemoic == "RSUB")
			{
				if (!operand.empty())
					throw err_message("operand_RSUB").append(operand);
				else
					loc += 3;
			}
			else
			{
				// Not in Opcode Tabel
				if (opcode(mnemoic) == -1 &&
						mnemoic != "WORD" &&
						mnemoic != "END")
					throw err_message("mnemoic_invalid").append(mnemoic);
				else
				{
					if (operand.empty())
						throw err_message("operand_empty");
					else if (
							operand.find(',') != string::npos &&
							operand.substr(operand.find(','), operand.back()) != ",X")
					{
						throw err_message("operand_addressing").append(operand);
					}
					loc += 3;
				}
			}
			// Check Symbol Name
			if (!label.empty() && label == mnemoic)
				throw err_message("lable_mnemoic").append(label);
			if (!label.empty() && label == operand)
				throw err_message("lable_mnemoic").append(label);
			if (opcode(operand) != -1)
				throw err_message("operand_opcode").append(operand);
		}
	}
	// catch (const char *err_message)
	catch (const string err_message)
	{
		cout << "\033[0;31m"
				 << "#" << dec << line_number
				 << ": " << err_message << "\033[0m\n";
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
