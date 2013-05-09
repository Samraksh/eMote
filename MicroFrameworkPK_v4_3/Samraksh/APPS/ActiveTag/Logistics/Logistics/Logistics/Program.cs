using System;
using Microsoft.SPOT;
using Samraksh.SPOT.Net;
using Samraksh.SPOT.Net.Mac;

using System.Threading;

namespace Logistics
{
    public enum MessageDestinationType
    {
        BaseStation,
        LocalNeighborhoodNode,
        SpecificNonBaseNode,  //use when intended destniation is *not* a local neighbor node
        AllNodes
    } ;


    /** mimic an enum's info with static members and methods */
    /*
   public class MessageDestinationType
   {
       //static String BaseStation = "BaseStation" ;
       //static String LocalNeighborhoodNode  = "LocalNeighborhoodNode" ;
       //static String SpecificNonBaseNode = "SpecificNonBaseNode" ;
       //static String AllNodes = "AllNodes" ;
       static byte BaseStation_num = 0;
       static byte LocalNeighborhoodNode_num = 1;
       static byte SpecificNonBaseNode_num = 2;
       static byte AllNodes_num = 3;

       static byte theDestinationType_num  ;   //NOTE this is not initialized -- must be done thru setter method

       public static void setDestinationType_num(byte mdt_num) 
       {
           theDestinationType_num = mdt_num;
       }

       public static byte getDestinationType_num()
       {
           return theDestinationType_num ;
       }
   } 
    */


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


    /**
     public class MessagePayloadType
     {
         static byte SnapShotRequest_num = 0;
         static byte SnapShotResponse_num = 1;
         static byte CriticalValueExceeded_num = 2;
         static byte NodeLeftNeighborhood_num = 3;
         static byte AreYouAliveRequest_num = 0;
         static byte AreYouAliveResponse_num = 1;
         static byte RteInfoReqFromBase_num = 2;
         static byte RteInfoRespToBase_num = 3;

         static byte theMessagePayloadType_num;   //NOTE this is not initialized -- must be done thru setter method

         public static void setMessagePayloadType_num(byte mpt_num)
         {
             theMessagePayloadType_num = mpt_num;
         }
         public static byte getMessagePayloadType_num()
         {
             return theMessagePayloadType_num;
         }
     }//end MessagePayload Type
    */


    public enum MessageStrings
    {
        IAmAlive,
        AreYouAlive,
        SendSnapshotRequest,
        SendRteInfoRequest
    } ;

    /**
    public class MessageStrings
    {
        static byte IAmAlive_num = 0;
        static byte AreYouAlive_num = 1;
        static byte SendSnapshotRequest_num = 2;
        static byte SendRteInfoRequest_num = 3;

        static byte theMessageString_num;   //NOTE this is not initialized -- must be done thru setter method

        public static void setMessageStrings_num(byte ms_num)
        {
            theMessageString_num = ms_num;
        }
        public static byte getMessageString_num()
        {
            return theMessageString_num;
        }
    }//end MessageStrings
   */



    public enum MessageAuthorType
    {
        BaseStationNodeAuthor,
        NeighborNodeAuthor,
        SpecificNodeAuthor
    } ;

    /**
    public class MessageAuthorType
    {
        static byte BaseStationNodeAuthor_num = 0;
        static byte NeighborNodeAuthor_num = 1;
        static byte SpecificNodeAuthor = 2;
        
        static byte theMessageAuthorType_num;   //NOTE this is not initialized -- must be done thru setter method

        public static void setMessageAuthorType_num(byte mat_num)
        {
            theMessageAuthorType_num   = mat_num;
        }
        public static byte getMessageAuthorType_num()
        {
            return theMessageAuthorType_num ;
        }
    }//end MessageAuthorType
     */


    public enum MessageAuthorFunctionType
    {
        BaseStationNodeFcn,
        FoodNodeFcn,
        MedicalNodeFcn,
        AmmunitionNodeFcn
    } ;

    /**
    public class MessageAuthorFunctionType
    {
        static byte BaseStationNodeFcn_num = 0;
        static byte FoodNodeFcn_num = 1;
        static byte MedicalNodeFcn_num = 2;
        static byte AmmunitionNodeFcn_num = 3;

        static byte theMessageAuthorFunctionType_num;   //NOTE this is not initialized -- must be done thru setter method

        public static void setMessageAuthorFunctionType_num(byte maft_num)
        {
            theMessageAuthorFunctionType_num = maft_num;
        }
        public static byte getMessageAuthorFunctionType_num()
        {
            return theMessageAuthorFunctionType_num;
        }
    } //end MessageAuthorFunctionType
    */


    public enum IntendedRecipientMacIsSpecified
    {
        Yes,
        No
    } ;


    /**
    public class IntendedRecipientMacIsSpecified
    {
        static byte Yes_num = 0;
        static byte No_num = 1;
        
        static byte theIntendedRecipientMacIsSpecified_num;   //NOTE this is not initialized -- must be done thru setter method

        public static void setIsIntendedRecipientMacSpecified_num(byte is_num)
        {
            theIntendedRecipientMacIsSpecified_num = is_num;
        }
        public static byte getIsIntendedRecipientMacIsSpecified_num()
        {
            return theIntendedRecipientMacIsSpecified_num ;
        }
    } //end IntendedRecipientMacIsSpecified
    */




    public class MessagePayload
    {
        MessagePayloadType msgPType;
        MessageDestinationType msgDestinationType;
        MessageAuthorFunctionType msgAuthFuncType;
        MessageAuthorType msgAuthType;

        UInt16 byteLengthOfEntireMessage;
        byte[] messagePayloadContent = new byte[83];
        byte[] messagePayloadBuffer = new byte[100];
        UInt16 numberBytesInContent;  //this is the true payload of the message, not the APPlication LEVEL headers in this class

        UInt16[] messagePayloadAsFieldArray = new UInt16[60];   // a stencil for better printing out of the application level payload,
        //  applicable to app level payloads of up to 60 bytes

        UInt16 senderMac;
        UInt16 msgNoFromSenderMac;
        UInt16 origSenderMac;
        UInt16 byteOffset = 17;  //known from structure of message payloadfields

        IntendedRecipientMacIsSpecified intendedRecMacIsSpecified;
        UInt16 intendedRecipientMacAddress = (UInt16)0;  //this is the default value if it is not set

        //for use in byte manipulation method
        static byte[] returnByteArray = { 0, 0 };

        //next used in manipulations in the methods
        byte[] array2 = new byte[2];
        byte[] arr2 = new byte[2];
        byte[] holdingArr = new byte[2];
        UInt16 theNumBytesInContent;
        byte[] arrayForContentPayload = new byte[100];   //changed to 100 from 200 to meet 15.4 limits
        int i, j0, j1;



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


        public void setDestinationType(MessageDestinationType _md)
        {
            msgDestinationType = _md;
        }
        public MessageDestinationType getDestinationType()
        {
            return msgDestinationType;
        }

        public void setMessageAuthorFunctionType(MessageAuthorFunctionType _maft)
        {
            msgAuthFuncType = _maft;
        }
        public MessageAuthorFunctionType getMessageAuthorFunctionType()
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
            //Debug.Print(" in setMsgPayloadInBytesUsingFields"); 
            //byte[] arr2 = new byte[2] ; //avoid use static 
            arr2[0] = 0;
            arr2[1] = 1;
            arr2 = GetBytes(getLengthOfEntireMessage());
            messagePayloadBuffer[0] = arr2[0];
            //Debug.Print(" in setMsgPayloadInBytesUsingFields messagePayloadBuffer[0]" + arr2[0]); 

