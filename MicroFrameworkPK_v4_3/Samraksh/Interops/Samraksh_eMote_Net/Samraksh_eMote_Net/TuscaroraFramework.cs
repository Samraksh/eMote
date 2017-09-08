using System;
using Microsoft.SPOT;

namespace Samraksh.eMote.Net.Tuscarora
{
    //North bound interface
    class PatternInterface
    {
        typedef PatternEvent<PTN_CONT_RES_EVT, ControlResponseParam> PTN_ControlResponseEvent_t;

        ///Event sent to Pattern to notify it of updates in the neighborhood
        typedef PatternEvent<PTN_NBR_CHG_EVT, NeighborUpdateParam> PTN_NeighborUpdateEvent_t;

        ///Event sent to Pattern when a new message is received on one of the waveforms for the pattern
        typedef PatternEvent<PTN_RECV_MSG_EVT, FMessage_t&> PTN_ReceivedMessageEvent_t;

        ///Event sent to the pattern to notify about the status of message the pattern sent to the framework
        typedef PatternEvent<PTN_DATA_NTY_EVT, DataStatusParam> PTN_DatNotificationEvent_t;


        typedef Delegate<void, FMessage_t&> RecvMessageDelegate_t;
        typedef Delegate<void, ControlResponseParam> ControlResponseDelegate_t;



        virtual void SendData(PatternId_t pid, NodeId_t destArray[MAX_DEST], uint16_t noOfDest, FMessage_t& msg, uint16_t nonce, bool noDestAck = false) = 0;
        virtual void SendData(PatternId_t pid, NodeId_t destArray[MAX_DEST], uint16_t noOfDest, LinkComparatorTypeE lcType, FMessage_t& msg, uint16_t nonce, bool noDestAck = false) = 0;
 
        virtual void ReplacePayloadRequest(PatternId_t patternId, FMessageId_t msgId, void* payload, uint16_t sizeOfPayload) = 0 ;
        virtual void RegisterPatternRequest(PatternId_t patternId, const char uniqueName[128], PatternTypeE type) = 0;
        virtual void AddDestinationRequest(PatternId_t patternId, FMessageId_t msgId, NodeId_t destArray[MAX_DEST], uint16_t noOfNbrs) = 0;
        virtual void AddDestinationRequest(PatternId_t patternId, FMessageId_t msgId, NodeId_t destArray[MAX_DEST], uint16_t noOfNbrs, LinkComparatorTypeE lcType) = 0;
        virtual void CancelDataRequest(PatternId_t patternId, FMessageId_t msgId, NodeId_t destArray[MAX_DEST], uint16_t noOfDest) = 0;

        virtual void DataStatusRequest(PatternId_t patternId, FMessageId_t msgId) = 0;
        virtual void SelectDataNotificationRequest(PatternId_t patternId, uint8_t notifierMask) = 0;
        virtual void SetLinkThresholdRequest(PatternId_t patternId, LinkMetrics threshold) = 0;

        virtual void FrameworkAttributesRequest(PatternId_t patternId) = 0;
        virtual void SelectLinkComparatorRequest(PatternId_t patternId, LinkComparatorTypeE lcType) = 0;

    }

    //South bound interface. Do we need to expose this to C# user?
    /*class WaveformInterface
    {

    }*/
}
