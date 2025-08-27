#include "lametop.h"
#include <ctype.h>
#include <dirent.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

char	*read_mem_file()
{
	FILE *meminfo = fopen("/proc/meminfo", "r");
    if (!meminfo) {
        error_log("ERROR: failure to open file at read_mem_file at mem_stat.c");
        return NULL;
    }
	char *buff = malloc(sizeof(char) * 100000);
    if (!buff) {
        error_log("ERROR: failed allocation at read_mem_file at mem_stat.c");
        return NULL;
    }
	fread(buff, 100000, 1, meminfo);
    return buff;
}

mem_stat *parse_mem_info(char *buff) {
    char *delim = ": \n";
    char *line = strtok(buff, delim);
    mem_stat *mem = malloc(sizeof(mem_stat));
    if (!mem) {
        error_log("ERROR: failed allocation at parse_mem_info at mem_stat.c");
        return NULL;
    }

    while (line) {
        if(strcmp("MemTotal", line) == 0) {
            line = strtok(NULL, delim);
            mem->total_memory = atoi(line);
            line = strtok(NULL, delim);
        } else if(strcmp("MemFree", line) == 0) {
            line = strtok(NULL, delim);
            mem->free_memory = atoi(line);
            line = strtok(NULL, delim);
        } else if(strcmp("MemAvailable", line) == 0) {
            line = strtok(NULL, delim);
            mem->mem_avaible = atoi(line);
            line = strtok(NULL, delim);
        } else if(strcmp("Cached", line) == 0) {
            line = strtok(NULL, delim);
            mem->mem_cached = atoi(line);
            line = strtok(NULL, delim);
        } else if(strcmp("SwapTotal", line) == 0) {
            line = strtok(NULL, delim);
            mem->swap_total = atoi(line);
            line = strtok(NULL, delim);
        } else if(strcmp("SwapFree", line) == 0) {
            line = strtok(NULL, delim);
            mem->swap_free = atoi(line);
            line = strtok(NULL, delim);
        }
        else 
            line = strtok(NULL, delim);
    }
    return mem;
}

int main()
{
    char *buff = read_mem_file();
    mem_stat *mem = parse_mem_info(buff);
    printf("%ld\n", mem->total_memory);
    printf("%ld\n", mem->swap_total);
    printf("%ld\n", mem->swap_free);
}
