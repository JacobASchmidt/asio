#include "asio.h"
#include <stdlib.h>
#include <sys/epoll.h>
#include <errno.h>
#include <assert.h>

void asio_create(struct asio *asio, int cap)
{
        fdmap_create(&asio->fdmap, cap);
        asio->poll_fd = epoll_create1(0);
}

int asio_add(struct asio *asio, int fd, enum events events,
             struct continuation continuation)
{
        struct epoll_event e;
        e.data.fd = fd;
        e.events  = (int) events;
        fdmap_add(&asio->fdmap, fd, continuation);

        return epoll_ctl(asio->poll_fd, EPOLL_CTL_ADD, fd, &e);
}

int asio_modify_events(struct asio *asio, int fd, enum events events)
{
        struct epoll_event e;
        e.data.fd = fd;
        e.events  = (int) events;
        return epoll_ctl(asio->poll_fd, EPOLL_CTL_MOD, fd, &e);
}
void asio_modify_continuation(struct asio *asio, int fd,
                              struct continuation continuation)
{
        fdmap_set(&asio->fdmap, fd, continuation);
}

int asio_remove(struct asio *asio, int fd)
{
        return epoll_ctl(asio->poll_fd, EPOLL_CTL_DEL, fd, NULL);
}

void asio_run(struct asio *asio, int timeout)
{
        struct epoll_event *events = NULL;
        int                 cap    = 0;

        struct continuation continuation;
        for (;;) {
                if (asio->fdmap.cap > cap) {
                        cap    = asio->fdmap.cap;
                        events = realloc(events,
                                         cap * sizeof(struct epoll_event));
                }
                int n = epoll_wait(asio->poll_fd, events, cap, timeout);
                if (n < 0) {
                        assert(errno == EAGAIN || errno == EINTR);
                        continue;
                }
                for (int i = 0; i < n; ++i) {
                        int fd       = events[i].data.fd;
                        continuation = fdmap_get(&asio->fdmap, fd);
                        continuation.func(fd, continuation.data,
                                          (enum events) events[i].events);
                }
        }
}