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
constexpr Flag flag_arr_sent[flag_dim] = {Flag::a, Flag::b, Flag::c};
constexpr float var_arr_sent[var_dim] = {-2.161e7, -1. / 3, 3.14, 123456789.};

void print_result();

int
main()
{
#ifdef WIN_COMPAT
	udp_msg::sock<Flag, float, flag_dim, var_dim> udp(hostname, port, false);
#else
	udp_msg::sock<Flag, float, flag_dim, var_dim> udp(hostname, port, false);
#endif
	udp.send(flag_arr_sent, var_arr_sent);
	print_result();

	return 0;
}

void
print_result()
{
	printf("Sent");

	for (size_t i = 0; i < flag_dim; ++i) {
		printf(" 0x%02x", static_cast<unsigned char>(flag_arr_sent[i]));
	}
	printf(":");

	for (size_t i = 0; i < var_dim; ++i) {
		printf(" %g", var_arr_sent[i]);
	}
	printf(" to %s:%u\n", hostname, port);
}