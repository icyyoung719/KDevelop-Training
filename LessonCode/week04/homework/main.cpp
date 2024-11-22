#include "BoyerMooreSearch.h"
#include "AhoCorasick.h"
#include <iostream>
#include <string>
#include <fstream>
#include <chrono> // 用于计时

int main() {
    std::string file_path = R"(D:\WPS_course\性能大作业-文件以及输入输出示例\enwiki-20231120-abstract1.xml)";
    std::string pattern_path = R"(D:\WPS_course\性能大作业-文件以及输入输出示例\mykeywords.txt)";
    std::string output_file_path = R"(D:\WPS_course\性能大作业-文件以及输入输出示例\times.txt)";

    try {
        // 计时开始
        auto start = std::chrono::high_resolution_clock::now();

        // 读取关键词文件
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
        file.close();

        auto read_keywords_end = std::chrono::high_resolution_clock::now();

        // 初始化线程池
        size_t thread_nums = std::thread::hardware_concurrency();
        BoyerMooreSearch::initializeThreadPool(thread_nums);

        auto thread_init_end = std::chrono::high_resolution_clock::now();

        // 设置文件和关键词
        BoyerMooreSearch bmSearch(file_path);
        bmSearch.setKeywords(keywords);

        // 搜索所有关键词
        bmSearch.searchAllKeywords();

        auto search_end = std::chrono::high_resolution_clock::now();
        // 获取结果
        std::vector<int> results = bmSearch.getKeywordMatchCounts();
        BoyerMooreSearch::destroyThreadPool();

        auto end = std::chrono::high_resolution_clock::now();

        // 用户选择功能
        std::string user_choice;
        while (true) {
            std::cout << "--------------------------------------------------------------------" << std::endl;
            std::cout << "Choose an output option:\n";
            std::cout << "1: Output to console\n";
            std::cout << "2: Output to file\n";
            std::cout << "3: Output time consumption detail\n";
            std::cout << "q: Quit\n";
            std::cin >> user_choice;

            if (user_choice == "1") {
                // 输出到控制台
                std::vector<std::chrono::steady_clock::time_point> search_timme_vector = bmSearch.getKewwordSearchTime();
                for (size_t i = 0; i < keywords.size(); ++i) {
                    std::cout << "Keyword: " << keywords[i] 
                        << ", Matches: " << results[i] 
                        << "  time:" << std::chrono::duration<double>(search_timme_vector[i] - thread_init_end).count() << " seconds.\n";
                }
            }
            else if (user_choice == "2") {
                // 输出到文件
                bmSearch.fileOutputKeywordMatchCounts(output_file_path);
                std::cout << "Results have been written to " << output_file_path << std::endl;
            }
            else if (user_choice == "3") {
                std::cout << "Time to read keywords: "
                    << std::chrono::duration<double>(read_keywords_end - start).count()
                    << " seconds." << std::endl;
                std::cout << "Time to initialize thread pool: "
                    << std::chrono::duration<double>(thread_init_end - read_keywords_end).count()
                    << " seconds." << std::endl;
                std::cout << "Time for keyword search: "
                    << std::chrono::duration<double>(search_end - thread_init_end).count()
                    << " seconds." << std::endl;
                std::cout << "Average search time: "
                    << std::chrono::duration<double>(search_end - thread_init_end).count() / keywords.size()
                    << " seconds." << std::endl;
                std::cout << "Total time: "
                    << std::chrono::duration<double>(end - start).count()
                    << " seconds." << std::endl;
            }
            else if (user_choice == "q") {
                std::cout << "Exiting..." << std::endl;
                break;
            }
            else {
                std::cout << "Invalid choice. Please try again." << std::endl;
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return 0;
}
