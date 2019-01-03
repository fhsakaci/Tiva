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
int saniyeb,saniyeo,saniye;

void timerkesme(void)
{
	saniye+=saniye;
	UARTCharPut(UART0_BASE,saniye/10+48);
	UARTCharPut(UART0_BASE,saniye%10+48);
	GPIOPinWrite(GPIO_PORTF_BASE,255,saniye);
}


int main(void) {
	SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,255);

	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE,GPIO_PIN_0 | GPIO_PIN_1);
	UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
	(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

	IntMasterEnable();
	TimerConfigure(TIMER0_BASE,TIMER_CFG_PERIODIC);
	TimerIntRegister(TIMER0_BASE,TIMER_A,timerkesme);
	TimerIntEnable(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
	TimerIntClear(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
	TimerLoadSet(TIMER0_BASE,TIMER_A,SysCtlClockGet());
	IntEnable(INT_TIMER0A);
	TimerEnable(TIMER0_BASE,TIMER_A);

	while(1)
	{
		UARTCharPut(UART0_BASE,'a');
		while(!UARTCharsAvail(UART0_BASE));
		saniyeb=UARTCharGet(UART0_BASE)-48;
		while(!UARTCharsAvail(UART0_BASE));
		saniyeo=UARTCharGet(UART0_BASE)-48;
		saniye=saniyeb+saniyeo*10;
		while(1);
	}
	return 0;
}
