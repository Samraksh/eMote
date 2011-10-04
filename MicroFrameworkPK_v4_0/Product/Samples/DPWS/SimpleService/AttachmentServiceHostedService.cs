//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     .NET Micro Framework MFSvcUtil.Exe
//     Runtime Version:2.0.00001.0001
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------


using System;
using System.Text;
using System.Xml;
using Dpws.Device;
using Dpws.Device.Services;
using Ws.Services;
using Ws.Services.Soap;
using Ws.Services.WsaAddressing;
using Ws.Services.Xml;

namespace schemas.example.org.AttachmentService
{
    
    
    public class AttachmentService : DpwsHostedService
    {
        
        private IAttachmentService m_service = null;
        
        public AttachmentService(IAttachmentService service)
        {
            // Set the service implementation properties
            m_service = service;

            // Set base service properties
            ServiceNamespace = new WsXmlNamespace("att", "http://schemas.example.org/AttachmentService");
            ServiceID = "urn:uuid:2d89f2b4-e0fd-4401-a351-2a51422a2641";
            ServiceTypeName = "AttachmentService";

            // Add service types here
            ServiceOperations.Add(new WsServiceOperation("http://schemas.example.org/AttachmentService", "OneWayAttachment"));
            ServiceOperations.Add(new WsServiceOperation("http://schemas.example.org/AttachmentService", "TwoWayAttachmentRequest"));

            // Add event sources here
        }
        
        public virtual Byte[] OneWayAttachment(WsWsaHeader header, XmlReader reader)
        {
            // Build request object
            OneWayAttachmentDataContractSerializer reqDcs;
            reqDcs = new OneWayAttachmentDataContractSerializer("OneWayAttachment", "http://schemas.example.org/AttachmentService");
            reqDcs.BodyParts = this.BodyParts;
            OneWayAttachment req;
            req = ((OneWayAttachment)(reqDcs.ReadObject(reader)));

            // Call service operation to process request.
            m_service.OneWayAttachment(req);

            // Return null response for oneway messages
            return null;
        }
        
        public virtual Byte[] TwoWayAttachmentRequest(WsWsaHeader header, XmlReader reader)
        {
            // Build request object
            TwoWayAttachmentRequestDataContractSerializer reqDcs;
            reqDcs = new TwoWayAttachmentRequestDataContractSerializer("TwoWayAttachmentRequest", "http://schemas.example.org/AttachmentService");
            reqDcs.BodyParts = this.BodyParts;
            TwoWayAttachmentRequest req;
            req = ((TwoWayAttachmentRequest)(reqDcs.ReadObject(reader)));

            // Create response object
            // Call service operation to process request and return response.
            TwoWayAttachmentResponse resp;
            resp = m_service.TwoWayAttachment(req);

            // Create response header
            WsWsaHeader respHeader = new WsWsaHeader("http://schemas.example.org/AttachmentService/TwoWayAttachmentResponse", header.MessageID, WsWellKnownUri.WsaAnonymousUri, null, null, null);

            // Create response serializer
            TwoWayAttachmentResponseDataContractSerializer respDcs;
            respDcs = new TwoWayAttachmentResponseDataContractSerializer("TwoWayAttachmentResponse", "http://schemas.example.org/AttachmentService");

            // Build response message and store as first Mtom Bodypart
            MessageType = WsMessageType.Mtom;
            BodyParts.Clear();
            BodyParts.Start = "<soap@soap>";
            BodyParts.Add(respDcs.CreateNewBodyPart(SoapMessageBuilder.BuildSoapMessage(respHeader, respDcs, resp), "<soap@soap>"));
            BodyParts.Add(respDcs.BodyParts[0]);
            return null;
        }
    }
}
