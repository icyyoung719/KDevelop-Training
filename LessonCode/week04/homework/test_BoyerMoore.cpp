// test_BoyerMoore.cpp
#include <iostream>
#include <fstream>
#include <vector>
#include "BoyerMoore.h"

int test_BoyerMoore(const std::string& file_path, const std::string& keyword) {
    BoyerMoore bm(keyword);
    // 打开输入文件
    std::ifstream file(file_path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Could not open input file\n";
        return -1;  // 返回错误码
    }

    // 获取文件大小并读取到内存中
    std::streamsize file_size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::string content(file_size, '\0');
    file.read(&content[0], file_size);
    file.close();

    // Boyer-Moore 搜索
    //BoyerMoore bm(keyword);
    std::vector<int> results = bm.search(content);

    // 输出匹配的个数
    std::cout << "Found " << results.size() << " occurrences of the keyword '" << keyword << "'\n";
    return results.size();
}
