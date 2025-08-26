/*
 * wiznet.h
 *
 *  Created on: Aug 21, 2025
 *      Author: hedg
 */

#pragma once
#include <stdint.h>
#include "wizchip_conf.h"


// Инициализация чипа и сети
void ETH_W5500_Init(void);

// Установить параметры сети
void ETH_W5500_SetNetInfo(const wiz_NetInfo *info);

// Получить текущие параметры сети
void ETH_W5500_GetNetInfo(wiz_NetInfo *info);

// Периодический вызов (например, каждые 100 мс)
void ETH_W5500_Tick(void);

void ETH_W5500_PrintNetInfo(void);
