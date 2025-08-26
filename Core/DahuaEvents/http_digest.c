/*
 * http_digest.c
 *
 *  Created on: Aug 15, 2025
 *      Author: hedg
 */


#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <http_digest.h>
#include <stdint.h>

#include "socket.h"
#include "wizchip_conf.h"
#include "logger.h"
#include "relay.h"

#include <digest.h>
#include <client.h>


static uint8_t s_sock;
static uint8_t s_ip[4];
static uint16_t s_port;
static const char *s_url;
static http_state_t s_state;
static http_digest_ctx_t ctx;

digest_t d;

#define SCAN_BUF 256
static char scan_buf[SCAN_BUF];
static int scan_len = 0;


static int in_whitelist = 0;

static void process_stream_chunk(const char *data, int len) {
    for (int i = 0; i < len; i++) {
        // shift left if buffer too full
        if (scan_len >= SCAN_BUF - 1) {
            memmove(scan_buf, scan_buf + 128, SCAN_BUF - 128);
            scan_len -= 128;
        }

        scan_buf[scan_len++] = data[i];
        scan_buf[scan_len] = '\0';

        // detect "WhiteList"
        if (!in_whitelist) {
            if (strstr(scan_buf, "\"WhiteList\"")) {
                in_whitelist = 1;
                // optional: clear buffer to reduce size
                scan_len = 0;
            }
        }

        // detect "PlateNumber" only if inside WhiteList
        if (in_whitelist) {
            char *p = strstr(scan_buf, "\"PlateNumber\"");
            if (p) {
                char *colon = strchr(p, ':');
                if (colon) {
                    colon++;
                    while (*colon == ' ' || *colon == '\"') colon++;

                    char plate[32] = {0};
                    int j = 0;
                    while (*colon && *colon != '\"' && j < (int)sizeof(plate)-1) {
                        plate[j++] = *colon++;
                    }
                    plate[j] = '\0';

                    LOGI("WhiteList Plate detected: %s\n", plate);
                    Relay_Off();

                    // reset state after success
                    in_whitelist = 0;
                    scan_len = 0;
                }
            }
        }
    }
}




static char *find_header_value(char *buf, const char *key) {
	char *p = buf;
	size_t keylen = strlen(key);
	while (p && *p) {
		char *line = p;
		char *nl = strstr(line, "\r\n");
		if (!nl) break;
		size_t linelen = nl - line;
		if (linelen >= keylen) {
			int match = 1;
			for (size_t i = 0; i < keylen; ++i) {
				if (tolower((unsigned char)line[i]) != tolower((unsigned char)key[i])) {
					match = 0; break;
				}
			}
			if (match && line[keylen] == ':') {
				char *val = line + keylen + 1;
				while (*val == ' ' && val < nl) ++val;
				*nl = '\0';
				return val;
			}
		}
		p = nl + 2;
	}
	return NULL;
}

/* Extract token like realm="..." or nonce="..." from header value string */
static int extract_quoted(const char *src, const char *name, char *out, size_t out_len) {
	const char *p = src;
	size_t nlen = strlen(name);
	while ((p = strstr(p, name)) != NULL) {
		const char *q = p + nlen;
		while (*q == ' ') ++q;
		if (*q == '=') {
			q++;
			while (*q == ' ') ++q;
			if (*q == '\"') {
				q++;
				const char *end = strchr(q, '\"');
				if (!end) return -1;
				size_t copy = end - q;
				if (copy >= out_len) copy = out_len - 1;
				memcpy(out, q, copy);
				out[copy] = '\0';
				return 0;
			} else {
				/* unquoted value */
				const char *end = q;
				while (*end && *end != ',' && *end != ' ') ++end;
				size_t copy = end - q;
				if (copy >= out_len) copy = out_len - 1;
				memcpy(out, q, copy);
				out[copy] = '\0';
				return 0;
			}
		}
		p += nlen;
	}
	return -1;
}

