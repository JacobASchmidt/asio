#include "tcp.h"
#include "buffer.h"
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/fcntl.h>
#include <stdlib.h>

struct asio *asio;

void tcp_set_asio(struct asio *_asio)
{
        asio = _asio;
}

int tcp_create(int port)
{
        int fd, err;
        fd = socket(AF_INET, SOCK_STREAM, 0);
        if (fd < 0)
                return fd;
        struct sockaddr_in sin;
        memset(&sin, 0, sizeof(sin));
        sin.sin_family      = AF_INET;
        sin.sin_addr.s_addr = htonl(INADDR_ANY);
        sin.sin_port        = htons(port);

        if ((err = bind(fd, (struct sockaddr *) &sin, sizeof(sin))) < 0)
                return err;
        if ((err = listen(fd, SOMAXCONN)) < 0)
                return err;
        int flags = fcntl(fd, F_GETFL, 0);
        if ((err = fcntl(fd, F_SETFL, flags | O_NONBLOCK)) < 0)
                return err;
        return fd;
}

void tcp_handle_con(int fd, void *_tcp_handle, enum events events)
{
        struct tcp_handle *tcp_handle = _tcp_handle;
        if (events == EPOLLERR) {
                (*tcp_handle->error)(errno);
                return;
        }
        int slave;

        struct sockaddr_in fsin;
        memset(&fsin, 0, sizeof(fsin));
        fsin.sin_family = AF_INET;
        fsin.sin_port   = htons(13);
        socklen_t len   = sizeof(fsin);
        slave           = accept(fd, (struct sockaddr *) &fsin, &len);
        if (slave < 0) {
                if (errno != EINTR)
                        (*tcp_handle->error)(errno);
                return;
        }
        int flags = fcntl(fd, F_GETFL, 0);
        int err   = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        if (err) {
                //these shouldn't be handled as lasily
                (*tcp_handle->error)(errno);
                return;
        }

        struct buffer *buff = malloc(sizeof(struct buffer));
        buff->len           = 0;
        buff->cap           = 256;
        buff->data          = malloc(buff->cap);

        struct continuation continuation;
        continuation.func = tcp_handle->func;
        continuation.data = buff;

        asio_add(asio, slave, EPOLLIN, continuation);
}
