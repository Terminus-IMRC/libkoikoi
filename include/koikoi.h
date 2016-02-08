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
#define KOIKOI_CMD_PLAY ((koikoi_cmd_t) 2)
#define KOIKOI_CMD_BYE ((koikoi_cmd_t) 3)

	typedef uint8_t koikoi_play_t;
#define KOIKOI_PLAY_ERROR ((koikoi_play_t) 0)
#define KOIKOI_PLAY_OP_ERROR ((koikoi_play_t) 1)
#define KOIKOI_PLAY_INIT ((koikoi_play_t) 2)
#define KOIKOI_PLAY_PHASE1 ((koikoi_play_t) 3)
#define KOIKOI_PLAY_PHASE2 ((koikoi_play_t) 4)
#define KOIKOI_PLAY_PHASE3 ((koikoi_play_t) 5)
#define KOIKOI_PLAY_FINALIZE ((koikoi_play_t) 6)

	typedef uint8_t koikoi_play_order_t;
#define KOIKOI_PLAY_ORDER_INVALID ((koikoi_play_order_t) 0)
#define KOIKOI_PLAY_ORDER_FIRST ((koikoi_play_order_t) 1)
#define KOIKOI_PLAY_ORDER_SECOND ((koikoi_play_order_t) 2)

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
