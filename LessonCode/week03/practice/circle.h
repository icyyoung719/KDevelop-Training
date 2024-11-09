#include<iostream>
class Circle
{
private:
    int r;

public:
    Circle(int input_r): r(input_r) {}
    ~Circle(){
        std::cout << "Circle destructor" << std::endl;
    }
    //周长
    double getPerimeter();
    //面积
    double getArea();
};


