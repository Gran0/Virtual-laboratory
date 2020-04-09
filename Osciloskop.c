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

// ------------------ Menu functions 
void CVICALLBACK MENU_OSC_Rozcestnik_OnClick (int menuBar, int menuItem, void *callbackData,
		int panel)
{
	DisplayPanel(*panelHandleRozcestnik);
	HidePanel(*panelHandleOscil);
}

void CVICALLBACK MENU_OSC_Voltmetr_OnClick (int menuBar, int menuItem, void *callbackData,
		int panel)
{
}

void CVICALLBACK MENU_OSC_Generator_OnClick (int menuBar, int menuItem, void *callbackData,
		int panel)
{
}


// -------------------- Panel functions
int CVICALLBACK TEST_SWITCH_Toogle (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
	int rocketSwitch;
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (*panelHandleOscil, PANEL_OSC_TEST_SWITCH, &rocketSwitch);
			SetCtrlVal (*panelHandleOscil, PANEL_OSC_LED, rocketSwitch);
			
			break;
		case EVENT_TIMER_TICK:

			break;
	}
	return 0;
}