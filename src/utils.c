#include "lametop.h"
#include <ctype.h>
#include <dirent.h>
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
	char *path;
	char name[NAME_MAX + 10];
	if(is_num(file->d_name)) {
		printf("it worked this is the path:\t%s\n", name);
	}
	strcpy(name, file->d_name);
	snprintf(name, sizeof(name), "/proc/%s", file->d_name);
}
