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
#include "Lcd.h"
#include "Saat.h"


uint32_t ui32ADC0Value[4];
uint32_t ui32TempAvg;
float ui32TempValueC;
uint32_t ui32TempValueF;
float voltage;
char ADCdeger[3];
uint32_t Temp;

void ADC_yaz(void)
{

	Lcd_Goto(2,5);
	Lcd_Putch(ADCdeger[0]);
	Lcd_Putch(ADCdeger[1]);
	Lcd_Putch(',');
	Lcd_Putch(ADCdeger[2]);
	//Timer0IntHandler();
}




void adcintt(void)
{
	ADCIntClear(ADC0_BASE, 1);
	ADCSequenceDataGet(ADC0_BASE, 1, ui32ADC0Value);
	ui32TempAvg = (ui32ADC0Value[0] + ui32ADC0Value[1] + ui32ADC0Value[2] + ui32ADC0Value[3])/4;
	voltage=ui32TempAvg*3.3/4096;
	ui32TempValueC = voltage*100;
	if(ui32TempValueC < 100)
	{
		Temp=ui32TempValueC/10;
		ADCdeger[0]=Temp+48;

		Temp=ui32TempValueC;
		Temp=Temp%10;
		ADCdeger[1]=Temp+48;

		Temp=ui32TempValueC*10;
		Temp=Temp%10;
		ADCdeger[2]=Temp+48;
	}
	ADC_yaz();
}




int main(void)
{
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

	GPIOPinTypeADC(GPIO_PORTE_BASE,GPIO_PIN_3);
	GPIOPadConfigSet(GPIO_PORTE_BASE,GPIO_PIN_3,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_STD_WPD);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);

    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()-1);

	ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_TIMER, 0);
	ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH0);
	ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_CH0);
	ADCSequenceStepConfigure(ADC0_BASE, 1, 2, ADC_CTL_CH0);
	ADCSequenceStepConfigure(ADC0_BASE,1,3,ADC_CTL_CH0|ADC_CTL_IE|ADC_CTL_END);
	ADCSequenceEnable(ADC0_BASE, 1);

	TimerControlTrigger(TIMER0_BASE, TIMER_A, true);
	TimerEnable(TIMER0_BASE, TIMER_A);

	IntMasterEnable();
	ADCIntClear(ADC0_BASE,1);
	ADCIntRegister(ADC0_BASE, 1,adcintt);
	ADCIntEnable(ADC0_BASE,1);
	IntEnable(INT_ADC0SS1);
	ADCProcessorTrigger(ADC0_BASE, 1);

	Lcd_init();
	SaatAyarla(22,00,01);
	SaatKoordinat(1,5);
	YazmaAktif(true);
	SaatBasla();

	while(1)
	{

	}
}



