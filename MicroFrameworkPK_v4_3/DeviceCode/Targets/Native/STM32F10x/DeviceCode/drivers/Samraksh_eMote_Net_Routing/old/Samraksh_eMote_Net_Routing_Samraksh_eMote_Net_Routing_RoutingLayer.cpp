//-----------------------------------------------------------------------------
//
//                   ** WARNING! ** 
//    This file was generated automatically by a tool.
//    Re-running the tool will overwrite this file.
//    You should copy this file to a custom location
//    before adding any customization in the copy to
//    prevent loss of your changes when the tool is
//    re-run.
//
//-----------------------------------------------------------------------------


#include "Samraksh_eMote_Net_Routing.h"
#include "Samraksh_eMote_Net_Routing_Samraksh_eMote_Net_Routing_RoutingLayer.h"
#include <Samraksh\RoutingEngine.h>

using namespace Samraksh_eMote_Net_Routing;

INT32 RoutingLayer::SendRoutingData( CLR_RT_TypedArray_UINT8 message, HRESULT &hr )
{
	CtpDataMessage msg;
	if(RoutingEngine::SendData(msg)==DS_Success)
		return 0;
    else
		return -1;
}

INT32 RoutingLayer::SendRoutingBeacon( HRESULT &hr )
{
    if(RoutingEngine::SendBeacon()==DS_Success)
		return 0;
    else
		return -1;
}


INT32 RoutingLayer::InitializeRouting( CLR_RT_TypedArray_UINT8 beaconMessage, UINT16 routingAddress, INT32 radioType, HRESULT &hr )
{
	CtpRoutingBeaconMessage beacon;
	//INT32 radio = RF231RADIOLR;
	INT32 radio = RF231RADIO;
	if(RoutingEngine::InitializeRoutingLayer(&beacon, routingAddress, radioType)==DS_Success)
		return 0;
	else
		return -1;
}

