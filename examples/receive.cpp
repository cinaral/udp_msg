#include "udp_msg.hpp"
#include <chrono>
#include <cstdio>

using udp_msg::Real_T;
using udp_msg::size_t;
using key_T = unsigned char;

constexpr char hostname[] = "127.0.0.1";
constexpr unsigned port = 11337;
constexpr size_t key_dim = 1;
constexpr size_t val_dim = 1;
constexpr int timeout_duration = 10000; //* ms

int
main()
{
	//* create a socket
	udp_msg::sock<key_T, Real_T, key_dim, val_dim> soc(hostname, port);
	key_T key_arr[key_dim];
	Real_T val_arr[val_dim];
	bool was_received = false;

	auto start = std::chrono::high_resolution_clock::now();
	auto now = std::chrono::high_resolution_clock::now();
	auto since_start = start - now;
	//* while loop on timeout
	while (since_start < std::chrono::milliseconds(timeout_duration)) {
		//* receive
		if (soc.receive(key_arr, val_arr) > 0) {
			//* print received
			printf("Received 0x%02x: %g from %s:%u\n", key_arr[0], val_arr[0], hostname,
			       port);
			was_received = true;
			break;
		}
		now = std::chrono::high_resolution_clock::now();
		since_start = now - start;
	}

	if (!was_received) {
		printf("No message received after %d ms\n", timeout_duration);
	}
	return 0;
}
