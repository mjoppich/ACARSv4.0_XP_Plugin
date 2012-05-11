/*
SDK210Tests Example
Tests the new SDK 2.10 functions.

Version 1.0.0.1		Intitial Sandy Barbour 25/01/2012

NOTES
1.
This example will only work with the new plugin SDK 2.10 and will only run in X-Plane V10.

2.
The 2.10 plugin SDK now has 64 bit safe variables.

Unfortunately early Micrsoft Visual Studio products do not have the required header files to support this.
I know that VS6 does not and I believe that 2003 does not either.
VS2008 onwards is fine and that is what I tested this example with on windows.

So, any VS6 users will need to try and located a header that will allow the example to compile.
This is a MS problem and not an SDK problem.

I did find this header that worked.

http://www.azillionmonkeys.com/qed/pstdint.h

But we will not be supporting it or any other headers usage in this example.
*/

//---------------------------------------------------------------------------

#include "XPLMPlugin.h"
#include "XPLMDisplay.h"
#include "XPLMProcessing.h"
#include "XPLMDataAccess.h"
#include "XPLMUtilities.h"
#include "XPLMScenery.h"
#include "XPLMPlanes.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#if IBM
#include <windows.h>
#include <stdio.h>
#endif

//---------------------------------------------------------------------------

enum
{
/*0*/	MAIN_FLCB = 0,
/*1*/	TEMP_FLCB,
/*2*/	INFO,
/*3*/	MENU_2,
/*4*/	DRAWING
};

enum
{
/*0*/	USER_AIRCRAFT = 0,
/*1*/	AI_AIRCRAFT
};

char SDK210TestsVersionNumber[] = "v1.00";

char SDK210Tests_Buffer[256];
XPLMFlightLoopID SDK210Tests_XPLMTempFlightLoopID = NULL;

// Datarefs
XPLMDataRef	SDK210Tests_refx = NULL;
XPLMDataRef	SDK210Tests_refy = NULL;
XPLMDataRef	SDK210Tests_refz = NULL;
XPLMDataRef	SDK210Tests_wrl_type = NULL;


// Used by the Draw Objects test
XPLMObjectRef	SDK210TestsObject=NULL;
int SDK210TestsDrawingEnabled = 0;

// Used by the FLCB test
int	SDK210TestsMainLoopCBCounter = 0;
int	SDK210TestsTempLoopCBCounter = 0;

char *pSDK210TestsAircraft[2];
char SDK210TestsAircraftPath[2][512];

//---------------------------------------------------------------------------

// Displays data in widget
float SDK210TestsMainLoopCB(float elapsedMe, float elapsedSim, int counter, void * refcon);

// Callback for Error Tests
void	SDK210TestsErrorCB(const char * msg)
{
    XPLMDebugString("================================================================\n");
    XPLMDebugString("SDK210Tests: SDK210TestsErrorCB - error CB called: ");
    XPLMDebugString(msg);
    XPLMDebugString("\n");
    XPLMDebugString("----------------------------------------------------------------\n");
}


//---------------------------------------------------------------------------

PLUGIN_API int XPluginStart(
                        char *		outName,
                        char *		outSig,
                        char *		outDesc)
{
    strcpy(outName, "SDK210Tests");
    strcpy(outSig, "sandybarbour.projects.SDK210Tests");
    strcpy(outDesc, "A plug-in to test the new SDK210 API's.");

    // Register the callback for errors
    XPLMSetErrorCallback(SDK210TestsErrorCB);

    // Datarefs to get the aicraft position
    SDK210Tests_refx = XPLMFindDataRef("sim/flightmodel/position/local_x");
    SDK210Tests_refy = XPLMFindDataRef("sim/flightmodel/position/local_y");
    SDK210Tests_refz = XPLMFindDataRef("sim/flightmodel/position/local_z");

    XPLMRegisterFlightLoopCallback(SDK210TestsMainLoopCB, 1.0, NULL);

    return 1;
}

PLUGIN_API void	XPluginStop(void)
{
    // Clean up
    XPLMUnregisterFlightLoopCallback(SDK210TestsMainLoopCB, NULL);
}

PLUGIN_API int XPluginEnable(void)
{
    return 1;
}

PLUGIN_API void XPluginDisable(void)
{
}

PLUGIN_API void XPluginReceiveMessage(XPLMPluginID inFrom, long inMsg, void * inParam)
{
    switch(inMsg)
    {
        case XPLM_MSG_WILL_WRITE_PREFS:
            XPLMDebugString("================================================================\n");
            XPLMDebugString("SDK210Tests: XPluginReceiveMessage - Will write prefs.\n");
            XPLMDebugString("----------------------------------------------------------------\n");
            break;
        case XPLM_MSG_LIVERY_LOADED:
            XPLMDebugString("================================================================\n");
            sprintf(SDK210Tests_Buffer,"SDK210Tests: XPluginReceiveMessage - Livery loaded for plane %d.\n", inParam);
            XPLMDebugString(SDK210Tests_Buffer);
            XPLMDebugString("----------------------------------------------------------------\n");
            break;
    }
}

//---------------------------------------------------------------------------

// Used to test the creation and destruction of a Flightloop Callback from within a Flightloop Callback.
float SDK210TestsMainLoopCB(float elapsedMe, float elapsedSim, int counter, void * refcon)
{



    return 1.0;
}
