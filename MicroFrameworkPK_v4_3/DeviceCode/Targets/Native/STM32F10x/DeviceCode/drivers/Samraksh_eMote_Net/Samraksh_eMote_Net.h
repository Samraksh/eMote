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
struct Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MACBase
{
    static const int FIELD_STATIC__DataBuffer = 0;
    static const int FIELD_STATIC___neighbor = 1;
    static const int FIELD_STATIC___macPipeHashtable = 2;
    static const int FIELD_STATIC___csmaInstanceSet = 3;
    static const int FIELD_STATIC___omacInstanceSet = 4;

    static const int FIELD___neighborListTemp = 5;
    static const int FIELD___byteNeighbor = 6;
    static const int FIELD___marshalBuffer = 7;
    static const int FIELD___packet = 8;
    static const int FIELD__OnReceiveAll = 9;
    static const int FIELD__OnReceive = 10;
    static const int FIELD__OnNeighborChange = 11;
    static const int FIELD___cca = 12;
    static const int FIELD___numberOfRetries = 13;
    static const int FIELD___ccaSenseTime = 14;
    static const int FIELD___bufferSize = 15;
    static const int FIELD___neighborLivenessDelay = 16;
    static const int FIELD__MACType = 17;
    static const int FIELD__MACRadioObj = 18;

    TINYCLR_NATIVE_DECLARE(RemovePacket___SamraksheMoteNetDeviceStatus__SZARRAY_U1);
    TINYCLR_NATIVE_DECLARE(PendingReceivePacketCount___U1);
    TINYCLR_NATIVE_DECLARE(PendingSendPacketCount___U1);
    TINYCLR_NATIVE_DECLARE(Send___SamraksheMoteNetNetOpStatus__U2__SZARRAY_U1__U2__U2);
    TINYCLR_NATIVE_DECLARE(UnInitialize___SamraksheMoteNetDeviceStatus);
    TINYCLR_NATIVE_DECLARE(InternalInitialize___SamraksheMoteNetDeviceStatus__SZARRAY_U1__U1);
    TINYCLR_NATIVE_DECLARE(InternalReConfigure___SamraksheMoteNetDeviceStatus__SZARRAY_U1);
    TINYCLR_NATIVE_DECLARE(GetNextPacket___SamraksheMoteNetDeviceStatus__SZARRAY_U1);
    TINYCLR_NATIVE_DECLARE(GetNeighborInternal___SamraksheMoteNetDeviceStatus__U2__SZARRAY_U1);
    TINYCLR_NATIVE_DECLARE(GetNeighborListInternal___SamraksheMoteNetDeviceStatus__SZARRAY_U2);
    TINYCLR_NATIVE_DECLARE(Send___SamraksheMoteNetNetOpStatus__U2__SZARRAY_U1__U2__U2__U4);
    TINYCLR_NATIVE_DECLARE(Send___SamraksheMoteNetNetOpStatus__U2__U1__SZARRAY_U1__U2__U2);
    TINYCLR_NATIVE_DECLARE(Send___SamraksheMoteNetNetOpStatus__U2__U1__SZARRAY_U1__U2__U2__U4);

    //--//

};

struct Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MAC_Link
{
    static const int FIELD__AverageRSSI = 1;
    static const int FIELD__LinkQuality = 2;
    static const int FIELD__AverageDelay = 3;


    //--//

};

struct Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MAC_Neighbor
{
    static const int FIELD__MACAddress = 1;
    static const int FIELD__ForwardLink = 2;
    static const int FIELD__ReverseLink = 3;
    static const int FIELD__Status = 4;
    static const int FIELD__PacketsReceived = 5;
    static const int FIELD__LastHeardTime = 6;
    static const int FIELD__ReceiveDutyCycle = 7;
    static const int FIELD__FrameLength = 8;


    //--//

};

struct Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MACPipe
{
    static const int FIELD__MACBase = 1;
    static const int FIELD__PayloadType = 2;
    static const int FIELD__OnReceive = 3;


    //--//

};

struct Library_Samraksh_eMote_Net_Samraksh_eMote_Net_Packet
{
    static const int FIELD__RSSI = 1;
    static const int FIELD__LQI = 2;
    static const int FIELD__Src = 3;
    static const int FIELD__IsUnicast = 4;
    static const int FIELD__Payload = 5;
    static const int FIELD__PayloadType = 6;
    static const int FIELD__Size = 7;
    static const int FIELD__SenderEventTimeStamp = 8;
    static const int FIELD__IsPacketTimeStamped = 9;


