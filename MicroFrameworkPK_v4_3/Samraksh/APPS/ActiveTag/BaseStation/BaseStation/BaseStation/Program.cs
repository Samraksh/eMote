using System;
using Microsoft.SPOT;

using System.Threading;
using Samraksh.SPOT.Net;
using Samraksh.SPOT.Net.Mac;

namespace BaseStation
{
    //declare Destination Enum
    public enum DestinationType
    {
        BaseStation,
        LocalNeighborhoodNode,
        SpecificNonBaseNode,  //use when intended destniation is *not* a local neighbor node
        AllNodes
    } ;


    public enum MessagePayloadType
    {
        SnapShotRequest,
        SnapShotResponse,
        CriticalValueExceeded,
        NodeLeftNeighborhood,
        AreYouAliveRequest,
        AreYouAliveResponse,
        RteInfoReqFromBase,
        RteInfoRespToBase
    } ;

    public enum MessageStrings
    {
        IAmAlive,
        AreYouAlive,
        SendSnapshotRequest,
        SendRteInfoRequest
    } ;

    public enum MessageAuthorType
    {
        BaseStationNodeAuthor,
        NeighborNodeAuthor,
        SpecificNodeAuthor
    } ;


    public enum MessageAuthorFunctionType
    {
        BaseStationNodeFcn,
        FoodNodeFcn,
        MedicalNodeFcn,
        AmmunitionNodeFcn
    } ;

    public enum IntendedRecipientMacIsSpecified
    {
        Yes,
        No
    } ;

    public class MessagePayload
    {
        MessagePayloadType msgPType;
        DestinationType msgDestinationType;
        MessageAuthorFunctionType msgAuthFuncType;
        MessageAuthorType msgAuthType;

        UInt16 byteLengthOfEntireMessage;
        byte[] messagePayloadContent = new byte[183];
        byte[] messagePayloadBuffer = new byte[200];
        UInt16 numberBytesInContent;  //this is the true payload of the message, not the headers in this class

        //UInt16[] messagePayloadAsFieldArray = new UInt16[116];  //divide this by 2
        UInt16[] messagePayloadAsFieldArray = new UInt16[92];  //assume that the last byte in the messagePayloadContent byte array is not used

        UInt16 senderMac;
        UInt16 msgNoFromSenderMac;
        UInt16 origSenderMac;
        UInt16 byteOffset = 17;  //known from structure of message payloadfields

        IntendedRecipientMacIsSpecified intendedRecMacIsSpecified;
        UInt16 intendedRecipientMacAddress = (UInt16)0;  //this is the default value if it is not set

        public void setLengthOfEntireMessage(UInt16 lenEntire)
        {
            byteLengthOfEntireMessage = lenEntire;
        }
        public UInt16 getLengthOfEntireMessage()
        {
            return byteLengthOfEntireMessage;
        }

        /** this next method setMessageContentInBytes NOT used in this class
         *  or in the LogisticsApp.cs
         */
        // INSTEAD of this, we USE setMsgPayloadContent(UInt16 _size, byte[] contentArr)  
        /**
        public void setMessageContentInBytes(byte[] _content, UInt16 len)
        {
            numberBytesInContent = len;
            for (int i = 0; i < len; i++)
            {
                messagePayloadContent[i] = _content[i];
            }
        }
        */

        public UInt16 getNumberBytesInContent()
        {
            return numberBytesInContent;
        }

        /** this is a repeat of the setting of numberBytesInContent via the method setMessageContentInBytes
         * BUT this is used in both the LogisticsApp and in this class
         */
        public void setNumberBytesInContent(UInt16 lenBytesInContent)
        {
            numberBytesInContent = lenBytesInContent;
        }


        public byte[] getContent()
        {
            return messagePayloadContent;
        }

        public void setMessagePayloadType(MessagePayloadType _type)
        {
            msgPType = _type;
        }
        public MessagePayloadType getMessagePayloadType()
        {
            return msgPType;
        }

        public void setDestinationType(DestinationType _destT)
        {
            msgDestinationType = _destT;
        }
        public DestinationType getMsgDestinationType()
        {
            return msgDestinationType;
        }

        public void setMessageAuthorFunctionType(MessageAuthorFunctionType _maft)
        {
            msgAuthFuncType = _maft;
        }
        public MessageAuthorFunctionType geMessageAuthorFunctionType()
        {
            return msgAuthFuncType;
        }

        public void setMessageAuthorType(MessageAuthorType _mat)
        {
            msgAuthType = _mat;
        }
        public MessageAuthorType getMessageAuthorType()
        {
            return msgAuthType;
        }

