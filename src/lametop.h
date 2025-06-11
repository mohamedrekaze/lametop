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

/* functions */

int file_check(struct dirent *file);
void open_and_print_proc(const char *path);
int pid_file_check(struct dirent *file);
int is_num(char *file_name);
char *read_stat(const char *path);




#endif
