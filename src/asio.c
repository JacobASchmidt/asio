#include "asio.h"
#include <stdlib.h>
#include <sys/epoll.h>
#include <errno.h>
#include <assert.h>

void jasio_create(struct jasio *asio, int cap)
{
        jasio_fdmap_create(&asio->fdmap, cap);
        asio->poll_fd = epoll_create1(0);
}

int jasio_add(struct jasio *asio, int fd, enum jasio_events events,
              struct jasio_continuation continuation)
{
        struct epoll_event e;
        e.data.fd = fd;
        e.events  = (int) events;
        jasio_fdmap_add(&asio->fdmap, fd, continuation);

        return epoll_ctl(asio->poll_fd, EPOLL_CTL_ADD, fd, &e);
}

int jasio_modify_events(struct jasio *asio, int fd, enum jasio_events events)
{
        struct epoll_event e;
        e.data.fd = fd;
        e.events  = (int) events;
        return epoll_ctl(asio->poll_fd, EPOLL_CTL_MOD, fd, &e);
}
void jasio_modify_continuation(struct jasio *asio, int fd,
                               struct jasio_continuation continuation)
{
        jasio_fdmap_set(&asio->fdmap, fd, continuation);
}

int jasio_remove(struct jasio *asio, int fd)
{
        return epoll_ctl(asio->poll_fd, EPOLL_CTL_DEL, fd, NULL);
}

void jasio_run(struct jasio *asio, int timeout)
{
        struct epoll_event *events = NULL;
        int                 cap    = 0;

        struct jasio_continuation continuation;
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
                        continuation = jasio_fdmap_get(&asio->fdmap, fd);
                        continuation.func(fd, continuation.data,
                                          (enum jasio_events) events[i].events);
                }
        }
}
