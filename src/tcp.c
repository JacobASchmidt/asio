#include "tcp.h"

int jasio_tcp_create(int port, int backlog)
{
	;
}

struct jasio_tcp jasio_tcp_serve(int fd, jasio_func func,
				 enum jasio_event events)
{
	;
}
int jasio_tcp_add(struct jasio *jasio, struct jasio_tcp tcp)
{
	;
}
