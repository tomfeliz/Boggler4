#pragma once
#include "stdafx.h"
#include <map>
#include <set>
#include <string>
#include <vector>
#include <memory>

typedef std::basic_string<TCHAR> tstring;

namespace Boggler
{
	struct TrieNode
	{
		TrieNode() : Letter(static_cast<TCHAR>(0)), IsWord(false) {}

		TrieNode(TCHAR c, bool b) : Letter(c), IsWord(b) {}
		//{
		//	children = std::make_shared<std::map<TCHAR, TrieNode>>();
		//}

		TCHAR Letter;
		bool IsWord;
		std::shared_ptr<std::map<TCHAR, TrieNode>> children;
	};

	class Dictionary
	{
	public:
		bool LoadWordList(const tstring &wordFileName);
		size_t GetWordCount() const;

		std::vector<tstring> WordList;
		std::set<TCHAR> FirstLetters;
	private:
		void MapWord(tstring word);

		TrieNode root;
	};
}