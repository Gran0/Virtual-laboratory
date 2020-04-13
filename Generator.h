/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL_GEN                        1       /* callback function: panelGenerator_Close */
#define  PANEL_GEN_RINGSLIDE_SIGNAL_TYPE  2       /* control type: slide, callback function: Signal_type_Change */
#define  PANEL_GEN_NUMERIC_NOISE          3       /* control type: scale, callback function: Noise_Change */
#define  PANEL_GEN_NUMERIC_FREQUENCY      4       /* control type: scale, callback function: Freq_Change */
#define  PANEL_GEN_NUMERIC_OFFSET         5       /* control type: scale, callback function: Offset_Change */
#define  PANEL_GEN_NUMERIC_AMPLITUDE      6       /* control type: scale, callback function: Amplitude_Change */
#define  PANEL_GEN_GRAPH_WAVEFORM         7       /* control type: graph, callback function: (none) */
#define  PANEL_GEN_BUTTON_ENABLE_GEN      8       /* control type: textButton, callback function: Generator_run_Click */
#define  PANEL_GEN_TIMER_GENERATOR        9       /* control type: timer, callback function: TimerGen_Tick */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

#define  MENUBAR                          1
#define  MENUBAR_MENU2                    2
#define  MENUBAR_MENU2_ITEM1              3       /* callback function: ShowFrameRozcestnik */
#define  MENUBAR_MENU2_ITEM2              4       /* callback function: ShowFrameOsciloskop */
#define  MENUBAR_MENU2_ITEM3              5       /* callback function: ShowFrameVoltmeter */


     /* Callback Prototypes: */

int  CVICALLBACK Amplitude_Change(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Freq_Change(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Generator_run_Click(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Noise_Change(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Offset_Change(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK panelGenerator_Close(int panel, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK ShowFrameOsciloskop(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK ShowFrameRozcestnik(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK ShowFrameVoltmeter(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK Signal_type_Change(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TimerGen_Tick(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif