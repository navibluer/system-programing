#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include "errTable.h"
using namespace std;

int line_number = 0;
// int inst_amount;
vector<string> instruction;
map<string, int> symTable;

// Store Symbol
void store_symbol(string symbol, int loc)
{
	if (symTable.count(symbol) != 0)
		cout << "Redefined symbol: " + symbol;
	else
		symTable[symbol] = loc;
}

void compile_two()
{
	for (size_t i = 0; i < instruction.size() - 1; i++)
	{
		/* code */
	}
	
}

// Main
int main(int argc, char *argv[])
{
	string input;
	while (cin)
	{
		getline(cin, input);
		string tmp = "";
		line_number++;
		for (size_t i = 0; i <= input.length(); i++)
		{
			if (input[i] == '\t' || i == input.length())
			{
				instruction.push_back(tmp);
				tmp = "";
			}
			else
				tmp += input[i];
		}
	}

	// Output
	for (size_t i = 0; i < instruction.size(); i++)
	{
		cout << instruction.at(i) << "\t";
		if (i % 6 == 5)
			cout << endl;
	}
	cout << endl;
	// Check Start, End
	if (instruction.at(2) != "START")
		cout << "\033[0;31m" << err_message("START") << "\033[0m\n";
	if (instruction.at(instruction.size() - 5) != "END")
		cout << "\033[0;31m" << err_message("END") << "\033[0m\n";
	// Compile Instruction
	compile_two();
	// End Main
	return 0;
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