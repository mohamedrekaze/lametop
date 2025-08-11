#include "lametop.h"
#include <dirent.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void	print_initial_frame(windows *table)
{
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
}

void	free_windows(windows *window)
{
	free(window->win_cpu);
	free(window->win_mem);
	free(window->win_proc);
	free(window);
}

int	win_orch(snapshot *file, windows *wind, int *cpu_usage)
{
	windows	*table;
	int		*res;

	initscr();
	clear();
	refresh();
	noecho();
	cbreak();
	keypad(stdscr, TRUE);
	if (!file || !wind)
	{
		error_log("error with win_orch params\n");
		return (1);
	}
	table = tables(wind);
	print_frame(table, file, *cpu_usage);
	clear();
	refresh();
	delwin(table->win_proc);
	delwin(table->win_mem);
	delwin(table->win_cpu);
	free(table);
	endwin();
	return (0);
}

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
	//win_proc = newwin(LINES - 11, COLS, 10, 0);
	win_proc = newpad(500, COLS);

	win_frame->win_proc_x = LINES - 11;
	win_frame->win_proc_y = COLS;
	win_frame->win_mem_x = 10;
	win_frame->win_mem_y = cols_mid;
	win_frame->win_cpu_x = 10;
	win_frame->win_cpu_y = cols_mid;

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
	int	width;
	int	height;

	i = 0;
	getmaxyx(win_cpu, height, width);
	MaxBarWidth = width - 20;
	BarLength = (usage * MaxBarWidth) / 100;
	x = 1;
	y = 1;
	mvwprintw(win_cpu, x, y, "%s", "CPU");
	y += strlen("CPU") + 1;
	mvwprintw(win_cpu, x, y, "%c", '[');
	y++;
	while (i <= MaxBarWidth - 1)
	{
		if (i < BarLength)
			mvwprintw(win_cpu, x, y, "%c", '*');
		else if (i == MaxBarWidth - 1)
			mvwprintw(win_cpu, x, y, "%c", ']');
		wrefresh(win_cpu);
		y++;
		i++;
	}
	mvwprintw(win_cpu, x, y, " %d%%", usage);
	wrefresh(win_cpu);
}

void	print_frame(windows *frame, snapshot *file, unsigned int usage)
{
	int			ch;
	snapshot	*file_tmp;
	pid_values	*process;
	int			i;
	int			scroll_off = 0;
	print_initial_frame(frame);
	if (!file || !frame) {
		error_log("print_frame: linked list null");
		return ;
	}
	if (!frame->win_proc || !frame->win_cpu || !frame->win_mem) {
		error_log("print_frame: frame elements null");
		return ;
	}
	file_tmp = file;
	i = 1;
	wattron(frame->win_proc, A_BOLD | COLOR_PAIR(1));
	mvwprintw(frame->win_proc, i++, 2, "%-6s %-30s %s", "Pid", "Name", "State");
	wattroff(frame->win_proc, A_BOLD | COLOR_PAIR(1));
	while (file_tmp->next) {
		mvwprintw(frame->win_proc, i, 2, "%-6s %-30s %s", file_tmp->process->pid,
			file_tmp->process->name, file_tmp->process->stat);
		cpu_usage_widget(usage, frame->win_cpu);
		file_tmp = file_tmp->next;
		//wrefresh(frame->win_proc);
		prefresh(frame->win_proc, scroll_off, 0, 10, 0, 
		   10 + frame->win_proc_x - 1, frame->win_proc_y - 1);
		i++;
	}
	while (((ch = getch()) != KEY_F(1)) && ch != 'q' && scroll_off < 500) {
		if(ch == KEY_UP || ch == 'k')
			scroll_off--;
		else if(ch == 'K')
			scroll_off -= 5;
		else if(ch == 'J')
			scroll_off += 5;
		else if(ch == KEY_DOWN || ch == 'j')
			scroll_off++;
		refresh();
		prefresh(frame->win_proc, scroll_off, 0, 10, 0, 
		   10 + frame->win_proc_x - 1, frame->win_proc_y - 1);
	}
}