        public void setSenderMac(UInt16 _senderMac)
        {
            senderMac = _senderMac;
        }
        public UInt16 getSenderMac()
        {
            return senderMac;
        }
        public void setMsgNumberFromSenderMac(UInt16 senderNo)
        {
            msgNoFromSenderMac = senderNo;
        }
        public UInt16 getMsgNumberFromSenderMac()
        {
            return msgNoFromSenderMac;
        }

        public void setOriginalSenderMac(UInt16 _origSenderMac)
        {
            origSenderMac = _origSenderMac;
        }
        public UInt16 getOriginalSenderMac()
        {
            return origSenderMac;
        }

        public UInt16 getByteOffset()
        {
            return byteOffset;
        }

        public void setIsIntendedRecipientMacSpecified(IntendedRecipientMacIsSpecified isSpecified)
        {
            intendedRecMacIsSpecified = isSpecified;
        }

        public IntendedRecipientMacIsSpecified getIsIntendedRecipientMacSpecified()
        {
            return intendedRecMacIsSpecified;
        }

        public void setIntendedRecipientMacAddress(UInt16 intendedRecipMacAddr)
        {
            intendedRecipientMacAddress = intendedRecipMacAddr;
        }
        public UInt16 getIntendedRecipientMacAddress()
        {
            return intendedRecipientMacAddress;
        }

        //this is used when setting up message sending
        public void setMsgPayloadInBytesUsingFields()
        {
            Debug.Print(" in setMsgPayloadInBytesUsingFields");
            //byte[] arr2 = new byte[2] ; //avoid use of new
            byte[] arr2 = { 0, 0 };
            arr2 = GetBytes(getLengthOfEntireMessage());
            messagePayloadBuffer[0] = arr2[0];
            //Debug.Print("  messagePayloadBuffer[0]" + arr2[0]) ; 
            messagePayloadBuffer[1] = arr2[1];
            //Debug.Print("  messagePayloadBuffer[1]" + arr2[1]); 
            arr2 = GetBytes(getSenderMac());
            messagePayloadBuffer[2] = arr2[0];
            //Debug.Print("  messagePayloadBuffer[2]" + arr2[0]); 
            messagePayloadBuffer[3] = arr2[1];
            //Debug.Print("  messagePayloadBuffer[3]" + arr2[1]); 
            arr2 = GetBytes(getMsgNumberFromSenderMac());
            messagePayloadBuffer[4] = arr2[0];
            //Debug.Print("  messagePayloadBuffer[4]" + arr2[0]); 
            messagePayloadBuffer[5] = arr2[1];
            //Debug.Print("  messagePayloadBuffer[5]" + arr2[1]); 
            arr2 = GetBytes(getOriginalSenderMac());
            messagePayloadBuffer[6] = arr2[0];
            //Debug.Print("  messagePayloadBuffer[6]" + arr2[0]); 
            messagePayloadBuffer[7] = arr2[1];
            //Debug.Print("  messagePayloadBuffer[7]" + arr2[1]); 
            messagePayloadBuffer[8] = (byte)getMsgDestinationType();
            //Debug.Print("  messagePayloadBuffer[8]" + messagePayloadBuffer[8]); 
            messagePayloadBuffer[9] = (byte)getMessagePayloadType();
            //Debug.Print("  messagePayloadBuffer[9]" + messagePayloadBuffer[9]); 
            messagePayloadBuffer[10] = (byte)getMessageAuthorType();
            //Debug.Print("  messagePayloadBuffer[10]" + messagePayloadBuffer[10]); 
            messagePayloadBuffer[11] = (byte)geMessageAuthorFunctionType();
            //Debug.Print("  messagePayloadBuffer[11]" + messagePayloadBuffer[11]); 
            messagePayloadBuffer[12] = (byte)getIsIntendedRecipientMacSpecified();
            //Debug.Print("  messagePayloadBuffer[12]" + messagePayloadBuffer[12]); 
            arr2 = GetBytes(getIntendedRecipientMacAddress());
            messagePayloadBuffer[13] = arr2[0];
            //Debug.Print("  messagePayloadBuffer[13]" + arr2[0]); 
            messagePayloadBuffer[14] = arr2[1];
            //Debug.Print("  messagePayloadBuffer[14]" + arr2[1]); 
            arr2 = GetBytes(getNumberBytesInContent());
            messagePayloadBuffer[15] = arr2[0];
            // Debug.Print("  messagePayloadBuffer[15]" + arr2[0]); 
            messagePayloadBuffer[16] = arr2[1];
            // Debug.Print("  messagePayloadBuffer[16]" + arr2[1]);
            //Debug.Print(" numberOfBytesInContent " + numberBytesInContent);
            for (int i = 0; i < numberBytesInContent; i++)
            {
                messagePayloadBuffer[i + byteOffset] = messagePayloadContent[i];
                //Debug.Print(" i and messagePayloadContent[i] = " + i + "  " + messagePayloadContent[i ]);
                //Debug.Print(" i and messagePayloadBuffer[i] = " + i + "  " + messagePayloadBuffer[i + byteOffset]);
            }
        }


