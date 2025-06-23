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
	int WIN_HEIGHT = 30;
	int WIN_WIDTH = 50;
	int TOTAL_LINES = 50;
	initscr();              // Initialize ncurses
	noecho();               // Don't echo input
	cbreak();               // Disable line buffering
	keypad(stdscr, TRUE);   // Enable arrow keys
	curs_set(0);            // Hide cursor

	WINDOW *win = newwin(WIN_HEIGHT, WIN_WIDTH, 1, 1);
	scrollok(win, TRUE);

	char lines[TOTAL_LINES][60];
	for (int i = 0; i < TOTAL_LINES; ++i) {
		sprintf(lines[i], "Line %d: This is sample content.", i + 1);
	}

	int top_line = 0;
	int ch;
	int i;

	while (1) {
		i = 0;
		werase(win);
		box(win, 0, 0);
		while (i < WIN_HEIGHT - 2 && (top_line + i) < TOTAL_LINES) {
			mvwprintw(win, i + 1, 1, "%s", lines[top_line + i]);
			++i;
		}
		wrefresh(win);

		ch = getch();
		if (ch == KEY_DOWN && top_line + WIN_HEIGHT - 2 < TOTAL_LINES) {
			top_line++;
		} else if (ch == KEY_UP && top_line > 0) {
			top_line--;
		} else if (ch == 'q') {
			break;
		}
	}

	delwin(win);
	endwin();
}
