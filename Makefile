TARGET := prog

CFLAGS := -MMD -MP -Iinclude

SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)
DEPS := $(SRCS:.c=.d)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

%.o: %c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

-include $(DEPS)

clean:
	rm $(OBJS) $(DEPS) $(TARGET)

