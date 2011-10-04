using System;
using System.Threading;
using Dpws.Client.Discovery;
using Dpws.Client.Eventing;
using Dpws.Client;
using Ws.Services;
using Ws.Services.Utilities;
using Ws.Services.WsaAddressing;
using schemas.example.org.EventingService;
using schemas.example.org.SimpleService;
using schemas.example.org.AttachmentService;

#if !Windows && !WindowsCE
using System.Ext;
#endif

namespace Interop.SimpleService
{
    public class MFSimpleServiceClient
    {
        /// <summary>
        /// This is a sample Micr Framework Dpws client application. This console application is designed
        /// to excercise the features of the MFWsdStack client classes. This application creates an instance of
        /// a SimpleServiceClient class. This class is derived DpwsClientServices. When an instance of this class
        /// is created the MFWsdStack services start automatically. Unlike the Device services that require an actual
        /// Start command. This sample app will wait for a hello discovery request. Wen received it will probe for
        /// services that implement a SimpleService type. Te Dpws Interop sample service implements this type. For each
        /// probe match received (up to 10) a resolve request is sent to endpoints obtained from the resolve matches.
        /// For each resolve match the app sends a Get request to the device hosting the service. The app then uses the
        /// metadata in the get response to call 1way, 2way methods on the service and then subscribes to the SimpleEvent
        /// and ItegerEvent hosted by the interop service. When the client receives each event from the service it sends
        /// an Unsubscribe for each event and waits for another hello. The application processes one hello at a time.
        /// resolve match 
        /// </summary>
        public static void Main()
        {
            // Turn console messages on
            Console.Verbose = true;

            System.Ext.Console.Write("Application started...");

            // Create a test application thread
            TestApplication testApp = new TestApplication();
            Thread testAppThread = new Thread(new ThreadStart(testApp.Run));
            testAppThread.Start();
        }
    }

    /// <summary>
    /// SimpleService test application class.
    /// </summary>
    public class TestApplication : IDisposable
    {
        private Random m_random;
        private object m_threadLock = new object();
        private DpwsMetadata m_selectedService = null;
        private bool m_inOperation = false;
        private bool m_inDiscovery = false;
        private DpwsEventSubscription m_simpleEventSubscription = null;
        private DpwsEventSubscription m_integerEventSubscription = null;

        SimpleServiceClientProxy m_simpleServiceClient = new SimpleServiceClientProxy(15337);
        EventingServiceClientProxy m_eventingServiceClient = new EventingServiceClientProxy(15338, new EventingClientImplementation());
        AttachmentServiceClientProxy m_attachmentServiceClient = new AttachmentServiceClientProxy(15339);
        DiscoClient m_discoClient = null;

        bool m_deviceSelected = false; // Tells the app a device is selected

        public TestApplication()
        {
            // Turn listening to this IP on
            m_simpleServiceClient.IgnoreRequestFromThisIP = false;
            m_eventingServiceClient.IgnoreRequestFromThisIP = false;
            m_attachmentServiceClient.IgnoreRequestFromThisIP = false;

            m_simpleServiceClient.EndpointAddress = "urn:uuid:3cb0d1ba-cc3a-46ce-b416-212ac2419b06";
            m_eventingServiceClient.EndpointAddress = "urn:uuid:3cb0d1ba-cc3a-46ce-b416-212ac2419b07";
            m_attachmentServiceClient.EndpointAddress = "urn:uuid:3cb0d1ba-cc3a-46ce-b416-212ac2419b08";

            m_discoClient = new DiscoClient(m_simpleServiceClient);

            m_random = new Random();
        }

