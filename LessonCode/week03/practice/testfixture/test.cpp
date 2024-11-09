#include "gtest/gtest.h"
#include "queuetest.h"

namespace queue_unit_test 
{
    //测试默认构造函数
    TEST_F(QueueTest, DefaultConstructor) 
    {
        EXPECT_EQ(3, m_kqueueFirst.getSize());
    }

    //测试deleteDataQueue
    TEST_F(QueueTest, deleteDataQueue)
    {
        EXPECT_EQ(10, m_kqueueFirst.deleteDataQueue());
        EXPECT_EQ(2, m_kqueueFirst.getSize());

        EXPECT_EQ(40, m_kqueueSecond.deleteDataQueue());
        EXPECT_EQ(50, m_kqueueSecond.deleteDataQueue());
        EXPECT_EQ(1, m_kqueueSecond.getSize());

        EXPECT_EQ(70, m_kqueueThird.deleteDataQueue());
        EXPECT_EQ(80, m_kqueueThird.deleteDataQueue());
        EXPECT_EQ(90, m_kqueueThird.deleteDataQueue());
        EXPECT_EQ(0, m_kqueueThird.getSize());
    }

    //测试clear()函数
    TEST_F(QueueTest, clear)
    {
        m_kqueueFirst.clear();
        EXPECT_EQ(0, m_kqueueFirst.getSize());
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}