#include <koikoi.h>
#include <stdio.h>
#include <inttypes.h>

int main()
{
	koikoi_state_t ks;
	uint8_t data[48];
	int len;
	int i;

	koikoi_init(&ks);
	koikoi_socket_connect("sock", &ks);

	len = 0;
	data[len++] = KOIKOI_CMD_HELLO;
	printf("client -> server: len = %d\n", len);
	for (i = 0; i < len; i ++)
		printf("  %-2d: %" PRIx8 "\n", i, data[i]);
	koikoi_socket_cmd_send(i, data, 0, &ks);

	len = koikoi_socket_cmd_recv(sizeof(data), data, 0, &ks);
	printf("server -> client: len = %d\n", len);
	for (i = 0; i < len; i ++)
		printf("  %-2d: %" PRIx8 "\n", i, data[i]);

	koikoi_socket_disconnect(&ks);
	koikoi_finalize(&ks);

	return 0;
}
