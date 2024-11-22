#include "BoyerMooreSearch.h"
#include "AhoCorasick.h"
#include "util.h"
#include <iostream>
#include <string>
#include <fstream>
#include <chrono> // 用于计时

int main() {
    std::string file_path = R"(D:\WPS_course\性能大作业-文件以及输入输出示例\enwiki-20231120-abstract1.xml)";
    std::string keywords_path = R"(D:\WPS_course\性能大作业-文件以及输入输出示例\mykeywords - 副本.txt)";
    std::string output_file_path = R"(D:\WPS_course\性能大作业-文件以及输入输出示例\times.txt)";
    int AC_min_use_keywords_num = 20;
    int AC_max_use_keywords_num = 20000;

//---------------------------------------------Pre-set------------------------------------------
    bool search_algorithm = false; // 0(false) as BoyerMoore, 1(true) as AhoCorasick
    std::chrono::steady_clock::time_point start;
    std::chrono::steady_clock::time_point read_keywords_end;
    std::chrono::steady_clock::time_point thread_init_end;
    std::chrono::steady_clock::time_point search_end;
    std::chrono::steady_clock::time_point end;
    std::chrono::steady_clock::time_point file_write_end;
    std::vector<int> search_result;

    std::vector<std::chrono::steady_clock::time_point> bm_search_time;
    std::vector<std::chrono::steady_clock::time_point> search_time_vector; // 存放BM算法keyword搜索时间

    auto is_default_algorithm = set_config(file_path, keywords_path, output_file_path, search_algorithm);
    try {
//---------------------------------------------Init and Search------------------------------------------
        start = std::chrono::high_resolution_clock::now();

        // 读取关键词文件
        std::vector<std::string> keywords;
        loadKeywords(keywords_path, keywords);
        if (keywords.size() >= AC_min_use_keywords_num && is_default_algorithm) {
            search_algorithm = true; //use AC algorithm
        }
        if (keywords.size() >= AC_max_use_keywords_num && is_default_algorithm) {
            search_algorithm = true; //use AC algorithm
            std::cout<<"Too many keywords, AC will not retain the positions of string matches."<<std::endl;
        }
        read_keywords_end = std::chrono::high_resolution_clock::now();

        if (!search_algorithm) {  // BM算法
            size_t thread_nums = std::thread::hardware_concurrency();
            BoyerMooreSearch::initializeThreadPool(thread_nums);
            BoyerMooreSearch bm(file_path);
            bm.setKeywords(keywords);
            thread_init_end = std::chrono::high_resolution_clock::now();

            bm.searchAllKeywords();
            search_end = std::chrono::high_resolution_clock::now();
            search_result = bm.getKeywordMatchCounts();
            search_time_vector = bm.getKeywordSearchTime();

            BoyerMooreSearch::destroyThreadPool();
        }
        else {  // AC算法
            AhoCorasick ac(file_path);
            for (const auto& keyword : keywords) {
                ac.insertPattern(keyword);
            }
            ac.buildAutomaton();
            thread_init_end = std::chrono::high_resolution_clock::now();

            if (keywords.size() >= AC_max_use_keywords_num) {
                ac.searchWithoutPositions();
            }
            else {
                ac.search();
            }
            search_end = std::chrono::high_resolution_clock::now();
            if (keywords.size() >= AC_max_use_keywords_num) {

            }

            if (keywords.size() < AC_max_use_keywords_num) {
                ac.generateTimesResult();
            }
            const std::unordered_map<std::string, int>& AC_results = ac.getTimesResults();
            for (auto i = 0; i < keywords.size(); ++i) {
                auto it = AC_results.find(keywords[i]); // 查找关键词是否存在结果中
                if (it != AC_results.end()) {
                    search_result.push_back(it->second);
                }
                else {
                    search_result.push_back(0);
                }
            }
        }

        // 输出结果到文件
        std::ofstream output_file(output_file_path);
        if (!output_file.is_open()) {
            throw std::runtime_error("Could not open output file");
        }
        if (!search_algorithm) {
            for (int i = 0; i < keywords.size(); i++) {
                output_file << i + 1 << ". " << keywords[i]
                    << ": count:" << search_result[i]
                    << "  time:" << std::chrono::duration<double>(search_time_vector[i] - thread_init_end).count() << " seconds.\n";
            }
            output_file.flush(); // 强制刷新
        }
        else {
            for (int i = 0; i < keywords.size(); i++) {
                output_file << i + 1 << ". " << keywords[i]
                    << ": count:" << search_result[i] << '\n';
            }
            output_file.flush(); // 强制刷新
        }
        std::cout << "Results have been written to " << output_file_path << std::endl;

        end = std::chrono::high_resolution_clock::now();
//---------------------------------------------Interactive---------------------------------------
        // 用户选择功能
        std::string user_choice;
        while (true) {
            std::cout << "--------------------------------------------------------------------" << std::endl;
            std::cout << "Choose an output option:\n";
            std::cout << "1: Output search result to console\n";
            std::cout << "2: Output search result to file\n";
            std::cout << "3: Output time consumption detail to console\n";
            std::cout << "q: Quit\n";
            std::cin >> user_choice;
            std::cout << "--------------------------------------------------------------------" << std::endl;

            if (user_choice == "1") {
                // 输出到控制台
                std::cout << "Time for keyword search: "
                    << std::chrono::duration<double>(search_end - thread_init_end).count()
                    << " seconds." << std::endl;
                std::cout << "Keywords num: " << keywords.size() << std::endl;
                std::cout << "Average search time: "
                    << std::chrono::duration<double>(search_end - thread_init_end).count() / keywords.size()
                    << " seconds." << std::endl;
                if (!search_algorithm) {
                    for (size_t i = 0; i < keywords.size(); ++i) {
                        std::cout << "Keyword: " << keywords[i]
                            << ", Matches: " << search_result[i]
                            << "  time:" << std::chrono::duration<double>(search_time_vector[i] - thread_init_end).count() << " seconds.\n";
                    }
                }
                else {
                    for (size_t i = 0; i < keywords.size(); ++i) {
                        std::cout << "Keyword: " << keywords[i]
                            << ", Matches: " << search_result[i] << "\n";
                    }
                }
            }
            else if (user_choice == "2") {
                // 输出到文件
                std::ofstream output_file(output_file_path);
                if (!output_file.is_open()) {
                    throw std::runtime_error("Could not open output file");
                }

                if (!search_algorithm) {
                    for (int i = 0; i < keywords.size(); i++) {
                        output_file << i + 1 << ". " << keywords[i]
                            << ": count:" << search_result[i]
                            << "  time:" << std::chrono::duration<double>(search_time_vector[i] - thread_init_end).count() << " seconds.\n";
                    }
                }
                else {
                    for (int i = 0; i < keywords.size(); i++) {
                        output_file << i + 1 << ". " << keywords[i]
                            << ": count:" << search_result[i] << '\n';
                    }
                }
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
                std::cout << "Keywords num: " << keywords.size() << std::endl;
                std::cout << "Average search time: "
                    << std::chrono::duration<double>(search_end - thread_init_end).count() / keywords.size()
                    << " seconds." << std::endl;
                std::cout<<"Time for keyword output: "
                    << std::chrono::duration<double>(end - search_end).count() << " seconds." << std::endl;
                std::cout << "Total time: "
                    << std::chrono::duration<double>(end - start).count()
                    << " seconds." << std::endl;
            }
            else if (user_choice == "q") {
                std::cout << "Exiting..." << std::endl;
                break;
            }
            else {
                std::cout << "Invalid choice. Please enter 1, 2, 3, or q." << std::endl;
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return 0;
}