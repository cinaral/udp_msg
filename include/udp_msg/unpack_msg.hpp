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

#ifndef UNPACK_MSG_HPP_CINARAL_221129_1718
#define UNPACK_MSG_HPP_CINARAL_221129_1718

#include "types.hpp"

namespace udp_msg
{
/*
 * Unpacks keys and values from a msg.
 *
 * `unpack_msg<OPT: KEY_T, VAL_T, KEY_DIM, VAL_DIM>(msg, OUT:key_arr, OUT:val_arr)`
*
 * 1. `msg`: array of keys and values
 *
 * OUT:
 * 2. `key_arr`: array of keys
 * 3. `val_arr`: array of values
 */
template <typename KEY_T, typename VAL_T, size_t KEY_DIM, size_t VAL_DIM>
void
unpack_msg(const char (&msg)[sizeof(KEY_T[KEY_DIM]) + sizeof(VAL_T[VAL_DIM])],
           KEY_T (&key_arr)[KEY_DIM], VAL_T (&val_arr)[VAL_DIM])
{
	constexpr size_t key_size = sizeof(KEY_T[KEY_DIM]); //* flag size in bytes
	constexpr size_t val_size = sizeof(VAL_T[VAL_DIM]); //* variable size in bytes
	static var_bT<KEY_T[KEY_DIM]> key_byte_arr; //* buffer of KEY_T and unsigned char union
	static var_bT<VAL_T[VAL_DIM]> val_byte_arr; //* buffer of VAL_T and unsigned char union

	//* read keys in msg
	for (size_t i = 0; i < key_size; ++i) {
		key_byte_arr.b[i] = msg[i];
	}
	//* read values in msg
	for (size_t i = 0; i < val_size; ++i) {
		val_byte_arr.b[i] = msg[key_size + i];
	}
	//* copy keys from the union
	for (size_t i = 0; i < KEY_DIM; ++i) {
		key_arr[i] = key_byte_arr.v[i];
	}
	//* copy values from the union
	for (size_t i = 0; i < VAL_DIM; ++i) {
		val_arr[i] = val_byte_arr.v[i];
	}
}
} // namespace udp_msg

#endif
