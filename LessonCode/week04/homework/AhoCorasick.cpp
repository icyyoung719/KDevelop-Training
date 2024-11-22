#include "AhoCorasick.h"
#include <fstream>
#include <queue>
#include <stdexcept>

std::string AhoCorasick::file_content = ""; // 静态成员初始化

// 加载文件内容到静态成员
void AhoCorasick::loadFile(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open input file: " + filePath);
    }
    std::streamsize file_size = file.tellg();
    file.seekg(0, std::ios::beg);
    file_content.resize(file_size);
    file.read(&file_content[0], file_size);
    file.close();
}

AhoCorasick::AhoCorasick(const std::string& file_path) {
    root = new TrieNode();
    AhoCorasick::loadFile(file_path);
}

// 插入模式
void AhoCorasick::insertPattern(const std::string& pattern) {
    TrieNode* node = root;
    for (char ch : pattern) {
        if (!node->children.count(ch)) {
            node->children[ch] = new TrieNode();
        }
        node = node->children[ch];
    }
    node->output.push_back(pattern); // 在终止节点存储模式
}

// 构建自动机的失败指针
void AhoCorasick::buildAutomaton() {
    std::queue<TrieNode*> q;

    // 初始层的失败指针指向根节点
    for (auto& [ch, child] : root->children) {
        child->failureLink = root;
        q.push(child);
    }

    // 构建失败指针
    while (!q.empty()) {
        TrieNode* current = q.front();
        q.pop();

        // 遍历当前节点的所有子节点
        for (auto& [ch, child] : current->children) {
            TrieNode* fail = current->failureLink;

            // 找到failureLink直到找到与字符ch匹配的节点
            while (fail && !fail->children.count(ch)) {
                fail = fail->failureLink;
            }
            child->failureLink = fail ? fail->children[ch] : root;

            // 合并输出
            if (child->failureLink) {
                child->output.insert(child->output.end(),
                    child->failureLink->output.begin(),
                    child->failureLink->output.end());
            }
            q.push(child);
        }
    }
}

// 搜索静态文件内容中的所有模式
void AhoCorasick::search() {
    if (file_content.empty()) {
        throw std::runtime_error("No file content loaded for searching.");
    }

    search_results.clear(); // 清空之前的搜索结果
    times_results.clear();
    TrieNode* node = root;

    for (int i = 0; i < file_content.size(); ++i) {
        char ch = file_content[i];

        // 根据失败指针找到匹配节点
        while (node && !node->children.count(ch)) {
            node = node->failureLink;
        }
        node = node ? node->children[ch] : root;

        // 处理所有的输出
        for (const std::string& pattern : node->output) {
            search_results[pattern].push_back(i - pattern.size() + 1);
        }
    }
}
void AhoCorasick::searchWithoutPositions() {
    if (file_content.empty()) {
        throw std::runtime_error("No file content loaded for searching.");
    }

    search_results.clear(); // 清空之前的搜索结果
    times_results.clear();
    TrieNode* node = root;

    for (int i = 0; i < file_content.size(); ++i) {
        char ch = file_content[i];

        // 根据失败指针找到匹配节点
        while (node && !node->children.count(ch)) {
            node = node->failureLink;
        }
        node = node ? node->children[ch] : root;

        // 处理所有的输出
        for (const std::string& pattern : node->output) {
            times_results[pattern] += 1;
        }
    }
}

// 获取搜索结果
const std::unordered_map<std::string, std::vector<int>>& AhoCorasick::getSearchResults() const {
    return search_results;
}
const std::unordered_map<std::string, int>& AhoCorasick::getTimesResults() const {
    return times_results;
}

// 删除Trie树，防止内存泄漏
void AhoCorasick::deleteTrie(TrieNode* node) {
    if (node) {
        for (auto& [_, child] : node->children) {
            deleteTrie(child);
        }
        delete node;
    }
}

void AhoCorasick::generateTimesResult() {
    for (auto& [pattern, positions] : search_results) {
        times_results[pattern] = positions.size();
    }
}

// 析构函数调用deleteTrie释放内存
AhoCorasick::~AhoCorasick() {
    deleteTrie(root);
    file_content.clear();
}
