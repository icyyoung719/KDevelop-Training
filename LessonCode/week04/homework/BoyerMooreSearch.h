#ifndef BOYERMOORESEARCH_H
#define BOYERMOORESEARCH_H

#include <string>
#include <vector>
#include <thread>
#include <queue>
#include <atomic>
#include <mutex>
#include <functional>
#include <chrono>
#include <condition_variable>
#include "BoyerMoore.h"

class BoyerMooreSearch {
public:
    BoyerMooreSearch(const std::string& filePath);

    // 设置关键词列表
    void setKeywords(const std::vector<std::string>& keywords);

    // 启动搜索任务并返回匹配结果
    void searchAllKeywords();

    // 获取匹配结果
    std::vector<int> getKeywordMatchCounts();

    std::vector<std::chrono::steady_clock::time_point> getKeywordSearchTime();

    // 输出匹配结果到目标文件
    void fileOutputKeywordMatchCounts(std::string outputPath);

    // 静态方法：启动线程池
    static void initializeThreadPool(size_t poolSize);

    // 静态方法：销毁线程池
    static void destroyThreadPool();

private:
    // 单个关键词的搜索
    int searchWord(const std::string& keyword);

    // 文件路径
    std::string file_path;

    // 关键词列表
    std::vector<std::string> keywords;

    // 匹配结果
    std::vector<int> keyword_match_counts;

    // 每个关键词的搜索时间
    std::vector<std::chrono::steady_clock::time_point> keyword_search_time;

    // 静态成员：文件内容
    static std::string file_content;

    // 静态成员：线程池
    static std::chrono::steady_clock::time_point begin_time;
    static std::vector<std::thread> thread_pool;
    static std::queue<std::function<void()>> task_queue;
    static std::atomic<size_t> pending_tasks;    // 当前有多少个线程正在运行
    static std::mutex queue_mutex;
    static std::condition_variable queue_condition;
    static bool stop_threads;

    // 辅助方法：线程池任务分发
    static void workerThread();
};

#endif // BOYERMOORESEARCH_H