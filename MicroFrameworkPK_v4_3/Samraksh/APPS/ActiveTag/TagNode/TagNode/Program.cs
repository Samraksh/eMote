using System;
using Microsoft.SPOT;
using System.Threading;
using Samraksh.SPOT.Net;
using Samraksh.SPOT.Net.Mac;

namespace Samraksh.APPS.ActiveTag
{

    public enum MessageDestinationType : byte
    {
        BaseStation,
        LocalNeighborhoodNode,
        SpecificNonBaseNode,  //use when intended destniation is *not* a local neighbor node
        AllNodes
    } ;


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
    } ;

    public enum MessageStrings : byte
    {
        IAmAlive,
        AreYouAlive,
        SendSnapshotRequest,
        SendRteInfoRequest
    } ;

    public enum MessageAuthorType : byte
    {
        BaseStationNodeAuthor,
        NeighborNodeAuthor,
        SpecificNodeAuthor
    } ;

    public enum MessageAuthorFunctionType : byte
    {
        BaseStationNodeFcn,
        FoodNodeFcn,
        MedicalNodeFcn,
        AmmunitionNodeFcn
    } ;

    public enum TagFunctionType :byte
    {
        FoodFcn,
        MedicalNodeFcn,
        AmmunitionNodeFcn,
    };

    public enum IntendedRecipientMacIsSpecified : byte
    {
        Yes,
        No
    } ;






    public class TagNode
    {

        public Random rng;

        public TagFunctionType tagType;

        public MACBase macObject;

        public UInt16 temperature;

        public ReceiveCallBack rcallback;

        public NeighbourhoodChangeCallBack ncallback;

        public MacConfiguration macconfig;

        public Networking nw;

        NeighbourList neighbourList;

        UInt16 tagAddress;

        Timer sensorSamplerTimer;

        SynchronizedVariable sendCritialAlarm;

        SynchronizedVariable processMessage;

        SynchronizedVariable processNeighbourChange;

        // Holds the current packet from the native buffer
        Samraksh.SPOT.Net.Message incomingMessage;

        // Holds the current message being processed
        MessagePayload mp;

        UInt16 theMsgNoFromThisNodeSendingMac = 1;

        UInt16 numberOfMessagesToProcess = 0;

        public void HandleMessage(UInt16 numberOfPackets)
        {
            // Possibility of race condition here 
            numberOfMessagesToProcess = numberOfPackets;
            
            processMessage.SetState(true);
        }

        public void HandleNeighbourChanged(UInt16 numberOfNeighboursChanged)
        {
            processNeighbourChange.SetState(true);
        }

        public void AssignTagFunction()
        {
            rng = new Random(tagAddress);

            tagType = (TagFunctionType)(rng.Next() % 4);
        }

        public TagNode()
        {
            macconfig = new MacConfiguration();

            // Default value is 300s, changing to 10s for testing 
            //macconfig.NeighbourLivelinesDelay = 10;

            // Set to extreme low power 
            macconfig.radioConfig.SetTxPower(SPOT.Net.Radio.TxPowerValue.Power_Minus17dBm);

            rcallback = HandleMessage;

            ncallback = HandleNeighbourChanged;

            // Configure the csma before grabbing the instance 
            MACBase.Configure(macconfig, rcallback, ncallback);

            // Grab the current csma instance 
            try
            {
                macObject = CSMA.Instance;
            }
            catch (MacNotConfiguredException e)
            {
                Debug.Print("The Mac has not been configured \n");
            }

            // Grab the address of the node
            tagAddress = macObject.GetAddress();

            Debug.Print("This Node ID is : " + tagAddress + "\n");

            // Initialize the routing layer with the csma object 
            nw = new Networking(macObject);


            AssignTagFunction();

            // Disabling sensor sampler timer for testing 
            sensorSamplerTimer = new Timer(SensorSamplerCallback, null, 200, tagAddress * 10);

            sendCritialAlarm = new SynchronizedVariable();

            processMessage = new SynchronizedVariable();

            processNeighbourChange = new SynchronizedVariable();

            neighbourList = new NeighbourList();

        }

        public void SensorSamplerCallback(Object state)
        {
            temperature = (UInt16) (rng.Next() % 1000);

            if (temperature > 300)
            {
                sendCritialAlarm.SetState(true);
            }
            else
                sendCritialAlarm.SetState(false);
        }

        private bool SendCriticalValueExceeded()
        {
            UInt16 contentLength = 0;
            UInt16 totalMessageLength = 0;

            MessagePayload mp = new MessagePayload();

            mp.senderMac = tagAddress;
            mp.msgNoFromSenderMac = theMsgNoFromThisNodeSendingMac;

            // Keep track of the message number from this tag
            theMsgNoFromThisNodeSendingMac++;

            mp.msgDestinationType = MessageDestinationType.BaseStation;

            mp.msgPType = MessagePayloadType.CriticalValueExceeded;

            mp.msgAuthType = MessageAuthorType.SpecificNodeAuthor;

            mp.msgAuthFuncType = (MessageAuthorFunctionType) tagType;

            mp.intendedRecMacIsSpecified = IntendedRecipientMacIsSpecified.No;

            mp.intendedRecMacIsSpecified = 0;

            mp.origSenderMac = tagAddress;

            mp.messageContent[contentLength++] = (byte) (temperature & 0xff);
            mp.messageContent[contentLength++] = (byte)((temperature >> 8) & 0xff);

            mp.messageContentLength = contentLength;

            nw.sendingMsg(mp.MarshallIntoBytes(), 0, mp.messageLength);



            return true;
        }

        public void GenerateAreYouAliveMessagesForMissingNeighbors(UInt16 neighbour)
        {
            UInt16 contentLength = 0;

            Debug.Print("Neighbour node " + neighbour.ToString() + " missing \n");
            Debug.Print("Generating are you alive messages \n");

            MessagePayload mp = new MessagePayload();

            mp.intendedRecMacIsSpecified = IntendedRecipientMacIsSpecified.Yes;
            mp.intendedRecipientMacAddress = neighbour;
            mp.msgNoFromSenderMac = theMsgNoFromThisNodeSendingMac;
            theMsgNoFromThisNodeSendingMac++;
            mp.msgDestinationType = MessageDestinationType.LocalNeighborhoodNode;
            mp.msgAuthType = MessageAuthorType.NeighborNodeAuthor;
            mp.msgAuthFuncType = (MessageAuthorFunctionType)tagType;
            mp.msgPType = MessagePayloadType.AreYouAliveRequest;
            mp.origSenderMac = tagAddress;
            mp.senderMac = tagAddress;
            mp.messageContent[contentLength++] = 0xff;
            mp.messageContent[contentLength++] = 0xff;

            mp.messageContentLength = contentLength;

            nw.sendingMsg(neighbour, mp.MarshallIntoBytes(), 0, mp.messageLength);

        }

        public void GenerateAliveResponse(UInt16 destMac)
        {
            UInt16 contentLength = 0;

            MessagePayload mp = new MessagePayload();

            mp.intendedRecMacIsSpecified = IntendedRecipientMacIsSpecified.Yes;
            mp.intendedRecipientMacAddress = destMac;
            mp.msgNoFromSenderMac = theMsgNoFromThisNodeSendingMac;
            theMsgNoFromThisNodeSendingMac++;
            mp.msgDestinationType = MessageDestinationType.LocalNeighborhoodNode;
            mp.msgAuthType = MessageAuthorType.NeighborNodeAuthor;
            mp.msgAuthFuncType = (MessageAuthorFunctionType)tagType;
            mp.msgPType = MessagePayloadType.AreYouAliveResponse;
            mp.origSenderMac = tagAddress;
            mp.senderMac = tagAddress;
            mp.messageContent[contentLength++] = 0xff;
            mp.messageContent[contentLength++] = 0xff;

            mp.messageContentLength = contentLength;

            nw.sendingMsg(destMac, mp.MarshallIntoBytes(), 0, mp.messageLength);
        }

        public void ProcessNeighbourChange()
        {
            UInt16[] missingNeighbours = null;

            if (!neighbourList.Insert(macObject.GetNeighbourList()))
            {
                missingNeighbours = neighbourList.GetLostNeighbours();

                foreach (UInt16 neighbour in missingNeighbours)
                {
                    GenerateAreYouAliveMessagesForMissingNeighbors(neighbour);
                }

            }
        }

        public void ForwardMessage()
        {
            mp.senderMac = tagAddress;
            mp.msgNoFromSenderMac = theMsgNoFromThisNodeSendingMac;
            theMsgNoFromThisNodeSendingMac++;
            nw.sendingMsg(mp.MarshallIntoBytes(), 0, mp.messageLength);
        }

        public void ProcessRteInfoReqFromBase()
        {
            UInt16[] activeNeighbours = neighbourList.GetActiveNeighbours();
            UInt16 contentLength = 0;

            if (neighbourList.numberOfActiveNeighbours == 0)
            {
                Debug.Print("Node " + tagAddress.ToString() + " does not see any neighbours \n");
                return;
            }

            Debug.Print("Sending Information about ");

            for (int i = 0; i < neighbourList.numberOfActiveNeighbours; i++)
            {
                Debug.Print(activeNeighbours[i].ToString());
            }

            MessagePayload mp = new MessagePayload();

            mp.senderMac = tagAddress;
            mp.origSenderMac = tagAddress;
            mp.msgPType = MessagePayloadType.RteInfoRespToBase;
            mp.msgDestinationType = MessageDestinationType.BaseStation;
            mp.msgAuthType = MessageAuthorType.SpecificNodeAuthor;
            mp.msgAuthFuncType = (MessageAuthorFunctionType)tagType;
            mp.msgNoFromSenderMac = theMsgNoFromThisNodeSendingMac;
            theMsgNoFromThisNodeSendingMac++;
            mp.intendedRecMacIsSpecified = IntendedRecipientMacIsSpecified.No;
            mp.intendedRecipientMacAddress = 0;

            // load information about neighbours as message content
            for (int i = 0; i < neighbourList.numberOfActiveNeighbours; i++)
            {
                mp.messageContent[contentLength++] = (byte) (activeNeighbours[i] & 0xff);
                mp.messageContent[contentLength++] = (byte) ((activeNeighbours[i] >> 8) & 0xff);
            }
            mp.messageContentLength = contentLength;

            // Send data about my neighbours to base 
            nw.sendingMsg(mp.MarshallIntoBytes(), 0, mp.messageLength);
            
            
        }

        public void ProcessRteInfoRespToBase()
        {
        }

        public void Run()
        {
            while (true)
            {
                if (sendCritialAlarm.IsReady())
                {
                    SendCriticalValueExceeded();
                    sendCritialAlarm.SetState(false);
                }
                if (processMessage.IsReady())
                {

                    while (macObject.GetPendingPacketCount() > 0)
                    {
                        incomingMessage = macObject.GetNextPacket();

                        if (incomingMessage != null)
                        {
                            mp = new MessagePayload(incomingMessage.GetMessage());

                            // If message is not meant for me then just forward it 
                            if (mp.msgDestinationType == MessageDestinationType.BaseStation)
                            {
                                ForwardMessage();
                                continue;
                            }

                            switch (mp.msgPType)
                            {
                                case MessagePayloadType.AreYouAliveResponse:
                                    // No need to add this to the list because the neighbour table is the mac would have done this already
                                    Debug.Print("Neighbour " + mp.senderMac.ToString() + " is alive\n");
                                    break;

                                case MessagePayloadType.AreYouAliveRequest:
                                    Debug.Print("Generating response to node " + mp.senderMac.ToString() + " \n");
                                    GenerateAliveResponse(mp.senderMac);
                                    break;

                                case MessagePayloadType.RteInfoReqFromBase:
                                    Debug.Print("Neighbourhood Info Request from Base");
                                    ForwardMessage();
                                    ProcessRteInfoReqFromBase();
                                    break;

                                case MessagePayloadType.RteInfoRespToBase:
                                    ForwardMessage();
                                    ProcessRteInfoRespToBase();
                                    break;

                                   
                            }
                        }
                    }

                    processMessage.SetState(false);
                }
                if (processNeighbourChange.IsReady())
                {

                    Debug.Print("Neighbourhood Change Detected");

                    ProcessNeighbourChange();

                    processNeighbourChange.SetState(false);
                }

                Thread.Sleep(100);
            }
        }
    }


    public class Program
    {
        public static void Main()
        {
            TagNode tn = new TagNode();

            tn.Run();
        }

    }
}