        public byte[] getMessagePayloadBuffer()
        {
            return messagePayloadBuffer;
        }

        //here _size is the number of bytes in the content
        //  this is used upon receiving the message and reconstructing the payload form bytes
        public void setMsgPayloadContent(UInt16 _size, byte[] contentArr)
        {
            Debug.Print("  in setMsgPayloadContent   _size = " + _size);

            for (int i = 0; i < _size; i++)
            {
                messagePayloadContent[i] = contentArr[i];
                Debug.Print(" in setMsgPayloadContent - in bytes  the content byte [i]  = " + i + "  " + messagePayloadContent[i]);
            }
            //for (int i = 0; i < 183; i++)
            //{
            //     Debug.Print(" i and messagePayloadContent[i] = " + i + "  " + messagePayloadContent[i]);
            // }
        }


        //  contentArr is the msg payload content expressed in bytes
        //   this is used for the received payload of the received message (as opposed to the sending message)
        // THIS IS AN example for inserting a template/stencil over the received message payload to have it print out
        //   as something other than bytes.
        public void setMsgPayloadContentAsFieldArray(UInt16 _size, byte[] contentArr)
        {
            byte[] holdingArr = new byte[2];
            holdingArr[0] = 0;
            holdingArr[1] = 0;

            for (int i = 0; i < _size; i++)
            {
                int j0 = 2 * _size;
                int j1 = 2 * _size + 1;
                holdingArr[0] = contentArr[j0];
                Debug.Print(" holdingArr[i] " + i + " " + holdingArr[i]);
                holdingArr[1] = contentArr[j1];
                Debug.Print(" holdingArr[i] " + i + " " + holdingArr[i]);
                messagePayloadAsFieldArray[i] = ToUInt16(holdingArr, 0);
                Debug.Print("messagePayloadAsFieldArray[i] " + i + " " + messagePayloadAsFieldArray[i]);
            }
        }


        public UInt16[] getMsgPayloadContentAsFieldArray()
        {
            return messagePayloadAsFieldArray;
        }

        // this is used upon receiving the message and unpacking it into fields 
        public void setPayloadFieldsFromMsgByteArray(byte[] msgIn, UInt16 len)
        {
            //byte[] array2 = new byte[2];  //avoid use of new
            byte[] array2 = { 0, 0 };
            setLengthOfEntireMessage(len);

            //Debug.Print(" in setPayloadFieldsFromMsgByteArray = " );
            array2[0] = msgIn[2];
            array2[1] = msgIn[3];
            //Debug.Print("  senderMac[0] = " + array2[0]);
            //Debug.Print("  senderMac[1] = " + array2[1]);
            setSenderMac(ToUInt16(array2, 0));
            array2[0] = msgIn[4];
            array2[1] = msgIn[5];
            //Debug.Print("  msgNoFromSenderMac[0] = " + array2[0]);
            //Debug.Print("  msgNoFromSenderMac[1] = " + array2[1]);
            setMsgNumberFromSenderMac(ToUInt16(array2, 0));
            //Debug.Print("msgNoFromSenderMac = " + msgNoFromSenderMac);

            array2[0] = msgIn[6];
            array2[1] = msgIn[7];
            setOriginalSenderMac(ToUInt16(array2, 0));
            //Debug.Print("origSenderMac = " + origSenderMac);

            setDestinationType((DestinationType)msgIn[8]);
            //Debug.Print("DestinationType  " + msgDestinationType)  ;
            setMessagePayloadType((MessagePayloadType)msgIn[9]);
            //Debug.Print(" MessagePayloadType " + msgPType);
            setMessageAuthorType((MessageAuthorType)msgIn[10]);
            //Debug.Print(" MessageAuthorType " + msgAuthType);
            setMessageAuthorFunctionType((MessageAuthorFunctionType)msgIn[11]);
            //Debug.Print(" MessageAuthorFunctionType " + msgAuthFuncType);


            // array2[0] = msgIn[10];
            // array2[1] = msgIn[11];
            // setOriginalSenderMac(ToUInt16(array2, 0));
            // Debug.Print("origSenderMac = " + origSenderMac);
            // array2[0] = msgIn[12];
            // array2[1] = msgIn[13];
            setIsIntendedRecipientMacSpecified((IntendedRecipientMacIsSpecified)msgIn[12]);
            //Debug.Print("intendedRecMacIsSpecified " + intendedRecMacIsSpecified);
            array2[0] = msgIn[13];
            array2[1] = msgIn[14];
            setIntendedRecipientMacAddress(ToUInt16(array2, 0));
            // Debug.Print("intended recipient mac = " + intendedRecipientMacAddress);

            //Don't know a priori how big the payload content field since reconstructing from 
            array2[0] = msgIn[15];
            array2[1] = msgIn[16];
            setNumberBytesInContent(ToUInt16(array2, 0)); //this is the length of the content (also called payload of msg)

            UInt16 theNumBytesInContent = getNumberBytesInContent();

            byte[] arrayForContentPayload = new byte[theNumBytesInContent];
            for (int i = 0; i < theNumBytesInContent; i++)
            {
                arrayForContentPayload[i] = msgIn[byteOffset + i];
            }
            //need to capture the Msg payload field in bytes first, with the correct byte offset done
            //  this is setting the payload content field -- in bytes -- for the received message
            setMsgPayloadContent(theNumBytesInContent, arrayForContentPayload);

            //from here you could put a template over the received message in bytes
            //  this is up to the application design
            //  example,  call   setMsgPayloadContentAsFieldArray
        }


