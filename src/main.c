#include <unistd.h>
#include <sys/mman.h>

#include <common.h>
#include <buffer.h>
#include <text.h>
#include <int32_vec.h>

#define BUFSIZE 4096

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


int main(int argc, char** argv) {
    TRACE("starting up");

    buffer_t* code_buf = buffer_new(BUFSIZE);
    if(!code_buf || !code_buf->buf) {
        PANIC("malloc failed");
    }

    TRACE("generating code");

    int32_t nums[4] = {3,5,7,9};
    uint32_t code_len = generate_code(code_buf, 4, nums);

    TRACE("generated code");

    text_t* code = text_new(align_to_page(code_len));
    if(!code || !code->alloc_len) {
        PANIC("Couldn't allocate executable memory");
    }

    TRACE("mmap done");

    text_populate(code, code_buf, code_len);

    TRACE("memcpy ok");

#ifdef DEBUG
    text_print(code);
#endif
    text_set_mode(code, TEXTMODE_RE);

    int (*fn)();
    fn = (int (*)())code->buf;

    int res = fn();
    printf("result = %d\n", res);

    text_free(code);
    buffer_free(code_buf);

    return 0;
}

