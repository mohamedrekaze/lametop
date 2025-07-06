#include "lametop.h"
#include <ctype.h>
#include <dirent.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int *cpu_stat_orch(struct dirent *file) {
	const char *path = stat_file_path(file);
	int *usage;
	if(!path) return NULL;
	char *stat_file = read_stat(path);
	if(!stat_file) return NULL;
	cpu_stat *stat = parse_cpu_stat(stat_file);
	if(!stat) return NULL;
	usage = cpu_usage(stat);
	if(usage == 0)
		usage = cpu_usage(stat);
	free((void*)path);
	free(stat);
	free(stat_file);
	return usage;
}

char *stat_file_path(struct dirent *file) {
	int path_len = NAME_MAX + 10; 
	char *stat_path = malloc(sizeof(char) * path_len);
	if(stat_path) {
		snprintf(stat_path, path_len, "/proc/%s", file->d_name);
		return stat_path;
	}
	return NULL;
}

cpu_stat *parse_cpu_stat(const char *file) {
	int i = 0;
	long val = 0;
	int x = 0;
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
	return snap;
}

int* cpu_usage(cpu_stat* snap) {
	static int first = 1;
	static long long last_total = 0, last_idle = 0;
	int *usage = malloc(sizeof(int *));
	long long total = snap->user + snap->nice + snap->system + snap->idle +
		snap->iowait + snap->irq + snap->softirq + snap->steal;
	long long idle = snap->idle + snap->iowait;
	if (first) {
		first = 0;
		last_total = total;
		last_idle = idle;
		*usage = 0;
	}
	else {
		long long delta_total = total - last_total;
		long long delta_idle = idle - last_idle;
		last_total = total;
		last_idle = idle;
		*usage = delta_total ? (100 * (delta_total - delta_idle)) / delta_total : 0;
	}
	return usage;
}
