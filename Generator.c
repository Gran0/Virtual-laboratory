//==============================================================================
//
// Title:		Generator.c
// Purpose:		A short description of the implementation.
//
// Created on:	09.04.2020 at 17:31:49 by Lukáš Granzer.
// Copyright:	Vysoká škola báòská - Technická univerzita Ostrava. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files
#include <advanlys.h>
#include "toolbox.h"
#include "Voltmetr.h"
#include <userint.h>
#include "Generator.h"
#include "GlobalVariables.h"


double amplitude,offset,noise;
unsigned int frequence = 1;
double phase = 0;
enum{SINUS,TRIANG,SQUARE} waveformType;

const unsigned int bufferDeep = 3*SAMPLING_RATE/10;	// Hloubka bufferu 300 ms;
double generatorGraphBuffer[bufferDeep];
unsigned int bufferLastIndex = 0;

int CVICALLBACK panelGenerator_Close (int panel, int event, void *callbackData,
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

int CVICALLBACK TimerGen_Tick (int panel, int control, int event,
									 void *callbackData, int eventData1, int eventData2)
{	
	double noiseSamples[BUFFER_SIZE];
	switch (event)
	{
		case EVENT_TIMER_TICK:
			// Generování poleho hodnot signálu podle nastavení
			while(readMutexFlag);	// Èekání až ostatní moduly dokonèí ètení.
	
			switch (waveformType)
			{
				case SINUS:
					SineWave(BUFFER_SIZE,amplitude,frequence/SAMPLING_RATE,&phase,generatorSignalArray);
					break;
				case TRIANG:
					TriangleWave(BUFFER_SIZE,amplitude,frequence/SAMPLING_RATE,&phase,generatorSignalArray);
					break;
				case SQUARE:
					SquareWave(BUFFER_SIZE,amplitude,frequence/SAMPLING_RATE,&phase,50,generatorSignalArray);
					break;	
				default:
					break;
			}
			
			// Pøidání šumu do signálu
			WhiteNoise (BUFFER_SIZE, noise,0, noiseSamples);
			for (int i=0; i <BUFFER_SIZE; i++)
			{
				generatorSignalArray[i] += noiseSamples[i]+offset;
			}
			newDataFlag = true;
			// Kopírování nových namìøených hodnot do kruhového bufferu grafu generátoru
			for (unsigned int index = 0; index<BUFFER_SIZE; index++)
			{
				generatorGraphBuffer[bufferLastIndex] = generatorSignalArray[index];
				if(bufferLastIndex < bufferDeep-1)
					bufferLastIndex++;
				else
					bufferLastIndex = 0;
			}
			
			// Vykreslování grafu v generátoru signálu
			RefreshGraph (*panelHandleGenerator, PANEL_GEN_GRAPH_WAVEFORM);
			DeleteGraphPlot(*panelHandleGenerator, PANEL_GEN_GRAPH_WAVEFORM,-1, VAL_IMMEDIATE_DRAW);
			int plotHandle = PlotY (*panelHandleGenerator, PANEL_GEN_GRAPH_WAVEFORM, generatorGraphBuffer, bufferDeep, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID,
									SAMPLING_RATE, VAL_YELLOW);
			break;
	}
	return 0;
}

int CVICALLBACK Signal_type_Change (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			int choice;
			GetCtrlVal (*panelHandleGenerator, PANEL_GEN_RINGSLIDE_SIGNAL_TYPE, &choice);
			waveformType = choice;			
			break;
	}
	return 0;
}

int CVICALLBACK Amplitude_Change (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(*panelHandleGenerator,PANEL_GEN_NUMERIC_AMPLITUDE,&amplitude);
			break;
	}
	return 0;
}

int CVICALLBACK Offset_Change (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(*panelHandleGenerator,PANEL_GEN_NUMERIC_OFFSET,&offset);
			break;
	}
	return 0;
}

int CVICALLBACK Freq_Change (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(*panelHandleGenerator,PANEL_GEN_NUMERIC_FREQUENCY,&frequence);
			
			break;
	}
	return 0;
}

int CVICALLBACK Noise_Change (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(*panelHandleGenerator,PANEL_GEN_NUMERIC_NOISE,&noise);
			break;
	}
	return 0;
}

int CVICALLBACK Generator_run_Click (int panel, int control, int event,
									 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:	// Start-Stop signal generator 
			int choice;
			GetCtrlVal (*panelHandleGenerator, PANEL_GEN_BUTTON_ENABLE_GEN, &choice);		
			GetCtrlVal(*panelHandleGenerator,PANEL_GEN_NUMERIC_AMPLITUDE,&amplitude);
			GetCtrlVal(*panelHandleGenerator,PANEL_GEN_NUMERIC_OFFSET,&offset);
			GetCtrlVal(*panelHandleGenerator,PANEL_GEN_NUMERIC_FREQUENCY,&frequence);
			GetCtrlVal(*panelHandleGenerator,PANEL_GEN_NUMERIC_NOISE,&noise);
			SetCtrlAttribute (*panelHandleGenerator, PANEL_GEN_TIMER_GENERATOR, ATTR_ENABLED, choice);
			break;
	}
	return 0;
}


void CVICALLBACK SaveWaveformToFile (int menuBar, int menuItem, void *callbackData,
									 int panel)
{
	int status;
    static FILE *file;
	static char path[400], dir[400];

    GetProjectDir (dir);
    status = FileSelectPopupEx (dir, "datafile.txt","Datafiles (*.txt)", "DataFile Storage",VAL_SAVE_BUTTON, 0, 1, path);
    if (status != VAL_NO_FILE_SELECTED) {
	    file = fopen (path, "w+");
		fprintf(file,"Generovaný signál (vzorkovací frekvence %.2f kHz)        Vygenerováno programem VirtualInstrumentation - autor GRA0084\n",SAMPLING_RATE/1000.0);
		
        for (unsigned int i=0; i<BUFFER_SIZE;i++)
    	    fprintf (file, "%.2f\n", generatorSignalArray[i]);
        fclose (file);
	}
	
}
