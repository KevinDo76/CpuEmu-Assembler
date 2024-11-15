#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "token.h"

void removeComment(std::string& toRemove)
{
	std::string tmp = "";
	for (int i = 0; i < toRemove.size(); i++)
	{
		if (toRemove[i] == '#')
		{
			break;
		}
		tmp += toRemove[i];
	}
	toRemove = tmp;
}

void trimEndString(std::string& toTrim)
{
	bool spaceTrimming = true;
	std::string tmp = "";
	for (int i = toTrim.size()-1; i >=0 ; i--)
	{
		if (toTrim[i] != ' ')
		{
			spaceTrimming = false;
		}
		if (spaceTrimming)
		{
			continue;
		}

		tmp = toTrim[i] + tmp;
	}
	toTrim = tmp;
}

void trimBeginString(std::string& toTrim)
{
	bool spaceTrimming = true;
	std::string tmp = "";
	for (int i = 0 ; i < toTrim.size(); i++)
	{
		if (toTrim[i] != ' ')
		{
			spaceTrimming = false;
		}
		if (spaceTrimming)
		{
			continue;
		}

		tmp += toTrim[i];
	}
	toTrim = tmp;
}

void sanitizeAssembly(std::vector<std::string>& originalVec, std::vector<std::pair<unsigned int, std::string>>& returnVec)
{
	for (int i = 0; i < originalVec.size(); i++)
	{

		std::cout << originalVec[i] << "\n";

		std::string sanitizedString = originalVec[i];
		removeComment(sanitizedString);
		trimEndString(sanitizedString);
		trimBeginString(sanitizedString);
		if (sanitizedString.size() == 0)
		{
			continue;
		}
		returnVec.push_back(std::pair<unsigned int, std::string>(i + 1, sanitizedString));
	}

}

bool readFile(std::string assemblyFilePath, std::vector<std::string>& stringVec)
{
	std::string stringBuff;
	std::ifstream AsmFile(assemblyFilePath);

	if (!AsmFile.is_open())
	{
		return false;
	}

	while (std::getline(AsmFile, stringBuff)) {
		// Output the text from the file
		stringVec.push_back(stringBuff);
	}

	// Close the file
	AsmFile.close();
	return true;
}

int main()
{
	std::string assemblyFilePath = "program.casm";
	std::string binaryOutputFilePath = "program.bin";
	std::vector<std::string> AsmStringBuff;
	std::vector<std::pair<unsigned int, std::string>> AsmCleanStringBuff;

	readFile(assemblyFilePath, AsmStringBuff);
	sanitizeAssembly(AsmStringBuff, AsmCleanStringBuff);

	std::vector<token> tokenList;

	Lexical::lexcialAnalyzer(tokenList, AsmCleanStringBuff[0]);

	return 0;
}