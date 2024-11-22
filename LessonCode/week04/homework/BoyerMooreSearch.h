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

    // ���ùؼ����б�
    void setKeywords(const std::vector<std::string>& keywords);

    // �����������񲢷���ƥ����
    void searchAllKeywords();

    // ��ȡƥ����
    std::vector<int> getKeywordMatchCounts();

    std::vector<std::chrono::steady_clock::time_point> getKeywordSearchTime();

    // ���ƥ������Ŀ���ļ�
    void fileOutputKeywordMatchCounts(std::string outputPath);

    // ��̬�����������̳߳�
    static void initializeThreadPool(size_t poolSize);

    // ��̬�����������̳߳�
    static void destroyThreadPool();

private:
    // �����ؼ��ʵ�����
    int searchWord(const std::string& keyword);

    // �ļ�·��
    std::string file_path;

    // �ؼ����б�
    std::vector<std::string> keywords;

    // ƥ����
    std::vector<int> keyword_match_counts;

    // ÿ���ؼ��ʵ�����ʱ��
    std::vector<std::chrono::steady_clock::time_point> keyword_search_time;

    // ��̬��Ա���ļ�����
    static std::string file_content;

    // ��̬��Ա���̳߳�
    static std::chrono::steady_clock::time_point begin_time;
    static std::vector<std::thread> thread_pool;
    static std::queue<std::function<void()>> task_queue;
    static std::atomic<size_t> pending_tasks;    // ��ǰ�ж��ٸ��߳���������
    static std::mutex queue_mutex;
    static std::condition_variable queue_condition;
    static bool stop_threads;

    // �����������̳߳�����ַ�
    static void workerThread();
};

#endif // BOYERMOORESEARCH_H