/*
 * Saat.c
 *
 *  Created on: Nov 29, 2017
 *      Author: SAKACI
 */

#include "Saat.h"
void SaatAyarla(int sa,int dk,int sn)
{
saat=sa;
dakika=dk;
saniye=sn;
}

void SaatYaz(int sn,int dk,int sa)
{
	int san_birler,san_onlar;
	int d_birler,d_onlar;
	int s_birler,s_onlar;
	san_onlar=sn/10;
	san_birler=sn%10;
	d_onlar=dk/10;
	d_birler=dk%10;
	s_onlar=sa/10;
	s_birler=sa%10;
	Lcd_Goto(satir,sutun);
	Lcd_Putch(s_onlar+48);
	Lcd_Putch(s_birler+48);
	Lcd_Putch(':');
	Lcd_Putch(d_onlar+48);
	Lcd_Putch(d_birler+48);
	Lcd_Putch(':');
	Lcd_Putch(san_onlar+48);
	Lcd_Putch(san_birler+48);
}

void Timer1IntHandler(void)
{
	TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);// timer kesmesini temizle.
	saniye++;
		if(saniye==60)
		{
			dakika++;
			if(dakika==60)
			{
				saat++;
				if(saat==24)
				{
					saat=0;
				}
				dakika=0;
			}
			saniye=0;
		}
		if(yazmadurumu==true)	SaatYaz(saniye,dakika,saat);
}

void SaatBasla(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1); // cevre birimi aktif
	TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC); // aşağıda doğru sayıcı
	TimerLoadSet(TIMER1_BASE, TIMER_A, SysCtlClockGet()-1); // 1sn için değer
	TimerIntRegister(TIMER1_BASE, TIMER_A,Timer1IntHandler); // kesme için hangi fonksiyonun calısacağı ve timera için kesme oluşacak
	IntEnable(INT_TIMER1A);// Timer0A için kesme aktif
	TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);// zaman bitince kesme oluacak.
	IntMasterEnable(); // global kesme aktif
	TimerEnable(TIMER1_BASE, TIMER_A); // timera saymaya başlasın.
}

void SaatKoordinat(m,n)
{
satir=m;
sutun=n;
}
void YazmaAktif(bool gelen)
{
	yazmadurumu=gelen;

}
