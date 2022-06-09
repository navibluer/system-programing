#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
#include <map>
#include "CompileOne.h"
using namespace std;

// Pass One
fstream pass_one;
map<string, int> symTable;
int line_number = 1;
int loc = 0;
int has_error = 0;
// Pass Two
vector<string> instruction;
vector<int> m_record;
vector<string> obj_program;

// Check Pseudo Instruction
int is_pseudo(string mnemoic)
{
	if (mnemoic == "START")
		return 1;
	else if (mnemoic == "END")
		return 1;
	else if (mnemoic == "BYTE")
		return 1;
	else if (mnemoic == "WORD")
		return 1;
	else if (mnemoic == "RESB")
		return 1;
	else if (mnemoic == "RESW")
		return 1;
	else
		return 0;
}

// Compile and ingnore empty line
void CompileOne(string input)
{
	try
	{ // statement [Label, Mnemoic, Operand, Addressing]
		map<string, string> statement = compile(input);
		string label = statement["Label"];
		string mnemoic = statement["Mnemoic"];
		string operand = statement["Operand"];
		string addressing = statement["Addressing"];
		// START from this line
		if (mnemoic == "START")
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
		// Output Label, Mnemoic, Operand
		if (!(label.empty() && mnemoic.empty() && operand.empty()))
		{
			pass_one << hex << loc << '\t';
			pass_one << label << '\t';
			pass_one << mnemoic << '\t';
			pass_one << setfill(' ') << setw(9) << left
							 << operand << '\t';
			// Output Opcode
			if (opcode(mnemoic) != -1)
				pass_one << opcode(mnemoic) << '\t';
			else
				pass_one << '\t';
			// Output Addressing Mode
			if (!addressing.empty())
				pass_one << addressing;

			pass_one << "\n";
			// Store Symbol
			if (symTable.count(label) != 0)
				throw err_message("symbol_redefined").append(label);
			else if (!label.empty())
				symTable[label] = loc;
			// Check Mnemoic, Operand and set location counter
			// RESB
			if (mnemoic == "RESB")
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
						mnemoic != "START" &&
						mnemoic != "WORD" &&
						mnemoic != "END")
					throw err_message("mnemoic_invalid").append(mnemoic);
				else
				{
					// Has no Operand
					if (operand.empty())
						throw err_message("operand_empty");
					// Index Addressing Syntax Error
					else if (
							operand.find(',') != string::npos &&
							operand.substr(operand.find(','), operand.back()) != ",X")
					{
						throw err_message("operand_addressing").append(operand);
					}
					// Check Operand
					if (mnemoic != "END")
						m_record.push_back(loc + 1);
					// Next
					if (mnemoic != "START")
						loc += 3;
				}
			}
			// Check Symbol Name
			if (!label.empty() && label == mnemoic)
				throw err_message("lable_mnemoic").append(label);
			if (!label.empty() && label == operand)
				throw err_message("lable_operand").append(label);
			if (opcode(operand) != -1)
				throw err_message("operand_opcode").append(operand);
		}
	}
	catch (const string err_message)
	{
		has_error = 1;
		pass_one << "\033[0;31m"
						 << "#" << dec << line_number
						 << ": " << err_message << "\033[0m\n";
	}
}

