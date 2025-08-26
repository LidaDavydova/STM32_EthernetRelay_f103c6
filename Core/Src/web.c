/*
 * web.c
 *
 *  Created on: Aug 6, 2025
 *      Author: hedg
 */
#include "web.h"
#include "wizchip_conf.h"
#include "socket.h"
#include "relay.h"
#include <string.h>
#include <stdio.h>

// JSON response for success
static const char page_json_ok_body[] = "{\"status\":\"ok\"}";
// Not found page body
static const char page_404_body[]     = "404 Not Found";

static void send_http_response(int socket, const char* content_type, const char* body)
{
    char header[128];
    int body_len = strlen(body);
    int header_len = snprintf(header, sizeof(header),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: %s\r\n"
        "Connection: close\r\n"
        "Content-Length: %d\r\n\r\n",
        content_type, body_len);

    send(socket, (uint8_t*)header, header_len);
    send(socket, (uint8_t*)body, body_len);
}

static void send_http_404(int socket)
{
    char header[128];
    int body_len = strlen(page_404_body);
    int header_len = snprintf(header, sizeof(header),
        "HTTP/1.1 404 Not Found\r\n"
        "Content-Type: text/plain\r\n"
        "Connection: close\r\n"
        "Content-Length: %d\r\n\r\n", body_len);
    send(socket, (uint8_t*)header, header_len);
    send(socket, (uint8_t*)page_404_body, body_len);
}





void Web_HandleRequest(uint8_t *request, int len, int socket)
{
	(void)len;

	send_http_response(socket, "text/plain", (char *)request);

	// Handle ON command
	if (strncmp((char *)request, "GET /on", 7) == 0)
	{
		Relay_On();
	}
	// Handle OFF command
	else if (strncmp((char *)request, "GET /off", 8) == 0)
	{
		Relay_Off();
	}
	else if (strncmp((char *)request, "GET /status", 11) == 0)
	{
		// Optional JSON endpoint (keep if needed)
		char buf[64];
		snprintf(buf, sizeof(buf), "{\"relay\":%d}", Relay_GetState());
		send_http_response(socket, "application/json", buf);
		return;
	}
	else if (strncmp((char *)request, "GET / ", 6) != 0)
	{
		send_http_404(socket);
		return;
	}

	// ---- Always send main page with updated relay state ----
	char body[256];
	const char* state_msg = Relay_GetState() ? "Relay is ON" : "Relay is OFF";
	snprintf(body, sizeof(body),
		"<!DOCTYPE html><html><head><title>Smart Socket</title></head>"
		"<body><h1>Smart Socket</h1>"
		"<p>Status: <b>%s</b></p>"
		"<p><a href=\"/on\">Turn ON</a></p>"
		"<p><a href=\"/off\">Turn OFF</a></p>"
		"</body></html>",
		state_msg);

	send_http_response(socket, "text/html", body);
}

