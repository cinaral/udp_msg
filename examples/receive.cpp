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

void print_result(Flag (&key_arr)[KEY_DIM], float (&val_arr)[VAL_DIM]);

int
main()
{
	udp_msg::sock<Flag, float, KEY_DIM, VAL_DIM> udp(hostname, port);

	Flag key_arr[KEY_DIM];
	float val_arr[VAL_DIM];

	while (true) {

		if (udp.receive(key_arr, val_arr)) {
			print_result(key_arr, val_arr);
		}
	}
	return 0;
}

void
print_result(Flag (&key_arr)[KEY_DIM], float (&val_arr)[VAL_DIM])
{
	printf("Received");

	for (size_t i = 0; i < KEY_DIM; ++i) {
		printf(" 0x%02x", static_cast<unsigned char>(key_arr[i]));
	}
	printf(":");

	for (size_t i = 0; i < VAL_DIM; ++i) {
		printf(" %g", val_arr[i]);
	}
	printf(" to %s:%u\n", hostname, port);
}