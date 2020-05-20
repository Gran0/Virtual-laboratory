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

#include <windows.h>
#include <formatio.h>
#include <advanlys.h>
#include <ansi_c.h>
#include <utility.h>

#define GRAPH_POINT_COUNT  1000;

unsigned int oscGraphBufferDeep = 5*SAMPLING_RATE/10;	
const unsigned int maxBFSize = 5*SAMPLING_RATE/10;		// Hloubka bufferu 500 ms, což je nejdelší èasový úsek, který lze zobrazit na osciloskopu
double osciloscopeGraphBuffer[maxBFSize];				// Pole kruhového bufferu
unsigned int oscGraphBufferLastIndex = 0;				// Adresa posledního místa zápisu

short unsigned int pointTimeDelta;		// Krok pro pøeskakování data pøi kompresi pole namìøených hodnot
unsigned int oscGraphBufferSampleRate;	// Pøepoètená frekvence vzorkování dat po kompresi pole namìøených dat

double FFTarray[BUFFER_SIZE];			// Pole pro výpoèet FFT
NIComplexNumber fftTable[BUFFER_SIZE];

typedef enum{ GND=0,
			  DC,
			  AC
			 } CPL;		// Výèet pro nastavení vazby signálu
CPL coupling = GND;
// -------------------- Panel functions
int CVICALLBACK panelOSC_Close (int panel, int event, void *callbackData,
								int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_CLOSE:
			QuitUserInterface(0);
			break;
		default:
			break;
	}
	return 0;
}

void RenderGraphAndFFT(){
	
	int timeBase;
	GetCtrlVal (*panelHandleOscil, PANEL_OSC_RINGKNOB_TIMEBASE, &timeBase);

	// Vykreslování grafu
	DeleteGraphPlot(*panelHandleOscil, PANEL_OSC_GRAPH_OSCIL,-1, VAL_DELAYED_DRAW);
	PlotY (*panelHandleOscil,
								PANEL_OSC_GRAPH_OSCIL,
								osciloscopeGraphBuffer,
								oscGraphBufferDeep,
								VAL_DOUBLE,
								VAL_THIN_LINE,
								VAL_EMPTY_SQUARE,
								VAL_SOLID,
								oscGraphBufferSampleRate,
								VAL_YELLOW);
	RefreshGraph (*panelHandleOscil, PANEL_OSC_GRAPH_OSCIL);
	
	FFTEx (generatorSignalArray, 5000, 5000, NULL, 0, fftTable);	// Výpoèet FFT
	for (unsigned int i = 0; i<BUFFER_SIZE; i++)
	{
		FFTarray[i] = sqrt(pow(fftTable[i].real,2) + pow(fftTable[i].imaginary,2));
	}
	for (unsigned int i = 0; i<BUFFER_SIZE; i++)
	{
		FFTarray[i] /= 5000.0 ;
	}
	DeleteGraphPlot (*panelHandleOscil, PANEL_OSC_GRAPH_FFT, -1, VAL_DELAYED_DRAW);
	PlotY (*panelHandleOscil, PANEL_OSC_GRAPH_FFT, FFTarray, BUFFER_SIZE, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_YELLOW);
	
	// Výpoèet mìøítka X osy FFT
	double dt = BUFFER_SIZE/SAMPLING_RATE;
	double df = 2/dt;
	SetCtrlAttribute(*panelHandleOscil,PANEL_OSC_GRAPH_FFT,ATTR_XAXIS_GAIN,df);	// Upravení mìøítka osy
	SetAxisRange (*panelHandleOscil, PANEL_OSC_GRAPH_FFT, VAL_MANUAL, 0.0, 100.0, VAL_NO_CHANGE, 0.0, 1.0);
}

