#include <userint.h>

//==============================================================================
//
// Title:		Osciloskop.c
// Purpose:		A short description of the implementation.
//
// Created on:	09.04.2020 at 16:38:05 by Luk·ö Granzer.
// Copyright:	Vysok· ökola b·Úsk· - Technick· univerzita Ostrava. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

#include "Osciloskop.h"
#include "GlobalVariables.h"

// -------------------- Panel functions
int CVICALLBACK panelOSC_Close (int panel, int event, void *callbackData,
								int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			QuitUserInterface(0);
			break;
	}
	return 0;
}
