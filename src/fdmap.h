#pragma once
#include "continuation.h"

struct jasio_fdmap {
        struct jasio_continuation *data;
        int                        cap;
};

void jasio_fdmap_create(struct jasio_fdmap *fdmap, int cap);
void jasio_fdmap_add(struct jasio_fdmap *fdmap, int fd,
                     struct jasio_continuation continuation);

struct jasio_continuation jasio_fdmap_get(struct jasio_fdmap *fdmap, int fd);
void                      jasio_fdmap_set(struct jasio_fdmap *fdmap, int fd,
                                          struct jasio_continuation continuation);

void jasio_fdmap_destroy(struct jasio_fdmap *fdmap);
