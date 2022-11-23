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

#ifndef RECEIVER_HPP_CINARAL_221122_1423
#define RECEIVER_HPP_CINARAL_221122_1423

#include "receive.hpp"
#include <string>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	#include <winsock2.h>
#else
	#include <arpa/inet.h>  //* htons, inet_addr
	#include <netinet/in.h> //* sockaddr_in
	#include <sys/socket.h> //* socket, sendto
	#include <unistd.h>     //* close
#endif

namespace udp_msg
{
template <typename SOCK_T, typename FLAG_T, typename VAR_T, size_t FLAG_DIM, size_t VAR_DIM> class receiver
{
  public:
	receiver(std::string hostname, const uint16_t port)
	{
		sock_ = ::socket(AF_INET, SOCK_DGRAM, 0); //* start socket
		dest_.sin_family = AF_INET;
		dest_.sin_addr.s_addr = inet_addr(hostname.c_str());
		dest_.sin_port = htons(port);
		dest_size_ = sizeof(dest_);

		if (bind(sock_, (struct sockaddr *)&dest_, sizeof(dest_)) < 0) {
			perror("bind");
			exit(1);
		}
	};
	~receiver()
	{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
		::closesocket(sock_);
#else
		::close(sock_);
#endif
	};

	bool
	receive(FLAG_T (&flag_arr)[FLAG_DIM], VAR_T (&var_arr)[VAR_DIM])
	{
		return udp_msg::receive<SOCK_T, FLAG_T, VAR_T, FLAG_DIM, VAR_DIM>(&sock_, &dest_, &dest_size_, flag_arr,
		                                                                  var_arr);
	};

  private:
	SOCK_T sock_;
	sockaddr_in dest_;
	socklen_t dest_size_;
};
} // namespace udp_msg

#endif