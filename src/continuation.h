#pragma once

enum events {
        ASIO_IN  = 0x001,
        ASIO_OUT = 0x004,
        ASIO_ERR = 0x008,
};

struct continuation {
        void (*func)(int fd, void *data, enum events events);
        void *data;
};
