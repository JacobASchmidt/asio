#pragma once
#include "continuation.h"

struct fdmap {
        struct continuation *data;
        int                  cap;
};

void fdmap_create(struct fdmap *fdmap, int cap);
void fdmap_add(struct fdmap *fdmap, int fd, struct continuation continuation);

struct continuation fdmap_get(struct fdmap *fdmap, int fd);
void fdmap_set(struct fdmap *fdmap, int fd, struct continuation continuation);

void fdmap_destroy(struct fdmap *fdmap);
