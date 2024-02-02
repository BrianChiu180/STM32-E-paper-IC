/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "stdio.h"
#include "string.h"
#include "gpio.h"
#include "usart.h"
#include "main.h"
#include "8253_LUT.h"
#include "Ap_29demo.h" //BW imaage of 420 x 416
#include "Ap_30demo.h" //Red imaage of 420 x 416

/* USER CODE BEGIN 0 */
GPIO_InitTypeDef GPIO_InitStruct = {0};
uint8_t rd_sda, data, temp=0;
uint16_t busy_status;
/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5|GPIO_PIN_8, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_12|GPIO_PIN_14, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA5 PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PC4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PB1 PB2 PB12 PB14 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_12|GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */
void EPD_RST(void)
{
HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);  //RST_N=Hi
HAL_Delay(10);   
HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);  //RST_N=LOW
HAL_Delay(10);   
HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);  //RST_N=HI
HAL_Delay(10);
}

void SPI_Write(unsigned char value1)
{
	int i;
	for(i=0; i<8; i++)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);   //SCL=LOW
		if(value1 & 0x80)
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET); //SDA=HI
		else
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);	//SDA=LOW
		value1 = (value1 << 1);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);   //SCL=HI
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);   //SCL=LOW
	}
}

void SPI_Read()
{
	  GPIO_InitStruct.Pin = GPIO_PIN_4;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

      int j;

		for(j=0; j<8; j++)
		{
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);   //SCL=LOW

		  temp = (temp << 1);
		  data = HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_4);              //Read SDA value
		  if (data == 1)                                          //When SDA=1, execute left shift 1 time
		  temp++;                                                 //When SDA=0, don't execute left 1 time

		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);         //SCL=HI
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);      //SCL=LOW
	      //printf("Data Bit Read value= %x\n\r", data);               //Output SDA value by binary

		}
        rd_sda = temp;                                            //Build up 8 times SDA value
        //printf("SDA Read value=0X %x\n\r",rd_sda);                //Output SDA value by Hex

}




void EPD_WriteCMD(unsigned char command)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);  //CSB=LOW
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);   //DC=LOW
	SPI_Write(command);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);  //CSB=HI
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);   //DC=HI
}
void EPD_WriteDATA(unsigned char data1)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);  //CSB=LOW
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);   //DC=HI
	SPI_Write(data1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);  //CSB=HI
}

void EPD_ReadDATA()
{

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);  //CSB=LOW
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);   //DC=HI
	SPI_Read();
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);  //CSB=HI
}

void EPD_init(void)
{
 EPD_WriteCMD(0x00);   //0x00h_PSR(Panel Setting)
 EPD_WriteDATA(0x0f);  //0f is default setting;Reg=1 for register (0x2f) ; Reg=0 for otp (0x0f)
 EPD_WriteDATA(0x8d);  //default setting=0x0d

 EPD_WriteCMD(0x01);   //0x01h_PWR(Power Setting)
 EPD_WriteDATA(0x03);  //default setting=0x03   
 EPD_WriteDATA(0x10);  //default setting=0x10
 EPD_WriteDATA(0x3f);  //default setting=0x3f
 EPD_WriteDATA(0x3f);  //default setting=0x3f
 EPD_WriteDATA(0x0d);  //default setting=0x0d

 EPD_WriteCMD(0x03);   //0x03h_PFS(Powee off Sequence Setting)
 EPD_WriteDATA(0x00);  //default setting=0x00
 	
 EPD_WriteCMD(0x06);   //0x06h_BTST (Booster Soft Start)
 EPD_WriteDATA(0x17);  //default setting=0x17   
 EPD_WriteDATA(0x17);  //default setting=0x17
 EPD_WriteDATA(0x1e);  //default setting=0x17	
 
 EPD_WriteCMD(0x30);   //0x30h_PLL control
 EPD_WriteDATA(0x09);  //0x09(50hz) is default setting   
 	
 EPD_WriteCMD(0x50);   //0x50h_CDI
 EPD_WriteDATA(0x57);  //0x09(50hz) is default setting 	
 	
 EPD_WriteCMD(0x61);   //240(s)x416(g)
 EPD_WriteDATA(0xf0);
 EPD_WriteDATA(0x01);
 EPD_WriteDATA(0xa0);	

 EPD_WriteCMD(0x82);   //Reg-0x82_VCOM_DC
 EPD_WriteDATA(0x1a);  //0x1a=-2.7
 
 EPD_WriteCMD(0xe3);   //0xE3h_PWS
 EPD_WriteDATA(0x00);	 //0x00 is default setting

}

void EPD_LUT(void)
{
	 unsigned int count;
	 EPD_WriteCMD(0x20);     //Reg-0x20_VCOM LUT (LUTC)
		for(count=0;count<56;count++) 	 
		{EPD_WriteDATA(VCOM_LUT[count]);}

		 EPD_WriteCMD(0x22);     //Reg-0x22_K2W LUT (LUTKW/LUTR)
		for(count=0;count<56;count++) 	 
		{EPD_WriteDATA(LUTR[count]);}	
	
  	EPD_WriteCMD(0x23);     //Reg-0x23_W2K LUT (LUTWK/LUTK)
		for(count=0;count<56;count++) 	 
		{EPD_WriteDATA(LUTW[count]);}	  
		
		EPD_WriteCMD(0x24);     //Reg-0x24_K2K LUT (LUTKK/LUTK)
		for(count=0;count<56;count++) 	 
		{EPD_WriteDATA(LUTK[count]);}	 	
}


