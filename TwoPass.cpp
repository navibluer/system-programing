#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include "compile.h"
using namespace std;

// Pass One
fstream middle_file;
map<string, int> symTable;
int line_number = 1;
int loc = 0;
bool has_error = false;
// Pass Two
vector<string> instruction;
vector<int> m_record;
vector<string> obj_program;
string spaces = "\t";
string spaces_s = "  ";

// Convert All Char in String
// Original stoi: 5g5 => 5
int better_stoi(string number, int type)
{
	size_t fail_pos = 0;
	int result;
	while (fail_pos != number.length())
	{
		// fail_pos will be set to fail position
		result = stoi(number.substr(fail_pos), &fail_pos, type);
	}
	return result;
}

// Compile statement and ingnore empty line
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
				loc = better_stoi(operand, 16);
			}
			catch (invalid_argument &e)
			{
				throw err_message("operand_START").append(operand);
			}
		}
		// Output Label, Mnemoic, Operand
		if (!(label.empty() && mnemoic.empty() && operand.empty()))
		{
			middle_file << hex << loc << '\t';
			middle_file << label << '\t';
			middle_file << mnemoic << '\t';
			middle_file << setfill(' ') << setw(9) << left
									<< operand << '\t';
			// Output Opcode
			if (opcode(mnemoic) != -1)
				middle_file << opcode(mnemoic) << '\t';
			else
				middle_file << '\t';
			// Output Addressing Mode
			if (!addressing.empty())
				middle_file << addressing << '\t';
			else
				middle_file << '\t';
			// Output Line Number
			middle_file << dec << line_number;
			middle_file << "\n";
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
					int byte = better_stoi(operand, 10);
					// int byte = all_to_dec(operand);
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
					int word = better_stoi(operand, 10);
					loc += word * 3;
				}
				catch (invalid_argument &e)
				{
					throw err_message("operand_RESW").append(operand);
				}
			}
			// WORD
			else if (mnemoic == "WORD")
			{
				try
				{
					int word = better_stoi(operand, 10);
					loc += 3;
				}
				catch (invalid_argument &e)
				{
					throw err_message("operand_WORD").append(operand);
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
							{ //  X' '\n
								better_stoi(operand.substr(2, operand.length() - 3), 16);
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
				// Invalid Mnemoic
				if (opcode(mnemoic) == -1 &&
						mnemoic != "START" &&
						mnemoic != "WORD" &&
						mnemoic != "END")
					throw err_message("mnemoic_invalid").append(mnemoic);
				else
				{
					// Invalid Operand
					if (operand.empty())
						throw err_message("operand_empty");
					// Index Addressing Syntax Error
					else if (
							operand.find(',') != string::npos &&
							operand.substr(operand.find(','), operand.back()) != ",X")
					{
						throw err_message("operand_addressing").append(operand);
					}
					// M Record
					if (mnemoic != "START" &&
							mnemoic != "END" &&
							mnemoic != "WORD")
						m_record.push_back(loc + 1);
					// Next Line
					if (mnemoic != "START")
						loc += 3;
				}
			}
			// Check Symbol Name
			if (!label.empty() && opcode(label) != -1)
				throw err_message("lable_opcode").append(label);
			if (!label.empty() && label == operand)
				throw err_message("lable_operand").append(label);
			if (opcode(operand) != -1)
				throw err_message("operand_opcode").append(operand);
		}
	}
	catch (const string err_message)
	{
		has_error = true;
		middle_file << "\033[0;31m"
								<< "#" << dec << line_number
								<< ": " << err_message << "\033[0m\n";
	}
}

