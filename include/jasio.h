#pragma once
/*
MIT License

Copyright (c) 2021 JacobASchmidt

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include "fdmap.h"

struct jasio {
        int                poll_fd;
        struct jasio_fdmap fdmap;
};

void jasio_create(struct jasio *jasio, int cap);

int jasio_add(struct jasio *jasio, int fd, enum jasio_events events,
              struct jasio_continuation continuation);

int  jasio_modify_events(struct jasio *jasio, int fd, enum jasio_events events);
void jasio_modify_continuation(struct jasio *jasio, int fd,
                               struct jasio_continuation continuation);

int jasio_remove(struct jasio *jasio, int fd);
int jasio_destroy(struct jasio *jasio);

void jasio_run(struct jasio *jasio, int timeout);
