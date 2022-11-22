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

#ifndef RECEIVE_HPP_CINARAL_221122_1122
#define RECEIVE_HPP_CINARAL_221122_1122

#include "types.hpp"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	#include <winsock2.h>
#else
	#include <netinet/in.h> //* sockaddr_in
	#include <sys/socket.h> //* socket, sendto
#endif

namespace udp_msg
{
template <typename SOCK_T, typename FLAG_T, typename VAR_T, size_t FLAG_DIM, size_t VAR_DIM>
bool
receive(SOCK_T *sock, sockaddr_in *dest, socklen_t *dest_size, FLAG_T (&cmd_arr)[FLAG_DIM], VAR_T (&var_arr)[VAR_DIM])
{
	constexpr size_t flag_size = sizeof(FLAG_T[FLAG_DIM]); //* flag size in bytes
	constexpr size_t var_size = sizeof(VAR_T[VAR_DIM]);    //* variable size in bytes
	constexpr size_t msg_size = flag_size + var_size;      //* message size in bytes

	static unsigned char msg[msg_size]; //* buffer to hold incoming packet
	static var_bT<FLAG_T[FLAG_DIM]> cmd_byte_arr;
	static var_bT<VAR_T[VAR_DIM]> var_byte_arr;

	if (::recvfrom(*sock, msg, msg_size, 0, reinterpret_cast<sockaddr *>(dest), dest_size) > 0) {

		for (size_t i = 0; i < flag_size; ++i) {
			cmd_byte_arr.b[i] = msg[i];
		}

		for (size_t i = 0; i < var_size; ++i) {
			var_byte_arr.b[i] = msg[flag_size + i];
		}

		for (size_t i = 0; i < FLAG_DIM; ++i) {
			cmd_arr[i] = cmd_byte_arr.v[i];
		}

		for (size_t i = 0; i < VAR_DIM; ++i) {
			var_arr[i] = var_byte_arr.v[i];
		}
		return true;
	} else {
		return false;
	}
}
} // namespace udp_msg
#endif