#pragma once
#include "fdmap.h"

struct jasio {
        int                poll_fd;
        struct jasio_fdmap fdmap;
};

void jasio_create(struct jasio *asio, int cap);

int jasio_add(struct jasio *asio, int fd, enum jasio_events events,
              struct jasio_continuation continuation);

int  jasio_modify_events(struct jasio *asio, int fd, enum jasio_events events);
void jasio_modify_continuation(struct jasio *asio, int fd,
                               struct jasio_continuation continuation);

int jasio_remove(struct jasio *asio, int fd);
int jasio_destroy(struct jasio *asio);

void jasio_run(struct jasio *asio, int timeout);
