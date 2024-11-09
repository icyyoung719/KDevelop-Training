#include <iostream>
#include "gtest/gtest.h"

int  add(int a, int b)
{
    return a + b;
}

int stringCmp(const char* str1, const char* str2)
{
    return strcmp(str1, str2);
}

//testTestSuiteName , testcasename
TEST(AddTest, BinaryComparison)
{
    EXPECT_EQ(add(1, 2), add(2, 1));  //ture
    EXPECT_GT(add(1, 2), 1);  //ture
    EXPECT_NE(std::string("a"), std::string("b"));  //true
    EXPECT_EQ(std::string("a"), std::string("a"));  //true
}

TEST(StringCmpTest, CmpStringResult)
{
    std::string strCodezh = "CoderZh";
    const char* pstrCodezh = "CodeZh";
    EXPECT_STREQ("CoderZh", pstrCodezh);
    EXPECT_STRNE("CoderZh", pstrCodezh);
    EXPECT_STRCASEEQ("coderzh", pstrCodezh); //忽略大小写

    EXPECT_EQ("CoderZh", strCodezh);
}

TEST(DoubleTest, DoubleAssert)
{
    ASSERT_FLOAT_EQ(1.0000001f, 1.0f);
    ASSERT_NEAR(1.009f, 1.0f, 0.01f);

    ADD_FAILURE() << "Sorry";   //继续向下执行

    //FAIL();  //不继续向下执行

    SUCCEED();
}