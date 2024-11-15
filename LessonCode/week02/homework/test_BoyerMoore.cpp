#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "BoyerMoore.h"

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

int main() {
    // Path of the input file
    std::string path = R"(D:\WPS_course\性能大作业-文件以及输入输出示例\enwiki-20231120-abstract1.xml)";
    std::string root_out_path = R"(D:\WPS_course\性能大作业-文件以及输入输出示例\)";

    // Open the input file in binary mode to handle large files
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open input file");
    }

    // Get file size and read the entire file into memory
    std::streamsize file_size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::string content(file_size, '\0');
    file.read(&content[0], file_size);
    file.close();

    // Define the keyword to search
    std::string keyword = R"(<anchor>83601–83700</anchor>)";
    //std::string keyword = "Panda";

    // Create a Boyer-Moore instance for searching the keyword
    BoyerMoore bm(keyword);

    // Search the keyword in the entire content
    std::vector<int> results = bm.search(content);

    // Write the results to a file
    //write_results(results, root_out_path + "results.txt");

    // Optionally, print the results to the console
    std::cout << "Found " << results.size() << " occurrences of the keyword '" << keyword << "'\n";
    return 0;
}
