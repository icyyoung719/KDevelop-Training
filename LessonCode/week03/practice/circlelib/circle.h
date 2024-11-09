#include<iostream>
class Circle
{
public:
    int r;

public:
    Circle(): r(0) {}
    Circle(int input_r): r(input_r) {}
    ~Circle(){
        std::cout << "Circle destructor" << std::endl;
    }
    int setR(int input_r);
    //周长
    double getPerimeter();
    //面积
    double getArea();
};