void Image_Display(void)	
{
    unsigned int j;

	  EPD_WriteCMD(0x10);		     //Transfer new data (original setting-0x13) 
	  for(j=0;j<12480;j++)
	  {	  
//		  EPD_WriteDATA(0x00);     //All white : DTM1=0xFF + DTM2= 0x00 ; All red : DTM1=0x00 + DTM2= 0xFF
   		EPD_WriteDATA(BW_240x416[j]);  //Transfer the actual displayed data for DTM1 (BW)
			
		}  
	  
	    EPD_WriteCMD(0x13);		     //Transfer new data (original setting-0x13) 
	  for(j=0;j<12480;j++)
	  {	  
//			EPD_WriteDATA(0xff);			
  		EPD_WriteDATA(Red_240x416[j]);  //Transfer the actual displayed data for DTM1 (Red)  	
			
		}  

}





//0x40h TSC(Temperature Sensor Calibration)
void EPD_TSC(void)
{
	EPD_WriteCMD(0x40);
	EPD_ReadDATA();
	printf("CMD-0x40 value=0X%x\n\r", rd_sda);
	HAL_Delay(1);
	EPD_ReadDATA();
	printf("CMD-0x40 value=0X%x\n\r", rd_sda);
}

//0x44h PBC(Panel Break Check)
void EPD_PBC(void)
{
	EPD_WriteCMD(0x44);
	EPD_ReadDATA();
	printf("CMD-0x44 value=0X%x\n\r", rd_sda);
}

//0x51h LPD(Low)
void EPD_LPD(void)
{
	EPD_WriteCMD(0x51);
	EPD_ReadDATA();
	printf("CMD-0x51 value=0X%x\n\r", rd_sda);
}

//0x70h REV(Revision)
void EPD_REV(void)
{
	EPD_WriteCMD(0x70);
	for (int k=0; k<5 ; k++)
	{
	  EPD_ReadDATA();
	  printf("CMD-0x70 value=0X%x\n\r", rd_sda);
	  HAL_Delay(1);
	}
}

//0x71h FLG(Get Status)
void EPD_FLG(void)
{
	EPD_WriteCMD(0x71);
	EPD_ReadDATA();
	printf("CMD-0x71 value=0X%x\n\r", rd_sda);
}

//0x72h CRC(Cyclic Redundancy Check)
void EPD_CRC(void)
{
	EPD_WriteCMD(0x72);
	EPD_ReadDATA();
	printf("CMD-0x72 value=0X%x\n\r", rd_sda);
    HAL_Delay(1);
	EPD_ReadDATA();
	printf("CMD-0x72 value=0X%x\n\r", rd_sda);
}

//0x81h CVV(Read VCOM Value)
void EPD_VV(void)
{
	EPD_WriteCMD(0x81);
	EPD_ReadDATA();
	printf("CMD-0x81 value=0X%x\n\r", rd_sda);
}

//0xA2h ROTP(Read OTP)
void EPD_ROTP(void)
{
	EPD_WriteCMD(0xa2);
	for (int l=0 ; l<4096 ; l++)
	{
	EPD_ReadDATA();
	printf("CMD-0xA2 value=0X%x\n\r", rd_sda);
    HAL_Delay(1);
	}
}

void EPD_OTP(void)
{
 	unsigned int k;
	EPD_RST();                  //Reset
	
	EPD_WriteCMD(0xa3);		     //Reg-0xa3_PGAR(OTP Programming Address)
	EPD_WriteDATA(0x00);       // Set ST_ADDR[12:8]=0x00
	EPD_WriteDATA(0x00);       // Set ST_ADDR[7:0]=0x00
	EPD_WriteDATA(0x1f);       // Set END_ADDR[12:8]=0x1f
	EPD_WriteDATA(0xff);       // Set END_ADDR[7:0]=0xff
	
	EPD_WriteCMD(0xa0);		     //Reg-0xa0_Program Mode_PGM

	  EPD_WriteCMD(0x10);		     //Transfer new data (original setting-0x13) 

	  for(k=0;k<5624;k++)
	  {
	  EPD_WriteDATA(0x00);  //Transfer the actual displayed data
	  
	  }
	
	
	EPD_WriteCMD(0x01);		     //Reg-0x01_PWR(Power Setting)
	EPD_WriteDATA(0x83);       //0x8_Enable internal Vpp+0x03-default setting
	EPD_WriteDATA(0x00);       //Bit 4-VCOM_slew =0
	EPD_WriteDATA(0x3A);       //VSH[5:0] = 0x3A(+14V) , default setting=0x3f(+15V)
	EPD_WriteDATA(0x3A);       //VSL[5:0] = 0x3A(-14V) , default setting=0x3f(-15V)
	EPD_WriteDATA(0x03);       //

	
 EPD_WriteCMD(0x06);   //0x06h_BTST (Booster Soft Start)
 EPD_WriteDATA(0x17);  //default setting=0x17   
 EPD_WriteDATA(0x17);  //default setting=0x17
 EPD_WriteDATA(0x17);  //default setting=0x17	
 
 EPD_WriteCMD(0x04);   //CMD-0x04 PON
 Wait_Busy();
 HAL_Delay(100);	
	
 EPD_WriteCMD(0xa1);   //Reg-0xa1_Active Program (APG)	
 Wait_Busy();
 HAL_Delay(100);	

 EPD_WriteCMD(0x02);   //Reg-0x02_Power off (POF)	
 Wait_Busy();
 HAL_Delay(100);	
	
EPD_ROTP();	
	
}


/*
void Wait_Busy(void)
{
	busy_status = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_2);
	do
	{
		if (busy_status == GPIO_PIN_SET)
			break;
		else
			printf("Busy Status= %x\r\n",busy_status);
			HAL_Delay(100);

	}while(busy_status== GPIO_PIN_SET);
}
*/

/* USER CODE END 2 */
