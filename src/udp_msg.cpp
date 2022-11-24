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
#include <iomanip>
#include <iostream>
#include <string>

// constexpr char default_hostname[] = "127.0.0.1";
// constexpr unsigned default_port = 11337;

enum class Key : unsigned char {
	a = 0x30, //* ASCII: 0
	b = 0x31, //* ASCII: 1
	c = 0x32  //* ASCII: 2
};
constexpr size_t key_dim = 1;
constexpr size_t val_dim = 1;

const std::string default_IP = "127.0.0.1";
const std::string default_key{static_cast<unsigned char>(Key::a)};
const std::string default_val = std::to_string(3.14159265358979323846);
const std::string default_port = std::to_string(11337);

// void print_telemetry(config::Flag flag_recv, float val_recv);

int
main(int argc, char const *argv[])
{
	cxxopts::Options options(
	    "send_udp",
	    "Send a single 5-byte flag-value pair UDP packet to a given IP and port. \nThe "
	    "available flags are defined in shared/config.hpp.");
	options.positional_help("[ip]");
	options.show_positional_help();
	// clang-format off
	options.add_options()
		("h,help", "Print usage")
		("ip", "IP Address (e.g. \"192.168.1.1\")", cxxopts::value<std::string>()->default_value(default_IP))
		("l,listen", "Enable listening mode")
		("p,port", "Port", cxxopts::value<std::string>()->default_value(default_port))
		("f,flag", "Key", cxxopts::value<std::string>()->default_value(default_key))
		("v,value", "Value", cxxopts::value<std::string>()->default_value(default_val));
	// clang-format on
	options.parse_positional({"ip"});

	try {
		auto result = options.parse(argc, argv);

		if (result.count("help")) {
			std::cout << options.help() << std::endl;
			return 0;
		}

		if (result.count("listen")) {
			std::cout << options.help() << std::endl;
			return 0;
		}

		const std::string hostname = result["ip"].as<std::string>();
		const unsigned port = std::stoi(result["port"].as<std::string>());
		const Key key[] = {result["key"].as<std::string>().c_str()[0]};
		const float value[] = {std::stof(result["value"].as<std::string>())};

		udp_msg::sock<Key, float, key_dim, val_dim> soc(hostname.c_str(), port);

		soc.send(key, val_arr);

		////* send UDP packet
		// UdpHandler udp(hostname, port);
		// udp.initialize();
		// udp.write(static_cast<config::Flag>(flag), value);

		// std::cout << "Sent 0x" << std::hex << +flag << ": " << std::dec << value << " to
		// "
		//	  << hostname << ":" << port << std::endl;

		////* echo if needed
		// if (flag == static_cast<unsigned char>(config::Flag::echo)) {
		//	config::Flag flag_recv = config::Flag::null;
		//	float val_recv = 0;

		//	for (int i = 0; i < config::echo_size; ++i) {
		//		flag_recv = udp.read(&val_recv);
		//		print_telemetry(flag_recv, val_recv);
		//	}
		//}
	} catch (cxxopts::option_has_no_value_exception const &) {
		std::cout << "Incorrect arguments!" << std::endl;
		std::cout << options.help() << std::endl;
		return 0;
	}

	return 0;
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
#ifdef WIN_COMPAT
	udp_msg::sock<Flag, float, KEY_DIM, VAL_DIM> udp(hostname, port, false);
#else
	udp_msg::sock<Flag, float, KEY_DIM, VAL_DIM> udp(hostname, port, false);
#endif
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
