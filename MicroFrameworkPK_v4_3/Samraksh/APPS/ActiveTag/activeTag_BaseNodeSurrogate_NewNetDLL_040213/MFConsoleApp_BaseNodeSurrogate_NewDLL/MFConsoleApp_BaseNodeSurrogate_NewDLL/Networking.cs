using System;
using Microsoft.SPOT;
//using System.Collections;
using System.Threading;
using Samraksh.SPOT.Net;
using Samraksh.SPOT.Net.Radio;
using Samraksh.SPOT.Net.Mac;

namespace MFConsoleApp_BaseNodeSurrogate_NewDLL_040213
{

    enum RoutingApproach
    {
        Flood
    } ;


    class Networking
    {
        //max size of WSN is 10 nodes
        UInt16[] macAddressTable = new UInt16[10];

        //stores the neighbor table for each neighbor of this node
        //Hashtable neighborHashtable = new Hashtable(10);

        //UInt16 baseStationMACAddress;  //never used

        //not used
        // Hashtable foodNodesHashtable = new Hashtable(10);
        // Hashtable medicalSupplyHastable = new Hashtable(10);
        // Hashtable ammoSupplyHashtable = new Hashtable(10);

        //variables declared global 
        NetOpStatus theOpStatus;
        byte[] byteArrayMP = new byte[200]; //set to size of max size message to be sent out
        int sendTryOK;
        int numAppLevelTriesAllowed = 2;
        int numAppLevelTriesSoFar = 0;

        /** only for BASE STATION to develop a routing algorithm
        public void initializeRouting(MessagePayload  _theMP, RoutingApproach _theApproach)
        {
            //do a broadcast message to all
            //use the Mac broadcast address 

            //this is a flood approach

        }
         */


        // the actual message sending
        public void sendingMsg(MessagePayload mp, CSMA myCSMAMac)
        {
            mp.setMsgPayloadInBytesUsingFields();
            byteArrayMP = mp.getMessagePayloadBuffer();

            sendTryOK = 0;
            while (sendTryOK == 0 && numAppLevelTriesSoFar < numAppLevelTriesAllowed)
            {
                //sending message payload to base using a Broadcast
                //NetOpStatus theOpStatus = Samraksh.SPOT.Net.Mac.CSMA.Send(Samraksh.SPOT.Net.Mac.Addresses.BROADCAST, byteArrayMP, offset, mpSize);
                theOpStatus = myCSMAMac.Send((ushort)Samraksh.SPOT.Net.Mac.Addresses.BROADCAST, byteArrayMP, (UInt16)mp.getByteOffset(), (UInt16)mp.getLengthOfEntireMessage());
                if (theOpStatus == NetOpStatus.S_Success)
                {
                    sendTryOK = 1;
                }
                numAppLevelTriesSoFar++;
            }
            Debug.Print("in Networking.cs sendingMsg sendTryOK = " + sendTryOK);
            Debug.Print("in Networking.cs theOpStatus = " + theOpStatus);
        }  //end sendingMsg


        //alternate structure for sendingMsg
        public void sendingMsg(byte[] byteArrayMP, UInt16 offset, UInt16 mpSize, CSMA myCSMA)
        {
            sendTryOK = 0;
            while (sendTryOK == 0 && numAppLevelTriesSoFar < numAppLevelTriesAllowed)
            {
                //sending message payload to base using a Broadcast
                //NetOpStatus theOpStatus = Samraksh.SPOT.Net.Mac.CSMA.Send(Samraksh.SPOT.Net.Mac.Addresses.BROADCAST, byteArrayMP, offset, mpSize);
                theOpStatus = myCSMA.Send((ushort)Samraksh.SPOT.Net.Mac.Addresses.BROADCAST, byteArrayMP, (UInt16)offset, (UInt16)mpSize);
                if (theOpStatus == NetOpStatus.S_Success)
                {
                    sendTryOK = 1;
                }
                numAppLevelTriesSoFar++;
            }
            Debug.Print("in Networking.cs sendingMsg alternate sendTryOK = " + sendTryOK);
            Debug.Print("in Networking.cs theOpStatus = " + theOpStatus);
        }  //end sendingMsg alternate signature used




        public void sendNodeSpecificMsgToBase(byte[] byteArrayMP, UInt16 offset, UInt16 mpSize, CSMA myCSMA)
        {
            sendTryOK = 0;

            while (sendTryOK == 0 && numAppLevelTriesSoFar < numAppLevelTriesAllowed)
            {
                //sending message payload to base using a Broadcast
                //NetOpStatus theOpStatus = Samraksh.SPOT.Net.Mac.CSMA.Send(Samraksh.SPOT.Net.Mac.Addresses.BROADCAST, byteArrayMP, offset, mpSize);
                theOpStatus = myCSMA.Send((ushort)Samraksh.SPOT.Net.Mac.Addresses.BROADCAST, byteArrayMP, (UInt16)offset, (UInt16)mpSize);
                if (theOpStatus == NetOpStatus.S_Success)
                {
                    sendTryOK = 1;
                }
                numAppLevelTriesSoFar++;
            }
            Debug.Print("in Networking.cs sendTryOK = " + sendTryOK);
            Debug.Print("in Networking.cs theOpStatus = " + theOpStatus);
        } //send NodeSpecificMsgToBase



        /**
        //after code refactoring, this method now calls sendingMsg method
        //  this is a simplified strcture of Networking.cs
        public void forwardMessageToBase(MessagePayload theMP, CSMA myCSMA)
        {
            sendingMsg(theMP, myCSMA);
        }



        //after code refactoring, this method now calls sendingMsg method
        //  this is a simplified strcture of Networking.cs
        public void sendMsgToNonBaseSpecificDestination(MessagePayload theMP, CSMA myCSMA)
        {
            sendingMsg(theMP, myCSMA);
        }


        //after code refactoring, this method now calls sendingMsg method
        //  this is a simplified strcture of Networking.cs
        public void forwardMessageInFlood(MessagePayload mp, CSMA myCSMAMac)
        {
            sendingMsg(mp, myCSMAMac);
        }

        //after code refactoring, this method now calls sendingMsg method
        //  this is a simplified strcture of Networking.cs
        public void sendQueryAliveMessageToNeighbor(MessagePayload mp, CSMA myCSMAMac)
        {
            sendingMsg(mp, myCSMAMac);
        }

        //after code refactoring, this method now calls sendingMsg method
        //  this is a simplified strcture of Networking.cs
        public void sendIAmAliveMessageToNeighbor(MessagePayload mp, CSMA myCSMAMac)
        {
            sendingMsg(mp, myCSMAMac);
        }

        */


    } //end class Networking
}

