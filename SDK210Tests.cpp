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

#define WIN32_LEAN_AND_MEAN

#ifdef __cplusplus
extern "C" {
#endif

#pragma comment(lib, "ws2_32.lib") // einbinden der ws2_32.lib
#pragma comment(lib, "XPLM.lib") // einbinden der ws2_32.lib


#include "XPLMPlugin.h"
#include "XPLMDisplay.h"
#include "XPLMProcessing.h"
#include "XPLMDataAccess.h"
#include "XPLMUtilities.h"
#include "XPLMScenery.h"
#include "XPLMPlanes.h"
#include <string.h>
#include <stdlib.h>

#if IBM
#include <WinSock2.h>

#include <windows.h>
#include <stdio.h>
#endif

//---------------------------------------------------------------------------

char PluginVersionNumber[] = "v0.1";

char StringBuffer[256];

//socket
  long rc;
  SOCKET s;
  char buf[256];
  SOCKADDR_IN addr;
  SOCKADDR_IN remoteAddr;
  int         remoteAddrLen=sizeof(SOCKADDR_IN);

// Datarefs POSITION
XPLMDataRef	xpLatitude = NULL;
XPLMDataRef	xpLongitude = NULL;
XPLMDataRef	xpAltitude = NULL;
XPLMDataRef	xpAltitudeAboveGround = NULL;

// Datarefs AC 

// Used by the FLCB test
int	MainLoopCBCounter = 0;

//---------------------------------------------------------------------------

// Displays data in widget
float SDK210TestsMainLoopCB(float elapsedMe, float elapsedSim, int counter, void * refcon);

// Callback for Error Tests
void	SDK210TestsErrorCB(const char * msg)
{
    XPLMDebugString("================================================================\n");
    XPLMDebugString("ACARS 4.0 XP Plugin - error CB called: ");
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
    strcpy(outName, "ACARSPLUGIN");
    strcpy(outSig, "mjoppich.acars.acarsv4");
    strcpy(outDesc, "ACARS plugin for forwarding sim data");


	WSADATA wsa;
	rc=WSAStartup(MAKEWORD(2,0),&wsa);

	  if(rc!=0)
	  {
		XPLMDebugString("Fehler: startWinsock, fehler code");
	  }
	  else
	  {
		XPLMDebugString("Winsock gestartet!\n");
	  }

	s=socket(AF_INET,SOCK_DGRAM,0);

	  if(s==INVALID_SOCKET)
	  {
		XPLMDebugString("Fehler: Der Socket konnte nicht erstellt werden");
	  }
	  else
	  {
		XPLMDebugString("UDP Socket erstellt!\n");
	  }

    // addr vorbereiten
	addr.sin_family=AF_INET;
	addr.sin_port=htons(7777);
	addr.sin_addr.s_addr=inet_addr("127.0.0.1");

	XPLMDebugString(outDesc);


    // Register the callback for errors
    XPLMSetErrorCallback(SDK210TestsErrorCB);

    // Datarefs to get the aicraft position
    xpLatitude = XPLMFindDataRef("sim/flightmodel/position/local_x");
    xpLongitude = XPLMFindDataRef("sim/flightmodel/position/local_y");
    xpAltitude = XPLMFindDataRef("sim/flightmodel/position/local_z");

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
            sprintf(StringBuffer,"SDK210Tests: XPluginReceiveMessage - Livery loaded for plane %d.\n", inParam);
            XPLMDebugString(StringBuffer);
            XPLMDebugString("----------------------------------------------------------------\n");
            break;
    }
}

//---------------------------------------------------------------------------

// Used to test the creation and destruction of a Flightloop Callback from within a Flightloop Callback.
float SDK210TestsMainLoopCB(float elapsedMe, float elapsedSim, int counter, void * refcon)
{

	sprintf(buf,"Du mich auch %i",counter);

    rc=sendto(s,buf,strlen(buf),0,(SOCKADDR*)&addr,sizeof(SOCKADDR_IN));

	if(rc==SOCKET_ERROR)
    {
      XPLMDebugString("nothing sent");
    }
    else
    {
		sprintf(buf,"%d Bytes gesendet in run %i !\n", rc, counter);
      XPLMDebugString(buf);
    }

    return 1.0;
}

#ifdef __cplusplus
}
#endif