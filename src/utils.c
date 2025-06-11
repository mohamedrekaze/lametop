#include "lametop.h"
#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

int is_num(char *file_name) {
	int i = 0;
	while(file_name[i]) {
		if(!isdigit(file_name[i]))
			return 0;
		i++;
	}
	return 1;
}

void check_stats() {
	
}

int pid_file_check(struct dirent *file) {
	char path[NAME_MAX + 100];
	if(!is_num(file->d_name)) {
	}
	strcpy(path, file->d_name);
	snprintf(path, sizeof(path), "/proc/%s/stat", file->d_name);
	read_stat(path);
}

char *read_stat(const char *path) {
	char *buff;
	int fd;
	//buff = realloc(buff, sizeof(char) * (strlen(path) * 2));
	open(path, O_RDWR);
	printf("%d\n", fd);
}