        //see forums.netduino.com/index.php?/topic/308-bitconverter
        public UInt16 ToUInt16(byte[] value, int index = 0)
        {
            return (UInt16)(value[0 + index] << 0 | value[1 + index] << 8);
        }

        public byte[] GetBytes(UInt16 value)
        {
            //try alternate code
            byte[] returnByteArray = { 0, 0 };
            returnByteArray[0] = (byte)(value & 0xFF);
            Debug.Print(" in getBytes byte [0] is returnByteArray[0]" + returnByteArray[0]);
            returnByteArray[1] = (byte)((value >> 8) & 0xFF);
            Debug.Print(" in getBytes byte [1] is returnByteArray[0]" + returnByteArray[1]);
            // avoid the use of new
            //return new byte[2] { (byte)(value & 0xFF), (byte)((value >> 8) & 0xFF) };
            return returnByteArray;
        }
    } //end MessagePayload class

    public class BaseNodeSurrogate
    {
        //bool wasCriticalTempExceeded = false;
        //bool critMsgFlag = false;
        bool shouldProcessThisMessage = false;

        //this assumes up to 10 nodes in the WSN
        static int MAX_WSN_NODES = 10;
        UInt16[] senderMACIDArray = new UInt16[MAX_WSN_NODES];
        UInt16[] highestMsgIDFromEachNodeArray = new UInt16[MAX_WSN_NODES];

        Samraksh.SPOT.Net.Message recvdMessage;

        MessagePayload mp = new MessagePayload();
        MessagePayload recvMP = new MessagePayload();
        MessagePayload sendMP = new MessagePayload();
        //This next is a choice
        MessageAuthorFunctionType myMAFT = MessageAuthorFunctionType.BaseStationNodeFcn;

        //used for received and composed message payload fields
        MessagePayloadType theMPT;
        MessageAuthorType theMAT;
        MessageAuthorFunctionType theMAFT;
        DestinationType theMsgDestType;

        UInt16 theSenderMacAddress;
        // this next is incremented every time this node sends out a message
        //  start it at message number 1 
        UInt16 theMsgNoFromThisNodeSendingMac = 1;
        UInt16 theOriginalSenderMac;

        // these next are helper globals
        UInt16 theOriginalSenderMacListed;
        UInt16 theMostRecentSenderMacListed;

        // this next used as a holder for the msg number from a rcvd message that enables us to check
        //   by associating with its sender mac if that msg has been received before
        UInt16 theMsgNoFromRecvdMsgMac;
        //UInt16 aHighSenderMsgNum;


        //helper variables for sending a message
        int totalMsgLen, theMsg;
        // aTwoByteArray as a helper array in this demo work
        byte[] a2ByteArray = new byte[2];


        byte[] theTotalMsgBuffer = new byte[200];
        UInt16 totalMsgLength;
        UInt16 appLevelHeaderOffsetInBytes;
        byte[] theTotalMsgContentBuffer_bytes = new byte[183];
        UInt16[] theTotalMsgContentBuffer_UInt16 = new UInt16[92];
        UInt16 numberOfUInt16InMsgActualContent = 0;
        // other helper bytes arrays
        byte[] partOfContentArray_bytes = new byte[2];

