#include "test_config.hpp"

//* setup
const std::string test_name = "send";
const std::string dat_prefix = test_config::dat_dir + "/" + test_name + "-";
const std::string ref_dat_prefix = test_config::ref_dat_dir + "/" + test_name + "-";
const std::string hostname = "127.0.0.1";
constexpr unsigned port = 11337;

using test_config::Flag;

int
main()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	udp_msg::sender<SOCKET, Flag, float, test_config::flag_dim, test_config::var_dim> udp(hostname, port);
#else
	udp_msg::sender<int, Flag, float, test_config::flag_dim, test_config::var_dim> udp(hostname, port);
#endif

	udp.send(test_config::flag_arr_sent, test_config::var_arr_sent);
	std::cout << "Sent" << std::hex;

	for (size_t i = 0; i < test_config::flag_dim; ++i) {
		std::cout << " 0x" << +static_cast<unsigned char>(test_config::flag_arr_sent[i]);
	}
	std::cout << ":" << std::dec;

	for (size_t i = 0; i < test_config::var_dim; ++i) {
		std::cout << " " << test_config::var_arr_sent[i];
	}
	std::cout << " to " << hostname << ":" << port << std::endl;
		
	return 0;
}
