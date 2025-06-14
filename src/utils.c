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

int is_num(char *file_name) {
	int i = 0;
	while(file_name[i]) {
		if(!isdigit(file_name[i]))
			return 0;
		i++;
	}
	return 1;
}

char *construct_path(struct dirent *file) {
	char path[NAME_MAX + 100];
	if(is_num(file->d_name)) {
		strcpy(path, file->d_name);
		snprintf(path, sizeof(path), "/proc/%s/status", file->d_name);
	}
	return strdup(path);
}

char *read_stat(const char *path) {
	FILE *file= fopen(path, "r");
	unsigned int buf_size = 1024;
	char *buffer = malloc(sizeof(char) * buf_size);
	unsigned int total_read = 0;
	unsigned int byte_read = 0;

	while((byte_read = fread(buffer+total_read, 1, 1, file)) > 0
		&& total_read < buf_size - 1) {
		if(buffer[total_read] == '\n')
			break;
		total_read += byte_read;
	}
	return buffer;
}

char *copy_pid_value(char *line) {
	int i = 0;
	unsigned int token_len = 20;
	char *token_tmp;
	char *token = malloc(sizeof(char) * token_len);
	unsigned int j = 0;

	while(line[j] != ':') {
		j++;
	}
	while(!isalpha(line[j])) {
		j++;
	}
	while(!isdigit(line[j])) {
		j++;
	}
	line += j;
	while(*line && *line != '\n') {
		if(i + 1 == token_len) {
			token_tmp = token;
			token_len *= 2;
			token = realloc(token, token_len);
			if(!token) {
				printf("realloc error!\n");
				return token = token_tmp;
			}
		}
		token[i] = *line;
		line++;
		i++;
	}
	return token;
}

struct pid_values *get_field_value() {
	int i = 0;
	struct pid_values *pid =  malloc(sizeof(struct pid_values));
	const char *fields[] = {"Name", "Pid","\0"};

	while() {
		
	}
}
