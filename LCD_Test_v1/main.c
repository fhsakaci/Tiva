#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "Lcd.h"


int main(void)
{
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	SysCtlPeripheralEnable(LCDPORTENABLE);
	GPIOPinTypeGPIOOutput(LCDPORT, E | RS | D4 | D5 | D6 | D7);
	Lcd_init();
	Lcd_Goto(1,2);
	Lcd_Putch('F');
	Lcd_Putch('U');
	Lcd_Putch('R');
	Lcd_Putch('K');
	Lcd_Putch('A');
	Lcd_Putch('N');
	Lcd_Putch(' ');
	Lcd_Putch('H');
	Lcd_Putch('A');
	Lcd_Putch('S');
	Lcd_Putch('A');
	Lcd_Putch('N');

	Lcd_Goto(2,4);
	Lcd_Putch('S');
	Lcd_Putch('A');
	Lcd_Putch('K');
	Lcd_Putch('A');
	Lcd_Putch('C');
	Lcd_Putch('I');
	 	while(1)
	{
	}
}

