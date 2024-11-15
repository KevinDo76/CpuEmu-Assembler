#pragma once
#include <stdint.h>
class token
{
public:
	enum tokenType {
		instruction,
		label,
		oprand,
		stringChunk,
	};

	enum dataType {
		hex,
		integer,
		string,
	};

	tokenType type;
	dataType dataT;
	std::string stringData;
	uint32_t intData;
	uint32_t lineNumber;

	token();
};



namespace lexer {
	void lexcialAnalyzer(std::vector<token>& tokenList, std::pair<unsigned int, std::string>line);
	bool convertToken(unsigned int lineNumber, unsigned int wordIndex, bool inTextChunk, std::string word, token& returnToken);
	void santizeHex(std::string& word);
	char hex2char(char n);
	uint32_t hex2int(std::string n);
}

