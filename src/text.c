#include <text.h>

#include <unistd.h>
#include <sys/mman.h>

size_t align_to_page(size_t len) {
    const size_t page_size = sysconf(_SC_PAGE_SIZE);
    size_t res;

    for(res=0; res<len; res += page_size);
    
    return res;
}

text_t* text_new(size_t buflen) {
    text_t* res = (text_t*)malloc(sizeof(text_t));
    if(!res) {
        return NULL;
    }

    res->code_len = 0;
    res->buf = (uint8_t*)mmap(NULL, buflen, 
            PROT_READ | PROT_WRITE, 
            MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if(res->buf==MAP_FAILED) {
        res->alloc_len = 0;
    } else {
        res->alloc_len = buflen;
    }

    return res;
}

bool text_populate(text_t* text, buffer_t* buf, size_t len) {
    if(len > text->alloc_len) {
        return false;
    }

    memcpy(text->buf, buf->buf, len);
    text->code_len = len;

    return true;
}

bool text_set_mode(text_t* code, text_mode_t mode) {
    if(!code) {
        return false;
    }

    int res;

    switch(mode) {
        case TEXTMODE_RW:
            res = mprotect(code->buf, code->alloc_len, PROT_READ | PROT_WRITE);
            break;
        case TEXTMODE_RE:
            res = mprotect(code->buf, code->alloc_len, PROT_READ | PROT_EXEC);
            break;
        default:
            PANIC("Unknown text type");
            break;
    }

    return (res == 0);
}

void text_free(text_t* code) {
    munmap(code->buf, code->alloc_len);
    free(code);
}

void text_print(text_t* code) {
    for(size_t i=0; i<code->code_len; i++) {
        printf("%02x", code->buf[i]);
        if(i%8 != 7) {
            putchar(' ');
        } else {
            putchar('\n');
        }
    }

    puts("");
}

