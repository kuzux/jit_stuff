#include <stddef.h>
#include <stdint.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <sys/mman.h>

#define BUFSIZE 4096

size_t generate_code(size_t buflen, uint8_t* buf, size_t numslen, int32_t* nums) {
    size_t pos = 0;
    for(size_t i=0; i<numslen-1 && pos<buflen ;i++) {
        // mov eax
        buf[pos++] = 0xb8;

        // put in that number
        buf[pos++] = (nums[i] & 0x000000FF);
        buf[pos++] = (nums[i] & 0x0000FF00) >> 8;
        buf[pos++] = (nums[i] & 0x00FF0000) >> 16;
        buf[pos++] = (nums[i] & 0xFF000000) >> 24;

        // mov ebx
        buf[pos++] = 0xbb;

        // put in that number
        buf[pos++] = (nums[i+1] & 0x000000FF);
        buf[pos++] = (nums[i+1] & 0x0000FF00) >> 8;
        buf[pos++] = (nums[i+1] & 0x00FF0000) >> 16;
        buf[pos++] = (nums[i+1] & 0xFF000000) >> 24;

        // add eax, ebx
        buf[pos++] = 0x01;
        buf[pos++] = 0xd8;
    }

    buf[pos++] = 0xc3;

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

void print_code(size_t codelen, uint8_t* code) {
    for(size_t i=0; i<codelen; i++) {
        printf("%02x", code[i]);
        if(i%8 != 7){
            putchar(' ');
        } else {
            putchar('\n');
        }
    }

    puts("");
}

int main(int argc, char* argv) {
    printf("test\n");
    printf("%d\n", sysconf(_SC_PAGE_SIZE));

    uint8_t* code_buf = (uint8_t*)malloc(BUFSIZE);
    if(!code_buf) {
        panic("Couldn't allocate memory");
    }

    int32_t nums[4] = {3,5,7,9};
    uint32_t code_len = generate_code(BUFSIZE, code_buf, 4, nums);
    
    print_code(code_len, code_buf);

    size_t exec_len = page_aligned_length(code_len);

    uint8_t* exec_mem = (uint8_t*)mmap(NULL, exec_len, 
            PROT_READ | PROT_WRITE | PROT_EXEC, 
            MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if(exec_mem == MAP_FAILED) {
        panic("Couldn't mmap()");
    }

    memcpy(exec_mem, code_buf, code_len);

    int (*fn)();
    fn = (int (*)())exec_mem;

    int res = fn();
    printf("result = %d\n", res);

    munmap(exec_mem, exec_len);

    return 0;
}

