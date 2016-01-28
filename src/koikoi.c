#include "koikoi.h"
#include "err.h"
#include <hanafuda.h>
#include <stdio.h>
#include <errno.h>

void koikoi_init(koikoi_state_t *ksp)
{
	errno = 0;
	ksp->prot.type = KOIKOI_PROT_TYPE_INVALID;
	ksp->prot.socket.fds[0] = -1;
	ksp->prot.socket.fds[1] = -1;
	ksp->prot.socket.fds[2] = -1;
	ksp->prot.socket.name = NULL;
	ksp->func.error_exit = koikoi_func_error_exit;
}

void koikoi_finalize(koikoi_state_t *ksp)
{
	(void) ksp;
}
