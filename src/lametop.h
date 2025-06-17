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
#include <string.h>

/* structs */

typedef struct pid_values {
	char *name;
	char *stat;
	char *pid;
} pid_values;

typedef struct snapshot snapshot;
typedef struct snapshot {
	pid_values *process;
	snapshot *next;
} snapshot;

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
void add_tail(pid_values *process, snapshot *file);
void constuct_file(pid_values *process, snapshot *file);
void print_ll(snapshot *file);
int *get_max_column_width(int *cur_width, snapshot *file);
void ll_len(snapshot *file, int *len);
void print_rows(int table_len, snapshot *file);


/* macros */
#define fields_num 3



#endif
