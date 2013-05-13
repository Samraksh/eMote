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
        public MACBase macObject;

        public ReceiveCallBack rcallback;

        public NeighbourhoodChangeCallBack ncallback;

        public MacConfiguration macconfig;

        Samraksh.SPOT.Net.Message incomingMessage;

        UInt16 myAddress;

        UInt16 numberOfMessagesToProcess;

        SynchronizedVariable processMessage;

        Timer NetworkNodeListRefresher;

        UInt16 theMsgNoFromThisNodeSendingMac = 1;

        Networking nw;

        MessagePayload currentMessage = new MessagePayload();

        MessagePayload lastMessage = new MessagePayload();

        MessagePayload refreshMessage = new MessagePayload();
        byte[] refreshMessageInBytes;

        Timer Visualizer;

        NetworkNodeList nwNodeList;

        public void HandleMessage(UInt16 numberOfPackets)
        {

            // The basestation may not have been initialized at this point 
            if (processMessage == null)
                return;

            if(!processMessage.IsReady())
                processMessage.SetState(true);

        }

        public void HandleNeighbourChanged(UInt16 numberOfNeighboursChanged)
        {
        }

        public void BuildNodeListRefreshMessage()
        {
            UInt16 contentLength = 0;

            //refreshMessage = new MessagePayload();

            refreshMessage.senderMac = myAddress;
            refreshMessage.origSenderMac = myAddress;
            refreshMessage.msgPType = MessagePayloadType.RteInfoReqFromBase;
            refreshMessage.msgDestinationType = MessageDestinationType.AllNodes;
            refreshMessage.msgAuthType = MessageAuthorType.BaseStationNodeAuthor;
            refreshMessage.intendedRecMacIsSpecified = IntendedRecipientMacIsSpecified.No;
            refreshMessage.intendedRecipientMacAddress = 0;
            refreshMessage.msgNoFromSenderMac = theMsgNoFromThisNodeSendingMac;
            theMsgNoFromThisNodeSendingMac++;
            refreshMessage.messageContent[contentLength++] = 0xff;
            refreshMessage.messageContent[contentLength++] = 0xff;

            refreshMessage.messageContentLength = contentLength;
            refreshMessage.msgAuthFuncType = MessageAuthorFunctionType.BaseStationNodeFcn;

            refreshMessageInBytes = refreshMessage.MarshallIntoBytes();
        }

        public BaseStation()
        {

            macconfig = new MacConfiguration();

            // Go to lowest power mode and see what happens 
            //macconfig.radioConfig.SetTxPower(SPOT.Net.Radio.TxPowerValue.Power_Minus17dBm);

            rcallback = HandleMessage;

            ncallback = HandleNeighbourChanged;

            MACBase.Configure(macconfig, rcallback, ncallback);

            try
            {
                macObject = CSMA.Instance;
            }
            catch (MacNotConfiguredException e)
            {
                Debug.Print("The Mac has not been configured \n");
            }

            myAddress = macObject.GetAddress();

            Debug.Print("The Base Station ID is : " + myAddress + "\n");

            processMessage = new SynchronizedVariable();

            BuildNodeListRefreshMessage();

            nw = new Networking(macObject);

            // Maintains information  about all the nodes 
            nwNodeList = new NetworkNodeList();


            // Refresh the node list every 1s initially, can change this after app is stable
            NetworkNodeListRefresher = new Timer(NodeListRefresh, null, 0, 13331);



            Visualizer = new Timer(Visualize, null, 4000, 40000);

            

            

        }

        public void Visualize(Object state)
        {
            Node[] nodeList = nwNodeList.GetList();
            string function = "";
            UInt16 numberOfMembers = nwNodeList.getNumberOfMembers();

            if (numberOfMembers == 0)
            {
                Debug.Print("No nodes in the network\n");
                return;
            }

            Debug.Print("\n");
            Debug.Print("===========================================================================");
            Debug.Print("Node Address\t|\tFunction\t|\tNode Status\t|\tAlarm Count");
            Debug.Print("===========================================================================");
            

            for (int i = 0; i < numberOfMembers; i++)
            {
                if (nodeList[i].status == NodeStatus.Active)
                {

                    switch (nodeList[i].function)
                    {
                        case MessageAuthorFunctionType.AmmunitionNodeFcn:
                            function = "Ammunition";
                            break;

                        case MessageAuthorFunctionType.FoodNodeFcn:
                            function = "Food";
                            break;

                        case MessageAuthorFunctionType.MedicalNodeFcn:
                            function = "Medicine";
                            break;

                        default:
                            function = "Unknown";
                            break;
                    }

                    Debug.Print(nodeList[i].macaddress.ToString() + "\t|\t" + function + "\t|\t Active \t|\t" + nodeList[i].alarmCount.ToString());
                    Debug.Print("\n");
                }
            }
        }

        public void ProcessRteInfoRespToBase(MessagePayload mp)
        {
          
            UInt16 numberOfNodes = 0;

            UInt16 nodeId = 0;

            for (int i = 0; i < mp.messageContentLength;)
            {
                nodeId = mp.messageContent[i++];
                nodeId |= (UInt16) (mp.messageContent[i++] << 8);

                if(nodeId != myAddress && nodeId != 0)
                    nwNodeList.Add(nodeId);
            }

            

            

        }

        // Handler that is called to refresh the node list maintained by the base station 
        public void NodeListRefresh(Object state)
        {
            //Debug.Print("Sending Node List Refresh Messages");
            // Send node list refresh message three times 
            nw.sendingMsg(refreshMessageInBytes, 0, refreshMessage.messageLength);
            refreshMessage.msgNoFromSenderMac = theMsgNoFromThisNodeSendingMac;
            theMsgNoFromThisNodeSendingMac++;
            
        }

        public void Run()
        {

            Debug.EnableGCMessages(false);

            while (true)
            {
                // Check if a radio message recieved interrupt happened 
                if (processMessage.IsReady())
                {
                    // Mac message handle

                    // Process all the packets in the buffer before going to sleep 
                    // This will ensure that any interrupts that came in from the radio during the processing are not missed out 
                    // as we are processing everything in the buffer 
                    while (macObject.GetPendingPacketCount() > 0)
                    {
                        // Grab next packet from the native buffer
                        incomingMessage = macObject.GetNextPacket();

                        if (incomingMessage != null)
                        {
                            // Get the payload 
                            currentMessage.Flush();
                            currentMessage.BuildMessage(incomingMessage.GetMessage());

                            // Check if we have already processed this message, if so ignore it , overloaded == operator 
                            if (lastMessage.senderMac == currentMessage.senderMac && lastMessage.msgNoFromSenderMac == currentMessage.msgNoFromSenderMac)
                                continue;

                            // Process the packet that came in 
                            switch (currentMessage.msgPType)
                            {
                                case MessagePayloadType.CriticalValueExceeded:
                                    nwNodeList.UpdateNodeStats(currentMessage.senderMac, currentMessage.msgAuthFuncType, NodeStatus.Active);
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
                                    ProcessRteInfoRespToBase(currentMessage);
                                    break;

                                case MessagePayloadType.SnapShotRequest:
                                    break;

                                case MessagePayloadType.SnapShotResponse:
                                    break;

                                default:
                                    Debug.Print("Unknown Message Type\n");
                                    break;
                            }

                            lastMessage.Flush();
                            // Assign the processed packet as the last packet 
                            lastMessage.CopyMessage(currentMessage);
                        }

                        //Debug.GC(true);
                        
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
