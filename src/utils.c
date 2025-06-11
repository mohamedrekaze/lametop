#include "lametop.h"
#include <ctype.h>
#include <dirent.h>
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
		i++;
	}
}

void check_pid(struct dirent *file) {
	
}

void file_check(struct dirent *file) {
	if((file->d_name))
}
