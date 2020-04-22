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

float gaugeValue = 0;
float animatedGaugeValue = 0;

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

int CVICALLBACK Voltmeter_Range_Change (int panel, int control, int event,
										void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			double range;
			GetCtrlVal (*panelHandleVoltmetr, PANEL_VOLT_RINGDIAL_VOLT_RANGE, &range);
			SetCtrlAttribute (*panelHandleVoltmetr, PANEL_VOLT_GAUGE, ATTR_MAX_VALUE, range);
			SetCtrlAttribute (*panelHandleVoltmetr, PANEL_VOLT_GAUGE, ATTR_MIN_VALUE, -range);
			if(range < 20.0)
				SetCtrlAttribute (*panelHandleVoltmetr, PANEL_VOLT_GAUGE, ATTR_PRECISION, 3);
			else
				SetCtrlAttribute (*panelHandleVoltmetr, PANEL_VOLT_GAUGE, ATTR_PRECISION, 0);
			break;
	}
	return 0;
}

int CVICALLBACK Voltmeter_Smoothing_Change (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			double tm;
			GetCtrlVal (*panelHandleVoltmetr, PANEL_VOLT_RINGDIAL_SMOOTHING, &tm);	
			SetCtrlAttribute (*panelHandleVoltmetr, PANEL_VOLT_TIMER_VOLT_READ, ATTR_INTERVAL, tm);
			break;
	}
	return 0;
}

int CVICALLBACK Voltmeter_Signal_Change (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int signal;
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (*panelHandleVoltmetr, PANEL_VOLT_SW_VOLTMETER_ENABLE, &signal);
			/*if(signal == 1){
				SetCtrlAttribute (*panelHandleVoltmetr, PANEL_VOLT_VOLTMETER_TIMER, ATTR_ENABLED, 1);
			}
			else{
				SetCtrlAttribute (*panelHandleVoltmetr, PANEL_VOLT_VOLTMETER_TIMER, ATTR_ENABLED, 0);
			}*/
			break;
	}
	return 0;
}

int CVICALLBACK Voltmeter_Coupling_Change (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			break;
	}
	return 0;
}

int CVICALLBACK V_TIMER_TICK (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
			int coupling,enable;
			GetCtrlVal (*panelHandleVoltmetr, PANEL_VOLT_SW_VOLTMETER_COUPLING, &coupling);
			GetCtrlVal (*panelHandleVoltmetr, PANEL_VOLT_SW_VOLTMETER_ENABLE, &enable);
			if(enable == 1){
				float sum = 0.0f;
				for(int i=0; i<BUFFER_SIZE; i++){
					sum += generatorSignalArray[i];
				}
				sum /= BUFFER_SIZE;
				gaugeValue = sum;
			}
			else{
				gaugeValue = 0.0f;				
			}
		
			SetCtrlVal (*panelHandleVoltmetr,PANEL_VOLT_NUMERIC_DIG_VOLTAGE, gaugeValue);
			break;
	}
	return 0;
}

int CVICALLBACK TIME_ANIMATION_TICK (int panel, int control, int event,
									 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
	    // Animov·nÌ pohybu ruËiËky voltmetru - simuluje setrvaËnost ukazatele re·lnÈho analogovÈho p¯Ìstroje
		case EVENT_TIMER_TICK:
			animatedGaugeValue = animatedGaugeValue + (-animatedGaugeValue+gaugeValue)*0.2f;			
			SetCtrlVal (*panelHandleVoltmetr,PANEL_VOLT_GAUGE, animatedGaugeValue);
			break;
	}
	return 0;
}
