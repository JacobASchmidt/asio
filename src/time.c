#include "time.h"

#include <sys/timerfd.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define NANOSECONDS_PER_SECOND 1000000000LL

void jasio_time__continuation_func(int fd, void *data, enum jasio_event events)
{
	struct jasio_time_continuation *time_continuation = data;
	struct jasio_time jasio_time;
	jasio_time.fd = fd;
	jasio_time.continuation.data = data;
	jasio_time.continuation.func = jasio_time__continuation_func;

	long long n_expirations = -1;
	if (events & JASIO_EVENT_ERR) {
		(*time_continuation->func)(jasio_time, time_continuation->data,
					   (enum jasio_time_status)events,
					   n_expirations);
		return;
	}

	int ok = read(fd, &n_expirations, sizeof(n_expirations));
	if (ok != sizeof(n_expirations)) {
		(*time_continuation->func)(jasio_time, time_continuation->data,
					   JASIO_TIME_STATUS_ERR, -1);
		return;
	}
	(*time_continuation->func)(jasio_time, time_continuation->data,
				   (enum jasio_time_status)events,
				   n_expirations);
}

struct jasio_time jasio_time_every_after(long long start, long long every,
					 jasio_time_func func, void *data)
{
	struct itimerspec itimerspec;
	itimerspec.it_value.tv_sec = start / NANOSECONDS_PER_SECOND;
	itimerspec.it_value.tv_nsec = start % NANOSECONDS_PER_SECOND;
	itimerspec.it_interval.tv_sec = every / NANOSECONDS_PER_SECOND;
	itimerspec.it_interval.tv_nsec = every % NANOSECONDS_PER_SECOND;

	struct jasio_time_continuation *time_continuation =
		malloc(sizeof(*time_continuation));
	time_continuation->data = data;
	time_continuation->func = func;

	struct jasio_continuation jasio_continuation;
	jasio_continuation.data = time_continuation;
	jasio_continuation.func = jasio_time__continuation_func;

	struct jasio_time jasio_time;
	jasio_time.fd = timerfd_create(CLOCK_BOOTTIME, TFD_NONBLOCK);
	jasio_time.continuation = jasio_continuation;

	timerfd_settime(jasio_time.fd, 0, &itimerspec, NULL);

	return jasio_time;
}

struct jasio_time jasio_time_after(long long nsec, jasio_time_func func,
				   void *data)
{
	return jasio_time_every_after(nsec, 0, func, data);
}

struct jasio_time jasio_time_every(long long nsec, jasio_time_func func,
				   void *data)
{
	return jasio_time_every_after(nsec, nsec, func, data);
}

void jasio_time_add(struct jasio *jasio, struct jasio_time jasio_time)
{
	jasio_add(jasio, jasio_time.fd, JASIO_EVENT_IN,
		  jasio_time.continuation.func, jasio_time.continuation.data);
}

void jasio_time_close(struct jasio_time jasio_time)
{
	close(jasio_time.fd);
	free(jasio_time.continuation.data);
}
