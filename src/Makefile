CC = gcc

CFLAGS = -std=c11 -g3
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Werror
CFLAGS += -Wpendantic
CFLAGS += -Wunused
CFLAGS += -fPIC

SRCS = lib.c
SRCS += log.c
SRCS += file.c
SRCS += sysctl.c

OBJS = wrappers.o
OBJS += log.o
OBJS += file.o
OBJS += sysctl.o

configc: $(OBJS)
	$(CC) -shared -Wl,-soname -o $(CONFIGC_SO) $^

%(OBJS): $(SRCS)
	$(CC) -c $(CFLAGS) -o $@

.PHONY: clean

clean:
	rm -rf *.o

