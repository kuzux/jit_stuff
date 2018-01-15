#ifndef _INT32_VEC_H
#define _INT32_VEC_H

#include <common.h>

typedef struct {
    int32_t* data;
    size_t len;
    size_t allocated;
} int32_vec_t;

#endif

