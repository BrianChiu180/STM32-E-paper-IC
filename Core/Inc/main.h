/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
void EPD_RST(void);
void EPD_init(void);
void EPD_LUT(void);
void SPI_Write(unsigned char value1);
void SPI_Read();
void EPD_WriteCMD(unsigned char command);
void EPD_WriteDATA(unsigned char data1);
void EPD_ReadDATA();
void EPD_PSR(void);
void EPD_PWR(void);
void Image_Display(void);	
void EPD_sleep(void);
void EPD_TSC(void);
void EPD_PBC(void);
void EPD_LPD(void);
void EPD_REV(void);
void EPD_FLG(void);
void EPD_CRC(void);
void EPD_VV(void);
void EPD_ROTP(void);
void OTP_PGM(char* otpData);
void Wait_Busy(void);
void EPD_TRES(void);

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
