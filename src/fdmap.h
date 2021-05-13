#pragma once
#include <sys/epoll.h>

enum jasio_event {
	JASIO_EVENT_IN = 0x001,
	JASIO_EVENT_OUT = 0x004,
	JASIO_EVENT_ERR = 0x008,
};

typedef void (*jasio_func)(int fd, void *data, enum jasio_event event);
struct jasio_continuation {
	jasio_func func;
	void *data;
};

struct jasio_fdmap {
	struct jasio_continuation *continuations;
	int cap;
};

void jasio_fdmap_create(struct jasio_fdmap *jasio);
void jasio_fdmap_add(struct jasio_fdmap *jasio, int fd,
		     struct jasio_continuation continuation);
struct jasio_continuation jasio_fdmap_get(struct jasio_fdmap *jasio, int fd);
void jasio_fdmap_set(struct jasio_fdmap *jasio, int fd,
		     struct jasio_continuation continuation);

void jasio_fdmap_destroy(struct jasio_fdmap *jasio);
