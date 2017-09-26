#include  "stdafx.h"
#include <fstream>
#include <vector>
#include <string>
#include <regex>
#include <set>
#include "Dictionary.h"

using namespace Boggler;

typedef std::basic_string<TCHAR> tstring;
typedef std::basic_regex<TCHAR> tregex;
typedef std::basic_ifstream<TCHAR> tifstream;

bool Dictionary::LoadWordList(const tstring &wordFileName)
{
	WordList.reserve(25000);
	tregex alphaRegex(_T("^\\s*[a-zA-Z]+\\s*$"));
	tifstream wordFile(wordFileName);
	tstring currentLine;

	while (wordFile.good())
	{
		std::getline(wordFile, currentLine);

		// Skip empty lines and match on the alpha-numeric regex.
		if (std::regex_match(currentLine, alphaRegex))
		{
			WordList.emplace_back(currentLine);
			MapWord(currentLine);
		}
	}
	wordFile.close();

	return true;
}

void Dictionary::MapWord(tstring word)
{
	// Node 'root' is the top of the hierarchy.
	auto *current = &root;

	// Iterate through all letters in word.
	for (auto c : word)
	{
		// Instantiate child map, if needed.
		if (current->children == nullptr)
		{
			current->children = std::make_shared<std::map<TCHAR, TrieNode>>();
		}

		// Add a new child TrieNode, if needed.
		if (current->children->find(c) == current->children->end())
		{
			TrieNode child;
			(*current->children)[c] = child; //copy
		}
		
		// Set current node pointer to matching child node.
		current = &(*current->children)[c];
	}

	// Assume we have a complete word at this point.
	current->IsWord = true;
}

std::pair<bool /* letters match */, bool /* Is Word */> Dictionary::Find(tstring stringToFind)
{
	auto current = &root;

	for (auto c : stringToFind)
	{
		// No children left to match letters.
		if (current->children == nullptr || current->children->size() == 0)
			return{ false, false };

		// Next letter doesn't match any children.
		if (current->children->find(c) == current->children->end())
		{
			return{ false, false };
		}
		
		// Set current node pointer to matching child node.
		current = &(*current->children)[c];
	}

	return{ true, current->IsWord };
}

size_t Dictionary::GetWordCount() const
{
	return WordList.size();
}
