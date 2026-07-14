/*
 * xpt2046_touch.c
 *
 *  Created on: 5 апр. 2020 г.
 *      Author: dima
 */


#include "main.h"
#include "xpt2046_touch.h"
#include "ILI9341_GFX.h"
#include "nrf24.h"
#include "stdio.h"
#include "string.h"
#include "button.h"

#define TOUCH_SCALE_X 320
#define TOUCH_SCALE_Y 240

#define TOUCH_MIN_RAW_X 1500
#define TOUCH_MAX_RAW_X 30000
#define TOUCH_MIN_RAW_Y 2500
#define TOUCH_MAX_RAW_Y 30000

extern TIM_HandleTypeDef htim2;

static const uint8_t cmd_read_x = 0x90;
static const uint8_t cmd_read_y = 0xD0;
static const uint8_t zeroes_tx[] = {0x00, 0x00};

extern uint16_t ed_result;
extern uint16_t score_nr1;							//количество очков команды 1
extern uint16_t score_nr2;							//количество очков команды 2
extern uint16_t score_nr3;							//количество очков команды 3
extern uint16_t score_nr4;							//количество очков команды 4
extern uint16_t score_nr5;							//количество очков команды 5
extern uint16_t score_nr6;							//количество очков команды 6
extern uint16_t score_nr7;							//количество очков команды 7
extern uint16_t score_nr8;							//количество очков команды 8
extern uint16_t screen;
extern uint16_t x;
extern uint16_t y;
extern uint8_t flag_press;
extern uint16_t amount;								//номер команды
extern char scope_buf[6];

extern uint8_t timer_running;
extern volatile int g_timer_seconds;						//Начальное значение таймера
extern volatile uint8_t update_flag;		//старт/стоп таймера

// калибровка
/*#include "string.h"
#include "stdio.h"
extern UART_HandleTypeDef huart1;*/

