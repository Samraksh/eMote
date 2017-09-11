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
    static const int FIELD__<MACType>k__BackingField = 8;
    static const int FIELD__OnReceiveAll = 9;
    static const int FIELD__OnReceive = 10;
    static const int FIELD__OnNeighborChange = 11;
    static const int FIELD__OnSendStatus = 12;
    static const int FIELD___cca = 13;
    static const int FIELD___numberOfRetries = 14;
    static const int FIELD___ccaSenseTime = 15;
    static const int FIELD___bufferSize = 16;
    static const int FIELD___neighborLivenessDelay = 17;

    TINYCLR_NATIVE_DECLARE(RemovePacket___SamraksheMoteNetDeviceStatus);
    TINYCLR_NATIVE_DECLARE(PendingReceivePacketCount___U1);
    TINYCLR_NATIVE_DECLARE(PendingSendPacketCount___U1);
    TINYCLR_NATIVE_DECLARE(Send___SamraksheMoteNetNetOpStatus__U2__SZARRAY_U1__U2__U2);
    TINYCLR_NATIVE_DECLARE(UnInitialize___SamraksheMoteNetDeviceStatus);
    TINYCLR_NATIVE_DECLARE(InternalInitialize___SamraksheMoteNetDeviceStatus__SZARRAY_U1__U1);
    TINYCLR_NATIVE_DECLARE(InternalReConfigure___SamraksheMoteNetDeviceStatus__SZARRAY_U1);
    TINYCLR_NATIVE_DECLARE(GetNextPacket___SamraksheMoteNetDeviceStatus__SZARRAY_U1);
    TINYCLR_NATIVE_DECLARE(GetNeighborInternal___SamraksheMoteNetDeviceStatus__U2__SZARRAY_U1);
    TINYCLR_NATIVE_DECLARE(GetNeighborListInternal___SamraksheMoteNetDeviceStatus__SZARRAY_U2);
    TINYCLR_NATIVE_DECLARE(GetMACNeighborListInternal___SamraksheMoteNetDeviceStatus__SZARRAY_U2);
    TINYCLR_NATIVE_DECLARE(DeletePacketWithIndexInternal___SamraksheMoteNetDeviceStatus__U2);
    TINYCLR_NATIVE_DECLARE(GetPacketWithIndex___SamraksheMoteNetDeviceStatus__SZARRAY_U1__U1__U2);
    TINYCLR_NATIVE_DECLARE(GetPacketSizeWithIndex___SamraksheMoteNetDeviceStatus__BYREF_U1__U2);
    TINYCLR_NATIVE_DECLARE(EnqueueToSend___U2__U2__U1__SZARRAY_U1__U2__U2);
    TINYCLR_NATIVE_DECLARE(EnqueueToSend___U2__U2__U1__SZARRAY_U1__U2__U2__U4);
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
    static const int FIELD__SendLink = 2;
    static const int FIELD__ReceiveLink = 3;
    static const int FIELD__NeighborStatus = 4;
    static const int FIELD__IsAvailableForUpperLayers = 5;
    static const int FIELD__NumTimeSyncMessagesSent = 6;
    static const int FIELD__NumOfTimeSamplesRecorded = 7;
    static const int FIELD__LastHeardTime = 8;
    static const int FIELD__ReceiveDutyCycle = 9;
    static const int FIELD__FrameLength = 10;


    //--//

};

struct Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MACPipe
{
    static const int FIELD__MACBase = 1;
    static const int FIELD__PayloadType = 2;
    static const int FIELD__OnReceive = 3;
    static const int FIELD__OnSendStatus = 4;
    static const int FIELD__msg_id_list = 5;
    static const int FIELD__msg_id_list_ro = 6;


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
    static const int FIELD__<TxPower>k__BackingField = 1;
    static const int FIELD__<Channel>k__BackingField = 2;
    static const int FIELD__<RadioName>k__BackingField = 3;


    //--//

};

struct Library_Samraksh_eMote_Net_Samraksh_eMote_Net_Radio_RF231RadioConfiguration
{
    static const int FIELD__<TxPower>k__BackingField = 1;
    static const int FIELD__<Channel>k__BackingField = 2;
    static const int FIELD__<RadioName>k__BackingField = 3;


    //--//

};

struct Library_Samraksh_eMote_Net_Samraksh_eMote_Net_Radio_SI4468RadioConfiguration
{
    static const int FIELD__<TxPower>k__BackingField = 1;
    static const int FIELD__<Channel>k__BackingField = 2;
    static const int FIELD__<RadioName>k__BackingField = 3;


    //--//

};



extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_Samraksh_eMote_Net;

#endif  //_SAMRAKSH_EMOTE_NET_H_
