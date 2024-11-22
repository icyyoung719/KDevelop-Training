#include "BoyerMooreSearch.h"
#include <fstream>
#include <iostream>
#include <future>
#include <functional>

// 静态成员初始化
std::string BoyerMooreSearch::file_content;
std::chrono::steady_clock::time_point BoyerMooreSearch::begin_time = std::chrono::high_resolution_clock::now();
std::vector<std::thread> BoyerMooreSearch::thread_pool;
std::queue<std::function<void()>> BoyerMooreSearch::task_queue;
std::atomic<size_t> BoyerMooreSearch::pending_tasks{ 0 };
std::mutex BoyerMooreSearch::queue_mutex;
std::condition_variable BoyerMooreSearch::queue_condition;
bool BoyerMooreSearch::stop_threads = false;

BoyerMooreSearch::BoyerMooreSearch(const std::string& file_path)
    : file_path(file_path) {
    if (file_content.empty()) {
        std::ifstream file(file_path, std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open input file");
        }
        std::streamsize file_size = file.tellg();
        file.seekg(0, std::ios::beg);
        file_content.resize(file_size);
        file.read(&file_content[0], file_size);
        file.close();
    }
}

void BoyerMooreSearch::setKeywords(const std::vector<std::string>& keywords) {
    this->keywords = keywords;
    keyword_match_counts.resize(keywords.size());
    keyword_search_time.resize(keywords.size());
}

void BoyerMooreSearch::searchAllKeywords() {
    for (size_t i = 0; i < keywords.size(); i++) {
        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            task_queue.emplace([this, i]() {
                int count = searchWord(keywords[i]);
                keyword_match_counts[i] = count;
                keyword_search_time[i] = std::chrono::high_resolution_clock::now();

                // 减少计数器
                {
                    std::lock_guard<std::mutex> lock(queue_mutex);
                    pending_tasks--;
                }
                queue_condition.notify_all();
                });
            pending_tasks++; // 增加计数器
        }
        queue_condition.notify_one();
    }

    // 等待所有任务完成
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        queue_condition.wait(lock, [] { return pending_tasks == 0; });
    }
}

std::vector<int> BoyerMooreSearch::getKeywordMatchCounts() {
    return keyword_match_counts;
}

std::vector<std::chrono::steady_clock::time_point> BoyerMooreSearch::getKeywordSearchTime() {
    return keyword_search_time;
}

void BoyerMooreSearch::fileOutputKeywordMatchCounts(std::string output_path) {
    std::ofstream output_file(output_path);
    if (!output_file.is_open()) {
        throw std::runtime_error("Could not open output file");
    }
    for (int i = 0; i < keywords.size(); i++) {
        output_file << i + 1 << ". " << keywords[i]
            << ": count:" << keyword_match_counts[i]
            << "  time:" << std::chrono::duration<double>(keyword_search_time[i] - begin_time).count() << " seconds.\n";
    }
}

int BoyerMooreSearch::searchWord(const std::string& keyword) {
    BoyerMoore bm(keyword);
    std::vector<int> results = bm.search(file_content);
    return results.size();
}

void BoyerMooreSearch::initializeThreadPool(size_t pool_size) {
    stop_threads = false;
    for (size_t i = 0; i < pool_size; ++i) {
        thread_pool.emplace_back(workerThread);
    }
}

void BoyerMooreSearch::destroyThreadPool() {
    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        stop_threads = true;
    }
    queue_condition.notify_all();
    for (std::thread& thread : thread_pool) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    thread_pool.clear();
}

void BoyerMooreSearch::workerThread() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            queue_condition.wait(lock, [] { return stop_threads || !task_queue.empty(); });
            if (stop_threads && task_queue.empty()) {
                return;
            }
            task = std::move(task_queue.front());
            task_queue.pop();
        }
        task();
    }
}