#include "tema3.h"

void doubleConsonants(int i, vector<string> &paragraphLines) {
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
            char c = word[j];
            newWord += c;

            bool isLowercaseVowel = (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');
            bool isUppercaseVowel = (c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U');

            if (!isLowercaseVowel && !isUppercaseVowel && isalpha(c)) {
                newWord += tolower(c);
            }
        }

        newWord += " ";
        newLine += newWord;
    }

    newLine.erase(newLine.begin() + newLine.size() - 1);

    paragraphLines[i] = newLine;
}
