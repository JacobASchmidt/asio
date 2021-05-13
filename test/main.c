#include "jasio.h"
#include "time.h"
#include "tcp.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

struct jasio jasio;

void send_response(int fd, void *data, enum jasio_event events);

#define MAX_SIZE 256
#define LEN_PREFIX (sizeof("you sent: ") - 1)

void read_request(int fd, void *data, enum jasio_event events)
{
	static union {
		char buff[MAX_SIZE];
		struct {
			char you_sent[LEN_PREFIX];
			char rest[MAX_SIZE - LEN_PREFIX];
		};
	} buff = { .buff = "you sent: " };

	if (events & JASIO_EVENT_ERR) {
		close(fd);
		return;
	}
	int len = read(fd, buff.rest, sizeof(buff.rest) - 1);
	if (len == -1) {
		close(fd);
		return;
	}
	buff.rest[len++] = '\0';
	int total = LEN_PREFIX + len;
	char *str = malloc(total);
	memcpy(str, buff.buff, total);
	jasio_modify_continuation(&jasio, fd, send_response, str);
	jasio_modify_events(&jasio, fd, JASIO_EVENT_OUT);
}

void send_response(int fd, void *data, enum jasio_event events)
{
	char *str = data;
	int len = strlen(str);
	write(fd, str, len);
	free(str);
	close(fd);
}

int main()
{
	jasio_create(&jasio);
	int fd = jasio_tcp_create(8080, 40);
	jasio_tcp_add(&jasio,
		      jasio_tcp_serve(fd, read_request, JASIO_EVENT_IN));
	jasio_run(&jasio);
}