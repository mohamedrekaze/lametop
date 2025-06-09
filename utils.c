#include "lametop.h"
#include <stdatomic.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>

void open_and_print_proc() {
	DIR *dir = opendir("/proc/");
	struct dirent *s;
	size_t buff_size = 1000;
	char *buff = malloc(sizeof(char) * buff_size);

	while((s = readdir(dir) ) != NULL) {
		if(s->d_type == 0) {
			printf("file is unknown must fall to stat(): %u\n", s->d_type);
		}
		else if(s->d_type == 1) {
			printf("file is FIFO: %u\n", s->d_type);
		}
		else if(s->d_type == 2) {
			printf("file is CHARCHTER: %u\n", s->d_type);
		}
		else if(s->d_type == 4)
			printf("file is Directory: %u\n", s->d_type);
		else if(s->d_type == 6)
			printf("file is Block: %u\n", s->d_type);
		else if(s->d_type == 8)
			printf("file is Regular file: %u\n", s->d_type);
		else if(s->d_type == 10)
			printf("file is Symbolic link: %u\n", s->d_type);
		else if(s->d_type == 12)
			printf("file is Socket: %u\n", s->d_type);
	}
}

int main() {
	open_and_print_proc();
}
