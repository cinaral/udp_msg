/*
 * udp_msg
 *
 * MIT License
 *
 * Copyright (c) 2022 cinaral
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef SOCKET_HPP_CINARAL_221122_1423
#define SOCKET_HPP_CINARAL_221122_1423

#include "compat_config.hpp"
#include "receive.hpp"
#include "send.hpp"

namespace udp_msg
{
template <typename SOCK_T, typename SOCKLEN_T, typename FLAG_T, typename VAR_T, size_t FLAG_DIM,
          size_t VAR_DIM>
class socket
{
  public:
	socket(const char hostname[], const unsigned port, const bool is_receiving)
	{
		start_socket<SOCK_T, SOCKLEN_T>(hostname, port, &sock_, &dest_, &dest_size_);

		if (is_receiving) {
			bind_socket<SOCK_T, SOCKLEN_T>(&sock_, &dest_);
		}
	};
	~socket()
	{
		close_socket<SOCK_T>(&sock_);
	};

	bool
	receive(FLAG_T (&flag_arr)[FLAG_DIM], VAR_T (&var_arr)[VAR_DIM])
	{
		return udp_msg::receive<SOCK_T, SOCKLEN_T, FLAG_T, VAR_T, FLAG_DIM, VAR_DIM>(
		    &sock_, &dest_, &dest_size_, flag_arr, var_arr);
	};

	bool
	send(const FLAG_T (&flag_arr)[FLAG_DIM], const VAR_T (&var_arr)[VAR_DIM])
	{
		return udp_msg::send<SOCK_T, SOCKLEN_T, FLAG_T, VAR_T, FLAG_DIM, VAR_DIM>(
		    &sock_, &dest_, &dest_size_, flag_arr, var_arr);
	};

  private:
	SOCK_T sock_;
	sockaddr_in dest_;
	SOCKLEN_T dest_size_;
};
} // namespace udp_msg

#endif