#include "lametop.h"
#include <dirent.h>
#include <ncurses.h>
#include <string.h>
#include <unistd.h>

void	print_initial_frame(windows *table)
{
	// res = cpu_stat_orch(stat_file);
	box(table->win_proc, 0, 0);
	box(table->win_cpu, 0, 0);
	box(table->win_mem, 0, 0);
	wprintw(table->win_cpu, "win_cpu");
	wprintw(table->win_proc, "win_proc");
	wprintw(table->win_mem, "win_mem");
	wrefresh(table->win_proc);
	wrefresh(table->win_cpu);
	wrefresh(table->win_mem);
	refresh();
	//sleep(3);
	//wclear(stdscr);
}

int	win_orch(snapshot *file, windows *wind, struct dirent *stat_file)
{
	windows	*table;
	int		*res;

	initscr();
	clear();
	refresh();
	noecho();
	cbreak();
	keypad(stdscr, TRUE);
	if (!file || !wind || !stat_file)
	{
		error_log("error with win_orch params\n");
		return (1);
	}
	table = tables(wind);
	//print_initial_frame(table);
	print_frame(table, file);
	endwin();
	return (0);
}

int	*get_max_column_width(int *cur_width, snapshot *file)
{
	pid_values	*process;
	int			i;
	int			len;

	i = 0;
	len = 0;
	while (file)
	{
		process = file->process;
		if ((len = strlen(process->name)) > i)
			i = len;
		if ((len = strlen(process->pid)) > i)
			i = len;
		if ((len = strlen(process->stat)) > i)
			i = len;
		file = file->next;
	}
	*cur_width = i;
	return (cur_width);
}

void	print_rows(int table_len, snapshot *file)
{
	pid_values		*process;
	unsigned int	i;
	unsigned int	pos;
	unsigned int	bufr;
	int				len_field_name;
	int				len_field_pid;
	int				len_field_stat;
	int				ch;

	initscr();
	keypad(stdscr, TRUE);
	noecho();
	cbreak();
	i = 0;
	pos = 0;
	bufr = 1;
	len_field_name = col_max_width(file, "name");
	len_field_pid = col_max_width(file, "pid");
	len_field_stat = col_max_width(file, "stat");
	while (file)
	{
		pos = 0;
		process = file->process;
		mvprintw(i, pos, "%s", "|");
		mvprintw(i, pos += bufr, "%s", process->name);
		mvprintw(i, pos += len_field_name, "%s", "|");
		mvprintw(i, pos += bufr, "%s", process->pid);
		mvprintw(i, pos += len_field_pid, "%s", "|");
		mvprintw(i, pos += bufr, "%s", process->stat);
		mvprintw(i, pos += len_field_stat, "%s", "|");
		file = file->next;
		i++;
	}
	while (((ch = getch()) != KEY_F(1)) && ch != 'q')
	{
		refresh();
	}
	endwin();
}

int	col_max_width(snapshot *file, char *field)
{
	pid_values	*process;
	int			len;
	int			cur_len;

	len = 0;
	cur_len = 0;
	while (file)
	{
		process = file->process;
		if (strcmp("name", field) == 0)
		{
			if ((cur_len = strlen(process->name)) > len)
				len = cur_len;
		}
		if (strcmp("pid", field) == 0)
		{
			if ((cur_len = strlen(process->pid)) > len)
				len = cur_len;
		}
		if (strcmp("stat", field) == 0)
		{
			if ((cur_len = strlen(process->stat)) > len)
				len = cur_len;
		}
		file = file->next;
	}
	return (len);
}

void	get_xy(void)
{
	int	rows;
	int	cols;

	initscr();
	getmaxyx(stdscr, rows, cols);
	printw("%d %d", rows, cols);
	refresh();
	getch();
	endwin();
}

// WINDOW *newwin(int nlines, int ncols, int begin_y, int begin_x);
windows	*tables(windows *win_frame)
{
	int		cols_mid;
	int		lins_mid;
	WINDOW	*win_mem;
	WINDOW	*win_cpu;
	WINDOW	*win_proc;

	if (LINES < 12 || COLS < 4)
	{
		error_log("tables: terminal size too small\n");
		return (0);
	}
	cols_mid = (COLS / 2);
	lins_mid = (LINES / 2);
	win_mem = newwin(10, cols_mid, 0, 0);
	win_cpu = newwin(10, cols_mid, 0, cols_mid);
	win_proc = newwin(LINES - 11, COLS, 10, 0);
	if (!win_mem || !win_cpu || !win_proc)
	{
		error_log("tables: window creation failed\n");
		return (0);
	}
	win_frame->win_mem = win_mem;
	win_frame->win_cpu = win_cpu;
	win_frame->win_proc = win_proc;
	return (win_frame);
}

void	cpu_usage_widget(unsigned int usage, WINDOW *win_cpu)
{
	int	i;
	int	MaxBarWidth;
	int	BarLength;
	int	x;
	int	y;

	i = 1;
	int width, height;
	getmaxyx(win_cpu, height, width);
	MaxBarWidth = width - 4;
	BarLength = usage * MaxBarWidth / 100;
	x = 2;
	y = 2;
	while (i < MaxBarWidth)
	{
		if (i < BarLength)
		{
			mvwprintw(win_cpu, 1, i, "%c", '|');
			wrefresh(win_cpu);
			// mvwaddch(win_cpu, y, x + i, '|');
		}
		else
		{
			mvwprintw(win_cpu, 1, i, "%c", ' ');
			wrefresh(win_cpu);
		}
		i++;
	}
	wrefresh(win_cpu);
}

void	print_frame(windows *frame, snapshot *file)
{
	print_initial_frame(frame);
	int			ch;
	snapshot	*file_tmp;
	pid_values	*process;
	int			i;

	if (!file || !frame) {
		error_log("print_frame: linked list null");
		return;
	}
	if (!frame->win_proc || !frame->win_cpu || !frame->win_mem) {
		error_log("print_frame: frame elements null");
		return ;
	}
	file_tmp = file;
	i = 0;

	while (file_tmp->next)
	{
		file_tmp = file_tmp->next;
		i++;
	}
	while (((ch = getch()) != KEY_F(1)) && ch != 'q')
	{
		refresh();
	}
}
