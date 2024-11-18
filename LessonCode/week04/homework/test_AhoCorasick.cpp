//#include <iostream>
//#include <fstream>
//#include <vector>
//#include <unordered_map>
//#include "AhoCorasick.h"
//
//// Write results to a file
//void write_results(const std::unordered_map<std::string, std::vector<int>>& matches, const std::string& out_path) {
//    std::ofstream out_file(out_path);
//    if (!out_file.is_open()) {
//        throw std::runtime_error("Could not open output file");
//    }
//
//    for (const auto& [pattern, positions] : matches) {
//        out_file << "Pattern '" << pattern << "' found at positions: ";
//        for (int pos : positions) {
//            out_file << pos << " ";
//        }
//        out_file << "\n";
//    }
//
//    out_file.close();
//}
//
//int main() {
//    // Path of the input file
//    std::string path = R"(D:\WPS_course\性能大作业-文件以及输入输出示例\enwiki-20231120-abstract1.xml)";
//    std::string pattern_path = R"(D:\WPS_course\性能大作业-文件以及输入输出示例\keyword.txt)";
//    std::string root_out_path = R"(D:\WPS_course\性能大作业-文件以及输入输出示例\)";
//
//    std::vector<std::string> keywords;
//    std::ifstream pattern_file(pattern_path);
//    if (!pattern_file.is_open()) {
//        std::cerr << "Failed to open pattern file: " << pattern_path << std::endl;
//        return 1;
//    }
//    std::string line;
//    while (std::getline(pattern_file, line)) {
//        keywords.push_back(line);
//    }
//
//    // Open the input file in binary mode to handle large files
//    std::ifstream file(path, std::ios::binary | std::ios::ate);
//    if (!file.is_open()) {
//        throw std::runtime_error("Could not open input file");
//    }
//
//    // Get file size and read the entire file into memory
//    std::streamsize file_size = file.tellg();
//    file.seekg(0, std::ios::beg);
//    std::string content(file_size, '\0');
//    file.read(&content[0], file_size);
//    file.close();
//
//    AhoCorasick ac;
//
//    for (const auto& keyword : keywords) {
//        ac.insertPattern(keyword);
//    }
//
//    // Build the automaton
//    ac.buildAutomaton();
//
//    // Search the text for all patterns
//    auto matches = ac.search(content);
//
//    // Write the results to a file
//    //write_results(matches, root_out_path + "results.txt");
//
//    std::ofstream times_file(root_out_path + "times.txt");
//    if (!times_file.is_open()) {
//        throw std::runtime_error("Could not open input file");
//    }
//
//    for (const auto& [pattern, positions] : matches) {
//        std::cout << "Pattern '" << pattern << "' found at positions: ";
//        std::cout << "Nums " << positions.size();
//        std::cout << "\n";
//
//        times_file << "Pattern '" << pattern << "' found at positions: ";
//        times_file << "Nums " << positions.size();
//        times_file << "\n";
//    }
//
//    return 0;
//}
