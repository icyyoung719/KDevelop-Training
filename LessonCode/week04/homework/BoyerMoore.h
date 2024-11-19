#ifndef BOYERMOORE_H
#define BOYERMOORE_H

#include <string>
#include <vector>
#include <array>
#include <unordered_map>

class BoyerMoore {
public:
    explicit BoyerMoore(const std::string& pattern);

    // we can pass a string reference and it won't be copied, for it's read only
    std::vector<int> search(const std::string& text);

private:
    std::string pattern;
    std::array<int, 256> badCharTable; // 存储坏字符规则
    std::vector<int> goodSuffixTable;

    // 预处理坏字符规则
    void preprocessBadCharTable();

    // 预处理好后缀规则
    void preprocessGoodSuffixTable();
};

#endif // BOYERMOORE_H
