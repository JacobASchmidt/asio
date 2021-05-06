/*
MIT License

Copyright (c) 2021 JacobASchmidt

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include "fdmap.h"
#include "bit.h"
#include <stdlib.h>
#include <assert.h>

void jasio_fdmap_create(struct jasio_fdmap *fdmap, int cap)
{
        fdmap->data = malloc(sizeof(struct jasio_continuation) * cap);
        fdmap->cap  = cap;
}

void jasio_fdmap_add(struct jasio_fdmap *fdmap, int fd,
                     struct jasio_continuation continuation)
{
        if (fd >= fdmap->cap) {
                fdmap->cap = bit_ceil((fdmap->cap + 1) | (fd + 1));
                fdmap->data =
                        realloc(fdmap->data,
                                fdmap->cap * sizeof(struct jasio_continuation));
        }
        fdmap->data[fd] = continuation;
}

struct jasio_continuation jasio_fdmap_get(struct jasio_fdmap *fdmap, int fd)
{
        assert(fd < fdmap->cap);
        return fdmap->data[fd];
}
void jasio_fdmap_set(struct jasio_fdmap *fdmap, int fd,
                     struct jasio_continuation continuation)
{
        assert(fd < fdmap->cap);
        fdmap->data[fd] = continuation;
}

void jasio_fdmap_destroy(struct jasio_fdmap *fdmap)
{
        free(fdmap->data);
}
