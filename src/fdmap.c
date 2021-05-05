#include "fdmap.h"
#include "bit.h"
#include <stdlib.h>
#include <assert.h>

void fdmap_create(struct fdmap *fdmap, int cap)
{
        fdmap->data = malloc(sizeof(struct continuation) * cap);
        fdmap->cap  = cap;
}

void fdmap_add(struct fdmap *fdmap, int fd, struct continuation continuation)
{
        if (fd >= fdmap->cap) {
                fdmap->cap  = bit_ceil((fdmap->cap + 1) | (fd + 1));
                fdmap->data = realloc(fdmap->data,
                                      fdmap->cap * sizeof(struct continuation));
        }
        fdmap->data[fd] = continuation;
}

struct continuation fdmap_get(struct fdmap *fdmap, int fd)
{
        assert(fd < fdmap->cap);
        return fdmap->data[fd];
}
void fdmap_set(struct fdmap *fdmap, int fd, struct continuation continuation)
{
        assert(fd < fdmap->cap);
        fdmap->data[fd] = continuation;
}

void fdmap_destroy(struct fdmap *fdmap)
{
        free(fdmap->data);
}
