#include "stdafx.h"
#define _ITERATOR_DEBUG_LEVEL 0

#include <fstream>
#include <io.h>
#include <iostream>
#include <memory>
#include <ppl.h>
#include <regex>
#include <thread>
#include <string>
#include <vector>

#include "Cube.h"
#include "Dictionary.h"
#include "parallel.h"

using namespace std;
using namespace Boggler;

typedef basic_string<TCHAR> tstring;
typedef basic_regex<TCHAR> tregex;
typedef basic_ifstream<TCHAR> tifstream;

bool LoadCubes(const tstring &cubeFileName);

vector<shared_ptr<Cube<TCHAR>>> CubeList;

int _tmain(int argc, _TCHAR* argv[])
{
	// Preallocate cubes and words vectors.
	CubeList.reserve(1000);

#pragma region Check command line arguments.

	auto abort = false;

	// Make sure the correct arguments are passed into program.
	if (argc != 3)
	{
		cout << "*** Invalid number of command line arguments." << endl;
		abort = true;
	}

	if ((abort == false) && ((_taccess(argv[1], 0) != 0) | (_taccess(argv[2], 0) != 0)))
	{
		cout << "*** One or both files were not found." << endl;
		abort = true;
	}

	if (abort)
	{
		cout << "Correct Usage: Boggler cube-filename word-filename" << endl;
		return 0;
	}

#pragma endregion

	const tstring cubeFileName = argv[1];
	const tstring wordFileName = argv[2];

	// Set global start clock
	const auto start0 = clock();

	// Load the word file.
	const auto start1 = clock();
	Dictionary dict;
	dict.LoadWordList(wordFileName);
	const auto finish1 = clock();
	cout << "LoadWordList: Loaded " << dict.GetWordCount() << " words in " 
		<< static_cast<float>(finish1 - start1) / CLOCKS_PER_SEC << " seconds." << endl;

	// Load the cube file.
	const auto start2 = clock();
	LoadCubes(cubeFileName);
	const auto finish2 = clock();
	cout << "LoadCubes: Loaded " << CubeList.size() << " cubes in " 
		<< static_cast<float>(finish2 - start2) / CLOCKS_PER_SEC << " seconds." << endl;

	const int numBlocks = thread::hardware_concurrency();; // number of threads
	cout << "Using " << numBlocks << " hardware threads." << endl;
	map<int, pair<int, float>> results;
	atomic<int> cubeNum = 0;
	//mutex cons;

	parallel_for_each(begin(CubeList), end(CubeList), [&dict, &results, &cubeNum](shared_ptr<Cube<TCHAR>>& cube)
	{
		auto count = 0;
		auto start3 = clock();

		cube->CountWords(dict);
		//for (auto word : dict.WordList)
		//{
		//	if (cube->FindWord(word))
		//	{
		//		++count;
		//	}
		//}

		auto finish3 = clock();
		results[cubeNum++] = make_pair(count, static_cast<float>(finish3 - start3) * 1000 / CLOCKS_PER_SEC);
	});

	for (unsigned int i = 0; i < results.size(); i++)
	{
		cout << "Cube " << (i + 1) << ": " << results[i].first << " words ("
			<< results[i].second << " ms)" << endl;
	}

	auto finish0 = clock();
    cout << "Total execution time: " << static_cast<float>(finish0 - start0) / CLOCKS_PER_SEC << " seconds" << endl;

	string temp;
	cout << "Press enter to terminate program..." << endl;
	getline(cin, temp);
	return 0;
}

bool LoadCubes(const tstring &cubeFileName)
{
	tifstream cubeFile(cubeFileName);
	tstring currentLine;

	while (cubeFile.good())
	{
		getline(cubeFile, currentLine);
		if (currentLine.length() > 0)
		{
			auto cube = make_shared<Cube<TCHAR>>(currentLine);
			CubeList.emplace_back(cube);
		}
	}
	cubeFile.close();

	// Calculate cube path cache.
	parallel_for_each(begin(CubeList), end(CubeList), [](shared_ptr<Cube<TCHAR>> cube)
	{
		cube->PopulatePathCache();
	});

	return true;
}
