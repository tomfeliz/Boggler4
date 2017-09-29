#pragma once
#pragma warning(disable:4503)

#include <unordered_set>
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
			explicit Cube(const tstring &rawData);
			~Cube() { }
			int FindWords(const Dictionary& dict) const;

		private:
			static const int Dimension = 4;
			static const int NumCubies = Dimension * Dimension * Dimension;
			static const int PrefixLength = 2;
			std::vector<Cubie<T>*> _cubies;
			
			void FindWordsRecursive(const Cubie<T>* curCubie, const tstring subWord, std::vector<bool>& cubieMap,
				const Dictionary& dict, std::unordered_set<tstring>& wordsFound) const;
			void PopulateCube(const tstring &rawData);
			void PopulateNeighbors();
			std::vector<Cubie<T>*> GetCubieNeighbors(int cubieNum) const;
	};
}
