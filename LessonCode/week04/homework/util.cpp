#include "util.h"
#include <iostream>

void set_config(std::string& file_path, std::string& keywords_path, std::string& output_file_path, bool& search_algorithm) {
    std::string user_choice;
    std::string user_input; //for path and mode
    while (true) {
        std::cout << "--------------------------------------------------------------------" << std::endl;
        std::cout << "Search setting:\n";
        std::cout << "1: Set keyword.txt file path\n";
        std::cout << "2: Set document file path\n";
        std::cout << "3: Set output file path\n";
        std::cout << "4: Set search algorithm and search pattern, use BoyerMoore algorithm as default\n";
        std::cout << "q: Quit and begin searching\n";
        std::cin >> user_choice;

        if (user_choice == "1") {  
            std::cout << "Current keyword.txt path:  " << keywords_path << std::endl;
            std::cout << "Please input your file path, leave blank as default" << std::endl;
            std::getline(std::cin, user_input);
            std::getline(std::cin, user_input);
            if (!user_input.empty()) {
                keywords_path = user_input;
                std::cout << "Keyword.txt file path set to " << keywords_path << std::endl;
            }
            else { std::cout << "Leave as before" << std::endl; }
        }
        else if (user_choice == "2") {
            std::cout << "Current docs file path:  " << file_path << std::endl;
            std::cout << "Please input your document file path, leave blank as default" << std::endl;
            std::getline(std::cin, user_input);
            std::getline(std::cin, user_input);
            if (!user_input.empty()) {
                file_path = user_input;
                std::cout << "document file path set to " << file_path << std::endl;
            }
            else { std::cout << "Leave as before" << std::endl; }
        }
        else if (user_choice == "3") {
            std::cout << "Current output file path:  " << output_file_path << std::endl;
            std::cout << "Please input your output file path, leave blank as default" << std::endl;
            std::getline(std::cin, user_input);
            std::getline(std::cin, user_input);
            if (!user_input.empty()) {
                output_file_path = user_input;
                std::cout << "output file path set to " << output_file_path << std::endl;
            }
            else { std::cout << "Leave as before" << std::endl; }
        }
        else if (user_choice == "4") {
            std::cout << "AhoCorasick algotithm: Faster in searching all the keywords, but can't give single keyword search time" << std::endl;
            std::cout << "BoyerMoore algorithm: Faster single keyword search, but slower in all keywords search" << std::endl;
            std::cout << "Default as BoyerMoore" << std::endl;
            std::cout << "Please input your option, 1:AhoCorasick ,2:BoyerMoore" << std::endl;
            std::cin >> user_input;
            if (user_input == "1") {//AC
                search_algorithm = true;
                std::cout << "Search algorithm set to AhoCorasick" << std::endl;
            }
            else if (user_input == "2") {//BM
                search_algorithm = false;
                std::cout << "Search algorithm set to BoyerMoore" << std::endl;
            }
            else {
                std::cout << "Invalid input" << std::endl;
            }
        }
        else if (user_choice == "q") {
            std::cout << "Setting finished" << std::endl;
            std::cout << "Begin searching, use ";
            if (!search_algorithm) {
                std::cout<<"BoyerMoore algorithm"<<std::endl;
            }
            else
            {
                std::cout << "AhoCorasick algorithm" << std::endl;
            }
            break;
        }
        else {
            std::cout << "Invalid input" << std::endl;
        }
    }
}

// Write results to a file
void write_results(const std::vector<int>& results, const std::string& out_path) {
    std::ofstream out_file(out_path);
    if (!out_file.is_open()) {
        throw std::runtime_error("Could not open output file");
    }
    for (int place : results) {
        out_file << place << ' ';
    }
    out_file.close();
}

void loadKeywords(const std::string& path, std::vector<std::string>& keywords) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::ios_base::failure("Failed to open keyword file: " + path);
    }
    std::string line;
    while (std::getline(file, line)) {
        keywords.push_back(line);
    }
    file.close();
}
