using System;
using Microsoft.SPOT;
using Samraksh.SPOT.Net;
using Samraksh.SPOT.Net.Mac;

namespace Logistics
{
    enum RoutingApproach
    {
        Flood
    } ;

    class Networking
    {
        //max size of WSN is 10 nodes
        UInt16[] macAddressTable = new UInt16[10];

        //variables declared global 
        NetOpStatus theOpStatus;
        byte[] byteArrayMP = new byte[200]; //set to size of max size message to be sent out
        int sendTryOK;
        int numAppLevelTriesAllowed = 2;
        int numAppLevelTriesSoFar = 0;
        MACBase macObject;

        public Networking(MACBase mac)
        {
            macObject = mac;
        }

        private void Send(ushort macaddress, byte[] data, UInt16 offset, UInt16 length)
        {
            // Casting back to DeviceStatus because the netopstatus is different from devicestatus
            // and device status is being returned by the net dll 
            DeviceStatus sendStatus;

            numAppLevelTriesSoFar = 0;
            sendTryOK = 0;

            while (sendTryOK == 0 && numAppLevelTriesSoFar < numAppLevelTriesAllowed)
            {
                sendStatus = (DeviceStatus) macObject.Send(macaddress, data, offset, length);

                if (sendStatus == (DeviceStatus) 1)
                {
                    Debug.Print("Networking : Send done was successful\n");
                    sendTryOK = 1;
                }

                numAppLevelTriesSoFar++;
            }

        }

        // the actual message sending
        public void sendingMsg(MessagePayload mp)
        {
            mp.setMsgPayloadInBytesUsingFields();
            byteArrayMP = mp.getMessagePayloadBuffer();

            Send((ushort)Samraksh.SPOT.Net.Mac.Addresses.BROADCAST, byteArrayMP, (UInt16)mp.getByteOffset(), (UInt16)mp.getLengthOfEntireMessage());

        }  //end sendingMsg

        //alternate structure for sendingMsg
        public void sendingMsg(byte[] byteArrayMP, UInt16 offset, UInt16 mpSize)
        {

            Send((ushort)Samraksh.SPOT.Net.Mac.Addresses.BROADCAST, byteArrayMP, offset, mpSize);

        }  //end sendingMsg alternate signature used


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
