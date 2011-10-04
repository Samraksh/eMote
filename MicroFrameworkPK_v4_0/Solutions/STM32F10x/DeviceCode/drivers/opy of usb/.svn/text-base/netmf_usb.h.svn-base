#include <tinyhal.h>
#include <..\pal\com\usb\USB.h>
#include <misc\misc.h>
#include <rcc\stm32f10x_rcc.h>
#include <gpio\stm32f10x_gpio.h>
#include <led\stm32f10x_led.h>
//extern "C"
//{
#include <usb\usb_regs.h>
#include <usb\usb_sil.h>
#include <usb\usb_core.h>
#include <usb\usb_int.h>
#include <usb\usb_def.h>
#include <usb\usb_mem.h>
#include <usb\usb_init.h>
//}

//Kartik : USB_conf macros
//
//Number of endpoints used by the device
#define EP_NUM                          (2)

/* buffer table base address */
#define BTABLE_ADDRESS      (0x00)

/* EP0  */
/* rx/tx buffer base address */
#define ENDP0_RXADDR        (0x18)
#define ENDP0_TXADDR        (0x58)

/* EP1  */
/* tx buffer base address */
#define ENDP1_TXADDR        (0x98)

/* EP2  */
/* Rx buffer base address */
#define ENDP2_RXADDR        (0xD8)

/* ISTR events */
/* IMR_MSK */
/* mask defining which events has to be handled by the device application software */
#define IMR_MSK (CNTR_RESETM | CNTR_SUSPM | CNTR_WKUPM | CNTR_ERRM | CNTR_CTRM | CNTR_DOVRM | CNTR_RESUME | CNTR_FSUSP | CNTR_LPMODE | CNTR_PDWN | CNTR_FRES)

/* CTR service routines */
/* associated to defined endpoints */
#define  EP1_IN_Callback   NOP_Process
#define  EP2_IN_Callback   NOP_Process
#define  EP3_IN_Callback   NOP_Process
#define  EP4_IN_Callback   NOP_Process
#define  EP5_IN_Callback   NOP_Process
#define  EP6_IN_Callback   NOP_Process
#define  EP7_IN_Callback   NOP_Process


#define  EP1_OUT_Callback   NOP_Process
#define  EP2_OUT_Callback   NOP_Process
#define  EP3_OUT_Callback   NOP_Process
#define  EP4_OUT_Callback   NOP_Process
#define  EP5_OUT_Callback   NOP_Process
#define  EP6_OUT_Callback   NOP_Process
#define  EP7_OUT_Callback   NOP_Process


//

#define USB_DISCONNECT                    GPIOB  
#define USB_DISCONNECT_PIN                GPIO_Pin_14
#define RCC_APB2Periph_GPIO_DISCONNECT    RCC_APB2Periph_GPIOB

//Kartik : USB_core.c variables defined here
//

//Kartik : For test purpose; Delete when donw
/* MASS Storage Requests*/
#define GET_MAX_LUN                0xFE
#define MASS_STORAGE_RESET         0xFF
#define LUN_DATA_LENGTH            1
//

//Kartik : Variables from usb_istr.c
UINT16 wInterrupt_Mask;
//ISTR register last read value
volatile UINT16 wIstr;
//SOFs received between 2 consecutive packets
__IO uint8_t bIntPackSOF = 0;

uint32_t Max_Lun = 0;

//Power variables (usb_pwr)
//
typedef enum _RESUME_STATE
{
  RESUME_EXTERNAL,
  RESUME_INTERNAL,
  RESUME_LATER,
  RESUME_WAIT,
  RESUME_START,
  RESUME_ON,
  RESUME_OFF,
  RESUME_ESOF
} RESUME_STATE;

typedef enum _DEVICE_STATE
{
  UNCONNECTED,
  ATTACHED,
  POWERED,
  SUSPENDED,
  ADDRESSED,
  CONFIGURED
} DEVICE_STATE;

__IO uint32_t bDeviceState = UNCONNECTED; /* USB device status */
__IO bool fSuspendEnabled = TRUE;  /* true when suspend is possible */

struct
{
  __IO RESUME_STATE eState;
  __IO uint8_t bESOFcnt;
}ResumeS;
//


//Kartik : USB_init variables
DEVICE_INFO *pInformation;
DEVICE_PROP *pProperty;
DEVICE_INFO	Device_Info;
USER_STANDARD_REQUESTS  *pUser_Standard_Requests;
//

