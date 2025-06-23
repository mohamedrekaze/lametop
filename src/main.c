#include "lametop.h"
#include <stdatomic.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>

void constuct_file(pid_values *process, snapshot *file) {
	snapshot file_dummy;
	add_tail(process, file);
}

void open_and_print_proc(const char *path) {
	DIR *dir = opendir("/proc/");
	struct dirent *file;
	size_t buff_size = 1000;
	char *buff = malloc(sizeof(char) * buff_size);
	int i = 0;
	char *full_path;
	snapshot *first = malloc(sizeof(snapshot));
	pid_values *process;
	int len_ll = 0;
	while((file = readdir(dir)) != NULL) {
		full_path = construct_path(file);
		if(full_path) {
			process = get_field_value(full_path);
			constuct_file(process, first);
		}
	}
	first = first->next;
	//get_max_column_width(&i, first);
	//print_rows(len_ll, first);
	snapshot *res = ll_sort(first, "name");
	//test_scroll();
}

int main() {
	const char *path = "/proc/";
	open_and_print_proc(path);
}
