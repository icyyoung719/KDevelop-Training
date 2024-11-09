#ifndef __KQueue_H__
#define __KQueue_H__

#include <iostream>
#include <string>
#include "kqueuenode.h"

class KQueue
{
public:
    explicit KQueue();

    ~KQueue();

    //删除队列所有结点
    void clear();
    
    //获取元素个数
    int getSize() const;

    //判断队列是否为空
    int isEmptyQueue()const;

    //获得队列头部地址
    KQueueNode * getHead() const;
    KQueueNode * getTail() const;

    //数据入队
    void enterDataQueue(const int& data);

    //数据出队
    int deleteDataQueue();

private:
    KQueueNode  *m_head; //队列头结点地址
    KQueueNode  *m_tail;  //队列尾结点地址

    int m_size; //队列元素个数
};

#endif