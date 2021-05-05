#include "buffer.h"
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

int maybe_error()
{
        if (errno == EWOULDBLOCK)
                return 0;
        return -1;
};
int buffer_read(struct buffer *buffer, int fd)
{
        const int old_len = buffer->len;
        int       max_read, n;

        max_read = buffer->cap - buffer->len;
        n        = read(fd, buffer->data, buffer->cap - buffer->len);
        if (n < 0)
                return maybe_error();

        buffer->len += n;

        while (n == max_read) {
                buffer->cap *= 2;
                buffer->data = realloc(buffer->data, buffer->cap);
                max_read     = buffer->cap - buffer->len;
                n = read(fd, buffer->data, buffer->cap - buffer->len);
                if (n < 0)
                        return maybe_error();

                buffer->len += n;
        }
        return buffer->len - old_len;
}