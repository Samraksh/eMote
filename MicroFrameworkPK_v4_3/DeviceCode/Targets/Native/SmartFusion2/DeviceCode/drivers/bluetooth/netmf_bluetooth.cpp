#include <tinyhal.h>
extern "C" {
#include "include\SS1BTPS.h"
#include "hcitrans\HCITRANS.h"
#include "btpskrnl\BTPSKRNL.h"
#include "include\HCICommT.h"
#include "include\HCIAPI.h"
#include "btpsvend\bvendapi.h"
#include "hcitrans\HCITRANS.h"
#include "include\GAPAPI.h"
#include "profiles\gaps\GAPSAPI.h"
#include "include\SPPLETyp.h"
};

   // The following string table is used to map HCI Version information 
   // to an easily displayable version string.                          
static BTPSCONST char *HCIVersionStrings[] =
{
   "1.0b",
   "1.1",
   "1.2",
   "2.0",
   "2.1",
   "3.0",
   "4.0",
   "4.1",
   "Unknown (greater 4.1)"
} ;

#define HAL_HCI_UART_MAX_BAUD_RATE      2000000
#define APPLICATION_ERROR_INVALID_PARAMETERS       (-1000)
#define APPLICATION_ERROR_UNABLE_TO_OPEN_STACK     (-1001)
#define UNABLE_TO_INITIALIZE_STACK                 (-7)  // Denotes that an error occurred while Initializing the Bluetooth Protocol Stack. 
#define INVALID_STACK_ID_ERROR                     (-8)  // Denotes that an occurred due to attempted  execution of a Command when a Bluetooth Protocol  Stack has not been opened.         
#define INVALID_PARAMETERS_ERROR                   (-6)
#define MAX_INQUIRY_RESULTS                        (10)  // Denotes the max number of inquiry results
#define MAX_SUPPORTED_LINK_KEYS                     (1)  // Max supported Link keys
#define DEFAULT_LE_IO_CAPABILITY   (licNoInputNoOutput)  // Denotes the default I/O Capability that is  used with LE Pairing 
#define DEFAULT_LE_MITM_PROTECTION              (TRUE)   // Denotes the default value used for Man in the Middle (MITM) protection used  with LE Pairing.
#define DEFAULT_IO_CAPABILITY          (icDisplayYesNo)  // Denotes the default I/O Capability that is used with Secure Simple Pairing. 
#define NUM_SUPPORTED_HCI_VERSIONS              (sizeof(HCIVersionStrings)/sizeof(char *) - 1)
#define DEFAULT_MITM_PROTECTION                  (TRUE)  // Denotes the  default value used for Man in the Middle (MITM) protection used with Secure Simple Pairing.         
#define LE_APP_DEMO_NAME                        "SF2 Bluetooth"

   // Bluetooth Protocol Demo Application Major Version Release Number. 
#define DEMO_APPLICATION_MAJOR_VERSION_NUMBER      0

   // Bluetooth Protocol Demo Application Minor Version Release Number. 
#define DEMO_APPLICATION_MINOR_VERSION_NUMBER      5

   // Constants used to convert numeric constants to string constants   
   // (used in MACRO's below).                                          
#define VERSION_NUMBER_TO_STRING(_x)               #_x
#define VERSION_CONSTANT_TO_STRING(_y)             VERSION_NUMBER_TO_STRING(_y)

//  Demo Application Version String of the form "a.b"                
// where:                                                            
//    a - DEMO_APPLICATION_MAJOR_VERSION_NUMBER                      
//    b - DEMO_APPLICATION_MINOR_VERSION_NUMBER                      
#define DEMO_APPLICATION_VERSION_STRING            VERSION_CONSTANT_TO_STRING(DEMO_APPLICATION_MAJOR_VERSION_NUMBER) "." VERSION_CONSTANT_TO_STRING(DEMO_APPLICATION_MINOR_VERSION_NUMBER)
#define MAX_SIMULTANEOUS_SPP_PORTS                  (1) // Maximum SPP Ports  
                                                        // that we support.   
#define SPPLE_DATA_CREDITS        (SPPLE_DATA_BUFFER_LENGTH*1) // Defines the 
                                                         // number of credits 
                                                         // in an SPPLE Buffer
#define SPPLE_DATA_BUFFER_LENGTH  (BTPS_CONFIGURATION_GATT_MAXIMUM_SUPPORTED_MTU_SIZE)
                                                         // Defines the length
                                                         // of a SPPLE Data   
                                                         // Buffer.           






   // The following structure holds status information about a send     
   // process.                                                          
typedef struct _tagSend_Info_t
{
   Boolean_t    BufferFull;
   DWord_t      BytesToSend;
   DWord_t      BytesSent;
   unsigned int DataStrIndex;
} Send_Info_t;


// The following structure represents the information we will store  
// on a Discovered GAP Service.                                      
typedef struct _tagGAPS_Client_Info_t
{
   Word_t DeviceNameHandle;
   Word_t DeviceAppearanceHandle;
} GAPS_Client_Info_t;

// The following structure is used to hold a list of information on all paired devices.                                            
typedef struct _tagDeviceInfo_t
{
   Byte_t                   Flags;
   Byte_t                   EncryptionKeySize;
   GAP_LE_Address_Type_t    ConnectionAddressType;
   BD_ADDR_t                ConnectionBD_ADDR;
   Long_Term_Key_t          LTK;
   Random_Number_t          Rand;
   Word_t                   EDIV;
   GAPS_Client_Info_t       GAPSClientInfo;
   SPPLE_Client_Info_t      ClientInfo;
   SPPLE_Server_Info_t      ServerInfo;
   struct _tagDeviceInfo_t *NextDeviceInfoInfoPtr;
} DeviceInfo_t;

// Structure used to hold all of the GAP LE Parameters.              
typedef struct _tagGAPLE_Parameters_t
{
   GAP_LE_Connectability_Mode_t ConnectableMode;
   GAP_Discoverability_Mode_t   DiscoverabilityMode;
   GAP_LE_IO_Capability_t       IOCapability;
   Boolean_t                    MITMProtection;
   Boolean_t                    OOBDataPresent;
} GAPLE_Parameters_t;

// The following type definition represents the container type which 
// holds the mapping between Bluetooth devices (based on the BD_ADDR)
// and the Link Key (BD_ADDR <-> Link Key Mapping).                 
typedef struct _tagLinkKeyInfo_t
{
   BD_ADDR_t  BD_ADDR;
   Link_Key_t LinkKey;
} LinkKeyInfo_t;

   // The following defines the format of a SPPLE Data Buffer.          
typedef struct _tagSPPLE_Data_Buffer_t
{
   unsigned int InIndex;
   unsigned int OutIndex;
   unsigned int BytesFree;
   unsigned int BufferSize;
   Byte_t       Buffer[SPPLE_DATA_CREDITS];
} SPPLE_Data_Buffer_t;


   // The following structure is used to hold all of the SPPLE related  
   // information pertaining to buffers and credits.                    
typedef struct _tagSPPLE_Buffer_Info_t
{
   Send_Info_t         SendInfo;
   unsigned int        TransmitCredits;
   Word_t              QueuedCredits;
   SPPLE_Data_Buffer_t ReceiveBuffer;
   SPPLE_Data_Buffer_t TransmitBuffer;
} SPPLE_Buffer_Info_t;


   // The following structure is used to hold information on a connected
   // LE Device.                                                        
typedef struct _tagLE_Context_Info_t
{
   BD_ADDR_t           ConnectionBD_ADDR;
   unsigned int        ConnectionID;
   SPPLE_Buffer_Info_t SPPLEBufferInfo;
   Boolean_t           BufferFull;
}  LE_Context_Info_t;

   // The following defines the structure that is used to hold          
   // information about all open SPP Ports.                             
typedef struct SPP_Context_Info_t
{
   unsigned int  LocalSerialPortID;
   unsigned int  ServerPortNumber;
   Word_t        Connection_Handle;
   BD_ADDR_t     BD_ADDR;
   DWord_t       SPPServerSDPHandle;
   Boolean_t     Connected;
   Send_Info_t   SendInfo;

#if MAXIMUM_SPP_LOOPBACK_BUFFER_SIZE > 0

   unsigned int  BufferLength;
   unsigned char Buffer[MAXIMUM_SPP_LOOPBACK_BUFFER_SIZE];

#endif

} SPP_Context_Info_t;




static unsigned int BluetoothStackID;
static unsigned int InputIndex;
static Boolean_t SleepAllowed;
static BTPS_Initialization_t         BTPS_Initialization;
static HCI_DriverInformation_t       HCI_DriverInformation;
static HCI_HCILLConfiguration_t      HCILLConfig;
static HCI_Driver_Reconfigure_Data_t DriverReconfigureData;
static GAPLE_Parameters_t  LE_Parameters;           // Holds GAP Parameters like Discoverability, Connectability Modes.    
static unsigned int        GAPSInstanceID;          // Holds the Instance ID for the  GAP Service.
static unsigned int        SPPLEServiceID;          // The following holds the SPP LE Service ID that is returned from GATT_Register_Service().
static DeviceInfo_t       *DeviceInfoList;          // Holds the list head for the device info list.             
typedef char BoardStr_t[16];    					// User to represent a structure to hold a BD_ADDR return from BD_ADDRToStr    
static BD_ADDR_t           InquiryResultList[MAX_INQUIRY_RESULTS]; // Variable which contains the inquiry result received from the most recently preformed inquiry.
static unsigned int        NumberofValidResponses;  // Variable which holds the number of valid inquiry results within the inquiry results array.
static LinkKeyInfo_t       LinkKeyInfo[MAX_SUPPORTED_LINK_KEYS]; // Variable holds  BD_ADDR <-> Link Keys for  pairing. 
static BD_ADDR_t           CurrentCBRemoteBD_ADDR;  // Variable which holds the current CB BD_ADDR of the device which is currently pairing or authenticating  
static GAP_IO_Capability_t IOCapability;            // Variable which holds the current I/O Capabilities that  are to be used for Secure Simple Pairing      
static Boolean_t           MITMProtection;          // Variable which flags whether or not Man in the Middle (MITM)  protection is to be requested during a Secure Simple Pairing procedure
static Boolean_t           OOBSupport;              // Variable which flags whether  or not Out of Band Secure Simple  Pairing exchange is supported.  
typedef char BoardStr_t[16]; // User to represent a structure to hold a BD_ADDR return from  BD_ADDRToStr. 
static BD_ADDR_t           CurrentLERemoteBD_ADDR;  // Variable which holds the current LE BD_ADDR of the device which is currently pairing or authenticating.    
static Boolean_t           LocalDeviceIsMaster;     // Boolean that tells if the local device is the master of the current connection. 
#define MAX_LE_CONNECTIONS                          (2)  // Denotes the max number of LE connections that are allowed at the same time.   
static LE_Context_Info_t   LEContextInfo[MAX_LE_CONNECTIONS]; // Array that contains the connection ID and BD_ADDR of each connected device. 
static BTPSCONST Encryption_Key_t ER = {0x28, 0xBA, 0xE1, 0x37, 0x13, 0xB2, 0x20, 0x45, 0x16, 0xB2, 0x19, 0xD0, 0x80, 0xEE, 0x4A, 0x51}; //The Encryption Root Key should be generated in such a way as to guarantee 128 bits of entropy.
static BTPSCONST Encryption_Key_t IR = {0x41, 0x09, 0xA0, 0x88, 0x09, 0x6B, 0x70, 0xC0, 0x95, 0x23, 0x3C, 0x8C, 0x48, 0xFC, 0xC9, 0xFE}; // The Identity Root Key should be generated in such a way as to guarantee 128 bits of entropy.

// The following keys can be regenerated on the 
// fly using the constant IR and ER keys and    
// are used globally, for all devices.         
static Encryption_Key_t DHK;
static Encryption_Key_t IRK;
static SPP_Context_Info_t  SPPContextInfo[MAX_SIMULTANEOUS_SPP_PORTS];
                                                    // Variable that contains          
                                                    // information about the current   
                                                    // open SPP Ports                  
   // The following structure is used to hold information of the        
   // FIRMWARE version.                                                 
typedef struct FW_Version_t
{
   Byte_t StatusResult;
   Byte_t HCI_VersionResult;
   Word_t HCI_RevisionResult;
   Byte_t LMP_VersionResult;
   Word_t Manufacturer_NameResult;
   Word_t LMP_SubversionResult;
} FW_Version;



static void DisplayFWVersion (void);
static int DeleteLinkKey(BD_ADDR_t BD_ADDR);
static void BD_ADDRToStr(BD_ADDR_t Board_Address, BoardStr_t BoardStr);
static void BTPSAPI GATT_Connection_Event_Callback(unsigned int BluetoothStackID, GATT_Connection_Event_Data_t *GATT_Connection_Event_Data, unsigned long CallbackParameter);

