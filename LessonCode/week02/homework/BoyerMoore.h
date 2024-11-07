#ifndef BOYERMOORE_H
#define BOYERMOORE_H

#include <string>
#include <vector>
#include <unordered_map>

class BoyerMoore {
public:
    // 构造函数初始化模式
    explicit BoyerMoore(const std::string& pattern);

    // 在给定文本中查找模式，返回匹配位置的索引列表
    std::vector<int> search(const std::string& text);

private:
    std::string pattern;
    std::unordered_map<char, int> badCharTable;
    std::vector<int> goodSuffixTable;

    // 预处理坏字符规则
    void preprocessBadCharTable();

    // 预处理好后缀规则
    void preprocessGoodSuffixTable();
};

#endif // BOYERMOORE_H
