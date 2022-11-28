#include "udp_msg.hpp"
#include <cstdio>
#include <future>
#include <string>
#include <thread>
#include <unistd.h>

using udp_msg::Real_T;
using udp_msg::size_t;

const char hostname[] = "127.0.0.1"; //* host IP (local machine)
constexpr unsigned port = 11337;     //* socket port

/*
 * Example data
 */
enum class Key : unsigned char {
	null = 0x0,
	a = 0x30, //* ASCII: 0
	b = 0x31, //* ASCII: 1
	c = 0x32  //* ASCII: 2
};
constexpr size_t key_dim = 3;
constexpr size_t val_dim = 4;
constexpr Key key_arr_sent[key_dim] = {Key::a, Key::b, Key::c};
constexpr Real_T val_arr_sent[val_dim] = {-2.161e7, -1. / 3, 3.14159265358979323846, 123456789.};

void send_fun();
void receive_fun();
void print_result(Key (&key_arr)[key_dim], Real_T (&val_arr)[val_dim]);

//* create a socket
udp_msg::sock<Key, Real_T, key_dim, val_dim> soc(hostname, port);

int
main()
{
	//* start receive thread
	//auto a3 = std::async(std::launch::async, receive_fun());
	std::thread thread_receive(receive_fun);
	//* send
	std::thread thread_send(send_fun);
	//* join the threads
	thread_send.join();
	thread_receive.join();

	return 0;
}

void
send_fun()
{
	soc.send(key_arr_sent, val_arr_sent);
}

void
receive_fun()
{
	Key key_arr[key_dim];
	Real_T val_arr[val_dim];

	bool did_receive = false;

	while (!did_receive) {
		if (soc.receive(key_arr, val_arr) > 0) {
			did_receive = true;
		}
	}
	//* verify
	print_result(key_arr, val_arr);

	for (size_t i = 0; i < key_dim; ++i) {
		if (key_arr[i] != key_arr_sent[i]) {
			exit(1);
		}
	}

	for (size_t i = 0; i < val_dim; ++i) {
		if (val_arr[i] != val_arr_sent[i]) {
			exit(1);
		}
	}
}

void
print_result(Key (&key_arr)[key_dim], Real_T (&val_arr)[val_dim])
{
	printf("Received");

	for (size_t i = 0; i < key_dim; ++i) {
		printf(" 0x%02x", static_cast<unsigned char>(key_arr[i]));
	}
	printf(":");

	for (size_t i = 0; i < val_dim; ++i) {
		printf(" %g", val_arr[i]);
	}
	printf(" to %s:%u\n", hostname, port);
}