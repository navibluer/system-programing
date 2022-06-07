#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
using namespace std;

// Main
int main(int argc, char *argv[])
{
	string input;
	vector<string> instruction;
	string tmp;
	while (cin)
	{
		getline(cin, input);
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
		tmp = "";
	}

	for (size_t i = 0; i < instruction.size(); i++)
	{
		cout << instruction.at(i) << "\t";
		if (i % 6 == 5)
			cout << endl;
	}
	cout << endl;
	
	int program_len = instruction.size() / 6;
	cout << "len: " << program_len << endl;
	// End Main
	return 0;
}


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