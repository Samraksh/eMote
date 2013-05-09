using System;
using Microsoft.SPOT;
using Samraksh.SPOT.Net.Mac;
using Samraksh.SPOT.Net;

namespace Samraksh.APPS.ActiveTag
{
    enum RoutingApproach
    {
        Flood
    } ;

    public class Networking
    {
        MACBase macObject;

        int sendTryOK;

        int numAppLevelTriesAllowed = 2;

        int numAppLevelTriesSoFar = 0;

        private Networking()
        {
        }

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
                sendStatus = (DeviceStatus)macObject.Send(macaddress, data, offset, length);

                if (sendStatus == (DeviceStatus)1)
                {
                    Debug.Print("Networking : Send done was successful\n");
                    sendTryOK = 1;
                }

                numAppLevelTriesSoFar++;
            }

        }


        public void sendingMsg(byte[] byteArrayMP, UInt16 offset, UInt16 mpSize)
        {

            Send((ushort)Samraksh.SPOT.Net.Mac.Addresses.BROADCAST, byteArrayMP, offset, mpSize);

        }

        public void sendingMsg(UInt16 address, byte[] byteArrayMP, UInt16 offset, UInt16 mpSize)
        {

            Send(address, byteArrayMP, offset, mpSize);

        }

    }
}
