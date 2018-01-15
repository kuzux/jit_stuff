#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <sys/mman.h>

#include <buffer.h>

#define BUFSIZE 4096

#ifdef DEBUG
#define TRACE(x) { fprintf(stderr, "%s\n", (x)); }
#else
#define TRACE /* TRACE no-op */
#endif

typedef struct {
    uint8_t* buf;
    size_t buflen;
} buffer_t;

typedef struct {
    int32_t* data;
    size_t len;
    size_t allocated;
} int32_vec_t;

bool alloc_buffer(buffer_t* buf, size_t len) {
    buf->buf = (uint8_t*)malloc(len);
    if(!buf->buf) {
        // malloc failed
        buf->buflen = 0;
        return false;
    } else {
        buf->buflen = len;
        return true;
    }
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

void free_buffer(buffer_t* buf) {
    free(buf->buf);
    buf->buf = NULL;
    buf->buflen = 0;
}

size_t generate_code(buffer_t* buf, size_t numslen, int32_t* nums) {
    size_t pos = 0;
    for(size_t i=0; i<numslen-1 && pos<buf->buflen; i++) {
        // mov eax
        buffer_checked_put(buf, pos++, 0xb8);

        // put in that number
        buffer_checked_put(buf, pos++, (nums[i] & 0x000000FF));
        buffer_checked_put(buf, pos++, (nums[i] & 0x0000FF00) >> 8);
        buffer_checked_put(buf, pos++, (nums[i] & 0x00FF0000) >> 16);
        buffer_checked_put(buf, pos++, (nums[i] & 0xFF000000) >> 24);

        // mov ebx
        buffer_checked_put(buf, pos++, 0xbb);

        // put in that number
        buffer_checked_put(buf, pos++, (nums[i] & 0x000000FF));
        buffer_checked_put(buf, pos++, (nums[i] & 0x0000FF00) >> 8);
        buffer_checked_put(buf, pos++, (nums[i] & 0x00FF0000) >> 16);
        buffer_checked_put(buf, pos++, (nums[i] & 0xFF000000) >> 24);

        // add eax, ebx
        buffer_checked_put(buf, pos++, 0x01);
        buffer_checked_put(buf, pos++, 0xd8);
    }

    buffer_checked_put(buf, pos++, 0xc3);

    return pos;
}

void panic(const char* msg) {
    perror(msg);
    exit(1);
}

size_t page_aligned_length(size_t len) {
    const size_t page_size = sysconf(_SC_PAGE_SIZE);
    size_t res;

    for(res=0; res<len; res += page_size);
    
    return res;
}

void print_code(buffer_t* buf, int len) {
    for(size_t i=0; i<len; i++) {
        printf("%02x", buffer_unchecked_at(buf, i));
        if(i%8 != 7) {
            putchar(' ');
        } else {
            putchar('\n');
        }
    }

    puts("");
}

int main(int argc, char* argv) {
    TRACE("starting up");

    buffer_t* code_buf;
    code_buf = (buffer_t*)malloc(sizeof(buffer_t));
    if(!code_buf) {
        panic("malloc failed");
    }

    if(!alloc_buffer(code_buf, BUFSIZE)) {
        panic("Couldn't allocate memory");
    }

    TRACE("generating code");

    int32_t nums[4] = {3,5,7,9};
    uint32_t code_len = generate_code(code_buf, 4, nums);
    
#ifdef DEBUG
    print_code(code_buf, code_len);
#endif

    size_t exec_len = page_aligned_length(code_len);

    TRACE("generated code");

    uint8_t* exec_mem = (uint8_t*)mmap(NULL, exec_len, 
            PROT_READ | PROT_WRITE | PROT_EXEC, 
            MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if(exec_mem == MAP_FAILED) {
        panic("Couldn't mmap()");
    }

    TRACE("mmap done");

    memcpy(exec_mem, code_buf->buf, code_len);

    int (*fn)();
    fn = (int (*)())exec_mem;

    int res = fn();
    printf("result = %d\n", res);

    munmap(exec_mem, exec_len);

    free_buffer(code_buf);
    free(code_buf);

    return 0;
}

