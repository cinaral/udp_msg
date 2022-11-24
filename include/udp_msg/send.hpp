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

#ifndef SENDER_HPP_CINARAL_221122_1423
#define SENDER_HPP_CINARAL_221122_1423

#include "compat_config.hpp"
#include "types.hpp"

namespace udp_msg
{
template <typename SOCK_T, typename SOCKLEN_T, typename FLAG_T, typename VAR_T, size_t FLAG_DIM,
          size_t VAR_DIM>
int
send(SOCK_T *sock, sockaddr_in *dest, SOCKLEN_T *dest_size, const FLAG_T (&flag_arr)[FLAG_DIM],
     const VAR_T (&var_arr)[VAR_DIM])
{
	constexpr size_t max_msg_size = 512; //* the UDP packet should not be too big
	constexpr size_t flag_size = sizeof(FLAG_T[FLAG_DIM]); //* flag size in bytes
	constexpr size_t var_size = sizeof(VAR_T[VAR_DIM]);    //* var size in bytes
	constexpr size_t msg_size = flag_size + var_size;      //* message size in bytes
	static_assert(msg_size <= max_msg_size);

	static char msg[msg_size];                     //* buffer to hold outgoing packet,
	static var_bT<FLAG_T[FLAG_DIM]> flag_byte_arr; //* buffer to hold flag array
	static var_bT<VAR_T[VAR_DIM]> var_byte_arr;    //* buffer to hold var array

	for (size_t i = 0; i < FLAG_DIM; ++i) {
		flag_byte_arr.v[i] = flag_arr[i];
	}

	for (size_t i = 0; i < VAR_DIM; ++i) {
		var_byte_arr.v[i] = var_arr[i];
	}

	for (size_t i = 0; i < flag_size; ++i) {
		msg[i] = flag_byte_arr.b[i];
	}

	for (size_t i = 0; i < var_size; ++i) {
		msg[flag_size + i] = var_byte_arr.b[i];
	}
	return ::sendto(*sock, msg, msg_size, 0, reinterpret_cast<sockaddr *>(dest), *dest_size);
}
} // namespace udp_msg
#endif
