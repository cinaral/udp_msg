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
template <typename SOCK_T, typename SOCKLEN_T, typename KEY_T, typename VAL_T, size_t KEY_DIM,
          size_t VAL_DIM>
int
send(SOCK_T *sock, sockaddr_in *dest, SOCKLEN_T *dest_size, const KEY_T (&key_arr)[KEY_DIM],
     const VAL_T (&val_arr)[VAL_DIM])
{
	constexpr size_t max_msg_size = 512;                //* the UDP packet should not be too big
	constexpr size_t key_size = sizeof(KEY_T[KEY_DIM]); //* flag size in bytes
	constexpr size_t val_size = sizeof(VAL_T[VAL_DIM]); //* var size in bytes
	constexpr size_t msg_size = key_size + val_size;    //* message size in bytes
	static_assert(msg_size <= max_msg_size);

	static char msg[msg_size];                  //* buffer to hold outgoing packet,
	static var_bT<KEY_T[KEY_DIM]> key_byte_arr; //* buffer to hold flag array
	static var_bT<VAL_T[VAL_DIM]> val_byte_arr; //* buffer to hold var array

	for (size_t i = 0; i < KEY_DIM; ++i) {
		key_byte_arr.v[i] = key_arr[i];
	}

	for (size_t i = 0; i < VAL_DIM; ++i) {
		val_byte_arr.v[i] = val_arr[i];
	}

	for (size_t i = 0; i < key_size; ++i) {
		msg[i] = key_byte_arr.b[i];
	}

	for (size_t i = 0; i < val_size; ++i) {
		msg[key_size + i] = val_byte_arr.b[i];
	}
	return ::sendto(*sock, msg, msg_size, 0, reinterpret_cast<sockaddr *>(dest), *dest_size);
}
} // namespace udp_msg
#endif
