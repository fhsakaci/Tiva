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

int gelen;
char karakter;
uint32_t ui32ADC0Value,onlar,birler,tam;
float sicaklik;
void adcintt(void)
{
	ADCIntClear(ADC0_BASE, 3);
	while(!ADCIntStatus(ADC0_BASE,3,false));
	ADCSequenceDataGet(ADC0_BASE, 3, &ui32ADC0Value);
	sicaklik=(ui32ADC0Value*3.3/4096)*100;
	tam=sicaklik;
	onlar=tam/10;
	birler=tam%10;
	UARTCharPut(UART0_BASE,onlar+48);
	UARTCharPut(UART0_BASE,birler+48);
}

int main(void) {
	SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);


	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
				(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_1 | GPIO_PIN_4);
	GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_1 | GPIO_PIN_4, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);

	//ADC PIN AYARI
	GPIOPinTypeADC(GPIO_PORTE_BASE,GPIO_PIN_3);
	GPIOPadConfigSet(GPIO_PORTE_BASE,GPIO_PIN_3,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_STD_WPD);
	//ADC PIN AYARI

	ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_TIMER, 0);
	ADCSequenceStepConfigure(ADC0_BASE,3,0,ADC_CTL_CH0|ADC_CTL_IE|ADC_CTL_END);
	ADCSequenceEnable(ADC0_BASE, 3);

	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
	TimerControlTrigger(TIMER0_BASE, TIMER_A, true);
	TimerEnable(TIMER0_BASE, TIMER_A);

	IntMasterEnable();
	ADCIntClear(ADC0_BASE,3);
	ADCIntRegister(ADC0_BASE, 3,adcintt);
	ADCIntEnable(ADC0_BASE,3);
	IntEnable(INT_ADC0SS3);
	ADCProcessorTrigger(ADC0_BASE, 3);

	while(1)
	{
		if(UARTCharsAvail(UART0_BASE))
		{
			 karakter=UARTCharGet(UART0_BASE);
			 if(karakter=='x')
			 {
				 TimerDisable(TIMER0_BASE, TIMER_A);
			 }
			 gelen=karakter-48;
			 TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()/4*gelen);
		}

		if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_1)==0)
		{
			UARTCharPut(UART0_BASE,'a');
		}

		else if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4)==0)
		{
			UARTCharPut(UART0_BASE,'b');
		}


	}

	return 0;
}
