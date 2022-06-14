#include "opTable.h"
#include "errTable.h"

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
		if ((instruction.size() <= 2 &&
				 (opcode(*front) != -1 || *front == "END")))
		{
			instruction.insert(instruction.begin(), "***");
		}
		else if (instruction.size() == 3 &&
						 instruction[2].find(',') != string::npos || 
						 instruction[1].find(',') != string::npos)
			instruction.insert(instruction.begin(), "***");
		else if (instruction.size() == 4 &&
						 instruction[2].find(',') != string::npos)
			instruction.insert(instruction.begin(), "***");
		// Assign Label, Mnemoic, Operand
		if (instruction.at(0) != "***")
			*label = instruction.at(0);
		*mnemoic = instruction.at(1);
		if (instruction.size() > 2)
			*operand = instruction.at(2);
	}
	// Check Operand, Addressing mode
	if (instruction.size() > 5)
		throw err_message("invalid_syntax");
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
		else if (instruction[3].find(',') != string::npos) // Operand , ?
		{
			*addressing = "Index";
			*operand += instruction[3];
			if (instruction.size() == 5)
				*operand += instruction.at(4);
		}
		else if (*mnemoic == "BYTE")
		{
			*operand += instruction[3]; // BYTE X '?'
			*addressing = "Direct";
			if (instruction.size() == 5)
				throw err_message("invalid_syntax");
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
