#ifndef lametop
#define lametop

/* header files */
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

/* structs */

struct pid_values {
	char *name;
	char *stat;
	int pid;
};

/* functions */

int file_check(struct dirent *file);
void open_and_print_proc(const char *path);
void pid_file_check(struct dirent *file);
int is_num(char *file_name);
char *read_stat(const char *path);
char *get_next_line(const char *file);
char *get_field_value(char *line);
char *copy_pid_value(char *line);




#endif
