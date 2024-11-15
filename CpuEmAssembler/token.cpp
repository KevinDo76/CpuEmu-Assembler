#include <string>
#include <stdint.h>
#include <vector>
#include <iostream>
#include "token.h"

token::token(){}



void Lexical::lexcialAnalyzer(std::vector<token>& tokenList, std::pair<unsigned int, std::string>line)
{
	std::string currentIngest = "";
	for (int i=0;i<line.second.size();i++)
	{
		if (line.second[i] == ' ')
		{
			token Token;
			std::cout << currentIngest << "\n";
			if (convertToken(line.first, currentIngest, Token)) 
			{
				tokenList.push_back(Token);
			}
			currentIngest = "";
			continue;
		}
		currentIngest += line.second[i];
	}
	std::cout << currentIngest << "\n";
	token Token;
	if (convertToken(line.first, currentIngest, Token))
	{
		tokenList.push_back(Token);
	}
}

bool Lexical::convertToken(unsigned int lineNumber, std::string word, token& returnToken)
{
	returnToken.lineNumber = lineNumber;
	if (word.size() == 0)
	{
		return false;
	}
	if (word[word.size() - 2] == ':')
	{
		returnToken.type = token::tokenType::label;
		returnToken.dataT = token::dataType::string;
		returnToken.stringData = word;
		return true;
	}
	if (word[0] == '0' && word[1] == 'x')
	{
		returnToken.type = token::tokenType::oprand;
		returnToken.dataT = token::dataType::integer;
		santizeHex(word);
		returnToken.intData = hex2int(word);
		return true;
	}
	returnToken.type = token::tokenType::instruction;
	returnToken.dataT = token::dataType::string;
	returnToken.stringData = word;
	return true;
}


void Lexical::santizeHex(std::string& word)
{
	std::string output = "";
	bool inZeroPadding = true;
	for (int i = 0; i < word.size(); i++)
	{
		if (word[i] == 'x')
		{
			continue;
		}
		if (word[i] == '0' && inZeroPadding)
		{
			continue;
		}
		else
		{
			inZeroPadding = false;
		}
		unsigned char nibble = (unsigned char)word[i];
		if (nibble > 70)
		{
			nibble -= 32;
		}

		if ((nibble >= 0x30 &&
			nibble <= 0x39) ||
			(nibble >= 0x41 &&
				nibble <= 0x46))
		{
			output += nibble;
		}
		else
		{
			std::cout << "INVALID HEX CHARACTER\n";
			throw;
		}
	}
	word = output;
}



char Lexical::hex2char(char n)
{
	n = (n > 70) ? n - 32 : n;

	if (n >= '0' && n <= '9')
	{
		return n - '0';
	}
	else if (n >= 'A' && n <= 'F')
	{
		return n - 'A' + 10;
	}
}

uint32_t Lexical::hex2int(std::string n)
{
	uint32_t number = 0;
	for (int i = 0; i < n.size(); i++)
	{
		number = number << 4 | hex2char(n[i]);
	}
	return number;
}
