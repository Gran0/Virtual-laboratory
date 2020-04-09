#include "Osciloskop.h"

//==============================================================================
//
// Title:		GRA0084
// Purpose:		A short description of the application.
//
// Created on:	09.04.2020 at 14:45:13 by Luk·ö Granzer.
// Copyright:	Vysok· ökola b·Úsk· - Technick· univerzita Ostrava. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include "GRA0084.h"
#include "Osciloskop.h"
#include "toolbox.h"
#include "GlobalVariables.h"

//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// Static global variables
int pHRozcestnik = 0;
int pHOscil = 1;
int pHVolt = 2;
int pHGen = 3;

*panelHandleRozcestnik = &pHRozcestnik;
*panelHandleOscil = &pHOscil;
*panelHandleVoltmetr = &pHVolt;
*panelHandleGenerator = &pHGen;
//==============================================================================
// Static functions

//==============================================================================
// Global variables

//==============================================================================
// Global functions

/// HIFN The main entry-point function.
int main (int argc, char *argv[])
{
	/* initialize and load resources */
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;
	if((*panelHandleRozcestnik = LoadPanel (0, "GRA0084.uir", PANEL)) < 0)
		return -1;
	if((*panelHandleOscil = LoadPanel (0, "Osciloskop.uir", PANEL))<0)
		return -2;
	
	/* display the panel and run the user interface */
	DisplayPanel (*panelHandleRozcestnik);
	RunUserInterface ();
	DiscardPanel (*panelHandleRozcestnik);
	return 0;
}

//==============================================================================
// UI callback function prototypes

/// HIFN Exit when the user dismisses the panel.

void CVICALLBACK ITEM_Osciloskop_OnClick (int menuBar, int menuItem, void *callbackData,
		int panel)
{
	DisplayPanel(*panelHandleOscil);
	HidePanel(*panelHandleRozcestnik);
}

void CVICALLBACK ITEM_Voltmetr_OnClick (int menuBar, int menuItem, void *callbackData,
										int panel)
{
}

void CVICALLBACK ITEM_Generator_OnClick (int menuBar, int menuItem, void *callbackData,
		int panel)
{
}

void CVICALLBACK ITEM_Autor_OnClick (int menuBar, int menuItem, void *callbackData,
									 int panel)
{
}

int CVICALLBACK panelRozcestnik (int panel, int event, void *callbackData,
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
