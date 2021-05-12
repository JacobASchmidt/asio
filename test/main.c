#include "../src/jasio.h"
#include <unistd.h>

void read_(int fd, void *data, enum jasio_events events)
{
	char buff[256];
	int n = read(fd, buff, sizeof(buff));
	buff[n] = '\n';
	write(1, buff, n + 1);
}

int main(void)
{
	struct jasio jasio;
	jasio_create(&jasio, 1);
	struct jasio_continuation continuation;
	continuation.func = read_;
	jasio_add(&jasio, 0, JASIO_IN, continuation);
	jasio_run(&jasio, -1);
}