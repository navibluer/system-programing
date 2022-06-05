#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "opTable.h"
using namespace std;

map<string, string> compile(string input, int line_number)
{
	int flag = 0;	 // meet char?
	int count = 0; // word count
	string tmp = "";
	vector<string> instruction;

	for (int i = 0; i < input.length(); i++)
	{
		// Ignore Comment
		if (input[i] == '.' || input[i] == '\n')
		{
			if (flag == 1)
			{
				instruction.push_back(tmp);
				tmp = "";
				flag = 0;
			}
			break;
		}

		// Ignore Witespace
		else if (input[i] == ' ' || input[i] == '\t')
		{
			if (flag == 1)
			{
				instruction.push_back(tmp);
				tmp = "";
				flag = 0;
			}
			continue;
		}

		// Store Char
		else
		{
			// if previous is space, count ++
			if (flag == 0)
				count++;

			flag = 1;
			tmp += input[i];
		}
	}

	// Store Last Word
	if (flag == 1)
	{
		instruction.push_back(tmp);
	}

	// Statement
	map<string, string> statement;
	statement["line_number"] = to_string(line_number);
	statement["Label"] = "";
	statement["Mnemoic"] = "";
	statement["Operand"] = "";
	statement["Addressing"] = "";

	if (instruction.size() > 0)
	{

			// if have no label
		if (opcode(instruction.front()) != -1)
		{
			instruction.insert(instruction.begin(), " ");
		}

		for (int i = 0; i < instruction.size(); i++)
		{
			switch (i)
			{
			case 0:
				if (instruction.at(i) != " ")
					statement["Label"] = instruction.at(i);
				break;
			case 1:
				statement["Mnemoic"] = instruction.at(i);
				break;
			case 2:
				statement["Operand"] = instruction.at(i);
				break;
			}
		}
	}

	// Addressing mode
	if (instruction.size() >= 4) // FIXME
	// if (instruction.size() > 3 && instruction.back() == "X")
	{
		statement["Addressing"] = "Index";
		statement["Operand"] += "X";
	}
	else if (instruction.size() >= 3)
	{
		statement["Addressing"] = "Direct";
	}

	return statement;
}