

#include <util/delay.h>
#include "./Drivers/Keypad/keypad.h"
#include "./Drivers/LCD/lcd.h"
#include "./Utils/std_types.h"
// States for Calculator's FSM
typedef enum state {
	// Building Operand
	operand_state,
	// Capturing Sign
	sign_state,
	// Received '=' and currently displaying result
	equal_state
}state;
// Stores Result of operations so far
sint32 result;
// Holds sign of curr_operand
uint8 curr_sign;
// Holds value of current operand
sint32 curr_operand;
// Holds current operation
uint8 curr_operation;
// Holds current state
state current_state;
// Resets Application Variables and clears screen
void app_reset()
{
	LCD_clearScreen();
	result = 0;
	curr_sign = 0;
	curr_operand = 0;
	curr_operation = '+';
	current_state = sign_state;
}
// Displays a message and resets application (Used for errors)
void display_message(const char * msg)
{
	LCD_clearScreen();
	LCD_displayString(msg);
	_delay_ms(750);
	LCD_clearScreen();
	app_reset();
}
// Updates result, clears screen and displays the new result
uint8 update_result(sint32 operand,uint8 operation)
{
	switch(operation){
	case '+':
		result+=operand;
		break;
	case '-':
		result-=operand;
		break;
	case '*':
		result*=operand;
		break;
	case '%':
		if(operand==0)
		{
			display_message("Dividing By Zero");
			return 0;
		}
		result/=operand;
		break;
	}
	LCD_clearScreen();
	LCD_integerToString(result);
	return 1;
}
// Converts numbers to their ASCII values
uint8 get_ascii(uint8 key)
{
	if(key<=9 && key>=0)
	{
		return key + 48;
	}
	return key;
}
int main(void)
{
	LCD_init(); /* Initialize the LCD */
	app_reset();
	while(1)
	{
		// Get Key Press
		uint8 key = KEYPAD_getPressedKey();
		// Clear Button Handler
		if(key==13)
		{
			app_reset();
			continue;
		}
		// State Machine
		switch(current_state){
		case operand_state:
			if(key<10)
			{
				// Build Operand
				curr_operand = curr_operand*10 + key*curr_sign;
			}
			else
			{
				// On failure reset and continue
				if(!update_result(curr_operand,curr_operation))
				{
					app_reset();
					continue;
				}
				curr_operation = key;
				curr_operand = 0;
				curr_sign = 0;
				current_state = key=='='?equal_state:sign_state;
			}
			break;
		case sign_state:
			if(key<10)
			{
				curr_operand = curr_sign ? -key : key;
				curr_sign = 1;
				current_state = operand_state;
			}
			else if(key=='-'&&!curr_sign)
			{
				curr_sign = -1;
			}
			else
			{
				display_message("Error");
				continue;
			}
			break;
		case equal_state:
			// Reset and start new calculations
			if(key<10)
			{
				app_reset();
				curr_operand = key;
				current_state = operand_state;
			}
			// Continue Calculations
			else
			{
				curr_operation = key;
				curr_operand = 0;
				curr_sign = 0;
				current_state = key=='='?equal_state:sign_state;
			}
			break;
		}
		// Display Pressed Keys
		if(key!='=')
		{
			LCD_displayCharacter(get_ascii(key));
		}
	}
}
