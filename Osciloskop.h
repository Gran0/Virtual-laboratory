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

#define  PANEL_OSC                        1
#define  PANEL_OSC_LED                    2       /* control type: LED, callback function: (none) */
#define  PANEL_OSC_TEST_SWITCH            3       /* control type: binary, callback function: TEST_SWITCH_Toogle */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

#define  MENUBAR                          1
#define  MENUBAR_M_OSC1                   2
#define  MENUBAR_M_OSC1_I_OSC             3       /* callback function: MENU_OSC_Rozcestnik_OnClick */
#define  MENUBAR_M_OSC1_I_OSC2            4       /* callback function: MENU_OSC_Voltmetr_OnClick */
#define  MENUBAR_M_OSC1_I_OSC3            5       /* callback function: MENU_OSC_Generator_OnClick */


     /* Callback Prototypes: */

void CVICALLBACK MENU_OSC_Generator_OnClick(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK MENU_OSC_Rozcestnik_OnClick(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK MENU_OSC_Voltmetr_OnClick(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK TEST_SWITCH_Toogle(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif