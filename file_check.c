#include "lametop.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

void file_check(struct dirent *file) {
	
	if(file->d_type == 0) {
		printf("file is unknown must fall to stat(): %u\n", file->d_type);
		printf("file name: %s\n", file->d_name);
	}
	else if(file->d_type == 1) {
		printf("file is FIFO: %u\n", file->d_type);
		printf("file name: %s\n", file->d_name);
	}
	else if(file->d_type == 2) {
		printf("file is CHARCHTER: %u\n", file->d_type);
		printf("file name: %s\n", file->d_name);
	}
	else if(file->d_type == 4) {
		printf("file name: %s\n", file->d_name);
		printf("file is Directory: %u\n", file->d_type);
	}
	else if(file->d_type == 6) {
		printf("file name: %s\n", file->d_name);
		printf("file is Block: %u\n", file->d_type);
	}
	else if(file->d_type == 8) {
		printf("file name: %s\n", file->d_name);
		printf("file is Regular file: %u\n", file->d_type);
	}
	else if(file->d_type == 10) {
		printf("file name: %s\n", file->d_name);
		printf("file is Symbolic link: %u\n", file->d_type);
	}
	else if(file->d_type == 12) {
		printf("file name: %s\n", file->d_name);
		printf("file is Socket: %u\n", file->d_type);
	}
}

/*
char *construct_path(const char *filename, const char *path) {
	
}
*/
