#include "lametop.h"
#include <ncurses.h>
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
	//initscr();
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
		if(strcmp(file->d_name, "stat") == 0) {
			/*
			while(1) {
				int *res = cpu_stat_orch(file);
				sleep(2);
			}
			*/
		}
		full_path = construct_path(file);
		if(full_path) {
			process = get_field_value(full_path);
			constuct_file(process, first);
		}
	}
	first = first->next;
	/*
	get_max_column_width(&i, first);
	print_rows(len_ll, first);
	snapshot *res = ll_sort(first, "name");
	windows *frame = malloc(sizeof(windows));
	tables(frame);
	print_frame(frame, res);
	endwin();
	*/
}



int main() {
	const char *path = "/proc/";
	open_and_print_proc(path);
}
