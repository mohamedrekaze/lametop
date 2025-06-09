#include "lametop.h"
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <limits.h>

void open_and_print_proc() {
	DIR *dir = opendir("/home/mohamed/dotfiles/");
	struct dirent *s = readdir(dir);
	printf("%s\n", s->d_name);
}



int main() {
	open_and_print_proc();
}
