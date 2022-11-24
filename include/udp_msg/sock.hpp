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
#include <fcntl.h>

namespace udp_msg
{

/*
 * A socket that can be used to send and receive messages.
 *
 * `sock sock_obj(hostname, port, is_receiving, is_nonblocking)`
 */
template <typename FLAG_T, typename VAR_T, size_t FLAG_DIM, size_t VAR_DIM> class sock
{
  public:
	sock(const char hostname[], const unsigned port, const bool is_receiving = true,
	     const bool is_nonblocking = true)
	{
		//* Create the socket
		sock_ = ::udp_msg::socket(AF_INET, SOCK_DGRAM, 0);

		if (sock_ > 0) {
			dest_.sin_family = AF_INET;
			dest_.sin_addr.s_addr = inet_addr(hostname);
			dest_.sin_port = htons(port);
			dest_size_ = sizeof(dest_);

			//* Do not block to send and receive
			if (is_nonblocking) {
				::udp_msg::set_nonblocking(sock_);
			}

			//* Bind to port to receive, this is not required to send
			if (is_receiving) {
				if (::udp_msg::bind(sock_, dest_) < 0) {
					printf("Binding failed.\n");
				}
			}
		} else {
			printf("Socket creation failed.\n");
			::udp_msg::close(sock_);
		}
	};
	~sock()
	{
		//* Close the socket
		::udp_msg::close(sock_);
	};

	/*
	 * Receives a UDP message of size FLAG_DIM + VAR_DIM
	 *
	 * `receive(OUT:flag_arr, OUT:var_arr)`
	 *
	 * OUT:
	 * 1. flag_arr - array of flags
	 * 2. var_arr - array of variables
	 */
	int
	receive(FLAG_T (&flag_arr)[FLAG_DIM], VAR_T (&var_arr)[VAR_DIM])
	{
		return ::udp_msg::receive(&sock_, &dest_, &dest_size_, flag_arr, var_arr);
	};

	/*
	 * Sends a UDP message of size FLAG_DIM + VAR_DIM
	 *
	 * `receive(flag_arr, var_arr)`
	 * 
	 * IN:
	 * 1. flag_arr - array of flags
	 * 2. var_arr - array of variables
	 */
	int
	send(const FLAG_T (&flag_arr)[FLAG_DIM], const VAR_T (&var_arr)[VAR_DIM])
	{
		return ::udp_msg::send(&sock_, &dest_, &dest_size_, flag_arr, var_arr);
	};

  private:
	SOCKET sock_;
	sockaddr_in dest_;
	socklen_t dest_size_;
};
} // namespace udp_msg

#endif