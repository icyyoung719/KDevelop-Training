// main.cpp
#include <iostream>
#include <string>
#include "test_BoyerMoore.h"
#include "AhoCorasick.h"
#include "util.h"

int main() {
    std::string file_path = R"(D:\WPS_course\性能大作业-文件以及输入输出示例\enwiki-20231120-abstract1.xml)";
    std::string pattern_path = R"(D:\WPS_course\性能大作业-文件以及输入输出示例\keyword.txt)";
    std::string root_out_path = R"(D:\WPS_course\性能大作业-文件以及输入输出示例\)";

    // Read pattern_path to get keyword
    std::vector<std::string> keywords;
    std::ifstream file(pattern_path);
    if (!file.is_open()) {
        std::cerr << "Failed to open pattern file: " << pattern_path << std::endl;
        return -1;
    }
    std::string line;
    while (std::getline(file, line)) {
        keywords.push_back(line);
    }

    //auto keyword = keywords[2];
    //auto keyword2 = "<anchor>83601–83700</anchor>";
    // Use test_BoyerMoore to search a single keyword
    //int result = test_BoyerMoore(file_path, keyword);

    
    // Test Aho-Corasick
    AhoCorasick ac;
    for (const auto& keyword : keywords) {
        ac.insertPattern(keyword);
    }
    ac.buildAutomaton();
    std::string content = read_whole_file(file_path);

    // Search for patterns using Aho-Corasick
    std::unordered_map<std::string, std::vector<int>> matches = ac.search(content);

    std::ofstream times_file(root_out_path + "times.txt");
    if (!times_file.is_open()) {
        throw std::runtime_error("Could not open input file");
    }
    for (const auto& [pattern, positions] : matches) {
        std::cout << "Pattern: " << pattern << "Nums " << positions.size();
        std::cout << "\n";
    
        times_file << "Pattern: " << pattern << "Nums " << positions.size();
        times_file << "\n";
    }

    return 0;
}
