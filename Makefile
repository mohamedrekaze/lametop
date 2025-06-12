header = src/lametop.h
exec = lametop
src = src/utils.c src/main.c
obj = $(src:.c=.o)

all: $(exec)

$(exec): $(src)

$(exec): $(obj)
	cc -g $(obj) -o $(exec)

%.o: %.c $(header)
	cc -c $< -o $@

clean: 
	rm $(obj)

fclean: 
	rm $(obj) $(exec)

re: clean all

.PHONY: all clean re