        //  set up the payload queue for 20 messages that can be stored
        static byte maxIncomingMsgQSize = 20;
        MessagePayload[] msgPayloadQueue = new MessagePayload[20];
        sbyte nextMsgPayloadToProcessNum = -1;  //range -128 to 127
        byte nextMsgPayloadToAddToQNum = 0;


        //the routing class
        static Networking theNetworking = new Networking();

        //used to wait for robust call to neighbor table, time in ms
        //  TODO:  obtain what would be a reasonable time 
        private static UInt16 timeToWaitForNghbrTableAfterMacInit = 200;

        //the callback 
        static ReceiveCallBack myReceive;

        static NeighbourhoodChangeCallBack myNeighbourChanged;

        // instantiating the CSMA
        static Samraksh.SPOT.Net.Mac.CSMA myCSMAMac;
        // instantiating the struct MacConfiguration
        static Samraksh.SPOT.Net.Mac.MacConfiguration macConfig = new MacConfiguration();
        static UInt16 myNodeID;      //this is the mac ID of this emote (and app) node, used as Id
        static UInt16 myMacAddress; // this is the mac address of this emote (and app node) 

        // static Samraksh.SPOT.Net.Mac.Neighbor myNeighbor;
        static Samraksh.SPOT.Net.Mac.NeighborTable myNeighborTable;
        static Samraksh.SPOT.Net.Mac.NeighborTable myPrevNeighborTable;

        // static Samraksh.SPOT.Net.Mac.NeighborTable myNeighborTable_currentPull;
        //next refers to MAC Addr that is used as payload in certain types of messages
        //static byte MAX_NUM_MACADDR_IN_PAYLOAD_INFO = 20;  //this is probably more than is atually going to be needed, but  less than the available payload space

        static UInt16[] possibleMissingNeighborMacIDs = new UInt16[20];
        // static UInt16 numberOfPossibleMissingNeighbors = 0;
        static MessagePayload[] msgForPossibleMissingNghbrs = new MessagePayload[20];
        static UInt16[] likelyMissingNeighborMacIDs = new UInt16[20];

        // the flag the surrogate base node uses in sending out a message 
        //   to request a snapshot from the nodes in the wsn
        //static bool haveSentSnapshotRequest = false;
        static UInt16 cycleTimeSendOutSnapshotRequest = 5000;
        int cycleInt = 0;

        //helper global variables
        int indexMax, i, i2;
        bool breakFlag, breakFlag2;


        void HandleNeighbourChange(UInt16 neighboursChanged)
        {
        }

        void init()
        {
         
            Debug.Print("Initializing: BaseNode Surrogate App");

             //assign callback delegate to function
            myReceive = HandleMessage;

            myNeighbourChanged = HandleNeighbourChange;

            CSMA.Configure(macConfig, myReceive, myNeighbourChanged);

            Thread.Sleep(1000);

            Debug.Print("Initializing:  CSMA MAC ...");
            try
            {
                  myCSMAMac = Samraksh.SPOT.Net.Mac.CSMA.Instance;
            }
            catch (Exception e)
            {
                Debug.Print(e.ToString());
            }

            Debug.Print("CSMA Init done.");
            myMacAddress = myCSMAMac.GetAddress();
            Debug.Print("My mac address is :  " + myMacAddress.ToString());

            myNodeID = myMacAddress;

            //waiting for MAC to be robust before acquiring the neighbor table
            Thread.Sleep(timeToWaitForNghbrTableAfterMacInit);

            //already have my node ID from the setup 
            //  but still must get the neighbor tables  set up

            //NO NGHBR TABLE RETURNED FROM CSMA MAC CALLS
            // myNeighborTable = myCSMAMac.GetNeighborTable();
            // myPrevNeighborTable = myNeighborTable;
            // myNeighborTable = myCSMAMac.GetNeighborTable();

        }// end init


