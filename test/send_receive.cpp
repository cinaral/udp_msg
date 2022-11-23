#include "udp_msg.hpp"
#include <cstdio>
#include <string>
#include <thread>

#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif

using udp_msg::Real_T;
using udp_msg::size_t;

//* IP and port
const char hostname[] = "127.0.0.1";
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
constexpr float var_arr_sent[var_dim] = {-2.161e7, -1. / 3, M_PI, 123456789.};


void send_fun();
void receive_fun();
void print_result(Flag (&flag_arr)[flag_dim], float (&var_arr)[var_dim]);

#ifdef WIN_COMPAT
udp_msg::socket<SOCKET, int, Flag, float, flag_dim, var_dim> udp(hostname, port, true);
#else
udp_msg::socket<int, socklen_t, Flag, float, flag_dim, var_dim> udp(hostname, port, true);
#endif

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
	udp.send(flag_arr_sent, var_arr_sent);
}

void
receive_fun()
{
	Flag flag_arr[flag_dim];
	float var_arr[var_dim];

	bool did_receive = false;

	while (!did_receive) {
		if (udp.receive(flag_arr, var_arr)) {
			did_receive = true;
		}
	}

	//* verify
	print_result(flag_arr, var_arr);

	for (size_t i = 0; i < flag_dim; ++i) {
		if (flag_arr[i] != flag_arr_sent[i]) {
			exit(1);
		}
	}

	for (size_t i = 0; i < var_dim; ++i) {
		if (var_arr[i] != var_arr_sent[i]) {
			exit(1);
		}
	}
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