CC = gcc

CFLAGS = -std=c11
CFLAGS += -g0
CFLAGS += -Wunused
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Werror
CFLAGS += -Wpedantic

LDFLAGS = -D SYSPROC_ROOT='"/tmp/runfc/"'
LDFLAGS += -I../src

# defines src files with helpers and utilities
LIBS = lib/testlib.c
LIBS += ../src/log.c
LIBS += ../src/lib.c

TESTS_OBJS = sysctl.run
TESTS_OBJS += file.run

.PHONY: run clean

tests: $(TESTS_OBJS)
	@./run

sysctl.run: sysctl.c $(LIBS) ../src/sysctl.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) -o sysctl.run ../src/sysctl.c sysctl.c

file.run: file.c $(LIBS) ../src/file.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) -o file.run ../src/file.c file.c

clean:
	rm -rf *.run *.o *.run.out
