#include "lametop.h"
#include <ncurses.h>
#include <string.h>

int mem_utils(mem_stat *mem) {
    double mem_uage_perc;
    char buff[100];
    if (!mem) {
        error_log("mem_utils: NULL memory info struct");
        return -1;
    }
    mem_uage_perc = ((double)mem->total_memory / (double)mem->used_memory);
    if (mem_uage_perc > (double)90)
        return COLOR_RED;
    else if (mem_uage_perc < (double)20)
        return COLOR_GREEN;
    else if (mem_uage_perc > (double)30)
        return COLOR_YELLOW;
    return COLOR_WHITE;
}

void print_mem_usage_titles(windows *frame, int row, int col, int i)
{
    getmaxyx(frame->win_mem, row, col);
    mvwprintw(frame->win_mem, row - (row - i++), col - (col - 2) , "%s", "Total mem:");
    mvwprintw(frame->win_mem, row - (row - i++), col - (col - 2) , "%s", "Used mem:");
    mvwprintw(frame->win_mem, row - (row - i++), col - (col - 2) , "%s", "Cached mem:");
    mvwprintw(frame->win_mem, row - (row - i++), col - (col - 2) , "%s", "Avaible mem:");
}

void print_mem_frame(mem_stat *mem, windows *frame)
{
    int row, col, i, ret, color;
    char buff[1000];
    getmaxyx(frame->win_mem, row, col);
    
    i = 1;
    color = mem_utils(mem);
    if (color < 0) {
        error_log("print_mem_frame: failed to get color");
        color = COLOR_WHITE;
    }

    print_mem_usage_titles(frame, row, col, i);
    error_log("color");
    sprintf(buff, "%d", color);
    error_log(buff);
    init_pair(1, color, COLOR_BLACK);
    wattron(frame->win_mem, COLOR_PAIR(1));

    mvwprintw(frame->win_mem, row - (row - i++), col - (col - strlen("Total mem:")), "%15ld", mem->total_memory);
    mvwprintw(frame->win_mem, row - (row - i++), col - (col - strlen("Used mem:")), "%15ld", (mem->total_memory - mem->mem_avaible));
    mvwprintw(frame->win_mem, row - (row - i++), col - (col - strlen("Cashed mem:")), "%15ld", mem->mem_cached);
    mvwprintw(frame->win_mem, row - (row - i++), col - (col - strlen("Avaible mem:")), "%15ld", mem->mem_avaible);
    wattroff(frame->win_mem, COLOR_PAIR(1));

    if (ret < 0) {
        error_log("print_mem_frame: return val of mvwprintw");
        sprintf(buff, "%d", ret);
        error_log(buff);
        return;
    }
}
