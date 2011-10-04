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
using System.Xml;
using System.Ext;
using System.Ext.Xml;
using Ws.ServiceModel;
using Ws.Services.Mtom;
using Ws.Services.Serialization;
using XmlElement = Ws.Services.Xml.WsXmlNode;
using XmlAttribute = Ws.Services.Xml.WsXmlAttribute;
using XmlConvert = Ws.Services.Serialization.WsXmlConvert;

namespace schemas.example.org.EventingService
{
    
    
    [DataContract(Namespace="http://schemas.example.org/EventingService")]
    public class SimpleEvent
    {
    }
    
    public class SimpleEventDataContractSerializer : DataContractSerializer
    {
        
        public SimpleEventDataContractSerializer(string _rootName, string _rootNameSpace) : 
                base(_rootName, _rootNameSpace)
        {
        }
        
        public override object ReadObject(XmlReader reader)
        {
            SimpleEvent SimpleEventField = null;
            if (IsParentStartElement(reader, false, true))
            {
                SimpleEventField = new SimpleEvent();
                reader.Read();
                reader.ReadEndElement();
            }
            return SimpleEventField;
        }
        
        public override void WriteObject(XmlWriter writer, object graph)
        {
            SimpleEvent SimpleEventField = ((SimpleEvent)(graph));
            if (WriteParentElement(writer, true, true, graph))
            {
                writer.WriteEndElement();
            }
            return;
        }
    }
    
    [DataContract(Namespace="http://schemas.example.org/EventingService")]
    public class IntegerEvent
    {
        
        [DataMember(Order=0)]
        public int Param;
        
        [DataMember(IsNillable=true, IsRequired=false)]
        public XmlElement[] Any;
        
        [DataMember(IsNillable=true, IsRequired=false)]
        public XmlAttribute[] AnyAttr;
    }
    
    public class IntegerEventDataContractSerializer : DataContractSerializer
    {
        
        public IntegerEventDataContractSerializer(string _rootName, string _rootNameSpace) : 
                base(_rootName, _rootNameSpace)
        {
        }
        
        public override object ReadObject(XmlReader reader)
        {
            IntegerEvent IntegerEventField = null;
            if (IsParentStartElement(reader, false, true))
            {
                IntegerEventField = new IntegerEvent();
                IntegerEventField.AnyAttr = ReadAnyAttribute(reader);
                reader.Read();
                if (IsChildStartElement(reader, "Param", false, true))
                {
                    reader.Read();
                    IntegerEventField.Param = XmlConvert.ToInt32(reader.ReadString());
                    reader.ReadEndElement();
                }
                IntegerEventField.Any = ReadAnyElement(reader, false);
                reader.ReadEndElement();
            }
            return IntegerEventField;
        }
        
        public override void WriteObject(XmlWriter writer, object graph)
        {
            IntegerEvent IntegerEventField = ((IntegerEvent)(graph));
            if (WriteParentElement(writer, true, true, graph))
            {
                WriteAnyAttribute(writer, IntegerEventField.AnyAttr);
                if (WriteChildElement(writer, "Param", false, true, IntegerEventField.Param))
                {
                    writer.WriteString(XmlConvert.ToString(IntegerEventField.Param));
                    writer.WriteEndElement();
                }
                WriteAnyElement(writer, IntegerEventField.Any, false);
                writer.WriteEndElement();
            }
            return;
        }
    }
    
    [ServiceContract(Namespace="http://schemas.example.org/EventingService", CallbackContract=typeof(IEventingServiceCallback))]
    [PolicyAssertion(Namespace="http://schemas.xmlsoap.org/ws/2005/05/devprof", Name="Profile", PolicyID="Eventing")]
    [PolicyAssertion(Namespace="http://schemas.xmlsoap.org/ws/2005/05/devprof", Name="PushDelivery", PolicyID="Eventing")]
    [PolicyAssertion(Namespace="http://schemas.xmlsoap.org/ws/2005/05/devprof", Name="DurationExpiration", PolicyID="Eventing")]
    [PolicyAssertion(Namespace="http://schemas.xmlsoap.org/ws/2005/05/devprof", Name="ActionFilter", PolicyID="Eventing")]
    public interface IEventingService
    {
    }
    
    public interface IEventingServiceCallback
    {
        
        [OperationContract(Action="http://schemas.example.org/EventingService/SimpleEvent")]
        void SimpleEvent(SimpleEvent resp);
        
        [OperationContract(Action="http://schemas.example.org/EventingService/IntegerEvent")]
        void IntegerEvent(IntegerEvent resp);
    }
}