/* Minimal helper to send a C string fully */
static int send_all(uint8_t sock, const char *buf, uint16_t len)
{
    int sent = 0;
    while (sent < (int)len) {
        int ret = send(sock, (uint8_t *)buf + sent, len - sent);
        if (ret <= 0) return -1;
        sent += ret;
    }
    return sent;
}

/* Read available data into buffer (simple, blocking-ish with small timeout). Returns bytes read or negative. */
static int recv_once(uint8_t sock, uint8_t *buf, uint16_t bufsize)
{
    LOGI("RECV_ONCE: trying to recv %d bytes\n", bufsize);

    int r = recv(sock, buf, bufsize);
    return r;
}



void http_digest_init(uint8_t sock, uint8_t *ip, uint16_t port,
                      const char *url, const char *user, const char *pass)
{
    s_sock = sock;
    memcpy(s_ip, ip, 4);
    s_port = port;
    s_url = url;
    s_state = HTTP_STATE_CONNECT;
    ctx.user = user;
    ctx.password = pass;
    digest_init(&d);
}

http_digest_status_t http_digest_process(void)
{
    static char buf[HTTP_BUF_SIZE];
    int len;

    switch (s_state)
    {
    case HTTP_STATE_CONNECT:
        if (socket(s_sock, Sn_MR_TCP, 0, 0) != s_sock) {
            return HTTP_DIGEST_RETRY;
        }
        if (connect(s_sock, s_ip, s_port) == SOCK_OK) {
        	LOGD("HTTP_STATE_CONNECT: sock connected\n");
            s_state = HTTP_STATE_SEND_REQ;
        }
        return HTTP_DIGEST_OK;

    case HTTP_STATE_SEND_REQ:
        snprintf(buf, sizeof(buf),
            "GET %s HTTP/1.1\r\n"
            "Host: %d.%d.%d.%d\r\n"
			"User-Agent: python-requests/2.25.1\r\n"
			"Accept-Encoding: gzip, deflate\r\n"
			"Accept: */*\r\n"
            "Connection: keep-alive\r\n"
            "\r\n",
            s_url, s_ip[0], s_ip[1], s_ip[2], s_ip[3]);
        if (send_all(s_sock, buf, strlen(buf)) < 0)
            return HTTP_DIGEST_ERROR;
        s_state = HTTP_STATE_WAIT_RESP;
        return HTTP_DIGEST_OK;

    case HTTP_STATE_WAIT_RESP:
        len = recv_once(s_sock, (uint8_t *)buf, sizeof(buf) - 1);
        Logger_Process();
        if (len > 0) {
            buf[len] = 0;
            if (strstr(buf, "HTTP/1.1 200")) {
            	LOGI("SUCCESS: Already connected! Starting stream...\n");
				s_state = HTTP_STATE_STREAM;
				return HTTP_DIGEST_OK;
			}
            if (strstr(buf, "401 Unauthorized")) {
			   char *val = find_header_value(buf, "WWW-Authenticate");
			   if (val) {
				   // Извлекаем все параметры из заголовка
				   extract_quoted(val, "realm", ctx.realm, sizeof(ctx.realm));
				   extract_quoted(val, "nonce", ctx.nonce, sizeof(ctx.nonce));
				   extract_quoted(val, "opaque", ctx.opaque, sizeof(ctx.opaque));
				   extract_quoted(val, "qop", ctx.qop, sizeof(ctx.qop));
				   extract_quoted(val, "algorithm", ctx.algorithm, sizeof(ctx.algorithm));

				   // Инициализируем счётчик nonce
				   ctx.nc = 0;

				   log_printf("Extracted auth params: realm=%s, nonce=%s, opaque=%s, qop=%s, algorithm=%s\n",
					   ctx.realm, ctx.nonce, ctx.opaque, ctx.qop, ctx.algorithm);


				   digest_client_parse(&d, "Digest realm=\"%s\", qop=\"%s\", nonce=\"%s\", opaque=\"%s\", algorithm=\"%s\"",
						   ctx.realm, ctx.qop, ctx.nonce, ctx.opaque, ctx.algorithm);

				   digest_set_attr(&d, D_ATTR_USERNAME, (digest_attr_value_t) "admin");
				   digest_set_attr(&d, D_ATTR_PASSWORD, (digest_attr_value_t) "sdfSDF543");
				   digest_set_attr(&d, D_ATTR_URI, (digest_attr_value_t) s_url);
				   digest_set_attr(&d, D_ATTR_METHOD, (digest_attr_value_t) DIGEST_METHOD_GET);

				   s_state = HTTP_STATE_SEND_AUTH;
			   }
		   }
           else if (strstr(buf, "HTTP/1.1") && strstr(buf, "200")) {
        	   LOGI("Connected successfully, switching to stream mode\n");
				s_state = HTTP_STATE_STREAM;
           }
        }
        return HTTP_DIGEST_OK;

    case HTTP_STATE_RECONNECT:
    	close(s_sock);
    	for(volatile int i = 0; i < 500000; i++);
        if (socket(s_sock, Sn_MR_TCP, 0, 0) != s_sock)
            return HTTP_DIGEST_RETRY;
        for(volatile int i = 0; i < 200000; i++);

        uint8_t connect_result = connect(s_sock, s_ip, s_port);

        if (connect_result == SOCK_OK) {
            s_state = HTTP_STATE_STREAM;
        	return HTTP_DIGEST_OK;
		} else {
			return HTTP_DIGEST_RETRY;
		}

    case HTTP_STATE_SEND_AUTH:
    	Logger_Process();
    	LOGD("HTTP_STATE_SEND_AUTH\n");
    	char result[1024];
    	digest_client_generate_header(&d, result, sizeof (result));

    	LOGD("result: %s\n", result);


        snprintf(buf, sizeof(buf),
			"GET %s HTTP/1.1\r\n"
			"Host: %d.%d.%d.%d\r\n"
			"User-Agent: python-requests/2.25.1\r\n"  // Custom but similar to Python
			"Accept-Encoding: gzip, deflate\r\n"     // Match Python
			"Accept: */*\r\n"                        // Match Python
			"Connection: keep-alive\r\n"             // Match Python
			"Authorization: %s"  // Сюда подставится сгенерированный заголовок Authorization
			"\r\n\r\n",
			s_url, s_ip[0], s_ip[1], s_ip[2], s_ip[3],
			result);

        close(s_sock);

		if (socket(s_sock, Sn_MR_TCP, 0, 0) != s_sock) {
		   LOGE("Failed to create socket\n");
		   return HTTP_DIGEST_ERROR;
		}

		if (connect(s_sock, s_ip, s_port) != SOCK_OK) {
		   LOGE("Failed to connect socket\n");
		   close(s_sock);
		   return HTTP_DIGEST_RETRY;
		}


		LOGD("SENDING REQUEST: %s\n", buf);

    	if (send_all(s_sock, buf, strlen(buf)) < 0) {
    		LOGE("Send failed after reconnect\n");
			close(s_sock);
			return HTTP_DIGEST_ERROR;
		}

		s_state = HTTP_STATE_STREAM;
		break;

    case HTTP_STATE_STREAM:
    	LOGI("HTTP_STATE_STREAM: start streaming...\n");

    	char recv_buf[3000];
    	int recv_len;

    	memset(recv_buf, 0, sizeof(recv_buf));
    	recv_len = recv_once(s_sock, recv_buf, sizeof(recv_buf) - 1);

    	if (recv_len > 0) {
    		process_stream_chunk(recv_buf, recv_len);
    	}
    	else if (recv_len == 0) {
    		LOGI("STREAM: peer closed connection\n");
    	    close(s_sock);
    	    s_state = HTTP_STATE_RECONNECT;
    	}
    	else {
    	    uint8_t st = getSn_SR(s_sock);
    	    if (st == SOCK_CLOSE_WAIT || st == SOCK_CLOSED) {
    	    	LOGI("STREAM: socket closed by peer\n");
    	        close(s_sock);
    	        s_state = HTTP_STATE_RECONNECT;
    	    }
    	}



        return HTTP_DIGEST_OK;
    }

    return HTTP_DIGEST_ERROR;
}






