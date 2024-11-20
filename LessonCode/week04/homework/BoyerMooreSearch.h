#ifndef BOYERMOORESEARCH_H
#define BOYERMOORESEARCH_H

#include <string>
#include <vector>
#include <thread>
#include <queue>
#include <atomic>
#include <mutex>
#include <functional>
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

    // ��̬�����������̳߳�
    static void initializeThreadPool(size_t poolSize);

    // ��̬�����������̳߳�
    static void destroyThreadPool();

private:
    // �����ؼ��ʵ�����
    int searchKeyword(const std::string& keyword);

    // �ļ�·��
    std::string filePath;

    // �ؼ����б�
    std::vector<std::string> keywords;

    // ƥ����
    std::vector<int> keywordMatchCounts;

    // ��̬��Ա���ļ�����
    static std::string fileContent;

    // ��̬��Ա���̳߳�
    static std::vector<std::thread> threadPool;
    static std::queue<std::function<void()>> taskQueue;
    static std::atomic<size_t> pendingTasks;
    static std::mutex queueMutex;
    static std::condition_variable queueCondition;
    static bool stopThreads;

    // �����������̳߳�����ַ�
    static void workerThread();
};

#endif // BOYERMOORESEARCH_H
