/* f3d_gyro.c --- 
 * 
 * Filename: f3d_gyro.c
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: Michael Ledesma<mledesma> and Russell Wang <ruswang>
 * Created: Thu Jan 24 05:43:01 2013 (-0500)
 * Version: 
 * Last-Updated: 03/06/2014
 *           By: Michael Ledesma <mledesma>
 *     Update #: 0
 * URL: 
 * Doc URL: 
 * Keywords: 
 * Compatibility: 
 * 
 */

/* Commentary: 
 * 
 * 
 * 
 */

/* Change Log:
 * 
 * 
 */

/* This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth
 * Floor, Boston, MA 02110-1301, USA.
 */

/* Code: */
#include <stdio.h>
#include <f3d_gyro.h>
#include <stm32f30x_spi.h>
#include <stm32f30x_rcc.h>

void f3d_gyro_interface_init() {
  //
  //initialization for the peripheral clock
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

  //configuration for I/O Pins
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_StructInit(&GPIO_InitStructure);

  //PA5
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_PinAFConfig(GPIOA,5,GPIO_AF_5);
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
  
  //PA6
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;  
  GPIO_PinAFConfig(GPIOA,6,GPIO_AF_5);
  GPIO_Init(GPIOA, &GPIO_InitStructure); 

  //PA7
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_PinAFConfig(GPIOA,7,GPIO_AF_5);
  GPIO_Init(GPIOA, &GPIO_InitStructure); 


  //PE3
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_SetBits(GPIOE, GPIO_Pin_3);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  

  //SPI Initialization
  SPI_InitTypeDef SPI_InitStructure;
  SPI_I2S_DeInit(SPI1);
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_Init(SPI1, &SPI_InitStructure);
  SPI_RxFIFOThresholdConfig(SPI1, SPI_RxFIFOThreshold_QF);
  SPI_Cmd(SPI1, ENABLE);
} 

void f3d_gyro_init(void) {
  uint8_t ctrl1, ctrl4;
  // CTRL1 Register 
  // Bit 7:6 Data Rate: Datarate 0
  // Bit 5:4 Bandwidth: Bandwidth 3
  // Bit 3: Power Mode: Active
  // Bit 2:0 Axes Enable: X,Y,Z enabled
  ctrl1 |= (uint8_t) (((uint8_t)0x00) |
		      ((uint8_t)0x30) |
		      ((uint8_t)0x08) |	       		       
		      ((uint8_t)0x07));
  // CTRL4 Register 
  // Bit 7 Block Update: Continuous */
  // Bit 6 Endianess: LSB first  */
  // Bit 5:4 Full Scale: 500 dps */
  ctrl4 |= (uint8_t) (((uint8_t)0x00) |
		      ((uint8_t)0x00) |
		      ((uint8_t)0x10));
  f3d_gyro_interface_init();
  f3d_gyro_write(&ctrl1, 0x20, 1);
  f3d_gyro_write(&ctrl4, 0x23, 1);
}

//Reads one or more bytes from the gyro interface
void f3d_gyro_read(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead) {

  if (NumByteToRead > 0x01) {
    ReadAddr |= (uint8_t)(0x80 | 0x40); // If sending more that one byte set multibyte commands
  }
  else {
    ReadAddr |= (uint8_t) (0x80); // Else just set the read mode 
  }
  GYRO_CS_LOW();
  f3d_gyro_sendbyte(ReadAddr);
  while(NumByteToRead > 0x00) {
    *pBuffer = f3d_gyro_sendbyte(((uint8_t)0x00));
    NumByteToRead--;
    pBuffer++;
  }
  GYRO_CS_HIGH();
}

//writes one or more bytes from the gyro interface
void f3d_gyro_write(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite) {

  if (NumByteToWrite > 0x01) {
    WriteAddr |= (uint8_t)(0x80 | 0x40); //If sent multiple bytes, set bit 6 of WriteAddr.
  }

  GYRO_CS_LOW();
  f3d_gyro_sendbyte(WriteAddr);
  while(NumByteToWrite > 0x00) {
    f3d_gyro_sendbyte(*pBuffer);
    NumByteToWrite--;
    pBuffer++;
  }
  GYRO_CS_HIGH();
}


//polls the gyro transmitter register and sends a single byte
static uint8_t f3d_gyro_sendbyte(uint8_t byte) {
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET); 
  SPI_SendData8(SPI1, byte);
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
  return (uint8_t)SPI_ReceiveData8(SPI1);
}


//reads data from the gyro and populates pfData with values in dps.
void f3d_gyro_getdata(float *pfData) {

  int i;
  uint8_t tmpbuffer[6];
  int16_t RawData[3]; 

  f3d_gyro_read(tmpbuffer,0x28,6);
  //x is stored at location 0, y is stored at location 1, z is stored at location 2
  for(i=0; i<3; i++) {
    RawData[i]=(int16_t)(((uint16_t)tmpbuffer[2*i+1] << 8) + tmpbuffer[2*i]);
  }
 
  for(i=0; i<3; i++) {
    pfData[i] = (float)RawData[i]/GYRO_Sensitivity_500dps;
  }
}





