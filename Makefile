header = src/lametop.h
exec = lametop
src = $(wildcard src/*.c)
obj = $(src:.c=.o)
cflags = -g 
ldflags = -lncurses 
#debflags = -fsanitize=address

all: $(exec)

$(exec): $(obj)
	cc $(obj) $(ldflags) -o $(exec)

%.o: %.c $(header)
	cc $(cflags) -c $< -o $@

clean: 
	rm $(obj)

fclean: 
	rm -f $(obj) $(exec)

re: clean all

.PHONY: all clean re