            messagePayloadBuffer[1] = arr2[1];
            //Debug.Print("  in setMsgPayloadInBytesUsingFieldsmessagePayloadBuffer[1]" + arr2[1]); 
            arr2 = GetBytes(getSenderMac());
            messagePayloadBuffer[2] = arr2[0];
            //Debug.Print(" in setMsgPayloadInBytesUsingFields messagePayloadBuffer[2]" + arr2[0]); 
            messagePayloadBuffer[3] = arr2[1];
            //Debug.Print("  in setMsgPayloadInBytesUsingFields messagePayloadBuffer[3]" + arr2[1]); 
            arr2 = GetBytes(getMsgNumberFromSenderMac());
            messagePayloadBuffer[4] = arr2[0];
            // Debug.Print("  in setMsgPayloadInBytesUsingFields messagePayloadBuffer[4]" + arr2[0]); 
            messagePayloadBuffer[5] = arr2[1];
            //Debug.Print(" in setMsgPayloadInBytesUsingFields messagePayloadBuffer[5]" + arr2[1]); 
            arr2 = GetBytes(getOriginalSenderMac());
            messagePayloadBuffer[6] = arr2[0];
            // Debug.Print("in setMsgPayloadInBytesUsingFields  messagePayloadBuffer[6]" + arr2[0]); 
            messagePayloadBuffer[7] = arr2[1];
            //Debug.Print(" in setMsgPayloadInBytesUsingFields messagePayloadBuffer[7]" + arr2[1]);
            messagePayloadBuffer[8] = (byte)getDestinationType();
            //Debug.Print(" in setMsgPayloadInBytesUsingFields messagePayloadBuffer[8]" + messagePayloadBuffer[8]);
            messagePayloadBuffer[9] = (byte)getMessagePayloadType();
            // Debug.Print(" in setMsgPayloadInBytesUsingFields messagePayloadBuffer[9]" + messagePayloadBuffer[9]); 
            messagePayloadBuffer[10] = (byte)getMessageAuthorType();
            //Debug.Print(" in setMsgPayloadInBytesUsingFields messagePayloadBuffer[10]" + messagePayloadBuffer[10]); 
            messagePayloadBuffer[11] = (byte)getMessageAuthorFunctionType();
            //Debug.Print(" in setMsgPayloadInBytesUsingFields messagePayloadBuffer[11]" + messagePayloadBuffer[11]);
            messagePayloadBuffer[12] = (byte)getIsIntendedRecipientMacSpecified();
            //Debug.Print("in setMsgPayloadInBytesUsingFields  messagePayloadBuffer[12]" + messagePayloadBuffer[12]); 
            arr2 = GetBytes(getIntendedRecipientMacAddress());
            messagePayloadBuffer[13] = arr2[0];
            // Debug.Print(" in setMsgPayloadInBytesUsingFields messagePayloadBuffer[13]" + arr2[0]); 
            messagePayloadBuffer[14] = arr2[1];
            // Debug.Print(" in setMsgPayloadInBytesUsingFields messagePayloadBuffer[14]" + arr2[1]); 
            arr2 = GetBytes(getNumberBytesInContent());
            messagePayloadBuffer[15] = arr2[0];
            //Debug.Print(" in setMsgPayloadInBytesUsingFields messagePayloadBuffer[15]" + arr2[0]); 
            messagePayloadBuffer[16] = arr2[1];
            // Debug.Print(" in setMsgPayloadInBytesUsingFields messagePayloadBuffer[16]" + arr2[1]);
            // Debug.Print("in setMsgPayloadInBytesUsingFields numberOfBytesInContent " + numberBytesInContent);
            for (i = 0; i < numberBytesInContent; i++)
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

            for (i = 0; i < _size; i++)
            {
                messagePayloadContent[i] = contentArr[i];
                //Debug.Print(" in setMsgPayloadContent - in bytes  the content byte [i]  = " + i + "  " + messagePayloadContent[i]);
            }
            //for (int i = 0; i < 83; i++)
            //{
            //     Debug.Print(" i and messagePayloadContent[i] = " + i + "  " + messagePayloadContent[i]);
            // }
        }


