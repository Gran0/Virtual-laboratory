#include <userint.h>

//==============================================================================
//
// Title:		Osciloskop.c
// Purpose:		A short description of the implementation.
//
// Created on:	09.04.2020 at 16:38:05 by Luk� Granzer.
// Copyright:	Vysok� �kola b��sk� - Technick� univerzita Ostrava. All Rights Reserved.
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
const unsigned int maxBFSize = 5*SAMPLING_RATE/10;		// Hloubka bufferu 500 ms, co� je nejdel�� �asov� �sek, kter� lze zobrazit na osciloskopu
double osciloscopeGraphBuffer[maxBFSize];				// Pole kruhov�ho bufferu
unsigned int oscGraphBufferLastIndex = 0;				// Adresa posledn�ho m�sta z�pisu

short unsigned int pointTimeDelta;		// Krok pro p�eskakov�n� data p�i kompresi pole nam��en�ch hodnot
unsigned int oscGraphBufferSampleRate;	// P�epo�ten� frekvence vzorkov�n� dat po kompresi pole nam��en�ch dat

double FFTarray[BUFFER_SIZE];			// Pole pro v�po�et FFT
NIComplexNumber fftTable[BUFFER_SIZE];

typedef enum{ GND=0,
			  DC,
			  AC
			 } CPL;		// V��et pro nastaven� vazby sign�lu
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

	// Vykreslov�n� grafu
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
	
	FFTEx (generatorSignalArray, 5000, 5000, NULL, 0, fftTable);	// V�po�et FFT
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
	
	// V�po�et m���tka X osy FFT
	double dt = BUFFER_SIZE/SAMPLING_RATE;
	double df = 2/dt;
	SetCtrlAttribute(*panelHandleOscil,PANEL_OSC_GRAPH_FFT,ATTR_XAXIS_GAIN,df);	// Upraven� m���tka osy
	SetAxisRange (*panelHandleOscil, PANEL_OSC_GRAPH_FFT, VAL_MANUAL, 0.0, 100.0, VAL_NO_CHANGE, 0.0, 1.0);
}

