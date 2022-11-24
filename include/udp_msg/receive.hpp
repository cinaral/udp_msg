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

#include "compat_config.hpp"
#include "types.hpp"

namespace udp_msg
{
template <typename SOCK_T, typename SOCKLEN_T, typename KEY_T, typename VAL_T, size_t KEY_DIM,
          size_t VAL_DIM>
int
receive(SOCK_T *sock, sockaddr_in *dest, SOCKLEN_T *dest_size, KEY_T (&key_arr)[KEY_DIM],
        VAL_T (&val_arr)[VAL_DIM])
{
	constexpr size_t key_size = sizeof(KEY_T[KEY_DIM]); //* flag size in bytes
	constexpr size_t val_size = sizeof(VAL_T[VAL_DIM]); //* variable size in bytes
	constexpr size_t msg_size = key_size + val_size;    //* message size in bytes

	static char msg[msg_size]; //* buffer to hold incoming packet
	static var_bT<KEY_T[KEY_DIM]> key_byte_arr;
	static var_bT<VAL_T[VAL_DIM]> val_byte_arr;

	int res =
	    ::recvfrom(*sock, msg, msg_size, 0, reinterpret_cast<sockaddr *>(dest), dest_size);

	if (res > 0) {
		for (size_t i = 0; i < key_size; ++i) {
			key_byte_arr.b[i] = msg[i];
		}

		for (size_t i = 0; i < val_size; ++i) {
			val_byte_arr.b[i] = msg[key_size + i];
		}

		for (size_t i = 0; i < KEY_DIM; ++i) {
			key_arr[i] = key_byte_arr.v[i];
		}

		for (size_t i = 0; i < VAL_DIM; ++i) {
			val_arr[i] = val_byte_arr.v[i];
		}
	}
	return res;
}
} // namespace udp_msg
#endif