        //  contentArr is the msg payload content expressed in bytes
        //   this is used for the received payload of the received message (as opposed to the sending message)
        // THIS IS AN example for inserting a template/stencil over the received message payload to have it print out
        //   as something other than bytes.
        //  NOTE THAT THE stencil is assuming a payload of less than the 83 bytes that is supported.
        //     It is using 60 bytes -- an even number of bytes, less then the max amt of Application Level payload that is supported 
        public void setMsgPayloadContentAsFieldArray(UInt16 _size, byte[] contentArr)
        {
            //byte[] holdingArr = new byte[2];  now using a static
            holdingArr[0] = 0;
            holdingArr[1] = 0;

            for (i = 0; i < _size; i++)
            {
                j0 = 2 * _size;
                j1 = 2 * _size + 1;
                holdingArr[0] = contentArr[j0];
                Debug.Print("setMsgPayloadContentAsFieldArray holdingArr[i] " + i + " " + holdingArr[i]);
                holdingArr[1] = contentArr[j1];
                Debug.Print("setMsgPayloadContentAsFieldArray holdingArr[i] " + i + " " + holdingArr[i]);
                messagePayloadAsFieldArray[i] = ToUInt16(holdingArr, 0);
                Debug.Print("setMsgPayloadContentAsFieldArray messagePayloadAsFieldArray[i] " + i + " " + messagePayloadAsFieldArray[i]);
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
            // byte[] array2 = { 0, 0 };
            array2[0] = 0;
            array2[1] = 0;
            setLengthOfEntireMessage(len);
            Debug.Print(" in setPayloadFieldsFromMsgByteArray getLengthOFEntireMessage from method call = " + getLengthOfEntireMessage());

            //Debug.Print(" in setPayloadFieldsFromMsgByteArray = " );
            array2[0] = msgIn[2];
            array2[1] = msgIn[3];
            Debug.Print(" setPayloadFieldsFromMsgByteArray senderMac[0] = " + array2[0]);
            Debug.Print("  setPayloadFieldsFromMsgByteArray senderMac[1] = " + array2[1]);
            setSenderMac(ToUInt16(array2, 0));

            array2[0] = msgIn[4];
            array2[1] = msgIn[5];
            Debug.Print(" setPayloadFieldsFromMsgByteArray msgNoFromSenderMac[0] = " + array2[0]);
            Debug.Print(" setPayloadFieldsFromMsgByteArray msgNoFromSenderMac[1] = " + array2[1]);
            setMsgNumberFromSenderMac(ToUInt16(array2, 0));
            Debug.Print(" setPayloadFieldsFromMsgByteArray msgNoFromSenderMac = " + msgNoFromSenderMac);

            array2[0] = msgIn[6];
            array2[1] = msgIn[7];
            setOriginalSenderMac(ToUInt16(array2, 0));
            Debug.Print("setPayloadFieldsFromMsgByteArray origSenderMac = " + origSenderMac);

            setDestinationType((MessageDestinationType)msgIn[8]);
            Debug.Print(" setPayloadFieldsFromMsgByteArray DestinationType  " + msgDestinationType);
            setMessagePayloadType((MessagePayloadType)msgIn[9]);
            Debug.Print("setPayloadFieldsFromMsgByteArray MessagePayloadType " + msgPType);
            setMessageAuthorType((MessageAuthorType)msgIn[10]);
            Debug.Print("setPayloadFieldsFromMsgByteArray MessageAuthorType " + msgAuthType);
            setMessageAuthorFunctionType((MessageAuthorFunctionType)msgIn[11]);
            Debug.Print(" setPayloadFieldsFromMsgByteArray MessageAuthorFunctionType " + msgAuthFuncType);


            // array2[0] = msgIn[10];
            // array2[1] = msgIn[11];
            // setOriginalSenderMac(ToUInt16(array2, 0));
            // Debug.Print("setPayloadFieldsFromMsgByteArray origSenderMac = " + origSenderMac);
            // array2[0] = msgIn[12];
            // array2[1] = msgIn[13];
            setIsIntendedRecipientMacSpecified((IntendedRecipientMacIsSpecified)msgIn[12]);
            Debug.Print(" setPayloadFieldsFromMsgByteArray intendedRecMacIsSpecified " + intendedRecMacIsSpecified);
            array2[0] = msgIn[13];
            array2[1] = msgIn[14];
            setIntendedRecipientMacAddress(ToUInt16(array2, 0));
            Debug.Print(" setPayloadFieldsFromMsgByteArray intended recipient mac = " + intendedRecipientMacAddress);

            //Don't know a priori how big the payload content field since reconstructing from 
            array2[0] = msgIn[15];
            array2[1] = msgIn[16];
            setNumberBytesInContent(ToUInt16(array2, 0)); //this is the length of the content (also called payload of msg)

            theNumBytesInContent = getNumberBytesInContent();
            Debug.Print(" in setPayloadFieldsFromMsgByteArray  theNumBytesInContent from method call  " + theNumBytesInContent);
            for (i = 0; i < theNumBytesInContent; i++)
            {
                try
                {
                    arrayForContentPayload[i] = msgIn[byteOffset + i];
                }
                catch
                {
                    Debug.Print(" in setPayloadFieldsFromMsgByteArray  i = " + i);
                    Debug.Print(" in setPayloadFieldsFromMsgByteArray  theNumBytesInContent " + theNumBytesInContent);
                }

            }
            //need to capture the Msg payload field in bytes first, with the correct byte offset done
            //  this is setting the payload content field -- in bytes -- for the received message
            setMsgPayloadContent(theNumBytesInContent, arrayForContentPayload);

            //from here you could put a template over the received message in bytes
            //  this is up to the application design
            //  example,  call   setMsgPayloadContentAsFieldArray
        }


        //see forums.netduino.com/index.php?/topic/308-bitconverter
        static public UInt16 ToUInt16(byte[] value, int index = 0)
        {
            return (UInt16)(value[0 + index] << 0 | value[1 + index] << 8);
        }

        static public byte[] GetBytes(UInt16 value)
        {
            //try alternate code
            //byte[] returnByteArray = { 0, 0 };
            returnByteArray[0] = (byte)(value & 0xFF);
            //Debug.Print(" in getBytes byte [0] is returnByteArray[0]" + returnByteArray[0]);
            returnByteArray[1] = (byte)((value >> 8) & 0xFF);
            //Debug.Print(" in getBytes byte [1] is returnByteArray[0]" + returnByteArray[1]);
            // avoid the use of new
            //return new byte[2] { (byte)(value & 0xFF), (byte)((value >> 8) & 0xFF) };
            return returnByteArray;
        }
    } //end MessagePayload class


    //###########################################################################
    //###########################################################################
    //###########################################################################

    public class LogisticsApp
    {

        //declaring all variables global

        //these next are set as the conditions for testing
        bool wasCriticalTempExceeded = true;
        bool critMsgFlag = false;
        bool shouldProcessThisMessage = false;
        //  end conditions for testing

        //this assumes up to 10 nodes in the WSN
        static int MAX_WSN_NODES = 10;
        UInt16[] senderMACIDArray = new UInt16[MAX_WSN_NODES];
        UInt16[] highestMsgIDFromEachNodeArray = new UInt16[MAX_WSN_NODES];

        MessagePayload mp = new MessagePayload();
        MessagePayload recvMP = new MessagePayload();

        MessagePayload sendMP = new MessagePayload();

        //used for field setting in the message payload to be sent
        MessageAuthorFunctionType myMAFT = MessageAuthorFunctionType.MedicalNodeFcn;
        MessagePayloadType theMPT;
        MessageAuthorType theMAT;
        MessageAuthorFunctionType theMAFT;
        MessageDestinationType theMsgDestType;
        //UInt16 intendedRecipientMacAddress; //never used 
        UInt16 theSenderMacAddress;
        // this next is incremented every time this node sends out a message
        //  start it at message number 1 
        UInt16 theMsgNoFromThisNodeSendingMac = 1;
        // these next are helper globals
        UInt16 theOriginalSenderMacListed;
        UInt16 theMostRecentSenderMacListed;

        //helper variables
        int totalMsgLen;
        // aTwoByteArray as a helper array in this demo work
        byte[] a2ByteArray = new byte[2];
        // other helper bytes arrays
        byte[] contentArray = new byte[2];
        byte[] avgTemperatureArray = new byte[2];

        // for use in local getBytes method
        byte[] returnByteArray2 = { 0, 0 };

        // this next used as a holder for the msg number from a rcvd message that enables us to check
        //   by associating with its sender mac if that msg has been received before
        UInt16 theMsgNoFromRecvdMsgMac;

        //  set up the payload queue for 20 messages that can be stored
        static byte maxIncomingMsgQSize = 20;
        MessagePayload[] msgPayloadQueue = new MessagePayload[20];
        sbyte nextMsgPayloadToProcessNum = -1;  //range -128 to 127
        byte nextMsgPayloadToAddToQNum = 0;

        // the message payload content (Payload content to the LOWER Netowrk layers, but not to the Application level layer) may be as large as 100
        //  this is used on the incoming messages
        byte[] byteBuffer = new byte[100];

        //the routing class
        static Networking theNetworking;

        //used to wait for robust call to neighbor table, time in ms
        //  
        static UInt16 timeToWaitForNghbrTableAfterMacInit = 200;

        static Timer sensorCheckTimer;

        //the callback 
        static ReceiveCallBack myReceive;

        static NeighbourhoodChangeCallBack myNeighbourChanged;

        static Samraksh.SPOT.Net.Message lmessage;

        // instantiating the CSMA
        static Samraksh.SPOT.Net.Mac.CSMA myCSMAMac;
        // instantiating the struct MacConfiguration
        static Samraksh.SPOT.Net.Mac.MacConfiguration macConfig = new MacConfiguration();
        static UInt16 myNodeID;      //this is the mac ID of this emote (and app) node, used as Id
        static UInt16 myMacAddress; // this is the mac address of this emote (and app node) 

        //static Samraksh.SPOT.Net.Mac.Neighbor myNeighbor;
        static Samraksh.SPOT.Net.Mac.NeighborTable myNeighborTable;
        static Samraksh.SPOT.Net.Mac.NeighborTable myPrevNeighborTable;

        static Samraksh.SPOT.Net.Mac.NeighborTable myNeighborTable_currentPull;
        //next refers to MAC Addr that is used as payload in certain types of messages
        static byte MAX_NUM_MACADDR_IN_PAYLOAD_INFO = 20;  //this is probably more than is atually going to be needed, but  less than the available payload space


