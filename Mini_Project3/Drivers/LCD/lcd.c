 /******************************************************************************
 *
 * Module: LCD
 *
 * File Name: lcd.h
 *
 * Description: Source file for the LCD driver
 *
 * Author: Noureldin Ashraf
 *
 *******************************************************************************/
#include <util/delay.h>
#include <stdlib.h>
#include "./lcd.h"
/*******************************************************************************
 *                      Functions Prototypes(Private)                          *
 *******************************************************************************/
/*
 * Description :
 * Initialize LCD module
 */
void LCD_init()
{
#if BIT_MODE==4
	for (int i = 0; i < 4; i++) {
		GPIO_setupPinDirection(LCD_DATA_PORT,LCD_DATA_START_PIN+i,PIN_OUTPUT);
	}
#else
	GPIO_setupPortDirection(LCD_DATA_PORT,PORT_OUTPUT);
#endif
	GPIO_setupPinDirection(LCD_RS_PORT,LCD_RS_PIN,PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_EN_PORT,LCD_EN_PIN,PIN_OUTPUT);
	_delay_ms(20);
#if BIT_MODE==4
	LCD_sendCommand(0x02);
	LCD_sendCommand(0x28);
#else
	LCD_sendCommand(0x38);
#endif
	LCD_sendCommand(0x0c);
	LCD_sendCommand(0x01);
}
/*
 * Description :
 * Sends given command to LCD
 */
void LCD_sendCommand(uint8 command)
{
#if BIT_MODE==4
	for(int i = 1; i>=0; i--)
	{
		GPIO_writePin(LCD_RS_PORT,LCD_RS_PIN,0);
		_delay_ms(1);
		GPIO_writePin(LCD_EN_PORT,LCD_EN_PIN,1);
		_delay_ms(1);
		for(int j = 0; j<4; j++)
		{
			GPIO_writePin(LCD_DATA_PORT,LCD_DATA_START_PIN+j,GET_BIT(command,(4*i+j)));
		}
		_delay_ms(1);
		GPIO_writePin(LCD_EN_PORT,LCD_EN_PIN,0);
		_delay_ms(1);
	}
#else
	GPIO_writePin(LCD_RS_PORT,LCD_RS_PIN,0);
	_delay_ms(1);
	GPIO_writePin(LCD_EN_PORT,LCD_EN_PIN,1);
	_delay_ms(1);
	GPIO_writePort(LCD_DATA_PORT,command);
	_delay_ms(1);
	GPIO_writePin(LCD_EN_PORT,LCD_EN_PIN,0);
	_delay_ms(1);
#endif
}
/*
 * Description :
 * Displays given character on LCD screen
 */
void LCD_displayCharacter(uint8 character)
{
#if BIT_MODE==4
	for(int i = 1; i>=0; i--)
	{
		GPIO_writePin(LCD_RS_PORT,LCD_RS_PIN,1);
		_delay_ms(1);
		GPIO_writePin(LCD_EN_PORT,LCD_EN_PIN,1);
		_delay_ms(1);
		for(int j = 0; j<4; j++)
		{
			GPIO_writePin(LCD_DATA_PORT,LCD_DATA_START_PIN+j,GET_BIT(character,(4*i+j)));
		}
		_delay_ms(1);
		GPIO_writePin(LCD_EN_PORT,LCD_EN_PIN,0);
		_delay_ms(1);
	}
#else
	GPIO_writePin(LCD_RS_PORT,LCD_RS_PIN,1);
	_delay_ms(1);
	GPIO_writePin(LCD_EN_PORT,LCD_EN_PIN,1);
	_delay_ms(1);
	GPIO_writePort(LCD_DATA_PORT,character);
	_delay_ms(1);
	GPIO_writePin(LCD_EN_PORT,LCD_EN_PIN,0);
	_delay_ms(1);
#endif
}
/*
 * Description :
 * Displays given string on LCD screen
 */
void LCD_displayString(const char* str)
{
	while(*str!='\0')
	{
		LCD_displayCharacter(*str++);
	}
}
/*
 * Description :
 * Moves LCD cursor to given row & column
 */
void LCD_moveCursor(uint8 row, uint8 col)
{
	uint8 lcd_memory_address;

	/* Calculate the required address in the LCD DDRAM */
	switch(row)
	{
		case 0:
			lcd_memory_address=col;
				break;
		case 1:
			lcd_memory_address=col+0x40;
				break;
		case 2:
			lcd_memory_address=col+0x10;
				break;
		case 3:
			lcd_memory_address=col+0x50;
				break;
	}
	/* Move the LCD cursor to this specific address */
	LCD_sendCommand(lcd_memory_address | 0x80);
}
/*
 * Description :
 * Displays given string in given position on LCD screen
 */
void LCD_displayStringRowColumn(uint8 row, uint8 col, const char* str)
{
	LCD_moveCursor(row,col);
	LCD_displayString(str);
}
/*
 * Description :
 * Clears LCD module's screen
 */
void LCD_clearScreen()
{
	LCD_sendCommand(0x01);
}
/*
 * Description :
 * Displays given number on LCD screen
 */
void LCD_integerToString(sint32 num)
{
	char buffer[11];
	LCD_displayString(itoa(num,buffer,10));
}
