#include <tinyhal.h>
extern "C" {
#include "include\SS1BTPS.h"
#include "hcitrans\HCITRANS.h"
#include "btpskrnl\BTPSKRNL.h"
#include "include\HCICommT.h"
#include "include\HCIAPI.h"
#include "btpsvend\bvendapi.h"
#include "hcitrans\HCITRANS.h"
};

#define HAL_HCI_UART_MAX_BAUD_RATE      2000000
#define APPLICATION_ERROR_INVALID_PARAMETERS       (-1000)
#define APPLICATION_ERROR_UNABLE_TO_OPEN_STACK     (-1001)

#define INVALID_PARAMETERS_ERROR                   (-6)

static unsigned int BluetoothStackID;
static unsigned int InputIndex;
static Boolean_t SleepAllowed;
static BTPS_Initialization_t         BTPS_Initialization;
static HCI_DriverInformation_t       HCI_DriverInformation;
static HCI_HCILLConfiguration_t      HCILLConfig;
static HCI_Driver_Reconfigure_Data_t DriverReconfigureData;

int Bluetooth_MessageOutputCallback(int length, char* Message){
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

   // First check to see if the Stack has already been opened.          
   if(!BluetoothStackID)
   {
      // Next, makes sure that the Driver Information passed appears to 
      // be semi-valid.                                                 
      if(HCI_DriverInformation)
      {
		hal_printf("uncomment ****** BTPS_Init\r\n");
         // Initialize BTPSKNRL.                                        
//BTPS_Init((void *)BTPS_Initialization);

         hal_printf(("\r\nOpenStack().\r\n"));

		 // these two force compile functions allow their files to be linked in properly, otherwise the linker can't find functions the bluetooth library needs
		 HCI_ForceCompile(HCI_DriverInformation);
		 BTPSAPI_ForceCompile(HCI_DriverInformation);
         // Initialize the Stack                                        
         Result = BSC_Initialize(HCI_DriverInformation, 0);
/*
         // Next, check the return value of the initialization to see   
         // if it was successful.                                       
         if(Result > 0)
         {
            // The Stack was initialized successfully, inform the user  
            // and set the return value of the initialization function  
            // to the Bluetooth Stack ID.                               
            BluetoothStackID = Result;
            hal_printf(("Bluetooth Stack ID: %d\r\n", BluetoothStackID));

            ret_val          = 0;

            // Attempt to enable the WBS feature.                       
            Result = BSC_EnableFeature(BluetoothStackID, BSC_FEATURE_BLUETOOTH_LOW_ENERGY);
            if(!Result)
            {
               hal_printf(("LOW ENERGY Support initialized.\r\n"));
            }
            else
            {
               hal_printf(("LOW ENERGY Support not initialized %d.\r\n", Result));
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
               hal_printf(("Device Chipset: %s\r\n", (HCIVersion <= NUM_SUPPORTED_HCI_VERSIONS)?HCIVersionStrings[HCIVersion]:HCIVersionStrings[NUM_SUPPORTED_HCI_VERSIONS]));

            // Printing the BTPS version                                
            hal_printf(("BTPS Version  : %s \r\n", BTPS_VERSION_VERSION_STRING));
            // Printing the FW version                                  
            DisplayFWVersion();

            // Printing the Demo Application name and version           
            hal_printf(("App Name      : %s \r\n", LE_APP_DEMO_NAME));
            hal_printf(("App Version   : %s \r\n", DEMO_APPLICATION_VERSION_STRING));

            // Let's output the Bluetooth Device Address so that the    
            // user knows what the Device Address is.                   
            if(!GAP_Query_Local_BD_ADDR(BluetoothStackID, &BD_ADDR))
            {
               BD_ADDRToStr(BD_ADDR, BluetoothAddress);

               hal_printf(("LOCAL BD_ADDR: %s\r\n", BluetoothAddress));
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
                  GAP_Set_Local_Device_Name (BluetoothStackID,LE_APP_DEMO_NAME);
                  GAPS_Set_Device_Name(BluetoothStackID, GAPSInstanceID, LE_APP_DEMO_NAME);
                  GAPS_Set_Device_Appearance(BluetoothStackID, GAPSInstanceID, GAP_DEVICE_APPEARENCE_VALUE_GENERIC_COMPUTER);

                  // Return success to the caller.                      
                  ret_val        = 0;
               }
               else
               {
                  // The Stack was NOT initialized successfully, inform 
                  // the user and set the return value of the           
                  // initialization function to an error.               
                  hal_printf("GAPS_Initialize_Service %d", Result);

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
               hal_printf("GATT_Initialize %d", Result);

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
            hal_printf("BSC_Initialize %d", Result);

            BluetoothStackID = 0;

            ret_val          = UNABLE_TO_INITIALIZE_STACK;
         }*/
      }
      else
      {
         // One or more of the necessary parameters are invalid.        
         ret_val = INVALID_PARAMETERS_ERROR;
      }
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
	   hal_printf("Opening stack\r\n");
      // Try to Open the stack and check if it was successful.          
      if(!OpenStack(HCI_DriverInformation, BTPS_Initialization))
      {
         // First, attempt to set the Device to be Connectable.         
         /*ret_val = SetConnect();

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
                     UI_Mode = UI_MODE_SELECT;

                     // Save the maximum supported baud rate.           
                     MaxBaudRate = (DWord_t)(HCI_DriverInformation->DriverInformation.COMMDriverInformation.BaudRate);

                     // Set up the Selection Interface.                 
                     UserInterface_Selection();

                     // Display the first command prompt.               
                     DisplayPrompt();

                     // Return success to the caller.                   
                     ret_val = (int)BluetoothStackID;
                  }
               }
               else
                  hal_printf("SetPairable %d", ret_val);
            }
            else
               hal_printf("SetDisc %d", ret_val);
         }
         else
            hal_printf("SetDisc %d", ret_val);

         // In some error occurred then close the stack.                
         if(ret_val < 0)
         {
            // Close the Bluetooth Stack.                               
            CloseStack();
         }*/
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

	hal_printf("init Bluetooth\r\n");
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


