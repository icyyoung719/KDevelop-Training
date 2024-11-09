#include "kqueuenode.h"

KQueueNode::KQueueNode(const int& data):m_data(data),m_next(nullptr)
{
    ;
}

int KQueueNode::getData() const
{
    return m_data;
}

KQueueNode* KQueueNode::next() const
{
    return m_next;
}

void KQueueNode::setNextAddr(KQueueNode* temp) 
{
    m_next = temp;
}
