#include "circle.h"

double Circle::getArea() {
    return 3.14 * r * r;
}
double Circle::getPerimeter() {
    return 2 * 3.14 * r;
}
int Circle::setR(int input_r){
    if (input_r >= 0){
        r = input_r;
    }
    return r;
} 