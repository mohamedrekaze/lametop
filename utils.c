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

	while((s = readdir(dir) )!= NULL) {
		
	}
}

int main() {
	//open_and_print_proc();
}
