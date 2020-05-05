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

#define  PANEL_OSC                        1       /* callback function: panelOSC_Close */
#define  PANEL_OSC_GRAPH_OSCIL            2       /* control type: graph, callback function: (none) */
#define  PANEL_OSC_GRAPH_FFT              3       /* control type: graph, callback function: (none) */
#define  PANEL_OSC_RINGSLIDE_MODE         4       /* control type: slide, callback function: (none) */
#define  PANEL_OSC_RINGSLIDE_COUPLING     5       /* control type: slide, callback function: (none) */
#define  PANEL_OSC_RINGKNOB_SENSITIVITY   6       /* control type: slide, callback function: OSC_SENSITIVITY_CHANGE */
#define  PANEL_OSC_RINGKNOB_TIMEBASE      7       /* control type: slide, callback function: OSC_TIMEBASE_CHANGE */
#define  PANEL_OSC_BUTTON_OSCIL_RUN       8       /* control type: textButton, callback function: BTN_RUN_CLICK */
#define  PANEL_OSC_DECORATION             9       /* control type: deco, callback function: (none) */
#define  PANEL_OSC_TIMER_OSC              10      /* control type: timer, callback function: TIME_OSC_TICK */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

#define  MENUBAR                          1
#define  MENUBAR_M_OSC1                   2
#define  MENUBAR_M_OSC1_I_OSC             3       /* callback function: ShowFrameRozcestnik */
#define  MENUBAR_M_OSC1_I_OSC2            4       /* callback function: ShowFrameVoltmeter */
#define  MENUBAR_M_OSC1_I_OSC3            5       /* callback function: ShowFrameGenerator */


     /* Callback Prototypes: */

int  CVICALLBACK BTN_RUN_CLICK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OSC_SENSITIVITY_CHANGE(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OSC_TIMEBASE_CHANGE(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK panelOSC_Close(int panel, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK ShowFrameGenerator(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK ShowFrameRozcestnik(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK ShowFrameVoltmeter(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK TIME_OSC_TICK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif