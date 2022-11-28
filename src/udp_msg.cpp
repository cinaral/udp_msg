/*
 * udp_msg
 *
 * MIT License
 *
 * Copyright (c) 2022 cinaral
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "udp_msg.hpp"
#include "cxxopts.hpp"
#include <cstdio>
#include <string>
#include <thread>

//* IP and port
constexpr char hostname[] = "127.0.0.1";
constexpr unsigned port = 11337;

using udp_msg::Real_T;
using udp_msg::size_t;

enum class Key : unsigned char {
	a = 0x30, //* ASCII: 0
	b = 0x31, //* ASCII: 1
	c = 0x32  //* ASCII: 2
};
constexpr size_t key_dim = 1;
constexpr size_t val_dim = 1;

const std::string default_IP = "127.0.0.1";
const std::string default_port = std::to_string(11337);
const std::string default_key{static_cast<unsigned char>(Key::a)};
const std::string default_val = std::to_string(3.14159265358979323846);
const std::string default_timeout = std::to_string(5000);

bool is_listening = false;
// void print_telemetry(config::Flag flag_recv, float val_recv);
void receive_fun();

int
main(int argc, char const *argv[])
{
	cxxopts::Options opts(
	    "udp_msg", "Send or receive a flag-value pair UDP packet to a given IP and port.");
	opts.positional_help("[ip]");
	opts.show_positional_help();
	// clang-format off
	opts.add_options()
		("h,help", "Print usage")
		("ip", "IP Address (e.g. \"192.168.1.1\")", cxxopts::value<std::string>()->default_value(default_IP))
		("l,listen", "Enable listening mode")
		("p,port", "Port", cxxopts::value<std::string>()->default_value(default_port))
		("f,flag", "Key", cxxopts::value<std::string>()->default_value(default_key))
		("v,value", "Value", cxxopts::value<std::string>()->default_value(default_val));
		("t,timeout", "Timeout", cxxopts::value<std::string>()->default_value(default_timeout));
	// clang-format on
	opts.parse_positional({"ip"});

	try {
		//* parse the options
		auto result = opts.parse(argc, argv);

		if (result.count("help")) {
			std::cout << opts.help() << std::endl;
			return 0;
		}

		if (result.count("listen")) {
			is_listening = true;
		}
		const std::string hostname = result["ip"].as<std::string>();
		const unsigned port = std::stoi(result["port"].as<std::string>());
		const Key key[] = {static_cast<Key>(result["key"].as<std::string>().c_str()[0])};
		const Real_T value[] = {std::stof(result["value"].as<std::string>())};
		const int timeout_duration = std::stoi(result["timeout"].as<std::string>());
		//* open the socket
		udp_msg::sock<Key, Real_T, key_dim, val_dim> soc(hostname.c_str(), port,
		                                                 is_listening);

		if (is_listening) {
			std::thread thread_receive(receive_fun, std::ref(soc),
			                           std::cref(timeout_duration));

			//* send UDP packet
			soc.send(key, value);
			printf("Sent 0x%02x: %g to %s:%u\n", key[0], value[0], hostname.c_str(),
			       port);

		} else {
			//* send UDP packet
			soc.send(key, value);
			printf("Sent 0x%02x: %g to %s:%u\n", key[0], value[0], hostname.c_str(),
			       port);
		}

	} catch (cxxopts::option_has_no_value_exception const &) {
		std::cout << "Incorrect arguments!" << std::endl;
		std::cout << opts.help() << std::endl;
	}
	return 0;
}

void
receive_fun(udp_msg::sock<Key, Real_T, key_dim, val_dim> &soc, const int &timeout_duration)
{
	Key key_arr[key_dim];
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
			printf("Received 0x%02x: %g to %s:%u\n", key_arr[0], val_arr[0], hostname,
			       port);
			break;
		}
	}
	if (was_received) {
		printf("No message received after %d ms\n", timeout_duration);
	}
}

// void
// print_telemetry(config::Flag flag_recv, float val_recv)
//{
//	constexpr int tag_width = 15;
//	constexpr int hex_width = 4;

//	std::cout << std::left << std::setw(tag_width);

//	//* print known flags
//	switch (flag_recv) {
//	case config::Flag::t_idx:
//		std::cout << "t_idx ";
//		break;
//	case config::Flag::t:
//		std::cout << "t ";
//		break;
//	case config::Flag::x:
//		std::cout << "x ";
//		break;
//	case config::Flag::u:
//		std::cout << "u ";
//		break;
//	case config::Flag::r:
//		std::cout << "r ";
//		break;
//	case config::Flag::K_p:
//		std::cout << "K_p ";
//		break;
//	case config::Flag::K_d:
//		std::cout << "K_d ";
//		break;
//	case config::Flag::max_sample_us:
//		std::cout << "max_sample_us ";
//		break;
//	default:
//		break;
//	}

//	std::cout << "0x" << std::hex << +static_cast<unsigned char>(flag_recv) << ": "
//		  << std::setw(hex_width) << std::dec << val_recv << std::endl;
//}