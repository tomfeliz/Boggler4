#include "stdafx.h"
#include "Cube.h"
#include <atomic>
#include <iostream>

using namespace std;

namespace Boggler
{
	// Must explicitly instantiate template so linker can find it.
	template Cube<TCHAR>;

	template<typename T>
	Cube<T>::Cube(const tstring &cubeData)
	{
		_cubies.reserve(NumCubies);

		// First, populate the Cubie array from the raw data.
		PopulateCube(cubeData);

        // Second, find adjacent Cubies and populate path cache.
        PopulateNeighbors();
	}

#pragma region CountWords

	template<typename T>
	int Cube<T>::CountWords(const Dictionary& dict) const
	{
		auto wordCount = 0;

		for (auto curCubie : _cubies)
		{
			tstring word{ curCubie->GetValue() };
			vector<bool> cubieMap(NumCubies, false);
			wordCount += CountWordsRecursive(curCubie, word, cubieMap, dict);
		}
		return wordCount;
	}

	template<typename T>
	int Cube<T>::CountWordsRecursive(const Cubie<T>* curCubie, const tstring subWord, vector<bool>& cubieMap, const Dictionary& dict) const
	{
		auto wordCount = 0;
		auto match = false;
		auto isWord = false;

		// Search for passed in word/sub-word
		std::tie(match, isWord) = dict.Find(subWord);
		if (!match) return 0; // No match, so we're done.
		if (isWord) wordCount++; // If the word is in dictionary, increment counter.

		// Set visited flag for cubie.
		cubieMap[curCubie->GetCubieNumber()] = true;

		// Iterate through all cubie neighbors.
		for (const auto nextCubie : curCubie->GetNeighbors())
		{
			// Do not visit any previously visited cubies.
			if (cubieMap[nextCubie->GetCubieNumber()])
				continue;

			// Recursive call to next neighbor cubie.
			wordCount += CountWordsRecursive(nextCubie, subWord + nextCubie->GetValue(), cubieMap, dict);
		}

		// Clear visited flag for cubie.
		cubieMap[curCubie->GetCubieNumber()] = false;

		return wordCount;
	}

#pragma endregion

	template<typename T>
	void Cube<T>::PopulateCube(const tstring &rawData)
    {
        for (unsigned int i = 0; i < rawData.length(); i++)
        {
            //auto cubie = make_shared<Cubie<T>>(rawData[i], i);
			auto cubie = new Cubie<T>(rawData[i], i);
            _cubies.emplace_back(cubie);
        }
    }


	template<typename T>
	void Cube<T>::PopulateNeighbors()
    {
        for (auto c = 0; c < NumCubies; c++)
        {
            auto neighbors = GetCubieNeighbors(c);
            _cubies[c]->SetNeighbors(neighbors);
        }
    }

	template<typename T>
	std::vector<Cubie<T>*> Cube<T>::GetCubieNeighbors(int cubieNum) const
    {
		vector<Cubie<T>*> cubieNeighbors;
		cubieNeighbors.reserve(10);
		
        int z = cubieNum / (Dimension * Dimension);
        int y = (cubieNum - z * Dimension * Dimension) / Dimension;
        int x = (cubieNum - z * Dimension * Dimension - y * Dimension);
            
        for (int z1 = z - 1; z1 <= z + 1; z1++)
        {
            if (z1 >= 0 && z1 < Dimension)
            {
                for (int y1 = y - 1; y1 <= y + 1; y1++)
                {
                    if (y1 >= 0 && y1 < Dimension)
                    {
                        for (int x1 = x - 1; x1 <= x + 1; x1++)
                        {
                            if (x1 >= 0 && x1 < Dimension)
                            {
                                int c1 = x1 + y1 * Dimension + z1 * Dimension * Dimension;
                                if (_cubies[c1] != _cubies[cubieNum])
                                {
                                    cubieNeighbors.emplace_back(_cubies[c1]);
                                }
                            }
                        }
                    }
                }
            }
        }

        return cubieNeighbors;
    }

}