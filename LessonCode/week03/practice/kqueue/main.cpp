#include <iostream>
#include "kqueue.h"

int main()
{
    KQueue que;
    for (int i = 1; i <= 10; i++)
    {
        que.enterDataQueue(i);
    }

    while (!que.isEmptyQueue())
    {
        std::cout << que.deleteDataQueue() << " ";
    }

    std::cout << std::endl;
    return 0;
}