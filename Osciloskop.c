#include <userint.h>

//==============================================================================
//
// Title:		Osciloskop.c
// Purpose:		A short description of the implementation.
//
// Created on:	09.04.2020 at 16:38:05 by Lukáš Granzer.
// Copyright:	Vysoká škola báòská - Technická univerzita Ostrava. All Rights Reserved.
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

void RenderGraphAndFFT(){
	
	int timeBase;
	GetCtrlVal (*panelHandleOscil, PANEL_OSC_RINGKNOB_TIMEBASE, &timeBase);
	int xDataLength = timeBase*0.5;	// Délka èasové osy v sekundách
	xDataLength = SAMPLING_RATE*xDataLength;	// Poèet vzorkù
	
	
	// Vykreslování grafu
	RefreshGraph (*panelHandleOscil, PANEL_OSC_GRAPH_OSCIL);
	DeleteGraphPlot(*panelHandleOscil, PANEL_OSC_GRAPH_OSCIL,-1, VAL_IMMEDIATE_DRAW);
	int plotHandle = PlotY (*panelHandleOscil,
							PANEL_OSC_GRAPH_OSCIL,
							generatorSignalArray,
							xDataLength,
							VAL_DOUBLE,
							VAL_THIN_LINE,
							VAL_EMPTY_SQUARE,
							VAL_SOLID,
							SAMPLING_RATE,
							VAL_YELLOW);

	
	
	
}

int CVICALLBACK TIME_OSC_TICK (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
			RenderGraphAndFFT();
			break;
	}
	return 0;
}

int CVICALLBACK BTN_RUN_CLICK (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	
	int value;
	
	switch (event)
	{
		case EVENT_COMMIT:
			
			GetCtrlVal (*panelHandleOscil, PANEL_OSC_RINGSLIDE_MODE, &value);
			if(value == 1)	// Single mode
				RenderGraphAndFFT();
			else{	// Continuos mode
				GetCtrlAttribute (*panelHandleOscil, PANEL_OSC_TIMER_OSC, ATTR_ENABLED, &value);
				if(value == 0)
					SetCtrlAttribute (*panelHandleOscil, PANEL_OSC_TIMER_OSC, ATTR_ENABLED, 1);	// Start mìøení
				else
					SetCtrlAttribute (*panelHandleOscil, PANEL_OSC_TIMER_OSC, ATTR_ENABLED, 0);	// Stop
			}
			break;
	}
	return 0;
}

int CVICALLBACK OSC_SENSITIVITY_CHANGE (int panel, int control, int event,
										void *callbackData, int eventData1, int eventData2)
{
	double voltageRange;
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (*panelHandleOscil, PANEL_OSC_RINGKNOB_SENSITIVITY, &voltageRange);
			
			SetAxisRange (*panelHandleOscil, PANEL_OSC_GRAPH_OSCIL, VAL_NO_CHANGE, 0.0, 0.0, VAL_MANUAL, -voltageRange, voltageRange);
			
			if(voltageRange <= 1.0){
				SetCtrlAttribute (*panelHandleOscil, PANEL_OSC_GRAPH_OSCIL, ATTR_YAXIS_GAIN, 1000.0);
				SetCtrlAttribute (*panelHandleOscil, PANEL_OSC_GRAPH_OSCIL, ATTR_YNAME, "U [mV]");
			}
			else{
				SetCtrlAttribute (*panelHandleOscil, PANEL_OSC_GRAPH_OSCIL, ATTR_YAXIS_GAIN, 1.0);
				SetCtrlAttribute (*panelHandleOscil, PANEL_OSC_GRAPH_OSCIL, ATTR_YNAME, "U [V]");
			}	
			break;
	}
	return 0;
}

int CVICALLBACK OSC_TIMEBASE_CHANGE (int panel, int control, int event,
									 void *callbackData, int eventData1, int eventData2)
{
	int timeBase;
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (*panelHandleOscil, PANEL_OSC_RINGKNOB_TIMEBASE, &timeBase);
			timeBase *= 5;
			SetAxisRange (*panelHandleOscil, PANEL_OSC_GRAPH_OSCIL, VAL_MANUAL, 0.0, timeBase, VAL_NO_CHANGE, 0.0, 1.0);
			
			break;
	}
	return 0;
}


