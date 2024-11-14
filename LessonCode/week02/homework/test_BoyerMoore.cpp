#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include "BoyerMoore.h"

// Global mutexes for thread-safe operations
std::mutex vector_mtx;  // Protects access to the results map
std::mutex file_mtx;    // Protects file writing operations
std::mutex freq_mtx;    // Protects frequency updates

// Sub-thread function to search for a single keyword in the content
void search_keyword(const std::string& content, const std::string& keyword, std::vector<int>& result) {
    BoyerMoore bm(keyword);
    result = bm.search(content);
}

// Write intermediate results to a file
void write_intermediate_results(const std::vector<int>& results, const std::string& out_path) {
    std::lock_guard<std::mutex> lock(file_mtx); // Lock for file writing
    std::ofstream out_file(out_path, std::ios_base::app); // Open file in append mode
    if (!out_file.is_open()) {
        throw std::runtime_error("Could not open file");
    }
    for (int place : results) {
        out_file << place << ' ';
    }
    out_file.close();
}

// Main thread function to process 50MB chunks of the file
void process_chunk(const std::string& chunk, const std::vector<std::string>& keywords, std::unordered_map<std::string, std::vector<int>>& results) {
    std::vector<std::thread> threads;
    // Temporary storage for partial results of each keyword
    std::vector<std::vector<int>> partial_results(keywords.size());

    for (size_t i = 0; i < keywords.size(); ++i) {
        threads.emplace_back(search_keyword, chunk, keywords[i], std::ref(partial_results[i]));
    }

    for (auto& t : threads) {
        t.join();
    }

    std::lock_guard<std::mutex> lock(vector_mtx); // Lock for updating results map
    for (size_t i = 0; i < keywords.size(); ++i) {
        results[keywords[i]].insert(results[keywords[i]].end(), std::make_move_iterator(partial_results[i].begin()), std::make_move_iterator(partial_results[i].end()));
    }
}

int main() {
    // Path of the input file
    std::string path = R"(D:\WPS_course\性能大作业-文件以及输入输出示例\json_language_en.txt)";
    std::string root_out_path = R"(D:\WPS_course\性能大作业-文件以及输入输出示例\)";
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file");
    }
    // Get file size
    std::streamsize file_size = file.tellg();
    // Test the first 100MB
    file_size = std::min(file_size, static_cast<std::streamsize>(100 * 1024 * 1024));
    file.seekg(0, std::ios::beg);

    // Set every 50MB as a chunk
    size_t chunk_size = 50 * 1024 * 1024; // 50MB

    // Keywords to search for
    std::vector<std::string> keywords = { "vertin", "Vertin", "good", "bad", "result", "nature", "plan", "you", "master", "test" };

    // Map to store the results for each keyword
    std::unordered_map<std::string, std::vector<int>> results;
    // Map to store the frequency of each keyword
    std::unordered_map<std::string, int> frequency;
    for (const auto& keyword : keywords) {
        results[keyword] = std::vector<int>();
    }

    std::vector<std::thread> threads;
    //---------------------------------------------Initialization---------------------------------------------//

    // Read the file and create threads
    int i = 0;  // Counter for the chunk number
    for (size_t offset = 0; offset < file_size; offset += chunk_size, i++) {
        size_t current_chunk_size = std::min(chunk_size, static_cast<size_t>(file_size - offset));
        std::string chunk(current_chunk_size, '\0');
        file.read(&chunk[0], current_chunk_size);

        threads.emplace_back(process_chunk, chunk, keywords, std::ref(results));

        // Check if we need to write the results
        for (auto& result : results) {
            if (result.second.size() > 10000) {
                write_intermediate_results(result.second, root_out_path + result.first + "_out.txt");
                {
                    std::lock_guard<std::mutex> lock(freq_mtx); // Lock for frequency updates
                    frequency[result.first] += result.second.size();
                }
                result.second.clear();
            }
        }
    }
    file.close();

    // Wait for all threads to finish
    for (auto& t : threads) {
        t.join();
    }

    // Write the remaining results
    for (auto& result : results) {
        if (result.second.size() > 0) {
            write_intermediate_results(result.second, root_out_path + result.first + "_out.txt");
            {
                std::lock_guard<std::mutex> lock(freq_mtx); // Lock for frequency updates
                frequency[result.first] += result.second.size();
            }
            result.second.clear();
        }
    }

    // Write the frequency to a file
    std::ofstream times_file(root_out_path + "times.txt");
    if (!times_file.is_open()) {
        throw std::runtime_error("Could not open times.txt");
    }
    for (const auto& keyword : keywords) {
        times_file << keyword << ": " << frequency[keyword] << "\n";
        //test output
        std::cout << keyword << ": " << frequency[keyword] << "\n";
    }
    times_file.close();

    return 0;
}