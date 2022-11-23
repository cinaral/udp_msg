#include "test_config.hpp"

//* setup
using namespace test_config;
const std::string test_name = "send";
const std::string dat_prefix = dat_dir + "/" + test_name + "-";
const std::string ref_dat_prefix = ref_dat_dir + "/" + test_name + "-";
void print_result();

int
main()
{
#ifdef WIN_COMPAT
	udp_msg::socket<SOCKET, int, Flag, float, flag_dim, var_dim> udp(hostname, port, false);
#else
	udp_msg::socket<int, socklen_t, Flag, float, flag_dim, var_dim> udp(hostname, port, false);
#endif
	udp.send(flag_arr_sent, var_arr_sent);
	print_result();

	return 0;
}

void
print_result()
{
	printf("Sent");

	for (size_t i = 0; i < flag_dim; ++i) {
		printf(" 0x%02x", static_cast<unsigned char>(flag_arr_sent[i]));
	}
	printf(":");

	for (size_t i = 0; i < var_dim; ++i) {
		printf(" %g", var_arr_sent[i]);
	}
	printf(" to %s:%u\n", hostname, port);
}