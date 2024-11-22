#ifndef AHOCORASICK_H
#define AHOCORASICK_H

#include <string>
#include <vector>
#include <unordered_map>

class AhoCorasick {
public:

    AhoCorasick(const std::string& filePath);
    void insertPattern(const std::string& pattern);   // 插入模式
    void buildAutomaton();                            // 构建自动机
    void search();                                    // 搜索静态文件内容，结果存储到内部成员变量
    const std::unordered_map<std::string, std::vector<int>>& getSearchResults() const; // 获取搜索结果
    ~AhoCorasick();

private:
    static void loadFile(const std::string& filePath); // 加载文件内容到静态成员
    struct TrieNode {
        std::unordered_map<char, TrieNode*> children;
        TrieNode* failureLink = nullptr;             // 失败指针
        std::vector<std::string> output;             // 节点的输出模式
    };

    TrieNode* root;
    std::unordered_map<std::string, std::vector<int>> search_results; // 搜索结果
    std::unordered_map<std::string, int> times_results; //只存放搜索到的次数，不含位置
    static std::string file_content;                // 静态成员存储文件内容

    void deleteTrie(TrieNode* node);                // 删除Trie树
};

#endif // AHOCORASICK_H
