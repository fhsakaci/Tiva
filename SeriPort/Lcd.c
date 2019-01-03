/*
 * Lcd.c
 *
 *  Created on: Nov 5, 2017
 *      Author: SAKACI
 */
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "Lcd.h"

void Lcd_init(void)
{
	SysCtlPeripheralEnable(LCDPORTENABLE);
	GPIOPinTypeGPIOOutput(LCDPORT, E | RS | D4 | D5 | D6 | D7);
	GPIOPinWrite(LCDPORT, RS, 0x00);
	Lcd_YariKomut(0x30);
	Lcd_YariKomut(0x30);
	Lcd_YariKomut(0x30);
	Lcd_YariKomut(0x20);
	Lcd_Komut(0x2C);
	Lcd_Komut(0x0F);
	Lcd_Komut(0x06);
	Lcd_Temizle();
}

void Lcd_Komut(unsigned char c)
{
    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (c & 0xf0) );
    GPIOPinWrite(LCDPORT, RS, 0x00);
    GPIOPinWrite(LCDPORT, E, 0x02);
    SysCtlDelay(50000);
    GPIOPinWrite(LCDPORT, E, 0x00);

    SysCtlDelay(50000);

    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (c & 0x0f) << 4 );
    GPIOPinWrite(LCDPORT, RS, 0x00);
    GPIOPinWrite(LCDPORT, E, 0x02);
    SysCtlDelay(50000);
    GPIOPinWrite(LCDPORT, E, 0x00);

    SysCtlDelay(50000);
}

void Lcd_Putch(unsigned char c)
{

	    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (c & 0xf0));
	    GPIOPinWrite(LCDPORT, RS, 0x01);
	    GPIOPinWrite(LCDPORT, E, 0x02);
	    SysCtlDelay(50000);
	    GPIOPinWrite(LCDPORT, E, 0x00);

	    SysCtlDelay(50000);

	    GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7, (c & 0x0f) << 4 );
	    GPIOPinWrite(LCDPORT, RS, 0x01);
	    GPIOPinWrite(LCDPORT, E, 0x02);
	    SysCtlDelay(50000);
	    GPIOPinWrite(LCDPORT, E, 0x00);

	    SysCtlDelay(50000);

}

void Lcd_Temizle(void)
{
	Lcd_Komut(0x01);
	SysCtlDelay(10);
}

void Lcd_YariKomut(unsigned char c)
{
	GPIOPinWrite(LCDPORT, D4 | D5 | D6 | D7,  c );
	GPIOPinWrite(LCDPORT, E, 0x02);
	SysCtlDelay(10);
	GPIOPinWrite(LCDPORT, E, 0x00);
	SysCtlDelay(50000);
}

void Lcd_Goto(int x, int y)
{
		y=y-1;
		if(x>2 | y>16 | x<0 | y<0)
		{
			////Hata Fırlatıp Programı Çökertmeli
		}

        if(x==1)
                Lcd_Komut(0x80+y);
        else
                Lcd_Komut(0xC0+y);
}
