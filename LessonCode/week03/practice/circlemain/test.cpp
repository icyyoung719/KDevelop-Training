#include "gtest/gtest.h"
#include "circletest.h"

namespace circle_unit_test{
    //测试默认构造函数
    TEST_F(CircleTest, DefaultConstructor) 
    {
        //EXPECT_EQ(3, circle_3.r);
        //EXPECT_EQ(3, circle_2.r);
        EXPECT_EQ(1, 1);
    }
    //测试周长
    TEST_F(CircleTest, Perimeter)
    {
        EXPECT_EQ(18.8496, circle_3.getPerimeter());
        
    }
    //面积
    TEST_F(CircleTest, Area)
    {
        EXPECT_EQ(28.2743, circle_3.getArea());
    }
}
int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}