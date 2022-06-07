#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
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
	// Compile Instruction
	try
	{
		// Check Start, End
		if (instruction.at(2) != "START")
			throw "Program do not start from 'START'.";
		if (instruction.at(instruction.size() - 5) != "END")
			throw "Program do not end at 'END'.";
		compile_two();
	}
	catch (const char *err_massage)
	{
		cout << "\033[0;31m" << err_massage << "\033[0m\n";
	}

	// Output
	for (size_t i = 0; i < instruction.size(); i++)
	{
		cout << instruction.at(i) << "\t";
		if (i % 6 == 5)
			cout << endl;
	}
	cout << endl;
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