#include "lametop.h"
#include <sys/stat.h>

void file_check(struct dirent *file_) {
	struct stat st;
	stat(file_, &st);
}
