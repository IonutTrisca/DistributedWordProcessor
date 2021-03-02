#include "tema3.h"

void capitalEven(int i, vector<string> &paragraphLines) {
    string line = paragraphLines[i];
    stringstream lineStream(line);
    string word;
    vector<string> words;

    while(getline(lineStream, word, ' ')) {
        words.push_back(word);
    }

    string newLine;

    for (auto word : words) {
        string newWord = "";

        for(uint64_t j = 0; j < word.size(); j++) {
            if((j + 1) % 2 == 0 && isalpha(word[j]))
                newWord += toupper(word[j]);
            else
                newWord += word[j];
        }

        newWord += " ";
        newLine += newWord;
    }

    newLine.erase(newLine.begin() + newLine.size() - 1);

    paragraphLines[i] = newLine;
}
