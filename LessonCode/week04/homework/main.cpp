#include "BoyerMooreSearch.h"
#include <iostream>
#include <string>
#include <fstream>

int main() {
    try {
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

        BoyerMooreSearch::initializeThreadPool(4);
        BoyerMooreSearch bmSearch(file_path);
        bmSearch.setKeywords(keywords);
        bmSearch.searchAllKeywords();

        std::vector<int> results = bmSearch.getKeywordMatchCounts();
        for (size_t i = 0; i < keywords.size(); ++i) {
            std::cout << "Keyword: " << keywords[i] << ", Matches: " << results[i] << std::endl;
        }

        BoyerMooreSearch::destroyThreadPool();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return 0;
}
