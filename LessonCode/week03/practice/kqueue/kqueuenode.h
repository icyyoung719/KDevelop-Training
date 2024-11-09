#ifndef __KQUEUENODE_H__
#define __KQUEUENODE_H__

#include <iostream>
#include <string>

class KQueueNode
{
public:

    explicit KQueueNode(const int& data);

    //获得当前节点的数据
    int getData() const;

    //获得下一个结点的地址
    KQueueNode* next() const;

    //设置当前结点next域的地址
    void setNextAddr(KQueueNode *temp);

    // 这里禁用默认的赋值操作和拷贝构造
    KQueueNode& operator=(const KQueueNode) = delete;
    KQueueNode(const KQueueNode&) = delete;

private:
    int m_data;  //存放数据
    KQueueNode* m_next;  //存放下一个节点的地址
};

#endif