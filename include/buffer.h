#ifndef _BUFFER_H
#define _BUFFER_H

#include <common.h>

typedef struct {
    uint8_t* buf;
    size_t buflen;
} buffer_t;

buffer_t* buffer_new(size_t);
void buffer_free(buffer_t*);

bool buffer_checked_at(buffer_t*, size_t, uint8_t*);
uint8_t buffer_unchecked_at(buffer_t*, size_t);

#endif

