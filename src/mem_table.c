#include "lametop.h"

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

void print_mem_usage_titles()
{
    
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

    error_log("color");
    sprintf(buff, "%d", color);
    error_log(buff);
    init_pair(1, color, COLOR_BLACK);
    wattron(frame->win_mem, COLOR_PAIR(1));
    ret = mvwprintw(frame->win_mem, row - (row - i++), col - (col - 2)
            , "%s\t%ld%s", "Total mem:", mem->total_memory, " KB");
    ret = mvwprintw(frame->win_mem, row - (row - i++), col - (col - 2)
            , "%s\t%ld%s", "Used mem:", mem->used_memory, " KB");
    ret = mvwprintw(frame->win_mem, row - (row - i++), col - (col - 2)
            , "%s\t%ld%s", "Cached mem:", mem->mem_cached, " KB");
    ret = mvwprintw(frame->win_mem, row - (row - i++), col - (col - 2)
            , "%s\t%ld%s", "Free mem:", mem->free_memory, " KB");
    ret = mvwprintw(frame->win_mem, row - (row - i++), col - (col - 2)
            , "%s\t%ld%s", "Avaible mem:", mem->mem_avaible, " KB");
    ret = mvwprintw(frame->win_mem, row - (row - i++), col - (col - 2)
            , "%s\t%ld%s", "Swap mem:", mem->swap_total, " KB");
    ret = mvwprintw(frame->win_mem, row - (row - i++), col - (col - 2)
            , "%s\t%ld%s", "Swap Free:", mem->swap_free, " KB");
    wattroff(frame->win_mem, COLOR_PAIR(1));
    if (ret < 0) {
        error_log("print_mem_frame: return val of mvwprintw");
        sprintf(buff, "%d", ret);
        error_log(buff);
        return;
    }
}
