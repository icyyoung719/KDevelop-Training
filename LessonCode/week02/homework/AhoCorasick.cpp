#include "AhoCorasick.h"
#include <queue>

AhoCorasick::AhoCorasick() {
    root = new TrieNode();
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

// 搜索文本中的所有模式并返回每个模式的匹配位置
std::unordered_map<std::string, std::vector<int>> AhoCorasick::search(const std::string& text) {
    std::unordered_map<std::string, std::vector<int>> result;
    TrieNode* node = root;

    for (int i = 0; i < text.size(); ++i) {
        char ch = text[i];

        // 根据失败指针找到匹配节点
        while (node && !node->children.count(ch)) {
            node = node->failureLink;
        }
        node = node ? node->children[ch] : root;

        // 处理所有的输出
        for (const std::string& pattern : node->output) {
            result[pattern].push_back(i - pattern.size() + 1);
        }
    }

    return result;
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

// 析构函数调用deleteTrie释放内存
AhoCorasick::~AhoCorasick() {
    deleteTrie(root);
}
