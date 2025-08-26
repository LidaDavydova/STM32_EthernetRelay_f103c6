/*
 * wiznet_init.c
 *
 *  Created on: Aug 21, 2025
 *      Author: hedg
 */

#include "eth_w5500.h"
#include "logger.h"
#include "main.h"      // GPIO для CS/RST
#include "config.h"

extern SPI_HandleTypeDef hspi1;

static wiz_NetInfo gWIZNETINFO = {
    .mac  = {0x00, 0x08, 0xDC, 0x11, 0x22, 0x33},
    .ip   = {192, 168, 1, 171},
    .sn   = {255, 255, 255, 0},
    .gw   = {192, 168, 1, 1},
    .dns  = {8, 8, 8, 8},
    .dhcp = NETINFO_STATIC
};


void update_gWIZNETINFO_from_device_config(void) {
    for (int i = 0; i < 6; i++) {
        gWIZNETINFO.mac[i] = device_config.mac[i];
    }
    for (int i = 0; i < 4; i++) {
        gWIZNETINFO.ip[i] = device_config.ip[i];
        gWIZNETINFO.sn[i] = device_config.sn[i];
        gWIZNETINFO.gw[i] = device_config.gw[i];
        gWIZNETINFO.dns[i] = device_config.dns[i];
    }
    gWIZNETINFO.dhcp = device_config.dhcp; // Assigned at runtime
}


/* --- Callbacks --- */
static void W5500_Select(void)   { HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET); }
static void W5500_Unselect(void) { HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET); }

static uint8_t W5500_ReadByte(void) {
    uint8_t byte;
    HAL_SPI_Receive(&hspi1, &byte, 1, HAL_MAX_DELAY);
    return byte;
}

static void W5500_WriteByte(uint8_t byte) {
    HAL_SPI_Transmit(&hspi1, &byte, 1, HAL_MAX_DELAY);
}

/* --- Public API --- */
void ETH_W5500_Init(void) {
	update_gWIZNETINFO_from_device_config();;
    // Reset chip
    HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_SET);
    HAL_Delay(1000);

    // Register SPI callbacks
    reg_wizchip_cs_cbfunc(W5500_Select, W5500_Unselect);
    reg_wizchip_spi_cbfunc(W5500_ReadByte, W5500_WriteByte);

    // Init memory
    uint8_t rx_tx_buff_sizes[8] = {2,2,2,2,2,2,2,2};
    wizchip_init(rx_tx_buff_sizes, rx_tx_buff_sizes);

    // Apply default net info
    wizchip_setnetinfo(&gWIZNETINFO);
    ctlnetwork(CN_SET_NETINFO, (void*) &gWIZNETINFO);
    HAL_Delay(500);
}

void ETH_W5500_SetNetInfo(const wiz_NetInfo *info) {
    gWIZNETINFO = *info; // копируем
    wizchip_setnetinfo(&gWIZNETINFO);
}

void ETH_W5500_GetNetInfo(wiz_NetInfo *info) {
    wizchip_getnetinfo(info);
}

void ETH_W5500_Tick(void) {
    // сюда можно вставить проверку линка или dhcp
     ctlnetwork(CN_GET_NETINFO, &gWIZNETINFO);
}

void ETH_W5500_PrintNetInfo(void) {
    wiz_NetInfo netinfo;
    wizchip_getnetinfo(&netinfo);

    LOGI("W5500 NetInfo:");
    LOGI("  MAC  %02X:%02X:%02X:%02X:%02X:%02X",
         netinfo.mac[0], netinfo.mac[1], netinfo.mac[2],
         netinfo.mac[3], netinfo.mac[4], netinfo.mac[5]);
    Logger_Process();

    LOGI("  IP   %d.%d.%d.%d",
         netinfo.ip[0], netinfo.ip[1], netinfo.ip[2], netinfo.ip[3]);
    Logger_Process();

    LOGI("  GW   %d.%d.%d.%d",
         netinfo.gw[0], netinfo.gw[1], netinfo.gw[2], netinfo.gw[3]);
    Logger_Process();

    LOGI("  MASK %d.%d.%d.%d",
         netinfo.sn[0], netinfo.sn[1], netinfo.sn[2], netinfo.sn[3]);
    Logger_Process();

    LOGI("  DNS  %d.%d.%d.%d",
         netinfo.dns[0], netinfo.dns[1], netinfo.dns[2], netinfo.dns[3]);
    Logger_Process();

    LOGI("  DHCP %s", (netinfo.dhcp == NETINFO_DHCP) ? "ON" : "OFF");
}

