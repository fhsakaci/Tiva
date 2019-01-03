#include <stdint.h>
#include <stdbool.h>
#include "driverlib/sysctl.h"
#include "Saat.h"
#include "Lcd.h"

int main(void)
{
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	Lcd_init();
	SaatAyarla(1,30,50);
	SaatKoordinat(1,1);
	YazmaAktif(true);
	SaatBasla();

	while(1)
	{
	}
}

