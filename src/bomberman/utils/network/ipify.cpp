/* Simple C library for querying your IP address using http://ipify.org
 *
 * Copyright (c) 2015-2016  Joachim Nilsson <troglobit@gmail.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define IPIFY_HOST    "api.ipify.org"
#define AGENT_NAME    "retroarch/lr_multi_bomberman_libretro"
#define HTTP_REQUEST				\
	"GET / HTTP/1.0\r\n"			\
	"Host: " IPIFY_HOST "\r\n"		\
	"User-Agent: " AGENT_NAME "\r\n\r\n";


int ipify_connect(void)
{
	int ret, sd;
	struct addrinfo hints, *result;

	sd = socket(AF_INET, SOCK_STREAM, 0);
	if (sd < 0)
		return -1;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family   = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	ret = getaddrinfo(IPIFY_HOST, "http", &hints, &result);
	if (ret || !result) {
		close(sd);
		return -1;
	}

	ret = connect(sd, result->ai_addr, result->ai_addrlen);
	freeaddrinfo(result);

	if (ret < 0) {
		close(sd);
		return -1;
	}

	return sd;
}

int ipify_query(int sd, char *addr, size_t len)
{
	int domain, ret;
	char buf[512], *ptr, tmp[sizeof(struct in6_addr)];
	const char *req = HTTP_REQUEST;

	ret = send(sd, req, strlen(req), 0);
	if (ret < 0)
		return ret;

	ret = recv(sd, buf, sizeof(buf), 0);
	if (ret < 0)
		return ret;

	ptr = strstr(buf, "200 OK");
	if (!ptr)
		return 1;

	ptr = strstr(ptr, "\r\n\r\n");
	if (!ptr)
		return 1;
	ptr += 4;

	domain = AF_INET;
	if (!inet_pton(domain, ptr, tmp)) {
		domain = AF_INET6;
		if (!inet_pton(domain, ptr, tmp))
			return 1;
	}

	if (!inet_ntop(domain, tmp, addr, len))
		return 1;

	return 0;
}

int ipify_disconnect(int sd)
{
	shutdown(sd, SHUT_RDWR);
	return close(sd);
}

int ipify(char *addr, size_t len)
{
	int sd, ret;

	sd = ipify_connect();
	if (sd < 0)
		return 1;

	ret  = ipify_query(sd, addr, len);
	ret |= ipify_disconnect(sd);

	return ret;
}
