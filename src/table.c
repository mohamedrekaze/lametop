#include "lametop.h"
#include <ncurses.h>
#include <stdio.h>
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

void print_header(int cur_rows, int cur_ols) {
	
}

void table_structure() {
	
}

void test_scroll() {
	initscr();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0);

	int pad_rows = 100;
	int pad_cols = 50;
	int i = 0;
	int pad_top = 0;
	int screen_r = 20;
	int screen_c = 50;
	int ch;
	WINDOW *pad = newpad(pad_rows, pad_cols);

	while(i < pad_rows) {
		mvwprintw(pad, i, 0, "line %d", i);
		i++;
	}

	prefresh(pad, pad_top, 0, 0, 0, screen_r - 1, screen_c - 1);
	while((ch = getch()) != 'q') {
		if(ch == KEY_UP) {
			if(pad_top > 0)
				pad_top--;
		}
		if(ch == KEY_DOWN) {
			if(pad_top < pad_rows - screen_r)
				pad_top++;
		}
		prefresh(pad, pad_top, 0, 0, 0, screen_r - 1, screen_c - 1);
	}
	endwin();
}
