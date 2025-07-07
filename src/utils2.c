#include "lametop.h"

void free_orch(DIR *dir, snapshot *tmp, snapshot *res) {
	free(tmp);
	free(dir);
	free_list_snapshot(res);
}

