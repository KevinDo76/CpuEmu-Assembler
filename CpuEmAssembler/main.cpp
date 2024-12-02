#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "assembler.h"

int main()
{
	std::string assemblyFilePath = "program.casm";
	std::string binaryOutputFilePath = "program.bin";
	std::vector<std::string> AsmStringBuff;
	std::vector<std::pair<unsigned int, std::string>> AsmCleanStringBuff;

	lexer::readFile(assemblyFilePath, AsmStringBuff);
	lexer::sanitizeAssembly(AsmStringBuff, AsmCleanStringBuff);

	std::vector<token> tokenList;

	for (int i = 0; i < AsmCleanStringBuff.size(); i++)
	{
		std::string error;
		if (!lexer::lexcialAnalyzer(tokenList, AsmCleanStringBuff[i], error))
		{
			std::cout << error << "\n";
			std::cout << "Failed to tokenize, exiting\n";
			return 1;
		}
	}

	syntax::Assemble(tokenList, binaryOutputFilePath);

	return 0;
}