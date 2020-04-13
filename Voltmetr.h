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
#define  PANEL_VOLT_RINGMETER             2       /* control type: slide, callback function: (none) */
#define  PANEL_VOLT_RINGDIAL_VOLT_RANGE   3       /* control type: slide, callback function: (none) */
#define  PANEL_VOLT_RINGDIAL_SMOOTHING    4       /* control type: slide, callback function: (none) */
#define  PANEL_VOLT_SW_VOLTMETER_COUPLING 5       /* control type: binary, callback function: (none) */
#define  PANEL_VOLT_SW_VOLTMETER_ENABLE   6       /* control type: binary, callback function: (none) */
#define  PANEL_VOLT_NUMERIC_DIG_VOLTAGE   7       /* control type: numeric, callback function: (none) */


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


#ifdef __cplusplus
    }
#endif