#ifndef KOIKOI_H_INCLUDED
#define KOIKOI_H_INCLUDED

#include <hanafuda.h>
#include <stdint.h>

	enum koikoi_prot_type {
		KOIKOI_PROT_TYPE_SOCKET,
		KOIKOI_PROT_TYPE_INVALID
	};

	typedef uint8_t koikoi_cmd_t;
#define KOIKOI_CMD_INVALID ((koikoi_cmd_t) 0)
#define KOIKOI_CMD_HELLO ((koikoi_cmd_t) 1)

	typedef struct {
		struct {
			enum koikoi_prot_type type;
			struct {
				int fds[3];
				char *name;
			} socket;
		} prot;
		struct {
			void (*error_exit)(const char *);
		} func;
	} koikoi_state_t;

	void koikoi_init(koikoi_state_t *ksp);
	void koikoi_finalize(koikoi_state_t *ksp);

	void koikoi_socket_server_init(const char *sockname, koikoi_state_t *ksp);
	void koikoi_socket_server_finalize(koikoi_state_t *ksp);
	void koikoi_socket_server_accept_connection(const int fd_n, koikoi_state_t *ksp);
	void koikoi_socket_server_close_connection(const int fd_n, koikoi_state_t *ksp);
	void koikoi_socket_connect(const char *sockname, koikoi_state_t *ksp);
	void koikoi_socket_disconnect(koikoi_state_t *ksp);
	int koikoi_socket_cmd_send(const uint8_t len, const uint8_t data[], const int fd_n, koikoi_state_t *ksp);
	int koikoi_socket_cmd_recv(const uint8_t maxlen, uint8_t data[], const int fd_n, koikoi_state_t *ksp);

	void koikoi_func_error_exit(const char *str);

#endif /* KOIKOI_H_INCLUDED */