uint8_t ILI9341_TouchGetCoordinates(uint16_t *x, uint16_t *y)
{
	if(HAL_GPIO_ReadPin(GPIOB, T_IRQ_Pin) != GPIO_PIN_RESET) return 0;
  uint32_t avg_x = 0;
  uint32_t avg_y = 0;
	HAL_SPI_Transmit(TOUCH_SPI_PTR, (uint8_t*)&cmd_read_y, 1, 1000);
	uint8_t y_raw[2] = {0,};
	HAL_SPI_TransmitReceive(TOUCH_SPI_PTR, (uint8_t*)zeroes_tx, y_raw, 2, 1000);
	HAL_SPI_Transmit(TOUCH_SPI_PTR, (uint8_t*)&cmd_read_x, 1, 1000);
	uint8_t x_raw[2] = {0,};
	HAL_SPI_TransmitReceive(TOUCH_SPI_PTR, (uint8_t*)zeroes_tx, x_raw, 2, 1000);
	avg_x = (((uint16_t)x_raw[0]) << 8) | ((uint16_t)x_raw[1]);
	avg_y = (((uint16_t)y_raw[0]) << 8) | ((uint16_t)y_raw[1]);
	// калибровка
	/*char buf[64] = {0,};
	snprintf(buf, 64, "ADC_X = %lu, ADC_Y = %lu\n", avg_x, avg_y);
	HAL_UART_Transmit(&huart1, (uint8_t*)buf, strlen(buf), 100);*/
	if(avg_x < TOUCH_MIN_RAW_X) avg_x = TOUCH_MIN_RAW_X;
	if(avg_x > TOUCH_MAX_RAW_X) avg_x = TOUCH_MAX_RAW_X;

	if(avg_y < TOUCH_MIN_RAW_X) avg_y = TOUCH_MIN_RAW_Y;
	if(avg_y > TOUCH_MAX_RAW_Y) avg_y = TOUCH_MAX_RAW_Y;

	*x = (avg_x - TOUCH_MIN_RAW_X) * TOUCH_SCALE_X / (TOUCH_MAX_RAW_X - TOUCH_MIN_RAW_X);
	*y = (avg_y - TOUCH_MIN_RAW_Y) * TOUCH_SCALE_Y / (TOUCH_MAX_RAW_Y - TOUCH_MIN_RAW_Y);
  return 1;
}
void editing_result(void)
{
	if(x > 140 && x < 170 && y > 50 && y < 70)//Редактирование результата команды 1
	{
		//код
		ed_result=1;
		snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr1);
		ILI9341_WriteString(135, 50, scope_buf, Font_11x18, ORANGE, MYFON);

		ILI9341_Draw_Filled_Rectangle_Coord(20, 220, 60, 240, WHITE);					//координаты кнопки "+"
		ILI9341_Draw_Filled_Rectangle_Coord(130, 220, 190, 240, WHITE);				//координаты кнопки "save"
		ILI9341_Draw_Filled_Rectangle_Coord(260, 220, 300, 240, WHITE);				//координаты кнопки "-"
		ILI9341_WriteString(30, 225, "+", Font_7x10, BLUE, WHITE);
		ILI9341_WriteString(142, 225, "save", Font_7x10, BLUE, WHITE);
		ILI9341_WriteString(272, 225, "-", Font_7x10, BLUE, WHITE);
	}
	else if (x > 140 && x < 170 && y > 70 && y < 90)//Редактирование результата команды 2
	{
		//код
		ed_result=2;
		snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr2);
		ILI9341_WriteString(135, 70, scope_buf, Font_11x18, ORANGE, MYFON);

		ILI9341_Draw_Filled_Rectangle_Coord(20, 220, 60, 240, WHITE);					//координаты кнопки "+"
		ILI9341_Draw_Filled_Rectangle_Coord(130, 220, 190, 240, WHITE);				//координаты кнопки "save"
		ILI9341_Draw_Filled_Rectangle_Coord(260, 220, 300, 240, WHITE);				//координаты кнопки "-"
		ILI9341_WriteString(30, 225, "+", Font_7x10, BLUE, WHITE);
		ILI9341_WriteString(142, 225, "save", Font_7x10, BLUE, WHITE);
		ILI9341_WriteString(272, 225, "-", Font_7x10, BLUE, WHITE);
	}
	else if (x > 140 && x < 170 && y > 90 && y < 110)//Редактирование результата команды 3
	{
		//код
		ed_result=3;
		snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr3);
		ILI9341_WriteString(135, 90, scope_buf, Font_11x18, ORANGE, MYFON);

		ILI9341_Draw_Filled_Rectangle_Coord(20, 220, 60, 240, WHITE);					//координаты кнопки "+"
		ILI9341_Draw_Filled_Rectangle_Coord(130, 220, 190, 240, WHITE);				//координаты кнопки "save"
		ILI9341_Draw_Filled_Rectangle_Coord(260, 220, 300, 240, WHITE);				//координаты кнопки "-"
		ILI9341_WriteString(30, 225, "+", Font_7x10, BLUE, WHITE);
		ILI9341_WriteString(142, 225, "save", Font_7x10, BLUE, WHITE);
		ILI9341_WriteString(272, 225, "-", Font_7x10, BLUE, WHITE);
	}
	else if (x > 140 && x < 170 && y > 110 && y < 130)//Редактирование результата команды 4
	{
		//код
		ed_result=4;
		snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr4);
		ILI9341_WriteString(135, 110, scope_buf, Font_11x18, ORANGE, MYFON);

		ILI9341_Draw_Filled_Rectangle_Coord(20, 220, 60, 240, WHITE);					//координаты кнопки "+"
		ILI9341_Draw_Filled_Rectangle_Coord(130, 220, 190, 240, WHITE);				//координаты кнопки "save"
		ILI9341_Draw_Filled_Rectangle_Coord(260, 220, 300, 240, WHITE);				//координаты кнопки "-"
		ILI9341_WriteString(30, 225, "+", Font_7x10, BLUE, WHITE);
		ILI9341_WriteString(142, 225, "save", Font_7x10, BLUE, WHITE);
		ILI9341_WriteString(272, 225, "-", Font_7x10, BLUE, WHITE);
	}
	else if (x > 140 && x < 170 && y > 130 && y < 150)//Редактирование результата команды 5
	{
		//код
		ed_result=5;
		snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr5);
		ILI9341_WriteString(135, 130, scope_buf, Font_11x18, ORANGE, MYFON);

		ILI9341_Draw_Filled_Rectangle_Coord(20, 220, 60, 240, WHITE);					//координаты кнопки "+"
		ILI9341_Draw_Filled_Rectangle_Coord(130, 220, 190, 240, WHITE);				//координаты кнопки "save"
		ILI9341_Draw_Filled_Rectangle_Coord(260, 220, 300, 240, WHITE);				//координаты кнопки "-"
		ILI9341_WriteString(30, 225, "+", Font_7x10, BLUE, WHITE);
		ILI9341_WriteString(142, 225, "save", Font_7x10, BLUE, WHITE);
		ILI9341_WriteString(272, 225, "-", Font_7x10, BLUE, WHITE);
	}
	else if (x > 140 && x < 170 && y > 150 && y < 170)//Редактирование результата команды 6
	{
		//код
		ed_result=6;
		snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr6);
		ILI9341_WriteString(135, 150, scope_buf, Font_11x18, ORANGE, MYFON);

		ILI9341_Draw_Filled_Rectangle_Coord(20, 220, 60, 240, WHITE);					//координаты кнопки "+"
		ILI9341_Draw_Filled_Rectangle_Coord(130, 220, 190, 240, WHITE);				//координаты кнопки "save"
		ILI9341_Draw_Filled_Rectangle_Coord(260, 220, 300, 240, WHITE);				//координаты кнопки "-"
		ILI9341_WriteString(30, 225, "+", Font_7x10, BLUE, WHITE);
		ILI9341_WriteString(142, 225, "save", Font_7x10, BLUE, WHITE);
		ILI9341_WriteString(272, 225, "-", Font_7x10, BLUE, WHITE);
	}
	else if (x > 140 && x < 170 && y > 170 && y < 190)//Редактирование результата команды 7
	{
		//код
		ed_result=7;
		snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr7);
		ILI9341_WriteString(135, 170, scope_buf, Font_11x18, ORANGE, MYFON);

		ILI9341_Draw_Filled_Rectangle_Coord(20, 220, 60, 240, WHITE);					//координаты кнопки "+"
		ILI9341_Draw_Filled_Rectangle_Coord(130, 220, 190, 240, WHITE);				//координаты кнопки "save"
		ILI9341_Draw_Filled_Rectangle_Coord(260, 220, 300, 240, WHITE);				//координаты кнопки "-"
		ILI9341_WriteString(30, 225, "+", Font_7x10, BLUE, WHITE);
		ILI9341_WriteString(142, 225, "save", Font_7x10, BLUE, WHITE);
		ILI9341_WriteString(272, 225, "-", Font_7x10, BLUE, WHITE);
	}
	else if (x > 140 && x < 170 && y > 190 && y < 210)//Редактирование результата команды 8
	{
		//код
		ed_result=8;
		snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr8);
		ILI9341_WriteString(135, 190, scope_buf, Font_11x18, ORANGE, MYFON);

		ILI9341_Draw_Filled_Rectangle_Coord(20, 220, 60, 240, WHITE);					//координаты кнопки "+"
		ILI9341_Draw_Filled_Rectangle_Coord(130, 220, 190, 240, WHITE);				//координаты кнопки "save"
		ILI9341_Draw_Filled_Rectangle_Coord(260, 220, 300, 240, WHITE);				//координаты кнопки "-"
		ILI9341_WriteString(30, 225, "+", Font_7x10, BLUE, WHITE);
		ILI9341_WriteString(142, 225, "save", Font_7x10, BLUE, WHITE);
		ILI9341_WriteString(272, 225, "-", Font_7x10, BLUE, WHITE);
	}
}
void editing_result_key(void)
{
//>>>>>>>>>>Левая кнопка
	if(Button_Read_left())
	{
		switch(ed_result)
		{
		    case 1:
		  		score_nr1++;
		  		snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr1);
		  		ILI9341_WriteString(135, 50, scope_buf, Font_11x18, ORANGE, MYFON);
		        break;

		    case 2:
		  		score_nr2++;
		  		snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr2);
		  		ILI9341_WriteString(135, 70, scope_buf, Font_11x18, ORANGE, MYFON);
		        break;

		    case 3:
		  		score_nr3++;
		  		snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr3);
		  		ILI9341_WriteString(135, 90, scope_buf, Font_11x18, ORANGE, MYFON);
		        break;

		    case 4:
		  		score_nr4++;
		  		snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr4);
		  		ILI9341_WriteString(135, 110, scope_buf, Font_11x18, ORANGE, MYFON);
		        break;

		    case 5:
		  		score_nr5++;
		  		snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr5);
		  		ILI9341_WriteString(135, 130, scope_buf, Font_11x18, ORANGE, MYFON);
		        break;

		    case 6:
		  		score_nr6++;
		  		snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr6);
		  		ILI9341_WriteString(135, 150, scope_buf, Font_11x18, ORANGE, MYFON);
		        break;

		    case 7:
		  		score_nr7++;
		  		snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr7);
		  		ILI9341_WriteString(135, 170, scope_buf, Font_11x18, ORANGE, MYFON);
		        break;

		    case 8:
		  		score_nr8++;
		  		snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr8);
		  		ILI9341_WriteString(135, 190, scope_buf, Font_11x18, ORANGE, MYFON);
		        break;

		    default:
		        // код если ничего не совпало
		        break;
		}
	}
