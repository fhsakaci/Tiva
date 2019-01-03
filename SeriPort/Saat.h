/*
 * Saat.h
 *
 *  Created on: Nov 29, 2017
 *      Author: SAKACI
 */

#ifndef SAAT_H_
#define SAAT_H_
#include "Lcd.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
int saniye, dakika, saat;
int satir, sutun;
bool yazmadurumu;
void YazmaAktif(bool);
void SaatAyarla(int,int,int);
void SaatYaz (int,int,int);
void SaatKoordinat(int,int);
void Timer1IntHandler(void);
void SaatBasla(void);

#endif /* SAAT_H_ */
