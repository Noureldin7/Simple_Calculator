 /******************************************************************************
 *
 * Module: LCD
 *
 * File Name: lcd.h
 *
 * Description: Header file for the LCD driver
 *
 * Author: Noureldin Ashraf
 *
 *******************************************************************************/
#ifndef LCD_H_
#define LCD_H_

#include "../../Utils/std_types.h"
#include "../../Utils/common_macros.h"
#include "../GPIO/gpio.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define 	BIT_MODE 			4

#define 	LCD_RS_PORT 		PORTA_ID
#define 	LCD_RS_PIN 			PIN1_ID

#define 	LCD_EN_PORT 		PORTA_ID
#define 	LCD_EN_PIN 			PIN2_ID

#define 	LCD_DATA_PORT 		PORTA_ID
#define		LCD_DATA_START_PIN	PIN3_ID
/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description :
 * Initialize LCD module
 */
void LCD_init();
/*
 * Description :
 * Sends given command to LCD
 */
void LCD_sendCommand(uint8);
/*
 * Description :
 * Displays given character on LCD screen
 */
void LCD_displayCharacter(uint8);
/*
 * Description :
 * Displays given string on LCD screen
 */
void LCD_displayString(const char*);
/*
 * Description :
 * Moves LCD cursor to given row & column
 */
void LCD_moveCursor(uint8, uint8);
/*
 * Description :
 * Displays given string in given position on LCD screen
 */
void LCD_displayStringRowColumn(uint8, uint8, const char*);
/*
 * Description :
 * Clears LCD module's screen
 */
void LCD_clearScreen();
/*
 * Description :
 * Displays given number on LCD screen
 */
void LCD_integerToString(sint32);

#endif /* LCD_H_ */
