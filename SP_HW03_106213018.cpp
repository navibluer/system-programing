#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include "complie.h"
#include "symbolTable.h"

using namespace std;

// void store_symbol(map<string, int> symTable, map<int, string> error_log, string data_name, int line_number, int loc)
// {
// 	if (symTable.count(data_name) != 0)
// 	{
// 		// Set Error
// 		error_log[line_number] = "Duplicate Lable.";
// 	}
// 	else
// 	{
// 		symTable[data_name] = loc;
// 	}
// }

int main(int argc, char *argv[])
{
	bool is_start = false;
	string input_line;
	int line_number = 1;
	int loc = 0;
	map<string, int> symTable;
	map<int, string> error_log;

	while (cin)
	{
		getline(cin, input_line);
		cout << to_string(line_number) << ":";
		cout << "\033[4;37m" << input_line << "\033[0m\n";

		// START
		if (input_line.find("START") != string::npos)
		{
			is_start = true;
		}

		// compile and ingnore empty line
		if (is_start && input_line.length() > 1)
		{
			// [ Label, Mnemoic, Operand ]
			map<string, string> statement = compile(input_line, line_number);

			cout << "\033[3;35m"
						<< "  line: " << statement["line_number"]
						<< "\tloc: " << hex << loc
						<< "\033[0m" << endl;

			// END Program
			if (statement["Label"] == "END")
			{
				is_start = false;
				cout << "\033[3;34m"
						 << "  End of program."
						 << "\033[0m\n\n";
				break;
			}
			// START from this line
			else if (statement["Mnemoic"] == "START")
			{
				// convert hex string to int, set start position
				loc = stoi(statement["Operand"], 0, 16);
				cout << "\033[3;34m"
						 << "  Program name is " << statement["Label"] << ".\n"
						 << "  Start from " << statement["Operand"] << "."
						 << "\033[0m\n\n";
			}
			// RESB
			else if (statement["Mnemoic"] == "RESB")
			{
				cout << "\033[3;34m"
						 << "  RESB is pesudo instruction code."
						 << "\033[0m\n\n";

				// Store data in Symbol Table
				if (!statement["Label"].empty())
				{
					string data_name = statement["Label"];
					if (symTable.count(data_name) != 0)
						error_log[line_number] = "Duplicate Lable."; // Set Error
					else
						symTable[data_name] = loc;
				}

				int byte = stoi(statement["Operand"]);
				// cout << "byte: " <<  byte;
				loc += byte;
			}
			// RESW
			else if (statement["Mnemoic"] == "RESW")
			{
				cout << "\033[3;34m"
						 << "  RESW is pesudo instruction code."
						 << "\033[0m\n\n";

				// Store data in Symbol Table
				if (!statement["Label"].empty())
				{
					string data_name = statement["Label"];
					if (symTable.count(data_name) != 0)
						error_log[line_number] = "Duplicate Lable."; // Set Error
					else
						symTable[data_name] = loc;
				}

				int word = stoi(statement["Operand"]);
				loc += word * 3;
			}
			// Other
			else
			{
				// Output Addressing Mode
				if (!statement["Addressing"].empty())
				{
					cout << "\033[3;32m"
							 << "  Addressing: "
							 << "\033[0m";
					cout << "\033[0;33m" << statement["Addressing"] << "\033[0m\n";
				}

				// Output Label, Mnemoic, Operand
				if (!(statement["Label"].empty() && statement["Mnemoic"].empty() && statement["Operand"].empty()))
				{
					cout << "\033[3;32m"
							 << "  Label: "
							 << "\033[0m";
					cout << "\033[0;33m" << statement["Label"] << "\033[0m";
					cout << "\033[3;32m"
							 << "\tMnemoic: "
							 << "\033[0m";
					cout << "\033[0;33m" << statement["Mnemoic"] << "\033[0m";
					cout << "\033[3;32m"
							 << "\tOperand: "
							 << "\033[0m";
					cout << "\033[0;33m" << statement["Operand"] << "\033[0m\n\n";

					// Store data in Symbol Table
					if (!statement["Label"].empty())
					{
						string data_name = statement["Label"];
						if (symTable.count(data_name) != 0)
							error_log[line_number] = "Duplicate Lable."; // Set Error
						else
							symTable[data_name] = loc;
					}

					loc += 3;
				}
			}
		}

		line_number++;
	}

	// Output Symble Table
	cout << "\033[3;36m"
			 << "  Symbol Table: " << endl;

	auto iter_s = symTable.begin();
	while (iter_s != symTable.end())
	{
		cout << "  [\t" << iter_s->first << ":\t"
				 << iter_s->second << hex << "\t]\n";
		iter_s++;
	}
	cout << "\033[0m\n\n";

	// Output Error log
	cout << "\033[3;31m"
			 << "  Error log: " << endl;

	auto iter_e = error_log.begin();
	while (iter_e != error_log.end())
	{
		cout << "  [\t" << to_string(iter_e->first) << ":\t"
				 << iter_e->second << "\t]\n";
		iter_e++;
	}
	cout << "\033[0m\n\n";

	return 0;
}