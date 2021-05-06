#pragma once

enum jasio_events {
        ASIO_IN  = 0x001,
        ASIO_OUT = 0x004,
        ASIO_ERR = 0x008,
};

struct jasio_continuation {
        void (*func)(int fd, void *data, enum jasio_events events);
        void *data;
};
