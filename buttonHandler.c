
#include <stdio.h>
#include <string.h>

#include "display.h"
#include "keyboard.h"
#include "led.h"
#include "buttonHandler.h"

void checkButton( void )
{
	switch(KbGetKey())
	{
		case KEY_UP: break;
		case KEY_DOWN: break;
		case KEY_LEFT: break;
		case KEY_RIGHT: break;
		default: break;
	}
}


void pollButton( void )
{
		if(KbGetKey() != KEY_UNDEFINED)
		{
			if(x != 0)
			{
				x = 0;
				LedControl(LED_ON);
				LcdBackLight(LCD_BACKLIGHT_ON);
			}
			
			checkButton();			
		}
		else
		{
			if(x < 100)
			{
				LedControl(LED_OFF);
				x++;
			}
			else{
				LcdBackLight(LCD_BACKLIGHT_OFF);
			}
		}
}

