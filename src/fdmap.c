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
