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

#include "sock_compat.hpp"
#include "types.hpp"
#include "unpack_msg.hpp"

namespace udp_msg
{
/*
 * Receives a UDP message to a socket.
 *
 * `receive(sock, dest, dest_size, OUT:key_arr, OUT:val_arr)`
 *
 * 1. `sock`: socket
 * 2. `from`: sender's address
 * 3. `from_size`: size of `from`
 *
 * OUT:
 * 4. `key_arr`: array of keys
 * 5. `val_arr`: array of values
 */
template <typename SOCK_T, typename SOCKLEN_T, typename KEY_T, typename VAL_T, size_t KEY_DIM,
          size_t VAL_DIM>
int
receive(const SOCK_T &sock, sockaddr *const from, SOCKLEN_T *const from_size,
        KEY_T (&key_arr)[KEY_DIM], VAL_T (&val_arr)[VAL_DIM])
{
	constexpr size_t key_size = sizeof(KEY_T[KEY_DIM]); //* flag size in bytes
	constexpr size_t val_size = sizeof(VAL_T[VAL_DIM]); //* variable size in bytes
	constexpr size_t msg_size = key_size + val_size;    //* message size in bytes
	static char msg[msg_size];                          //* buffer to hold incoming packet

	//* receive message
	int res = ::recvfrom(sock, msg, msg_size, 0, from, from_size);

	//* if received data
	if (res > 0) {
		//* unpack keys and values from a msg
		unpack_msg<KEY_T, VAL_T, KEY_DIM, VAL_DIM>(msg, key_arr, val_arr);
	}
	return res;
}
} // namespace udp_msg
#endif
