#include "circletest.h"

void circle_unit_test::CircleTest::SetUp(){
    circle_1.setR(1);
    circle_2.setR(2);
    circle_3.setR(3);
    std::cout << "SetUp()" << std::endl;
}
void circle_unit_test::CircleTest::TearDown(){
    std::cout << "TearDown()" << std::endl;
}