int Bluetooth_MessageOutputCallback(int length, char* Message){
	hal_printf("Bluetooth message callback: ");
	hal_printf(Message);

	return 0;
}

static int OpenStack(HCI_DriverInformation_t *HCI_DriverInformation, BTPS_Initialization_t *BTPS_Initialization)
{
   int                           i;
   int                           Result;
   int                           ret_val = 0;
   char                          BluetoothAddress[16];
   Byte_t                        Status;
   Byte_t                        NumberLEPackets;
   Word_t                        LEPacketLength;
   BD_ADDR_t                     BD_ADDR;
   unsigned int                  ServiceID;
   HCI_Version_t                 HCIVersion;
   L2CA_Link_Connect_Params_t    L2CA_Link_Connect_Params;

   hal_printf(("\r\nOpenStack().\r\n"));
   // First check to see if the Stack has already been opened.          
   if(!BluetoothStackID)
   {
      // Next, makes sure that the Driver Information passed appears to 
      // be semi-valid.                                                 
      if(HCI_DriverInformation)
      {
         // Initialize BTPSKNRL.                                        
		BTPS_Init((void *)BTPS_Initialization);

		 // these two force compile functions allow their files to be linked in properly, otherwise the linker can't find functions the bluetooth library needs
		 HCI_ForceCompile(HCI_DriverInformation);
		 BTPSAPI_ForceCompile(HCI_DriverInformation);
         // Initialize the Stack                                        
         Result = BSC_Initialize(HCI_DriverInformation, 0);

         // Next, check the return value of the initialization to see   
         // if it was successful.                                       
         if(Result > 0)
         {
            // The Stack was initialized successfully, inform the user  
            // and set the return value of the initialization function  
            // to the Bluetooth Stack ID.                               
            BluetoothStackID = Result;
            hal_printf("Bluetooth Stack ID: %d\r\n", BluetoothStackID);

            ret_val          = 0;

            // Attempt to enable the WBS feature.                       
            Result = BSC_EnableFeature(BluetoothStackID, BSC_FEATURE_BLUETOOTH_LOW_ENERGY);
            if(!Result)
            {
               hal_printf("LOW ENERGY Support initialized.\r\n");
            }
            else
            {
               hal_printf("LOW ENERGY Support not initialized %d.\r\n", Result);
            }


            // Initialize the Default Pairing Parameters.               
            LE_Parameters.IOCapability   = DEFAULT_LE_IO_CAPABILITY;
            LE_Parameters.MITMProtection = DEFAULT_LE_MITM_PROTECTION;
            LE_Parameters.OOBDataPresent = FALSE;

            // Initialize the default Secure Simple Pairing parameters. 
            IOCapability                 = DEFAULT_IO_CAPABILITY;
            OOBSupport                   = FALSE;
            MITMProtection               = DEFAULT_MITM_PROTECTION;

            if(!HCI_Version_Supported(BluetoothStackID, &HCIVersion))
               hal_printf("Device Chipset: %s\r\n", (HCIVersion <= NUM_SUPPORTED_HCI_VERSIONS)?HCIVersionStrings[HCIVersion]:HCIVersionStrings[NUM_SUPPORTED_HCI_VERSIONS]);

            // Printing the BTPS version                                
            hal_printf("BTPS Version  : %s \r\n", BTPS_VERSION_VERSION_STRING);
            // Printing the FW version                                  
            DisplayFWVersion();

            // Printing the Demo Application name and version           
            hal_printf("App Name      : %s \r\n", LE_APP_DEMO_NAME);
            hal_printf("App Version   : %s \r\n", DEMO_APPLICATION_VERSION_STRING);

            // Let's output the Bluetooth Device Address so that the    
            // user knows what the Device Address is.                   
            if(!GAP_Query_Local_BD_ADDR(BluetoothStackID, &BD_ADDR))
            {
               BD_ADDRToStr(BD_ADDR, BluetoothAddress);

               hal_printf("LOCAL BD_ADDR: %s\r\n", BluetoothAddress);
            }

            if(HCI_Command_Supported(BluetoothStackID, HCI_SUPPORTED_COMMAND_WRITE_DEFAULT_LINK_POLICY_BIT_NUMBER) > 0)
               HCI_Write_Default_Link_Policy_Settings(BluetoothStackID, (HCI_LINK_POLICY_SETTINGS_ENABLE_MASTER_SLAVE_SWITCH|HCI_LINK_POLICY_SETTINGS_ENABLE_SNIFF_MODE), &Status);

            // Go ahead and allow Master/Slave Role Switch.             
            L2CA_Link_Connect_Params.L2CA_Link_Connect_Request_Config  = cqAllowRoleSwitch;
            L2CA_Link_Connect_Params.L2CA_Link_Connect_Response_Config = csMaintainCurrentRole;

            L2CA_Set_Link_Connection_Configuration(BluetoothStackID, &L2CA_Link_Connect_Params);

            if(HCI_Command_Supported(BluetoothStackID, HCI_SUPPORTED_COMMAND_WRITE_DEFAULT_LINK_POLICY_BIT_NUMBER) > 0)
               HCI_Write_Default_Link_Policy_Settings(BluetoothStackID, (HCI_LINK_POLICY_SETTINGS_ENABLE_MASTER_SLAVE_SWITCH|HCI_LINK_POLICY_SETTINGS_ENABLE_SNIFF_MODE), &Status);

            // Delete all Stored Link Keys.                             
            ASSIGN_BD_ADDR(BD_ADDR, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

            DeleteLinkKey(BD_ADDR);

            // Flag that no connection is currently active.             
            ASSIGN_BD_ADDR(CurrentLERemoteBD_ADDR, 0, 0, 0, 0, 0, 0);
            ASSIGN_BD_ADDR(CurrentCBRemoteBD_ADDR, 0, 0, 0, 0, 0, 0);
            LocalDeviceIsMaster = FALSE;

            for(i=0; i<MAX_LE_CONNECTIONS; i++)
            {
               LEContextInfo[i].ConnectionID = 0;
               ASSIGN_BD_ADDR(LEContextInfo[i].ConnectionBD_ADDR, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
            }

            // Regenerate IRK and DHK from the constant Identity Root   
            // Key.                                                     
            GAP_LE_Diversify_Function(BluetoothStackID, (Encryption_Key_t *)(&IR), 1,0, &IRK);
            GAP_LE_Diversify_Function(BluetoothStackID, (Encryption_Key_t *)(&IR), 3, 0, &DHK);

            // Flag that we have no Key Information in the Key List.    
            DeviceInfoList = NULL;

            // Initialize the GATT Service.                             
            if((Result = GATT_Initialize(BluetoothStackID, GATT_INITIALIZATION_FLAGS_SUPPORT_LE, GATT_Connection_Event_Callback, 0)) == 0)
            {
               // Determine the number of LE packets that the controller
               // will accept at a time.                                
               if((!HCI_LE_Read_Buffer_Size(BluetoothStackID, &Status, &LEPacketLength, &NumberLEPackets)) && (!Status) && (LEPacketLength))
               {
                  NumberLEPackets = (NumberLEPackets/MAX_LE_CONNECTIONS);
                  NumberLEPackets = (NumberLEPackets == 0)?1:NumberLEPackets;
               }
               else
                  NumberLEPackets = 1;

               // Set a limit on the number of packets that we will     
               // queue internally.                                     
               GATT_Set_Queuing_Parameters(BluetoothStackID, (unsigned int)NumberLEPackets, (unsigned int)(NumberLEPackets-1), FALSE);

               // Initialize the GAPS Service.                          
               Result = GAPS_Initialize_Service(BluetoothStackID, &ServiceID);
               if(Result > 0)
               {
                  // Save the Instance ID of the GAP Service.           
                  GAPSInstanceID = (unsigned int)Result;

                  // Set the GAP Device Name and Device Appearance.     
                  GAP_Set_Local_Device_Name (BluetoothStackID,(char*)LE_APP_DEMO_NAME);
                  GAPS_Set_Device_Name(BluetoothStackID, GAPSInstanceID, (char*)LE_APP_DEMO_NAME);
                  GAPS_Set_Device_Appearance(BluetoothStackID, GAPSInstanceID, GAP_DEVICE_APPEARENCE_VALUE_GENERIC_COMPUTER);

                  // Return success to the caller.                      
                  ret_val        = 0;
               }
               else
               {
                  // The Stack was NOT initialized successfully, inform 
                  // the user and set the return value of the           
                  // initialization function to an error.               
                  hal_printf("GAPS_Initialize_Service %d\r\n", Result);

                  // Cleanup GATT Module.                               
                  GATT_Cleanup(BluetoothStackID);

                  BluetoothStackID = 0;

                  ret_val          = UNABLE_TO_INITIALIZE_STACK;
               }
            }
            else
            {
               // The Stack was NOT initialized successfully, inform the
               // user and set the return value of the initialization   
               // function to an error.                                 
               hal_printf("GATT_Initialize %d\r\n", Result);

               BluetoothStackID = 0;

               ret_val          = UNABLE_TO_INITIALIZE_STACK;
            }

            // Initialize SPP context.                                  
            BTPS_MemInitialize(SPPContextInfo, 0, sizeof(SPPContextInfo));
         }
         else
         {
            // The Stack was NOT initialized successfully, inform the   
            // user and set the return value of the initialization      
            // function to an error.                                    
            hal_printf("BSC_Initialize %d\r\n", Result);

            BluetoothStackID = 0;

            ret_val          = UNABLE_TO_INITIALIZE_STACK;
         }
      }
      else
      {
         // One or more of the necessary parameters are invalid.        
         ret_val = INVALID_PARAMETERS_ERROR;
      }
   }

   return(ret_val);
}

   // The following function is for displaying The FW Version by reading
   // The Local version information form the FW.                        
static void DisplayFWVersion (void)
{
    FW_Version FW_Version_Details;

    // This function retrieves the Local Version Information of the FW. 
    HCI_Read_Local_Version_Information(BluetoothStackID, &FW_Version_Details.StatusResult, &FW_Version_Details.HCI_VersionResult, &FW_Version_Details.HCI_RevisionResult, &FW_Version_Details.LMP_VersionResult, &FW_Version_Details.Manufacturer_NameResult, &FW_Version_Details.LMP_SubversionResult);
    if (!FW_Version_Details.StatusResult)
    {
        // This function prints The project type from firmware, Bits    
        // 10 to 14 (5 bits) from LMP_SubversionResult parameter.       
        hal_printf("Project Type  : %d \r\n", ((FW_Version_Details.LMP_SubversionResult >> 10)) & 0x1F);
        // This function prints The version of the firmware. The first  
        // number is the Major version, Bits 7 to 9 and 15 (4 bits) from
        // LMP_SubversionResult parameter, the second number is the     
        // Minor Version, Bits 0 to 6 (7 bits) from LMP_SubversionResult
        // parameter.                                                   
        hal_printf("FW Version    : %d.%d \r\n", ((FW_Version_Details.LMP_SubversionResult >> 7) & 0x07) + ((FW_Version_Details.LMP_SubversionResult >> 12) & 0x08), FW_Version_Details.LMP_SubversionResult & 0x7F);
    }
    else
        // There was an error with HCI_Read_Local_Version_Information.  
        // Function.                                                    
        hal_printf("HCI_Read_Local_Version_Information %d", FW_Version_Details.StatusResult);
}

  // The following function is a utility function that exists to delete
   // the specified Link Key from the Local Bluetooth Device.  If a NULL
   // Bluetooth Device Address is specified, then all Link Keys will be 
   // deleted.                                                          
static int DeleteLinkKey(BD_ADDR_t BD_ADDR)
{
   int       Result;
   Byte_t    Status_Result;
   Word_t    Num_Keys_Deleted = 0;
   BD_ADDR_t NULL_BD_ADDR;

   Result = HCI_Delete_Stored_Link_Key(BluetoothStackID, BD_ADDR, TRUE, &Status_Result, &Num_Keys_Deleted);

   // Any stored link keys for the specified address (or all) have been 
   // deleted from the chip.  Now, let's make sure that our stored Link 
   // Key Array is in sync with these changes.                          

   // First check to see all Link Keys were deleted.                    
   ASSIGN_BD_ADDR(NULL_BD_ADDR, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

   if(COMPARE_BD_ADDR(BD_ADDR, NULL_BD_ADDR))
      BTPS_MemInitialize(LinkKeyInfo, 0, sizeof(LinkKeyInfo));
   else
   {
      // Individual Link Key.  Go ahead and see if know about the entry 
      // in the list.                                                   
      for(Result=0;(Result<sizeof(LinkKeyInfo)/sizeof(LinkKeyInfo_t));Result++)
      {
         if(COMPARE_BD_ADDR(BD_ADDR, LinkKeyInfo[Result].BD_ADDR))
         {
            LinkKeyInfo[Result].BD_ADDR = NULL_BD_ADDR;

            break;
         }
      }
   }

   return(Result);
}



   // The following function is responsible for converting data of type 
   // BD_ADDR to a string.  The first parameter of this function is the 
   // BD_ADDR to be converted to a string.  The second parameter of this
   // function is a pointer to the string in which the converted BD_ADDR
   // is to be stored.                                                  
static void BD_ADDRToStr(BD_ADDR_t Board_Address, BoardStr_t BoardStr)
{
   BTPS_SprintF((char *)BoardStr, "0x%02X%02X%02X%02X%02X%02X", Board_Address.BD_ADDR5, Board_Address.BD_ADDR4, Board_Address.BD_ADDR3, Board_Address.BD_ADDR2, Board_Address.BD_ADDR1, Board_Address.BD_ADDR0);
}

   // The following function is responsible for processing HCI Mode     
   // change events.                                                    
static void BTPSAPI HCI_Event_Callback(unsigned int BluetoothStackID, HCI_Event_Data_t *HCI_Event_Data, unsigned long CallbackParameter)
{
   char *Mode;

#if ((SPP_PERFORM_MASTER_ROLE_SWITCH) && (MAX_SIMULTANEOUS_SPP_PORTS > 1))
   int   SerialPortIndex;
#endif

   // Make sure that the input parameters that were passed to us are    
   // semi-valid.                                                       
   if((BluetoothStackID) && (HCI_Event_Data))
   {
      // Process the Event Data.                                        
      switch(HCI_Event_Data->Event_Data_Type)
      {

#if ((SPP_PERFORM_MASTER_ROLE_SWITCH) && (MAX_SIMULTANEOUS_SPP_PORTS > 1))

         case etRole_Change_Event:
            if(HCI_Event_Data->Event_Data.HCI_Role_Change_Event_Data)
            {
               // Find the Serial Port entry for this event.            
               if((SerialPortIndex = FindSPPPortIndexByAddress(HCI_Event_Data->Event_Data.HCI_Role_Change_Event_Data->BD_ADDR)) >= 0)
               {
                  if((HCI_Event_Data->Event_Data.HCI_Role_Change_Event_Data->Status == HCI_ERROR_CODE_NO_ERROR) && (HCI_Event_Data->Event_Data.HCI_Role_Change_Event_Data->New_Role == HCI_CURRENT_ROLE_MASTER))
                  {
                     hal_printf(("\r\nSPP Port %u: Role Change Success. %d\r\n", SPPContextInfo[SerialPortIndex].LocalSerialPortID));
                  }
                  else
                  {
                     hal_printf(("\r\nSPP Port %u: Role Change Failure (Status 0x%02X, Role 0x%02X).\r\n", SPPContextInfo[SerialPortIndex].LocalSerialPortID, HCI_Event_Data->Event_Data.HCI_Role_Change_Event_Data->Status, HCI_Event_Data->Event_Data.HCI_Role_Change_Event_Data->New_Role));
                  }

                  //DisplayPrompt();
               }
            }
            break;

#endif

         case etMode_Change_Event:
            if(HCI_Event_Data->Event_Data.HCI_Mode_Change_Event_Data)
            {
               switch(HCI_Event_Data->Event_Data.HCI_Mode_Change_Event_Data->Current_Mode)
               {
                  case HCI_CURRENT_MODE_HOLD_MODE:
                     Mode = "Hold";
                     break;
                  case HCI_CURRENT_MODE_SNIFF_MODE:
                     Mode = "Sniff";
                     break;
                  case HCI_CURRENT_MODE_PARK_MODE:
                     Mode = "Park";
                     break;
                  case HCI_CURRENT_MODE_ACTIVE_MODE:
                  default:
                     Mode = "Active";
                     break;
               }

               hal_printf("\r\n");
               hal_printf("HCI Mode Change Event, Status: 0x%02X, Connection Handle: %d, Mode: %s\r\n", HCI_Event_Data->Event_Data.HCI_Mode_Change_Event_Data->Status, HCI_Event_Data->Event_Data.HCI_Mode_Change_Event_Data->Connection_Handle, Mode);
               //DisplayPrompt();
            }
            break;
      }
   }
}

  // The following function is for an GATT Connection Event Callback.  
   // This function is called for GATT Connection Events that occur on  
   // the specified Bluetooth Stack.  This function passes to the caller
   // the GATT Connection Event Data that occurred and the GATT         
   // Connection Event Callback Parameter that was specified when this  
   // Callback was installed.  The caller is free to use the contents of
   // the GATT Client Event Data ONLY in the context of this callback.  
   // If the caller requires the Data for a longer period of time, then 
   // the callback function MUST copy the data into another Data Buffer.
   // This function is guaranteed NOT to be invoked more than once      
   // simultaneously for the specified installed callback (i.e.  this   
   // function DOES NOT have be reentrant).  It Needs to be noted       
   // however, that if the same Callback is installed more than once,   
   // then the callbacks will be called serially.  Because of this, the 
   // processing in this function should be as efficient as possible.   
   // It should also be noted that this function is called in the Thread
   // Context of a Thread that the User does NOT own.  Therefore,       
   // processing in this function should be as efficient as possible    
   // (this argument holds anyway because another GATT Event            
   // (Server/Client or Connection) will not be processed while this    
   // function call is outstanding).                                    
   // * NOTE * This function MUST NOT Block and wait for Events that can
   //          only be satisfied by Receiving a Bluetooth Event         
   //          Callback.  A Deadlock WILL occur because NO Bluetooth    
   //          Callbacks will be issued while this function is currently
   //          outstanding.                                             
static void BTPSAPI GATT_Connection_Event_Callback(unsigned int BluetoothStackID, GATT_Connection_Event_Data_t *GATT_Connection_Event_Data, unsigned long CallbackParameter)
{
   int           LEConnectionIndex;
   Word_t        Credits;
   Boolean_t     SuppressResponse = FALSE;
   BoardStr_t    BoardStr;
   DeviceInfo_t *DeviceInfo;

   hal_printf("**** not finished GATT_Connection_Event_Callback\r\n");
   // Verify that all parameters to this callback are Semi-Valid.       
   /*if((BluetoothStackID) && (GATT_Connection_Event_Data))
   {
      // Determine the Connection Event that occurred.                  
      switch(GATT_Connection_Event_Data->Event_Data_Type)
      {
         case etGATT_Connection_Device_Connection:
            if(GATT_Connection_Event_Data->Event_Data.GATT_Device_Connection_Data)
            {
               // Update the ConnectionID associated with the BD_ADDR   
               // If UpdateConnectionID returns -1, then it failed.     
               if(UpdateConnectionID(GATT_Connection_Event_Data->Event_Data.GATT_Device_Connection_Data->ConnectionID, GATT_Connection_Event_Data->Event_Data.GATT_Device_Connection_Data->RemoteDevice) < 0)
                   Display(("Error - No matching ConnectionBD_ADDR found."));

               Display(("\r\netGATT_Connection_Device_Connection with size %u: \r\n", GATT_Connection_Event_Data->Event_Data_Size));
               BD_ADDRToStr(GATT_Connection_Event_Data->Event_Data.GATT_Device_Connection_Data->RemoteDevice, BoardStr);
               Display(("   Connection ID:   %u.\r\n", GATT_Connection_Event_Data->Event_Data.GATT_Device_Connection_Data->ConnectionID));
               Display(("   Connection Type: %s.\r\n", ((GATT_Connection_Event_Data->Event_Data.GATT_Device_Connection_Data->ConnectionType == gctLE)?"LE":"BR/EDR")));
               Display(("   Remote Device:   %s.\r\n", BoardStr));
               Display(("   Connection MTU:  %u.\r\n", GATT_Connection_Event_Data->Event_Data.GATT_Device_Connection_Data->MTU));

               // Find the LE Connection Index for this connection.     
               if((LEConnectionIndex = FindLEIndexByAddress(GATT_Connection_Event_Data->Event_Data.GATT_Device_Connection_Data->RemoteDevice)) >= 0)
               {
                  // Search for the device info for the connection.     
                  if((DeviceInfo = SearchDeviceInfoEntryByBD_ADDR(&DeviceInfoList, LEContextInfo[LEConnectionIndex].ConnectionBD_ADDR)) != NULL)
                  {
                     // Clear the SPPLE Role Flag.                      
                     DeviceInfo->Flags &= ~DEVICE_INFO_FLAGS_SPPLE_SERVER;

                     // Initialize the Transmit and Receive Buffers.    
                     InitializeBuffer(&(LEContextInfo[LEConnectionIndex].SPPLEBufferInfo.ReceiveBuffer));
                     InitializeBuffer(&(LEContextInfo[LEConnectionIndex].SPPLEBufferInfo.TransmitBuffer));

                     // Flag that we do not have any transmit credits   
                     // yet.                                            
                     LEContextInfo[LEConnectionIndex].SPPLEBufferInfo.TransmitCredits = 0;

                     // Flag that no credits are queued.                
                     LEContextInfo[LEConnectionIndex].SPPLEBufferInfo.QueuedCredits   = 0;

                     if(!LocalDeviceIsMaster)
                     {
                        // Flag that we will act as the Server.         
                        DeviceInfo->Flags |= DEVICE_INFO_FLAGS_SPPLE_SERVER;

                        // Send the Initial Credits if the Rx Credit CCD
                        // is already configured (for a bonded device   
                        // this could be the case).                     
                        SPPLESendCredits(&(LEContextInfo[LEConnectionIndex]), DeviceInfo, LEContextInfo[LEConnectionIndex].SPPLEBufferInfo.ReceiveBuffer.BytesFree);
                     }
                     else
                     {
                        // Attempt to update the MTU to the maximum     
                        // supported.                                   
                        GATT_Exchange_MTU_Request(BluetoothStackID, GATT_Connection_Event_Data->Event_Data.GATT_Device_Connection_Data->ConnectionID, BTPS_CONFIGURATION_GATT_MAXIMUM_SUPPORTED_MTU_SIZE, GATT_ClientEventCallback_SPPLE, 0);
                     }
                  }
               }
            }
            else
               Display(("Error - Null Connection Data.\r\n"));
            break;
         case etGATT_Connection_Device_Disconnection:
            if(GATT_Connection_Event_Data->Event_Data.GATT_Device_Disconnection_Data)
            {
               // Clear the Connection ID.                              
               RemoveConnectionInfo(GATT_Connection_Event_Data->Event_Data.GATT_Device_Disconnection_Data->RemoteDevice);

               Display(("\r\netGATT_Connection_Device_Disconnection with size %u: \r\n", GATT_Connection_Event_Data->Event_Data_Size));
               BD_ADDRToStr(GATT_Connection_Event_Data->Event_Data.GATT_Device_Disconnection_Data->RemoteDevice, BoardStr);
               Display(("   Connection ID:   %u.\r\n", GATT_Connection_Event_Data->Event_Data.GATT_Device_Disconnection_Data->ConnectionID));
               Display(("   Connection Type: %s.\r\n", ((GATT_Connection_Event_Data->Event_Data.GATT_Device_Disconnection_Data->ConnectionType == gctLE)?"LE":"BR/EDR")));
               Display(("   Remote Device:   %s.\r\n", BoardStr));
            }
            else
               Display(("Error - Null Disconnection Data.\r\n"));
            break;
         case etGATT_Connection_Device_Buffer_Empty:
            if(GATT_Connection_Event_Data->Event_Data.GATT_Device_Buffer_Empty_Data)
            {
               // Find the LE Connection Index for this connection.     
               if((LEConnectionIndex = FindLEIndexByAddress(GATT_Connection_Event_Data->Event_Data.GATT_Device_Buffer_Empty_Data->RemoteDevice)) >= 0)
               {
                  // Grab the device info for the currently connected   
                  // device.                                            
                  if((DeviceInfo = SearchDeviceInfoEntryByBD_ADDR(&DeviceInfoList, LEContextInfo[LEConnectionIndex].ConnectionBD_ADDR)) != NULL)
                  {
                     // Flag that the buffer is no longer empty.        
                     LEContextInfo[LEConnectionIndex].BufferFull = FALSE;

                     // Attempt to send any queued credits that we may  
                     // have.                                           
                     SPPLESendCredits(&(LEContextInfo[LEConnectionIndex]), DeviceInfo, 0);

                     // If may be possible for transmit queued data now.
                     // So fake a Receive Credit event with 0 as the    
                     // received credits.                               
                     SPPLEReceiveCreditEvent(&(LEContextInfo[LEConnectionIndex]), DeviceInfo, 0);

                     // Suppress the command prompt.                    
                     SuppressResponse   = TRUE;
                  }
               }
            }
            break;
         case etGATT_Connection_Server_Notification:
            if(GATT_Connection_Event_Data->Event_Data.GATT_Server_Notification_Data)
            {
               // Find the LE Connection Index for this connection.     
               if((LEConnectionIndex = FindLEIndexByAddress(GATT_Connection_Event_Data->Event_Data.GATT_Server_Notification_Data->RemoteDevice)) >= 0)
               {
                  // Find the Device Info for the device that has sent  
                  // us the notification.                               
                  if((DeviceInfo = SearchDeviceInfoEntryByBD_ADDR(&DeviceInfoList, LEContextInfo[LEConnectionIndex].ConnectionBD_ADDR)) != NULL)
                  {
                     // Determine the characteristic that is being      
                     // notified.                                       
                     if(GATT_Connection_Event_Data->Event_Data.GATT_Server_Notification_Data->AttributeHandle == DeviceInfo->ClientInfo.Rx_Credit_Characteristic)
                     {
                        // Verify that the length of the Rx Credit      
                        // Notification is correct.                     
                        if(GATT_Connection_Event_Data->Event_Data.GATT_Server_Notification_Data->AttributeValueLength == WORD_SIZE)
                        {
                           Credits = READ_UNALIGNED_WORD_LITTLE_ENDIAN(GATT_Connection_Event_Data->Event_Data.GATT_Server_Notification_Data->AttributeValue);

                           // Handle the received credits event.        
                           SPPLEReceiveCreditEvent(&(LEContextInfo[LEConnectionIndex]), DeviceInfo, Credits);

                           // Suppress the command prompt.              
                           SuppressResponse   = TRUE;
                        }
                     }
                     else
                     {
                        if(GATT_Connection_Event_Data->Event_Data.GATT_Server_Notification_Data->AttributeHandle == DeviceInfo->ClientInfo.Tx_Characteristic)
                        {
                           // This is a Tx Characteristic Event.  So    
                           // call the function to handle the data      
                           // indication event.                         
                           SPPLEDataIndicationEvent(&(LEContextInfo[LEConnectionIndex]), DeviceInfo, GATT_Connection_Event_Data->Event_Data.GATT_Server_Notification_Data->AttributeValueLength, GATT_Connection_Event_Data->Event_Data.GATT_Server_Notification_Data->AttributeValue);

                           // If we are not looping back or doing       
                           // automatic reads we will display the       
                           // prompt.                                   
                           if((!AutomaticReadActive) && (!LoopbackActive))
                              SuppressResponse = FALSE;
                           else
                              SuppressResponse = TRUE;
                        }
                     }
                  }
               }
            }
            else
               Display(("Error - Null Server Notification Data.\r\n"));
            break;
      }

      // Print the command line prompt.                                 
      if(!SuppressResponse)
         DisplayPrompt();
   }
   else
   {
      // There was an error with one or more of the input parameters.   
      Display(("\r\n"));

      Display(("GATT Connection Callback Data: Event_Data = NULL.\r\n"));

      DisplayPrompt();
   }*/
}


// The following function is for the GAP Event Receive Data Callback.
   // This function will be called whenever a Callback has been         
   // registered for the specified GAP Action that is associated with   
   // the Bluetooth Stack.  This function passes to the caller the GAP  
   // Event Data of the specified Event and the GAP Event Callback      
   // Parameter that was specified when this Callback was installed.    
   // The caller is free to use the contents of the GAP Event Data ONLY 
   // in the context of this callback.  If the caller requires the Data 
   // for a longer period of time, then the callback function MUST copy 
   // the data into another Data Buffer.  This function is guaranteed   
   // NOT to be invoked more than once simultaneously for the specified 
   // installed callback (i.e.  this function DOES NOT have be          
   // reentrant).  It Needs to be noted however, that if the same       
   // Callback is installed more than once, then the callbacks will be  
   // called serially.  Because of this, the processing in this function
   // should be as efficient as possible.  It should also be noted that 
   // this function is called in the Thread Context of a Thread that the
   // User does NOT own.  Therefore, processing in this function should 
   // be as efficient as possible (this argument holds anyway because   
   // other GAP Events will not be processed while this function call is
   // outstanding).                                                     
   // * NOTE * This function MUST NOT Block and wait for events that    
   //          can only be satisfied by Receiving other GAP Events.  A  
   //          Deadlock WILL occur because NO GAP Event Callbacks will  
   //          be issued while this function is currently outstanding.  
static void BTPSAPI GAP_Event_Callback(unsigned int BluetoothStackID, GAP_Event_Data_t *GAP_Event_Data, unsigned long CallbackParameter)
{
   int                               Result;
   int                               Index;
   BD_ADDR_t                         NULL_BD_ADDR;
   Boolean_t                         OOB_Data;
   Boolean_t                         MITM;
   BoardStr_t                        Callback_BoardStr;
   GAP_IO_Capability_t               RemoteIOCapability;
   GAP_Inquiry_Event_Data_t         *GAP_Inquiry_Event_Data;
   GAP_Remote_Name_Event_Data_t     *GAP_Remote_Name_Event_Data;
   GAP_Authentication_Information_t  GAP_Authentication_Information;

   // First, check to see if the required parameters appear to be       
   // semi-valid.                                                       
   if((BluetoothStackID) && (GAP_Event_Data))
   {
      // The parameters appear to be semi-valid, now check to see what  
      // type the incoming event is.                                    
      switch(GAP_Event_Data->Event_Data_Type)
      {
         case etInquiry_Result:
            // The GAP event received was of type Inquiry_Result.       
            GAP_Inquiry_Event_Data = GAP_Event_Data->Event_Data.GAP_Inquiry_Event_Data;

            // Next, Check to see if the inquiry event data received    
            // appears to be semi-valid.                                
            if(GAP_Inquiry_Event_Data)
            {
               // Now, check to see if the gap inquiry event data's     
               // inquiry data appears to be semi-valid.                
               if(GAP_Inquiry_Event_Data->GAP_Inquiry_Data)
               {
                  hal_printf("\r\n");

                  // Display a list of all the devices found from       
                  // performing the inquiry.                            
                  for(Index=0;(Index<GAP_Inquiry_Event_Data->Number_Devices) && (Index<MAX_INQUIRY_RESULTS);Index++)
                  {
                     InquiryResultList[Index] = GAP_Inquiry_Event_Data->GAP_Inquiry_Data[Index].BD_ADDR;
                     BD_ADDRToStr(GAP_Inquiry_Event_Data->GAP_Inquiry_Data[Index].BD_ADDR, Callback_BoardStr);

                     hal_printf("Result: %d,%s.\r\n", (Index+1), Callback_BoardStr);
                  }

                  NumberofValidResponses = GAP_Inquiry_Event_Data->Number_Devices;
               }
            }
            break;
         case etInquiry_Entry_Result:
            // Next convert the BD_ADDR to a string.                    
            BD_ADDRToStr(GAP_Event_Data->Event_Data.GAP_Inquiry_Entry_Event_Data->BD_ADDR, Callback_BoardStr);

            // Display this GAP Inquiry Entry Result.                   
            hal_printf(("\r\n"));
            hal_printf(("Inquiry Entry: %s.\r\n", Callback_BoardStr));
            break;
         case etAuthentication:
            // An authentication event occurred, determine which type of
            // authentication event occurred.                           
            switch(GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->GAP_Authentication_Event_Type)
            {
               case atLinkKeyRequest:
                  BD_ADDRToStr(GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device, Callback_BoardStr);
                  hal_printf("\r\n");
                  hal_printf("atLinkKeyRequest: %s\r\n", Callback_BoardStr);

                  // Setup the authentication information response      
                  // structure.                                         
                  GAP_Authentication_Information.GAP_Authentication_Type    = atLinkKey;
                  GAP_Authentication_Information.Authentication_Data_Length = 0;

                  // See if we have stored a Link Key for the specified 
                  // device.                                            
                  for(Index=0;Index<(sizeof(LinkKeyInfo)/sizeof(LinkKeyInfo_t));Index++)
                  {
                     if(COMPARE_BD_ADDR(LinkKeyInfo[Index].BD_ADDR, GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device))
                     {
                        // Link Key information stored, go ahead and    
                        // respond with the stored Link Key.            
                        GAP_Authentication_Information.Authentication_Data_Length   = sizeof(Link_Key_t);
                        GAP_Authentication_Information.Authentication_Data.Link_Key = LinkKeyInfo[Index].LinkKey;

                        break;
                     }
                  }

                  // Submit the authentication response.                
                  Result = GAP_Authentication_Response(BluetoothStackID, GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device, &GAP_Authentication_Information);

                  // Check the result of the submitted command.         
                  if(!Result)
                     hal_printf("GAP_Authentication_Response\r\n");
                  else
                     hal_printf("GAP_Authentication_Response %d\r\n", Result);
                  break;
               case atPINCodeRequest:
                  // A pin code request event occurred, first display   
                  // the BD_ADD of the remote device requesting the pin.
                  BD_ADDRToStr(GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device, Callback_BoardStr);
                  hal_printf("\r\n");
                  hal_printf("atPINCodeRequest: %s\r\n", Callback_BoardStr);

                  // Note the current Remote BD_ADDR that is requesting 
                  // the PIN Code.                                      
                  CurrentCBRemoteBD_ADDR = GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device;

                  // Inform the user that they will need to respond with
                  // a PIN Code Response.                               
                  hal_printf("Respond with: PINCodeResponse\r\n");
                  break;
               case atAuthenticationStatus:
                  // An authentication status event occurred, display   
                  // all relevant information.                          
                  BD_ADDRToStr(GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device, Callback_BoardStr);
                  hal_printf("\r\n");
                  hal_printf("atAuthenticationStatus: %d for %s\r\n", GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Authentication_Event_Data.Authentication_Status, Callback_BoardStr);

                  // Flag that there is no longer a current             
                  // Authentication procedure in progress.              
                  ASSIGN_BD_ADDR(CurrentCBRemoteBD_ADDR, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
                  break;
               case atLinkKeyCreation:
                  // A link key creation event occurred, first display  
                  // the remote device that caused this event.          
                  BD_ADDRToStr(GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device, Callback_BoardStr);
                  hal_printf("\r\n");
                  hal_printf("atLinkKeyCreation: %s\r\n", Callback_BoardStr);

                  // Now store the link Key in either a free location OR
                  // over the old key location.                         
                  ASSIGN_BD_ADDR(NULL_BD_ADDR, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

                  for(Index=0,Result=-1;Index<(sizeof(LinkKeyInfo)/sizeof(LinkKeyInfo_t));Index++)
                  {
                     if(COMPARE_BD_ADDR(LinkKeyInfo[Index].BD_ADDR, GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device))
                        break;
                     else
                     {
                        if((Result == (-1)) && (COMPARE_BD_ADDR(LinkKeyInfo[Index].BD_ADDR, NULL_BD_ADDR)))
                           Result = Index;
                     }
                  }

                  // If we didn't find a match, see if we found an empty
                  // location.                                          
                  if(Index == (sizeof(LinkKeyInfo)/sizeof(LinkKeyInfo_t)))
                     Index = Result;

                  // Check to see if we found a location to store the   
                  // Link Key information into.                         
                  if(Index != (-1))
                  {
                     LinkKeyInfo[Index].BD_ADDR = GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device;
                     LinkKeyInfo[Index].LinkKey = GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Authentication_Event_Data.Link_Key_Info.Link_Key;

                     hal_printf("Link Key Stored.\r\n");
                  }
                  else
                     hal_printf("Link Key array full.\r\n");
                  break;
               case atIOCapabilityRequest:
                  BD_ADDRToStr(GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device, Callback_BoardStr);
                  hal_printf("\r\n");
                  hal_printf("atIOCapabilityRequest: %s\r\n", Callback_BoardStr);

                  // Setup the Authentication Information Response      
                  // structure.                                         
                  GAP_Authentication_Information.GAP_Authentication_Type                                      = atIOCapabilities;
                  GAP_Authentication_Information.Authentication_Data_Length                                   = sizeof(GAP_IO_Capabilities_t);
                  GAP_Authentication_Information.Authentication_Data.IO_Capabilities.IO_Capability            = (GAP_IO_Capability_t)IOCapability;
                  GAP_Authentication_Information.Authentication_Data.IO_Capabilities.MITM_Protection_Required = MITMProtection;
                  GAP_Authentication_Information.Authentication_Data.IO_Capabilities.OOB_Data_Present         = OOBSupport;

                  // Submit the Authentication Response.                
                  Result = GAP_Authentication_Response(BluetoothStackID, GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device, &GAP_Authentication_Information);

                  // Check the result of the submitted command.         
                  // Check the result of the submitted command.         
                  if(!Result)
                     hal_printf("Auth\r\n");
                  else
                     hal_printf("Auth %d\r\n", Result);
                  break;
               case atIOCapabilityResponse:
                  BD_ADDRToStr(GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device, Callback_BoardStr);
                  hal_printf("\r\n");
                  hal_printf("atIOCapabilityResponse: %s\r\n", Callback_BoardStr);

                  RemoteIOCapability = GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Authentication_Event_Data.IO_Capabilities.IO_Capability;
                  MITM               = (Boolean_t)GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Authentication_Event_Data.IO_Capabilities.MITM_Protection_Required;
                  OOB_Data           = (Boolean_t)GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Authentication_Event_Data.IO_Capabilities.OOB_Data_Present;

                  //hal_printf("Capabilities: %s%s%s\r\n", IOCapabilitiesStrings[RemoteIOCapability], ((MITM)?", MITM":""), ((OOB_Data)?", OOB Data":""));
                  break;
               case atUserConfirmationRequest:
                  BD_ADDRToStr(GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device, Callback_BoardStr);
                  hal_printf("\r\n");
                  hal_printf("atUserConfirmationRequest: %s\r\n", Callback_BoardStr);

                  CurrentCBRemoteBD_ADDR = GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device;

                  if(IOCapability != icDisplayYesNo)
                  {
                     // Invoke JUST Works Process...                    
                     GAP_Authentication_Information.GAP_Authentication_Type          = atUserConfirmation;
                     GAP_Authentication_Information.Authentication_Data_Length       = (Byte_t)sizeof(Byte_t);
                     GAP_Authentication_Information.Authentication_Data.Confirmation = TRUE;

                     // Submit the Authentication Response.             
                     hal_printf("\r\nAuto Accepting: %l\r\n", GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Authentication_Event_Data.Numeric_Value);

                     Result = GAP_Authentication_Response(BluetoothStackID, GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device, &GAP_Authentication_Information);

                     if(!Result)
                        hal_printf("GAP_Authentication_Response\r\n");
                     else
                        hal_printf("GAP_Authentication_Response %d\r\n", Result);

                     // Flag that there is no longer a current          
                     // Authentication procedure in progress.           
                     ASSIGN_BD_ADDR(CurrentCBRemoteBD_ADDR, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
                  }
                  else
                  {
                     hal_printf("User Confirmation: %l\r\n", (unsigned long)GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Authentication_Event_Data.Numeric_Value);

                     // Inform the user that they will need to respond  
                     // with a PIN Code Response.                       
                     hal_printf("Respond with: UserConfirmationResponse\r\n");
                  }
                  break;
               case atPasskeyRequest:
                  BD_ADDRToStr(GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device, Callback_BoardStr);
                  hal_printf("\r\n");
                  hal_printf("atPasskeyRequest: %s\r\n", Callback_BoardStr);

                  // Note the current Remote BD_ADDR that is requesting 
                  // the Passkey.                                       
                  CurrentCBRemoteBD_ADDR = GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device;

                  // Inform the user that they will need to respond with
                  // a Passkey Response.                                
                  hal_printf("Respond with: PassKeyResponse\r\n");
                  break;
               case atRemoteOutOfBandDataRequest:
                  BD_ADDRToStr(GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device, Callback_BoardStr);
                  hal_printf("\r\n");
                  hal_printf("atRemoteOutOfBandDataRequest: %s\r\n", Callback_BoardStr);

                  // This application does not support OOB data so      
                  // respond with a data length of Zero to force a      
                  // negative reply.                                    
                  GAP_Authentication_Information.GAP_Authentication_Type    = atOutOfBandData;
                  GAP_Authentication_Information.Authentication_Data_Length = 0;

                  Result = GAP_Authentication_Response(BluetoothStackID, GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device, &GAP_Authentication_Information);

                  if(!Result)
                     hal_printf("GAP_Authentication_Response\r\n");
                  else
                     hal_printf("GAP_Authentication_Response %d\r\n", Result);
                  break;
               case atPasskeyNotification:
                  BD_ADDRToStr(GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device, Callback_BoardStr);
                  hal_printf("\r\n");
                  hal_printf("atPasskeyNotification: %s\r\n", Callback_BoardStr);

                  hal_printf("Passkey Value: %u\r\n", GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Authentication_Event_Data.Numeric_Value);
                  break;
               case atKeypressNotification:
                  BD_ADDRToStr(GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Remote_Device, Callback_BoardStr);
                  hal_printf("\r\n");
                  hal_printf("atKeypressNotification: %s\r\n", Callback_BoardStr);

                  hal_printf("Keypress: %d\r\n", (int)GAP_Event_Data->Event_Data.GAP_Authentication_Event_Data->Authentication_Event_Data.Keypress_Type);
                  break;
               default:
                  hal_printf("Un-handled Auth. Event.\r\n");
                  break;
            }
            break;
         case etRemote_Name_Result:
            // Bluetooth Stack has responded to a previously issued     
            // Remote Name Request that was issued.                     
            GAP_Remote_Name_Event_Data = GAP_Event_Data->Event_Data.GAP_Remote_Name_Event_Data;
            if(GAP_Remote_Name_Event_Data)
            {
               // Inform the user of the Result.                        
               BD_ADDRToStr(GAP_Remote_Name_Event_Data->Remote_Device, Callback_BoardStr);

               hal_printf("\r\n");
               hal_printf("BD_ADDR: %s.\r\n", Callback_BoardStr);

               if(GAP_Remote_Name_Event_Data->Remote_Name)
                  hal_printf("Name: %s.\r\n", GAP_Remote_Name_Event_Data->Remote_Name);
               else
                  hal_printf("Name: NULL.\r\n");
            }
            break;
         case etEncryption_Change_Result:
            BD_ADDRToStr(GAP_Event_Data->Event_Data.GAP_Encryption_Mode_Event_Data->Remote_Device, Callback_BoardStr);
            /*hal_printf("\r\netEncryption_Change_Result for %s, Status: 0x%02X, Mode: %s.\r\n", Callback_BoardStr,
                                                                                             GAP_Event_Data->Event_Data.GAP_Encryption_Mode_Event_Data->Encryption_Change_Status,
                                                                                             ((GAP_Event_Data->Event_Data.GAP_Encryption_Mode_Event_Data->Encryption_Mode == emDisabled)?"Disabled": "Enabled"));*/
            break;
         default:
            // An unknown/unexpected GAP event was received.            
            hal_printf("\r\nUnknown Event: %d.\r\n", GAP_Event_Data->Event_Data_Type);
            break;
      }
   }
   else
   {
      // There was an error with one or more of the input parameters.   
      hal_printf("\r\n");
      hal_printf("Null Event\r\n");
   }

   //DisplayPrompt();
}

// The following function is for the GAP LE Event Receive Data       
   // Callback.  This function will be called whenever a Callback has   
   // been registered for the specified GAP LE Action that is associated
   // with the Bluetooth Stack.  This function passes to the caller the 
   // GAP LE Event Data of the specified Event and the GAP LE Event     
   // Callback Parameter that was specified when this Callback was      
   // installed.  The caller is free to use the contents of the GAP LE  
   // Event Data ONLY in the context of this callback.  If the caller   
   // requires the Data for a longer period of time, then the callback  
   // function MUST copy the data into another Data Buffer.  This       
   // function is guaranteed NOT to be invoked more than once           
   // simultaneously for the specified installed callback (i.e.  this   
   // function DOES NOT have be reentrant).  It Needs to be noted       
   // however, that if the same Callback is installed more than once,   
   // then the callbacks will be called serially.  Because of this, the 
   // processing in this function should be as efficient as possible.   
   // It should also be noted that this function is called in the Thread
   // Context of a Thread that the User does NOT own.  Therefore,       
   // processing in this function should be as efficient as possible    
   // (this argument holds anyway because other GAP Events will not be  
   // processed while this function call is outstanding).               
   // * NOTE * This function MUST NOT Block and wait for Events that can
   //          only be satisfied by Receiving a Bluetooth Event         
   //          Callback.  A Deadlock WILL occur because NO Bluetooth    
   //          Callbacks will be issued while this function is currently
   //          outstanding.                                             
static void BTPSAPI GAP_LE_Event_Callback(unsigned int BluetoothStackID, GAP_LE_Event_Data_t *GAP_LE_Event_Data, unsigned long CallbackParameter)
{
   int                                           Result;
   int                                           LEConnectionInfo;
   BoardStr_t                                    BoardStr;
   unsigned int                                  Index;
   DeviceInfo_t                                 *DeviceInfo;
   Long_Term_Key_t                               GeneratedLTK;
   GAP_LE_Security_Information_t                 GAP_LE_Security_Information;
   GAP_LE_Connection_Parameters_t                ConnectionParameters;
   GAP_LE_Advertising_Report_Data_t             *DeviceEntryPtr;
   GAP_LE_Authentication_Event_Data_t           *Authentication_Event_Data;
   GAP_LE_Authentication_Response_Information_t  GAP_LE_Authentication_Response_Information;

   hal_printf("**** not finished GAP_LE_Event_Callback\r\n");
   // Verify that all parameters to this callback are Semi-Valid.       
   /*if((BluetoothStackID) && (GAP_LE_Event_Data))
   {
      switch(GAP_LE_Event_Data->Event_Data_Type)
      {
         case etLE_Advertising_Report:
            Display(("\r\netLE_Advertising_Report with size %d.\r\n",(int)GAP_LE_Event_Data->Event_Data_Size));
            Display(("  %d Responses.\r\n",GAP_LE_Event_Data->Event_Data.GAP_LE_Advertising_Report_Event_Data->Number_Device_Entries));

            for(Index = 0; Index < GAP_LE_Event_Data->Event_Data.GAP_LE_Advertising_Report_Event_Data->Number_Device_Entries; Index++)
            {
               DeviceEntryPtr = &(GAP_LE_Event_Data->Event_Data.GAP_LE_Advertising_Report_Event_Data->Advertising_Data[Index]);

               // Display the packet type for the device                
               switch(DeviceEntryPtr->Advertising_Report_Type)
               {
                  case rtConnectableUndirected:
                     Display(("  Advertising Type: %s.\r\n", "rtConnectableUndirected"));
                     break;
                  case rtConnectableDirected:
                     Display(("  Advertising Type: %s.\r\n", "rtConnectableDirected"));
                     break;
                  case rtScannableUndirected:
                     Display(("  Advertising Type: %s.\r\n", "rtScannableUndirected"));
                     break;
                  case rtNonConnectableUndirected:
                     Display(("  Advertising Type: %s.\r\n", "rtNonConnectableUndirected"));
                     break;
                  case rtScanResponse:
                     Display(("  Advertising Type: %s.\r\n", "rtScanResponse"));
                     break;
               }

               // Display the Address Type.                             
               if(DeviceEntryPtr->Address_Type == latPublic)
               {
                  Display(("  Address Type: %s.\r\n","atPublic"));
               }
               else
               {
                  Display(("  Address Type: %s.\r\n","atRandom"));
               }

               // Display the Device Address.                           
               Display(("  Address: 0x%02X%02X%02X%02X%02X%02X.\r\n", DeviceEntryPtr->BD_ADDR.BD_ADDR5, DeviceEntryPtr->BD_ADDR.BD_ADDR4, DeviceEntryPtr->BD_ADDR.BD_ADDR3, DeviceEntryPtr->BD_ADDR.BD_ADDR2, DeviceEntryPtr->BD_ADDR.BD_ADDR1, DeviceEntryPtr->BD_ADDR.BD_ADDR0));
               Display(("  RSSI: %d.\r\n", (int)DeviceEntryPtr->RSSI));
               Display(("  Data Length: %d.\r\n", DeviceEntryPtr->Raw_Report_Length));

               DisplayAdvertisingData(&(DeviceEntryPtr->Advertising_Data));
            }
            break;
         case etLE_Connection_Complete:
            Display(("\r\netLE_Connection_Complete with size %d.\r\n",(int)GAP_LE_Event_Data->Event_Data_Size));

            if(GAP_LE_Event_Data->Event_Data.GAP_LE_Connection_Complete_Event_Data)
            {
               BD_ADDRToStr(GAP_LE_Event_Data->Event_Data.GAP_LE_Connection_Complete_Event_Data->Peer_Address, BoardStr);

               Display(("   Status:       0x%02X.\r\n", GAP_LE_Event_Data->Event_Data.GAP_LE_Connection_Complete_Event_Data->Status));
               Display(("   Role:         %s.\r\n", (GAP_LE_Event_Data->Event_Data.GAP_LE_Connection_Complete_Event_Data->Master)?"Master":"Slave"));
               Display(("   Address Type: %s.\r\n", (GAP_LE_Event_Data->Event_Data.GAP_LE_Connection_Complete_Event_Data->Peer_Address_Type == latPublic)?"Public":"Random"));
               Display(("   BD_ADDR:      %s.\r\n", BoardStr));

               if(GAP_LE_Event_Data->Event_Data.GAP_LE_Connection_Complete_Event_Data->Status == HCI_ERROR_CODE_NO_ERROR)
               {
                  // If not already in the connection info array, add   
                  // it.                                                
                  if(FindLEIndexByAddress(GAP_LE_Event_Data->Event_Data.GAP_LE_Connection_Complete_Event_Data->Peer_Address) < 0)
                  {
                     // Find an unused position in the connection info  
                     // array.                                          
                     LEConnectionInfo = FindFreeLEIndex();
                     if(LEConnectionInfo >= 0)
                        LEContextInfo[LEConnectionInfo].ConnectionBD_ADDR = GAP_LE_Event_Data->Event_Data.GAP_LE_Connection_Complete_Event_Data->Peer_Address;
                  }

                  // Set a global flag to indicate if we are the        
                  // connection master.                                 
                  LocalDeviceIsMaster = GAP_LE_Event_Data->Event_Data.GAP_LE_Connection_Complete_Event_Data->Master;

                  // Make sure that no entry already exists.            
                  if((DeviceInfo = SearchDeviceInfoEntryByBD_ADDR(&DeviceInfoList, GAP_LE_Event_Data->Event_Data.GAP_LE_Connection_Complete_Event_Data->Peer_Address)) == NULL)
                  {
                     // No entry exists so create one.                  
                     if((DeviceInfo = CreateNewDeviceInfoEntry(&DeviceInfoList, GAP_LE_Event_Data->Event_Data.GAP_LE_Connection_Complete_Event_Data->Peer_Address_Type, GAP_LE_Event_Data->Event_Data.GAP_LE_Connection_Complete_Event_Data->Peer_Address)) != NULL)
                     {
                        // Flag that we are now connected.              
                        DeviceInfo->Flags |= DEVICE_INFO_FLAGS_CONNECTED;
                     }
                     else
                     {
                        Display(("Failed to add device to Device Info List.\r\n"));
                     }
                  }
                  else
                  {
                     // Flag that we are now connected.                 
                     DeviceInfo->Flags |= DEVICE_INFO_FLAGS_CONNECTED;

                     // If we are the Master of the connection we will  
                     // attempt to Re-Establish Security if a LTK for   
                     // this device exists (i.e.  we previously paired).
                     if(LocalDeviceIsMaster)
                     {
                        // Re-Establish Security if there is a LTK that 
                        // is stored for this device.                   
                        if(DeviceInfo->Flags & DEVICE_INFO_FLAGS_LTK_VALID)
                        {
                           // Re-Establish Security with this LTK.      
                           Display(("Attempting to Re-Establish Security.\r\n"));

                           // Attempt to re-establish security to this  
                           // device.                                   
                           GAP_LE_Security_Information.Local_Device_Is_Master                                      = TRUE;
                           BTPS_MemCopy(&(GAP_LE_Security_Information.Security_Information.Master_Information.LTK), &(DeviceInfo->LTK), LONG_TERM_KEY_SIZE);
                           BTPS_MemCopy(&(GAP_LE_Security_Information.Security_Information.Master_Information.Rand), &(DeviceInfo->Rand), RANDOM_NUMBER_DATA_SIZE);

                           GAP_LE_Security_Information.Security_Information.Master_Information.EDIV                = DeviceInfo->EDIV;
                           GAP_LE_Security_Information.Security_Information.Master_Information.Encryption_Key_Size = DeviceInfo->EncryptionKeySize;

                           Result = GAP_LE_Reestablish_Security(BluetoothStackID, DeviceInfo->ConnectionBD_ADDR, &GAP_LE_Security_Information, GAP_LE_Event_Callback, 0);
                           if(Result)
                           {
                              Display(("GAP_LE_Reestablish_Security returned %d.\r\n",Result));
                           }
                        }
                     }
                  }
               }
               else
               {
                  // Clear the Connection ID.                           
                  RemoveConnectionInfo(GAP_LE_Event_Data->Event_Data.GAP_LE_Connection_Complete_Event_Data->Peer_Address);
               }
            }
            break;
         case etLE_Disconnection_Complete:
            Display(("\r\netLE_Disconnection_Complete with size %d.\r\n", (int)GAP_LE_Event_Data->Event_Data_Size));

            if(GAP_LE_Event_Data->Event_Data.GAP_LE_Disconnection_Complete_Event_Data)
            {
               Display(("   Status: 0x%02X.\r\n", GAP_LE_Event_Data->Event_Data.GAP_LE_Disconnection_Complete_Event_Data->Status));
               Display(("   Reason: 0x%02X.\r\n", GAP_LE_Event_Data->Event_Data.GAP_LE_Disconnection_Complete_Event_Data->Reason));

               BD_ADDRToStr(GAP_LE_Event_Data->Event_Data.GAP_LE_Disconnection_Complete_Event_Data->Peer_Address, BoardStr);

               Display(("   BD_ADDR: %s.\r\n", BoardStr));

               // Check to see if the device info is present in the     
               // list.                                                 
               if((DeviceInfo = SearchDeviceInfoEntryByBD_ADDR(&DeviceInfoList, GAP_LE_Event_Data->Event_Data.GAP_LE_Disconnection_Complete_Event_Data->Peer_Address)) != NULL)
               {
                  // Flag that we are no longer connected.              
                  DeviceInfo->Flags &= ~DEVICE_INFO_FLAGS_CONNECTED;

                  // Flag that no service discovery operation is        
                  // outstanding for this device.                       
                  DeviceInfo->Flags &= ~DEVICE_INFO_FLAGS_SERVICE_DISCOVERY_OUTSTANDING;

                  // Clear the CCCDs stored for this device.            
                  DeviceInfo->ServerInfo.Rx_Credit_Client_Configuration_Descriptor = 0;
                  DeviceInfo->ServerInfo.Tx_Client_Configuration_Descriptor        = 0;

                  // If this device is not paired, then delete it.  The 
                  // link will be encrypted if the device is paired.    
                  if(!(DeviceInfo->Flags & DEVICE_INFO_FLAGS_LINK_ENCRYPTED))
                  {
                     if((DeviceInfo = DeleteDeviceInfoEntry(&DeviceInfoList, GAP_LE_Event_Data->Event_Data.GAP_LE_Disconnection_Complete_Event_Data->Peer_Address)) != NULL)
                        FreeDeviceInfoEntryMemory(DeviceInfo);
                  }
                  else
                  {
                     // Flag that the Link is no longer encrypted since 
                     // we have disconnected.                           
                     DeviceInfo->Flags &= ~DEVICE_INFO_FLAGS_LINK_ENCRYPTED;
                  }
               }
            }
            break;
         case etLE_Connection_Parameter_Update_Request:
            Display(("\r\netLE_Connection_Parameter_Update_Request with size %d.\r\n", (int)GAP_LE_Event_Data->Event_Data_Size));

            if(GAP_LE_Event_Data->Event_Data.GAP_LE_Connection_Parameter_Update_Request_Event_Data)
            {
               BD_ADDRToStr(GAP_LE_Event_Data->Event_Data.GAP_LE_Connection_Parameter_Update_Request_Event_Data->BD_ADDR, BoardStr);
               Display(("   BD_ADDR:             %s.\r\n", BoardStr));
               Display(("   Minimum Interval:    %u.\r\n", (unsigned int)GAP_LE_Event_Data->Event_Data.GAP_LE_Connection_Parameter_Update_Request_Event_Data->Conn_Interval_Min));
               Display(("   Maximum Interval:    %u.\r\n", (unsigned int)GAP_LE_Event_Data->Event_Data.GAP_LE_Connection_Parameter_Update_Request_Event_Data->Conn_Interval_Max));
               Display(("   Slave Latency:       %u.\r\n", (unsigned int)GAP_LE_Event_Data->Event_Data.GAP_LE_Connection_Parameter_Update_Request_Event_Data->Slave_Latency));
               Display(("   Supervision Timeout: %u.\r\n", (unsigned int)GAP_LE_Event_Data->Event_Data.GAP_LE_Connection_Parameter_Update_Request_Event_Data->Conn_Supervision_Timeout));

               // Initialize the connection parameters.                 
               ConnectionParameters.Connection_Interval_Min    = GAP_LE_Event_Data->Event_Data.GAP_LE_Connection_Parameter_Update_Request_Event_Data->Conn_Interval_Min;
               ConnectionParameters.Connection_Interval_Max    = GAP_LE_Event_Data->Event_Data.GAP_LE_Connection_Parameter_Update_Request_Event_Data->Conn_Interval_Max;
               ConnectionParameters.Slave_Latency              = GAP_LE_Event_Data->Event_Data.GAP_LE_Connection_Parameter_Update_Request_Event_Data->Slave_Latency;
               ConnectionParameters.Supervision_Timeout        = GAP_LE_Event_Data->Event_Data.GAP_LE_Connection_Parameter_Update_Request_Event_Data->Conn_Supervision_Timeout;
               ConnectionParameters.Minimum_Connection_Length  = 0;
               ConnectionParameters.Maximum_Connection_Length  = 10000;

               Display(("\r\nAttempting to accept connection parameter update request.\r\n"));

               // Go ahead and accept whatever the slave has requested. 
               Result = GAP_LE_Connection_Parameter_Update_Response(BluetoothStackID, GAP_LE_Event_Data->Event_Data.GAP_LE_Connection_Parameter_Update_Request_Event_Data->BD_ADDR, TRUE, &ConnectionParameters);
               if(!Result)
               {
                  Display(("      GAP_LE_Connection_Parameter_Update_Response() success.\r\n"));
               }
               else
               {
                  Display(("      GAP_LE_Connection_Parameter_Update_Response() error %d.\r\n", Result));
               }
            }
            break;
         case etLE_Connection_Parameter_Updated:
            Display(("\r\netLE_Connection_Parameter_Updated with size %d.\r\n", (int)GAP_LE_Event_Data->Event_Data_Size));

            if(GAP_LE_Event_Data->Event_Data.GAP_LE_Connection_Parameter_Updated_Event_Data)
            {
               BD_ADDRToStr(GAP_LE_Event_Data->Event_Data.GAP_LE_Connection_Parameter_Updated_Event_Data->BD_ADDR, BoardStr);
               Display(("   Status:              0x%02X.\r\n", GAP_LE_Event_Data->Event_Data.GAP_LE_Connection_Parameter_Updated_Event_Data->Status));
               Display(("   BD_ADDR:             %s.\r\n", BoardStr));

               if(GAP_LE_Event_Data->Event_Data.GAP_LE_Connection_Parameter_Updated_Event_Data->Status == HCI_ERROR_CODE_NO_ERROR)
               {
                  Display(("   Connection Interval: %u.\r\n", (unsigned int)GAP_LE_Event_Data->Event_Data.GAP_LE_Connection_Parameter_Updated_Event_Data->Current_Connection_Parameters.Connection_Interval));
                  Display(("   Slave Latency:       %u.\r\n", (unsigned int)GAP_LE_Event_Data->Event_Data.GAP_LE_Connection_Parameter_Updated_Event_Data->Current_Connection_Parameters.Slave_Latency));
                  Display(("   Supervision Timeout: %u.\r\n", (unsigned int)GAP_LE_Event_Data->Event_Data.GAP_LE_Connection_Parameter_Updated_Event_Data->Current_Connection_Parameters.Supervision_Timeout));
               }
            }
            break;
         case etLE_Encryption_Change:
            Display(("\r\netLE_Encryption_Change with size %d.\r\n",(int)GAP_LE_Event_Data->Event_Data_Size));

            // Search for the device entry to see flag if the link is   
            // encrypted.                                               
            if((DeviceInfo = SearchDeviceInfoEntryByBD_ADDR(&DeviceInfoList, GAP_LE_Event_Data->Event_Data.GAP_LE_Encryption_Change_Event_Data->BD_ADDR)) != NULL)
            {
               // Check to see if the encryption change was successful. 
               if((GAP_LE_Event_Data->Event_Data.GAP_LE_Encryption_Change_Event_Data->Encryption_Change_Status == HCI_ERROR_CODE_NO_ERROR) && (GAP_LE_Event_Data->Event_Data.GAP_LE_Encryption_Change_Event_Data->Encryption_Mode == emEnabled))
                  DeviceInfo->Flags |= DEVICE_INFO_FLAGS_LINK_ENCRYPTED;
               else
                  DeviceInfo->Flags &= ~DEVICE_INFO_FLAGS_LINK_ENCRYPTED;
            }
            break;
         case etLE_Encryption_Refresh_Complete:
            Display(("\r\netLE_Encryption_Refresh_Complete with size %d.\r\n", (int)GAP_LE_Event_Data->Event_Data_Size));

            // Search for the device entry to see flag if the link is   
            // encrypted.                                               
            if((DeviceInfo = SearchDeviceInfoEntryByBD_ADDR(&DeviceInfoList, GAP_LE_Event_Data->Event_Data.GAP_LE_Encryption_Refresh_Complete_Event_Data->BD_ADDR)) != NULL)
            {
               // Check to see if the refresh was successful.           
               if(GAP_LE_Event_Data->Event_Data.GAP_LE_Encryption_Refresh_Complete_Event_Data->Status == HCI_ERROR_CODE_NO_ERROR)
                  DeviceInfo->Flags |= DEVICE_INFO_FLAGS_LINK_ENCRYPTED;
               else
                  DeviceInfo->Flags &= ~DEVICE_INFO_FLAGS_LINK_ENCRYPTED;
            }
            break;
         case etLE_Authentication:
            Display(("\r\netLE_Authentication with size %d.\r\n", (int)GAP_LE_Event_Data->Event_Data_Size));

            // Make sure the authentication event data is valid before  
            // continuing.                                              
            if((Authentication_Event_Data = GAP_LE_Event_Data->Event_Data.GAP_LE_Authentication_Event_Data) != NULL)
            {
               BD_ADDRToStr(Authentication_Event_Data->BD_ADDR, BoardStr);

               switch(Authentication_Event_Data->GAP_LE_Authentication_Event_Type)
               {
                  case latLongTermKeyRequest:
                     Display(("    latKeyRequest: \r\n"));
                     Display(("      BD_ADDR: %s.\r\n", BoardStr));

                     // The other side of a connection is requesting    
                     // that we start encryption. Thus we should        
                     // regenerate LTK for this connection and send it  
                     // to the chip.                                    
                     Result = GAP_LE_Regenerate_Long_Term_Key(BluetoothStackID, (Encryption_Key_t *)(&DHK), (Encryption_Key_t *)(&ER), Authentication_Event_Data->Authentication_Event_Data.Long_Term_Key_Request.EDIV, &(Authentication_Event_Data->Authentication_Event_Data.Long_Term_Key_Request.Rand), &GeneratedLTK);
                     if(!Result)
                     {
                        Display(("      GAP_LE_Regenerate_Long_Term_Key Success.\r\n"));

                        // Respond with the Re-Generated Long Term Key. 
                        GAP_LE_Authentication_Response_Information.GAP_LE_Authentication_Type                                        = larLongTermKey;
                        GAP_LE_Authentication_Response_Information.Authentication_Data_Length                                        = GAP_LE_LONG_TERM_KEY_INFORMATION_DATA_SIZE;
                        GAP_LE_Authentication_Response_Information.Authentication_Data.Long_Term_Key_Information.Encryption_Key_Size = GAP_LE_MAXIMUM_ENCRYPTION_KEY_SIZE;

                        BTPS_MemCopy(&(GAP_LE_Authentication_Response_Information.Authentication_Data.Long_Term_Key_Information.Long_Term_Key), &GeneratedLTK, LONG_TERM_KEY_SIZE);
                     }
                     else
                     {
                        Display(("      GAP_LE_Regenerate_Long_Term_Key returned %d.\r\n",Result));

                        // Since we failed to generate the requested key
                        // we should respond with a negative response.  
                        GAP_LE_Authentication_Response_Information.GAP_LE_Authentication_Type = larLongTermKey;
                        GAP_LE_Authentication_Response_Information.Authentication_Data_Length = 0;
                     }

                     // Send the Authentication Response.               
                     Result = GAP_LE_Authentication_Response(BluetoothStackID, Authentication_Event_Data->BD_ADDR, &GAP_LE_Authentication_Response_Information);
                     if(Result)
                     {
                        Display(("      GAP_LE_Authentication_Response returned %d.\r\n",Result));
                     }
                     break;
                  case latSecurityRequest:
                     // Display the data for this event.                
                     // * NOTE * This is only sent from Slave to Master.
                     //          Thus we must be the Master in this     
                     //          connection.                            
                     Display(("    latSecurityRequest:.\r\n"));
                     Display(("      BD_ADDR: %s.\r\n", BoardStr));
                     Display(("      Bonding Type: %s.\r\n", ((Authentication_Event_Data->Authentication_Event_Data.Security_Request.Bonding_Type == lbtBonding)?"Bonding":"No Bonding")));
                     Display(("      MITM: %s.\r\n", ((Authentication_Event_Data->Authentication_Event_Data.Security_Request.MITM == TRUE)?"YES":"NO")));

                     // Determine if we have previously paired with the 
                     // device. If we have paired we will attempt to    
                     // re-establish security using a previously        
                     // exchanged LTK.                                  
                     if((DeviceInfo = SearchDeviceInfoEntryByBD_ADDR(&DeviceInfoList, Authentication_Event_Data->BD_ADDR)) != NULL)
                     {
                        // Determine if a Valid Long Term Key is stored 
                        // for this device.                             
                        if(DeviceInfo->Flags & DEVICE_INFO_FLAGS_LTK_VALID)
                        {
                           Display(("Attempting to Re-Establish Security.\r\n"));

                           // Attempt to re-establish security to this  
                           // device.                                   
                           GAP_LE_Security_Information.Local_Device_Is_Master                                      = TRUE;

                           BTPS_MemCopy(&(GAP_LE_Security_Information.Security_Information.Master_Information.LTK), &(DeviceInfo->LTK), LONG_TERM_KEY_SIZE);
                           BTPS_MemCopy(&(GAP_LE_Security_Information.Security_Information.Master_Information.Rand), &(DeviceInfo->Rand), RANDOM_NUMBER_DATA_SIZE);

                           GAP_LE_Security_Information.Security_Information.Master_Information.EDIV                = DeviceInfo->EDIV;
                           GAP_LE_Security_Information.Security_Information.Master_Information.Encryption_Key_Size = DeviceInfo->EncryptionKeySize;

                           Result = GAP_LE_Reestablish_Security(BluetoothStackID, Authentication_Event_Data->BD_ADDR, &GAP_LE_Security_Information, GAP_LE_Event_Callback, 0);
                           if(Result)
                           {
                              Display(("GAP_LE_Reestablish_Security returned %d.\r\n",Result));
                           }
                        }
                        else
                        {
                           CurrentLERemoteBD_ADDR = Authentication_Event_Data->BD_ADDR;

                           // We do not have a stored Link Key for this 
                           // device so go ahead and pair to this       
                           // device.                                   
                           SendPairingRequest(Authentication_Event_Data->BD_ADDR, TRUE);
                        }
                     }
                     else
                     {
                        CurrentLERemoteBD_ADDR = Authentication_Event_Data->BD_ADDR;

                        // There is no Key Info Entry for this device   
                        // so we will just treat this as a slave        
                        // request and initiate pairing.                
                        SendPairingRequest(Authentication_Event_Data->BD_ADDR, TRUE);
                     }

                     break;
                  case latPairingRequest:
                     CurrentLERemoteBD_ADDR = Authentication_Event_Data->BD_ADDR;

                     Display(("Pairing Request: %s.\r\n",BoardStr));
                     DisplayPairingInformation(Authentication_Event_Data->Authentication_Event_Data.Pairing_Request);

                     // This is a pairing request. Respond with a       
                     // Pairing Response.                               
                     // * NOTE * This is only sent from Master to Slave.
                     //          Thus we must be the Slave in this      
                     //          connection.                            

                     // Send the Pairing Response.                      
                     SlavePairingRequestResponse(Authentication_Event_Data->BD_ADDR);
                     break;
                  case latConfirmationRequest:
                     Display(("latConfirmationRequest.\r\n"));

                     if(Authentication_Event_Data->Authentication_Event_Data.Confirmation_Request.Request_Type == crtNone)
                     {
                        Display(("Invoking Just Works.\r\n"));

                        // Just Accept Just Works Pairing.              
                        GAP_LE_Authentication_Response_Information.GAP_LE_Authentication_Type = larConfirmation;

                        // By setting the Authentication_Data_Length to 
                        // any NON-ZERO value we are informing the GAP  
                        // LE Layer that we are accepting Just Works    
                        // Pairing.                                     
                        GAP_LE_Authentication_Response_Information.Authentication_Data_Length = DWORD_SIZE;

                        Result = GAP_LE_Authentication_Response(BluetoothStackID, Authentication_Event_Data->BD_ADDR, &GAP_LE_Authentication_Response_Information);
                        if(Result)
                        {
                           Display(("GAP_LE_Authentication_Response returned %d.\r\n",Result));
                        }
                     }
                     else
                     {
                        if(Authentication_Event_Data->Authentication_Event_Data.Confirmation_Request.Request_Type == crtPasskey)
                        {
                           Display(("Call LEPasskeyResponse [PASSCODE].\r\n"));
                        }
                        else
                        {
                           if(Authentication_Event_Data->Authentication_Event_Data.Confirmation_Request.Request_Type == crtDisplay)
                           {
                              Display(("Passkey: %06ld.\r\n", Authentication_Event_Data->Authentication_Event_Data.Confirmation_Request.Display_Passkey));
                           }
                        }
                     }
                     break;
                  case latSecurityEstablishmentComplete:
                     Display(("Security Re-Establishment Complete: %s.\r\n", BoardStr));
                     Display(("                            Status: 0x%02X.\r\n", Authentication_Event_Data->Authentication_Event_Data.Security_Establishment_Complete.Status));
                     break;
                  case latPairingStatus:
                     ASSIGN_BD_ADDR(CurrentLERemoteBD_ADDR, 0, 0, 0, 0, 0, 0);

                     Display(("Pairing Status: %s.\r\n", BoardStr));
                     Display(("        Status: 0x%02X.\r\n", Authentication_Event_Data->Authentication_Event_Data.Pairing_Status.Status));

                     if(Authentication_Event_Data->Authentication_Event_Data.Pairing_Status.Status == GAP_LE_PAIRING_STATUS_NO_ERROR)
                     {
                        Display(("        Key Size: %d.\r\n", Authentication_Event_Data->Authentication_Event_Data.Pairing_Status.Negotiated_Encryption_Key_Size));
                     }
                     else
                     {
                        // Failed to pair so delete the key entry for   
                        // this device and disconnect the link.         
                        if((DeviceInfo = DeleteDeviceInfoEntry(&DeviceInfoList, Authentication_Event_Data->BD_ADDR)) != NULL)
                           FreeDeviceInfoEntryMemory(DeviceInfo);

                        // Disconnect the Link.                         
                        GAP_LE_Disconnect(BluetoothStackID, Authentication_Event_Data->BD_ADDR);
                     }
                     break;
                  case latEncryptionInformationRequest:
                     Display(("Encryption Information Request %s.\r\n", BoardStr));

                     // Generate new LTK,EDIV and Rand and respond with 
                     // them.                                           
                     EncryptionInformationRequestResponse(Authentication_Event_Data->BD_ADDR, Authentication_Event_Data->Authentication_Event_Data.Encryption_Request_Information.Encryption_Key_Size, &GAP_LE_Authentication_Response_Information);
                     break;
                  case latEncryptionInformation:
                     // Display the information from the event.         
                     Display((" Encryption Information from RemoteDevice: %s.\r\n", BoardStr));
                     Display(("                             Key Size: %d.\r\n", Authentication_Event_Data->Authentication_Event_Data.Encryption_Information.Encryption_Key_Size));

                     // ** NOTE ** If we are the Slave we will NOT      
                     //            store the LTK that is sent to us by  
                     //            the Master.  However if it was ever  
                     //            desired that the Master and Slave    
                     //            switch roles in a later connection   
                     //            we could store that information at   
                     //            this point.                          
                     if(LocalDeviceIsMaster)
                     {
                        // Search for the entry for this slave to store 
                        // the information into.                        
                        if((DeviceInfo = SearchDeviceInfoEntryByBD_ADDR(&DeviceInfoList, Authentication_Event_Data->BD_ADDR)) != NULL)
                        {
                           BTPS_MemCopy(&(DeviceInfo->LTK), &(Authentication_Event_Data->Authentication_Event_Data.Encryption_Information.LTK), LONG_TERM_KEY_SIZE);
                           BTPS_MemCopy(&(DeviceInfo->Rand), &(Authentication_Event_Data->Authentication_Event_Data.Encryption_Information.Rand), RANDOM_NUMBER_DATA_SIZE);

                           DeviceInfo->EDIV              = Authentication_Event_Data->Authentication_Event_Data.Encryption_Information.EDIV;
                           DeviceInfo->EncryptionKeySize = Authentication_Event_Data->Authentication_Event_Data.Encryption_Information.Encryption_Key_Size;
                           DeviceInfo->Flags            |= DEVICE_INFO_FLAGS_LTK_VALID;
                        }
                        else
                        {
                           Display(("No Key Info Entry for this Slave.\r\n"));
                        }
                     }
                     break;
               }
            }
            break;
      }

      // Display the command prompt.                                    
      DisplayPrompt();
   }*/
}

   // The following function deletes (and frees all memory) every       
   // element of the specified Key Info List.  Upon return of this      
   // function, the Head Pointer is set to NULL.                        
static void FreeDeviceInfoList(DeviceInfo_t **ListHead)
{
   BSC_FreeGenericListEntryList((void **)(ListHead), BTPS_STRUCTURE_OFFSET(DeviceInfo_t, NextDeviceInfoInfoPtr));
}


  // The following function is responsible for closing the SS1         
   // Bluetooth Protocol Stack.  This function requires that the        
   // Bluetooth Protocol stack previously have been initialized via the 
   // OpenStack() function.  This function returns zero on successful   
   // execution and a negative value on all errors.                     
static int CloseStack(void)
{
   int ret_val = 0;

   // First check to see if the Stack has been opened.                  
   if(BluetoothStackID)
   {
      // Cleanup GAP Service Module.                                    
      if(GAPSInstanceID)
         GAPS_Cleanup_Service(BluetoothStackID, GAPSInstanceID);

      // Un-registered SPP LE Service.                                  
      if(SPPLEServiceID)
         GATT_Un_Register_Service(BluetoothStackID, SPPLEServiceID);

      // Cleanup GATT Module.                                           
      GATT_Cleanup(BluetoothStackID);

      // Simply close the Stack                                         
      BSC_Shutdown(BluetoothStackID);

      // Free BTPSKRNL allocated memory.                                
      BTPS_DeInit();

      hal_printf("Stack Shutdown.\r\n");

      // Free the Key List.                                             
      FreeDeviceInfoList(&DeviceInfoList);

      // Flag that the Stack is no longer initialized.                  
      BluetoothStackID = 0;

      // Flag success to the caller.                                    
      ret_val          = 0;
   }
   else
   {
      // A valid Stack ID does not exist, inform to user.               
      ret_val = UNABLE_TO_INITIALIZE_STACK;
   }

   return(ret_val);
}


   // The following function is responsible for placing the Local       
   // Bluetooth Device into Connectable Mode.  Once in this mode the    
   // Device will respond to Page Scans from other Bluetooth Devices.   
   // This function requires that a valid Bluetooth Stack ID exists     
   // before running.  This function returns zero on success and a      
   // negative value if an error occurred.                              
static int SetConnect(void)
{
   int ret_val = 0;

   // First, check that a valid Bluetooth Stack ID exists.              
   if(BluetoothStackID)
   {
      // Attempt to set the attached Device to be Connectable.          
      ret_val = GAP_Set_Connectability_Mode(BluetoothStackID, cmConnectableMode);

      // Next, check the return value of the                            
      // GAP_Set_Connectability_Mode() function for successful          
      // execution.                                                     
      if(!ret_val)
      {
         // * NOTE * Connectability is only an applicable when          
         //          advertising so we will just save the default       
         //          connectability for the next time we enable         
         //          advertising.                                       
         LE_Parameters.ConnectableMode = lcmConnectable;
      }
      else
      {
         // An error occurred while trying to make the Device           
         // Connectable.                                                
         hal_printf("Bluetooth Set Connectability Mode error %d\r\n", ret_val);
      }
   }
   else
   {
      // No valid Bluetooth Stack ID exists.                            
      ret_val = INVALID_STACK_ID_ERROR;
   }

   return(ret_val);
}

   // The following function is responsible for placing the Local       
   // Bluetooth Device into General Discoverability Mode.  Once in this 
   // mode the Device will respond to Inquiry Scans from other Bluetooth
   // Devices.  This function requires that a valid Bluetooth Stack ID  
   // exists before running.  This function returns zero on successful  
   // execution and a negative value if an error occurred.              
static int SetDisc(void)
{
   int ret_val = 0;

   // First, check that a valid Bluetooth Stack ID exists.              
   if(BluetoothStackID)
   {
      // A semi-valid Bluetooth Stack ID exists, now attempt to set the 
      // attached Devices Discoverability Mode to General.              
      ret_val = GAP_Set_Discoverability_Mode(BluetoothStackID, dmGeneralDiscoverableMode, 0);

      // Next, check the return value of the GAP Set Discoverability    
      // Mode command for successful execution.                         
      if(!ret_val)
      {
         // * NOTE * Discoverability is only applicable when we are     
         //          advertising so save the default Discoverability    
         //          Mode for later.                                    
         LE_Parameters.DiscoverabilityMode = dmGeneralDiscoverableMode;
      }
      else
      {
         // An error occurred while trying to set the Discoverability   
         // Mode of the Device.                                         
         hal_printf("Set Discoverable Mode\r\n", ret_val);
      }
   }
   else
   {
      // No valid Bluetooth Stack ID exists.                            
      ret_val = INVALID_STACK_ID_ERROR;
   }

   return(ret_val);
}


  // The following function is responsible for placing the local       
   // Bluetooth device into Pairable mode.  Once in this mode the device
   // will response to pairing requests from other Bluetooth devices.   
   // This function returns zero on successful execution and a negative 
   // value on all errors.                                              
static int SetPairable(void)
{
   int Result;
   int ret_val = 0;

   // First, check that a valid Bluetooth Stack ID exists.              
   if(BluetoothStackID)
   {
      // Attempt to set the attached device to be pairable.             
      Result = GAP_Set_Pairability_Mode(BluetoothStackID, pmPairableMode);

      // Next, check the return value of the GAP Set Pairability mode   
      // command for successful execution.                              
      if(!Result)
      {
         // The device has been set to pairable mode, now register an   
         // Authentication Callback to handle the Authentication events 
         // if required.                                                
         Result = GAP_Register_Remote_Authentication(BluetoothStackID, GAP_Event_Callback, (unsigned long)0);

         // Next, check the return value of the GAP Register Remote     
         // Authentication command for successful execution.            
         if(!Result)
         {
            // Now Set the LE Pairability.                              

            // Attempt to set the attached device to be pairable.       
            Result = GAP_LE_Set_Pairability_Mode(BluetoothStackID, lpmPairableMode);

            // Next, check the return value of the GAP Set Pairability  
            // mode command for successful execution.                   
            if(!Result)
            {
               // The device has been set to pairable mode, now register
               // an Authentication Callback to handle the              
               // Authentication events if required.                    
               Result = GAP_LE_Register_Remote_Authentication(BluetoothStackID, GAP_LE_Event_Callback, (unsigned long)0);

               // Next, check the return value of the GAP Register      
               // Remote Authentication command for successful          
               // execution.                                            
               if(Result)
               {
                  // An error occurred while trying to execute this     
                  // function.                                          
                  hal_printf("GAP_LE_Register_Remote_Authentication %d\r\n", Result);

                  ret_val = Result;
               }
            }
            else
            {
               // An error occurred while trying to make the device     
               // pairable.                                             
               hal_printf("GAP_LE_Set_Pairability_Mode %d\r\n", Result);

               ret_val = Result;
            }
         }
         else
         {
            // An error occurred while trying to execute this function. 
            hal_printf("GAP_Register_Remote_Authentication %d\r\n", Result);

            ret_val = Result;
         }
      }
      else
      {
         // An error occurred while trying to make the device pairable. 
         hal_printf("GAP_Set_Pairability_Mode %d\r\n", Result);

         ret_val = Result;
      }
   }
   else
   {
      // No valid Bluetooth Stack ID exists.                            
      ret_val = INVALID_STACK_ID_ERROR;
   }

   return(ret_val);
}



int InitializeApplication(HCI_DriverInformation_t *HCI_DriverInformation, BTPS_Initialization_t *BTPS_Initialization)
{
   int ret_val = APPLICATION_ERROR_UNABLE_TO_OPEN_STACK;

   // Next, makes sure that the Driver Information passed appears to be 
   // semi-valid.                                                       
   if((HCI_DriverInformation) && (BTPS_Initialization))
   {
      // Try to Open the stack and check if it was successful.          
      if(!OpenStack(HCI_DriverInformation, BTPS_Initialization))
      {
         // First, attempt to set the Device to be Connectable.         
         ret_val = SetConnect();
		 hal_printf("SetConnect ret: %d\r\n", ret_val);
         // Next, check to see if the Device was successfully made      
         // Connectable.                                                
         if(!ret_val)
         {
            // Now that the device is Connectable attempt to make it    
            // Discoverable.                                            
            ret_val = SetDisc();

            // Next, check to see if the Device was successfully made   
            // Discoverable.                                            
            if(!ret_val)
            {
               // Now that the device is discoverable attempt to make it
               // pairable.                                             
               ret_val = SetPairable();
               if(!ret_val)
               {
                  // Attempt to register a HCI Event Callback.          
                  ret_val = HCI_Register_Event_Callback(BluetoothStackID, HCI_Event_Callback, (unsigned long)NULL);
                  if(ret_val > 0)
                  {
                     // Restart the User Interface Selection.           
                     //UI_Mode = UI_MODE_SELECT;

                     // Save the maximum supported baud rate.           
                     //MaxBaudRate = (DWord_t)(HCI_DriverInformation->DriverInformation.COMMDriverInformation.BaudRate);

                     // Return success to the caller.                   
                     ret_val = (int)BluetoothStackID;
                  }
               }
               else
                  hal_printf("BT: SetPairable %d\r\n", ret_val);
            }
            else
               hal_printf("BT: SetDisc %d\r\n", ret_val);
         }
         else
            hal_printf("BT: SetDisc %d\r\n", ret_val);

         // In some error occurred then close the stack.                
         if(ret_val < 0)
         {
            // Close the Bluetooth Stack.                               
            CloseStack();
         }
      }
      else
      {
         // There was an error while attempting to open the Stack.      
         hal_printf("Unable to open the stack.\r\n");
      }
   }
   else
      ret_val = APPLICATION_ERROR_INVALID_PARAMETERS;

   return(ret_val);
}


DeviceStatus CPU_Bluetooth_Initialize( )
{
	DeviceStatus status = DS_Fail;
	int Result;

	hal_printf("init Bluetooth\r\n");
	// configure hardware
	
	// Configure the nSHUT pin, drive it low to put the radio into reset.
	CPU_GPIO_EnableOutputPin(5, FALSE);
   	//GPIO_setOutputLowOnPin(HRDWCFG_NSHUTD_PORT_NUM, HRDWCFG_NSHUTD_PIN_NUM);
   	//GPIO_setAsOutputPin(HRDWCFG_NSHUTD_PORT_NUM, HRDWCFG_NSHUTD_PIN_NUM);	
	CPU_GPIO_SetPinState(5,TRUE);
	CPU_GPIO_SetPinState(5,FALSE);

	//USART_Initialize(1, 115200, USART_PARITY_NONE, 8, USART_STOP_BITS_ONE, USART_FLOW_NONE );
	CPU_USART_Initialize(1, 115200, USART_PARITY_NONE, 8, USART_STOP_BITS_ONE, USART_FLOW_NONE );

	// Flag that sleep is not currently enabled.                         
   	SleepAllowed = FALSE;

   	// Configure the UART Parameters.                                    
   	HCI_DRIVER_SET_COMM_INFORMATION(&HCI_DriverInformation, 1, HAL_HCI_UART_MAX_BAUD_RATE, cpHCILL_RTS_CTS);	

	// Set up the application callbacks.                                 
   	//BTPS_Initialization.GetTickCountCallback  = HAL_Time_CurrentTicks;
   	BTPS_Initialization.MessageOutputCallback = Bluetooth_MessageOutputCallback;

	// Initialize the application.                                       
   	if((Result = InitializeApplication(&HCI_DriverInformation, &BTPS_Initialization)) > 0)
   	{
      // Save the Bluetooth Stack ID.                                   
      BluetoothStackID = (unsigned int)Result;

      // Register a sleep mode callback if we are using HCILL Mode.     
      /*if((HCI_DriverInformation.DriverInformation.COMMDriverInformation.Protocol == cpHCILL) || (HCI_DriverInformation.DriverInformation.COMMDriverInformation.Protocol == cpHCILL_RTS_CTS))
      {
         HCILLConfig.SleepCallbackFunction        = HCI_Sleep_Callback;
         HCILLConfig.SleepCallbackParameter       = 0;
         DriverReconfigureData.ReconfigureCommand = HCI_COMM_DRIVER_RECONFIGURE_DATA_COMMAND_CHANGE_HCILL_PARAMETERS;
         DriverReconfigureData.ReconfigureData    = (void *)&HCILLConfig;

         // Register the sleep mode callback.  Note that if this      
         // function returns greater than 0 then sleep is currently     
         // enabled.                                                    
         Result = HCI_Reconfigure_Driver(BluetoothStackID, FALSE, &DriverReconfigureData);
         if(Result > 0)
         {
            hal_printf(("Sleep is allowed.\r\n"));

            // Flag that it is safe to go into sleep mode.              
            SleepAllowed = TRUE;
         }
      }

      // We need to execute Add a function to process the command line  
      // to the BTPS Scheduler.                                         
      if(BTPS_AddFunctionToScheduler(ProcessCharactersTask, NULL, 100))
      {
         // Add the idle task (which determines if LPM3 may be entered) 
         // to the scheduler.                                           
         if(BTPS_AddFunctionToScheduler(IdleTask, NULL, 100))
         {
            if(BTPS_AddFunctionToScheduler(ToggleLEDTask, NULL, 750))
            {
               HAL_SetLEDColor(hlcGreen);

               // Execute the scheduler, note that this function does   
               // not return.            
				                               
//put in main scheduler loop			   
               BTPS_ExecuteScheduler();
            }
         }
      }*/
   }
   
	return status;
}


BOOL CPU_Bluetooth_Reset()
{
	BOOL result = FALSE;

	
	return result;
}


BOOL CPU_Bluetooth_UnInitialize()
{
	BOOL result = FALSE;

	
	return result;
}



