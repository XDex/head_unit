/*****************************
 * nrf24.c
 *  Created on: 18 мая 2026 г.
 *  Author: Dmitrii Safonov
 *****************************/
#include "main.h"
#include "nrf24.h"
#include "stdio.h"
#include "string.h"
//------------------------------------------------
extern SPI_HandleTypeDef hspi3;
extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim3;
//------------------------------------------------
#define TX_ADR_WIDTH 3
#define TX_PLOAD_WIDTH 5
uint8_t TX_ADDRESS[TX_ADR_WIDTH] = {0xb3,0xb4,0x01};
uint8_t RX_BUF[TX_PLOAD_WIDTH] = {0};
extern char str1[150];
uint8_t ErrCnt_Fl = 0;
//------------------------------------------------
void Delay_us(uint16_t us)
{
    __HAL_TIM_SET_COUNTER(&htim3,0);
    while(__HAL_TIM_GET_COUNTER(&htim3) < us)
    {
    }
}
//--------------------------------------------------
uint8_t NRF24_ReadReg(uint8_t addr)
{
  uint8_t dt=0, cmd;
  CS_ON;
  HAL_SPI_TransmitReceive(&hspi3,&addr,&dt,1,1000);
  if (addr!=STATUS)																						//если адрес равен адрес регистра статус то и возварщаем его состояние
  {
    cmd=0xFF;
    HAL_SPI_TransmitReceive(&hspi3,&cmd,&dt,1,1000);
  }
  CS_OFF;
  return dt;
}
//------------------------------------------------
void NRF24_WriteReg(uint8_t addr, uint8_t dt)
{
  addr |= W_REGISTER;												//включим бит записи в адрес
  CS_ON;
  HAL_SPI_Transmit(&hspi3,&addr,1,1000);		//отправим адрес в шину
  HAL_SPI_Transmit(&hspi3,&dt,1,1000);			//отправим данные в шину
  CS_OFF;
}
//------------------------------------------------
void NRF24_ToggleFeatures(void)
{
  uint8_t dt[1] = {ACTIVATE};
  CS_ON;
  HAL_SPI_Transmit(&hspi3,dt,1,1000);
  Delay_us(1);
  dt[0] = 0x73;
  HAL_SPI_Transmit(&hspi3,dt,1,1000);
  CS_OFF;
}
//-----------------------------------------------
void NRF24_Read_Buf(uint8_t addr,uint8_t *pBuf,uint8_t bytes)
{
  CS_ON;
  HAL_SPI_Transmit(&hspi3,&addr,1,1000);		//отправим адрес в шину
  HAL_SPI_Receive(&hspi3,pBuf,bytes,1000);	//отправим данные в буфер
  CS_OFF;
}
//------------------------------------------------
void NRF24_Write_Buf(uint8_t addr,uint8_t *pBuf,uint8_t bytes)
{
  addr |= W_REGISTER;												//включим бит записи в адрес
  CS_ON;
  HAL_SPI_Transmit(&hspi3,&addr,1,1000);		//отправим адрес в шину
  Delay_us(1);
  HAL_SPI_Transmit(&hspi3,pBuf,bytes,1000);	//отправим данные в буфер
  CS_OFF;
}
//------------------------------------------------
void NRF24_FlushRX(void)
{
  uint8_t dt[1] = {FLUSH_RX};
  CS_ON;
  HAL_SPI_Transmit(&hspi3,dt,1,1000);
  Delay_us(1);
  CS_OFF;
}
//------------------------------------------------
void NRF24_FlushTX(void)
{
  uint8_t dt[1] = {FLUSH_TX};
  CS_ON;
  HAL_SPI_Transmit(&hspi3,dt,1,1000);
  Delay_us(1);
  CS_OFF;
}
//------------------------------------------------
void NRF24L01_RX_Mode(void)
{
  uint8_t regval=0x00;
  regval = NRF24_ReadReg(CONFIG);
  //>>>>>>>>>разбудим модуль и переведём его в режим приёмника, включив биты PWR_UP и PRIM_RX
  regval |= (1<<PWR_UP)|(1<<PRIM_RX);
  NRF24_WriteReg(CONFIG,regval);
  CE_SET;
  Delay_us(150); 			//Задержка минимум 130 мкс
  NRF24_FlushRX();
  NRF24_FlushTX();
}
//------------------------------------------------
void NRF24L01_TX_Mode(uint8_t *pBuf)
{
  NRF24_Write_Buf(TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);
  CE_RESET;
  NRF24_FlushRX();
  NRF24_FlushTX();
}
//------------------------------------------------
void NRF24_Transmit(uint8_t addr,uint8_t *pBuf,uint8_t bytes)
{
  CE_RESET;
  CS_ON;
  HAL_SPI_Transmit(&hspi3,&addr,1,1000);				//отправим адрес в шину
  Delay_us(1);
  HAL_SPI_Transmit(&hspi3,pBuf,bytes,1000);			//отправим данные в буфер
  CS_OFF;
  CE_SET;
}
//------------------------------------------------
uint8_t NRF24L01_Send(uint8_t *pBuf)
{
  uint8_t status=0x00, regval=0x00;
	NRF24L01_TX_Mode(pBuf);
	regval = NRF24_ReadReg(CONFIG);
	//>>>>>>>>>>если модуль ушел в спящий режим, то разбудим его, включив бит PWR_UP и выключив PRIM_RX
	regval |= (1<<PWR_UP);
	regval &= ~(1<<PRIM_RX);
	NRF24_WriteReg(CONFIG,regval);
	Delay_us(150); 																			//Задержка минимум 130 мкс
	//>>>>>>>>>>Отправим данные в воздух
	NRF24_Transmit(WR_TX_PLOAD, pBuf, TX_PLOAD_WIDTH);
	CE_SET;
	Delay_us(15); 																			//minimum 10us high pulse (Page 21)
	CE_RESET;
	while((GPIO_PinState)IRQ == GPIO_PIN_SET) {}
	status = NRF24_ReadReg(STATUS);
	if(status&TX_DS) 																		//tx_ds == 0x20
	{
		LED_TGL;
		NRF24_WriteReg(STATUS, 0x20);
	}
	else if(status&MAX_RT)
	{
		NRF24_WriteReg(STATUS, 0x10);
		NRF24_FlushTX();
	}
	regval = NRF24_ReadReg(OBSERVE_TX);
	//>>>>>>>>>>Уходим в режим приёмника
  NRF24L01_RX_Mode();
	return regval;
}
//------------------------------------------------
uint8_t NRF24L01_Receive(void)
{
    uint8_t status = 0x01;
    while((GPIO_PinState)IRQ == GPIO_PIN_SET) {}
    status = NRF24_ReadReg(STATUS);
    if(status & 0x40)
    {
        NRF24_Read_Buf(RD_RX_PLOAD,RX_BUF,TX_PLOAD_WIDTH);
        NRF24_WriteReg(STATUS,0x40);
        return RX_BUF[0];
    }
    return 0x00;
}

