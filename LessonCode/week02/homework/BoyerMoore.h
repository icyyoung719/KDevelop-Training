#ifndef BOYERMOORE_H
#define BOYERMOORE_H

#include <string>
#include <vector>
#include <unordered_map>

class BoyerMoore {
public:
    explicit BoyerMoore(const std::string& pattern);

    // 在给定文本中查找模式，返回匹配位置的索引列表
    std::vector<int> search(const std::string& text);

private:
    std::string pattern;
    std::unordered_map<std::string, int> badCharTable; // 存储坏字符规则，使用字符串作为键以支持UTF-8
    std::vector<int> goodSuffixTable;

    // 预处理坏字符规则
    void preprocessBadCharTable();

    // 预处理好后缀规则
    void preprocessGoodSuffixTable();

    // 辅助函数：检查字符是否为UTF-8的开始字节
    bool isUTF8StartByte(unsigned char byte);
};

#endif // BOYERMOORE_H
