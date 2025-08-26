/*
 * config.h
 *
 *  Created on: Aug 26, 2025
 *      Author: hedg
 */

#ifndef HTTPCONFIG_CONFIG_H_
#define HTTPCONFIG_CONFIG_H_

#include <stdint.h>  // for uint8_t, uint32_t
#include "wizchip_conf.h"

typedef struct {
    uint8_t mac[6];           // MAC address
    uint8_t ip[4];            // IP address
    uint8_t sn[4];   		  // Subnet Mask
    uint8_t gw[4];   		  // Gateway IP Address
    uint8_t dns[4];           // DNS server IP
    dhcp_mode dhcp;           // DHCP mode (static or DHCP)

    uint8_t logger_server_ip[4]; // logger server ip
    uint16_t logger_server_port; // logger server port

    uint8_t camera_ip[4];
    uint16_t camera_port;
    char camera_login[32];
    char camera_password[32];
    char camera_url[150];

    uint8_t relay_enabled;    // relay on/off
    uint32_t relay_timer;     // relay timer in seconds or milliseconds
    uint8_t function_select;  // function select
} Config_t;


extern Config_t device_config;  // объявление глобальной переменной конфигурации


void Flash_Write_Config(Config_t* config);


#endif /* HTTPCONFIG_CONFIG_H_ */
