#include<iostream>
#include<fcntl.h>
#include "TestAhoCorasick.h"
#include "TestBoyerMoore.h"
//#include<sys/mman.h>

int main() {
	//test AhoCorasick Algorithm
	int result = test_AhoCorasick();
    if (result == 0) {
        std::cout << "AhoCorasick Test Passed!" << std::endl;
    }
    else {
        std::cout << "AhoCorasick Test Failed!" << std::endl;
    }

    //test BoyerMoore Algorithm
    result = test_BoyerMoore();
    if (result == 0) {
        std::cout << "BoyerMoore Test Passed!" << std::endl;
    }
    else {
         std::cout << "BoyerMoore Test Failed!" << std::endl;
    }

    return 0;
}
