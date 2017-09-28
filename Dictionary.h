#pragma once
#include "stdafx.h"
#include <map>
#include <vector>
#include <memory>

typedef std::basic_string<TCHAR> tstring;

namespace Boggler
{
	struct TrieNode
	{
		TrieNode() : Letter(static_cast<TCHAR>(0)), IsWord(false) {}
		TrieNode(TCHAR c, bool b) : Letter(c), IsWord(b) {}

		TCHAR Letter;
		bool IsWord;
		std::shared_ptr<std::map<TCHAR, TrieNode>> children;
	};

	class Dictionary
	{
		public:
			bool LoadWordList(const tstring &wordFileName);
			std::pair<bool /* letters match */, bool /* Is Word */> Find(const tstring stringToFind) const;
			size_t GetWordCount() const;

		private:
			void MapWord(tstring word);

			std::vector<tstring> WordList;
			TrieNode root;
	};
}