        // this is the cycle on which neighbors are checked
        //static UInt16 recheckNeighborsReActivity = 0;
        static UInt16 maxRecheckCycle = 5000;  //originally set at 5000 but reset for testing
        UInt16 recheckNumber = 0;
        //static UInt16 cycleTimeSendOutMissingNeighbor = 4500;  //to be used in conjunction with the maxREcheckCycle 


        static UInt16[] possibleMissingNeighborMacIDs = new UInt16[20];
        static UInt16 numberOfPossibleMissingNeighbors = 0;
        static MessagePayload[] msgForPossibleMissingNghbrs = new MessagePayload[20];
        static UInt16[] likelyMissingNeighborMacIDs = new UInt16[20];

        // related to sensor provided info and used in two types of messages
        UInt16 tempFromSensor = 70;  //degrees celsius this should be the result of a sensor reading
        UInt16 exceededTemp = 95;

        // This is only used if there is a queue for
        //  outgoing messages  in a design approach that uses timers instead of a robund robin loop
        //Thread mySendMsgThread;

        //helper variables
        int i, j, indexMax, i2, jFlag;
        bool breakFlag, breakFlag2;

        //more helper variables
        byte numPrevNghbrs, numCurrentNghbrs;
        UInt16 macID_prev, macID_curr, senderAddr, theMissingMac;
        int msgLenEntire;
        int indexValOfSender;
        byte i3;
        UInt16 mpSize;

        void HandleNeighbourChanged(UInt16 neighboursChanged)
        {
        }

        void init()
        {
            Debug.Print("Initializing: Logistics App");
            Thread.Sleep(1000);

            // macConfiguration initialized 
            macConfig.CCA = true;
            macConfig.NumberOfRetries = 0;  //was originally 2
            macConfig.CCASenseTime = 140; //sensing time in micro seconds - was originally 200
            macConfig.BufferSize = 8;
            macConfig.RadioID = (byte)1;
            macConfig.NeighbourLivelinesDelay = 50; // Livelinessdelay in seconds 

            //assign callback delegate to function
            myReceive = MsgsReceived;

            myNeighbourChanged = HandleNeighbourChanged;

            CSMA.Configure(macConfig, myReceive, myNeighbourChanged);

            Debug.Print("Initializing:  CSMA MAC ...");
            try
            {
                myCSMAMac = CSMA.Instance;
            }
            catch (Exception e)
            {
                Debug.Print(e.ToString());
            }
            Debug.Print("CSMA Init done.");
            myMacAddress = myCSMAMac.GetAddress();
            Debug.Print("My mac address is :  " + myMacAddress.ToString());

            // Initialize networking with csma mac object 
            theNetworking = new Networking(myCSMAMac);

            //comment out while checking size issue
            myNodeID = myMacAddress;
            //myNodeID = 6;

            //COMMENTING OUT WHILE NOT HAVING NEIGHBORS IN TESTING PHASE
            //waiting for MAC to be robust before acquiring the neighbor table
            Thread.Sleep(timeToWaitForNghbrTableAfterMacInit);

            sensorCheckTimer = new Timer(sensorCheck, null, 200, 2000);

            //already have my node ID from the setup 
            //  but still must get the neighbor tables  set up
            // THIS HAS CHANGED from VER 1.0.0.1 --so commenting out here
            //myNeighborTable = myCSMAMac.GetNeighborTable();
            // myPrevNeighborTable = myNeighborTable;
            // myNeighborTable = myCSMAMac.GetNeighborTable();
        }// end init

        public void sensorCheck(Object state)
        {
            CheckIfSensorValExceededCritical();
        }

