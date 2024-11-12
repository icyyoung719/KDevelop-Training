#ifndef AHOCORASICK_H
#define AHOCORASICK_H

#include <string>
#include <vector>
#include <unordered_map>

class AhoCorasick {
public:
    AhoCorasick();
    void insertPattern(const std::string& pattern);  // 插入一个模式
    void buildAutomaton();                           // 构建自动机
    std::unordered_map<std::string, std::vector<int>> search(const std::string& text); // 在文本中查找所有模式
    AhoCorasick::~AhoCorasick();                    //析构函数

private:
    struct TrieNode {
        std::unordered_map<char, TrieNode*> children;
        TrieNode* failureLink = nullptr;             // 失败指针
        std::vector<std::string> output;             // 输出模式（终结模式）
    };

    TrieNode* root;

    void deleteTrie(TrieNode* node);                 // 删除Trie树
};

#endif // AHOCORASICK_H
