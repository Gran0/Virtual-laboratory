#include "Osciloskop.h"

//==============================================================================
//
// Title:		GRA0084
// Purpose:		A short description of the application.
//
// Created on:	09.04.2020 at 14:45:13 by Lukáš Granzer.
// Copyright:	Vysoká škola báòská - Technická univerzita Ostrava. All Rights Reserved.
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
// Definice panel handlerù
int pHRozcestnik = 0;
int pHOscil = 1;
int pHVolt = 2;
int pHGen = 3;

// Pøiøazení adres do globálních ukazatelù
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
	if((*panelHandleVoltmetr = LoadPanel (0, "Voltmetr.uir", PANEL))<0)
		return -2;
	if((*panelHandleGenerator = LoadPanel (0, "Generator.uir", PANEL))<0)
		return -2;
	
	int funID;
	CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE, BackgroundThread, NULL, &funID);
	/* display the panel and run the user interface */
	DisplayPanel (*panelHandleRozcestnik);
	RunUserInterface ();
	
	// Kill thread
	backgroundThreadRunning = false;
	CmtWaitForThreadPoolFunctionCompletion (DEFAULT_THREAD_POOL_HANDLE, funID, 0);	// Poèkej až chcípne
	DiscardPanel (*panelHandleRozcestnik);
	return 0;
}

int CVICALLBACK panelRozcestnik_Close (int panel, int event, void *callbackData,
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

///////////  Shared functions for windows switching. Don't change names!!!
void CVICALLBACK ShowFrameOsciloskop (int menuBar, int menuItem, void *callbackData,
									  int panel)
{
	DisplayPanel(*panelHandleOscil);
	HidePanel(*panelHandleGenerator);
	HidePanel(*panelHandleVoltmetr);
	HidePanel(*panelHandleRozcestnik);
}

void CVICALLBACK ShowFrameVoltmeter (int menuBar, int menuItem, void *callbackData,
									 int panel)
{
	DisplayPanel(*panelHandleVoltmetr);
	HidePanel(*panelHandleGenerator);
	HidePanel(*panelHandleOscil);
	HidePanel(*panelHandleRozcestnik);
}

void CVICALLBACK ShowFrameGenerator (int menuBar, int menuItem, void *callbackData,
									 int panel)
{
	DisplayPanel(*panelHandleGenerator);
	HidePanel(*panelHandleVoltmetr);	
	//HidePanel(*panelHandleOscil);
	HidePanel(*panelHandleRozcestnik);
}

void CVICALLBACK ShowFrameRozcestnik (int menuBar, int menuItem, void *callbackData,
									  int panel)
{
	DisplayPanel(*panelHandleRozcestnik);
	HidePanel(*panelHandleVoltmetr);
	HidePanel(*panelHandleOscil);
	HidePanel(*panelHandleGenerator);
}
///////////////////////////////// End shared functions