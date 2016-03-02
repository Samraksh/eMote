//-----------------------------------------------------------------------------
//
//    ** DO NOT EDIT THIS FILE! **
//    This file was generated by a tool
//    re-running the tool will overwrite this file.
//
//-----------------------------------------------------------------------------


#ifndef _SAMRAKSH_EMOTE_NET_H_
#define _SAMRAKSH_EMOTE_NET_H_

#include <TinyCLR_Interop.h>
struct Library_Samraksh_eMote_Net_Samraksh_eMote_Net_Callbacks
{
    static const int FIELD_STATIC___receiveCallBack = 0;
    static const int FIELD_STATIC___neighborChangeCallBack = 1;
    static const int FIELD_STATIC___neighbourChangeCallBack = 2;


    //--//

};

struct Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MACBase
{
    static const int FIELD_STATIC__MACConfig = 3;
    static const int FIELD_STATIC__neighbor = 4;
    static const int FIELD_STATIC__dataBuffer = 5;
    static const int FIELD_STATIC__MACRadio = 6;

    static const int FIELD__NeighborList = 1;
    static const int FIELD__ByteNeighbor = 2;
    static const int FIELD__MarshalBuffer = 3;
    static const int FIELD__packet = 4;
    static const int FIELD__MACType = 5;

    TINYCLR_NATIVE_DECLARE(UnInitialize___SamraksheMoteNetDeviceStatus);
    TINYCLR_NATIVE_DECLARE(RemovePacket___SamraksheMoteNetDeviceStatus__SZARRAY_U1);
    TINYCLR_NATIVE_DECLARE(GetPendingPacketCount_Receive___U1);
    TINYCLR_NATIVE_DECLARE(GetPendingPacketCount_Send___U1);
    TINYCLR_NATIVE_DECLARE(GetMACType___U1);
    TINYCLR_NATIVE_DECLARE(SetRadioAddress___SamraksheMoteNetDeviceStatus__U2);
    TINYCLR_NATIVE_DECLARE(GetRadioAddress___U2);
    TINYCLR_NATIVE_DECLARE(Send___SamraksheMoteNetNetOpStatus__U2__SZARRAY_U1__U2__U2);
    TINYCLR_NATIVE_DECLARE(InternalInitialize___SamraksheMoteNetDeviceStatus__SZARRAY_U1__U1);
    TINYCLR_NATIVE_DECLARE(InternalReConfigure___SamraksheMoteNetDeviceStatus__SZARRAY_U1);
    TINYCLR_NATIVE_DECLARE(GetNextPacket___SamraksheMoteNetDeviceStatus__SZARRAY_U1);
    TINYCLR_NATIVE_DECLARE(GetNeighborInternal___SamraksheMoteNetDeviceStatus__U2__SZARRAY_U1);
    TINYCLR_NATIVE_DECLARE(GetNeighborListInternal___SamraksheMoteNetDeviceStatus__SZARRAY_U2);
    TINYCLR_NATIVE_DECLARE(SendTimeStamped___SamraksheMoteNetNetOpStatus__U2__SZARRAY_U1__U2__U2);
    TINYCLR_NATIVE_DECLARE(SendTimeStamped___SamraksheMoteNetNetOpStatus__U2__SZARRAY_U1__U2__U2__U4);

    //--//

};

struct Library_Samraksh_eMote_Net_Samraksh_eMote_Net_Mac_CSMA
{
    static const int FIELD_STATIC__CSMAInstance = 7;
    static const int FIELD_STATIC__syncObject = 8;


    //--//

};

struct Library_Samraksh_eMote_Net_Samraksh_eMote_Net_Mac_Link
{
    static const int FIELD__AveRSSI = 1;
    static const int FIELD__LinkQuality = 2;
    static const int FIELD__AveDelay = 3;


    //--//

};

struct Library_Samraksh_eMote_Net_Samraksh_eMote_Net_Mac_MACConfiguration
{
    static const int FIELD__CCA = 1;
    static const int FIELD__NumberOfRetries = 2;
    static const int FIELD__CCASenseTime = 3;
    static const int FIELD__BufferSize = 4;
    static const int FIELD__RadioType = 5;
    static const int FIELD__NeighborLivenessDelay = 6;
    static const int FIELD__MACRadioConfig = 7;


    //--//

};

struct Library_Samraksh_eMote_Net_Samraksh_eMote_Net_Mac_Neighbor
{
    static const int FIELD__MacAddress = 1;
    static const int FIELD__ForwardLink = 2;
    static const int FIELD__ReverseLink = 3;
    static const int FIELD__Status = 4;
    static const int FIELD__PacketsReceived = 5;
    static const int FIELD__LastHeardTime = 6;
    static const int FIELD__ReceiveDutyCycle = 7;
    static const int FIELD__FrameLength = 8;


    //--//

};

struct Library_Samraksh_eMote_Net_Samraksh_eMote_Net_Mac_NeighborTable
{
    static const int FIELD__NumberValidNeighbor = 1;
    static const int FIELD__Neighbor = 2;


    //--//

};

