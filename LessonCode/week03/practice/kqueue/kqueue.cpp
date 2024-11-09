#include "kqueue.h"

KQueue::KQueue(): m_head(nullptr),m_tail(nullptr),m_size(0)
{

}

KQueue::~KQueue()
{
    clear();
}

void KQueue::clear()
{
    //删除所有结点
    if (m_size > 0)
    {
        KQueueNode* deleteNode = m_head;
        KQueueNode* temp = deleteNode->next();
        while (temp != nullptr)
        {
            delete deleteNode;
            deleteNode = temp;
            if (!deleteNode)  //delete为nullptr结束循环
                break;
            temp = deleteNode->next();
        }
    }
    //2.重置成员变量
    m_head = m_tail = nullptr;
    m_size = 0;
}

int KQueue::getSize() const
{
    return m_size;
}

int KQueue::isEmptyQueue() const
{
    return m_size == 0 ? 1 : 0;
}

KQueueNode* KQueue::getHead() const
{
    return m_head;
}

KQueueNode* KQueue::getTail() const
{
    return m_tail;
}

void KQueue::enterDataQueue(const int& data)
{
    KQueueNode* newNode = new KQueueNode(data);

    //若是空队列
    if (isEmptyQueue())
    {
        m_head = m_tail = newNode;
    }
    else
    {
        m_tail->setNextAddr(newNode);
        m_tail = newNode;
    }

    m_size++;
}

int KQueue::deleteDataQueue()
{
    //若是为空
    if (isEmptyQueue())
    {
        return -1;
    }

    KQueueNode* deleteNode = m_head;
    m_head = deleteNode->next();

    int data = deleteNode->getData();
    m_size--;

    if (m_size == 0)
        m_tail = nullptr;

    return data;
}