//>>>>>>>>>>правая кнопка
	if(Button_Read_right())
	{
		switch(ed_result)
		{
				case 1:
					if(score_nr1 > 0) score_nr1--;
					snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr1);
					ILI9341_WriteString(135, 50, scope_buf, Font_11x18, ORANGE, MYFON);
						break;

				case 2:
					if(score_nr2 > 0) score_nr2--;
					snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr2);
					ILI9341_WriteString(135, 70, scope_buf, Font_11x18, ORANGE, MYFON);
						break;

				case 3:
					if(score_nr3 > 0) score_nr3--;
					snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr3);
					ILI9341_WriteString(135, 90, scope_buf, Font_11x18, ORANGE, MYFON);
						break;

				case 4:
					if(score_nr4 > 0) score_nr4--;
					snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr4);
					ILI9341_WriteString(135, 110, scope_buf, Font_11x18, ORANGE, MYFON);
						break;

				case 5:
					if(score_nr5 > 0) score_nr5--;
					snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr5);
					ILI9341_WriteString(135, 130, scope_buf, Font_11x18, ORANGE, MYFON);
						break;

				case 6:
					if(score_nr6 > 0) score_nr6--;
					snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr6);
					ILI9341_WriteString(135, 150, scope_buf, Font_11x18, ORANGE, MYFON);
						break;

				case 7:
					if(score_nr7 > 0) score_nr7--;
					snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr7);
					ILI9341_WriteString(135, 170, scope_buf, Font_11x18, ORANGE, MYFON);
						break;

				case 8:
					if(score_nr8 > 0) score_nr8--;
					snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr8);
					ILI9341_WriteString(135, 190, scope_buf, Font_11x18, ORANGE, MYFON);
						break;

				default:
						// код если ничего не совпало
						break;
		}
	}
