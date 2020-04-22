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

#define  PANEL_VOLT                       1       /* callback function: panelVoltmeter_Close */
#define  PANEL_VOLT_RINGDIAL_VOLT_RANGE   2       /* control type: slide, callback function: Voltmeter_Range_Change */
#define  PANEL_VOLT_RINGDIAL_SMOOTHING    3       /* control type: slide, callback function: Voltmeter_Smoothing_Change */
#define  PANEL_VOLT_SW_VOLTMETER_COUPLING 4       /* control type: binary, callback function: Voltmeter_Coupling_Change */
#define  PANEL_VOLT_SW_VOLTMETER_ENABLE   5       /* control type: binary, callback function: Voltmeter_Signal_Change */
#define  PANEL_VOLT_GAUGE                 6       /* control type: scale, callback function: (none) */
#define  PANEL_VOLT_NUMERIC_DIG_VOLTAGE   7       /* control type: numeric, callback function: (none) */
#define  PANEL_VOLT_TIMER_ANIMATION       8       /* control type: timer, callback function: TIME_ANIMATION_TICK */
#define  PANEL_VOLT_TIMER_VOLT_READ       9       /* control type: timer, callback function: V_TIMER_TICK */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

#define  MENUBAR                          1
#define  MENUBAR_MENU1                    2
#define  MENUBAR_MENU1_ITEM1              3       /* callback function: ShowFrameRozcestnik */
#define  MENUBAR_MENU1_ITEM2              4       /* callback function: ShowFrameOsciloskop */
#define  MENUBAR_MENU1_ITEM3              5       /* callback function: ShowFrameGenerator */


     /* Callback Prototypes: */

int  CVICALLBACK panelVoltmeter_Close(int panel, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK ShowFrameGenerator(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK ShowFrameOsciloskop(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK ShowFrameRozcestnik(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK TIME_ANIMATION_TICK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK V_TIMER_TICK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Voltmeter_Coupling_Change(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Voltmeter_Range_Change(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Voltmeter_Signal_Change(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Voltmeter_Smoothing_Change(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif