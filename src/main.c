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

void open_and_print_proc(const char *path) {
	DIR *dir = opendir("/proc/");
	struct dirent *file;
	size_t buff_size = 1000;
	char *buff = malloc(sizeof(char) * buff_size);
	unsigned int i = 0;
	char *full_path;
	while((file = readdir(dir) ) != NULL) {
		if(full_path = construct_path(file))
			

	}
}

int main() {
	const char *path = "/proc/";
	open_and_print_proc(path);
}
