# asio

asio is a minimalist asynchronous-io library. 
It gives the neccessary building blocks to build a single-threaded concurrent
server, though with few additions it can be made to be used in a multi-threaded
environment.

# Example of use
# Tcp Connection
```

int tcp_create(int port)
{
        int fd, err;
        fd = socket(AF_INET, SOCK_STREAM, 0);
        if (fd < 0)
                return fd;
        struct sockaddr_in sin;
        memset(&sin, 0, sizeof(sin));
        sin.sin_family      = AF_INET;
        sin.sin_addr.s_addr = htonl(INADDR_ANY);
        sin.sin_port        = htons(port);

        if ((err = bind(fd, (struct sockaddr *) &sin, sizeof(sin))) < 0)
                return err;
        if ((err = listen(fd, SOMAXCONN)) < 0)
                return err;
        int flags = fcntl(fd, F_GETFL, 0);
        if ((err = fcntl(fd, F_SETFL, flags | O_NONBLOCK)) < 0)
                return err;
        return fd;
}



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
void tcp_handle_con(int fd, void *_tcp_handle, enum events events)
{
        struct tcp_handle *tcp_handle = _tcp_handle;
        if (events == EPOLLERR) {
                //these shouldn't be handled as lasily
                (*tcp_handle->master_error)(errno);
                return;
        }
        int slave;

        struct sockaddr_in fsin;
        memset(&fsin, 0, sizeof(fsin));
        fsin.sin_family = AF_INET;
        fsin.sin_port   = htons(13);
        socklen_t len   = sizeof(fsin);
        slave           = accept(fd, (struct sockaddr *) &fsin, &len);
        if (slave < 0) {
                if (errno != EINTR)
                        (*tcp_handle->failure_accepting)(errno);
                return;
        }
        int flags = fcntl(fd, F_GETFL, 0);
        int err   = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        if (err) {
                (*tcp_handle->failure_nonblocking)(errno);
                return;
        }

        struct buffer *buff = malloc(sizeof(struct buffer));
        buff->len           = 0;
        buff->cap           = 256;
        buff->data          = malloc(buff->cap);

        struct continuation continuation;
        continuation.func = tcp_handle->func;
        continuation.data = buff;

        asio_add(asio, slave, EPOLLIN, continuation);
}

void log_error_and_exit(int err)
{
        printf("error! %s", strerror(errno));
        exit(err);
}

void read_and_close(int fd, void *data, enum events events)
{
        if (events & EPOLL_ERR) {
                close(fd);
                free(data);
                return;
        }
        assert(events & EPOLL_IN);
        struct buffer *buffer = data;
        int n = read(fd, buffer->data, buffer->cap - buffer->len);
        if (n < 0) {
                close(fd);
                free(buffer->data);
                free(buffer);
                return;
        }
        buffer->len += n;
        while (buffer->len == buffer->cap) {
                buffer->cap *= 2;
                buffer->data = realloc(buffer->data, buffer->cap);
                n = read(fd, buffer->data, buffer->cap - buffer->len);
                if (n < 0) {
                        close(fd);
                        free(buffer->data);
                        free(buffer);
                        return;
                }
                buffer->len += buffer->cap;
        }
        write(1, buffer->data, buffer->len);
        close(fd);
        free(buffer->data);
        free(buffer);
}


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