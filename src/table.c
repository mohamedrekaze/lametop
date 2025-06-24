#include "lametop.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int *get_max_column_width(int *cur_width, snapshot *file) {
	pid_values *process;
	int i = 0;
	int len = 0;
	while(file) {
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
	initscr();
	keypad(stdscr, TRUE);
	noecho();
	cbreak();
	pid_values *process;
	unsigned int i = 0;
	unsigned int pos = 0;
	unsigned int bufr = 1;
	int len_field_name = col_max_width(file, "name");
	int len_field_pid = col_max_width(file, "pid");
	int len_field_stat = col_max_width(file, "stat");
	int ch;

	while(file) {
		pos = 0;
		process = file->process;
		mvprintw(i, pos,"%s", "|");
		mvprintw(i, pos += bufr,"%s", process->name);
		mvprintw(i, pos += len_field_name,"%s", "|");
		mvprintw(i, pos += bufr,"%s", process->pid);
		mvprintw(i, pos += len_field_pid,"%s", "|");
		mvprintw(i, pos += bufr,"%s", process->stat);
		mvprintw(i, pos += len_field_stat,"%s", "|");
		file = file->next;
		i++;
	}
	while(((ch = getch()) != KEY_F(1)) && ch != 'q') {
		refresh();
	}
	endwin();
}

int col_max_width(snapshot *file, char *field) {
	pid_values *process;
	int len = 0;
	int cur_len = 0;
	while(file) {
		process = file->process;
		if(strcmp("name", field) == 0) {
			if((cur_len = strlen(process->name)) > len)
				len = cur_len;
		}
		if(strcmp("pid", field) == 0) {
			if((cur_len = strlen(process->pid)) > len)
				len = cur_len;
		}
		if(strcmp("stat", field) == 0) {
			if((cur_len = strlen(process->stat)) > len)
				len = cur_len;
		}
		file = file->next;
	}
	return len;
}

void get_xy() {
	int rows;
	int cols;
	initscr();
	getmaxyx(stdscr, rows, cols);
	printw("%d %d", rows, cols);
	refresh();
	getch();
	endwin();
}

//WINDOW *newwin(int nlines, int ncols, int begin_y, int begin_x);
windows *tables(windows *win_frame) {
	int cols_mid = (COLS / 2);
	int lins_mid = (LINES / 2);

	WINDOW *win_mem = newwin(10, cols_mid, 0, 0);
	win_frame->win_mem = win_mem;

	WINDOW *win_cpu = newwin(10, cols_mid, 0, cols_mid);
	win_frame->win_cpu = win_cpu;

	WINDOW *win_proc = newwin(LINES - 11, COLS, 10, 0);
	win_frame->win_proc = win_proc;

	if(!win_frame->win_mem || !win_frame->win_proc || !win_frame->win_cpu) {
		endwin();
		fprintf(stderr, "newwin failed\n");
		exit(1);
	}
	return win_frame;
}

void print_frame(windows *frame, snapshot *file) {
	clear();
	refresh();
	noecho();
	cbreak();
	keypad(stdscr, TRUE);
	int ch;
	snapshot *file_tmp = file;
	pid_values *process;
	int i = 1;

	while(file_tmp) {
		box(frame->win_proc, 0, 0);
		mvwprintw(frame->win_proc, i, 1, "%s", file_tmp->process->name);
		wrefresh(frame->win_proc);
		file_tmp = file_tmp->next;
		i++;
	}

	/*
	file_tmp = file;
	while(file_tmp) {
		box(frame->win_cpu, 0, 0);
		mvwprintw(frame->win_cpu, 1, 1, "cpu");
		wrefresh(frame->win_cpu);
		file_tmp = file_tmp->next;
	}

	file_tmp = file;
	while(file_tmp) {
		box(frame->win_mem, 0, 0);
		mvwprintw(frame->win_mem, 1, 1, "memory");
		wrefresh(frame->win_mem);
		file_tmp = file_tmp->next;
	}
	*/

	refresh();
	while(((ch = getch()) != KEY_F(1)) && ch != 'q') {
		refresh();
	}
}


