﻿#include "BoyerMoore.h"
#include <iostream>
#include <algorithm>
#include <cstring>

// 构造函数初始化模式
BoyerMoore::BoyerMoore(const std::string& pattern)
    : pattern(pattern) {
    preprocessBadCharTable();
    preprocessGoodSuffixTable();
}

// 预处理坏字符规则
void BoyerMoore::preprocessBadCharTable() {
    int m = pattern.length();
    for (int i = 0; i < m; ++i) {
        std::string char_str(1, pattern[i]);
        badCharTable[char_str] = i;
    }
}

// 预处理好后缀规则
void BoyerMoore::preprocessGoodSuffixTable() {
    int m = pattern.length();
    goodSuffixTable.resize(m + 1, -1);

    std::vector<int> suffix(m + 1, -1);
    std::vector<bool> prefix(m + 1, false);

    // 计算后缀数组
    for (int i = 0; i < m; ++i) {
        int j = i;
        while (j >= 0 && pattern.substr(i, m - i) == pattern.substr(j, m - i)) {
            suffix[j] = i;
            j -= (m - i);
        }
    }

    // 计算前缀数组
    for (int i = 0; i < m; ++i) {
        if (suffix[i] != -1) {
            for (int k = 0; k < m - i; ++k) {
                if (!prefix[k]) {
                    prefix[k] = true;
                }
            }
        }
    }

    // 计算好后缀表
    for (int i = 0; i < m; ++i) {
        if (suffix[i] != -1) {
            goodSuffixTable[i] = m - 1 - suffix[i];
        }
        else {
            for (int k = 0; k < m - i; ++k) {
                if (prefix[k]) {
                    goodSuffixTable[i] = k;
                    break;
                }
            }
        }
    }
}

// 辅助函数：检查字符是否为UTF-8的开始字节
bool BoyerMoore::isUTF8StartByte(unsigned char byte) {
    return (byte & 0x80) == 0x00 || (byte & 0xE0) == 0xC0 || (byte & 0xF0) == 0xE0 || (byte & 0xF8) == 0xF0;
}

// 在给定文本中查找模式，返回匹配位置的索引列表
std::vector<int> BoyerMoore::search(const std::string& text) {
    int n = text.length();
    int m = pattern.length();
    std::vector<int> matches;

    int s = 0;  // s是模式在文本中的起始位置
    while (s <= n - m) {
        int j = m - 1;

        // 从后向前进行字符比较
        while (j >= 0 && pattern[j] == text[s + j]) {
            j--;
        }

        if (j < 0) {
            matches.push_back(s);  // 匹配成功
            s += (s + m < n) ? m - goodSuffixTable[0] : 1; // 按照好后缀规则位移
        }
        else {
            // 坏字符规则
            std::string badChar(1, text[s + j]);
            int badCharShift = j - badCharTable[badChar];
            if (badCharShift < 0) badCharShift = 1; // 确保至少向右移动1位

            // 好后缀规则
            int goodSuffixShift = (j < m - 1) ? m - 1 - goodSuffixTable[j + 1] : 1;

            // 使用坏字符和好后缀规则的最大位移
            s += std::max(badCharShift, goodSuffixShift);
        }
    }

    return matches;
}
