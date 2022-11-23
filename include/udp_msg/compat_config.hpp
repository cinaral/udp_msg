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

#ifndef COMPAT_CONFIG_HPP_CINARAL_221123_1238
#define COMPAT_CONFIG_HPP_CINARAL_221123_1238

#if defined(__WIN32__) || defined(__WIN32) || defined(_WIN32) || defined(WIN32) || \
    defined(__CYGWIN__) || defined(__MINGW32__) || defined(__BORLANDC__)
	#define WIN_COMPAT
#endif

#ifdef WIN_COMPAT
	#include <winsock2.h>
#else
	#include <arpa/inet.h>  //* htons, inet_addr
	#include <netinet/in.h> //* sockaddr_in
	#include <sys/socket.h> //* socket, sendto
#endif
#include <unistd.h> //* close
#include <cstdio>   //* printf

namespace udp_msg
{
//* ::close() compatibility
template <typename SOCK_T>
void
close_socket(SOCK_T *sock)
{
#ifdef WIN_COMPAT
	::closesocket(*sock);
#else
	::close(*sock);
#endif
};

//* ::socket() compatibility
template <typename SOCK_T, typename SOCKLEN_T>
bool
start_socket(const char hostname[], const unsigned port, SOCK_T *sock, sockaddr_in *dest,
             SOCKLEN_T *dest_size)
{
#ifdef WIN_COMPAT
	WSADATA wsa_data;

	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != NO_ERROR) {
		wprintf(L"Error at WSAStartup()\n");
		return false;
	}
#endif
	*sock = ::socket(AF_INET, SOCK_DGRAM, 0); //* start socket
	dest->sin_family = AF_INET;
	dest->sin_addr.s_addr = inet_addr(hostname);
	dest->sin_port = htons(port);
	*dest_size = sizeof(*dest);

#ifdef WIN_COMPAT
	if (*sock == INVALID_SOCKET) {
		wprintf(L"Error at socket(): %ld\n", WSAGetLastError());
		WSACleanup();
		close_socket<SOCK_T>(sock);
		return false;
	}
#else
	if (*sock < 0) {
		perror("socket");
		close_socket<SOCK_T>(sock);
		return false;
	}
#endif
	return true;
};

//* ::bind() compatibility
template <typename SOCK_T, typename SOCKLEN_T>
bool
bind_socket(SOCK_T *sock, sockaddr_in *dest)
{
#ifdef WIN_COMPAT
	if (::bind(*sock, (struct sockaddr *)dest, static_cast<int>(sizeof(*dest))) ==
	    SOCKET_ERROR) {
		wprintf(L"bind failed with error %u\n", WSAGetLastError());
		close_socket<SOCK_T>(sock);
		WSACleanup();
		return false;
	}
#else
	if (::bind(*sock, (struct sockaddr *)dest, sizeof(*dest)) < 0) {
		perror("bind");
		close_socket<SOCK_T>(sock);
		return false;
	}
#endif
	return true;
}
} // namespace udp_msg

#endif
