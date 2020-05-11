//==============================================================================
//
// Title:		Osciloskop_thread.h
// Purpose:		A short description of the interface.
//
// Created on:	08.05.2020 at 23:05:15 by Luk·ö Granzer.
// Copyright:	Vysok· ökola b·Úsk· - Technick· univerzita Ostrava. All Rights Reserved.
//
//==============================================================================

#ifndef __Osciloskop_thread_H__
#define __Osciloskop_thread_H__

#ifdef __cplusplus
    extern "C" {
#endif

#include "cvidef.h"

int CVICALLBACK BackgroundThread(int *data);
bool backgroundThreadRunning;


#ifdef __cplusplus
    }
#endif

#endif  /* ndef __Osciloskop_thread_H__ */
