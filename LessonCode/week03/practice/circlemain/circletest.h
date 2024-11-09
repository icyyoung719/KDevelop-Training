#ifndef __CIRCLETEST_H__
#define __CIRCLETEST_H__

#include <iostream>
#include "gtest/gtest.h"
#include "circle.h"
//在这个例子中，我们使用Gtest中更高级一点的特性，叫test fixture,

//test fixture用来放置对象和函数，并共享给所有的测试用例在一个测试cpp文件中
// 使用test fixture能够避免测试代码重复，特别是哪些每个测试都需要用到的初始化和清除操作

//测试从代码共享的意义上共享的是测试装置，而不是数据共享。
//每个测试都有自己的最新副本装置。 您不能期望一次测试修改的数据是传递给另一个测试，这是不是好的用法。

namespace circle_unit_test
{
    class CircleTest : public testing::Test
    {
    protected:
        void SetUp() override;
        void TearDown() override;

        Circle  circle_1;
        Circle  circle_2;
        Circle  circle_3;
    };
}

#endif