/*
 * button.c
 *
 *  Created on: 3 мая 2026 г.
 *      Author: dima
 */

#include "stm32l1xx_hal.h"
#include "main.h"
#include "button.h"
#include "nrf24.h"
#include "ILI9341_GFX.h"
#include "xpt2046_touch.h"
#include <stdio.h>

extern uint8_t rx_data;
extern uint8_t on_off;
extern uint16_t key_nr;
extern uint8_t timer_running;
extern uint8_t flag_press;
extern uint32_t time_press;
extern uint16_t x;
extern uint16_t y;
extern uint16_t screen;
extern char buf[64];
extern char buf[64];
extern uint16_t x;
extern uint16_t y;

extern uint16_t score_nr1;							//количество очков команды 1
extern uint16_t score_nr2;							//количество очков команды 2
extern uint16_t score_nr3;							//количество очков команды 3
extern uint16_t score_nr4;							//количество очков команды 4
extern uint16_t score_nr5;							//количество очков команды 5
extern uint16_t score_nr6;							//количество очков команды 6
extern uint16_t score_nr7;							//количество очков команды 7
extern uint16_t score_nr8;							//количество очков команды 8

extern volatile int g_timer_seconds;				//Начальное значение таймера
extern volatile uint8_t update_flag;		//старт/стоп таймера
extern uint16_t amount;									//номер команды
extern uint16_t tf;											//положительный или отрицательный ответ

extern TIM_HandleTypeDef htim2;
extern UART_HandleTypeDef huart1;

