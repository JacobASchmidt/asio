#pragma once
#include "jasio.h"

#define JASIO_TIME_NANOSECOND 1LL
#define JASIO_TIME_MICROSECOND (1000LL * JASIO_TIME_NANOSECOND)
#define JASIO_TIME_MILLISECOND (1000LL * JASIO_TIME_MICROSECOND)
#define JASIO_TIME_SECOND (1000LL * JASIO_TIME_MILLISECOND)
#define JASIO_TIME_MINUTE (60LL * JASIO_TIME_SECOND)
#define JASIO_TIME_HOUR (60LL * JASIO_TIME_MINUTE)
#define JASIO_TIME_DAY (24LL * JASIO_TIME_HOUR)
#define JASIO_TIME_WEEK (7LL * JASIO_TIME_DAY)
#define JASIO_TIME_IMMEDIATELY JASIO_TIME_NANOSECOND

enum jasio_time_status {
	JASIO_TIME_STATUS_OK = JASIO_EVENT_IN,
	JASIO_TIME_STATUS_ERR = JASIO_EVENT_ERR,
};

struct jasio_time;

typedef void (*jasio_time_func)(struct jasio_time jasio_time, void *data,
				enum jasio_time_status status,
				long long n_expirations);
struct jasio_time_continuation {
	jasio_time_func func;
	void *data;
};
struct jasio_time {
	int fd;
	struct jasio_continuation continuation;
};

struct jasio_time jasio_time_after(long long nsec, jasio_time_func func,
				   void *data);

struct jasio_time jasio_time_every(long long nsec, jasio_time_func func,
				   void *data);

struct jasio_time jasio_time_every_after(long long start, long long every,
					 jasio_time_func func, void *data);
void jasio_time_add(struct jasio *jasio, struct jasio_time);

void jasio_time_close(struct jasio_time jasio_time);
