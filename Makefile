CC      = gcc
CFLAGS = -Wall -Wextra -Werror -std=gnu11
LDFLAGS =

SRC     = main.c core.c virtual.c utils.c
OBJ     = $(SRC:.c=.o)

TARGET  = xkey

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.c xkey.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

format:
	git ls-files '*.c' '*.h' | xargs clang-format -i

re: clean all
