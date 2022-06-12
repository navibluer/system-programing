#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "opTable.h"
#include "errTable.h"
using namespace std;

map<string, string> compile(string input)
{
	int flag = 0;	 // meet char?
	int count = 0; // word count
	string tmp = "";
	vector<string> instruction;
	for (int i = 0; i < input.length(); i++)
	{
		// Ignore Comment, \r for fucking windows
		if (input[i] == '.' || input[i] == '\n' || input[i] == '\r')
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
		else
		{
			// Previous is Space
			if (flag == 0)
				count++;
			// Store Char
			flag = 1;
			tmp += input[i];
		}
	}

	// Store Last Word
	if (flag == 1)
		instruction.push_back(tmp);

	// Statement
	map<string, string> statement;
	string *label = &statement["Label"];
	string *mnemoic = &statement["Mnemoic"];
	string *operand = &statement["Operand"];
	string *addressing = &statement["Addressing"];

	if (instruction.size() > 0)
	{
		string *front = &instruction.front();
		// No label
		if (opcode(*front) != -1 || *front == "END")
			instruction.insert(instruction.begin(), "***");
		// Assign Label, Mnemoic, Operand
		for (int i = 0; i < instruction.size(); i++)
		{
			switch (i)
			{
			case 0:
				if (instruction.at(i) != "***")
					*label = instruction.at(i);
				break;
			case 1:
				*mnemoic = instruction.at(i);
				break;
			case 2:
				*operand = instruction.at(i);
				break;
			default:
				break;
			}
		}
		// if (instruction.at(0) != "***")
		// 	*label = instruction.at(0);
		// *mnemoic = instruction.at(1);
		// *operand = instruction.at(2);
	}
	// Check Operand, Addressing mode
	if (instruction.size() > 5)
	{
		// BYTE C content has space char
		if (*mnemoic == "BYTE" && statement["Operand"].front() == 'C')
		{
			*operand += instruction[3]; // first '
			// remaining content
			for (size_t i = 4; i < instruction.size(); i++)
			{
				*operand += " " + instruction.at(i);
			}
			*addressing = "Direct";
		}
		else
			throw err_message("invalid_syntax");
	}
	else if (instruction.size() > 3)
	{
		if (instruction[3] == "X") // Operand, X
		{
			*addressing = "Index";
			*operand += "X";
		}
		else if (instruction[3] == ",X") // Operand ,X
		{
			*addressing = "Index";
			*operand += ",X";
		}
		else if (instruction[3] == ",") // Operand , ?
		{
			*addressing = "Index";
			*operand += ",";
			if (instruction.size() >= 5)
				*operand += instruction.at(4);
		}
		else if (*mnemoic == "BYTE")
		{
			*operand += instruction[3]; // BYTE X '?'
			*addressing = "Direct";
		}
		else // Too Long
			throw err_message("invalid_syntax");
	}
	else if (
			instruction.size() > 2 &&
			*mnemoic != "START" &&
			*mnemoic != "END")
	{
		*addressing = "Direct";
	}
	return statement;
}