#include "Voltmetr.h"
#include <userint.h>

//==============================================================================
//
// Title:		Generator.c
// Purpose:		A short description of the implementation.
//
// Created on:	09.04.2020 at 17:31:49 by Luk� Granzer.
// Copyright:	Vysok� �kola b��sk� - Technick� univerzita Ostrava. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

#include "Generator.h"
#include "GlobalVariables.h"
//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// Static global variables

//==============================================================================
// Static functions

//==============================================================================
// Global variables

//==============================================================================
// Global functions

int CVICALLBACK Gauge_Event (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			break;
	}
	return 0;
}