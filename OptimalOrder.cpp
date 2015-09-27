/*
Copyright (c) 2015, Philip Waldron
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <iostream>
#include <set>
#include <string>
#include <map>
#include <fstream>
#include <algorithm>
#include <list>
#include <iterator>

using namespace std;

void ReadDict(std::multimap<std::string, std::string> &dict)
{
	std::ifstream dictfile("OWL3.txt");

	// Read the dictionary, put it in a map indexed by alphagram
    while (dictfile.good()) {
            std::string word, alphagram;

            dictfile >> word;

            alphagram = word;
            std::sort(alphagram.begin(), alphagram.end());

            dict.insert(std::pair<std::string, std::string>(alphagram, word));
    }

    dictfile.close();
}

void BuildStemLists(multimap<std::string, std::string> &dict, map<string, set<string> > &wordLists)
{
	std::ifstream stemfile("6LetterStem.txt");

	// Read the dictionary, put it in a map indexed by alphagram
    while (stemfile.good()) {
        std::string stem, alphagram;

		stemfile >> stem;

        for (char ch1 = 'A'; ch1 <= 'Z'; ch1++) {
            alphagram = stem + ch1;
            std::sort(alphagram.begin(), alphagram.end());

            for (char ch2 = ch1; ch2 <= 'Z'; ch2++) {
                alphagram = stem + ch1 + ch2;
                std::sort(alphagram.begin(), alphagram.end());

				std::pair <std::multimap<string, string>::iterator, std::multimap<string, string>::iterator> ret = dict.equal_range(alphagram);
				for (multimap<std::string, std::string>::iterator It = ret.first; It != ret.second; ++It) {
						wordLists[stem].insert(It->second);
				}
            }
        }
	}

    stemfile.close();

}

int main(int argc, char* argv[])
{
    std::multimap<std::string, std::string> dict;
	map<string, set<string> > wordLists;
	set<string> wordsSoFar;
	map<int, string> overlap;

	ReadDict(dict);
	BuildStemLists(dict, wordLists);

	do {
		overlap.clear();

		// Calculate the number of new words in each list
		for (map<string, set<string> >::iterator It = wordLists.begin(); It != wordLists.end(); ++It) {
			set<string> tempSet;

			set_difference(It->second.begin(), It->second.end(), wordsSoFar.begin(), wordsSoFar.end(), inserter(tempSet, tempSet.end()));
			overlap[tempSet.size()] = It->first;
		}

		// Find the list with the most new words
		cout << overlap.rbegin()->second << '\t' << overlap.rbegin()->first << endl;

		// Add the new words to the ones we've learned already
		wordsSoFar.insert(wordLists[overlap.rbegin()->second].begin(), wordLists[overlap.rbegin()->second].end());

	}
	while (overlap.rbegin()->first != 0);

	return 0;
}

