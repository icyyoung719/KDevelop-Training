#include "util.h"
#include <iostream>


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

std::string read_whole_file(std::string file_path) {
    // Read the file in binary mode to handle large files
    std::ifstream file_stream(file_path, std::ios::binary | std::ios::ate);
    if (!file_stream.is_open()) {
        std::cerr << "Failed to open input file: " << file_path << std::endl;
    }

    // Get file size and read the entire file into memory
    std::streamsize file_size = file_stream.tellg();
    file_stream.seekg(0, std::ios::beg);
    std::string content(file_size, '\0');
    file_stream.read(&content[0], file_size);
    file_stream.close();

    return content;
}