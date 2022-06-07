#include <iostream>
#include <iomanip>
#include <string>
using namespace std;



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