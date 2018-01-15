#include <buffer.h>

buffer_t* buffer_new(size_t len) {
    buffer_t* buf = (buffer_t*)malloc(sizeof(buffer_t));
    if(!buf) {
        return NULL;
    }
    
    buf->buf = (uint8_t*)malloc(len);
    if(!buf->buf) {
        // malloc failed
        buf->buflen = 0;
    } else {
        buf->buflen = len;
    }

    return buf;
}

bool buffer_checked_at(buffer_t* buf, size_t idx, uint8_t* res) {
    if(idx <= buf->buflen) {
        *res = buf->buf[idx];
        return true;
    } else {
        res = NULL;
        return false;
    }
}

uint8_t buffer_unchecked_at(buffer_t* buf, size_t idx) {
    return buf->buf[idx];
}

bool buffer_checked_put(buffer_t* buf, size_t idx, uint8_t elem) {
    if(idx <= buf->buflen) {
        buf->buf[idx] = elem;
        return true;
    } else {
        return false;
    }
}

void buffer_free(buffer_t* buf) {
    free(buf->buf);
    free(buf);
}

