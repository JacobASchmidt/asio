#include "bit.h"
int bit_clz(int a)
{
        return __builtin_clz(a);
}

int bit_ceil(int fd)
{
        if (fd <= 1)
                return 1;

        return 1 << ((sizeof(int) * 8) - bit_clz(fd - 1));
}
