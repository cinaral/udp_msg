#include "test_config.hpp"

//* setup
using namespace test_config;
const std::string test_name = "receive";
const std::string dat_prefix = dat_dir + "/" + test_name + "-";
const std::string ref_dat_prefix = ref_dat_dir + "/" + test_name + "-";
void print_result(Flag (&flag_arr)[flag_dim], float (&var_arr)[var_dim]);

int
main()
{
#ifdef WIN_COMPAT
	udp_msg::socket<SOCKET, int, Flag, float, flag_dim, var_dim> udp(hostname, port, true);
#else
	udp_msg::socket<int, socklen_t, Flag, float, flag_dim, var_dim> udp(hostname, port, true);
#endif
	Flag flag_arr[flag_dim];
	float var_arr[var_dim];

	while (true) {

		if (udp.receive(flag_arr, var_arr)) {
			print_result(flag_arr, var_arr);
		}
	}
	return 0;
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