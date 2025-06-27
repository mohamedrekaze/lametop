#include "lametop.h"
#include <ctype.h>
#include <dirent.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *stat_file_path(struct dirent *file) {
	int path_len = NAME_MAX + 10; 
	char *stat_path = malloc(sizeof(char) * path_len);
	if(stat_path) {
		snprintf(stat_path, path_len, "/proc/%s", file->d_name);
		parse_cpu_stat(stat_path);
		return stat_path;
	}
	return NULL;
}

cpu_stat *parse_cpu_stat(const char *path) {
	int i = 0;
	long val = 0;
	int x = 0;
	char *file = read_stat(path);
	cpu_stat *snap = malloc(sizeof(cpu_stat));
	while(!isdigit(file[i])) i++;
	while(file[i] != '\n' && x < 10) {
		val = 0;
		while(isdigit(file[i])) {
			val = (val * 10) + (file[i] - '0');
			i++;
		}
		switch (x) {
			case 0: snap->user = val; break;
			case 1: snap->nice = val; break;
			case 2: snap->system = val; break;
			case 3: snap->idle = val; break;
			case 4: snap->iowait = val; break;
			case 5: snap->irq = val; break;
			case 6: snap->softirq = val; break;
			case 7: snap->steal = val; break;
			case 8: snap->guest = val; break;
			case 9: snap->guest_nice = val; break;
		}
		x++;
		while(file[i] == ' ') i++;
	}
	cpu_usage(snap);
	return snap;
}

int cpu_usage(cpu_stat *snap) {
	static int first_read = 1;
	static long long prev_total = 0;
	static long long prev_idle = 0;

	long long cur_total = snap->user + snap->nice + snap->system + snap->idle
		+ snap->iowait + snap->irq + snap->softirq + snap->steal;
	long long cur_idle = snap->idle + snap->iowait;

	if (first_read) {
		prev_total = cur_total;
		prev_idle = cur_idle;
		first_read = 0;
		return 0;
	}

	long long delta_total = cur_total - prev_total;
	long long delta_idle = cur_idle - prev_idle;

	prev_total = cur_total;
	prev_idle = cur_idle;

	if (delta_total == 0) return 0;

	int total_cpu_usage = (int)(100.0 * (delta_total - delta_idle) / delta_total);
	return total_cpu_usage;
}


