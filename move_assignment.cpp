#include <iostream>
#include "Resource.h"

int main()
{
    Resource r1(10);

    for (int i = 0; i < 10; ++i)
    {
        r1[i] = i;
    }

    Resource r2;
    r2 = std::move(r1);

    r1.print();
    r2.print();

    return 0;
} // end main