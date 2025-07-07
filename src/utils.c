#include "lametop.h"
#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <math.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
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
	if (!path) {
		error_log("read_stat: path passed is null\n");
		return NULL;
	}

	size_t buf_size = 2048;
	char *buffer = malloc(buf_size);
	if (!buffer) {
		error_log("read_stat: buffer allocation failed\n");
		return NULL;
	}

	memset(buffer, 0, buf_size);

	FILE *file = fopen(path, "r");
	if (!file) {
		error_log("read_stat: failed to open file\n");
		free(buffer);
		return NULL;
	}

	size_t byte_read = fread(buffer, 1, buf_size - 1, file);
	buffer[byte_read] = '\0';  // safe null-termination

	fclose(file);
	return buffer;
}

char *copy_pid_value(char *line, unsigned int numerical_flag) {
	int i = 0;
	unsigned int token_len = 25;
	char *token = malloc(sizeof(char) * token_len);
	unsigned int j = 0;
	while(line[j] && line[j] != ':') j++;
	while(line[j] && !isalpha(line[j])) j++;
	if(numerical_flag) {
		while(line[j] && !isdigit(line[j]))
			j++;
	}
	while(line[j] && line[j] != '\n' && line[j] != '/' && i <= token_len - 1) {
		if (i >= token_len - 1) {
			token_len *= 2;
			char *token_tmp = realloc(token, token_len);
			if (!token_tmp) {
				free(token);
				return NULL;
			}
			token = token_tmp;
		}
		token[i++] = line[j++];
	}
	token[i] = '\0';
	return token;
}

char *clean_stat(char *token) {
	int i = 0;
	int j = 0;
	int size = strlen(token) + 1;
	while(token[i] != '(')
		i++;
	i++;
	while(token[i] != ')') {
		i++;
	}
	i--;
	token[i] = '\0';
	return token;
}

struct pid_values *get_field_value(const char *full_path) {
	struct pid_values *pid =  malloc(sizeof(struct pid_values));
	if(!pid) return NULL;
	const char *fields[fields_num] = {"Name", "Pid", "State"};
	int i = 0;
	char *line = read_stat(full_path);
	char *token;
	if (!line) {
		error_log("get_field_value: read_stat returned NULL for %s\n");
		free(pid);
		return NULL;
	}
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
		}
		i++;
	}
	free(line);
	return pid;
}

void add_tail(pid_values *process, snapshot *file) {
	if(!process || !file)
		return;
	while(file->next) {
		file = file->next;
	}
	snapshot *new_proc;
	if(file && !file->next) {
		file->next = malloc(sizeof(snapshot));
		file->next->process = process;
		file->next->next = NULL;
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

void free_list_snapshot(snapshot *list) {
	snapshot *tmp;
	while (list) {
		tmp = list;
		if (tmp->process) {
			free(tmp->process->stat);
			free(tmp->process->name);
			free(tmp->process->pid);
			free(tmp->process);
		}
		list = list->next;
		free(tmp);
	}
}