//Kartik : USB_prop macros
#define Mass_Storage_GetConfiguration          NOP_Process
/* #define Mass_Storage_SetConfiguration          NOP_Process*/
#define Mass_Storage_GetInterface              NOP_Process
#define Mass_Storage_SetInterface              NOP_Process
#define Mass_Storage_GetStatus                 NOP_Process
/* #define Mass_Storage_ClearFeature              NOP_Process*/
#define Mass_Storage_SetEndPointFeature        NOP_Process
#define Mass_Storage_SetDeviceFeature          NOP_Process
/*#define Mass_Storage_SetDeviceAddress          NOP_Process*/
//

//Kartik : USB_desc.h constants
//
#define MASS_SIZ_DEVICE_DESC              18
#define MASS_SIZ_CONFIG_DESC              32

#define MASS_SIZ_STRING_LANGID            4
//#define MASS_SIZ_STRING_VENDOR            38
#define MASS_SIZ_STRING_VENDOR            15
#define MASS_SIZ_STRING_PRODUCT           38
#define MASS_SIZ_STRING_SERIAL            26
#define MASS_SIZ_STRING_INTERFACE         16

const uint8_t MASS_DeviceDescriptor[MASS_SIZ_DEVICE_DESC] =
  {
    0x12,   /* bLength  */
    0x01,   /* bDescriptorType */
    0x00,   /* bcdUSB, version 2.00 */
    0x02,
    0x00,   /* bDeviceClass : each interface define the device class */
    0x00,   /* bDeviceSubClass */
    0x00,   /* bDeviceProtocol */
    0x40,   /* bMaxPacketSize0 0x40 = 64 */
    0x83,   /* idVendor     (0483) */
    0x04,
    0x20,   /* idProduct */
    0x57,
    0x00,   /* bcdDevice 2.00*/
    0x02,
    1,              /* index of string Manufacturer  */
    /**/
    //2,              /* index of string descriptor of product*/
	0,              /* index of string descriptor of product*/
    /* */
    //3,              /* */
	0,              /* */
    /* */
    /* */
    0x01    /*bNumConfigurations */
  };
const uint8_t MASS_ConfigDescriptor[MASS_SIZ_CONFIG_DESC] =
  {

    0x09,   /* bLength: Configuration Descriptor size */
    0x02,   /* bDescriptorType: Configuration */
    MASS_SIZ_CONFIG_DESC,

    0x00,
    0x01,   /* bNumInterfaces: 1 interface */
    0x01,   /* bConfigurationValue: */
    /*      Configuration value */
    0x00,   /* iConfiguration: */
    /*      Index of string descriptor */
    /*      describing the configuration */
    0xC0,   /* bmAttributes: */
    /*      bus powered */
    0x32,   /* MaxPower 100 mA */

    /******************** Descriptor of Mass Storage interface ********************/
    /* 09 */
    0x09,   /* bLength: Interface Descriptor size */
    0x04,   /* bDescriptorType: */
    /*      Interface descriptor type */
    0x00,   /* bInterfaceNumber: Number of Interface */
    0x00,   /* bAlternateSetting: Alternate setting */
    0x02,   /* bNumEndpoints*/
//    0x08,   /* bInterfaceClass: MASS STORAGE Class */
	0xff,   /* bInterfaceClass: MASS STORAGE Class */
    //0x06,   /* bInterfaceSubClass : SCSI transparent*/
	0x01,   /* bInterfaceSubClass : SCSI transparent*/
    //0x50,   /* nInterfaceProtocol */
	0x01,   /* nInterfaceProtocol */
    0,          /* iInterface: */
    /* 18 */
    0x07,   /*Endpoint descriptor length = 7*/
    0x05,   /*Endpoint descriptor type */
    0x81,   /*Endpoint address (IN, address 1) */
    0x02,   /*Bulk endpoint type */
    0x40,   /*Maximum packet size (64 bytes) */
    0x00,
    0x00,   /*Polling interval in milliseconds */
    /* 25 */
    0x07,   /*Endpoint descriptor length = 7 */
    0x05,   /*Endpoint descriptor type */
    0x02,   /*Endpoint address (OUT, address 2) */
    0x02,   /*Bulk endpoint type */
    0x40,   /*Maximum packet size (64 bytes) */
    0x00,
    0x00     /*Polling interval in milliseconds*/
    /*32*/
  };
