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

    std::vector<std::chrono::steady_clock::time_point> getKewwordSearchTime();

    // 输出匹配结果到目标文件
    void fileOutputKeywordMatchCounts(std::string outpath);

    // 静态方法：启动线程池
    static void initializeThreadPool(size_t poolSize);

    // 静态方法：销毁线程池
    static void destroyThreadPool();

private:
    // 单个关键词的搜索
    int searchKeyword(const std::string& keyword);

    // 文件路径
    std::string filePath;

    // 关键词列表
    std::vector<std::string> keywords;

    // 匹配结果
    std::vector<int> keywordMatchCounts;

    // 每个关键词的搜索时间
    std::vector<std::chrono::steady_clock::time_point> keywordSearchTime;

    // 静态成员：文件内容
    static std::string fileContent;

    // 静态成员：线程池
    static std::chrono::steady_clock::time_point begin_time;
    static std::vector<std::thread> threadPool;
    static std::queue<std::function<void()>> taskQueue;
    static std::atomic<size_t> pendingTasks;    //当前有多少个线程正在运行
    static std::mutex queueMutex;
    static std::condition_variable queueCondition;
    static bool stopThreads;

    // 辅助方法：线程池任务分发
    static void workerThread();
};

#endif // BOYERMOORESEARCH_H
