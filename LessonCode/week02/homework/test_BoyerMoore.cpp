#include <iostream>
#include <string>
#include <fstream>
#include "BoyerMoore.h"

int test_BoyerMoore() {
    //用关键词字符串初始化
    std::string key_word = { "vertin" };
    BoyerMoore bm(key_word);

    //读入文件到std::string
    std::string path = R"(D:\WPS_course\性能大作业-文件以及输入输出示例\json_language_en.txt)";
    std::ifstream file(path);
    // 检查文件是否成功打开
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file");
    }
    // 使用string对象读取整个文件
    std::string content((std::istreambuf_iterator<char>(file)),
        (std::istreambuf_iterator<char>()));

    file.close();

    auto result = bm.search(content);

    std::cout<<"Pattern found nums: "<<result.size()<<std::endl;

    //写入结果到 test_keyword_out.txt
    std::string out_path = R"(D:\WPS_course\性能大作业-文件以及输入输出示例\test_keyword_out.txt)";
    std::ofstream out_file(out_path);
    if (!out_file.is_open()) {
        throw std::runtime_error("Could not open file");
    }
    for (auto place : result) {
        out_file << place << ' ';
    }


    return 0;
}