        public static void Main()
        {
            // helper variables 

            //int i, j, jFlag,  indexMax , i2  ;
            //bool breakFlag, breakFlag2   ;

            LogisticsApp la = new LogisticsApp();

            //Debug.Print( " In Main" ) ;
            la.init();


            while (true)
            {
                la.shouldProcessThisMessage = false;  //this is a resetting to start at the next time through the loop
                //look at the next message to process and set up the circular q limit
                if (la.nextMsgPayloadToProcessNum != -1)
                {
                    la.mp = la.msgPayloadQueue[la.nextMsgPayloadToProcessNum];
                    if (la.mp != null)
                    {
                        la.nextMsgPayloadToProcessNum++;
                        if (la.nextMsgPayloadToProcessNum == MAX_WSN_NODES - 1)   //resets to the start of the circular queue
                        {
                            la.nextMsgPayloadToProcessNum = 0;
                        }
                    }
                }  //end  if (nextMsgPayloadToProcessNum != -1 )

                if (la.mp != null)
                {
                    //check to see if that message was sent before by getting the message id of 
                    //  the recvd msg
                    la.theSenderMacAddress = la.mp.getSenderMac();
                    la.theMsgNoFromRecvdMsgMac = la.mp.getMsgNumberFromSenderMac();
                    la.indexMax = -1;
                    la.breakFlag = false;
                    //this is to find the number of mac ids already in the array
                    //  there won't be any until after the first message
                    //  so let us check to see if this is the first message received from any node
                    while (la.breakFlag == false)
                    {
                        for (la.i = 0; la.i < MAX_WSN_NODES; la.i++)
                        {
                            if (la.senderMACIDArray[la.i] == 0)
                            {
                                la.indexMax = la.i;
                            }
                            if (la.indexMax != -1) la.breakFlag = true;
                        }
                    }
                    if (la.indexMax == 0)  //this is the first received by this node from Any sender (that is not the node irself)
                    {
                        //this info is related to the sender
                        la.senderMACIDArray[0] = la.theSenderMacAddress;
                        la.highestMsgIDFromEachNodeArray[0] = la.theMsgNoFromRecvdMsgMac;
                        la.shouldProcessThisMessage = true;
                    }
                    //looking to see if the sender is already known
                    la.breakFlag2 = false;
                    la.i2 = 0;
                    if (la.shouldProcessThisMessage == false)  //have received msgs previous to this
                    {
                        while ((la.breakFlag2 == false) && (la.i2 < la.indexMax + 1))
                        {  //this info is related to the most recent sender
                            if (la.senderMACIDArray[la.i2] == la.theSenderMacAddress)
                            {
                                la.breakFlag2 = true;
                            }
                            if (la.breakFlag2 == false)
                            {
                                la.i2++;
                            }
                        } //end while ((breakFlag2 == false) && (i2 < indexMax + 1))
                        if (la.breakFlag2 == false)  //this is the first message from a new node
                        {
                            if (la.indexMax + 1 < MAX_WSN_NODES)  //if there are more than max nodes, their msgs never get thru
                            {
                                la.senderMACIDArray[la.indexMax + 1] = la.theSenderMacAddress;
                                la.highestMsgIDFromEachNodeArray[la.indexMax + 1] = la.theMsgNoFromRecvdMsgMac;
                                la.shouldProcessThisMessage = true;
                            }
                        }
                        if (la.breakFlag2 == true) //have recv'd msg from this node before,must see if new msg has higher number
                        {
                            if (la.highestMsgIDFromEachNodeArray[la.i2] < la.theMsgNoFromRecvdMsgMac)
                            {
                                la.shouldProcessThisMessage = true;
                            }
                            if (la.shouldProcessThisMessage == true)
                            {
                                la.highestMsgIDFromEachNodeArray[la.i2] = la.theMsgNoFromRecvdMsgMac;
                            }
                        } //end  (breakFlag2 == true)
                    } //end if (shouldProcessThisMessage == false) 
                } //end if (mp != null)


                //so if the message is a new one, it can be processed
                //  
                if (la.shouldProcessThisMessage == true)
                {
                    la.theMPT = la.mp.getMessagePayloadType();
                    la.theMAT = la.mp.getMessageAuthorType();
                    la.theMAFT = la.mp.getMessageAuthorFunctionType();
                    la.theMsgDestType = la.mp.getDestinationType();

                    if (la.theMsgDestType == MessageDestinationType.BaseStation)
                    {
                        //this message is meant to be forwarded to the base station
                        // must call the router/networking layer
                        //  since this message is not processed further, the original sender's mac address
                        //  is embedded within the payload message
                        la.mp.setSenderMac(myNodeID);
                        la.mp.setMsgNumberFromSenderMac(la.theMsgNoFromThisNodeSendingMac);
                        la.theMsgNoFromThisNodeSendingMac++;
                        // all other fields of mp should remain the same, so can forward
                        //theNetworking.forwardMessageToBase(la.mp, myCSMAMac);

                        la.mp.setMsgPayloadInBytesUsingFields();
                        //now invoke a Routing method to send this along

                        Debug.Print("Check Point 1\n");

                        theNetworking.sendingMsg(la.mp.getMessagePayloadBuffer(), la.mp.getByteOffset(), (UInt16)la.mp.getLengthOfEntireMessage());
                    }


                    //Messages meant for all nodes are also forwarded 
                    //    in addition to being answered by the current node
                    //these are the only message destination types that are sent to all nodes
                    if (la.theMsgDestType == MessageDestinationType.AllNodes)
                    {
                        //I am a sensor node and must look further into the message
                        //  to determine what msg to compose
                        if (la.theMPT == MessagePayloadType.SnapShotRequest)
                        {
                            //forward this to other nodes since it is from base
                            la.forwardMsgFromBaseInFlood(la.mp);

                            //this method will compose the message and send it to the networking layer
                            la.SnapshotRequestFulfill();
                        }

                        /**  THIS message type not handled bacause MAC info of neighbors not readily available
                         //  with the new NET DLL v 1.0.0.2
                        if (la.theMPT == MessagePayloadType.RteInfoReqFromBase)
                        {
                            //and forward this to other nodes since it is from base
                            la.forwardMsgFromBaseInFlood(la.mp);

                            //this method will compose the message and send it to the netowrking layer
                            la.RteInfoRequestFulfill();
                        }
                         */
                    }

                    if (la.theMsgDestType == MessageDestinationType.LocalNeighborhoodNode)
                    {
                        //am in the local neighborhood and must inspect message
                        // the only message types expected is an Are You Alive request message
                        //  or an I Am Alive response message
                        if (la.theMPT == MessagePayloadType.AreYouAliveRequest)
                        {
                            //this should be being sent to this node's MAC address specifically,
                            //  AS PER THE DESIGN
                            //  check this here and drop any response if the message not meant for this node
                            if (la.mp.getIsIntendedRecipientMacSpecified() == IntendedRecipientMacIsSpecified.Yes)
                            {
                                //UInt16 theIntendedMacAddress = la.mp.getIntendedRecipientMacAddress();
                                //if (theIntendedMacAddress == myNodeID)
                                if (la.mp.getIntendedRecipientMacAddress() == myNodeID)
                                {
                                    //this node is the intended recipient of an are you alive message
                                    //compose response to keep alive message
                                    //  the SEMANTICS of this is to just send a response message to the sending node
                                    //  the response is content free
                                    //  determine the mac address of the sender
                                    // ONLY send a response if the sender's mac is known -- which is should be from
                                    //  the incoming message

                                    if (la.mp.getSenderMac() != 0)
                                    {
                                        la.sendBackAliveResponse(la.mp.getSenderMac());
                                    }
                                }
                            }
                        }
                        else if (la.theMPT == MessagePayloadType.AreYouAliveResponse)
                        {
                            la.ProcessAliveResponseFromNeighbor(la.mp);
                        }
                    }

                    //SEMANTICS to be used in cases in which the specific non base node is not a neighbor
                    //  in the context of liveness related messages.
                    //  that is, the actual mac destination might be a neighbor, but the current reciever just forward out
                    //   no need to see if the intended mac is actually in this new sender's nghbd. 
                    if (la.theMsgDestType == MessageDestinationType.SpecificNonBaseNode)
                    {
                        //is the message meant for me -- if not then forward it out
                        // but first check to see if there is an actual recipient mac id specified
                        //IntendedRecipientMacIsSpecified yesOrNo = la.mp.getIsIntendedRecipientMacSpecified();
                        //if (yesOrNo == IntendedRecipientMacIsSpecified.No)
                        if (la.mp.getIsIntendedRecipientMacSpecified() == IntendedRecipientMacIsSpecified.No)
                        {
                            //then the message is in error and 
                            //nothing can be done if intended recipient is not specified
                            break;
                        }
                        else
                        {
                            if (la.mp.getIntendedRecipientMacAddress() != myNodeID)
                            { //  message is not meant for me -- forward it on
                                //  forwarding is the primary routing concept used by the sensor nodes
                                //  implementation of forwarding is flooding
                                //change the sender mac and msg number
                                la.mp.setSenderMac(myNodeID);
                                la.mp.setMsgNumberFromSenderMac(la.theMsgNoFromThisNodeSendingMac);
                                la.theMsgNoFromThisNodeSendingMac++;
                                //all other info should remain the same
                                la.forwardMsgToSpecificNode(la.mp);
                            }
                            else if (la.mp.getIntendedRecipientMacAddress() == myNodeID)
                            {
                                // message is meant for me
                                // the only type of message this could be is one from a base station
                                //  that is specifically asking this node to send an update or
                                //  this node to send rte type info of which nodes are its neightbors
                                if (la.mp.getMessageAuthorType() == MessageAuthorType.BaseStationNodeAuthor)
                                {
                                    if (la.mp.getMessagePayloadType() == MessagePayloadType.SnapShotRequest)
                                    {
                                        la.SnapshotRequestFulfill();
                                    }
                                    //this next MSG type not handled until eqivalent method for obtaining MAC addresses is
                                    //   available in new NET DLL -- not clear in v 1.0.0.2
                                    //else if (la.mp.getMessagePayloadType() == MessagePayloadType.RteInfoReqFromBase)
                                    //{
                                    //    la.RteInfoRequestFulfill();
                                    //}
                                }
                            }
                        }// there are no other types of messages that would be sent to a specific node 
                        // that were not handled by the neighbor alive type message
                    }
                }//end of processing the messages


                //now check to see if max temperature was exceeded
                //la.CheckIfSensorValExceededCritical();
                //if (la.critMsgFlag == true && la.recheckNumber == maxRecheckCycle)
                if(la.critMsgFlag)
                {
                    la.doCriticalValueExceededMessage();
                    la.recheckNumber = 0; //reset to a new loop for this
                    la.critMsgFlag = false;
                }

                /* COMMENTED OUT FOR TESTING with ONLY SURROGATE BASE AND THIS NODE -- NO NEIGHBORS 
                //Look at updated neighbor table to see if neighbors are still active
                //   Could put this on a timer
                if (recheckNeighborsReActivity == maxRecheckCycle)   //this has been set to 5000 see global varialbe
                {
                    recheckNeighborsReActivity = 0;
                }
                if (recheckNeighborsReActivity == 0)
                {
                    CheckIfNeighborsActive();
                    // If a neighbor on the list has missed a check then generate an are you alive message
                    GenerateAreYouAliveMessagesForMissingNeighbors();
                }
                recheckNeighborsReActivity++;
                //Check to see if this node has received responses from the
                //  neighbors to which it sent an are you alive request


                //     if there are missing neighbors compose a message saying that  a neighbor left the group
                //     and mark the destination type as the base station
                //     use the router class assist and have this sent on a broadcast
                if (recheckNeighborsReActivity == cycleTimeSendOutMissingNeighbor)
                {
                    ComposeNeighborMissingMessagesToBase();
                }
                */
                //END COMMENTED OUT FOR TESTING with only 2 emotes == a surrogate base and no neighbors

                la.recheckNumber++;

            }  //end while true

        }  //end main