void CompileTwo(int instruction_len)
{
	int lines = instruction_len / 6;
	cout << "\033[0;35m" << dec << lines << " lines\033[0m\n";
	// Compile to Objec Program
	string spaces = "  ";
	int t_len = 0;
	int t_start;
	stringstream t_code;
	for (size_t i = 0; i < instruction_len; i += 6)
	{
		// Output Middle File
		cout << "\033[3;32m";
		for (size_t j = i; j < i + 6; j++)
		{
			if (j == i + 3) // Operand
			{
				cout << left << setfill(' ') << setw(9)
						 << instruction.at(j) << "\t";
			}
			else
				cout << instruction.at(j) << "\t";
		}
		cout << "\033[0m\n";
		//
		try
		{
			int loc = stoi(instruction.at(i), 0, 16);
			string symbol = instruction.at(i + 1);
			string mnemoic = instruction.at(i + 2);
			// Operand fill with whitespaces
			string operand = instruction.at(i + 3)
													 .substr(0, instruction.at(i + 3).find(' '));
			int op_code = -1;
			if (!instruction.at(i + 4).empty())
				op_code = stoi(instruction.at(i + 4), 0, 16);
			string addr_mode = instruction.at(i + 5);
			
			// Output Object Program
			if (line_number == 1)
			{
				int program_len =
						stoi(instruction.at(instruction_len - 6), 0, 16) - loc;
				cout << "H" << spaces << setfill(' ') << setw(8) << symbol
						 << right << setfill('0') << setw(6) << operand
						 << spaces << program_len << endl;
				t_start = loc;
			}
			else if (line_number < lines)
			{
				if (loc - t_start < 0x1d)
				{
					// FIXME: index, pseudo
					int target = symTable[operand];
					if (op_code != -1)
					{
						t_code << right << setfill('0') << setw(2) << hex << op_code
									 << target << spaces;
					}
				}
				// else
				// 	t_code.str("");
				// 
				int next_loc = stoi(instruction.at(i + 6), 0, 16);
				if (next_loc - t_start < 0x1d)
					t_len = next_loc - t_start;
				else
				{
					cout << "T" << spaces
							 << right << setfill('0') << setw(6) << hex << t_start
							 << spaces << t_len
							 << spaces << t_code.str()
							 << endl;
					t_start = next_loc;
					t_len = 0;
					t_code.str("");
				}
			}
			else if (line_number == lines)
			{
				cout << "T" << spaces
						 << right << setfill('0') << setw(6) << hex << t_start
						 << spaces << right << setfill('0') << setw(6) << t_len
						 << endl;
			}
		}
		catch (const string err_message)
		{
			cout << "\033[0;31m"
					 << "#" << dec << line_number
					 << ": " << err_message << "\033[0m\n";
		}
		line_number++;
	}
}

void PassOne()
{
	pass_one.open("middle.txt", ios::out | ios::trunc);
	string input;
	while (cin)
	{
		getline(cin, input);
		transform(input.begin(), input.end(), input.begin(), ::toupper);
		if (input.length() > 1)
		{
			CompileOne(input);
		}
		line_number++;
	}
	pass_one.close();
}

void PassTwo()
{
	ifstream middle_file("middle.txt");
	string input;
	int instruction_len;
	line_number = 1;
	// Read Middle File
	while (getline(middle_file, input))
	{
		string tmp = "";
		for (size_t i = 0; i <= input.length(); i++)
		{
			// Split with Tab
			if (input[i] == '\t' || i == input.length())
			{
				instruction.push_back(tmp);
				tmp = "";
			}
			else if (input[i] == ',') // Trim ',X'
			{
				i++;
				continue;
			}
			else
				tmp += input[i];
		}
	}
	instruction_len = instruction.size();
	// Check Start, End
	if (instruction.at(2) != "START")
		cout << "\033[0;31m" << err_message("START") << "\033[0m\n";
	if (instruction.at(instruction_len - 4) != "END")
		cout << "\033[0;31m" << err_message("END") << "\033[0m\n";
	// Compile Instruction
	CompileTwo(instruction_len);
	// Output Modification Record
	// for (const auto &loc : m_record)
	// 	cout << "M  " << loc << "   04" << endl;
}

// Main
int main(int argc, char *argv[])
{
	// Pass One
	PassOne();
	// Output Symble Table
	cout << "\033[0;36m"
			 << "\nSymbol Table" << endl;
	for (auto iter = symTable.begin(); iter != symTable.end(); iter++)
	{
		cout << "[\t" << iter->first
				 << "\t->\t" << hex << iter->second
				 << "\t]\n";
	}
	cout << "\033[0m\n";
	ifstream middle_file("middle.txt");
	// Pass One Has Error, Output Middle File and Error, Exit
	if (has_error)
	{
		cout << middle_file.rdbuf();
		exit(1);
	}
	// Pass Two
	PassTwo();
	// End Main
	exit(0);
}
