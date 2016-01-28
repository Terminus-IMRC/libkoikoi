#include "koikoi.h"
#include "err.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void koikoi_func_error_exit(const char *str)
{
	fprintf(stderr, "error: %s: %s\n", str, strerror(errno));
	exit(EXIT_FAILURE);
}
