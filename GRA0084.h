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

#define  PANEL                            1       /* callback function: panelRozcestnik_Close */
#define  PANEL_AppTitle                   2       /* control type: textMsg, callback function: (none) */
#define  PANEL_TextPoweredBy              3       /* control type: textMsg, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

#define  MENUBAR                          1
#define  MENUBAR_MENU1                    2
#define  MENUBAR_MENU1_ITEM2              3       /* callback function: ShowFrameOsciloskop */
#define  MENUBAR_MENU1_ITEM3              4       /* callback function: ShowFrameVoltmeter */
#define  MENUBAR_MENU1_ITEM4              5       /* callback function: ShowFrameGenerator */


     /* Callback Prototypes: */

int  CVICALLBACK panelRozcestnik_Close(int panel, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK ShowFrameGenerator(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK ShowFrameOsciloskop(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK ShowFrameVoltmeter(int menubar, int menuItem, void *callbackData, int panel);


#ifdef __cplusplus
    }
#endif