#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "driverlib/interrupt.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "inc/hw_gpio.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
int x=0;
char gelen;
int gelenref;
float ref,boyut;
int deger;
void serikesme(void)
{
    uint32_t ui32Status;

    ui32Status = UARTIntStatus(UART0_BASE, true); //seri port kesme durumu

    UARTIntClear(UART0_BASE, ui32Status); //kesmeyi temizle

    while(UARTCharsAvail(UART0_BASE)) //veriler varsa donguye gir.
    {
        gelen=UARTCharGet(UART0_BASE);
        if(x==0)
        {
        	gelenref=gelen-48;
        	ref=gelenref*0.1;
        	x=1;
        }
        else
        {
        	if(gelen=='S')
        	{
        		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2,4);
        	}
        	else if(gelen=='D')
        	{
        		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2,0);
        		x=false;
        	}
        }


    }
}
int main(void)
{
	SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);


	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);

	UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
			(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	UARTIntRegister(UART0_BASE,serikesme);
	IntMasterEnable(); // işlemci kesmelerini aktif et.
	IntEnable(INT_UART0); //seri kesmeyi aktif et.
	UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT); //Hem RX ve Hem de TX olunca interrup et.


	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);
	TimerLoadSet (TIMER0_BASE,TIMER_A,0);
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC_UP);
	 while (1)
	 {
		while(x==0);



		if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)==0)
		{
			TimerEnable(TIMER0_BASE,TIMER_A);
			while(!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4));
			deger=TimerValueGet(TIMER0_BASE,TIMER_A);
			boyut=deger/SysCtlClockGet();
			TimerDisable(TIMER0_BASE,TIMER_A);
			TimerLoadSet (TIMER0_BASE,TIMER_A,0);
			if(boyut>ref)
			{
				UARTCharPut(UART0_BASE,'#');
				UARTCharPut(UART0_BASE,'0');
				UARTCharPut(UART0_BASE,'2');
			}
			else
			{
				UARTCharPut(UART0_BASE,'#');
				UARTCharPut(UART0_BASE,'1');
				UARTCharPut(UART0_BASE,'2');
				UARTCharPut(UART0_BASE,'3');
			}
		}
	 }
	return 0;
}