struct Library_Samraksh_eMote_Net_Samraksh_eMote_Net_Mac_Neighbour
{
    static const int FIELD__MacAddress = 1;
    static const int FIELD__ForwardLink = 2;
    static const int FIELD__ReverseLink = 3;
    static const int FIELD__Status = 4;
    static const int FIELD__PacketsReceived = 5;
    static const int FIELD__LastHeardTime = 6;
    static const int FIELD__ReceiveDutyCycle = 7;
    static const int FIELD__FrameLength = 8;


    //--//

};

struct Library_Samraksh_eMote_Net_Samraksh_eMote_Net_Mac_NeighbourTable
{
    static const int FIELD__NumberValidNeighbour = 1;
    static const int FIELD__Neighbor = 2;


    //--//

};

struct Library_Samraksh_eMote_Net_Samraksh_eMote_Net_Mac_OMAC
{
    static const int FIELD_STATIC__OMACInstance = 9;
    static const int FIELD_STATIC__syncObject = 10;


    //--//

};

struct Library_Samraksh_eMote_Net_Samraksh_eMote_Net_Packet
{
    static const int FIELD__RSSI = 1;
    static const int FIELD__LQI = 2;
    static const int FIELD__Src = 3;
    static const int FIELD__IsUnicast = 4;
    static const int FIELD__Payload = 5;
    static const int FIELD__Size = 6;
    static const int FIELD__SenderEventTimeStamp = 7;
    static const int FIELD__IsPacketTimeStamped = 8;


    //--//

};

struct Library_Samraksh_eMote_Net_Samraksh_eMote_Net_Radio_RadioConfiguration
{
    static const int FIELD__TxPower = 1;
    static const int FIELD__Channel = 2;
    static const int FIELD__RadioType = 3;
    static const int FIELD__OnReceiveCallback = 4;
    static const int FIELD__OnNeighborChangeCallback = 5;


    //--//

};

struct Library_Samraksh_eMote_Net_Samraksh_eMote_Net_Radio_Radio_802_15_4_Base
{
    static const int FIELD_STATIC__RadioBaseConfig = 11;
    static const int FIELD_STATIC__CurrUser = 12;

    static const int FIELD__dataBuffer = 5;
    static const int FIELD__packet = 6;
    static const int FIELD__marshalBuffer = 7;

    TINYCLR_NATIVE_DECLARE(UnInitialize___SamraksheMoteNetDeviceStatus);
    TINYCLR_NATIVE_DECLARE(GetAddress___U2__U1);
    TINYCLR_NATIVE_DECLARE(SetAddress___BOOLEAN__U1__U2);
    TINYCLR_NATIVE_DECLARE(TurnOnRx___SamraksheMoteNetDeviceStatus__U1);
    TINYCLR_NATIVE_DECLARE(Sleep___SamraksheMoteNetDeviceStatus__U1__U1);
    TINYCLR_NATIVE_DECLARE(PreLoad___SamraksheMoteNetNetOpStatus__SZARRAY_U1__U2);
    TINYCLR_NATIVE_DECLARE(SendStrobe___SamraksheMoteNetNetOpStatus__U1__U2);
    TINYCLR_NATIVE_DECLARE(Send___SamraksheMoteNetNetOpStatus__U1__SZARRAY_U1__U2);
    TINYCLR_NATIVE_DECLARE(SendTimeStamped___SamraksheMoteNetNetOpStatus__U1__SZARRAY_U1__U2__U4);
    TINYCLR_NATIVE_DECLARE(ClearChannelAssesment___BOOLEAN__U1);
    TINYCLR_NATIVE_DECLARE(ClearChannelAssesment___BOOLEAN__U1__U2);
    TINYCLR_NATIVE_DECLARE(InternalInitialize___SamraksheMoteNetDeviceStatus__SZARRAY_U1);
    TINYCLR_NATIVE_DECLARE(ReConfigure___SamraksheMoteNetDeviceStatus__SZARRAY_U1);
    TINYCLR_NATIVE_DECLARE(SetTxPower___SamraksheMoteNetDeviceStatus__U1__I4);
    TINYCLR_NATIVE_DECLARE(SetChannel___SamraksheMoteNetDeviceStatus__U1__I4);
    TINYCLR_NATIVE_DECLARE(GetTxPower___I4__U1);
    TINYCLR_NATIVE_DECLARE(GetActiveChannel___I4__U1);
    TINYCLR_NATIVE_DECLARE(TurnOffRx___SamraksheMoteNetDeviceStatus__U1);

    //--//

};

struct Library_Samraksh_eMote_Net_Samraksh_eMote_Net_Radio_Radio_802_15_4
{
    static const int FIELD_STATIC__GenericRadioInstance = 13;
    static const int FIELD_STATIC__syncObject = 14;


    //--//

};

struct Library_Samraksh_eMote_Net_Samraksh_eMote_Net_Radio_Radio_802_15_4_LR
{
    static const int FIELD_STATIC__LRRadioInstance = 15;
    static const int FIELD_STATIC__syncObject = 16;


    //--//

};



extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_Samraksh_eMote_Net;

#endif  //_SAMRAKSH_EMOTE_NET_H_