        //HERE: I am looking at a flag to see if sensor val exceeds set to true
        //  but i could poll a temp sensor -- if that was accessible
        //  or I could poll a gpio pin that the sensor was attached to
        //  this polling would have to take place how often ??
        //  I would like to attach an interrupt to the temperature sensor 
        //    and the interrupt would handle
        void CheckIfSensorValExceededCritical()
        {
            //Monitor.Enter(wasCriticalTempExceeded);
            if (wasCriticalTempExceeded == true)
            {
                Debug.Print("The Sensor Value Exceeded on Node " +  myMacAddress.ToString() + "\n");
                critMsgFlag = true;
            }
            //Monitor.Exit(wasCriticalTempExceeded);
        } //end CheckIfSensorValExceededCritical()


        void doCriticalValueExceededMessage()
        {
            // compose a msg for sending to the base station
            //  but we don't know the base station address in this code
            //   if that changes, must change code to reflect specific mac intended destination
            mp.setSenderMac(myNodeID);
            mp.setMsgNumberFromSenderMac(theMsgNoFromThisNodeSendingMac);
            theMsgNoFromThisNodeSendingMac++; //always increment after setting for sending
            mp.setDestinationType(MessageDestinationType.BaseStation);
            mp.setMessagePayloadType(MessagePayloadType.CriticalValueExceeded);
            mp.setMessageAuthorType(MessageAuthorType.SpecificNodeAuthor);
            mp.setMessageAuthorFunctionType(MessageAuthorFunctionType.MedicalNodeFcn);
            mp.setIsIntendedRecipientMacSpecified(IntendedRecipientMacIsSpecified.No);
            mp.setIntendedRecipientMacAddress((UInt16)0);
            mp.setOriginalSenderMac(myNodeID);
            //temp value is message payload content; this is a global whose value is just set (no sensors yet)
            //byte[] byteArray = getBytes(exceededTemp);
            a2ByteArray = getBytes(exceededTemp);
            mp.setMsgPayloadContent(2, a2ByteArray);
            mp.setNumberBytesInContent(2);
            // this next is against C# specs -- but I want to emphasize that the size of what we 
            //  are dealing with is such that there would not be an overflow.
            // UInt16 totalMsgLen = (UInt16) mp.getByteOffset() +(UInt16)  mp.getNumberBytesInContent();
            //  totalMsgLen declared global
            totalMsgLen = (UInt16)mp.getByteOffset() + (UInt16)mp.getNumberBytesInContent();
            mp.setLengthOfEntireMessage((UInt16)totalMsgLen);
            mpSize = mp.getLengthOfEntireMessage();
            mp.setMsgPayloadInBytesUsingFields();
            // Nived.Sivadas - Unsure what the rational behind making a special function was 
            //pass the message payload to the Routing helper
            //theNetworking.sendNodeSpecificMsgToBase(mp.getMessagePayloadBuffer(), mp.getByteOffset(), mpSize, myCSMAMac);
            
            theNetworking.sendingMsg(mp.getMessagePayloadBuffer(), mp.getByteOffset(), mpSize);
        }//end doCriticalValueExceededMessage()



        //NOTE NOTE NOTE
        //  THE CSMA MAC API has changed since this version of 1.0.01
        //  NOW on 1.0.0.2 version of NET DLL
        //  Semantics seem to differ comment this method out

        /**
        //update knowledge of neighbor status
        void CheckIfNeighborsActive()
        {
            myPrevNeighborTable = myNeighborTable;
            myNeighborTable = myCSMAMac.GetNeighborTable();

            //check if any mac on prev list that is missing from current list
            numPrevNghbrs = myPrevNeighborTable.NumberValidNeighbor;

            numCurrentNghbrs = myNeighborTable.NumberValidNeighbor;

            //TODO  Check again once the dll is corrected
            //TODO Confirm semantics of the Neighbor Table

            numberOfPossibleMissingNeighbors = 0;  //reset to zero, do not know how many missing this time
            for (i = 0; i < numPrevNghbrs; i++)
            {
                macID_prev = myPrevNeighborTable.Neighbor[i].MacAddress;
                j = 0;
                jFlag = 0;
                while (j < numCurrentNghbrs && jFlag == 0)
                {
                    macID_curr = myNeighborTable.Neighbor[j].MacAddress;
                    if (macID_curr == macID_prev)
                    {
                        jFlag = 1;  //found a match mac id not missing
                        break;
                    }
                    j++;
                }
                if (jFlag == 0)
                {
                    possibleMissingNeighborMacIDs[numberOfPossibleMissingNeighbors] = macID_prev;
                    // this next called likely is a copy of possible; likely will be reduced ni subsequent code bloc
                    likelyMissingNeighborMacIDs[numberOfPossibleMissingNeighbors] = macID_prev;
                    numberOfPossibleMissingNeighbors++;
                }
            }
        }//end CheckIfNeighborsActive()
        */

        //NOTE:  This next method not tested -- 
        //this next method uses a table of possibleMissingNeighborMacIDs
        //   but the semantics of the NET DLL version 1.0.0.2  
        //    does not offer an API to provide nghbd MACS 
        void GenerateAreYouAliveMessagesForMissingNeighbors()
        {
            for (j = 0; j < numberOfPossibleMissingNeighbors; j++)
            {
                //load all details of message
                mp.setIsIntendedRecipientMacSpecified(IntendedRecipientMacIsSpecified.Yes);
                mp.setIntendedRecipientMacAddress(possibleMissingNeighborMacIDs[j]);
                mp.setDestinationType(MessageDestinationType.LocalNeighborhoodNode);
                mp.setMessageAuthorType(MessageAuthorType.NeighborNodeAuthor);
                mp.setMessageAuthorFunctionType(myMAFT);
                mp.setMessagePayloadType(MessagePayloadType.AreYouAliveRequest);
                // UInt16 theMsg = (UInt16)MessageStrings.AreYouAlive;
                // byte[] byteArray = getBytes(theMsg);
                a2ByteArray = getBytes((UInt16)MessageStrings.AreYouAlive);
                mp.setMsgPayloadContent(2, a2ByteArray);
                mp.setNumberBytesInContent(2);
                msgLenEntire = mp.getByteOffset() + mp.getNumberBytesInContent();
                mp.setLengthOfEntireMessage((UInt16)msgLenEntire);
                mp.setSenderMac(myNodeID);
                mp.setMsgNumberFromSenderMac(theMsgNoFromThisNodeSendingMac);
                theMsgNoFromThisNodeSendingMac++;
                mp.setOriginalSenderMac(myNodeID);
                //convert all payload load to bytes
                mp.setMsgPayloadInBytesUsingFields();

                //now invoke a Routing method to send this along
                // theNetworking.sendQueryAliveMessageToNeighbor(mp, myCSMAMac);
                Debug.Print("Check Point 3\n");
                theNetworking.sendingMsg(mp.getMessagePayloadBuffer(), mp.getByteOffset(), (UInt16)mp.getLengthOfEntireMessage());
            }
        } //end GenerateAliveMessagesForMissingNeighbors


