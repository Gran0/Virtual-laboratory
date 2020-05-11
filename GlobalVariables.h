//==============================================================================
//
// Title:		GlobalVariables.h
// Purpose:		A short description of the interface.
//
// Created on:	09.04.2020 at 17:03:49 by Lukáš Granzer.
// Copyright:	Vysoká škola báòská - Technická univerzita Ostrava. All Rights Reserved.
//
//==============================================================================

#ifndef __GlobalVariables_H__
#define __GlobalVariables_H__
#include <stdbool.h>
#include "Osciloskop_thread.h"

#ifdef __cplusplus
    extern "C" {
#endif
		
int *panelHandleRozcestnik;
int *panelHandleOscil;
int *panelHandleVoltmetr;
int *panelHandleGenerator;

#define SAMPLING_RATE 100000.0	// Vzorkovací frekvence programu
/*
	Èasovaè ètení mìøení má periodu 100ms.
	Podle nastavené vzorkovací frekvence programu 100 kHz, tj. 10 us perioda vzorkování
	potøebujeme vygenerovat X poèet vzorkù, který odpovídá velikosti bufferu BUFFER_SIZE
	BUFFER_SIZE = 100 ms / 10 us;
	BUFFER_SIZE = 0.1 / (1/SAMPLING_RATE);
	BUFFER_SIZE = 0.1 * SAMPLING_RATE;
	BUFFER_SIZE = SAMPLING_RATE/10;
*/

#define BUFFER_SIZE (unsigned int)SAMPLING_RATE/10

double generatorSignalArray[BUFFER_SIZE];
bool   newDataFlag;
bool   readMutexFlag;


#ifdef __cplusplus
    }
#endif

#endif  /* ndef __GlobalVariables_H__ */
