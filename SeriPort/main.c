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
#include "LCD.h"
#include "Saat.h"

uint32_t ui32ADC0Value[4];
uint32_t ui32TempAvg;
float ui32TempValueC;
uint32_t ui32TempValueF;
float voltage;
char ADCdeger[3];
uint32_t Temp;
bool baglanti=false;
bool saatayaraktif=false;
bool verigelisi=false;
void ADC_yaz(void)
{

	Lcd_Goto(2,12);
	Lcd_Putch(ADCdeger[0]);
	Lcd_Putch(ADCdeger[1]);
	Lcd_Putch(',');
	Lcd_Putch(ADCdeger[2]);
	if(baglanti==true)
	{
		UARTCharPut(UART0_BASE,ADCdeger[0]);
		UARTCharPut(UART0_BASE,ADCdeger[1]);
		UARTCharPut(UART0_BASE,',');
		UARTCharPut(UART0_BASE,ADCdeger[2]);
	}
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

	//UARTCharPut(UART0_BASE,'a');
}










int main(void)
{
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE,GPIO_PIN_0 | GPIO_PIN_1);
	//UARTEnable(UART0_BASE);
	UARTConfigSetExpClk(UART0_BASE,SysCtlClockGet(),115200,UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);

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
	Lcd_Temizle();
	SaatAyarla(22,00,01);
	SaatKoordinat(2,1);
	YazmaAktif(true);
	SaatBasla();

	char gsaat[6];
	char setsaat[2];
	int ggsaat,ggdakika,ggsaniye;
	char gelen;
	int i=0;
	while(1)
	{
		if(UARTCharsAvail(UART0_BASE))
		{
			gelen=UARTCharGet(UART0_BASE);

			if(saatayaraktif==true)
						{
							gsaat[i]=gelen;
							i++;
							if(i==6)
							{

								setsaat[0]=gsaat[0];
								setsaat[1]=gsaat[1];
								sscanf(setsaat, "%d", &ggsaat);
								setsaat[0]=gsaat[2];
								setsaat[1]=gsaat[3];
								sscanf(setsaat, "%d", &ggdakika);
								setsaat[0]=gsaat[4];
								setsaat[1]=gsaat[5];
								sscanf(setsaat, "%d", &ggsaniye);
								SaatAyarla(ggsaat,ggdakika,ggsaniye);
								saatayaraktif=false;
								i=0;
							}
						}
			if(verigelisi==true)
			{
				if(gelen!='+')
				{
					Lcd_Putch(gelen);
				}
				else
				{
					verigelisi=false;
				}
			}


			if(gelen=='\r')
			{
				Lcd_Goto(2,1);
			}
			else if(gelen=='%')
			{
				baglanti=true;		////Baglanti Acildi
			}
			else if(gelen=='-')
			{
				saatayaraktif=true;
			}
			else if(gelen=='/')
			{
				verigelisi=true;
				Lcd_Temizle();
				Lcd_Goto(1,1);
			}




		}
	}
	return 0;
}
