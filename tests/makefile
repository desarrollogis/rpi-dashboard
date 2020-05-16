
all: ./test_configuration

%: %.c $(wildcard ../src/*.c)
	cc $$(pkg-config --libs --cflags yaml-0.1) $$(pkg-config --libs --cflags raylib) $^ -o $@
