#include "lametop.h"
#include <dirent.h>
#include <sys/stat.h>

void file_check(struct dirent *file) {
	
	if(file->d_type == 0) {
		printf("file is unknown must fall to stat(): %u\n", file->d_type);
	}
	else if(file->d_type == 1) {
		printf("file is FIFO: %u\n", file->d_type);
	}
	else if(file->d_type == 2) {
		printf("file is CHARCHTER: %u\n", file->d_type);
	}
	else if(file->d_type == 4) {
		printf("file is Directory: %u\n", file->d_type);
	}
	else if(file->d_type == 6) {
		printf("file is Block: %u\n", file->d_type);
	}
	else if(file->d_type == 8) {
		printf("file is Regular file: %u\n", file->d_type);
	}
	else if(file->d_type == 10) {
		printf("file is Symbolic link: %u\n", file->d_type);
	}
	else if(file->d_type == 12) {
		printf("file is Socket: %u\n", file->d_type);
	}
}

char *construct_path() {
	
}
