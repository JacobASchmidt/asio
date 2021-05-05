#pragma once
struct buffer {
        int   len, cap;
        char *data;
};

int buffer_read(struct buffer *buffer, int fd);

struct bufferout {
        int   idx, cap;
        char *data;
};
