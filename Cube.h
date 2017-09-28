#pragma once
#pragma warning(disable:4503)

//#include "stdafx.h"
#include <array>
#include <deque>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Cubie.h"
#include "Dictionary.h"

typedef std::basic_string<TCHAR> tstring;

namespace Boggler
{
	template<typename T>
	class Cube
	{
		public:
			Cube() { }
			Cube(const tstring &rawData);
			~Cube() { }
			bool FindWord(const tstring &word);
			int CountWords(Dictionary& dict);
			int CountWordsRecursive(const Cubie<T>* curCubie, tstring subWord, Dictionary& dict);
			void PopulatePathCache();

		private:
			static const int Dimension = 4;
			static const int NumCubies = Dimension * Dimension * Dimension;
			static const int PrefixLength = 2;

			std::vector<Cubie<T>*> _cubies;
			std::map<tstring, std::vector<std::vector<Cubie<T>*>>> _pathCache;

			bool FindWordRecursive(const std::vector<Cubie<T>*> &fromPath, tstring subWord, std::vector<bool> &cubieMap);
			void PopulateCube(const tstring &rawData);
			void PopulateNeighbors();
			std::vector<Cubie<T>*> GetCubieNeighbors(int cubieNum);
			void AddPathCacheEntry(const tstring &pattern, std::vector<Cubie<T>*> &cubiePath);
	};
}
