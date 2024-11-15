#include <string>
#include <stdint.h>
#include <vector>
#include <iostream>
#include "token.h"

token::token(){}

void lexer::lexcialAnalyzer(std::vector<token>& tokenList, std::pair<unsigned int, std::string>line)
{
	std::string currentIngest = "";
	bool textChunk = false;
	bool textChunkEnded = false;
	unsigned int wordIndex = 0;
	for (int i=0;i<line.second.size();i++)
	{
		if (line.second[i] == '"')
		{
			textChunk = !textChunk;
			if (!textChunk)
			{
				textChunkEnded = true;
			}
			continue;
		}
		if (line.second[i] == ' ' && !textChunk)
		{
			token Token;
			std::cout << currentIngest << "\n";
			if (convertToken(line.first, wordIndex, textChunkEnded, currentIngest, Token))
			{
				tokenList.push_back(Token);
			}
			textChunkEnded = false;
			wordIndex++;
			currentIngest = "";
			continue;
		}
		currentIngest += line.second[i];
	}
	std::cout << currentIngest << "\n";
	token Token;
	if (convertToken(line.first, wordIndex, textChunkEnded, currentIngest, Token))
	{
		tokenList.push_back(Token);
	}
}

bool lexer::convertToken(unsigned int lineNumber, unsigned int wordIndex, bool inTextChunk, std::string word, token& returnToken)
{
	returnToken.lineNumber = lineNumber;
	if (word.size() == 0)
	{
		return false;
	}
	if (word[word.size() - 1] == ':')
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
	if (inTextChunk || word[word.size() - 1] == '"')
	{
		returnToken.type = token::tokenType::stringChunk;
		returnToken.dataT = token::dataType::string;
		returnToken.stringData = word;
		return true;
	}
	if (wordIndex == 0)
	{
		returnToken.type = token::tokenType::instruction;
		returnToken.dataT = token::dataType::string;
		returnToken.stringData = word;
		return true;
	}
	returnToken.type = token::tokenType::label;
	returnToken.dataT = token::dataType::string;
	returnToken.stringData = word;
	return true;
}


void lexer::santizeHex(std::string& word)
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



char lexer::hex2char(char n)
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

uint32_t lexer::hex2int(std::string n)
{
	uint32_t number = 0;
	for (int i = 0; i < n.size(); i++)
	{
		number = number << 4 | hex2char(n[i]);
	}
	return number;
}
