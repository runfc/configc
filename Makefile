.PHONY: configc tests all clean

configc:
	make -C src

tests:
	make -C test

all: configc tests

clean:
	rm -rf *.so
	make -C src clean
	make -C test clean
