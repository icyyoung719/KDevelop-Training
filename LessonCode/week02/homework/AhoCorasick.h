#ifndef AHOCORASICK_H
#define AHOCORASICK_H

#include <string>
#include <vector>
#include <unordered_map>

class AhoCorasick {
public:
    AhoCorasick();
    void insertPattern(const std::string& pattern);  // add a pattern
    void buildAutomaton();                           // construct the Aho-Corasick automaton
    std::unordered_map<std::string, std::vector<int>> search(const std::string& text); // find all patterns in the text
    ~AhoCorasick();                    

private:
    struct TrieNode {
        std::unordered_map<char, TrieNode*> children;
        TrieNode* failureLink = nullptr;             // failure link
        std::vector<std::string> output;             // The output of the node
    };

    TrieNode* root;

    void deleteTrie(TrieNode* node);                 // delete the trie
};

#endif // AHOCORASICK_H
