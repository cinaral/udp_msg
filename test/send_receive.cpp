#include "udp_msg.hpp"
#include <cstdio>
#include <string>
#include <thread>
#include <unistd.h>

#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif

using udp_msg::Real_T;
using udp_msg::size_t;

//* IP and port
const char hostname[] = "127.0.0.1";
constexpr unsigned port = 11337;

enum class Key : unsigned char {
	null = 0x0,
	a = 0x30, //* ASCII: 0
	b = 0x31, //* ASCII: 1
	c = 0x32  //* ASCII: 2
};

constexpr size_t KEY_DIM = 3;
constexpr size_t VAL_DIM = 4;
constexpr Key key_arr_sent[KEY_DIM] = {Key::a, Key::b, Key::c};
constexpr float val_arr_sent[VAL_DIM] = {-2.161e7, -1. / 3, M_PI, 123456789.};

void send_fun();
void receive_fun();
void print_result(Key (&key_arr)[KEY_DIM], float (&val_arr)[VAL_DIM]);

udp_msg::sock<Key, float, KEY_DIM, VAL_DIM> udp(hostname, port);

int
main()
{
	//* send and receive
	std::thread thread_receive(receive_fun);
	std::thread thread_send(send_fun);
	thread_send.join();
	thread_receive.join();

	return 0;
}

void
send_fun()
{
	udp.send(key_arr_sent, val_arr_sent);
}

void
receive_fun()
{
	Key key_arr[KEY_DIM];
	float val_arr[VAL_DIM];

	bool did_receive = false;

	while (!did_receive) {
		if (udp.receive(key_arr, val_arr) > 0) {
			did_receive = true;
		}
	}

	//* verify
	print_result(key_arr, val_arr);

	for (size_t i = 0; i < KEY_DIM; ++i) {
		if (key_arr[i] != key_arr_sent[i]) {
			exit(1);
		}
	}

	for (size_t i = 0; i < VAL_DIM; ++i) {
		if (val_arr[i] != val_arr_sent[i]) {
			exit(1);
		}
	}
}

void
print_result(Key (&key_arr)[KEY_DIM], float (&val_arr)[VAL_DIM])
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