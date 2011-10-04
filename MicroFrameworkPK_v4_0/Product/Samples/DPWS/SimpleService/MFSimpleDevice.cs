using System;
using System.Threading;
using Dpws.Device;
using Ws.Services;
using Ws.Services.Utilities;
using Ws.Services.WsaAddressing;
using Ws.Services.Xml;
using schemas.example.org.EventingService;
using schemas.example.org.SimpleService;
using schemas.example.org.AttachmentService;
using Interop.SimpleService;

#if !WIndows && !WindowsCE
using System.Ext;
#endif

namespace Dpws.Device
{

    class Program
    {

        static void Main(string[] args)
        {
            Program p = new Program();
            p.Start();

            while (true)
            {
                Thread.Sleep(15000);
                Microsoft.SPOT.Debug.GC(true);
            }
        }

        //SimpleServiceClient client;

        public void Start()
        {
            // Set device information
            Device.EndpointAddress = "urn:uuid:3cb0d1ba-cc3a-46ce-b416-212ac2419b51";
            Device.ThisModel.Manufacturer = "Microsoft Corporation";
            Device.ThisModel.ManufacturerUrl = "http://www.microsoft.com/";
            Device.ThisModel.ModelName = "SimpleService Test Device";
            Device.ThisModel.ModelNumber = "1.0";
            Device.ThisModel.ModelUrl = "http://www.microsoft.com/";
            Device.ThisModel.PresentationUrl = "http://www.microsoft.com/";

            Device.ThisDevice.FriendlyName = "SimpleService";
            Device.ThisDevice.FirmwareVersion = "alpha";
            Device.ThisDevice.SerialNumber = "12345678";

            // Add a Host service type
            Device.Host = new SimpleDeviceHost();

            // Add Dpws hosted service(s) to the device
            Device.HostedServices.Add(new SimpleService(new SimpleServiceImplementation()));
            EventingService eventingService = new EventingService();
            Device.HostedServices.Add(eventingService);
            Device.HostedServices.Add(new AttachmentService(new AttachmentServiceImplementation()));

            // Add a Dpws client to this device. Uncomment to run a client and device
            //client = new SimpleServiceClient();

            // Set this device property if you want to ignore this clients request
            Device.IgnoreLocalClientRequest = false;

            // Turn console messages on
            Console.Verbose = true;
            
            // Start the device
            Device.Start();

            // Create and start EventSimulator
            EventSimulator eventSimulator = new EventSimulator(eventingService);
            eventSimulator.StartEventSimulator();
        }
    }
}
