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

void pid_file_check(struct dirent *file) {
	char path[NAME_MAX + 100];
	if(!is_num(file->d_name)) {
		printf("not a process id\n");
	}
	else {
		strcpy(path, file->d_name);
		snprintf(path, sizeof(path), "/proc/%s/status", file->d_name);
		read_stat(path);
	}
}

char *read_stat(const char *path) {
	FILE *file= fopen(path, "r");
	unsigned int buf_size = 1024;
	char *buffer = malloc(sizeof(char) * buf_size);
	fread(buffer, 10, buf_size/10, file);
	return get_field_value(buffer);
}

char *get_next_line(const char *file) {
	int i = 0;
	int line_len = 10;
	char *line = malloc(sizeof(char) * line_len);
	char *line_tmp;
	while(file[i]) {
		if(i == line_len - 1) {
			line_tmp = line;
			line = realloc(line, line_len * 2);
			if(!line_tmp) {
				printf("realloc error!\n");
				return line = line_tmp;
			}
		}
		if(file[i] != '\n')
			line[i] = file[i];
		else
			break;
		i++;
	}
	return line;
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
	while(!isalpha(line[j]))
		j++;
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

char *get_field_value(char *line) {
	int i = 0;
	struct pid_values *pid =  malloc(sizeof(struct pid_values));
	const char *fields[] = {"Name", "Pid","\0"};
	const char *pid_name;
	unsigned int j = 0;
	while(fields[i]) {
		pid->name =  copy_pid_value(strstr(line, fields[i]));
		//pid->pid = ;
		printf("%s\n", pid->name);
		break;
	}
}
