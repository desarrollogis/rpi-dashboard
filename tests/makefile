
all: ./test_configuration ./test_drawing

%: %.c $(wildcard ../src/*.c)
	cc $$(pkg-config --libs --cflags yaml-0.1) $$(pkg-config --libs --cflags raylib) $^ -o $@
