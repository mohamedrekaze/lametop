#include "lametop.h"
#include <dirent.h>
#include <fcntl.h>
#include <limits.h>
#include <ncurses.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <term.h>
#include <unistd.h>

void	constuct_file(pid_values *process, snapshot *file)
{
	snapshot	file_dummy;

	add_tail(process, file);
}

void	open_and_print_proc(const char *path)
{
	DIR				*dir;
	struct dirent	*file;
	struct dirent	*stat_file;
	size_t			buff_size;
	int				i;
	char			*full_path;
	snapshot		*first;
	pid_values		*process;
	snapshot		*res;
	windows			*frame;
	static int		*cp_usage;

	dir = opendir("/proc/");
	buff_size = 1000;
	i = 0;
	first = malloc(sizeof(snapshot));
	first->next = NULL;
	first->process = NULL;
	while ((file = readdir(dir)) != NULL) {
		if (strcmp(file->d_name, "stat") == 0) {
			stat_file = file;
			cp_usage = cpu_stat_orch(stat_file);
			if (!*cp_usage) {
				sleep(1);
				cp_usage = cpu_stat_orch(stat_file);
			}
		}
		full_path = construct_path(file);
		if (full_path) {
			process = get_field_value(full_path);
			if(!process) {
				error_log("open_and_read: null returned from get_field_value\n");
				return;
			}
			constuct_file(process, first);
			free(full_path);
		}
	}
	snapshot *tmp = first;
	first = first->next;
	res = ll_sort(first, "name");
	frame = malloc(sizeof(windows));
	win_orch(res, frame, cp_usage);
	free(tmp);
	free(dir);
	free_list_snapshot(res);
}

int	main(void)
{
	const char	*path = "/proc/";
	open_and_print_proc(path);
}
