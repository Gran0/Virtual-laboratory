#include <advanlys.h>
#include <ansi_c.h>
#include "Generator.h"
#include <userint.h>
#include <stdbool.h>

//==============================================================================
//
// Title:		Voltmetr.c
// Purpose:		A short description of the implementation.
//
// Created on:	09.04.2020 at 17:31:08 by Lukáš Granzer.
// Copyright:	Vysoká škola báòská - Technická univerzita Ostrava. All Rights Reserved.
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
			// Pøepínání rozsahu pøístroje
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
// Výpoèet støední hodnoty
float SSvalue(double *array,int size){
	double suma = 0.0f;
	for(uint16_t i=0; i<size-1; i++){
		suma += array[i];
	}
	suma /= size;
	return (float) suma;
}
// Výpoèet efektivní hodnoty
float RMScalc(double *array,int size){
	double suma = 0.0f;
	uint16_t counter = 0;
	bool underZero=false;
	bool aboveZero=false;
	
	for(uint16_t x=0; x<size-1; x++){
		// Detektor jedné periody
		if(underZero == false && array[x] < 0.0){	// Èekání na prùchod nulou
			underZero = true;
		}
		if(underZero == true && array[x] > 0.0){ 	// zaèátek pùlperiody
			aboveZero = true;
		}
		if(aboveZero == true && array[x] < 0.0)		// konec pùlperiody
			break;
		
		if(aboveZero == true){		// Integrování druhé mocniny
			suma += array[x]*array[x];
			counter++;
		}	
	}
	double result =  suma / counter;
	// Ochrana proti odmoninì ze záporného èísla
	if(result < 0)
		result = -sqrt(-result);
	else
		result = sqrt(result);
	
	return (float) result;
}
int CVICALLBACK V_TIMER_TICK (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	double values[BUFFER_SIZE];
	
	switch (event)
	{
		case EVENT_TIMER_TICK:
			int coupling,enable;
			GetCtrlVal (*panelHandleVoltmetr, PANEL_VOLT_SW_VOLTMETER_COUPLING, &coupling);
			GetCtrlVal (*panelHandleVoltmetr, PANEL_VOLT_SW_VOLTMETER_ENABLE, &enable);
			
			// Výpoèet hodnoty ukazatele v závislosti na nastavení pøístroje
			if(enable == 1){
				if(coupling == 1)	// DC
					gaugeValue = SSvalue(generatorSignalArray,BUFFER_SIZE);
				else{		// AC
					float DC = SSvalue(generatorSignalArray,BUFFER_SIZE);

					for (int i=0; i<BUFFER_SIZE-1; i++)
					{
						values[i] = generatorSignalArray[i] - DC;
					}
					gaugeValue = RMScalc(values,5000);
				}	
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
	    // Animování pohybu ruèièky voltmetru - simuluje setrvaènost ukazatele reálného analogového pøístroje
		case EVENT_TIMER_TICK:
			double range;
			GetCtrlVal (*panelHandleVoltmetr, PANEL_VOLT_RINGDIAL_VOLT_RANGE, &range);
			
			// pøepínání setrvaènosti ruèièky voltmetru podle rozsahu
			if(range >= 2.0)
				animatedGaugeValue = animatedGaugeValue + (-animatedGaugeValue+gaugeValue)*0.2f; // konstanta urèuje dynamiku ruèièky	
			else
				animatedGaugeValue = animatedGaugeValue + (-animatedGaugeValue+gaugeValue)*0.1f;
			
			SetCtrlVal (*panelHandleVoltmetr,PANEL_VOLT_GAUGE, animatedGaugeValue);
			break;
	}
	return 0;
}
