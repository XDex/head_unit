/*
 * screen.c
 *
 *  Created on: 6 апр. 2026 г.
 *      Author: dima
 */
#include "main.h"
#include "screen.h"
#include "ILI9341_GFX.h"
#include "xpt2046_touch.h"
#include "string.h"
#include "stdio.h"
#include "fonts.h"
	uint16_t z=0;
	uint16_t h=0;
	extern uint16_t screen;
	extern uint16_t amount;
	extern uint16_t key_nr;						//номер нажатой кнопки команды
	extern uint16_t score_nr1;				//количество очков команды 1
	extern uint16_t score_nr2;				//количество очков команды 2
	extern uint16_t score_nr3;				//количество очков команды 3
	extern uint16_t score_nr4;				//количество очков команды 4
	extern uint16_t score_nr5;				//количество очков команды 5
	extern uint16_t score_nr6;				//количество очков команды 6
	extern uint16_t score_nr7;				//количество очков команды 7
	extern uint16_t score_nr8;				//количество очков команды 8
	extern uint16_t tf;								//положительный или отрицательный ответ
	char scope_buf[6];



//============= координатная сетка =====================
void The_coordinate_grid ()
{
	for(z=20; z<240; z+=20)
		{
		ILI9341_Draw_Horizontal_Line(0, z, 320, WHITE);
		}
	for(h=20; h<320; h+=20)
		{
		ILI9341_Draw_Vertical_Line(h, 0, 240, WHITE);
		}
}
//============= Турнирная таблица =====================
void tournament_table ()
{
	for(z=50; z<220; z+=20)
		{
		ILI9341_Draw_Horizontal_Line(120, z, 80, RED);
		}
	for(h=120; h<200; h+=40)
		{
		ILI9341_Draw_Vertical_Line(h, 50, 160, RED);
		}
}
//==============установка цвета названия комманд для следующего вопроса===========
void reset_color()
{
if(amount==2)
	{
		ILI9341_WriteString(7, 50, "team nr-1", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 70, "team nr-2", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 90, "team nr-3", Font_11x18, BLACK, MYFON);
		ILI9341_WriteString(7, 110, "team nr-4", Font_11x18, BLACK, MYFON);
		ILI9341_WriteString(7, 130, "team nr-5", Font_11x18, BLACK, MYFON);
		ILI9341_WriteString(7, 150, "team nr-6", Font_11x18, BLACK, MYFON);
		ILI9341_WriteString(7, 170, "team nr-7", Font_11x18, BLACK, MYFON);
		ILI9341_WriteString(7, 190, "team nr-8", Font_11x18, BLACK, MYFON);
	}else if(amount==3){
		ILI9341_WriteString(7, 50, "team nr-1", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 70, "team nr-2", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 90, "team nr-3", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 110, "team nr-4", Font_11x18, BLACK, MYFON);
		ILI9341_WriteString(7, 130, "team nr-5", Font_11x18, BLACK, MYFON);
		ILI9341_WriteString(7, 150, "team nr-6", Font_11x18, BLACK, MYFON);
		ILI9341_WriteString(7, 170, "team nr-7", Font_11x18, BLACK, MYFON);
		ILI9341_WriteString(7, 190, "team nr-8", Font_11x18, BLACK, MYFON);
	}else if(amount==4){
		ILI9341_WriteString(7, 50, "team nr-1", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 70, "team nr-2", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 90, "team nr-3", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 110, "team nr-4", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 130, "team nr-5", Font_11x18, BLACK, MYFON);
		ILI9341_WriteString(7, 150, "team nr-6", Font_11x18, BLACK, MYFON);
		ILI9341_WriteString(7, 170, "team nr-7", Font_11x18, BLACK, MYFON);
		ILI9341_WriteString(7, 190, "team nr-8", Font_11x18, BLACK, MYFON);
	}else if(amount==5){
		ILI9341_WriteString(7, 50, "team nr-1", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 70, "team nr-2", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 90, "team nr-3", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 110, "team nr-4", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 130, "team nr-5", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 150, "team nr-6", Font_11x18, BLACK, MYFON);
		ILI9341_WriteString(7, 170, "team nr-7", Font_11x18, BLACK, MYFON);
		ILI9341_WriteString(7, 190, "team nr-8", Font_11x18, BLACK, MYFON);
	}else if(amount==6){
		ILI9341_WriteString(7, 50, "team nr-1", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 70, "team nr-2", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 90, "team nr-3", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 110, "team nr-4", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 130, "team nr-5", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 150, "team nr-6", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 170, "team nr-7", Font_11x18, BLACK, MYFON);
		ILI9341_WriteString(7, 190, "team nr-8", Font_11x18, BLACK, MYFON);
	}else if(amount==7){
		ILI9341_WriteString(7, 50, "team nr-1", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 70, "team nr-2", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 90, "team nr-3", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 110, "team nr-4", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 130, "team nr-5", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 150, "team nr-6", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 170, "team nr-7", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 190, "team nr-8", Font_11x18, BLACK, MYFON);
	}else if(amount==8){
		ILI9341_WriteString(7, 50, "team nr-1", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 70, "team nr-2", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 90, "team nr-3", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 110, "team nr-4", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 130, "team nr-5", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 150, "team nr-6", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 170, "team nr-7", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 190, "team nr-8", Font_11x18, WHITE, MYFON);
	}
}
//============= экран выбора игры =====================
void screen_menu ()
{
	screen=0;
	ILI9341_Fill_Screen(YELLOW);
	ILI9341_Draw_Filled_Rectangle_Coord(10, 10, 310, 230, RED);
	ILI9341_WriteString(90, 12, "MAIN MENU", Font_16x26, BLACK, RED);
	ILI9341_Draw_Filled_Rectangle_Coord(20, 60, 150, 120, BLUE);				//координаты кнопки Brain Ring
	ILI9341_Draw_Filled_Rectangle_Coord(170, 60, 300, 120, BLUE);				//координаты кнопки Erudit Quartet
	ILI9341_Draw_Filled_Rectangle_Coord(20, 150, 300, 210, BLUE);				//координаты кнопки setting
	ILI9341_WriteString(30, 80, "Brain Ring", Font_11x18, ORANGE, BLUE);
	ILI9341_WriteString(203, 70, "Erudit", Font_11x18, ORANGE, BLUE);
	ILI9341_WriteString(195, 90, "Quartet", Font_11x18, ORANGE, BLUE);
	ILI9341_WriteString(100, 165, "Setting", Font_16x26, ORANGE, BLUE);

	//The_coordinate_grid ();
	//HAL_Delay(10000);
}
//================== экран Brain Ring ==================
void screen_Brain_Ring()
{
	ILI9341_Fill_Screen(YELLOW);																					//Основной фон
	ILI9341_Draw_Filled_Rectangle_Coord(5, 5, 315, 235, MYFON);						//внутренний фон
	//The_coordinate_grid ();
	ILI9341_WriteString(20, 10, "Brain_Ring", Font_11x18, OLIVE, MYFON);
	ILI9341_WriteString(228, 10, "Timer:", Font_7x10, BLUE, MYFON);
	ILI9341_WriteString(120, 36, "scores", Font_7x10, BLUE, MYFON);
	ILI9341_WriteString(175, 38, "FS", Font_7x10, RED, MYFON);
	ILI9341_Draw_Filled_Rectangle_Coord(20, 220, 60, 240, WHITE);					//координаты кнопки "YES"
	ILI9341_Draw_Filled_Rectangle_Coord(130, 220, 190, 240, WHITE);				//координаты кнопки "start"
	ILI9341_Draw_Filled_Rectangle_Coord(260, 220, 300, 240, WHITE);				//координаты кнопки "YES"
	ILI9341_WriteString(30, 225, "yes", Font_7x10, BLUE, WHITE);
	ILI9341_WriteString(142, 225, "start", Font_7x10, BLUE, WHITE);
	ILI9341_WriteString(272, 225, "no", Font_7x10, BLUE, WHITE);
	tournament_table ();
	//>>>>>>>>>>Графическое отображение количетсва играющих команд
	if(amount==2)
	{
		ILI9341_WriteString(7, 50, "team nr-1", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 70, "team nr-2", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 90, "team nr-3", Font_11x18, BLACK, MYFON);
		ILI9341_WriteString(7, 110, "team nr-4", Font_11x18, BLACK, MYFON);
		ILI9341_WriteString(7, 130, "team nr-5", Font_11x18, BLACK, MYFON);
		ILI9341_WriteString(7, 150, "team nr-6", Font_11x18, BLACK, MYFON);
		ILI9341_WriteString(7, 170, "team nr-7", Font_11x18, BLACK, MYFON);
		ILI9341_WriteString(7, 190, "team nr-8", Font_11x18, BLACK, MYFON);

		ILI9341_WriteString(135, 50, "0", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(135, 70, "0", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(135, 90, "0", Font_11x18, BLACK, MYFON);
		ILI9341_WriteString(135, 110, "0", Font_11x18, BLACK, MYFON);
		ILI9341_WriteString(135, 130, "0", Font_11x18, BLACK, MYFON);
		ILI9341_WriteString(135, 150, "0", Font_11x18, BLACK, MYFON);
		ILI9341_WriteString(135, 170, "0", Font_11x18, BLACK, MYFON);
		ILI9341_WriteString(135, 190, "0", Font_11x18, BLACK, MYFON);
	}else if(amount==3){
		ILI9341_WriteString(7, 50, "team nr-1", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 70, "team nr-2", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 90, "team nr-3", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 110, "team nr-4", Font_11x18, BLACK, MYFON);
		ILI9341_WriteString(7, 130, "team nr-5", Font_11x18, BLACK, MYFON);
		ILI9341_WriteString(7, 150, "team nr-6", Font_11x18, BLACK, MYFON);
		ILI9341_WriteString(7, 170, "team nr-7", Font_11x18, BLACK, MYFON);
		ILI9341_WriteString(7, 190, "team nr-8", Font_11x18, BLACK, MYFON);

		ILI9341_WriteString(135, 50, "0", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(135, 70, "0", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(135, 90, "0", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(135, 110, "0", Font_11x18, BLACK, MYFON);
		ILI9341_WriteString(135, 130, "0", Font_11x18, BLACK, MYFON);
		ILI9341_WriteString(135, 150, "0", Font_11x18, BLACK, MYFON);
		ILI9341_WriteString(135, 170, "0", Font_11x18, BLACK, MYFON);
		ILI9341_WriteString(135, 190, "0", Font_11x18, BLACK, MYFON);
	}else if(amount==4){
		ILI9341_WriteString(7, 50, "team nr-1", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 70, "team nr-2", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 90, "team nr-3", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 110, "team nr-4", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 130, "team nr-5", Font_11x18, BLACK, MYFON);
		ILI9341_WriteString(7, 150, "team nr-6", Font_11x18, BLACK, MYFON);
		ILI9341_WriteString(7, 170, "team nr-7", Font_11x18, BLACK, MYFON);
		ILI9341_WriteString(7, 190, "team nr-8", Font_11x18, BLACK, MYFON);

		ILI9341_WriteString(135, 50, "0", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(135, 70, "0", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(135, 90, "0", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(135, 110, "0", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(135, 130, "0", Font_11x18, BLACK, MYFON);
		ILI9341_WriteString(135, 150, "0", Font_11x18, BLACK, MYFON);
		ILI9341_WriteString(135, 170, "0", Font_11x18, BLACK, MYFON);
		ILI9341_WriteString(135, 190, "0", Font_11x18, BLACK, MYFON);
	}else if(amount==5){
		ILI9341_WriteString(7, 50, "team nr-1", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 70, "team nr-2", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 90, "team nr-3", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 110, "team nr-4", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 130, "team nr-5", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 150, "team nr-6", Font_11x18, BLACK, MYFON);
		ILI9341_WriteString(7, 170, "team nr-7", Font_11x18, BLACK, MYFON);
		ILI9341_WriteString(7, 190, "team nr-8", Font_11x18, BLACK, MYFON);

		ILI9341_WriteString(135, 50, "0", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(135, 70, "0", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(135, 90, "0", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(135, 110, "0", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(135, 130, "0", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(135, 150, "0", Font_11x18, BLACK, MYFON);
		ILI9341_WriteString(135, 170, "0", Font_11x18, BLACK, MYFON);
		ILI9341_WriteString(135, 190, "0", Font_11x18, BLACK, MYFON);
	}else if(amount==6){
		ILI9341_WriteString(7, 50, "team nr-1", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 70, "team nr-2", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 90, "team nr-3", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 110, "team nr-4", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 130, "team nr-5", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 150, "team nr-6", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 170, "team nr-7", Font_11x18, BLACK, MYFON);
		ILI9341_WriteString(7, 190, "team nr-8", Font_11x18, BLACK, MYFON);

		ILI9341_WriteString(135, 50, "0", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(135, 70, "0", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(135, 90, "0", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(135, 110, "0", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(135, 130, "0", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(135, 150, "0", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(135, 170, "0", Font_11x18, BLACK, MYFON);
		ILI9341_WriteString(135, 190, "0", Font_11x18, BLACK, MYFON);
	}else if(amount==7){
		ILI9341_WriteString(7, 50, "team nr-1", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 70, "team nr-2", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 90, "team nr-3", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 110, "team nr-4", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 130, "team nr-5", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 150, "team nr-6", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 170, "team nr-7", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 190, "team nr-8", Font_11x18, BLACK, MYFON);

		ILI9341_WriteString(135, 50, "0", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(135, 70, "0", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(135, 90, "0", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(135, 110, "0", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(135, 130, "0", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(135, 150, "0", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(135, 170, "0", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(135, 190, "0", Font_11x18, BLACK, MYFON);
	}else if(amount==8){
		ILI9341_WriteString(7, 50, "team nr-1", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 70, "team nr-2", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 90, "team nr-3", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 110, "team nr-4", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 130, "team nr-5", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 150, "team nr-6", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 170, "team nr-7", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 190, "team nr-8", Font_11x18, WHITE, MYFON);

		ILI9341_WriteString(135, 50, "0", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(135, 70, "0", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(135, 90, "0", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(135, 110, "0", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(135, 130, "0", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(135, 150, "0", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(135, 170, "0", Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(135, 190, "0", Font_11x18, WHITE, MYFON);
	}

	//===========Блок кнопки "закрыть"=================================
	ILI9341_Draw_Filled_Rectangle_Coord(299, 1, 319, 21, BLACK);
	ILI9341_Random_line(302, 4, 315, 16, WHITE);
	ILI9341_Random_line(302, 16, 315, 4, WHITE);
	//HAL_Delay(10000);
}
//================== эрудит квартет ====================
void screen_Erudit_Quartet()
{
	ILI9341_Fill_Screen(YELLOW);
	ILI9341_Draw_Filled_Rectangle_Coord(10, 10, 310, 230, RED);
	ILI9341_WriteString(50, 12, "Erudit Quartet", Font_16x26, BLACK, RED);
	//The_coordinate_grid ();
	ILI9341_Draw_Filled_Rectangle_Coord(299, 1, 319, 21, BLACK);
	ILI9341_Random_line(302, 4, 315, 16, WHITE);
	ILI9341_Random_line(302, 16, 315, 4, WHITE);
	//HAL_Delay(10000);
}
//============= экран настроек =====================
void screen_setting ()
{
	ILI9341_Fill_Screen(YELLOW);																								//Основной фон
	ILI9341_Draw_Filled_Rectangle_Coord(10, 10, 310, 230, RED);									//внутренний фон
	ILI9341_WriteString(20, 20, "Brain Ring", Font_11x18, BLACK, RED);					//надпись Brain Ring
	ILI9341_WriteString(200, 15, "Erudit", Font_11x18, BLACK, RED);							//надпись Erudit Quartet
	ILI9341_WriteString(200, 35, "Quartet", Font_11x18, BLACK, RED);						//надпись Erudit Quartet
	ILI9341_Draw_Vertical_Line(160, 0, 240, YELLOW);														//вертикальная линия
	ILI9341_Draw_Horizontal_Line(10, 55, 300, WHITE);														//горизонтальная линия
//===========Блок кнопки "закрыть"=================================
	ILI9341_Draw_Filled_Rectangle_Coord(299, 1, 319, 21, BLACK);								//Элнмент кнопки "закрыть"
	ILI9341_Random_line(302, 4, 315, 16, WHITE);																//Элнмент кнопки "закрыть"
	ILI9341_Random_line(302, 16, 315, 4, WHITE);																//Элнмент кнопки "закрыть"
//===========Блок настройки фальшстарт=================================
	ILI9341_WriteString(15, 60, "False Start", Font_11x18, BLUE, RED);
	//ILI9341_WriteString(40, 80, "ON", Font_11x18, DARKGREY, RED);
	//ILI9341_WriteString(100, 80, "OFF", Font_11x18, GREEN, RED);
//===========Блок настроек колличества команд=================================
	ILI9341_Draw_Horizontal_Line(10, 100, 150, WHITE);													//горизонтальная линия
	ILI9341_WriteString(15, 103, "amount of", Font_11x18, BLUE, RED);
	ILI9341_WriteString(15, 118, "commands", Font_11x18, BLUE, RED);
	ILI9341_Draw_Filled_Rectangle_Coord(156, 104, 122, 138, BLACK);
	//ILI9341_WriteString(130, 110, "0", Font_16x26, WHITE, BLACK);							//колличество участников в игре
	ILI9341_Draw_Filled_Rectangle_Coord(30, 142, 68, 175, BLACK);								//Подложка для кнопки "-"
	ILI9341_Draw_Filled_Rectangle_Coord(90, 142, 128, 175, BLACK);							//Подложка для кнопки "+"
	ILI9341_WriteString(100, 145, "+", Font_16x26, WHITE, BLACK);
	ILI9341_WriteString(40, 145, "-", Font_16x26, WHITE, BLACK);
//===========Блок кнопки "save"=================================
	ILI9341_Draw_Horizontal_Line(10, 180, 150, WHITE);													//горизонтальная линия
	ILI9341_Draw_Filled_Rectangle_Coord(30, 190, 140, 220, WHITE);
	ILI9341_Draw_Filled_Rectangle_Coord(32, 192, 138, 219, BLUE);
	ILI9341_WriteString(65, 197, "SAVE", Font_11x18, BLACK, BLUE);
	//The_coordinate_grid ();
	//HAL_Delay(10000);
}
//>>>>>>>>>>функция окрашивает в разные цвета команду которая нажала кнопку. В зависимости от ответа цвет команды меняется
void button_event_handler()
{
	//если кнопку нажала команда номер 1
	if(key_nr==0x01 && tf==0)
	{
		snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr1);
		ILI9341_WriteString(135, 50, scope_buf, Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 50, "team nr-1", Font_11x18, BLACK, MYFON);
	}
	else if(key_nr==0x01 && tf==1)
	{
		score_nr1++;
		snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr1);
		ILI9341_WriteString(135, 50, scope_buf, Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 50, "team nr-1", Font_11x18, WHITE, MYFON);
	}
	//если кнопку нажала команда номер 2
	if(key_nr==0x02 && tf==0)
	{
		snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr2);
		ILI9341_WriteString(135, 70, scope_buf, Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 70, "team nr-2", Font_11x18, BLACK, MYFON);
	}
	else if(key_nr==0x02 && tf==1)
	{
		score_nr2++;
		snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr2);
		ILI9341_WriteString(135, 70, scope_buf, Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 70, "team nr-2", Font_11x18, WHITE, MYFON);
	}
	//если кнопку нажала команда номер 3
	if(key_nr==0x03 && tf==0)
	{
		snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr3);
		ILI9341_WriteString(135, 90, scope_buf, Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 90, "team nr-3", Font_11x18, BLACK, MYFON);
	}
	else if(key_nr==0x03 && tf==1)
	{
		score_nr3++;
		snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr3);
		ILI9341_WriteString(135, 90, scope_buf, Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 90, "team nr-3", Font_11x18, WHITE, MYFON);
	}
	//если кнопку нажала команда номер 4
	if(key_nr==0x04 && tf==0)
	{
		snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr4);
		ILI9341_WriteString(135, 110, scope_buf, Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 110, "team nr-4", Font_11x18, BLACK, MYFON);
	}
	else if(key_nr==0x04 && tf==1)
	{
		score_nr4++;
		snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr4);
		ILI9341_WriteString(135, 110, scope_buf, Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 110, "team nr-4", Font_11x18, WHITE, MYFON);
	}
	//если кнопку нажала команда номер 5
	if(key_nr==0x05 && tf==0)
	{
		snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr5);
		ILI9341_WriteString(135, 130, scope_buf, Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 130, "team nr-5", Font_11x18, BLACK, MYFON);
	}
	else if(key_nr==0x05 && tf==1)
	{
		score_nr5++;
		snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr5);
		ILI9341_WriteString(135, 130, scope_buf, Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 130, "team nr-5", Font_11x18, WHITE, MYFON);
	}
	//если кнопку нажала команда номер 6
	if(key_nr==0x06 && tf==0)
	{
		snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr6);
		ILI9341_WriteString(135, 150, scope_buf, Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 150, "team nr-6", Font_11x18, BLACK, MYFON);
	}
	else if(key_nr==0x06 && tf==1)
	{
		score_nr6++;
		snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr6);
		ILI9341_WriteString(135, 150, scope_buf, Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 150, "team nr-6", Font_11x18, WHITE, MYFON);
	}
	//если кнопку нажала команда номер 7
	if(key_nr==0x07 && tf==0)
	{
		snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr7);
		ILI9341_WriteString(135, 170, scope_buf, Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 170, "team nr-7", Font_11x18, BLACK, MYFON);
	}
	else if(key_nr==0x07 && tf==1)
	{
		score_nr7++;
		snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr7);
		ILI9341_WriteString(135, 170, scope_buf, Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 170, "team nr-7", Font_11x18, WHITE, MYFON);
	}
	//если кнопку нажала команда номер 8
	if(key_nr==0x08 && tf==0)
	{
		snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr8);
		ILI9341_WriteString(135, 190, scope_buf, Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 190, "team nr-8", Font_11x18, BLACK, MYFON);
	}
	else if(key_nr==0x08 && tf==1)
	{
		score_nr8++;
		snprintf(scope_buf, sizeof(scope_buf), "%u", score_nr8);
		ILI9341_WriteString(135, 190, scope_buf, Font_11x18, WHITE, MYFON);
		ILI9341_WriteString(7, 190, "team nr-8", Font_11x18, WHITE, MYFON);
	}
}
//>>>>>>>>>>функция окрашивает все надписи комманд в цвет по умолчанию
void base_setting_color()
{
	if(key_nr==1 && tf==0){ILI9341_WriteString(7, 50, "team nr-1", Font_11x18, WHITE, MYFON);}
	if(key_nr==1 && tf==1){ILI9341_WriteString(7, 50, "team nr-1", Font_11x18, WHITE, MYFON);}
	if(key_nr==2 && tf==0){ILI9341_WriteString(7, 70, "team nr-2", Font_11x18, WHITE, MYFON);}
	if(key_nr==2 && tf==1){ILI9341_WriteString(7, 70, "team nr-2", Font_11x18, WHITE, MYFON);}
	if(key_nr==3 && tf==0){ILI9341_WriteString(7, 90, "team nr-3", Font_11x18, WHITE, MYFON);}
	if(key_nr==3 && tf==1){ILI9341_WriteString(7, 90, "team nr-3", Font_11x18, WHITE, MYFON);}
	if(key_nr==4 && tf==0){ILI9341_WriteString(7, 110, "team nr-4", Font_11x18, WHITE, MYFON);}
	if(key_nr==4 && tf==1){ILI9341_WriteString(7, 110, "team nr-4", Font_11x18, WHITE, MYFON);}
	if(key_nr==5 && tf==0){ILI9341_WriteString(7, 130, "team nr-5", Font_11x18, WHITE, MYFON);}
	if(key_nr==5 && tf==1){ILI9341_WriteString(7, 130, "team nr-5", Font_11x18, WHITE, MYFON);}
	if(key_nr==6 && tf==0){ILI9341_WriteString(7, 150, "team nr-6", Font_11x18, WHITE, MYFON);}
	if(key_nr==6 && tf==1){ILI9341_WriteString(7, 150, "team nr-6", Font_11x18, WHITE, MYFON);}
	if(key_nr==7 && tf==0){ILI9341_WriteString(7, 170, "team nr-7", Font_11x18, WHITE, MYFON);}
	if(key_nr==7 && tf==1){ILI9341_WriteString(7, 170, "team nr-7", Font_11x18, WHITE, MYFON);}
	if(key_nr==8 && tf==0){ILI9341_WriteString(7, 190, "team nr-8", Font_11x18, WHITE, MYFON);}
	if(key_nr==8 && tf==1){ILI9341_WriteString(7, 190, "team nr-8", Font_11x18, WHITE, MYFON);}
}
//>>>>>>>>>>фунция сброса фальшстарта
void reset_falsstart(void)
{
	ILI9341_WriteString(175, 52, " ", Font_11x18, RED, MYFON);
	ILI9341_WriteString(175, 72, " ", Font_11x18, RED, MYFON);
	ILI9341_WriteString(175, 92, " ", Font_11x18, RED, MYFON);
	ILI9341_WriteString(175, 112, " ", Font_11x18, RED, MYFON);
	ILI9341_WriteString(175, 132, " ", Font_11x18, RED, MYFON);
	ILI9341_WriteString(175, 152, " ", Font_11x18, RED, MYFON);
	ILI9341_WriteString(175, 172, " ", Font_11x18, RED, MYFON);
	ILI9341_WriteString(175, 192, " ", Font_11x18, RED, MYFON);
}
void sub_window_scope (void)
{

}