//>>>>>>>>>>Центральная кнопка
	if(Button_Read_centr() && ed_result!=0)
	{
		ILI9341_Draw_Filled_Rectangle_Coord(20, 220, 60, 240, WHITE);					//координаты кнопки "YES"
		ILI9341_Draw_Filled_Rectangle_Coord(130, 220, 190, 240, WHITE);				//координаты кнопки "start"
		ILI9341_Draw_Filled_Rectangle_Coord(260, 220, 300, 240, WHITE);				//координаты кнопки "YES"
		ILI9341_WriteString(30, 225, "yes", Font_7x10, BLUE, WHITE);
		ILI9341_WriteString(142, 225, "start", Font_7x10, BLUE, WHITE);
		ILI9341_WriteString(272, 225, "no", Font_7x10, BLUE, WHITE);
		ed_result=0;
		if(amount==2)
		{
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr1);
			ILI9341_WriteString(135, 50, scope_buf, Font_11x18, WHITE, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr2);
			ILI9341_WriteString(135, 70, scope_buf, Font_11x18, WHITE, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr3);
			ILI9341_WriteString(135, 90, scope_buf, Font_11x18, BLACK, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr4);
			ILI9341_WriteString(135, 110, scope_buf, Font_11x18, BLACK, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr5);
			ILI9341_WriteString(135, 130, scope_buf, Font_11x18, BLACK, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr6);
			ILI9341_WriteString(135, 150, scope_buf, Font_11x18, BLACK, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr7);
			ILI9341_WriteString(135, 170, scope_buf, Font_11x18, BLACK, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr8);
			ILI9341_WriteString(135, 190, scope_buf, Font_11x18, BLACK, MYFON);
		}else if(amount==3){
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr1);
			ILI9341_WriteString(135, 50, scope_buf, Font_11x18, WHITE, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr2);
			ILI9341_WriteString(135, 70, scope_buf, Font_11x18, WHITE, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr3);
			ILI9341_WriteString(135, 90, scope_buf, Font_11x18, WHITE, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr4);
			ILI9341_WriteString(135, 110, scope_buf, Font_11x18, BLACK, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr5);
			ILI9341_WriteString(135, 130, scope_buf, Font_11x18, BLACK, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr6);
			ILI9341_WriteString(135, 150, scope_buf, Font_11x18, BLACK, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr7);
			ILI9341_WriteString(135, 170, scope_buf, Font_11x18, BLACK, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr8);
			ILI9341_WriteString(135, 190, scope_buf, Font_11x18, BLACK, MYFON);
		}else if(amount==4){
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr1);
			ILI9341_WriteString(135, 50, scope_buf, Font_11x18, WHITE, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr2);
			ILI9341_WriteString(135, 70, scope_buf, Font_11x18, WHITE, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr3);
			ILI9341_WriteString(135, 90, scope_buf, Font_11x18, WHITE, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr4);
			ILI9341_WriteString(135, 110, scope_buf, Font_11x18, WHITE, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr5);
			ILI9341_WriteString(135, 130, scope_buf, Font_11x18, BLACK, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr6);
			ILI9341_WriteString(135, 150, scope_buf, Font_11x18, BLACK, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr7);
			ILI9341_WriteString(135, 170, scope_buf, Font_11x18, BLACK, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr8);
			ILI9341_WriteString(135, 190, scope_buf, Font_11x18, BLACK, MYFON);
		}else if(amount==5){
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr1);
			ILI9341_WriteString(135, 50, scope_buf, Font_11x18, WHITE, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr2);
			ILI9341_WriteString(135, 70, scope_buf, Font_11x18, WHITE, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr3);
			ILI9341_WriteString(135, 90, scope_buf, Font_11x18, WHITE, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr4);
			ILI9341_WriteString(135, 110, scope_buf, Font_11x18, WHITE, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr5);
			ILI9341_WriteString(135, 130, scope_buf, Font_11x18, WHITE, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr6);
			ILI9341_WriteString(135, 150, scope_buf, Font_11x18, BLACK, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr7);
			ILI9341_WriteString(135, 170, scope_buf, Font_11x18, BLACK, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr8);
			ILI9341_WriteString(135, 190, scope_buf, Font_11x18, BLACK, MYFON);
		}else if(amount==6){
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr1);
			ILI9341_WriteString(135, 50, scope_buf, Font_11x18, WHITE, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr2);
			ILI9341_WriteString(135, 70, scope_buf, Font_11x18, WHITE, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr3);
			ILI9341_WriteString(135, 90, scope_buf, Font_11x18, WHITE, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr4);
			ILI9341_WriteString(135, 110, scope_buf, Font_11x18, WHITE, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr5);
			ILI9341_WriteString(135, 130, scope_buf, Font_11x18, WHITE, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr6);
			ILI9341_WriteString(135, 150, scope_buf, Font_11x18, WHITE, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr7);
			ILI9341_WriteString(135, 170, scope_buf, Font_11x18, BLACK, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr8);
			ILI9341_WriteString(135, 190, scope_buf, Font_11x18, BLACK, MYFON);
		}else if(amount==7){
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr1);
			ILI9341_WriteString(135, 50, scope_buf, Font_11x18, WHITE, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr2);
			ILI9341_WriteString(135, 70, scope_buf, Font_11x18, WHITE, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr3);
			ILI9341_WriteString(135, 90, scope_buf, Font_11x18, WHITE, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr4);
			ILI9341_WriteString(135, 110, scope_buf, Font_11x18, WHITE, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr5);
			ILI9341_WriteString(135, 130, scope_buf, Font_11x18, WHITE, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr6);
			ILI9341_WriteString(135, 150, scope_buf, Font_11x18, WHITE, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr7);
			ILI9341_WriteString(135, 170, scope_buf, Font_11x18, WHITE, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr8);
			ILI9341_WriteString(135, 190, scope_buf, Font_11x18, BLACK, MYFON);
		}else if(amount==8){
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr1);
			ILI9341_WriteString(135, 50, scope_buf, Font_11x18, WHITE, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr2);
			ILI9341_WriteString(135, 70, scope_buf, Font_11x18, WHITE, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr3);
			ILI9341_WriteString(135, 90, scope_buf, Font_11x18, WHITE, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr4);
			ILI9341_WriteString(135, 110, scope_buf, Font_11x18, WHITE, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr5);
			ILI9341_WriteString(135, 130, scope_buf, Font_11x18, WHITE, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr6);
			ILI9341_WriteString(135, 150, scope_buf, Font_11x18, WHITE, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr7);
			ILI9341_WriteString(135, 170, scope_buf, Font_11x18, WHITE, MYFON);
			snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr8);
			ILI9341_WriteString(135, 190, scope_buf, Font_11x18, WHITE, MYFON);
		}
	}
}
void reset_timer(void)
{
	if(x > 220 && x < 280 && y > 80 && y < 120)
	{
		ILI9341_Draw_Filled_Rectangle_Coord(220, 80, 280, 120, MYFON);		//Сброс тай мера при фальшстарте
  	g_timer_seconds = 60; 																										//Устанавливаем начальное время
  	update_flag = 1;																									//Сброс таймера
  	timer_running = 0;																								//флаг остановки таймера
  	HAL_TIM_Base_Stop_IT(&htim2);																			//Останавливаем таймер
	}
}










