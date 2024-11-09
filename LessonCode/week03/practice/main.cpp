#include<iostream>
#include"circle.h"
#include<memory>

int main(){
    int r;
    std::cout << "输入半径:";
    std::cin >> r;
    auto my_circle = std::make_unique<Circle>(r);
    std::cout<<"圆的面积是:"<<my_circle->getArea()<<std::endl;
    std::cout<<"圆的周长是:"<<my_circle->getPerimeter()<<std::endl;
    system("pause");
    return 0;
}