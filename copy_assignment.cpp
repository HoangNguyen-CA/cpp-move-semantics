#include <bits/stdc++.h>
#include "Resource.h"

int main()
{
    Resource r1(10);

    for (int i = 0; i < 10; ++i)
    {
        r1[i] = i;
    }

    Resource r2;
    r2 = r1;

    r1.print();

    return 0;
} // end main