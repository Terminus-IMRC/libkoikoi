#include "koikoi.h"
#include "socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>

void koikoi_socket_server_init(const char *sockname, koikoi_state_t *ksp)
{
	int sfd;
	struct sockaddr_un server_addr;
	size_t len;
	char *dupname;
	int ret;

	if (ksp->prot.type != KOIKOI_PROT_TYPE_INVALID) {
		errno = EADDRINUSE;
		ksp->func.error_exit("koikoi_socket_server_init");
	}
	ksp->prot.type = KOIKOI_PROT_TYPE_SOCKET;

	len = strlen(sockname);
	if (len >= sizeof(server_addr.sun_path)) {
		errno = ENAMETOOLONG;
		ksp->func.error_exit("koikoi_socket_server_init");
	}

	sfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sfd == -1) {
		ksp->func.error_exit("koikoi_socket_server_init: socket");
	}

	memset(&server_addr, 0, sizeof(struct sockaddr_un));

	server_addr.sun_family = AF_UNIX;
	strncpy(server_addr.sun_path, sockname, sizeof(server_addr.sun_path));

	ret = bind(sfd, (struct sockaddr*) &server_addr, sizeof(struct sockaddr_un));
	if (ret == -1) {
		ksp->func.error_exit("koikoi_socket_server_init: bind");
	}

	ret = listen(sfd, 128);
	if (ret == -1) {
		ksp->func.error_exit("koikoi_socket_server_init: listen");
	}

	dupname = strdup(sockname);
	if (dupname == NULL) {
		ksp->func.error_exit("koikoi_socket_server_init: strdup");
	}

	ksp->prot.socket.fds[0] = sfd;
	ksp->prot.socket.name = dupname;
}

void koikoi_socket_server_finalize(koikoi_state_t *ksp)
{
	int sfd;
	int ret;

	sfd = ksp->prot.socket.fds[0];

	ret = shutdown(sfd, SHUT_RDWR);
	if (ret == -1) {
		ksp->func.error_exit("koikoi_socket_server_finalize: shutdown");
	}

	ret = close(sfd);
	if (ret == -1) {
		ksp->func.error_exit("koikoi_socket_server_finalize: close");
	}

	ret = unlink(ksp->prot.socket.name);
	if (ret == -1) {
		ksp->func.error_exit("koikoi_socket_server_finalize: unlink");
	}

	free(ksp->prot.socket.name);

	ksp->prot.type = KOIKOI_PROT_TYPE_INVALID;
	ksp->prot.socket.fds[0] = -1;
	ksp->prot.socket.name = NULL;
}

void koikoi_socket_server_accept_connection(const int fd_n, koikoi_state_t *ksp)
{
	int sfd, cfd;
	struct sockaddr_un client_addr;
	socklen_t client_addr_size;

	if ((fd_n != 1) && (fd_n != 2)) {
		errno = EBADF;
		ksp->func.error_exit("koikoi_socket_server_accept_connection");
	}

	sfd = ksp->prot.socket.fds[0];
	client_addr_size = sizeof(struct sockaddr_un);

	cfd = accept(sfd, (struct sockaddr*) &client_addr, &client_addr_size);
	if (cfd == -1) {
		ksp->func.error_exit("koikoi_socket_server_accept_connection: accept");
	}

	ksp->prot.socket.fds[fd_n] = cfd;
}

void koikoi_socket_server_close_connection(const int fd_n, koikoi_state_t *ksp)
{
	int cfd;
	int ret;

	if ((fd_n != 1) && (fd_n != 2)) {
		errno = EBADF;
		ksp->func.error_exit("koikoi_socket_server_close_connection");
	}

	cfd = ksp->prot.socket.fds[fd_n];

	ret = shutdown(cfd, SHUT_RDWR);
	if (ret == -1) {
		ksp->func.error_exit("koikoi_socket_server_close_connection: shutdown");
	}

	ret = close(cfd);
	if (ret == -1) {
		ksp->func.error_exit("koikoi_socket_server_close_connection: close");
	}

	ksp->prot.socket.fds[fd_n] = -1;
}

void koikoi_socket_connect(const char *sockname, koikoi_state_t *ksp)
{
	int sfd;
	struct sockaddr_un server_addr;
	size_t len;
	int ret;

	len = strlen(sockname);
	if (len >= sizeof(server_addr.sun_path)) {
		errno = ENAMETOOLONG;
		ksp->func.error_exit("koikoi_socket_connect");
	}

	sfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sfd == -1) {
		ksp->func.error_exit("koikoi_socket_connect: socket");
	}

	memset(&server_addr, 0, sizeof(struct sockaddr_un));

	server_addr.sun_family = AF_UNIX;
	strncpy(server_addr.sun_path, sockname, sizeof(server_addr.sun_path));

	ret = connect(sfd, (const struct sockaddr*) &server_addr, sizeof(struct sockaddr_un));
	if (ret == -1) {
		ksp->func.error_exit("koikoi_socket_connect: connect");
	}

	ksp->prot.socket.fds[0] = sfd;
}

void koikoi_socket_disconnect(koikoi_state_t *ksp)
{
	int sfd = ksp->prot.socket.fds[0];
	int ret;

	ret = shutdown(sfd, SHUT_RDWR);
	if (ret == -1) {
		ksp->func.error_exit("koikoi_socket_disconnect: shutdown");
	}

	ret = close(sfd);
	if (ret == -1) {
		ksp->func.error_exit("koikoi_socket_disconnect: close");
	}

	ksp->prot.socket.fds[0] = -1;
}

int koikoi_socket_cmd_send(const uint8_t len, const uint8_t data[], const int fd_n, koikoi_state_t *ksp)
{
	int fd = ksp->prot.socket.fds[fd_n];
	ssize_t retss;

	retss = write(fd, &len, sizeof(len));
	if (retss == -1) {
		ksp->func.error_exit("koikoi_socket_cmd_send: write");
	}

	retss = write(fd, data, len);
	if (retss == -1) {
		ksp->func.error_exit("koikoi_socket_cmd_send: write");
	}

	return (int) retss;
}

int koikoi_socket_cmd_recv(const uint8_t maxlen, uint8_t data[], const int fd_n, koikoi_state_t *ksp)
{
	int fd = ksp->prot.socket.fds[fd_n];
	uint8_t len;
	ssize_t retss;


	retss = read(fd, &len, sizeof(len));
	if (retss == -1) {
		ksp->func.error_exit("koikoi_socket_cmd_recv: read");
	} else if (len > maxlen) {
		errno = ENOMEM;
		ksp->func.error_exit("koikoi_socket_cmd_recv");
	}

	retss = read(fd, data, len);
	if (retss == -1) {
		ksp->func.error_exit("koikoi_socket_cmd_recv: read");
	}

	return (int) retss;
}
