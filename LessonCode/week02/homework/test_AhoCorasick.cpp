#include <iostream>
#include "AhoCorasick.h"

int test_AhoCorasick() {
    AhoCorasick ac;

    // 插入多个模式
    ac.insertPattern("he");
    ac.insertPattern("she");
    ac.insertPattern("his");
    ac.insertPattern("hers");

    // 构建自动机
    ac.buildAutomaton();

    // 搜索文本中的所有模式
    std::string text = "ushers";
    auto matches = ac.search(text);

    // 输出匹配结果
    for (const auto& [pattern, positions] : matches) {
        std::cout << "Pattern '" << pattern << "' found at positions: ";
        for (int pos : positions) {
            std::cout << pos << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