/*void NRF24L01_Receive(void)
{
  uint8_t status=0x01;
  uint16_t dt=0;
	while((GPIO_PinState)IRQ == GPIO_PIN_SET) {}
	status = NRF24_ReadReg(STATUS);
	sprintf(str1,"STATUS: 0x%02X\r\n",status);
	HAL_UART_Transmit(&huart1,(uint8_t*)str1,strlen(str1),0x1000);
	LED_TGL;
	Delay_us(10);
  status = NRF24_ReadReg(STATUS);
  if(status & 0x40)
  {
    NRF24_Read_Buf(RD_RX_PLOAD,RX_BUF,TX_PLOAD_WIDTH);
    //sprintf(str1,"DATA = 0x%02X\r\n",RX_BUF[0]);
    sprintf(str1,"DATA = %s\r\n",RX_BUF);
    HAL_UART_Transmit(&huart1, (uint8_t*)str1,strlen(str1),0x1000);
    NRF24_WriteReg(STATUS, 0x40);
  }
}*/

//------------------------------------------------
void NRF24_ini(void)
{
	CE_RESET;
	Delay_us(5000);
	NRF24_WriteReg(CONFIG, 0x0a); 														// Set PWR_UP bit, enable CRC(1 byte) &Prim_RX:0 (Transmitter)
	Delay_us(5000);
	NRF24_WriteReg(EN_AA, 0x02); 															// Enable Pipe1
	NRF24_WriteReg(EN_RXADDR, 0x02); 													// Enable Pipe1
	NRF24_WriteReg(SETUP_AW, 0x01); 													// Setup address width=3 bytes
	NRF24_WriteReg(SETUP_RETR, 0x5F);													// 1500us, 15 retrans
	NRF24_ToggleFeatures();
	NRF24_WriteReg(FEATURE, 0);
	NRF24_WriteReg(DYNPD, 0);
	NRF24_WriteReg(STATUS, 0x70); 														//Reset flags for IRQ
	NRF24_WriteReg(RF_CH, 76); 																// частота 2476 MHz
	NRF24_WriteReg(RF_SETUP, 0x06); 													//TX_PWR:0dBm, Datarate:1Mbps
	NRF24_Write_Buf(TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);
	NRF24_Write_Buf(RX_ADDR_P1, TX_ADDRESS, TX_ADR_WIDTH);
	NRF24_WriteReg(RX_PW_P1, TX_PLOAD_WIDTH); 								//Number of bytes in RX payload in data pipe 1
 //>>>>>>>>>>пока уходим в режим приёмника
  NRF24L01_RX_Mode();
  LED_OFF;
}
//--------------------------------------------------
void NRF24_PrintReg(char *name, uint8_t reg)
{
    uint8_t value;
    value = NRF24_ReadReg(reg);
    sprintf(str1,"%s: 0x%02X\r\n",name,value);
    HAL_UART_Transmit(&huart1,(uint8_t*)str1,strlen(str1),0x1000);
}

