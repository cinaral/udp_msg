/*
 * udp_msg
 *
 * MIT License
 *
 * Copyright (c) 2022 Cinar, A. L.
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

#include "pack_msg.hpp"
#include "sock_compat.hpp"
#include "types.hpp"

namespace udp_msg
{
/*
 * Sends a UDP message from a socket.
 *
 * `send<OPT: SOCK_T, SOCKLEN_T, KEY_T, VAL_T, KEY_DIM, VAL_DIM>(sock, dest, dest_size, OUT:key_arr, OUT:val_arr)`
 *
 * 1. `sock`: socket
 * 2. `dest`: receiver's address
 * 3. `dest_size`: size of `dest`
 *
 * OUT:
 * 4. `key_arr`: array of keys
 * 5. `val_arr`: array of values
 */
template <typename SOCK_T, typename SOCKLEN_T, typename KEY_T, typename VAL_T, size_t KEY_DIM,
          size_t VAL_DIM>
int
send(const SOCK_T &sock, const sockaddr *const dest, const SOCKLEN_T &dest_size,
     const KEY_T (&key_arr)[KEY_DIM], const VAL_T (&val_arr)[VAL_DIM])
{
	constexpr size_t max_msg_size = 512;                //* the UDP packet should not be too big
	constexpr size_t key_size = sizeof(KEY_T[KEY_DIM]); //* flag size in bytes
	constexpr size_t val_size = sizeof(VAL_T[VAL_DIM]); //* var size in bytes
	constexpr size_t msg_size = key_size + val_size;    //* message size in bytes
	static char msg[msg_size];                          //* buffer to hold outgoing packet,

	//* check if the message is too big
	static_assert(msg_size <= max_msg_size, "msg_size is too big");

	//* pack keys and values into a msg
	pack_msg(key_arr, val_arr, msg);

	//* send message
	return ::sendto(sock, msg, msg_size, 0, dest, dest_size);
}
} // namespace udp_msg
#endif