// Compile to Objec Program
void CompileTwo(int instruction_len)
{
	// loc, label, mnemoic, operand, opcode, addressing, source line
	int per_line = 7;
	int total_lines = instruction_len / per_line;
	int t_len = 0;
	int t_start = 0;
	// For length of BYTE C > 30
	bool t_break = false;
	int break_t_len;
	int break_t_start;
	stringstream break_t_code;
	stringstream break_tmp;
	string break_operand_str = "";
	stringstream t_code;
	for (size_t i = 0; i < instruction_len; i += per_line)
	{
		// Output Middle File
		// cout << "\033[3;32m";
		// for (size_t j = i; j < i + per_line; j++)
		// {
		// 	// Operand
		// 	if (j == i + 3)
		// 		cout << left << setfill(' ') << setw(9)
		// 				 << instruction.at(j) << "\t";
		// 	// opcode
		// 	else if (j == i + 4 && (opcode(instruction.at(i + 2)) != -1))
		// 		cout << right << setfill('0') << setw(2)
		// 				 << instruction.at(j) << "\t";
		// 	else
		// 		cout << instruction.at(j) << "\t";
		// }
		// cout << "\033[0m\n";

		// Start Compiling
		int loc = stoi(instruction.at(i), 0, 16);
		string symbol = instruction.at(i + 1);
		string mnemoic = instruction.at(i + 2);
		// Operand fill with whitespaces
		string operand = instruction.at(i + 3)
												 .substr(0, instruction.at(i + 3).find(' '));
		int op_code = !instruction.at(i + 4).empty()
											? stoi(instruction.at(i + 4), 0, 16)
											: -1;
		string addr_mode = instruction.at(i + 5);
		string source_line = instruction.at(i + 6);
		// Check Symbol
		if (mnemoic != "START" && mnemoic != "WORD" &&
				mnemoic != "BYTE" && mnemoic != "RESB" &&
				mnemoic != "RESW" && mnemoic != "RSUB" &&
				!symTable.count(operand))
		{
			cout << "\033[0;31m"
					 << "#" << dec << source_line << ": "
					 << err_message("symbol_not_defined").append(operand)
					 << "\033[0m\n";
			has_error = true;
			return;
		}
		// Handle Addressing
		int addr_code = 0;
		if (addr_mode == "Index")
			addr_code = symTable[operand] + 32768; // 8000(16)
		else																		 // End
			addr_code = symTable[operand];
		// Handle Pseudo Operand: WORD / BYTE
		int operand_int = 0;
		string operand_str = "";
		if (mnemoic == "WORD")
			operand_int = stoi(operand);
		else if (mnemoic == "BYTE")
		{
			switch (operand.front())
			{
			case 'X': // X' '\n
				operand_str = operand.substr(2, operand.length() - 3);
				break;
			case 'C': // C' '\n
				string characters = operand.substr(2, operand.length() - 3);
				int charaters_len = characters.length();
				stringstream tmp;
				if (charaters_len > 0x1e)
				{
					for (size_t i = 0; i < 30; i++)
						tmp << hex << (int)characters[i];
					operand_str = tmp.str();

					t_break = true;
					break_t_len = charaters_len - 30;
					break_t_start = t_start + 30;
					for (size_t i = 30; i < charaters_len; i++)
						break_tmp << hex << (int)characters[i];
					break_operand_str = break_tmp.str();
				}
				else
				{
					for (size_t i = 0; i < charaters_len; i++)
						tmp << hex << (int)characters[i];
					operand_str = tmp.str();
				}
				break;
			}
		}
		// Output Object Program
		if (line_number == 1) // H Record
		{
			int program_len =
					stoi(instruction.at(instruction_len - per_line), 0, 16) - loc;
			cout << "\033[4;36m" << endl;
			cout << "H" << spaces_s
					 << left << setfill(' ') << setw(6) << symbol << spaces
					 << right << setfill('0') << setw(6) << operand << spaces
					 << right << setfill('0') << setw(6) << hex << program_len
					 << endl;
			t_start = loc;
			t_len = 0;
		}
		// T Record Start
		else if (line_number < total_lines - 1)
		{
			string next_mnemoic = instruction.at(i + per_line + 2);
			int next_loc = stoi(instruction.at(i + per_line), 0, 16);
			int next_next_loc = stoi(instruction.at(i + per_line * 2), 0, 16);
			t_len = next_loc - t_start;
			// Append T Record
			if (op_code != -1)
				t_code << right << setfill('0') << setw(2) << hex << op_code
							 << right << setfill('0') << setw(4) << addr_code
							 << spaces;
			else if (mnemoic == "WORD")
				t_code << right << setfill('0') << setw(6)
							 << operand_int << spaces;
			else if (mnemoic == "BYTE")
			{
				t_code << operand_str << spaces;
				// t_len = 30;
			}
			// T break
			if (t_break)
			{
				break_t_code << break_operand_str << spaces;
			}
			// T Record End
			if (next_next_loc - t_start > 0x1e ||
					next_mnemoic == "RESW" ||
					next_mnemoic == "RESB")
			{
				// Output T Record
				if (mnemoic != "RESW" && mnemoic != "RESB")
					cout << "T" << spaces_s
							 << right << setfill('0') << setw(6)
							 << hex << t_start << spaces_s
							 << right << setfill('0') << setw(2)
							 << t_len << spaces
							 << t_code.str()
							 << endl;
				// Initial Next T Record
				t_start = next_loc;
				t_code.str("");
			}
			// BYTE C length > 30
			if (t_break == true)
			{
				cout << "T" << spaces_s
						 << right << setfill('0') << setw(6)
						 << hex << break_t_start << spaces_s
						 << right << setfill('0') << setw(2)
						 << break_t_len << spaces
						 << break_t_code.str()
						 << endl;
				t_break = false;
			}
		}
		// Secon Last Lines
		else if (line_number == total_lines - 1)
		{
			int next_loc = stoi(instruction.at(i + per_line), 0, 16);
			t_len = next_loc - t_start;
			// Append T Record
			if (op_code != -1)
				t_code << right << setfill('0') << setw(2) << hex << op_code
							 << right << setfill('0') << setw(4) << addr_code
							 << spaces;
			else if (mnemoic == "WORD")
				t_code << right << setfill('0') << setw(6)
							 << operand_int << spaces;
			else if (mnemoic == "BYTE")
				t_code << operand_str << spaces;
			// Output T Record
			cout << "T" << spaces_s
					 << right << setfill('0') << setw(6)
					 << hex << t_start << spaces_s
					 << right << setfill('0') << setw(2)
					 << t_len << spaces
					 << t_code.str()
					 << endl;
		}
		// Last Lines
		else if (line_number == total_lines)
		{
			// Output M Record
			for (const auto &loc : m_record)
			{
				cout << "M" << spaces_s
						 << right << setfill('0') << setw(6)
						 << loc << spaces_s << "04" << endl;
			}
			// E Record
			cout << "E" << spaces_s
					 << right << setfill('0') << setw(6)
					 << hex << addr_code
					 << endl;
			cout << "\033[0m\n";
		}
		line_number++;
	}
}