const uint8_t MASS_StringLangID[MASS_SIZ_STRING_LANGID] =
  {
    MASS_SIZ_STRING_LANGID,
    0x03,
    0x09,
    0x04
  }
  ;      /* LangID = 0x0409: U.S. English */
const uint8_t MASS_StringVendor[MASS_SIZ_STRING_VENDOR] =
  {
    MASS_SIZ_STRING_VENDOR, /* Size of manufacturer string */
    0x03,           /* bDescriptorType = String descriptor */
    /* Manufacturer: "STMicroelectronics" */
    //'S', 0, 'T', 0, 'M', 0, 'i', 0, 'c', 0, 'r', 0, 'o', 0, 'e', 0,
    //'l', 0, 'e', 0, 'c', 0, 't', 0, 'r', 0, 'o', 0, 'n', 0, 'i', 0,
    //'c', 0, 's', 0
	'E', 0, 'M', 0, 'O', 0, 'T', 0, 'E', 0
  };
const uint8_t MASS_StringProduct[MASS_SIZ_STRING_PRODUCT] =
  {
    MASS_SIZ_STRING_PRODUCT,
    0x03,
    /* Product name: "STM32F10x:USB Mass Storage" */
    'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0, ' ', 0, 'M', 0, 'a', 0, 's', 0,
    's', 0, ' ', 0, 'S', 0, 't', 0, 'o', 0, 'r', 0, 'a', 0, 'g', 0, 'e', 0

  };

uint8_t MASS_StringSerial[MASS_SIZ_STRING_SERIAL] =
  {
    MASS_SIZ_STRING_SERIAL,
    0x03,
    /* Serial number*/
#ifdef STM32L1XX_MD
    'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0, 'L', 0, '1', 0
#else
    'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0, '1', 0, '0', 0      
#endif /* STM32L1XX_MD */
  };
const uint8_t MASS_StringInterface[MASS_SIZ_STRING_INTERFACE] =
  {
    MASS_SIZ_STRING_INTERFACE,
    0x03,
    /* Interface 0: "ST Mass" */
    'S', 0, 'T', 0, ' ', 0, 'M', 0, 'a', 0, 's', 0, 's', 0
  };

ONE_DESCRIPTOR Device_Descriptor =
  {
    (uint8_t*)MASS_DeviceDescriptor,
    MASS_SIZ_DEVICE_DESC
  };

ONE_DESCRIPTOR Config_Descriptor =
  {
    (uint8_t*)MASS_ConfigDescriptor,
    MASS_SIZ_CONFIG_DESC
  };

ONE_DESCRIPTOR String_Descriptor[5] =
  {
    {(uint8_t*)MASS_StringLangID, MASS_SIZ_STRING_LANGID},
    {(uint8_t*)MASS_StringVendor, MASS_SIZ_STRING_VENDOR},
    {(uint8_t*)MASS_StringProduct, MASS_SIZ_STRING_PRODUCT},
    {(uint8_t*)MASS_StringSerial, MASS_SIZ_STRING_SERIAL},
    {(uint8_t*)MASS_StringInterface, MASS_SIZ_STRING_INTERFACE},
  };


//

struct USBCS_Driver
{
    static const UINT32 c_Used_Endpoints           = 7;
    static const UINT32 c_default_ctrl_packet_size = 18;

    USB_CONTROLLER_STATE*   pUsbControllerState;
    
#if defined(USB_REMOTE_WAKEUP)
    #define USB_MAX_REMOTE_WKUP_RETRY 5

    HAL_COMPLETION          RemoteWKUP10msCompletion;
    HAL_COMPLETION          RemoteWKUP100msEOPCompletion;
    UINT32                  RemoteWkUpRetry;
#endif

	UINT8                   ControlPacketBuffer[c_default_ctrl_packet_size];
	//UINT16                  ControlPacketBuffer[c_default_ctrl_packet_size];
    UINT16                  EndpointStatus[c_Used_Endpoints];
    BOOL                    TxRunning [USB_MAX_QUEUES];
    BOOL                    TxNeedZLPS[USB_MAX_QUEUES];

