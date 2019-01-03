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

uint32_t ui32ADC0Value;
uint32_t ui32ADC1Value;
uint32_t ref;
uint32_t gelen;
void adcintt(void)
{
	ADCIntClear(ADC1_BASE, 3);
	ADCSequenceDataGet(ADC0_BASE, 3, ui32ADC1Value);
	gelen = ui32ADC1Value;
	if(ref>=gelen)
	{
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3 | GPIO_PIN_4, 24);
	}
	else
	{
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3 | GPIO_PIN_4, 0);
	}
	GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, 4);
}
int main(void)
{
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC0_BASE,3,0,ADC_CTL_TS|ADC_CTL_IE|ADC_CTL_END);
	ADCSequenceEnable(ADC0_BASE, 3);
	ADCSequenceConfigure(ADC1_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC1_BASE,3,0,ADC_CTL_TS|ADC_CTL_IE|ADC_CTL_END);
	ADCSequenceEnable(ADC1_BASE, 3);
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4);
	GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_1);
	GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_1, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);
	IntMasterEnable();
	ADCIntRegister(ADC1_BASE, 3,adcintt);
	ADCIntEnable(ADC1_BASE,3);
	IntEnable(INT_ADC1SS3);
	GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, 4);
	while(1)
	{

	   ADCProcessorTrigger(ADC0_BASE, 3);
	   while(!ADCIntStatus(ADC0_BASE, 3, false));
	   ADCSequenceDataGet(ADC0_BASE, 3, ui32ADC0Value);
	   ref = ui32ADC0Value;
	   ADCIntClear(ADC0_BASE, 3);

	   while(!GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_1));
	   GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, 0);

	   ADCProcessorTrigger(ADC1_BASE, 3);
	   //while(GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_1));

	}
}
