#ifndef TEST_CONFIG_HPP_CINARAL_221122_0839
#define TEST_CONFIG_HPP_CINARAL_221122_0839

#include "udp_msg.hpp"
#include <cstdio>
#include <string>

#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif

using udp_msg::Real_T;
using udp_msg::size_t;

namespace test_config
{
//* relative to the directory of the executable
const std::string dat_dir = "../dat";                       //* temporary data directory
const std::string ref_dat_dir = "../../test/reference_dat"; //* reference data directory
const std::string t_arr_fname = "t_arr.dat";                //* file name for the time array
const std::string x_arr_fname = "x_arr.dat";                //* file name for the x array

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
constexpr Flag flag_arr_sent[test_config::flag_dim] = {Flag::a, Flag::b, Flag::c};
constexpr float var_arr_sent[test_config::var_dim] = {-2.161e7, -1. / 3, M_PI, 123456789.};
} // namespace test_config

#endif