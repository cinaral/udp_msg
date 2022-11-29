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
constexpr key_T key_sent[key_dim] = {0x30}; //* ASCII: 0
constexpr Real_T val_sent[val_dim] = {3.14};

int
main()
{
	//* create a socket
	udp_msg::sock<key_T, Real_T, key_dim, val_dim> udp(hostname, port, false);
	//* send
	if (udp.send(key_sent, val_sent) > 0) {
		//* print sent
		printf("Sent 0x%02x: %g to %s:%u\n", key_sent[0], val_sent[0], hostname, port);
	}
	return 0;
}
