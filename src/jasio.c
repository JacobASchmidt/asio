
#include "jasio.h"
#include <stdlib.h>
#include <sys/epoll.h>
#include <errno.h>
#include <assert.h>
#include <stdbool.h>

void jasio_create(struct jasio *jasio)
{
	jasio_fdmap_create(&jasio->fdmap);
	jasio->pollfd = epoll_create1(0);
}

int jasio_add(struct jasio *jasio, int fd, enum jasio_event events,
	      jasio_func func, void *data)
{
	struct epoll_event e;
	e.data.fd = fd;
	e.events = (int)events;
	jasio_fdmap_add(&jasio->fdmap, fd,
			(struct jasio_continuation){
				.data = data,
				.func = func,
			});

	return epoll_ctl(jasio->pollfd, EPOLL_CTL_ADD, fd, &e);
}

int jasio_modify_events(struct jasio *jasio, int fd, enum jasio_event events)
{
	struct epoll_event e;
	e.data.fd = fd;
	e.events = (int)events;
	return epoll_ctl(jasio->pollfd, EPOLL_CTL_MOD, fd, &e);
}
void jasio_modify_continuation(struct jasio *jasio, int fd, jasio_func func,
			       void *data)
{
	jasio_fdmap_set(&jasio->fdmap, fd,
			(struct jasio_continuation){
				.data = data,
				.func = func,
			});
}

int jasio_remove(struct jasio *jasio, int fd)
{
	return epoll_ctl(jasio->pollfd, EPOLL_CTL_DEL, fd, NULL);
}
#define TIMEOUT -1
void jasio_run(struct jasio *jasio)
{
	struct epoll_event *events = NULL;
	int cap = 0;

	struct jasio_continuation continuation;
	for (;;) {
		if (jasio->fdmap.cap > cap) {
			cap = jasio->fdmap.cap;
			events = realloc(events,
					 cap * sizeof(struct epoll_event));
		}
		int n = epoll_wait(jasio->pollfd, events, cap, TIMEOUT);
		if (n < 0) {
			assert(errno == EAGAIN || errno == EINTR);
			continue;
		}
		for (int i = 0; i < n; ++i) {
			int fd = events[i].data.fd;
			continuation = jasio_fdmap_get(&jasio->fdmap, fd);
			continuation.func(fd, continuation.data,
					  (enum jasio_event)events[i].events);
		}
	}
	assert(false);
}
