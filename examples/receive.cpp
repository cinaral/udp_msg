#include "udp_msg.hpp"
#include <cstdio>

//* IP and port
constexpr char hostname[] = "127.0.0.1";
constexpr unsigned port = 11337;

enum class Flag : unsigned char {
	null = 0x0,
	a = 0x30, //* ASCII: 0
	b = 0x31, //* ASCII: 1
	c = 0x32  //* ASCII: 2
};

constexpr size_t flag_dim = 3;
constexpr size_t var_dim = 4;

void print_result(Flag (&flag_arr)[flag_dim], float (&var_arr)[var_dim]);

int
main()
{
#ifdef WIN_COMPAT
	udp_msg::socket<SOCKET, int, Flag, float, flag_dim, var_dim> udp(hostname, port, true);
#else
	udp_msg::socket<int, socklen_t, Flag, float, flag_dim, var_dim> udp(hostname, port, true);
#endif
	Flag flag_arr[flag_dim];
	float var_arr[var_dim];

	while (true) {

		if (udp.receive(flag_arr, var_arr)) {
			print_result(flag_arr, var_arr);
		}
	}
	return 0;
}

void
print_result(Flag (&flag_arr)[flag_dim], float (&var_arr)[var_dim])
{
	printf("Received");

	for (size_t i = 0; i < flag_dim; ++i) {
		printf(" 0x%02x", static_cast<unsigned char>(flag_arr[i]));
	}
	printf(":");

	for (size_t i = 0; i < var_dim; ++i) {
		printf(" %g", var_arr[i]);
	}
	printf(" to %s:%u\n", hostname, port);
}