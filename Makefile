TARGET := prog

CFLAGS := -MMD -MP -Iinclude -Wall -Werror 

ifdef RELEASE
	CFLAGS += -O2
else 
	CFLAGS += -g -DDEBUG
endif

SRCS := $(wildcard src/*.c)
OBJS := $(addprefix dest/, $(notdir $(SRCS:.c=.o)))
DEPS := $(OBJS:.o=.d)

dest/$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

dest/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

-include $(DEPS)

all: $(TARGET)

clean:
	rm -r dest
	mkdir -p dest

