#include <koikoi.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

int main()
{
	koikoi_state_t ks;
	uint8_t data[48];
	int len;
	int i;

	koikoi_init(&ks);
	koikoi_socket_server_init("sock", &ks);
	koikoi_socket_server_accept_connection(1, &ks);

	len = koikoi_socket_cmd_recv(sizeof(data), data, 1, &ks);
	printf("client -> server: len = %d\n", len);
	for (i = 0; i < len; i ++)
		printf("  %-2d: %" PRIx8 "\n", i, data[i]);

	len = 0;
	data[len++] = KOIKOI_CMD_INVALID;
	printf("server -> client: len = %d\n", len);
	for (i = 0; i < len; i ++)
		printf("  %-2d: %" PRIx8 "\n", i, data[i]);
	koikoi_socket_cmd_send(len, data, 1, &ks);

	koikoi_socket_server_close_connection(1, &ks);
	koikoi_socket_server_finalize(&ks);
	koikoi_finalize(&ks);

	return 0;
}
