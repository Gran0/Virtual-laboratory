//==============================================================================
//
// Title:		GlobalVariables.h
// Purpose:		A short description of the interface.
//
// Created on:	09.04.2020 at 17:03:49 by Luk·ö Granzer.
// Copyright:	Vysok· ökola b·Úsk· - Technick· univerzita Ostrava. All Rights Reserved.
//
//==============================================================================

#ifndef __GlobalVariables_H__
#define __GlobalVariables_H__

#ifdef __cplusplus
    extern "C" {
#endif

#define SAMPLING_RATE 100000.0	// Sampling frequence of program
		
int *panelHandleRozcestnik;
int *panelHandleOscil;
int *panelHandleVoltmetr;
int *panelHandleGenerator;

#define BUFFER_SIZE 40000
double generatorSignalArray[BUFFER_SIZE];
double someStuff;






#ifdef __cplusplus
    }
#endif

#endif  /* ndef __GlobalVariables_H__ */
