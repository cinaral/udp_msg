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

/*
 * This file is intended to address sock.hpp's WIN32 compatibility problems.
 */

#ifndef SOCK_COMPAT_HPP_CINARAL_221123_1238
#define SOCK_COMPAT_HPP_CINARAL_221123_1238

#if defined(__WIN32__) || defined(__WIN32) || defined(_WIN32) || defined(WIN32) || \
    defined(__CYGWIN__) || defined(__MINGW32__) || defined(__BORLANDC__)
	#define WIN32_COMPAT
#endif

#ifdef WIN32_COMPAT
	#include <winsock2.h>
#else
	#include <sys/socket.h> //* socket, sendto
	#include <arpa/inet.h>  //* htons, inet_addr
	#include <netinet/in.h> //* sockaddr_in
	#include <fcntl.h>      //* fcntl
#endif
#include <unistd.h> //* close
#include <cstdio>   //* printf

/*
 * `SOCKET` compatibility
 */
#ifndef WIN32_COMPAT
typedef int SOCKET;
#endif

/*
 * `socklen_t` compatibility
 */
#ifdef WIN32_COMPAT
typedef int socklen_t;
#endif

namespace udp_msg
{

/*
 * `::close()` compatibility
 */
inline int
close(SOCKET sock)
{
#ifdef WIN32_COMPAT
	WSACleanup();
	return ::closesocket(sock);
#else
	return ::close(sock);
#endif
};

/*
 *  `::socket()` compatibility
 */
inline SOCKET
socket(int af, int type, int protocol)
{
#ifdef WIN32_COMPAT
	WSADATA wsa_data;

	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != NO_ERROR) {
		printf("Error at WSAStartup()\n");
		return INVALID_SOCKET;
	}
#endif
	//* start socket
	return ::socket(af, type, protocol);
};

/*
 *  `fcntl()` compatibility
 */
inline void
set_nonblocking(SOCKET sock)
{
#ifdef WIN32_COMPAT
	u_long mode = 1; //* 1 to enable non-blocking socket
	ioctlsocket(sock, FIONBIO, &mode);
#else
	fcntl(sock, F_SETFL, O_NONBLOCK);
#endif
};
} // namespace udp_msg

#endif
