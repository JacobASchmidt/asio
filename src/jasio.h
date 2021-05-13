#pragma once
#include "fdmap.h"

struct jasio {
	struct jasio_fdmap fdmap;
	int pollfd;
};

void jasio_create(struct jasio *jasio);
int jasio_add(struct jasio *jasio, int fd, enum jasio_event events,
	      jasio_func func, void *data);
int jasio_modify_events(struct jasio *jasio, int fd, enum jasio_event events);
void jasio_modify_continuation(struct jasio *jasio, int fd, jasio_func func,
			       void *data);
int jasio_remove(struct jasio *jasio, int fd);
void jasio_run(struct jasio *jasio);
