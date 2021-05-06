#pragma once

enum jasio_events {
        JASIO_IN  = 0x001,
        JASIO_OUT = 0x004,
        JASIO_ERR = 0x008,
};

struct jasio_continuation {
        void (*func)(int fd, void *data, enum jasio_events events);
        void *data;
};
