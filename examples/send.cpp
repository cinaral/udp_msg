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

constexpr size_t KEY_DIM = 3;
constexpr size_t VAL_DIM = 4;
constexpr Flag flag_arr_sent[KEY_DIM] = {Flag::a, Flag::b, Flag::c};
constexpr float var_arr_sent[VAL_DIM] = {-2.161e7, -1. / 3, 3.14, 123456789.};

void print_result();

int
main()
{

	udp_msg::sock<Flag, float, KEY_DIM, VAL_DIM> udp(hostname, port, false);

	udp.send(flag_arr_sent, var_arr_sent);
	print_result();

	return 0;
}

void
print_result()
{
	printf("Sent");

	for (size_t i = 0; i < KEY_DIM; ++i) {
		printf(" 0x%02x", static_cast<unsigned char>(flag_arr_sent[i]));
	}
	printf(":");

	for (size_t i = 0; i < VAL_DIM; ++i) {
		printf(" %g", var_arr_sent[i]);
	}
	printf(" to %s:%u\n", hostname, port);
}