// Funkce pro kopírování dat z bufferu generátoru do bufferu osciloskopu
// Funkce bìží ve vlastním vláknu
int CVICALLBACK BackgroundThread(int *data){
	backgroundThreadRunning = true;
	unsigned int thisIterationIndexStart, cycleEnd,i;
	double sum = 0, ssValue=0;
	
	while(backgroundThreadRunning){	
		pointTimeDelta = BUFFER_SIZE/GRAPH_POINT_COUNT;				// Výpoèet kroku cyklu for, tj. faktor podvzorkování dat pro graf
		oscGraphBufferSampleRate = SAMPLING_RATE/pointTimeDelta;	// Výpoèet nové vzorkovací frekvence dat v grafu 
		oscGraphBufferDeep = 5*oscGraphBufferSampleRate/10;
		
		thisIterationIndexStart = oscGraphBufferLastIndex;
		readMutexFlag = true;	// Oznámení pro druhé vlákno, že teï probíhá ètení - dojde k blokaci zápisu do pole - MUTEX
		for (unsigned int index = 0; index<BUFFER_SIZE-pointTimeDelta; index+=pointTimeDelta)		
		{
			// Kopírování dat z generátoru
			if(coupling == GND)
				osciloscopeGraphBuffer[oscGraphBufferLastIndex] = 0.0;
			else 
				osciloscopeGraphBuffer[oscGraphBufferLastIndex] = generatorSignalArray[index];
		
			if(oscGraphBufferLastIndex < oscGraphBufferDeep-1)
				oscGraphBufferLastIndex++;
			else
				oscGraphBufferLastIndex = 0;
		}
		readMutexFlag = false;	// Uvolnìní zámku - MUTEX
		newDataFlag = false;
		
		// Odeèet stejnosmìrné složky od novì zapsaných dat
		if(coupling == AC){
			cycleEnd = (BUFFER_SIZE-pointTimeDelta)/pointTimeDelta;
			i = thisIterationIndexStart;
			sum = 0.0;
			for (unsigned int index=0; index < cycleEnd; index++)	// Výpoèet støední hodnoty
			{
				sum += osciloscopeGraphBuffer[i];
				if(i < oscGraphBufferDeep-1)
					i++;
				else
					i = 0;
			}
			ssValue = sum /( cycleEnd*1.0);
			
			i = thisIterationIndexStart;
			for (unsigned int index=0; index < cycleEnd; index++)	// Odeèet støední hodnoty
			{
				osciloscopeGraphBuffer[i] -= ssValue;
				
				if(i < oscGraphBufferDeep-1)
					i++;
				else
					i = 0;
			}
		}
		
		// Èekání na zápis nových hodnot do signálového bufferu generátoru
		while(!newDataFlag && backgroundThreadRunning){
			Sleep(2);
		};	
		
	}
	return 0;
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
			if(value == 1){	// Single mode
				RenderGraphAndFFT();
				SetCtrlAttribute (*panelHandleOscil, PANEL_OSC_BUTTON_OSCIL_RUN, ATTR_CTRL_VAL, 0);	// Zmìò tlaèítko
			}
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
			// Zmìna mezí Y osy
			SetAxisRange (*panelHandleOscil, PANEL_OSC_GRAPH_OSCIL, VAL_NO_CHANGE, 0.0, 0.0, VAL_MANUAL, -voltageRange, voltageRange);
			// Zmìna popisku osciloskopu v závislosti na rozsahu
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
			// Pøeètení hodnoty èasové základy a zmìna rozsahu X osy
			GetCtrlVal (*panelHandleOscil, PANEL_OSC_RINGKNOB_TIMEBASE, &timeBase);
			timeBase *= 5;
			SetAxisRange (*panelHandleOscil, PANEL_OSC_GRAPH_OSCIL, VAL_MANUAL, 0.0, timeBase, VAL_NO_CHANGE, 0.0, 1.0);
			break;
	}
	return 0;
}



int CVICALLBACK OSC_COUPLING_CHANGE (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	int cpl;
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (*panelHandleOscil, PANEL_OSC_RINGSLIDE_COUPLING, &cpl);
			switch (cpl)
			{
				case 0:
					coupling = GND;
					break;
				case 1:
					coupling = DC;
					break;
				case 2:
					coupling = AC;
					break;
				default:
					break;
			}
			break;
	}
	return 0;
}

int CVICALLBACK OSC_MODE_CHANGE (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	int value;
	switch (event)
	{
		case EVENT_COMMIT:
			// Zmìna režimu chodu osciloskopu
			GetCtrlVal (*panelHandleOscil, PANEL_OSC_RINGSLIDE_MODE, &value);
			if(value == 1){	
				// Pokud byl osciloskop pøepnut do SINGLE módu, zastav èasovaè a zmìn popisek tlaèítka Start
				SetCtrlAttribute(*panelHandleOscil, PANEL_OSC_TIMER_OSC, ATTR_ENABLED, 0);	// Stop
				SetCtrlAttribute (*panelHandleOscil, PANEL_OSC_BUTTON_OSCIL_RUN, ATTR_CTRL_VAL, 0);	// Zmìò tlaèítko
			}
			break;
	}
	return 0;
}

int CVICALLBACK GRAPH_FFT_EVEN (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	double K1amp,K1f,K2amp,K2f;
	switch (event)
	{
		case EVENT_COMMIT:
			GetGraphCursor (*panelHandleOscil, PANEL_OSC_GRAPH_FFT, 1 ,&K1f, &K1amp);
			GetGraphCursor (*panelHandleOscil, PANEL_OSC_GRAPH_FFT, 2 ,&K2f, &K2amp );
			// Aktualizace èíselných hodnot kurzorù na panelu pøístroje
			SetCtrlVal(*panelHandleOscil,PANEL_OSC_NUMERIC_K1_AMP,K1amp);
			SetCtrlVal(*panelHandleOscil,PANEL_OSC_NUMERIC_K1_F,K1f*20);
			SetCtrlVal(*panelHandleOscil,PANEL_OSC_NUMERIC_K2_AMP,K2amp);
			SetCtrlVal(*panelHandleOscil,PANEL_OSC_NUMERIC_K2_F,K2f*20);
			break;
		case EVENT_LEFT_CLICK:
			break;
	}
	return 0;
}

