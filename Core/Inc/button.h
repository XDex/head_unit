/*
 * button.h
 *
 *  Created on: 3 мая 2026 г.
 *      Author: dima
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#define BUTTON_PIN_LEFT left_key_Pin
#define BUTTON_PIN_CENTR centr_key_Pin
#define BUTTON_PIN_RIGHT right_key_Pin
#define BUTTON_PORT GPIOB


uint8_t Button_Read_left(void);
uint8_t Button_Read_centr(void);
uint8_t Button_Read_right(void);
void Button_transmiter(void);
void definition_of_coordinates(void);


#endif /* INC_BUTTON_H_ */
