#include "Generator.h"
#include <userint.h>

//==============================================================================
//
// Title:		Voltmetr.c
// Purpose:		A short description of the implementation.
//
// Created on:	09.04.2020 at 17:31:08 by Luk·ö Granzer.
// Copyright:	Vysok· ökola b·Úsk· - Technick· univerzita Ostrava. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

#include "Voltmetr.h"
#include "GlobalVariables.h"



int CVICALLBACK RINGSLIDE_OnClick (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			break;
	}
	return 0;
}

int CVICALLBACK panelVoltmeter_Close (int panel, int event, void *callbackData,
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
