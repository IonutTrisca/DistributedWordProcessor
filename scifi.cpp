#include "tema3.h"

void reverseSeventh(int i, vector<string> &paragraphLines) {
    string line = paragraphLines[i];
    stringstream lineStream(line);
    string word;
    vector<string> words;

    while(getline(lineStream, word, ' ')) {
        words.push_back(word);
    }

    string newLine;
    int wordCount = 0;

    for (auto word : words) {
        wordCount++;

        if (wordCount == 7) {
            reverse(word.begin(), word.end());
            wordCount = 0;
        }
        
        newLine += word + " ";
    }

    newLine.erase(newLine.begin() + newLine.size() - 1);

    paragraphLines[i] = newLine;
}