        // this composes a message to the sender to return an
        //   alive response -- no content in this message its all header info
        void sendBackAliveResponse(UInt16 _sendersMacAddr)
        {
            //load details of I am alive message
            mp.setIsIntendedRecipientMacSpecified(IntendedRecipientMacIsSpecified.Yes);
            mp.setIntendedRecipientMacAddress(_sendersMacAddr);
            mp.setDestinationType(MessageDestinationType.LocalNeighborhoodNode);
            mp.setMessageAuthorType(MessageAuthorType.NeighborNodeAuthor);
            mp.setMessageAuthorFunctionType(myMAFT);
            mp.setMessagePayloadType(MessagePayloadType.AreYouAliveResponse);
            // UInt16 theMsg = (UInt16)MessageStrings.IAmAlive;
            // byte[] byteArray = getBytes(theMsg);
            a2ByteArray = getBytes((UInt16)MessageStrings.IAmAlive);
            mp.setMsgPayloadContent(2, a2ByteArray);
            mp.setNumberBytesInContent(2);
            msgLenEntire = mp.getByteOffset() + mp.getNumberBytesInContent();
            mp.setLengthOfEntireMessage((UInt16)msgLenEntire);
            mp.setSenderMac(myNodeID);
            mp.setMsgNumberFromSenderMac(theMsgNoFromThisNodeSendingMac);
            theMsgNoFromThisNodeSendingMac++;
            mp.setOriginalSenderMac(myNodeID);
            //convert all loaded to bytes
            mp.setMsgPayloadInBytesUsingFields();
            //now invoke a Routing method to send this along
            //theNetworking.sendIAmAliveMessageToNeighbor(mp, myCSMAMac);
            Debug.Print("Check Point 4\n");
            theNetworking.sendingMsg(mp.getMessagePayloadBuffer(), mp.getByteOffset(), (UInt16)mp.getLengthOfEntireMessage());

        }//end sendBackAliveResponse

        //process the response message from neighbor
        //   and handle the bookkeeping recording which neighbors responded 
        //  NOTE:  The missing neighbors are the neighbors who were sent an 'are you alive message' 
        void ProcessAliveResponseFromNeighbor(MessagePayload mp)
        {
            senderAddr = mp.getSenderMac();
            //if this mac is responded remove the MAC from the list of likely missing macs

            i = 0;
            indexValOfSender = -1;
            while (i < numberOfPossibleMissingNeighbors && indexValOfSender == -1)
            {
                if (senderAddr == likelyMissingNeighborMacIDs[i])
                {
                    indexValOfSender = i;
                }
                i++;
            }
            if (indexValOfSender > -1)
            {  //this mac sender is not missing -- it responded with a Am ALive to a message query from sender
                //  so take it off of the list 
                likelyMissingNeighborMacIDs[indexValOfSender] = 0;
            }
        }


        void ComposeNeighborMissingMessagesToBase()
        {
            //for each non zero mac address still left on the likely list send a msg to base re missing neighbor
            i = 0;
            while (i < numberOfPossibleMissingNeighbors)
            {
                if (likelyMissingNeighborMacIDs[numberOfPossibleMissingNeighbors] != 0)
                {
                    theMissingMac = likelyMissingNeighborMacIDs[numberOfPossibleMissingNeighbors];
                    mp.setDestinationType(MessageDestinationType.BaseStation);
                    mp.setMessageAuthorType(MessageAuthorType.SpecificNodeAuthor);
                    mp.setMessageAuthorFunctionType(myMAFT);
                    mp.setSenderMac(myNodeID);
                    mp.setMessagePayloadType(MessagePayloadType.NodeLeftNeighborhood);
                    mp.setIsIntendedRecipientMacSpecified(IntendedRecipientMacIsSpecified.No);
                    mp.setIntendedRecipientMacAddress((UInt16)0);
                    mp.setOriginalSenderMac(myNodeID);
                    mp.setMsgNumberFromSenderMac(theMsgNoFromThisNodeSendingMac);
                    theMsgNoFromThisNodeSendingMac++;
                    //byte[] byteArray = getBytes(theMissingMac);
                    a2ByteArray = getBytes(theMissingMac);
                    mp.setMsgPayloadContent(2, a2ByteArray);
                    mp.setNumberBytesInContent(2);
                    msgLenEntire = mp.getByteOffset() + mp.getNumberBytesInContent();
                    mp.setLengthOfEntireMessage((UInt16)msgLenEntire);
                    mp.setMsgPayloadInBytesUsingFields();

                    // send the message
                    //forwardNodeSpecificMsgToBase(byte[ ] byteArrayMP, UInt16 offset, UInt16 mpSize, CSMA myCSMA)
                    //theNetworking.sendNodeSpecificMsgToBase(mp.getMessagePayloadBuffer(), mp.getByteOffset(), mp.getLengthOfEntireMessage(), myCSMAMac);
                    Debug.Print("Check Point 5\n");
                    theNetworking.sendingMsg(mp.getMessagePayloadBuffer(), mp.getByteOffset(), mp.getLengthOfEntireMessage());
                }

            }
        } //end ComposeNeighborMissingMessagesToBase()


        //compose forwarding message to send to a specific destination that is not the base node
        void forwardMsgToSpecificNode(MessagePayload theMP)
        {
            //no need to modify the bulk of the message
            //THESE NEXT MOVED to CALLING METHOD
            //theMP.setSenderMac(myNodeID);
            //theMP.setMsgNumberFromSenderMac(theMsgNoFromThisNodeSendingMac);
            //theMsgNoFromThisNodeSendingMac++;

            theMP.setMsgPayloadInBytesUsingFields();
            //theNetworking.sendMsgToNonBaseSpecificDestination(theMP, myCSMAMac);
            Debug.Print("Check Point 6\n");
            theNetworking.sendingMsg(theMP.getMessagePayloadBuffer(), theMP.getByteOffset(), (UInt16)theMP.getLengthOfEntireMessage());
        }//end forwardMsgToSpecificNode


        //this message is meant to be forwarded to all nodes 
        //  it originates from the base station so must call the router/networking layer
        //  the original sender's mac address --which is the base station -- 
        //  is embedded within the payload message
        //  the current forwarder becomes the sending MAC
        void forwardMsgFromBaseInFlood(MessagePayload theMP)
        {
            //this message is meant to be forwarded to the base station
            // must call the router/networking layer
            //  since this message is not processed further, the original sender's mac address
            //  is embedded within the payload message
            mp.setSenderMac(myNodeID);
            mp.setMsgNumberFromSenderMac(theMsgNoFromThisNodeSendingMac);
            theMsgNoFromThisNodeSendingMac++;
            // all other fields of mp should remain the same, so can forward
            //theNetworking.forwardMessageInFlood(mp, myCSMAMac);
            theMP.setMsgPayloadInBytesUsingFields();
            Debug.Print("Check Point 7\n");
            theNetworking.sendingMsg(theMP.getMessagePayloadBuffer(), theMP.getByteOffset(), (UInt16)theMP.getLengthOfEntireMessage());
        }//end sendMsgFromBaseInFlood