// Funkce pro kop�rov�n� dat z bufferu gener�toru do bufferu osciloskopu
// Funkce b�� ve vlastn�m vl�knu
int CVICALLBACK BackgroundThread(int *data){
	backgroundThreadRunning = true;
	unsigned int thisIterationIndexStart, cycleEnd,i;
	double sum = 0, ssValue=0;
	
	while(backgroundThreadRunning){	
		pointTimeDelta = BUFFER_SIZE/GRAPH_POINT_COUNT;				// V�po�et kroku cyklu for, tj. faktor podvzorkov�n� dat pro graf
		oscGraphBufferSampleRate = SAMPLING_RATE/pointTimeDelta;	// V�po�et nov� vzorkovac� frekvence dat v grafu 
		oscGraphBufferDeep = 5*oscGraphBufferSampleRate/10;
		
		thisIterationIndexStart = oscGraphBufferLastIndex;
		readMutexFlag = true;	// Ozn�men� pro druh� vl�kno, �e te� prob�h� �ten� - dojde k blokaci z�pisu do pole - MUTEX
		for (unsigned int index = 0; index<BUFFER_SIZE-pointTimeDelta; index+=pointTimeDelta)		
		{
			// Kop�rov�n� dat z gener�toru
			if(coupling == GND)
				osciloscopeGraphBuffer[oscGraphBufferLastIndex] = 0.0;
			else 
				osciloscopeGraphBuffer[oscGraphBufferLastIndex] = generatorSignalArray[index];
		
			if(oscGraphBufferLastIndex < oscGraphBufferDeep-1)
				oscGraphBufferLastIndex++;
			else
				oscGraphBufferLastIndex = 0;
		}
		readMutexFlag = false;	// Uvoln�n� z�mku - MUTEX
		newDataFlag = false;
		
		// Ode�et stejnosm�rn� slo�ky od nov� zapsan�ch dat
		if(coupling == AC){
			cycleEnd = (BUFFER_SIZE-pointTimeDelta)/pointTimeDelta;
			i = thisIterationIndexStart;
			sum = 0.0;
			for (unsigned int index=0; index < cycleEnd; index++)	// V�po�et st�edn� hodnoty
			{
				sum += osciloscopeGraphBuffer[i];
				if(i < oscGraphBufferDeep-1)
					i++;
				else
					i = 0;
			}
			ssValue = sum /( cycleEnd*1.0);
			
			i = thisIterationIndexStart;
			for (unsigned int index=0; index < cycleEnd; index++)	// Ode�et st�edn� hodnoty
			{
				osciloscopeGraphBuffer[i] -= ssValue;
				
				if(i < oscGraphBufferDeep-1)
					i++;
				else
					i = 0;
			}
		}
		
		// �ek�n� na z�pis nov�ch hodnot do sign�lov�ho bufferu gener�toru
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
				SetCtrlAttribute (*panelHandleOscil, PANEL_OSC_BUTTON_OSCIL_RUN, ATTR_CTRL_VAL, 0);	// Zm�� tla��tko
			}
			else{	// Continuos mode
				GetCtrlAttribute (*panelHandleOscil, PANEL_OSC_TIMER_OSC, ATTR_ENABLED, &value);
				if(value == 0)
					SetCtrlAttribute (*panelHandleOscil, PANEL_OSC_TIMER_OSC, ATTR_ENABLED, 1);	// Start m��en�
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
			// Zm�na mez� Y osy
			SetAxisRange (*panelHandleOscil, PANEL_OSC_GRAPH_OSCIL, VAL_NO_CHANGE, 0.0, 0.0, VAL_MANUAL, -voltageRange, voltageRange);
			// Zm�na popisku osciloskopu v z�vislosti na rozsahu
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
			// P�e�ten� hodnoty �asov� z�klady a zm�na rozsahu X osy
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
			// Zm�na re�imu chodu osciloskopu
			GetCtrlVal (*panelHandleOscil, PANEL_OSC_RINGSLIDE_MODE, &value);
			if(value == 1){	
				// Pokud byl osciloskop p�epnut do SINGLE m�du, zastav �asova� a zm�n popisek tla��tka Start
				SetCtrlAttribute(*panelHandleOscil, PANEL_OSC_TIMER_OSC, ATTR_ENABLED, 0);	// Stop
				SetCtrlAttribute (*panelHandleOscil, PANEL_OSC_BUTTON_OSCIL_RUN, ATTR_CTRL_VAL, 0);	// Zm�� tla��tko
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
			// Aktualizace ��seln�ch hodnot kurzor� na panelu p��stroje
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
	// Zastaven� �asova�e pro �ten�
	SetCtrlAttribute(*panelHandleOscil, PANEL_OSC_TIMER_OSC, ATTR_ENABLED, 0);	// Stop
	SetCtrlAttribute (*panelHandleOscil, PANEL_OSC_BUTTON_OSCIL_RUN, ATTR_CTRL_VAL, 0);	// Zm�� tla��tko
	
	unsigned int oscGraphBufferDeep = 5*SAMPLING_RATE/10;	
	const unsigned int maxBFSize = 5*SAMPLING_RATE/10;		
	double oscBuffer[oscGraphBufferDeep];
	double oscFile[maxBFSize];
	
	pointTimeDelta = BUFFER_SIZE/GRAPH_POINT_COUNT;				// V�po�et kroku cyklu for, tj. faktor podvzorkov�n� dat pro graf
	oscGraphBufferSampleRate = SAMPLING_RATE/pointTimeDelta;	// V�po�et nov� vzorkovac� frekvence dat v grafu 
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
			fgets(line,200,file);	// Zaho� prvn� ��dek
			while(fgets(line,200,file)){
				sscanf(line,"%f",&value);
				oscFile[indexF] =(double) value;
				indexF++;
			}
			
			// Zahozen� souboru, pokud nem� soubor spr�vnou d�lku dat
			if(indexF > maxBFSize)
				return;
			
			int i = 0;
			for (unsigned int index = 0; index<indexF-pointTimeDelta; index+=pointTimeDelta)		
			{
				// P�evzorkov�n� dat ze souboru
				oscBuffer[i] = oscFile[index];
				i++;
			}
			
			

			// Vykreslov�n� grafu
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
			
			FFTEx (oscFile, 5000, 5000, NULL, 0, fftTable);	// V�po�et FFT
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
			
			// V�po�et m���tka X osy FFT
			double dt = BUFFER_SIZE/SAMPLING_RATE;
			double df = 2/dt;
			SetCtrlAttribute(*panelHandleOscil,PANEL_OSC_GRAPH_FFT,ATTR_XAXIS_GAIN,df);	// Upraven� m���tka osy
			SetAxisRange (*panelHandleOscil, PANEL_OSC_GRAPH_FFT, VAL_MANUAL, 0.0, 100.0, VAL_NO_CHANGE, 0.0, 1.0);
			
			
		}
	}	
}
