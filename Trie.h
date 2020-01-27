#ifndef TRIE_H_41E9526E_11C7_47BA_B5F7_215E6F7044B1
#define TRIE_H_41E9526E_11C7_47BA_B5F7_215E6F7044B1

#include <string>
#include <vector>
#include <memory>

class TrieNode
{
    public:
        const std::string word;
        bool isValidWord;
        std::vector<std::unique_ptr<TrieNode> > childrenWords;

        TrieNode(const std::string& val, bool isWord);
};

class Trie {
    public:
        Trie();
        void insert(const std::string& word);
        bool isValidWordInDictionary(const std::string& word) const;
        bool isPrefixOfOtherWords(const std::string& prefix) const;

    private:
        void recursiveInsertion(TrieNode* parent, const std::string& word);
        TrieNode* recursiveFindNode(TrieNode* parent, const std::string& word) const;

    private:
        std::unique_ptr<TrieNode> mRootNode;
};

#endif