    UINT8                   PreviousDeviceState;
    UINT8                   RxExpectedToggle[USB_MAX_QUEUES];
    BOOL                    PinsProtected;
    BOOL                    FirstDescriptorPacket;

#if defined(USB_METRIC_COUNTING)
    USB_PERFORMANCE_METRICS PerfMetrics;
#endif

    //--//

    static USB_CONTROLLER_STATE * GetState( int Controller );

    static HRESULT Initialize( int Controller );

    static HRESULT Uninitialize( int Controller );

    static BOOL StartOutput( USB_CONTROLLER_STATE* State, int endpoint );

    static BOOL RxEnable( USB_CONTROLLER_STATE* State, int endpoint );

    static BOOL GetInterruptState();

    static BOOL ProtectPins( int Controller, BOOL On );


//Kartik : Making all the functions public so that the generic interrupt handler can call them
//uncomment after integration with INTC
//private:
    static void ClearTxQueue( USB_CONTROLLER_STATE* State, int endpoint );

    static void StartHardware();

    static void StopHardware ();

    static void TxPacket( USB_CONTROLLER_STATE* State, int endpoint );

    static void ControlNext();

    static void SuspendEvent();
    static void ResumeEvent ();
    static void ResetEvent  ();

    static void Global_ISR  ( void*  Param );
    static void EP0_ISR     ( UINT32 Param );    
    static void EP_TxISR    ( UINT32 Param );    
    static void EP_RxISR    ( UINT32 Param );    

	//STM Functions

	static void USB_Interrupts_Config();
	static void USB_Clock();
	static void Set_System();
	static void USB_Cable_Config(bool value);

#if defined(USB_REMOTE_WAKEUP)
    static void RemoteWkUp_ISR    ( void* Param );
    static void RemoteWkUp_Process( void* Param );
#endif
};

extern USBCS_Driver g_USB_Driver;

struct USBCS_CONFIG
{
    HAL_DRIVER_CONFIG_HEADER Header;

    //--//

    static LPCSTR GetDriverName() { return "eMote_USB"; }
};

extern "C" 
{
	void usb_irq_handler();
}
extern USB_CONFIG* const g_USB_Config;

//extern void USB_Interrupts_Config();
//extern void USB_Clock();
//extern void Set_System();
//extern void USB_Cable_Config(bool value);
//extern void SetDeviceAddress(UINT8 val);
//extern void Reset();
//extern void ctr_lp();
//extern UINT8 Setup0_Process(void);
//extern UINT16 ByteSwap(UINT16 wSwW);
//extern void Data_Setup0(void);
//extern void DataStageIn();
//extern void NoData_Setup0();
//extern UINT8 In0_Process();


//STM Application layer functions
void STM_USB_Init();
void STM_USB_Reset();
void STM_USB_Status_In();
void STM_USB_Status_Out();
RESULT STM_USB_Data_Setup(uint8_t RequestNo);
RESULT STM_USB_NoData_Setup(uint8_t RequestNo);
RESULT STM_USB_Get_Interface_Setting(uint8_t Interface, uint8_t AlternateSetting);
uint8_t* STM_USB_GetDeviceDescriptor(uint16_t Length);
uint8_t* STM_USB_GetConfigDescriptor(uint16_t Length);
uint8_t* STM_USB_GetStringDescriptor(uint16_t Length);
void STM_USB_SetConfiguration();
void STM_USB_ClearFeature();
void STM_USB_SetDeviceAddress();
void STM_USB_GetConfiguration();
void STM_USB_GetInterface();
void STM_USB_SetInterface();
void STM_USB_GetStatus();
void STM_USB_SetEndPointFeature();
void STM_USB_SetDeviceFeature();

//MSD related function
uint8_t* Get_Max_Lun(uint16_t Length);

//function pointers to non-control endpoints service routines
void (*pEpInt_IN[7])(void) =
  {
    EP1_IN_Callback,
    EP2_IN_Callback,
    EP3_IN_Callback,
    EP4_IN_Callback,
    EP5_IN_Callback,
    EP6_IN_Callback,
    EP7_IN_Callback,
  };

void (*pEpInt_OUT[7])(void) =
  {
    EP1_OUT_Callback,
    EP2_OUT_Callback,
    EP3_OUT_Callback,
    EP4_OUT_Callback,
    EP5_OUT_Callback,
    EP6_OUT_Callback,
    EP7_OUT_Callback,
  };
//