void CVICALLBACK LoadDataFromFile (int menuBar, int menuItem, void *callbackData,
								   int panel)
{
	// Zastavení èasovaèe pro ètení
	SetCtrlAttribute(*panelHandleOscil, PANEL_OSC_TIMER_OSC, ATTR_ENABLED, 0);	// Stop
	SetCtrlAttribute (*panelHandleOscil, PANEL_OSC_BUTTON_OSCIL_RUN, ATTR_CTRL_VAL, 0);	// Zmìò tlaèítko
	
	unsigned int oscGraphBufferDeep = 5*SAMPLING_RATE/10;	
	const unsigned int maxBFSize = 5*SAMPLING_RATE/10;		
	double oscBuffer[oscGraphBufferDeep];
	double oscFile[maxBFSize];
	
	pointTimeDelta = BUFFER_SIZE/GRAPH_POINT_COUNT;				// Výpoèet kroku cyklu for, tj. faktor podvzorkování dat pro graf
	oscGraphBufferSampleRate = SAMPLING_RATE/pointTimeDelta;	// Výpoèet nové vzorkovací frekvence dat v grafu 
	oscGraphBufferDeep = 5*oscGraphBufferSampleRate/10;
	
	int status;
    static FILE *file;
	static char path[400], dir[400];
	char line[200];
	float value;
	int indexF = 0;
	int timeBase;

    GetProjectDir (dir);
	status = FileSelectPopupEx (dir, "datafile.txt",
								"Datafiles (*.txt)",
								"DataFile Storage",
								VAL_LOAD_BUTTON,
								0,
								1,
								path);
	
    if (status != VAL_NO_FILE_SELECTED) {
		file = fopen(path,"r");
		
		if(file != NULL){	
			fgets(line,200,file);	// Zahoï první øádek
			while(fgets(line,200,file)){
				sscanf(line,"%f",&value);
				oscFile[indexF] =(double) value;
				indexF++;
			}
			
			// Zahození souboru, pokud nemá soubor správnou délku dat
			if(indexF > maxBFSize)
				return;
			
			int i = 0;
			for (unsigned int index = 0; index<indexF-pointTimeDelta; index+=pointTimeDelta)		
			{
				// Pøevzorkování dat ze souboru
				oscBuffer[i] = oscFile[index];
				i++;
			}
			
			

			// Vykreslování grafu
			GetCtrlVal (*panelHandleOscil, PANEL_OSC_RINGKNOB_TIMEBASE, &timeBase);
			DeleteGraphPlot(*panelHandleOscil, PANEL_OSC_GRAPH_OSCIL,-1, VAL_DELAYED_DRAW);
			PlotY (*panelHandleOscil,
										PANEL_OSC_GRAPH_OSCIL,
										oscBuffer,
										i,
										VAL_DOUBLE,
										VAL_THIN_LINE,
										VAL_EMPTY_SQUARE,
										VAL_SOLID,
										oscGraphBufferSampleRate,
										VAL_YELLOW);
			RefreshGraph (*panelHandleOscil, PANEL_OSC_GRAPH_OSCIL);
			
			FFTEx (oscFile, 5000, 5000, NULL, 0, fftTable);	// Výpoèet FFT
			for (unsigned int i = 0; i<BUFFER_SIZE; i++)
			{
				FFTarray[i] = sqrt(pow(fftTable[i].real,2) + pow(fftTable[i].imaginary,2));
			}
			for (unsigned int i = 0; i<BUFFER_SIZE; i++)
			{
				FFTarray[i] /= 5000.0 ;
			}
			DeleteGraphPlot (*panelHandleOscil, PANEL_OSC_GRAPH_FFT, -1, VAL_DELAYED_DRAW);
			PlotY (*panelHandleOscil, PANEL_OSC_GRAPH_FFT, FFTarray, BUFFER_SIZE, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_YELLOW);
			
			// Výpoèet mìøítka X osy FFT
			double dt = BUFFER_SIZE/SAMPLING_RATE;
			double df = 2/dt;
			SetCtrlAttribute(*panelHandleOscil,PANEL_OSC_GRAPH_FFT,ATTR_XAXIS_GAIN,df);	// Upravení mìøítka osy
			SetAxisRange (*panelHandleOscil, PANEL_OSC_GRAPH_FFT, VAL_MANUAL, 0.0, 100.0, VAL_NO_CHANGE, 0.0, 1.0);
			
			
		}
	}	
}
