#include <iostream>
#include <string>
#include <vector>
#include "BoyerMoore.h"

int main() {
    // 定义要搜索的文本和模式
    std::string text = "This is a simple example for the Boyer-Moore string search algorithm. "
        "This example is to test the Boyer-Moore algorithm for pattern matching.";
    std::string pattern = "Boyer-Moore";

    // 初始化BoyerMoore对象
    BoyerMoore bm(pattern);

    // 使用Boyer-Moore算法在文本中查找模式
    std::vector<int> positions = bm.search(text);

    // 输出匹配位置
    if (positions.empty()) {
        std::cout << "Pattern not found in the text." << std::endl;
    }
    else {
        std::cout << "Pattern found at positions: ";
        for (int pos : positions) {
            std::cout << pos << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
