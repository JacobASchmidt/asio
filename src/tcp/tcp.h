#pragma once
#include "asio.h"
int  tcp_create(int port);
void tcp_set_asio(struct asio *asio);

struct tcp_handle {
        void (*error)(int err);
        void (*func)(int fd, void *buffer, enum events events);
};
void tcp_handle_con(int fd, void *tcp_handle, enum events events);
