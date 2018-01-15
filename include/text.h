#ifndef _TEXT_H
#define _TEXT_H

#include <common.h>
#include <buffer.h>

// this represents a section of memory marked as executable
typedef struct {
    uint8_t* buf;
    size_t code_len;
    size_t alloc_len;
    
    uint8_t prot;
} text_t;

typedef enum {
    TEXTMODE_RW,
    TEXTMODE_RE
} text_mode_t;

size_t align_to_page(size_t);

text_t* text_new(size_t);
bool text_populate(text_t*, buffer_t*, size_t);
void text_free(text_t*);

bool text_set_mode(text_t*, text_mode_t);
void text_print(text_t*);

#endif
