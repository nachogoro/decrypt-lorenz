#include "Trie.h"
#include <fstream>
#include <iostream>
#include <cstdint>
#include <map>
#include <bitset>

#define WORD_LENGTH 4

namespace
{
    std::map<uint8_t, char> getCharactersMap()
    {
        std::map<uint8_t, char> result;

        for (uint8_t index = 0; index < 26; ++index)
        {
            result[index] = 'a' + index;
        }

        return result;
    }

    void recursiveDecryption(
            const std::map<uint8_t, char>& charMap,
            const uint8_t maxKeyValue,
            const Trie& dict,
            const std::vector<std::vector<uint8_t>>& cyphers,
            std::vector<uint8_t>& currentKey,
            std::vector<std::string>& prefixes)
    {
        const uint8_t index = currentKey.size();

        if (index == WORD_LENGTH)
        {
            // Found a solution
            std::cout << "Possible solution: ";
            for (const auto& s : prefixes)
            {
                std::cout << s << " ";
            }

            uint32_t key = 0;
            for (size_t i = 0; i < currentKey.size(); ++i)
            {
                key = (key << 5) + currentKey[i];
            }
            const std::string keyInBinary = std::bitset<20>(key).to_string();

            std::cout << "\nUsing key: " << keyInBinary << "\n--------\n";

            return;
        }

        for (uint8_t candidateKey = 0; candidateKey <= maxKeyValue; ++candidateKey)
        {
            bool keyWorksForAllCyphers = true;

            for (size_t i = 0; i < cyphers.size(); ++i)
            {
                const uint8_t decypheredCharIndex = cyphers[i][index] ^ candidateKey;
                if (!charMap.count(decypheredCharIndex)) {
                    keyWorksForAllCyphers = false;
                    break;
                }

                const std::string newPrefix
                    = prefixes[i] + std::string(1, charMap.at(decypheredCharIndex));

                if (index == WORD_LENGTH - 1)
                {
                    // We need to check that this is a word, not a prefix
                    if (!dict.isValidWordInDictionary(newPrefix))
                    {
                        keyWorksForAllCyphers = false;
                        break;
                    }
                }
                else if (!dict.isPrefixOfOtherWords(newPrefix))
                {
                    keyWorksForAllCyphers = false;
                    break;
                }
            }

            if (keyWorksForAllCyphers == false)
            {
                continue;
            }

            // This key might be promising
            currentKey.push_back(candidateKey);
            for (size_t i = 0; i < prefixes.size(); ++i)
            {
                prefixes[i].push_back(charMap.at(cyphers[i][index] ^ candidateKey));
            }
            recursiveDecryption(charMap, maxKeyValue, dict, cyphers, currentKey, prefixes);
            currentKey.pop_back();
            for (size_t i = 0; i < prefixes.size(); ++i)
            {
                prefixes[i].pop_back();
            }
        }
    }

    void printPossibleMessages(
            const std::map<uint8_t, char>& charMap,
            const uint8_t maxKeyValue,
            const Trie& dict,
            const std::vector<std::vector<uint8_t>>& cyphers)
    {
        std::vector<std::string> prefixes(cyphers.size());
        std::vector<uint8_t> currentKey;
        ::recursiveDecryption(charMap, maxKeyValue, dict, cyphers, currentKey, prefixes);
    }
}

int main()
{
    const std::map<uint8_t, char> charMap = ::getCharactersMap();
    const uint8_t maxKeyValue = 31;

    std::vector<std::vector<uint8_t>> cyphers = {
        {20, 24, 24, 21},
        {16, 24, 25, 27}};

    // Initialize a trie with all 4-letter words in Spanish
    Trie dict;

    std::ifstream infile("esp-4letters.txt");
    std::string line;

    while (std::getline(infile, line))
    {
        dict.insert(line);
    }

    // Try to find the right key char by char
    ::printPossibleMessages(charMap, maxKeyValue, dict, cyphers);
}
