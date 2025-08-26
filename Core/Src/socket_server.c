/*
 * socket_server.c
 *
 *  Created on: Aug 6, 2025
 *      Author: hedg
 */


#include "socket_server.h"
#include "wizchip_conf.h"
#include "socket.h"
#include "web.h"
#include <stdio.h>
#include <string.h>

#define HTTP_PORT       5000

#define MAX_SOCKETS 4
uint8_t rx_buffer[4][1024];

void SocketServer_Init(void)
{
    // здесь должен быть уже инициализирован W5500 (SPI, IP)
    printf("HTTP Server initialized on port %d\r\n", HTTP_PORT);
}

void SocketServer_Loop(void)
{
    for (int sock = 0; sock < MAX_SOCKETS; sock++)
    {
        switch (getSn_SR(sock))
        {
        case SOCK_CLOSED:
            socket(sock, Sn_MR_TCP, HTTP_PORT, 0);
            listen(sock);
            break;

        case SOCK_ESTABLISHED:
            if (getSn_RX_RSR(sock) > 0)
            {
                int32_t len = recv(sock, rx_buffer[sock], sizeof(rx_buffer[sock]) - 1);
                if (len > 0)
                {
                    rx_buffer[sock][len] = '\0';
                    Web_HandleRequest(rx_buffer[sock], len, sock);
                    close(sock);
                }
            }
            break;

        case SOCK_CLOSE_WAIT:
            close(sock);
            break;
        }
    }
}
