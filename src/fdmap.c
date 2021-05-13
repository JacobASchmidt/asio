
#include "fdmap.h"
#include <stdlib.h>
#include <assert.h>

inline int bit_clz(int a)
{
	return __builtin_clz(a);
}

inline int bit_ceil(int fd)
{
	if (fd <= 1)
		return 1;

	return 1 << ((sizeof(int) * 8) - bit_clz(fd - 1));
}

void jasio_fdmap_create(struct jasio_fdmap *fdmap)
{
	fdmap->cap = 4;
	fdmap->continuations =
		malloc(sizeof(struct jasio_continuation) * fdmap->cap);
}

void jasio_fdmap_add(struct jasio_fdmap *fdmap, int fd,
		     struct jasio_continuation continuation)
{
	if (fd >= fdmap->cap) {
		fdmap->cap = bit_ceil((fdmap->cap + 1) | (fd + 1));
		fdmap->continuations =
			realloc(fdmap->continuations,
				fdmap->cap * sizeof(struct jasio_continuation));
	}
	fdmap->continuations[fd] = continuation;
}

struct jasio_continuation jasio_fdmap_get(struct jasio_fdmap *fdmap, int fd)
{
	assert(fd < fdmap->cap);
	return fdmap->continuations[fd];
}
void jasio_fdmap_set(struct jasio_fdmap *fdmap, int fd,
		     struct jasio_continuation continuation)
{
	assert(fd < fdmap->cap);
	fdmap->continuations[fd] = continuation;
}

void jasio_fdmap_destroy(struct jasio_fdmap *fdmap)
{
	free(fdmap->continuations);
}
