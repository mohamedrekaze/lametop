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
#include <ncurses.h>

/* structs */
struct pid_values {
	char *name;
	char *stat;
	char *pid;
};

/* functions */
int file_check(struct dirent *file);
void open_and_print_proc(const char *path);
char *construct_path(struct dirent *file);
int is_num(char *file_name);
char *read_stat(const char *path);
char *get_next_line(FILE *file);
struct pid_values *get_field_value(const char *full_path);
char *copy_pid_value(char *line, unsigned int numerical_flag);
char *clean_stat(char *token);

/* macros */
#define fields_num 3



#endif
