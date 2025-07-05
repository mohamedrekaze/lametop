#include "lametop.h"
#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <math.h>
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
		return strdup(path);
	}
	return NULL;
}

char *read_stat(const char *path) {
	FILE *file= fopen(path, "r");
	unsigned int buf_size = 1024;
	char *buffer = malloc(sizeof(char) * buf_size);
	unsigned int total_read = 0;
	unsigned int byte_read = 0;
	while((byte_read = fread(buffer+total_read, 1, 1, file)) > 0 && total_read < buf_size - 1) {
		total_read += byte_read;
	}
	fclose(file);
	buffer[total_read] = '\0';
	return buffer;
}

	char *copy_pid_value(char *line, unsigned int numerical_flag) {
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
	if(numerical_flag) {
		while(!isdigit(line[j])) {
			j++;
		}
	}
	line += j;
	while(*line && *line != '\n' && *line != '/') {
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

char *clean_stat(char *token) {
	int i = 0;
	int j = 0;
	char new_token[strlen(token) + 10];
	while(token[i] != '(')
		i++;
	i++;
	while(token[i] != ')') {
		new_token[j] = token[i];
		j++;
		i++;
	}
	new_token[j] = '\0';
	return strdup(new_token);
}

struct pid_values *get_field_value(const char *full_path) {
	struct pid_values *pid =  malloc(sizeof(struct pid_values));
	const char *fields[fields_num] = {"Name", "Pid", "State"};
	int i = 0;
	char *line = read_stat(full_path);
	char *token;
	while(i < fields_num) {
		if(strcmp("Name", fields[i]) == 0) {
			pid->name = copy_pid_value(strstr(line, fields[i]), 0);
		}
		if(strcmp("Pid", fields[i]) == 0) {
			pid->pid = copy_pid_value(strstr(line, fields[i]), 1);
		}
		if(strcmp("State", fields[i]) == 0) {
			token = copy_pid_value(strstr(line, fields[i]), 0);
			pid->stat = clean_stat(token);
			free(token);
		}
		i++;
	}
	free(line);
	return pid;
}

void add_tail(pid_values *process, snapshot *file) {
	snapshot *new_proc = malloc(sizeof(snapshot));
	while(file->next) {
		file = file->next;
	}
	if(!file->next) {
		new_proc->process = process;
		new_proc->next = NULL;
		file->next = new_proc;
	}
}

void print_ll(snapshot *file) {
	while(file) {
		pid_values *process = file->process;
		printf("%p\t%s\t%s\t%s\n", 
		 (char *)file->next, process->name, process->pid, process->stat);
		file = file->next;
	}
}

void ll_len(snapshot *file, int *len) {
	while(file) {
		(*len)++;
		file = file->next;
	}
}

void swap(void *l_a, void *l_b) {
	void *tmp;
	tmp = l_a;
	l_a = l_b;
	l_b = l_a;
}

snapshot *ll_sort(snapshot *file, char *arg) {
	if(!file || !file->next) return file;
	snapshot *file_slow = file;
	snapshot *file_fast = file;
	snapshot *prev = NULL;
	snapshot *right;
	snapshot *left;
	while(file_fast && file_fast->next) {
		prev = file_slow;
		file_slow = file_slow->next;
		file_fast = file_fast->next->next;
	}
	if(prev)
		prev->next = NULL;
	left = file;
	right = file_slow;
	
	left = ll_sort(left, arg);
	right = ll_sort(right, arg);
	return ll_merge(left, right, arg);
}

snapshot *ll_merge(snapshot *left, snapshot *right, char *arg) {
	snapshot *new_file = NULL;
	snapshot *head = NULL;
	snapshot *tail = NULL;
	snapshot *curr = NULL;
	snapshot *rem;
	int name_flag = strcmp(arg, "name");
	int pid_flag = strcmp(arg, "pid");
	int stat_flag = strcmp(arg, "stat");
	int cmp_res;
	while(left && right) {
		if(name_flag == 0)
			cmp_res = strcmp(left->process->name, right->process->name);
		else if(pid_flag == 0)
			cmp_res = strcmp(left->process->pid, right->process->pid);
		else if(stat_flag == 0)
			cmp_res = strcmp(left->process->stat, right->process->stat);
		else
			return NULL;
		if(cmp_res <= 0) {
			curr = left;
			left = left->next;
		}
		else {
			curr = right;
			right = right->next;
		}
		curr->next = NULL;
		if(!head) {
			head = curr;
			tail = curr;
		}
		else {
			tail->next = curr;
			tail = curr;
		}
	}
	rem = (left != NULL) ? left : right;
	if(tail) {
		tail->next = rem;
	}
	else {
		head = rem;
	}
	return head;
}

void error_log(char *error) {
	FILE *log = fopen("error.log", "w");
	if (log) {
		fprintf(log, "%s\n", error);
		fclose(log);
	}
}
