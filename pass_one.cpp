#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <map>
// #include <vector>
// #include "opTable.h"
#include "complie.h"
#include "symbolTable.h"
using namespace std;

int line_number = 1;
int loc = 0;
map<string, int> symTable;
map<int, string> error_log;

void store_symbol(map<string, string> statement)
{
	if (!statement["Label"].empty())
	{
		string data_name = statement["Label"];
		if (symTable.count(data_name) != 0)
			error_log[line_number] = "Duplicate Lable '" + data_name + "'.";
		else
			symTable[data_name] = loc;
	}
}

int main(int argc, char *argv[])
{
	bool is_start = false;
	string input_line;

	while (cin)
	{
		getline(cin, input_line);

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

			// Output Label, Mnemoic, Operand
			if (
				!( statement["Label"].empty()
				&& statement["Mnemoic"].empty()
				&& statement["Operand"].empty()
				))
			{
				cout << "\033[0;35m" << hex << loc << "\t\033[0m";
				cout << "\033[0;36m" << statement["Label"] << "\t\033[0m";
				cout << "\033[0;36m" << statement["Mnemoic"] << "\t\033[0m";
				cout << "\033[0;36m" << setfill(' ') << setw(10) << left
						 								 << statement["Operand"] << "\t\033[0m";

				// Output Addressing Mode
				if (!statement["Addressing"].empty())
				{
					cout << "\033[0;32m" << statement["Addressing"] << "\033[0m";
				}
				cout << "\n";
			}

			// START from this line
			if (statement["Mnemoic"] == "START")
			{
				// convert hex string to int, set start position
				loc = stoi(statement["Operand"], 0, 16);
			}
			// RESB
			else if (statement["Mnemoic"] == "RESB")
			{
				// Store data in Symbol Table
				store_symbol(statement);

				int byte = stoi(statement["Operand"]);
				// cout << "byte: " <<  byte;
				loc += byte;
			}
			// RESW
			else if (statement["Mnemoic"] == "RESW")
			{
				// Store data in Symbol Table
				store_symbol(statement);

				int word = stoi(statement["Operand"]);
				loc += word * 3;
			}
			// END Program
			else if (statement["Label"] == "END")
			{
				is_start = false;
				break;
			}
			else if (
				!( statement["Label"].empty()
				&& statement["Mnemoic"].empty()
				&& statement["Operand"].empty()
				))
			{
				// Store data in Symbol Table
				store_symbol(statement);
				loc += 3;
			}
		}

		line_number++;
	}

	// Output Symble Table
	cout << "\033[0;37m"
			 << "\nSymbol Table: " << endl;

	auto iter_s = symTable.begin();
	while (iter_s != symTable.end())
	{
		cout << "[ " << iter_s->first << ":  \t"
				 << iter_s->second << hex << " ]\n";
		iter_s++;
	}
	cout << "\033[0m\n";

	// Output Error log
	cout << "\033[0;31m"
			 << "Error log: " << endl;

	auto iter_e = error_log.begin();
	while (iter_e != error_log.end())
	{
		cout << "[ " << to_string(iter_e->first) << ": "
				 << iter_e->second << " ]\n";
		iter_e++;
	}
	cout << "\033[0m\n\n";

	return 0;
}
