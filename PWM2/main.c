#include "driverlib/pin_map.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"

void delayMS(int ms) {
    SysCtlDelay( (SysCtlClockGet()/(3*1000))*ms ) ;
}

int
main(void)
{
    //40 000 000
   SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_OSC |   SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

   //PWM saat bölücüsü şu anda 1'e bölündü
   SysCtlPWMClockSet(SYSCTL_PWMDIV_1);

   // Enable the peripherals used by this program.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);  //The Tiva Launchpad has two modules (0 and 1).

    GPIOPinConfigure(GPIO_PF1_M1PWM5);// modül 1 pwm5 DATASHEETTEN BAK pd1 pinidir.

    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1);

    PWMGenConfigure(PWM1_BASE,  PWM_GEN_2, PWM_GEN_MODE_DOWN );

    //Set the Period 1000 PWM KARE DALGASI
    // (1/40 000 000)*1000 Saniyelik peryot
    // 40MHZ/1000 hz yani 40khz frekans
    PWMGenPeriodSet(PWM1_BASE,  PWM_GEN_2, 1000);

    //Set PWM duty-50% (Period /2)
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5,500);

    // Enable the PWM generator
    PWMGenEnable(PWM1_BASE, PWM_GEN_2);

    // Turn on the Output pins
    PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT, true);

    bool fadeUp = true;
        unsigned long increment = 10;
        unsigned long pwmNow = 100;
        while(1)
        {
            delayMS(20);
            if (fadeUp) {
                pwmNow += increment;
                if (pwmNow >= 1000) { fadeUp = false; }
            }
            else {
                pwmNow -= increment;
                if (pwmNow <= 10) { fadeUp = true; }
            }
            PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5,pwmNow);
        }

}
