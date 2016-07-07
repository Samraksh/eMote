using System;
using Microsoft.SPOT;

namespace Samraksh.eMote.ProgramManager
{
    //using UpdateID = UInt32;

    public enum UpdateMessage
    {
        NOMSG = 0,
        START = 1, START_ACK = 2,
        AUTHCMD = 4, AUTHCMD_ACK = 8,
        AUTHENTICATE = 16, AUTHETICATE_ACK = 32,
        GETMISSINGPACKETS = 64, GETMISSINGPACKETS_ACK = 128,
        ADDPACKET = 256, ADDPACKET_ACK = 512,
        INSTALL = 1024, INSTALL_ACK = 2048,
    };

    public class UpdateState
    {

        UInt32 updateID;

        UInt16 destAddr;

        UpdateMessage lastSent;

        UpdateMessage lastReceived;

        UInt32 packetSize;

        UInt32 missingPacketCount;

        DateTime latestActionDate;

        public UpdateState() {
            updateID = 0;
            destAddr = 0;
            lastSent = UpdateMessage.NOMSG;
            lastReceived = UpdateMessage.NOMSG;
            packetSize = 50;
            missingPacketCount = 0;
            latestActionDate = System.DateTime.MinValue;
        }

        public UpdateState(UInt32 updateID, UInt16 destAddr, UpdateMessage lastSent, UpdateMessage lastReceived, UInt32 packetSize, UInt32 missingPacketCount, DateTime latestActionDate)
        {
            this.updateID = updateID;
            this.destAddr = destAddr;
            this.lastSent = lastSent;
            this.lastReceived = lastReceived;
            this.packetSize = packetSize;
            this.missingPacketCount = missingPacketCount;
            this.latestActionDate = latestActionDate;
        }
    }
}