    //--//

};

struct Library_Samraksh_eMote_Net_Samraksh_eMote_Net_Radio_RF231LRRadioConfiguration
{
    static const int FIELD__TxPower = 1;
    static const int FIELD__Channel = 2;
    static const int FIELD__RadioName = 3;


    //--//

};

struct Library_Samraksh_eMote_Net_Samraksh_eMote_Net_Radio_RF231RadioConfiguration
{
    static const int FIELD__TxPower = 1;
    static const int FIELD__Channel = 2;
    static const int FIELD__RadioName = 3;


    //--//

};

struct Library_Samraksh_eMote_Net_Samraksh_eMote_Net_Radio_Radio_802_15_4_Base
{
    static const int FIELD_STATIC__CurrUser = 5;

    static const int FIELD___radioAddress = 1;
    static const int FIELD___txPower = 2;
    static const int FIELD___channel = 3;
    static const int FIELD___radioName = 4;

    TINYCLR_NATIVE_DECLARE(UnInitialize___SamraksheMoteNetDeviceStatus);
    TINYCLR_NATIVE_DECLARE(TurnOnRx___SamraksheMoteNetDeviceStatus__U1);
    TINYCLR_NATIVE_DECLARE(Sleep___SamraksheMoteNetDeviceStatus__U1__U1);
    TINYCLR_NATIVE_DECLARE(ClearChannelAssesment___BOOLEAN__U1);
    TINYCLR_NATIVE_DECLARE(ClearChannelAssesment___BOOLEAN__U1__U2);
    TINYCLR_NATIVE_DECLARE(PreLoad___SamraksheMoteNetNetOpStatus__SZARRAY_U1__U2);
    TINYCLR_NATIVE_DECLARE(SendStrobe___SamraksheMoteNetNetOpStatus__U1__U2);
    TINYCLR_NATIVE_DECLARE(Send___SamraksheMoteNetNetOpStatus__U1__SZARRAY_U1__U2);
    TINYCLR_NATIVE_DECLARE(SendTimeStamped___SamraksheMoteNetNetOpStatus__U1__SZARRAY_U1__U2__U4);
    TINYCLR_NATIVE_DECLARE(SetRadioName___SamraksheMoteNetDeviceStatus__U1);
    TINYCLR_NATIVE_DECLARE(SetTxPower___SamraksheMoteNetDeviceStatus__U1__I4);
    TINYCLR_NATIVE_DECLARE(SetChannel___SamraksheMoteNetDeviceStatus__U1__I4);
    TINYCLR_NATIVE_DECLARE(GetRadioAddress___U2__U1);
    TINYCLR_NATIVE_DECLARE(SetRadioAddress___BOOLEAN__U1__U2);
    TINYCLR_NATIVE_DECLARE(TurnOffRx___SamraksheMoteNetDeviceStatus__U1);

    //--//

};

struct Library_Samraksh_eMote_Net_Samraksh_eMote_Net_Radio_Radio_802_15_4_RF231
{
    static const int FIELD_STATIC___genericRadioInstanceSet = 6;


    //--//

};

struct Library_Samraksh_eMote_Net_Samraksh_eMote_Net_Radio_Radio_802_15_4_RF231_LR
{
    static const int FIELD_STATIC___lrRadioInstanceSet = 7;


    //--//

};

struct Library_Samraksh_eMote_Net_Samraksh_eMote_Net_Radio_Radio_802_15_4_SI4468
{
    static const int FIELD_STATIC___si4468RadioInstanceSet = 8;


    //--//

};

struct Library_Samraksh_eMote_Net_Samraksh_eMote_Net_Radio_SI4468RadioConfiguration
{
    static const int FIELD__TxPower = 1;
    static const int FIELD__Channel = 2;
    static const int FIELD__RadioName = 3;


    //--//

};


extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_CSMACallback;
extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_OMACCallback;
extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_MACCallback;
extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_RadioCallback_802_15_4;

extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_Samraksh_eMote_Net;

#endif  //_SAMRAKSH_EMOTE_NET_H_
