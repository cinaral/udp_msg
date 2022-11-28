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

#include "receive.hpp"
#include "send.hpp"
#include "sock_compat.hpp"
#include "types.hpp"
#include <fcntl.h>

namespace udp_msg
{

/*
 * A socket class to send and receive messages.
 */
template <typename KEY_T, typename VAL_T, size_t KEY_DIM, size_t VAL_DIM> class sock
{
  public:
	sock(const char hostname[], const unsigned port, const bool is_binding = true,
	     const bool is_nonblocking = true, const int af = AF_INET, const int type = SOCK_DGRAM,
	     const int protocol = 0)
	{
		//* create the socket
		sock_ = ::udp_msg::socket(af, type, protocol);

		if (sock_ > 0) {
			//* specify the host
			host_in_.sin_family = AF_INET;
			host_in_.sin_addr.s_addr = inet_addr(hostname);
			host_in_.sin_port = htons(port);
			host_ = reinterpret_cast<sockaddr *>(&host_in_);
			host_size_ = sizeof(host_in_);

			//* bind to port in order to receive, this is not required to send
			if (is_binding) {
				if (::bind(sock_, host_, host_size_) < 0) {
					printf("Binding failed.\n");
				}
			}
			//* do not block to send and receive
			if (is_nonblocking) {
				::udp_msg::set_nonblocking(sock_);
			}

		} else {
			printf("Socket creation failed.\n");
			::udp_msg::close(sock_);
		}
	};
	~sock()
	{
		//* close the socket
		::udp_msg::close(sock_);
	};

	/*
	 * Receives a UDP message of size KEY_DIM + VAL_DIM
	 *
	 * `receive(OUT:key_arr, OUT:val_arr)`
	 *
	 * OUT:
	 * 1. `key_arr`: array of keys
	 * 2. `val_arr`: array of values
	 */
	int
	receive(KEY_T (&key_arr)[KEY_DIM], VAL_T (&val_arr)[VAL_DIM])
	{
		return ::udp_msg::receive(sock_, host_, &host_size_, key_arr, val_arr);
	};

	/*
	 * Sends a UDP message of size KEY_DIM + VAL_DIM
	 *
	 * `receive(key_arr, val_arr)`
	 *
	 * IN:
	 * 1. `key_arr`: array of keys
	 * 2. `val_arr`: array of values
	 */
	int
	send(const KEY_T (&key_arr)[KEY_DIM], const VAL_T (&val_arr)[VAL_DIM]) const
	{
		return ::udp_msg::send(sock_, host_, host_size_, key_arr, val_arr);
	};

  private:
	SOCKET sock_;
	sockaddr_in host_in_;
	sockaddr *host_;
	socklen_t host_size_;
};
} // namespace udp_msg

#endif