# asio

asio is a minimalist asynchronous-io library. 
It gives the neccessary building blocks to build a single-threaded concurrent
server, though with few additions it can be made to be used in a multi-threaded
environment.

## Example of use
### Tcp Server
```c

int tcp_create(int port);
//create tcp connection

struct tcp_handle {
        void (*master_error)(int err);
        void (*failure_accepting)(int err);
        void (*failure_nonblocking)(int err);
        void (*func)(int fd, void *buffer, enum events events);
};
struct buffer {
        char *data;
        int len, cap;
};
void tcp_handle_accept(int master, void *_tcp_handle, enum events events)
{
        struct tcp_handle *handle = _tcp_handle;
        assert(!events & EPOLL_ERR);
        //handle the connection, master is able to accept, forward errors and
        //new fd as prescibed by tcp_handle
        int slave = accept(master);
        ...
        struct buffer *b = malloc(sizeof(b));
        b->cap = 256;
        b->data = malloc(b->cap);
        b->len = 0;

        struct continuation continuation;
        continuation.func = handle->func;
        continuation.data = b;

        asio_add(slave, continuation);
}

void log_error_and_exit(int err);

void read_and_close(int fd, void *data, enum events events);

int main()
{
        int master = tcp_create(8080);


        struct tcp_handle tcp_handle;
        tcp_handle.master_error = &log_error_and_exit;
        tcp_handle.failure_accepting = &log_error_and_exit;
        tcp_handle.failure_nonblocking = &log_error_and_exit;
        tcp_handle.func = &read_and_close;
        
        struct continuation continuation;
        continuation.func = &tcp_handle_con;
        continuation.data = &tcp_handle;

        int initial_cap = 0;
        struct asio asio;
        asio_init(&asio, initial_cap);

        asio_add(master, continuation);
        
        int timeout = -1;
        asio_run(timeout);
}

```