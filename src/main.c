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
	while((file = readdir(dir) ) != NULL) {
		pid_file_check(file);
	}
}

int main() {
	const char *path = "/proc/";
	open_and_print_proc(path);
}