//=======Обработчик левой кнопки KEY1==================
uint8_t Button_Read_left(void)
{
	static uint32_t last_time = 0;
	static uint8_t last_state = 0;
	uint8_t current_state;
	current_state = HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN_LEFT);
	if (current_state != last_state)
	{
			if (HAL_GetTick() - last_time > 20) // 20 мс антидребезг
			{
					last_time = HAL_GetTick();
					last_state = current_state;
					if (current_state != GPIO_PIN_SET)
					return 1; // кнопка нажата
			}
	}
	return 0;
}
//=======Обработчик сентральной кнопки KEY2==================
uint8_t Button_Read_centr(void)
{
	static uint32_t last_time = 0;
	static uint8_t last_state = 0;
	uint8_t current_state;
	current_state = HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN_CENTR);
	if (current_state != last_state)
	{
			if (HAL_GetTick() - last_time > 20) // 20 мс антидребезг
			{
					last_time = HAL_GetTick();
					last_state = current_state;
					if (current_state == GPIO_PIN_SET)
					return 1; // кнопка нажата
			}
	}
	return 0;
}
//=======Обработчик правой кнопки KEY3==================
uint8_t Button_Read_right(void)
{
	static uint32_t last_time = 0;
	static uint8_t last_state = 0;
	uint8_t current_state;
	current_state = HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN_RIGHT);
	if (current_state != last_state)
	{
			if (HAL_GetTick() - last_time > 20) // 20 мс антидребезг
			{
					last_time = HAL_GetTick();
					last_state = current_state;
					if (current_state == GPIO_PIN_SET)
					return 1; // кнопка нажата
			}
	}
	return 0;
}
//=======Обработчик кнопок передатчиков===============
void Button_transmiter(void)
{
	uint8_t status;
	  	status = NRF24_ReadReg(STATUS);
	  	if(status & 0x40 && screen==1)
	  	{
	    	rx_data = NRF24L01_Receive();
	    	//>>>>>>>>>> Передатчик 1
	    	if(rx_data == 0x01)
	    	{
	    		if(on_off==1)//>>>>>>>>>>Фальшстарт отключен
	    				{
								key_nr=0x01;					//номер нажатой кнопки команды
								LED_ON;
								ILI9341_WriteString(7, 50, "team nr-1", Font_11x18, YELLOW, MYFON);				//Цвет отвечающей команды
	    				}
	    				else if(on_off==0 && timer_running==1)//>>>>>>>>>>Без фальшстарта
	    				{
								key_nr=0x01;					//номер нажатой кнопки команды
								LED_ON;
								ILI9341_WriteString(7, 50, "team nr-1", Font_11x18, YELLOW, MYFON);				//Цвет отвечающей команды
	    				}
	    				else if(on_off==0 && timer_running==0)//>>>>>>>>>>Фальшстарт
	    				{
								key_nr=0x01;					//номер нажатой кнопки команды
								ILI9341_WriteString(7, 50, "team nr-1", Font_11x18, BLACK, MYFON);				//Цвет отвечающей команды
								ILI9341_WriteString(175, 52, "!", Font_11x18, RED, MYFON);								//Цвет отвечающей команды
	    				}
	    	}
	    	//>>>>>>>>>> Передатчик 2
	    	if(rx_data == 0x02)
	    	{
	    		if(on_off==1)//>>>>>>>>>>Фальшстарт отключен
	    				{
								key_nr=0x02;					//номер нажатой кнопки команды
								LED_ON;
								ILI9341_WriteString(7, 70, "team nr-2", Font_11x18, YELLOW, MYFON);				//Цвет отвечающей команды
	    				}
	    				else if(on_off==0 && timer_running==1)//>>>>>>>>>>Без фальшстарта
	    				{
								key_nr=0x02;					//номер нажатой кнопки команды
								LED_ON;
								ILI9341_WriteString(7, 70, "team nr-2", Font_11x18, YELLOW, MYFON);				//Цвет отвечающей команды
	    				}
	    				else if(on_off==0 && timer_running==0)//>>>>>>>>>>Фальшстарт
	    				{
								key_nr=0x02;					//номер нажатой кнопки команды
								ILI9341_WriteString(7, 70, "team nr-2", Font_11x18, BLACK, MYFON);				//Цвет отвечающей команды
								ILI9341_WriteString(175, 72, "!", Font_11x18, RED, MYFON);								//Цвет отвечающей команды
	    				}
	    	}
	    	//>>>>>>>>>> Передатчик 3
	    	if(rx_data == 0x03)
	    	{
	    		if(on_off==1)//>>>>>>>>>>Фальшстарт отключен
	    				{
								key_nr=0x03;					//номер нажатой кнопки команды
								LED_ON;
								ILI9341_WriteString(7, 90, "team nr-3", Font_11x18, YELLOW, MYFON);				//Цвет отвечающей команды
	    				}
	    				else if(on_off==0 && timer_running==1)//>>>>>>>>>>Без фальшстарта
	    				{
								key_nr=0x03;					//номер нажатой кнопки команды
								LED_ON;
								ILI9341_WriteString(7, 90, "team nr-3", Font_11x18, YELLOW, MYFON);				//Цвет отвечающей команды
	    				}
	    				else if(on_off==0 && timer_running==0)//>>>>>>>>>>Фальшстарт
	    				{
								key_nr=0x03;					//номер нажатой кнопки команды
								ILI9341_WriteString(7, 90, "team nr-3", Font_11x18, BLACK, MYFON);				//Цвет отвечающей команды
								ILI9341_WriteString(175, 92, "!", Font_11x18, RED, MYFON);								//Цвет отвечающей команды
	    				}
	    	}
	    	//>>>>>>>>>> Передатчик 4
	    	if(rx_data == 0x04)
	    	{
	    		if(on_off==1)//>>>>>>>>>>Фальшстарт отключен
	    				{
								key_nr=0x04;					//номер нажатой кнопки команды
								LED_ON;
								ILI9341_WriteString(7, 110, "team nr-4", Font_11x18, YELLOW, MYFON);			//Цвет отвечающей команды
	    				}
	    				else if(on_off==0 && timer_running==1)//>>>>>>>>>>Без фальшстарта
	    				{
								key_nr=0x04;					//номер нажатой кнопки команды
								LED_ON;
								ILI9341_WriteString(7, 110, "team nr-4", Font_11x18, YELLOW, MYFON);			//Цвет отвечающей команды
	    				}
	    				else if(on_off==0 && timer_running==0)//>>>>>>>>>>Фальшстарт
	    				{
								key_nr=0x04;					//номер нажатой кнопки команды
								ILI9341_WriteString(7, 110, "team nr-4", Font_11x18, BLACK, MYFON);				//Цвет отвечающей команды
								ILI9341_WriteString(175, 112, "!", Font_11x18, RED, MYFON);								//Цвет отвечающей команды
	    				}
	    	}
	    	//>>>>>>>>>> Передатчик 5
	    	if(rx_data == 0x05)
	    	{
	    		if(on_off==1)//>>>>>>>>>>Фальшстарт отключен
	    				{
								key_nr=0x05;					//номер нажатой кнопки команды
								LED_ON;
								ILI9341_WriteString(7, 130, "team nr-5", Font_11x18, YELLOW, MYFON);			//Цвет отвечающей команды
	    				}
	    				else if(on_off==0 && timer_running==1)//>>>>>>>>>>Без фальшстарта
	    				{
								key_nr=0x05;					//номер нажатой кнопки команды
								LED_ON;
								ILI9341_WriteString(7, 130, "team nr-5", Font_11x18, YELLOW, MYFON);			//Цвет отвечающей команды
	    				}
	    				else if(on_off==0 && timer_running==0)//>>>>>>>>>>Фальшстарт
	    				{
								key_nr=0x05;					//номер нажатой кнопки команды
								ILI9341_WriteString(7, 130, "team nr-5", Font_11x18, BLACK, MYFON);				//Цвет отвечающей команды
								ILI9341_WriteString(175, 132, "!", Font_11x18, RED, MYFON);								//Цвет отвечающей команды
	    				}
	    	}
	    	//>>>>>>>>>> Передатчик 6
	    	if(rx_data == 0x06)
	    	{
	    		if(on_off==1)//>>>>>>>>>>Фальшстарт отключен
	    				{
								key_nr=0x06;					//номер нажатой кнопки команды
								LED_ON;
								ILI9341_WriteString(7, 150, "team nr-6", Font_11x18, YELLOW, MYFON);			//Цвет отвечающей команды
	    				}
	    				else if(on_off==0 && timer_running==1)//>>>>>>>>>>Без фальшстарта
	    				{
								key_nr=0x06;					//номер нажатой кнопки команды
								LED_ON;
								ILI9341_WriteString(7, 150, "team nr-6", Font_11x18, YELLOW, MYFON);			//Цвет отвечающей команды
	    				}
	    				else if(on_off==0 && timer_running==0)//>>>>>>>>>>Фальшстарт
	    				{
								key_nr=0x06;					//номер нажатой кнопки команды
								ILI9341_WriteString(7, 150, "team nr-6", Font_11x18, BLACK, MYFON);				//Цвет отвечающей команды
								ILI9341_WriteString(175, 152, "!", Font_11x18, RED, MYFON);								//Цвет отвечающей команды
	    				}
	    	}
	    	//>>>>>>>>>> Передатчик 7
	    	if(rx_data == 0x07)
	    	{
	    		if(on_off==1)//>>>>>>>>>>Фальшстарт отключен
	    				{
								key_nr=0x07;					//номер нажатой кнопки команды
								LED_ON;
								ILI9341_WriteString(7, 170, "team nr-7", Font_11x18, YELLOW, MYFON);			//Цвет отвечающей команды
	    				}
	    				else if(on_off==0 && timer_running==1)//>>>>>>>>>>Без фальшстарта
	    				{
								key_nr=0x07;					//номер нажатой кнопки команды
								LED_ON;
								ILI9341_WriteString(7, 170, "team nr-7", Font_11x18, YELLOW, MYFON);			//Цвет отвечающей команды
	    				}
	    				else if(on_off==0 && timer_running==0)//>>>>>>>>>>Фальшстарт
	    				{
								key_nr=0x07;					//номер нажатой кнопки команды
								ILI9341_WriteString(7, 170, "team nr-7", Font_11x18, BLACK, MYFON);				//Цвет отвечающей команды
								ILI9341_WriteString(175, 172, "!", Font_11x18, RED, MYFON);								//Цвет отвечающей команды
	    				}
	    	}
	    	//>>>>>>>>>> Передатчик 8
	    	if(rx_data == 0x08)
	    	{
	    		if(on_off==1)//>>>>>>>>>>Фальшстарт отключен
	    				{
								key_nr=0x08;					//номер нажатой кнопки команды
								LED_ON;
								ILI9341_WriteString(7, 190, "team nr-8", Font_11x18, YELLOW, MYFON);			//Цвет отвечающей команды
	    				}
	    				else if(on_off==0 && timer_running==1)//>>>>>>>>>>Без фальшстарта
	    				{
								key_nr=0x08;					//номер нажатой кнопки команды
								LED_ON;
								ILI9341_WriteString(7, 190, "team nr-8", Font_11x18, YELLOW, MYFON);			//Цвет отвечающей команды
	    				}
	    				else if(on_off==0 && timer_running==0)//>>>>>>>>>>Фальшстарт
	    				{
								key_nr=0x08;					//номер нажатой кнопки команды
								ILI9341_WriteString(7, 190, "team nr-8", Font_11x18, BLACK, MYFON);				//Цвет отвечающей команды
								ILI9341_WriteString(175, 192, "!", Font_11x18, RED, MYFON);								//Цвет отвечающей команды
	    				}
	    	}
	    	if(on_off==0 && timer_running==0)
	    	{
	    	ILI9341_Draw_Filled_Rectangle_Coord(220, 80, 280, 120, WHITE);										//Сброс тай мера при фальшстарте
	    	ILI9341_WriteString(233, 90, "RESET", Font_7x10, BLACK, WHITE);										//Цвет текста "reset falshstart"
	    	ILI9341_WriteString(230, 105, "TAIMER", Font_7x10, BLACK, WHITE);									//Цвет текста "reset falshstart"
	    	}
	    	HAL_TIM_Base_Stop_IT(&htim2);	//Останавливаем таймер
	  	}
}
void definition_of_coordinates(void)
	{
	if(HAL_GPIO_ReadPin(GPIOB, T_IRQ_Pin) == GPIO_PIN_RESET && flag_press) //если нажат тачскрин
	{
		x = 0;
		y = 0;
	if(ILI9341_TouchGetCoordinates(&x, &y))
	{
		flag_press = 0;
		//>>>>>>>>>>вывод координат в UART для отладки
		snprintf(buf, 64, "X = %d, Y = %d\n", x, y);
		HAL_UART_Transmit(&huart1, (uint8_t*)buf, strlen(buf), 100);
		buf[strlen(buf) - 1] = '\0';
		//ILI9341_WriteString(10, 120, buf, Font_11x18, WHITE, MYFON);//вывод координат на экран для отладки
	}

	switch(screen)
	{
	case 0:
	  if(x > 20 && x < 150 && y > 60 && y < 120) //если нажали брейн-ринг
	  {
	  	//>>>>>>>>>>Сбрасываем очки у всех команд
	  	score_nr1=0;							//количество очков команды 1
	  	score_nr2=0;							//количество очков команды 2
	  	score_nr3=0;							//количество очков команды 3
	  	score_nr4=0;							//количество очков команды 4
	  	score_nr5=0;							//количество очков команды 5
	  	score_nr6=0;							//количество очков команды 6
	  	score_nr7=0;							//количество очков команды 7
	  	score_nr8=0;							//количество очков команды 8


	  	//>>>>>>>>>>обработка состояния таймера
	  	g_timer_seconds = 60; 								//Устанавливаем начальное время
	  	update_flag = 1;							//Сброс таймера
	  	timer_running = 0;						//флаг остановки таймера
	  	HAL_TIM_Base_Stop_IT(&htim2);	//Останавливаем таймер
		  screen_Brain_Ring();					//переходим в экран игры "Брэйринг"
		  screen=1;											//устанавливаем переменную для страктуры "switch(screen)"
	  }
	  else if(x > 170 && x < 300 && y > 60 && y < 120) // если нажали Эрудит квартет
	  {
		  screen_Erudit_Quartet();
		  screen=2;
	  }
	  else if(x > 20 && x < 300 && y > 150 && y < 210)// если нажали setting
	  {
		screen_setting ();
		//>>>>>>>>>>Блок сохранения количесва команд в игре
		if(amount==2){ILI9341_WriteString(130, 110, "2", Font_16x26, WHITE, BLACK);HAL_Delay(500);}
		else if(amount==3){ILI9341_WriteString(130, 110, "3", Font_16x26, WHITE, BLACK);HAL_Delay(500);}
		else if(amount==4){ILI9341_WriteString(130, 110, "4", Font_16x26, WHITE, BLACK);HAL_Delay(500);}
		else if(amount==5){ILI9341_WriteString(130, 110, "5", Font_16x26, WHITE, BLACK);HAL_Delay(500);}
		else if(amount==6){ILI9341_WriteString(130, 110, "6", Font_16x26, WHITE, BLACK);HAL_Delay(500);}
		else if(amount==7){ILI9341_WriteString(130, 110, "7", Font_16x26, WHITE, BLACK);HAL_Delay(500);}
		else if(amount==8){ILI9341_WriteString(130, 110, "8", Font_16x26, WHITE, BLACK);HAL_Delay(500);}
		//>>>>>>>>>>Настройки фальшстарта
		if(on_off==0)
		{
		  ILI9341_WriteString(40, 80, "ON", Font_11x18, GREEN, RED);
		  ILI9341_WriteString(100, 80, "OFF", Font_11x18, DARKGREY, RED);
		}
		else if(on_off==1)
		{
		  ILI9341_WriteString(40, 80, "ON", Font_11x18, DARKGREY, RED);
		  ILI9341_WriteString(100, 80, "OFF", Font_11x18, GREEN, RED);
		}
		screen=3;
	  }
	break;
	//-----------------------------------------------------------------------------------------------------------------------------------
	case 1:
		editing_result();//редактирование результата
		if(on_off==0)
			{
			reset_timer();//выключение кнопки "reset taimer"
			}
	  if(x > 300 && x < 320 && y > 0 && y < 20)//если нажали крестик
	  {
		  screen_menu ();
		  screen=0;
	  }
	break;
	//-----------------------------------------------------------------------------------------------------------------------------------
	case 2:
	  if(x > 300 && x < 320 && y > 0 && y < 20) //если нажали крестик
	  {
		  screen_menu ();
		  screen=0;
	  }
	break;
	//-----------------------------------------------------------------------------------------------------------------------------------
	case 3://=============="setting"=================
	  if(x > 300 && x < 320 && y > 0 && y < 20) // если нажали крестик
	  {
		  screen_menu ();
		  screen=0;
	  }
	  else if(x > 40 && x < 60 && y > 80 && y < 100)//Фальшстарт ON
	  {
		  on_off=0;
		  ILI9341_WriteString(40, 80, "ON", Font_11x18, GREEN, RED);
		  ILI9341_WriteString(100, 80, "OFF", Font_11x18, DARKGREY, RED);
	  }
	  else if(x > 100 && x < 130 && y > 80 && y < 100)//Фальшстарт OFF
	  {
		  on_off=1;
		  ILI9341_WriteString(40, 80, "ON", Font_11x18, DARKGREY, RED);
		  ILI9341_WriteString(100, 80, "OFF", Font_11x18, GREEN, RED);
	  }
	  else if(x > 30 && x < 68 && y > 142 && y < 175)//Если добавляем комманды "-"
	  {
		  amount--;
		  if(amount==2){ILI9341_WriteString(130, 110, "2", Font_16x26, WHITE, BLACK);HAL_Delay(500);}
		  else if(amount==3){ILI9341_WriteString(130, 110, "3", Font_16x26, WHITE, BLACK);HAL_Delay(500);}
		  else if(amount==4){ILI9341_WriteString(130, 110, "4", Font_16x26, WHITE, BLACK);HAL_Delay(500);}
		  else if(amount==5){ILI9341_WriteString(130, 110, "5", Font_16x26, WHITE, BLACK);HAL_Delay(500);}
		  else if(amount==6){ILI9341_WriteString(130, 110, "6", Font_16x26, WHITE, BLACK);HAL_Delay(500);}
		  else if(amount==7){ILI9341_WriteString(130, 110, "7", Font_16x26, WHITE, BLACK);HAL_Delay(500);}
		  else if(amount==8){ILI9341_WriteString(130, 110, "8", Font_16x26, WHITE, BLACK);HAL_Delay(500);}
	  }
	  else if(x > 90 && x < 128 && y > 142 && y < 175)//Если удаляем комманды "+"
	  {
		  amount++;
		  if(amount==2){ILI9341_WriteString(130, 110, "2", Font_16x26, WHITE, BLACK);HAL_Delay(500);}
		  else if(amount==3){ILI9341_WriteString(130, 110, "3", Font_16x26, WHITE, BLACK);HAL_Delay(500);}
		  else if(amount==4){ILI9341_WriteString(130, 110, "4", Font_16x26, WHITE, BLACK);HAL_Delay(500);}
		  else if(amount==5){ILI9341_WriteString(130, 110, "5", Font_16x26, WHITE, BLACK);HAL_Delay(500);}
		  else if(amount==6){ILI9341_WriteString(130, 110, "6", Font_16x26, WHITE, BLACK);HAL_Delay(500);}
		  else if(amount==7){ILI9341_WriteString(130, 110, "7", Font_16x26, WHITE, BLACK);HAL_Delay(500);}
		  else if(amount==8){ILI9341_WriteString(130, 110, "8", Font_16x26, WHITE, BLACK);HAL_Delay(500);}
	  }
	  else if(x > 30 && x < 140 && y > 190 && y < 220)//обработчик кнопки "save"
	  {
		  screen_menu();
		  screen=0;
	  }
	break;
	//-----------------------------------------------------------------------------------------------------------------------------------
	case 4:

	break;
	default:
		//Тут пишем код который не попал ни под один "case"
	return;
	}

		}
		if(!flag_press && (HAL_GetTick() - time_press) > 200) // задержка до следующего нажатия
		{
		flag_press = 1;
		}
	}