void PassOne()
{
	middle_file.open("middle.txt", ios::out | ios::trunc);
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
	middle_file.close();
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
	{
		cout << "\033[0;31m" << err_message("START") << "\033[0m\n";
		has_error = true;
		return;
	}
	if (instruction.at(instruction_len - 5) != "END")
	{
		cout << "\033[0;31m" << err_message("END") << "\033[0m\n";
		has_error = true;
		return;
	}
	// Compile Instruction
	CompileTwo(instruction_len);
}

// Main
int main(int argc, char *argv[])
{
	// Pass One
	PassOne();
	// Output Symble Table
	// cout << "\033[0;35m"
	// 		 << "\nSymbol Table:"
	// 		 << "\n\n";
	// for (auto iter = symTable.begin(); iter != symTable.end(); iter++)
	// {
	// 	cout << "[\t" << iter->first
	// 			 << "\t->\t" << hex << iter->second
	// 			 << "\t]\n";
	// }
	// cout << "\033[0m\n";
	ifstream middle_file("middle.txt");
	// Pass One Has Error, Output Middle File, Error, and Exit
	if (has_error)
	{
		cout << middle_file.rdbuf();
		exit(1);
	}
	// Pass Two
	PassTwo();
	if (has_error)
	{ // cout << middle_file.rdbuf();
		return (1);
	}
	// End Main
	exit(0);
}
