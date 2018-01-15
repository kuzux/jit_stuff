#ifndef _COMMON_H
#define _COMMON_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef DEBUG
#define TRACE(msg) { fprintf(stderr, "%s\n", (msg)); }
#else
#define TRACE
#endif

#define PANIC(msg) { perror(msg); exit(1); }

#endif
