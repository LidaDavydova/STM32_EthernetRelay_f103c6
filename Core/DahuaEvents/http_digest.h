/*
 * http_digest.h
 *
 *  Created on: Aug 14, 2025
 *      Author: hedg
 */

#ifndef INC_HTTP_DIGEST_H_
#define INC_HTTP_DIGEST_H_

#include <stdint.h>
#include <stddef.h>
#define HTTP_BUF_SIZE 1500

/*
 * Minimal HTTP Digest subscribe helper for Dahua camera event stream.
 * Notes:
 * - Uses WIZnet ioLibrary socket/connect/send/recv functions.
 * - Expects an md5_hex(in, out) helper (32-char hex string + NUL).
 *
 */

typedef enum {
    HTTP_STATE_CONNECT=0,
    HTTP_STATE_SEND_REQ,
    HTTP_STATE_WAIT_RESP,
    HTTP_STATE_RECONNECT,
    HTTP_STATE_SEND_AUTH,
    HTTP_STATE_STREAM
} http_state_t;


typedef struct {
	char realm[128];
	char nonce[64];
	char opaque[64];
	char qop[16];
	char algorithm[16];
	char auth_header[512];
	char url[128];
	const char *user;
	const char *password;
	uint32_t nc; // счётчик nonce
	char cnonce[33]; // client nonce

	// FSM
	http_state_t state;

	// Buffers
	char rx_buf[512];
} http_digest_ctx_t;

typedef enum {
    HTTP_DIGEST_OK = 0,        // Operation went fine, still running
    HTTP_DIGEST_RETRY,         // Try again in next loop
    HTTP_DIGEST_ERROR,         // Fatal error (e.g., bad socket)
    HTTP_DIGEST_DONE           // Finished successfully (streaming)
} http_digest_status_t;


http_digest_status_t http_digest_process(void);

void http_digest_init(uint8_t sock, uint8_t *ip, uint16_t port,
                      const char *url, const char *user, const char *pass);

#endif /* INC_HTTP_DIGEST_H_ */
