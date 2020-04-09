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

#define  PANEL                            1       /* callback function: panelRozcestnik */
#define  PANEL_AppTitle                   2       /* control type: textMsg, callback function: (none) */
#define  PANEL_TextPoweredBy              3       /* control type: textMsg, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

#define  MENUBAR                          1
#define  MENUBAR_MENU1                    2
#define  MENUBAR_MENU1_ITEM2              3       /* callback function: ITEM_Osciloskop_OnClick */
#define  MENUBAR_MENU1_ITEM3              4       /* callback function: ITEM_Voltmetr_OnClick */
#define  MENUBAR_MENU1_ITEM4              5       /* callback function: ITEM_Generator_OnClick */
#define  MENUBAR_MENU3                    6
#define  MENUBAR_MENU3_ITEM6              7       /* callback function: ITEM_Autor_OnClick */


     /* Callback Prototypes: */

void CVICALLBACK ITEM_Autor_OnClick(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK ITEM_Generator_OnClick(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK ITEM_Osciloskop_OnClick(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK ITEM_Voltmetr_OnClick(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK panelRozcestnik(int panel, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif