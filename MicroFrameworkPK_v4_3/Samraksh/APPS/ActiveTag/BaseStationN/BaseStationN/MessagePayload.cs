using System;
using Microsoft.SPOT;

namespace Samraksh.APPS.ActiveTag
{
    public class MessagePayload
    {
        private MessagePayloadType _msgPType;

        public MessagePayloadType msgPType
        {
            set { this._msgPType = value; }
            get { return this._msgPType; }
        }

        private MessageDestinationType _msgDestinationType;

        public MessageDestinationType msgDestinationType
        {
            set { this._msgDestinationType = value; }
            get { return this._msgDestinationType; }
        }

        private MessageAuthorFunctionType _msgAuthFuncType;

        public MessageAuthorFunctionType msgAuthFuncType
        {
            set { this._msgAuthFuncType = value; }
            get { return this._msgAuthFuncType; }
        }

        private MessageAuthorType _msgAuthType;

        public MessageAuthorType msgAuthType
        {
            set { this._msgAuthType = value; }
            get { return this._msgAuthType; }
        }

        private UInt16 _senderMac;

        public UInt16 senderMac
        {
            set { this._senderMac = value; }
            get { return this._senderMac; }
        }

        private UInt16 _msgNoFromSenderMac;

        public UInt16 msgNoFromSenderMac
        {
            set { this._msgNoFromSenderMac = value; }
            get { return this._msgNoFromSenderMac; }
        }

        private IntendedRecipientMacIsSpecified _intendedRecMacIsSpecified;

        public IntendedRecipientMacIsSpecified intendedRecMacIsSpecified
        {
            set { this._intendedRecMacIsSpecified = value; }
            get { return this._intendedRecMacIsSpecified; }
        }

        UInt16 _intendedRecipientMacAddress = (UInt16)0;  //this is the default value if it is not set

        public UInt16 intendedRecipientMacAddress
        {
            set { this._intendedRecipientMacAddress = value; }
            get { return this._intendedRecipientMacAddress; }
        }

        private UInt16 _origSenderMac;

        public UInt16 origSenderMac
        {
            set { this._origSenderMac = value; }
            get { return this._origSenderMac; }
        }

        // Message content can not exceed 64 bytes 
        public byte[] messageContent = new byte[64];

        public byte[] marshalBuffer = new byte[128];

        private UInt16 _messageContentLength;

        public UInt16 messageContentLength
        {
            set { this._messageContentLength = value; }
            get { return this._messageContentLength; }
        }

        private UInt16 _messageLength;

        public UInt16 messageLength
        {
            set { this._messageLength = value; }
            get { return this._messageLength; }
        }


        public byte[] MarshallIntoBytes()
        {
            UInt16 length = 0;

            UInt16 payloadType = (UInt16)msgPType;

            marshalBuffer[length++] = (byte)payloadType;
            marshalBuffer[length++] = (byte)(payloadType >> 8);
            marshalBuffer[length++] = (byte)msgAuthType;
            marshalBuffer[length++] = (byte)msgAuthFuncType;
            marshalBuffer[length++] = (byte)intendedRecMacIsSpecified;
            marshalBuffer[length++] = (byte)intendedRecipientMacAddress;
            marshalBuffer[length++] = (byte)(intendedRecipientMacAddress >> 8);
            marshalBuffer[length++] = (byte)msgDestinationType;
            marshalBuffer[length++] = (byte)(senderMac);
            marshalBuffer[length++] = (byte)(senderMac >> 8);
            marshalBuffer[length++] = (byte)msgNoFromSenderMac;
            marshalBuffer[length++] = (byte)(msgNoFromSenderMac >> 8);
            marshalBuffer[length++] = (byte)msgDestinationType;
            marshalBuffer[length++] = (byte)(origSenderMac);
            marshalBuffer[length++] = (byte)(origSenderMac >> 8);
            marshalBuffer[length++] = (byte)messageContentLength;
            marshalBuffer[length++] = (byte)(messageContentLength >> 8);

            for (int i = 0; i < messageContentLength; i++)
            {
                marshalBuffer[length++] = messageContent[i];
            }

            messageLength = length;

            return marshalBuffer;

        }

        public MessagePayload()
        {
        }

        public MessagePayload(byte[] data)
        {
            if (data == null)
                return;

            UInt16 length = 0;

            UInt16 payloadType;

            payloadType = data[length++];
            payloadType |= (UInt16)(data[length++] << 8);
            msgPType = (MessagePayloadType)payloadType;

            msgAuthType = (MessageAuthorType)data[length++];

            msgAuthFuncType = (MessageAuthorFunctionType)data[length++];

            intendedRecMacIsSpecified = (IntendedRecipientMacIsSpecified)data[length++];

            intendedRecipientMacAddress = (UInt16)data[length++];
            intendedRecipientMacAddress |= (UInt16)(data[length++] << 8);

            msgDestinationType = (MessageDestinationType)data[length++];

            senderMac = (UInt16)data[length++];
            senderMac |= (UInt16)(data[length++] << 8);

            msgNoFromSenderMac = (UInt16)data[length++];
            msgNoFromSenderMac |= (UInt16)(data[length++] << 8);

            msgDestinationType = (MessageDestinationType)data[length++];

            origSenderMac = (UInt16)data[length++];
            origSenderMac |= (UInt16)(data[length++] << 8);

            messageContentLength = (UInt16)data[length++];
            messageContentLength |= (UInt16)(data[length++] << 8);

            for (int i = 0; i < messageContentLength; i++)
            {
                messageContent[i] = data[length++];
            }

            messageLength = length;

        }
    }
}
