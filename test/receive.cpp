#include "test_config.hpp"

//* setup
const std::string test_name = "receive";
const std::string dat_prefix = test_config::dat_dir + "/" + test_name + "-";
const std::string ref_dat_prefix = test_config::ref_dat_dir + "/" + test_name + "-";
const std::string hostname = "127.0.0.1";
constexpr unsigned port = 11337;

using test_config::Flag;

int
main()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	udp_msg::receiver<SOCKET, Flag, float, test_config::flag_dim, test_config::var_dim> udp(hostname, port);
#else
	udp_msg::receiver<int, Flag, float, test_config::flag_dim, test_config::var_dim> udp(hostname, port);
#endif

	Flag flag[test_config::flag_dim];
	float val[test_config::var_dim];

	while (true) {

		if (udp.receive(flag, val)) {
			std::cout << "Received" << std::hex;

			for (size_t i = 0; i < test_config::flag_dim; ++i) {
				std::cout << " 0x" << +static_cast<unsigned char>(flag[i]);
			}

			std::cout << ":" << std::dec;

			for (size_t i = 0; i < test_config::var_dim; ++i) {
				std::cout << " " << val[i];
			}
			std::cout << " to " << hostname << ":" << port << std::endl;
		}
	}

	return 0;
}
