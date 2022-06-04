#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "opcodeTable.h"
using namespace std;

map<string, string> compile(string input, string line_number)
{
	int flag = 0;	 // meet char?
	int count = 0; // word count
	string tmp = "";
	vector<string> code;

	for (int i = 0; i < input.length(); i++)
	{
		// ignore comment
		if (input[i] == '.' || input[i] == '\n')
		{
			if (flag == 1)
			{
				code.push_back(tmp);
				flag = 0;
			}
			break;
		}

		// ignore witespace
		else if (input[i] == ' ' || input[i] == '\t')
		{
			if (flag == 1)
			{
				code.push_back(tmp);
				tmp = "";
				flag = 0;
			}
			continue;
		}

		// store char
		else
		{
			// if previous is space, count ++
			if (flag == 0)
				count++; flag = 1;

			tmp += input[i];
		}
	}

	if (flag == 1)
	{
		code.push_back(tmp);
	}

	// statement
	map<string, string> statement;
	statement["line_number"] = line_number;
	statement["Label"] = "";
	statement["Mnemoic"] = "";
	statement["Operand"] = "";
	statement["Addressing"] = "";

	if (code.size() > 0)
	{

		// if have no label
		if (opcode(code.front()) != -1)
		{
			code.insert(code.begin(), " ");
		}

		for (int i = 0; i < code.size(); i++)
		{
			switch (i)
			{
			case 0:
				if (code.at(i) != " ")
					statement["Label"] = code.at(i);
				break;
			case 1:
				statement["Mnemoic"] = code.at(i);
				break;
			case 2:
				statement["Operand"] = code.at(i);
				break;
			}
		}
	}

	// Addressing mode
	if (code.size() >= 4) // FIXME
	// if (code.size() > 3 && code.back() == "X")
	{
		statement["Addressing"] = "Index";
		statement["Operand"] += "X";
	}
	else if (code.size() >= 3)
	{
		statement["Addressing"] = "Direct";
	}

	return statement;
}
