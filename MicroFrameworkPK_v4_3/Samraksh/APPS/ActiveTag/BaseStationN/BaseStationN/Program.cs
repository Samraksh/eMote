using System;
using Microsoft.SPOT;

using Samraksh.SPOT.Net;
using Samraksh.SPOT.Net.Mac;
using System.Threading;

namespace Samraksh.APPS.ActiveTag
{
    public enum MessageDestinationType : byte
    {
        BaseStation,
        LocalNeighborhoodNode,
        SpecificNonBaseNode,  //use when intended destniation is *not* a local neighbor node
        AllNodes,
    } 


    public enum MessagePayloadType : ushort
    {
        SnapShotRequest,
        SnapShotResponse,
        CriticalValueExceeded,
        NodeLeftNeighborhood,
        AreYouAliveRequest,
        AreYouAliveResponse,
        RteInfoReqFromBase,
        RteInfoRespToBase,
    } 

    public enum MessageStrings : byte
    {
        IAmAlive,
        AreYouAlive,
        SendSnapshotRequest,
        SendRteInfoRequest,
    } 

    public enum MessageAuthorType : byte
    {
        BaseStationNodeAuthor,
        NeighborNodeAuthor,
        SpecificNodeAuthor,
    } 

    public enum MessageAuthorFunctionType : byte
    {
        BaseStationNodeFcn,
        FoodNodeFcn,
        MedicalNodeFcn,
        AmmunitionNodeFcn,
    } 

    public enum TagFunctionType : byte
    {
        FoodFcn,
        MedicalNodeFcn,
        AmmunitionNodeFcn,
    }

    public enum IntendedRecipientMacIsSpecified : byte
    {
        Yes,
        No,
    } 

    public class BaseStation
    {
        public CSMA csmaObject;

        public ReceiveCallBack rcallback;

        public NeighbourhoodChangeCallBack ncallback;

        public MacConfiguration macconfig;

        UInt16 myAddress;

        UInt16 numberOfMessagesToProcess;

        SynchronizedVariable processMessage;

        public void HandleMessage(UInt16 numberOfPackets)
        {
            numberOfMessagesToProcess = numberOfPackets;

            processMessage.SetState(true);

        }

        public void HandleNeighbourChanged(UInt16 numberOfNeighboursChanged)
        {
        }

        public BaseStation()
        {

            macconfig = new MacConfiguration();

            rcallback = HandleMessage;

            ncallback = HandleNeighbourChanged;

            CSMA.Configure(macconfig, rcallback, ncallback);

            try
            {
                csmaObject = CSMA.Instance;
            }
            catch (MacNotConfiguredException e)
            {
                Debug.Print("The Mac has not been configured \n");
            }

            myAddress = csmaObject.GetAddress();

            Debug.Print("The Base Station ID is : " + myAddress + "\n");

            processMessage = new SynchronizedVariable();
   
        }

        public void Run()
        {
            while (true)
            {
                if (processMessage.IsReady())
                {
                    Samraksh.SPOT.Net.Message incomingMessage;

                    incomingMessage = csmaObject.GetNextPacket();

                    if (incomingMessage != null)
                    {
                        MessagePayload mp = new MessagePayload(incomingMessage.GetMessage());

                        switch (mp.msgPType)
                        {
                            case MessagePayloadType.CriticalValueExceeded:
                                Debug.Print("Temperature Alarm on Node " + mp.senderMac.ToString());
                                break;

                            case MessagePayloadType.AreYouAliveRequest:
                                break;

                            case MessagePayloadType.AreYouAliveResponse:
                                break;

                            case MessagePayloadType.NodeLeftNeighborhood:
                                break;

                            case MessagePayloadType.RteInfoReqFromBase:
                                break;

                            case MessagePayloadType.RteInfoRespToBase:
                                break;

                            case MessagePayloadType.SnapShotRequest:
                                break;

                            case MessagePayloadType.SnapShotResponse:
                                break;

                            default:
                                Debug.Print("Unknown Message Type\n");
                                break;
                        }

                    }

                    processMessage.SetState(false);
                }

                Thread.Sleep(500);
            }
        }


    }

    public class Program
    {
        public static void Main()
        {
            BaseStation bs = new BaseStation();

            bs.Run();

        }

    }
}