        public static void Main()
        {

            BaseNodeSurrogate bns = new BaseNodeSurrogate();
            bns.init();
            Debug.Print(" In  Surrogate Base Main");

            while (true)
            {

                Thread.Sleep(500);

                bns.cycleInt++;

                bns.shouldProcessThisMessage = false;  //this is a resetting to start at the next time through the loop
                //look at the next message to process and set up the circular q limit
                if (bns.nextMsgPayloadToProcessNum != -1)
                {
                    bns.mp = bns.msgPayloadQueue[bns.nextMsgPayloadToProcessNum];
                    if (bns.mp != null)
                    {
                        bns.nextMsgPayloadToProcessNum++;
                        if (bns.nextMsgPayloadToProcessNum == MAX_WSN_NODES - 1)
                        {
                            bns.nextMsgPayloadToProcessNum = 0;
                        }
                    }
                }  //end  if (nextMsgPayloadToProcessNum != -1 )

                if (bns.mp != null)
                {
                    //check to see if that message was sent before by getting the message id of 
                    //  the recvd msg
                    bns.theSenderMacAddress = bns.mp.getSenderMac();
                    bns.theMsgNoFromRecvdMsgMac = bns.mp.getMsgNumberFromSenderMac();
                    bns.indexMax = -1;
                    bns.breakFlag = false;
                    //this is to find the number of mac ids already in the array
                    //  there won't be any until after the first message
                    //  so let us check to see if this is the first message received from any node
                    while (bns.breakFlag == false)
                    {
                        for (bns.i = 0; bns.i < MAX_WSN_NODES; bns.i++)
                        {
                            if (bns.senderMACIDArray[bns.i] == 0)
                            {
                                bns.indexMax = bns.i;
                            }
                            if (bns.indexMax != -1) bns.breakFlag = true;
                        }
                    }
                    if (bns.indexMax == 0)  //this is the first received by this node from any sender
                    {
                        //this info is related to the most recent sender
                        bns.senderMACIDArray[0] = bns.theSenderMacAddress;
                        bns.highestMsgIDFromEachNodeArray[0] = bns.theMsgNoFromRecvdMsgMac;
                        bns.shouldProcessThisMessage = true;
                    }
                    //looking to see if the sender is already known
                    bns.breakFlag2 = false;
                    bns.i2 = 0;
                    if (bns.shouldProcessThisMessage == false)  //have received msgs previous to this
                    {
                        while ((bns.breakFlag2 == false) && (bns.i2 < bns.indexMax + 1))
                        {  //this info is related to the most recent sender
                            if (bns.senderMACIDArray[bns.i2] == bns.theSenderMacAddress)
                            {
                                bns.breakFlag2 = true;
                            }
                            if (bns.breakFlag2 == false)
                            {
                                bns.i2++;
                            }
                        } //end while ((breakFlag2 == false) && (i2 < indexMax + 1))
                        if (bns.breakFlag2 == false && (bns.i2 < bns.indexMax + 1))  //this is the first message from a new node
                        {
                            if (bns.indexMax + 1 < MAX_WSN_NODES)  //if there are more than max nodes, their msgs never get thru
                            {
                                bns.senderMACIDArray[bns.indexMax + 1] = bns.theSenderMacAddress;
                                bns.highestMsgIDFromEachNodeArray[bns.indexMax + 1] = bns.theMsgNoFromRecvdMsgMac;
                                bns.shouldProcessThisMessage = true;
                            }
                        }
                        if (bns.breakFlag2 == true) //have recv'd msg from this node before,must see if new msg has higher number
                        {
                            if (bns.highestMsgIDFromEachNodeArray[bns.i2] < bns.theMsgNoFromRecvdMsgMac)
                            {
                                bns.shouldProcessThisMessage = true;
                            }
                            if (bns.shouldProcessThisMessage == true)
                            {
                                bns.highestMsgIDFromEachNodeArray[bns.i2] = bns.theMsgNoFromRecvdMsgMac;
                            }
                        } //end  (breakFlag2 == true)
                    } //end if (shouldProcessThisMessage == false) 
                } //end if (mp != null)


                //so if the message is a new one, it can be processed
                //  
                if (bns.shouldProcessThisMessage == true)
                {
                    bns.theMPT = bns.mp.getMessagePayloadType();
                    bns.theMAT = bns.mp.getMessageAuthorType();
                    bns.theMAFT = bns.mp.geMessageAuthorFunctionType();
                    bns.theMsgDestType = bns.mp.getMsgDestinationType();
                    bns.theOriginalSenderMac = bns.mp.getOriginalSenderMac();

                    bns.theTotalMsgBuffer = bns.mp.getMessagePayloadBuffer();
                    bns.totalMsgLength = bns.mp.getLengthOfEntireMessage();
                    bns.appLevelHeaderOffsetInBytes = bns.mp.getByteOffset();



                    if (bns.theMsgDestType == DestinationType.BaseStation)
                    {
                        //this message is meant for this base station node
                        //process this message
                        //mp.setSenderMac(myNodeID);
                        // mp.setMsgNumberFromSenderMac(theMsgNoFromThisNodeSendingMac);
                        //theMsgNoFromThisNodeSendingMac++;
                        // all other fields of mp should remain the same, so can forward
                        //theNetworking.forwardMessageToBase(mp, myCSMAMac);


                        if (bns.theMPT == MessagePayloadType.SnapShotResponse)
                        {
                            Debug.Print(" the Message Payload Type is " + bns.theMPT);
                            Debug.Print(" the Message Author Type is " + bns.theMAT);
                            Debug.Print(" the Message Author Function Type is " + bns.theMAFT);
                            Debug.Print(" the original sender MAC is " + bns.theOriginalSenderMac);

                            //now let us print out the content in UInt16
                            //  this is done by taking 2 bytes at a time
                            bns.numberOfUInt16InMsgActualContent = 0;
                            for (bns.i = 0; bns.i < (bns.totalMsgLength - bns.appLevelHeaderOffsetInBytes) / 2; bns.i++)
                            {
                                bns.partOfContentArray_bytes[0] = bns.theTotalMsgBuffer[bns.appLevelHeaderOffsetInBytes + bns.i];
                                bns.partOfContentArray_bytes[1] = bns.theTotalMsgBuffer[bns.appLevelHeaderOffsetInBytes + bns.i + 1];
                                bns.theTotalMsgContentBuffer_UInt16[bns.i] = ToUInt16(bns.partOfContentArray_bytes, 0);
                                bns.numberOfUInt16InMsgActualContent++;
                            }
                            //print message content in UInt16 for debugging assistance
                            for (bns.i = 0; bns.i < (bns.totalMsgLength - bns.appLevelHeaderOffsetInBytes) / 2; bns.i++)
                            {
                                Debug.Print(" the Message Payload [i] in UInt16 is " + bns.i + " " + bns.theTotalMsgContentBuffer_UInt16[bns.i + bns.appLevelHeaderOffsetInBytes]);
                            }
                        }
                        else if (bns.theMPT == MessagePayloadType.CriticalValueExceeded)
                        {

                            Debug.Print(" the Message Payload Type is " + bns.theMPT);
                            Debug.Print(" the Message Author Type is " + bns.theMAT);
                            Debug.Print(" the Message Author Function Type is " + bns.theMAFT);
                            Debug.Print(" the original sender MAC is " + bns.theOriginalSenderMac);
                            //now let us print out the content in UInt16
                            //  this is done by taking 2 bytes at a time
                            bns.numberOfUInt16InMsgActualContent = 0;
                            for (bns.i = 0; bns.i < (bns.totalMsgLength - bns.appLevelHeaderOffsetInBytes) / 2; bns.i++)
                            {
                                bns.partOfContentArray_bytes[0] = bns.theTotalMsgBuffer[bns.appLevelHeaderOffsetInBytes + bns.i];
                                bns.partOfContentArray_bytes[1] = bns.theTotalMsgBuffer[bns.appLevelHeaderOffsetInBytes + bns.i + 1];
                                bns.theTotalMsgContentBuffer_UInt16[bns.i] = ToUInt16(bns.partOfContentArray_bytes, 0);
                                bns.numberOfUInt16InMsgActualContent++;
                            }
                            //print message content in UInt16 for debugging assistance
                            for (bns.i = 0; bns.i < (bns.totalMsgLength - bns.appLevelHeaderOffsetInBytes) / 2; bns.i++)
                            {
                                Debug.Print(" the Message Payload [i] in UInt16 is " + bns.i + " " + bns.theTotalMsgContentBuffer_UInt16[bns.i + bns.appLevelHeaderOffsetInBytes]);
                            }
                        }
                    }
                    else
                    {

                        Debug.Print(" message destination type not meant for base station");
                    }
                }

                // set a flag for putting out a request for a snapshop
                if (bns.cycleInt == cycleTimeSendOutSnapshotRequest)  //time to send a request for a snapshot to all nodes in wsn
                {
                    //compose a message that is a snapshot request
                    bns.doRequestSnapshotMessage();
                    //reset the cycle back to zero
                    bns.cycleInt = 0;
                }

            }  //end while true
        }  //end main

