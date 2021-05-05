#pragma once
#include "fdmap.h"

struct asio {
        int          poll_fd;
        struct fdmap fdmap;
};

void asio_create(struct asio *asio, int cap);

int asio_add(struct asio *asio, int fd, enum events events,
             struct continuation continuation);

int  asio_modify_events(struct asio *asio, int fd, enum events events);
void asio_modify_continuation(struct asio *asio, int fd,
                              struct continuation continuation);

int asio_remove(struct asio *asio, int fd);
int asio_destroy(struct asio *asio);

void asio_run(struct asio *asio, int timeout);
