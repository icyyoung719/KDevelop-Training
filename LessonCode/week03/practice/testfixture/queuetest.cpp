#include "queuetest.h"


void queue_unit_test::QueueTest::SetUp()
{
    m_kqueueFirst.enterDataQueue(10);
    m_kqueueFirst.enterDataQueue(20);
    m_kqueueFirst.enterDataQueue(30);

    m_kqueueSecond.enterDataQueue(40);
    m_kqueueSecond.enterDataQueue(50);
    m_kqueueSecond.enterDataQueue(60);

    m_kqueueThird.enterDataQueue(70);
    m_kqueueThird.enterDataQueue(80);
    m_kqueueThird.enterDataQueue(90);
    std::cout << "SetUp()" << std::endl;
}

void queue_unit_test::QueueTest::TearDown()
{
}