        void doRequestSnapshotMessage()
        {
            // compose a msg for sending to the base station
            //  but we don't know the base station address in this code
            //   if that changes, must change code to reflect specific mac intended destination
            mp.setSenderMac(myNodeID);
            mp.setMsgNumberFromSenderMac(theMsgNoFromThisNodeSendingMac);
            theMsgNoFromThisNodeSendingMac++; //always increment after setting for sending
            mp.setDestinationType(DestinationType.AllNodes);
            mp.setMessagePayloadType(MessagePayloadType.SnapShotRequest);
            mp.setMessageAuthorType(MessageAuthorType.BaseStationNodeAuthor);
            mp.setMessageAuthorFunctionType(myMAFT);  //assigned as the function for this node
            mp.setIsIntendedRecipientMacSpecified(IntendedRecipientMacIsSpecified.No);
            mp.setIntendedRecipientMacAddress((UInt16)0);
            mp.setOriginalSenderMac(myNodeID);
            theMsg = (UInt16)MessageStrings.SendSnapshotRequest;
            a2ByteArray = mp.GetBytes((UInt16)theMsg);
            mp.setMsgPayloadContent(2, a2ByteArray);
            mp.setNumberBytesInContent(2);
            // this next is against C# specs -- but I want to emphasize that the size of what we 
            //  are dealing with is such that there would not be an overflow.
            // UInt16 totalMsgLen = (UInt16) mp.getByteOffset() +(UInt16)  mp.getNumberBytesInContent();
            totalMsgLen = (UInt16)mp.getByteOffset() + (UInt16)mp.getNumberBytesInContent();
            mp.setLengthOfEntireMessage((UInt16)totalMsgLen);
            //convert all loaded fields to bytes
            mp.setMsgPayloadInBytesUsingFields();
            //pass the message payload to the Routing helper
            //theNetworking.forwardMessageInFlood(mp, myCSMAMac);
            //CHANGING THE networking class -- making it more compact
            Debug.Print(" In Logistics App, doRequestSnapshotMessage ");
            theNetworking.sendingMsg(mp.getMessagePayloadBuffer(), mp.getByteOffset(), (UInt16)mp.getLengthOfEntireMessage(), myCSMAMac);
        }//end doCriticalValueExceededMessage()


