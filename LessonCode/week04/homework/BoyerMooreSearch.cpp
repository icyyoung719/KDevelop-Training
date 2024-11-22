﻿#include "BoyerMooreSearch.h"
#include <fstream>
#include <iostream>
#include <functional>

// 静态成员初始化
std::string BoyerMooreSearch::fileContent;
std::chrono::steady_clock::time_point BoyerMooreSearch::begin_time = std::chrono::high_resolution_clock::now();
std::vector<std::thread> BoyerMooreSearch::threadPool;
std::queue<std::function<void()>> BoyerMooreSearch::taskQueue;
std::atomic<size_t> BoyerMooreSearch::pendingTasks{ 0 };
std::mutex BoyerMooreSearch::queueMutex;
std::condition_variable BoyerMooreSearch::queueCondition;
bool BoyerMooreSearch::stopThreads = false;

BoyerMooreSearch::BoyerMooreSearch(const std::string& filePath)
    : filePath(filePath) {
    if (fileContent.empty()) {
        std::ifstream file(filePath, std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open input file");
        }
        std::streamsize fileSize = file.tellg();
        file.seekg(0, std::ios::beg);
        fileContent.resize(fileSize);
        file.read(&fileContent[0], fileSize);
    }
}

void BoyerMooreSearch::setKeywords(const std::vector<std::string>& keywords) {
    this->keywords = keywords;
    keywordMatchCounts.resize(keywords.size());
    keywordSearchTime.resize(keywords.size());
}

void BoyerMooreSearch::searchAllKeywords() {
    for (size_t i = 0; i < keywords.size(); i++) {
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            taskQueue.emplace([this, i]() {
                int count = searchKeyword(keywords[i]);
                keywordMatchCounts[i] = count;
                keywordSearchTime[i] = std::chrono::high_resolution_clock::now();

                // 减少计数器
                {
                    std::lock_guard<std::mutex> lock(queueMutex);
                    pendingTasks--;
                }
                queueCondition.notify_all();
                });
            pendingTasks++; // 增加计数器
        }
        queueCondition.notify_one();
    }

    // 等待所有任务完成
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        queueCondition.wait(lock, [] { return pendingTasks == 0; });
    }
}

std::vector<int> BoyerMooreSearch::getKeywordMatchCounts() {
    return keywordMatchCounts;
}

std::vector<std::chrono::steady_clock::time_point> BoyerMooreSearch::getKewwordSearchTime() {
    return keywordSearchTime;
}

void BoyerMooreSearch::fileOutputKeywordMatchCounts(std::string outpath) {
    std::ofstream output_file(outpath);
    if (!output_file.is_open()) {
        throw std::runtime_error("Could not open output file");
    }
    for (int i = 0; i < keywords.size(); i++) {
        output_file << i + 1 << ". " << keywords[i] 
            << ": count:" << keywordMatchCounts[i] 
            << "  time:" << std::chrono::duration<double>(keywordSearchTime[i] - begin_time).count() << " seconds.\n";
    }
}

int BoyerMooreSearch::searchKeyword(const std::string& keyword) {
    BoyerMoore bm(keyword);
    std::vector<int> results = bm.search(fileContent);
    return results.size();
}

void BoyerMooreSearch::initializeThreadPool(size_t poolSize) {
    stopThreads = false;
    for (size_t i = 0; i < poolSize; ++i) {
        threadPool.emplace_back(workerThread);
    }
}

void BoyerMooreSearch::destroyThreadPool() {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        stopThreads = true;
    }
    queueCondition.notify_all();
    for (std::thread& thread : threadPool) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    threadPool.clear();
}

void BoyerMooreSearch::workerThread() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            queueCondition.wait(lock, [] { return stopThreads || !taskQueue.empty(); });
            if (stopThreads && taskQueue.empty()) {
                return;
            }
            task = std::move(taskQueue.front());
            taskQueue.pop();
        }
        task();
    }
}
