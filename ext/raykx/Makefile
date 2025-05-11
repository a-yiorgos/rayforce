CC = clang
TARGET = libraykx.so
SRCS = raykx.c serde.c types.c
CFLAGS = -fPIC -Wall -Wextra -std=c17 -Ofast -march=native -fassociative-math -ftree-vectorize\
 -fno-math-errno -funsafe-math-optimizations -ffinite-math-only -funroll-loops -m64 -I.
LIBS = -lm -ldl -lpthread

default: debug

debug:
	$(CC) $(CFLAGS) -shared -fPIC -o $(TARGET) $(SRCS) -DDEBUG $(LIBS)

release:
	$(CC) $(CFLAGS) -shared -fPIC -o $(TARGET) $(SRCS) $(LIBS)

clean:
	rm -f $(TARGET)
	