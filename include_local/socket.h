#ifndef LOCAL_SOCKET_H_INCLUDED
#define LOCAL_SOCKET_H_INCLUDED

#include "koikoi.h"

	void koikoi_socket_server_init(const char *sockname, koikoi_state_t *ksp);
	void koikoi_socket_server_finalize(koikoi_state_t *ksp);
	void koikoi_socket_server_accept_connection(const int n, koikoi_state_t *ksp);
	void koikoi_socket_server_close_connection(const int n, koikoi_state_t *ksp);
	void koikoi_socket_connect(const char *sockname, koikoi_state_t *ksp);
	void koikoi_socket_disconnect(koikoi_state_t *ksp);
	int koikoi_socket_cmd_send(const uint8_t len, const uint8_t data[], const int fd_n, koikoi_state_t *ksp);
	int koikoi_socket_cmd_recv(const uint8_t maxlen, uint8_t data[], const int fd_n, koikoi_state_t *ksp);

#endif /* LOCAL_SOCKET_H_INCLUDED */