        protected virtual void Dispose(bool disposing)
        {
            if (disposing)
            {
                // dispose managed resources
                if (m_simpleServiceClient != null)
                {
                    m_simpleServiceClient.Dispose();
                    m_simpleServiceClient = null;
                }

                if (m_eventingServiceClient != null)
                {
                    m_eventingServiceClient.Dispose();
                    m_eventingServiceClient = null;
                }

                if (m_attachmentServiceClient != null)
                {
                    m_attachmentServiceClient.Dispose();
                    m_attachmentServiceClient = null;
                }
            }
            // free native resources
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        /// <summary>
        /// Method runs a loop waiting for Hello events. When on is received method probes, resolves
        /// and Get's device service information and calls 1way, 2way methods and subscribes to SimpleEvent
        /// and IntegerEvent and waits for them to fire. When they do Unsubscribe is called on the events
        /// and the receive hello's flag is reset.
        /// </summary>
        public void Run()
        {
            // Create Event handlers
            m_simpleServiceClient.ByeEvent += new ByeEventHandler(m_simpleControl_ByeEvent);
            m_simpleServiceClient.HelloEvent += new HelloEventHandler(m_simpleControl_HelloEvent);
            m_simpleServiceClient.SubscriptionEndEvent += new SubscriptionEndEventHandler(m_simpleControl_SubscriptionEndEvent);
            bool firstPass = true;

            bool twoWayAttach = false;

            // Continuous run loop
            while (true)
            {
                // If hello was received and a SimpleService device was found. SeletedService will not be null.
                // Process until Bye is received.
                if (m_deviceSelected == true)
                {
                    // If this is the first time through the loop for this device subscribe to events
                    if (firstPass == true)
                    {
                        // Test service host call
                        System.Ext.Console.Write("Testing Host service...");
                        DpwsSubscribeRequest subReq;
                        subReq = new DpwsSubscribeRequest(m_eventingServiceClient.EventSources["SimpleEvent"], m_eventingServiceClient.ServiceEndpoint, m_eventingServiceClient.TransportAddress, "PT1H", null);
                        m_simpleEventSubscription = m_eventingServiceClient.EventingClient.Subscribe(subReq);
                        subReq = new DpwsSubscribeRequest(m_eventingServiceClient.EventSources["IntegerEvent"], m_eventingServiceClient.ServiceEndpoint, m_eventingServiceClient.TransportAddress, "PT1H", null);
                        m_integerEventSubscription = m_eventingServiceClient.EventingClient.Subscribe(subReq);

                        firstPass = false;
                    }

                    // Make 1Way and 2Way service calls
                    if (m_deviceSelected == true)
                    {
                        System.Ext.Console.Write("");
                        System.Ext.Console.Write(">>>>>>>>>>>>> Sending 1way(10) request to: " + m_selectedService.ThisDevice.FriendlyName);
                        try
                        {
                            m_simpleServiceClient.OneWay(new OneWay());
                        }
                        catch (Exception e)
                        {
                            System.Ext.Console.Write("");
                            System.Ext.Console.Write("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! OneWay method failed. " + e.Message);
                        }
                    }

                    if (m_deviceSelected == true)
                    {
                        int x = System.Math.Abs(m_random.Next()) % 100;
                        int y = System.Math.Abs(m_random.Next()) % 100;

                        System.Ext.Console.Write("");
                        System.Ext.Console.Write(">>>>>>>>>>>>> Sending 2way(" + x.ToString() + ", " + y.ToString() + ") request to: " + m_selectedService.ThisDevice.FriendlyName);
                        try
                        {
                            TwoWayRequest req = new TwoWayRequest();
                            req.X = x;
                            req.Y = y;

                            TwoWayResponse resp = m_simpleServiceClient.TwoWayRequest(req);
                            if (resp.Sum == 0)
                            {
                                System.Ext.Console.Write("");
                                System.Ext.Console.Write("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 2way method did not receive a valid response.");
                            }
                            else
                            {
                                System.Ext.Console.Write("");
                                System.Ext.Console.Write("<<<<<<<<<<<<< 2way response returned " + resp.Sum);
                            }
                        }
                        catch (Exception e)
                        {
                            System.Ext.Console.Write("");
                            System.Ext.Console.Write("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! TwoWay method failed. " + e.Message);
                        }
                    }

                    // Make 1wayattach or a 2wayattach service calls
                    if (m_deviceSelected == true)
                    {
                        if (twoWayAttach)
                        {
                            // create an instance of the help icon test object
                            HelpIcon helpIcon = new HelpIcon();

                            System.Ext.Console.Write("");
                            System.Ext.Console.Write(">>>>>>>>>>>>> Sending 1wayattach request to: " + m_selectedService.ThisDevice.FriendlyName);
                            try
                            {
                                OneWayAttachment req = new OneWayAttachment();
                                req.Param = helpIcon.Data.ToArray();
                                m_attachmentServiceClient.OneWayAttachment(req);
                            }
                            catch (Exception e)
                            {
                                System.Ext.Console.Write("");
                                System.Ext.Console.Write("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 1wayattach method failed: " + e.Message);
                            }
                        }
                        else
                        {
                            // create an instance of the help icon test object
                            HelpIcon helpIcon = new HelpIcon();

                            System.Ext.Console.Write("");
                            System.Ext.Console.Write(">>>>>>>>>>>>> Sending 2wayattach request to: " + m_selectedService.ThisDevice.FriendlyName);
                            try
                            {
                                TwoWayAttachmentRequest req = new TwoWayAttachmentRequest();
                                req.Param = helpIcon.Data.ToArray();
                                TwoWayAttachmentResponse resp = m_attachmentServiceClient.TwoWayAttachmentRequest(req);

                                System.Ext.Console.Write("");
                                System.Ext.Console.Write("<<<<<<<<<<<<< Sending 2wayattach request succeeded");
                            }
                            catch (Exception e)
                            {
                                System.Ext.Console.Write("");
                                System.Ext.Console.Write("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! TwoWay method failed. " + e.Message);
                            }
                        }

                        twoWayAttach = !twoWayAttach;
                    }
                }
                else
                    firstPass = true;

                Thread.Sleep(5000);
            }
        }

        void m_simpleControl_SubscriptionEndEvent(object obj, SubscriptionEndEventArgs SubscriptionEndEventArgs)
        {
            System.Ext.Console.Write("");
            System.Ext.Console.Write("Subsription End event received:");
            System.Ext.Console.Write("  SubscriptionID = " + SubscriptionEndEventArgs.SubscriptionID);
        }

        void m_simpleControl_HelloEvent(object obj, DpwsServiceDescription helloEventArgs)
        {
            // If SelectedDevice is set don't do discovery again until Bye event is received
            if (m_deviceSelected == true || m_inDiscovery)
                return;
            m_inDiscovery = true;

            // Print Hello information
            try
            {
                System.Ext.Console.Write("");
                System.Ext.Console.Write("SimpleServiceClient received a hello request.");
                System.Ext.Console.Write("Endpoint address: " + helloEventArgs.Endpoint.Address.ToString());
                string types = "";
                bool simpleDeviceTypeFound = false;
                for (int i = 0; i < helloEventArgs.ServiceTypes.Count; ++i)
                {
                    if (helloEventArgs.ServiceTypes[i].TypeName == "SimpleDeviceType")
                        simpleDeviceTypeFound = true;
                    types += "NamespaceUri: " + helloEventArgs.ServiceTypes[i].NamespaceUri + " " + "TypeName: " + helloEventArgs.ServiceTypes[i].TypeName + "\n";
                }
                System.Ext.Console.Write("Types: " + types);
                System.Ext.Console.Write("Xaddrs: " + ((helloEventArgs.XAddrs == null) ? "none" : helloEventArgs.XAddrs[0]));
                System.Ext.Console.Write("Metadata version: " + helloEventArgs.MetadataVersion);
                System.Ext.Console.Write("");

                // If this is not a SimplDeviceType don't discover
                if (simpleDeviceTypeFound == false)
                {
                    System.Ext.Console.Write("");
                    System.Ext.Console.Write("Discovery will only resolve SimpleDeviceType services.");
                }

                // Get SimpleService metadata
                DpwsMetadata deviceMetadata = m_discoClient.GetSimpleService(helloEventArgs.Endpoint.Address.AbsoluteUri);
                if (deviceMetadata != null)
                {
                    SelectedService = deviceMetadata;
                    m_deviceSelected = true;
                }
            }
            finally
            {
                m_inDiscovery = false;
            }
        }

        void m_simpleControl_ByeEvent(object obj, DpwsServiceDescription byeEventArgs)
        {
            System.Ext.Console.Write("");
            System.Ext.Console.Write("SimpleServiceClient received a bye request.");
            System.Ext.Console.Write("Endpoint address: " + byeEventArgs.Endpoint.Address.ToString());
            System.Ext.Console.Write("Xaddrs: " + byeEventArgs.XAddrs[0]);
            System.Ext.Console.Write("");

            if (m_deviceSelected && m_simpleServiceClient.EndpointAddress == byeEventArgs.Endpoint.Address.ToString())
            {
                m_deviceSelected = false;
                m_selectedService = null;
            }
        }

        /// <summary>
        /// Property containing a DpwsMetadata object that defines the service endpoints.
        /// </summary>
        /// <remarks>
        /// Set this property prior to calling the methods. Obtain this information from a discovery
        /// Get reequest.
        /// </remarks>
        public DpwsMetadata SelectedService
        {
            get
            {
                lock (m_threadLock)
                {
                    return m_selectedService;
                }
            }
            set
            {
                lock (m_threadLock)
                {
                    // If an operatoin is pending wait until it finishes
                    while (m_inOperation == true)
                        Thread.Sleep(100);

                    // If existing event subscriptions are pending unsubscribe
                    if (m_integerEventSubscription != null)
                    {
                        // Unsubscribe to simple event
                        if (m_simpleEventSubscription != null)
                        {
                            if (!m_eventingServiceClient.EventingClient.Unsubscribe(new Uri(m_eventingServiceClient.EndpointAddress), m_simpleEventSubscription))
                                System.Ext.Console.Write("Unsubscribe to SimpleEvent at service endpoint " + m_eventingServiceClient.EndpointAddress + " failed!");
                        }

                        // Unsubscribe to integer event
                        if (m_integerEventSubscription != null)
                        {
                            if (!m_eventingServiceClient.EventingClient.Unsubscribe(new Uri(m_eventingServiceClient.EndpointAddress), m_integerEventSubscription))
                            {
                                System.Ext.Console.Write("");
                                System.Ext.Console.Write("Unsubscribe to IntegerEvent at service endpoint " + m_eventingServiceClient.EndpointAddress + " failed!");
                            }
                        }
                    }

                    // If value is null the controls service endpoint will be cleared
                    // otherwise set the SimpleService and EventingService endpoints
                    if (value != null)
                    {
                        // Locate SimpleService and EventingService in metadata and set their endpoint addresses
                        for (int i = 0; i < value.Relationship.HostedServices.Count; ++i)
                        {
                            DpwsMexService hostedService = value.Relationship.HostedServices[i];
                            if (hostedService.ServiceTypes["EventingService"] != null)
                                m_eventingServiceClient.ServiceEndpoint = hostedService.EndpointRefs[0].Address.AbsoluteUri;

                            if (hostedService.ServiceTypes["SimpleService"] != null)
                                m_simpleServiceClient.ServiceEndpoint = hostedService.EndpointRefs[0].Address.AbsoluteUri;

                            if (hostedService.ServiceTypes["AttachmentService"] != null)
                                m_attachmentServiceClient.ServiceEndpoint = hostedService.EndpointRefs[0].Address.AbsoluteUri;
                        }

                        // Verify that both addresses were found.
                        if (m_eventingServiceClient.EndpointAddress == null)
                        {
                            System.Ext.Console.Write("");
                            System.Ext.Console.Write("EventinService was not found! Events are disabled.");
                        }

                        if (m_simpleServiceClient.EndpointAddress == null)
                        {
                            System.Ext.Console.Write("");
                            System.Ext.Console.Write("SimpleService was not found! Method calls are disabled.");
                        }
                    }

                    // Set the controle endpoint to the new endpoint
                    m_selectedService = value;
                }
            }
        }
    }
}
