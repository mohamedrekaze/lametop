#include "lametop.h"
#include <ncurses.h>
#include <stdio.h>
#include <string.h>

int *get_max_column_width(int *cur_width, snapshot *file) {
	pid_values *process;
	int i = 0;
	int len = 0;
	while(file->next) {
		process = file->process;
		if((len = strlen(process->name)) > i)
			i = len;
		if((len = strlen(process->pid)) > i)
			i = len;
		if((len = strlen(process->stat)) > i)
			i = len;
		file = file->next;
	}
	*cur_width = i;
	return cur_width;
}

void print_rows(int table_len, snapshot *file) {
	pid_values *process;
	unsigned int i = 0;
	unsigned int pos = 0;
	initscr();
	while(file->next) {
		process = file->process;
		mvprintw(i, 0,"%s\t%s\t%s\n", process->name, process->pid, process->stat);
		/*
		refresh();
		mvprintw(i, pos + strlen(process->pid),"%s ", process->name);
		refresh();
		mvprintw(i, pos + strlen(process->stat),"%s ", process->name);
		refresh();
		*/
		file = file->next;
		i++;
	}
	getch();
	endwin();
}