        //compose snapshot message with destination base station
        //  when composed invoke the routing layer to send
        //  This is going to report the temperature
        //  A random value between 40 and 90 degrees will be selected and 
        //  then incorporated into a mesage
        //
        //  TODO   Is the sensor pull or does it report out every so often
        //   TODO  What is teh better way to get the sensor in here?
        // 
        //  This is viewed as a response to a request (another way to consider this would be proactive temperature info sent
        void SnapshotRequestFulfill()
        {
            //generate the content
            mp.setDestinationType(MessageDestinationType.BaseStation);
            mp.setMessageAuthorType(MessageAuthorType.SpecificNodeAuthor);
            mp.setMessageAuthorFunctionType(myMAFT);
            mp.setMessagePayloadType(MessagePayloadType.SnapShotResponse);
            mp.setOriginalSenderMac(myNodeID);
            mp.setSenderMac(myNodeID);
            mp.setIsIntendedRecipientMacSpecified(IntendedRecipientMacIsSpecified.No);
            mp.setIntendedRecipientMacAddress((UInt16)0);
            mp.setMsgNumberFromSenderMac(theMsgNoFromThisNodeSendingMac);
            theMsgNoFromThisNodeSendingMac++;

            //UInt16 theAvgTemp = avgTemp;
            avgTemperatureArray = getBytes(tempFromSensor); //2 bytes
            contentArray[0] = avgTemperatureArray[0];
            contentArray[1] = avgTemperatureArray[1];
            mp.setMsgPayloadContent(2, contentArray);

            //added missing settings for message
            mp.setNumberBytesInContent(2);
            msgLenEntire = mp.getByteOffset() + mp.getNumberBytesInContent();
            mp.setLengthOfEntireMessage((UInt16)msgLenEntire);

            mp.setMsgPayloadInBytesUsingFields();
            //theNetworking.sendNodeSpecificMsgToBase(mp.getMessagePayloadBuffer(), mp.getByteOffset(), (UInt16)mp.getLengthOfEntireMessage(), myCSMAMac);
            Debug.Print("Check Point 8\n");
            theNetworking.sendingMsg(mp.getMessagePayloadBuffer(), mp.getByteOffset(), (UInt16)mp.getLengthOfEntireMessage());

        } //end SnapshotREquestFulfill



        //NOTE NOTE NOTE
        //This next method uses an API from a previous version of the Network DLL
        //   and the new version does not seem to have a comparable API
        //   that provides neighbor MAC addresses
        //  
        // SO THIS METHOD is commented out

        /**
        //compose rte info message with destination base station
        //  when composed invoke the routing layer to send
        void RteInfoRequestFulfill()
        {
            //the approach here is to bundle into the message payload content the
            //  mac addresses of this nodes neighbors
            //   It is the responsibility of the BaseNode to put together a routing
            //
            // NOTE: The routing approach is not really implemented on the regular node
            //   side because the design supports flooding -- but this could be changed.
            //

            //get the latest list of active neighbor nodes from the MAC
            // NOTE THIS NO LONGER WORKS upon change of NET DLL
            myNeighborTable_currentPull = myCSMAMac.GetNeighborTable();

            // how many neighbors are there in the neighbor table ?
            byte numberNeighbors = myNeighborTable_currentPull.NumberValidNeighbor;
            //there are 83 bytes available in total for the payload of addresses
            //  each address is 2 bytes
            //  
            if (numberNeighbors > MAX_NUM_MACADDR_IN_PAYLOAD_INFO)
            {
                numberNeighbors = MAX_NUM_MACADDR_IN_PAYLOAD_INFO;
            }
            i3 = 0;
            byte[] payloadMACAddr = new byte[2 * numberNeighbors];

            if (numberNeighbors > 0)
            {
                //form the message header info
                mp.setDestinationType(MessageDestinationType.BaseStation);
                mp.setMessageAuthorType(MessageAuthorType.SpecificNodeAuthor);
                mp.setMessageAuthorFunctionType(myMAFT);
                mp.setMessagePayloadType(MessagePayloadType.RteInfoRespToBase);
                mp.setOriginalSenderMac(myNodeID);
                mp.setSenderMac(myNodeID);
                mp.setIsIntendedRecipientMacSpecified(IntendedRecipientMacIsSpecified.No);
                mp.setIntendedRecipientMacAddress((UInt16)0);
                mp.setMsgNumberFromSenderMac(theMsgNoFromThisNodeSendingMac);
                theMsgNoFromThisNodeSendingMac++;

                while (i3 < numberNeighbors)
                {
                    //form the message payload based on the number of MAC Addr to be sent
                    UInt16 anAddr = myNeighborTable_currentPull.Neighbor[i].MacAddress;
                    byte[] anAddrArray = getBytes(anAddr); //2 bytes
                    payloadMACAddr[2 * i3 + 0] = anAddrArray[0];
                    payloadMACAddr[2 * i3 + 1] = anAddrArray[1];
                    i++;
                }
            }
            //missing info now set
            mp.setNumberBytesInContent((UInt16)(2 * numberNeighbors));
            msgLenEntire = mp.getByteOffset() + mp.getNumberBytesInContent();
            mp.setLengthOfEntireMessage((UInt16)msgLenEntire);

            //now have set the message header info and have set up a byte array for the payload
            mp.setMsgPayloadContent(numberNeighbors, payloadMACAddr);
            theNetworking.sendNodeSpecificMsgToBase(mp.getMessagePayloadBuffer(), mp.getByteOffset(), (UInt16)mp.getLengthOfEntireMessage(), myCSMAMac);

        } //end RteInfoRequestFulfill()
        */



        // MAC API CALLBACK HANDLER METHOD  
        //  that is registered upon mac initializaiton
        //  this is the entirety of the message -- including the payload
        //  the MessagePayload class API are used here
        //NEW API void MsgsReceived(byte[] message, ushort size, ushort Source, bool Unicast, byte RSSI, byte linkQuality)

        //void MsgsReceived(byte[] msgIn, ushort msgSize, ushort Source, bool Unicast, byte RSSI, byte linkQuality)
        //void MsgsReceived(byte[] msgIn, ushort msgSize)
        void MsgsReceived(UInt16 NumberOfMessages)
        {

            lmessage = myCSMAMac.GetNextPacket();


            Debug.Print(" In Msgs Received:  should not be here now with no incoming msgs -- nothing set.");

            //construct the message payload in terms of fields from the byte array
            recvMP.setLengthOfEntireMessage(lmessage.Size);
            recvMP.setPayloadFieldsFromMsgByteArray(lmessage.GetMessage(), lmessage.Size);

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
            //  100 messages have  been received (indexing is from 0 to max size)
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


        //utility method for LogisticsApp
        byte[] getBytes(UInt16 value)
        {
            //try alternate code
            //byte[] returnByteArray2 = { 0, 0 };
            returnByteArray2[0] = (byte)(value & 0xFF);
            //Debug.Print(" in getBytes byte [0] is returnByteArray2[0]" + returnByteArray2[0]);
            returnByteArray2[1] = (byte)((value >> 8) & 0xFF);
            //Debug.Print(" in getBytes byte [1] is returnByteArray2[0]" + returnByteArray2[1]);
            // avoid the use of new
            //return new byte[2] { (byte)(value & 0xFF), (byte)((value >> 8) & 0xFF) };
            return returnByteArray2;
        }

    }
}
