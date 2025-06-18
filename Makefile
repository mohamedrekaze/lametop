header = src/lametop.h
exec = lametop
src = $(wildcard src/*.c)
obj = $(src:.c=.o)

all: $(exec)

$(exec): $(obj)
	cc -g $(obj) -lncurses -o $(exec)

%.o: %.c $(header)
	cc -c $< -o $@

clean: 
	rm $(obj)

fclean: 
	rm $(obj) $(exec)

re: clean all

.PHONY: all clean re