        // MAC API CALLBACK HANDLER METHOD  
        //  that is registered upon mac initializaiton
        //  this is the entirety of the message -- including the payload
        //  the MessagePayload class API are used here
         //NEW API void MsgsReceived(byte[] message, ushort size, ushort Source, bool Unicast, byte RSSI, byte linkQuality)

       //void MsgsReceived(byte[] msgIn, ushort msgSize, ushort Source, bool Unicast, byte RSSI, byte linkQuality)
        //void MsgsReceived(byte[] msgIn, ushort msgSize)
       void HandleMessage(UInt16 NumberOfPackets)
       {

           Debug.Print("Recieved Message\n");

           recvdMessage = myCSMAMac.GetNextPacket();    

            //construct the message payload in terms of fields from the byte array
            recvMP.setLengthOfEntireMessage(recvdMessage.Size);
            recvMP.setPayloadFieldsFromMsgByteArray(recvdMessage.GetMessage(), recvdMessage.Size);


            // check to see if this message received is one that this node sent out -- either originally or as a resend
            //  if so, drop the message by returning and not adding ot to the queue to process
            theOriginalSenderMacListed = recvMP.getOriginalSenderMac();
            theMostRecentSenderMacListed = recvMP.getSenderMac();
            if (theOriginalSenderMacListed == myNodeID || theMostRecentSenderMacListed == myNodeID)
            {
                return;
            }

            // add the message payload to the circular queue
            //  after checking index -- here we overwrite messages after
            //  maxIncomingMsgQSize number of messages have  been received (indexing is from 0 to max size)
            if (nextMsgPayloadToAddToQNum == maxIncomingMsgQSize)
            {
                nextMsgPayloadToAddToQNum = 0;
            }
            msgPayloadQueue[nextMsgPayloadToAddToQNum] = recvMP;
            nextMsgPayloadToAddToQNum++;
            //this next says that there is at least one message in the queue
            //  that can be processed
            if (nextMsgPayloadToProcessNum == -1)
            {
                nextMsgPayloadToProcessNum = 0;
            }
        }//end TheMsgsReceived

        public static UInt16 ToUInt16(byte[] value, int index = 0)
        {
            return (UInt16)(value[0 + index] << 0 | value[1 + index] << 8);
        }

        public static byte[] GetBytes(UInt16 value)
        {
            //try alternate code
            byte[] returnByteArray = { 0, 0 };
            returnByteArray[0] = (byte)(value & 0xFF);
            Debug.Print(" in getBytes byte [0] is returnByteArray[0]" + returnByteArray[0]);
            returnByteArray[1] = (byte)((value >> 8) & 0xFF);
            Debug.Print(" in getBytes byte [1] is returnByteArray[0]" + returnByteArray[1]);
            // avoid the use of new
            //return new byte[2] { (byte)(value & 0xFF), (byte)((value >> 8) & 0xFF) };
            return returnByteArray;
        }
    
    }
}
