#include "tema3.h"

void capitalFirstLetter(int i, vector<string> &paragraphLines) {
    string line = paragraphLines[i];
    stringstream lineStream(line);
    string word;
    vector<string> words;

    while(getline(lineStream, word, ' ')) {
        words.push_back(word);
    }

    string newLine;

    for (uint64_t j = 0; j < words.size(); j++) {
        words[j][0] = toupper(words[j][0]);
        newLine += words[j] + " ";
    }

    newLine.erase(newLine.begin() + newLine.size() - 1);

    paragraphLines[i] = newLine;
}
