#ifndef RADIO_H
#define RADIO_H

/* === INCLUDES ============================================================ */
#include <Tinyhal_types.h>

/* === EXTERNALS =========================================================== */


/* === TYPES =============================================================== */


/* === MACROS ============================================================== */

#define TRX_SUPPORTED_CHANNELS          (0x07FFF800)

/**
 * Tolerance of the phyTransmitPower PIB attribute.  This is encoded
 * into the two MSBits of the attribute, and is effectively read-only.
 */
#define TX_PWR_TOLERANCE                (0x80)

/*
 * Timing parameters
 */
#define RST_PULSE_WIDTH_US              (6)
#define SLP_TR_TOGGLE_US                (2)
#define SLEEP_TO_TRX_OFF_US             (880)
#define P_ON_TO_CLKM_AVAILABLE          (500)
#define PLL_LOCK_TIME_US                (180) /* us */
#define TRX_OFF_TO_SLEEP_TIME           (35)  /* us */
#define IRQ_PROCESSING_DLY_US           (32)

/*
 * CSMA timimg parameters
 */
#define CCA_DETECTION_TIME_SYM          (8)
#define CCA_PROCESS_TIME_SYM            (1)
#define CCA_DURATION_SYM                (CCA_DETECTION_TIME_SYM + CCA_PROCESS_TIME_SYM)
#define CSMA_PROCESSING_TIME_US         (100)
#define CCA_PREPARATION_DURATION_US     (50)
#define CCA_PRE_START_DURATION_US       (20)
#define PRE_TX_DURATION_US              (32)    // 16 us processing delay +
                                                // 16 us software handling duration

/*
 * Registers and values for TRX_STATUS
 */
#define RG_TRX_STATUS                   (0x01)
/** Access parameters for sub-register CCA_DONE in register @ref RG_TRX_STATUS */
#define SR_CCA_DONE                     0x01, 0x80, 7
#define CCA_DONE_BIT                    (0x80)
/** Access parameters for sub-register CCA_STATUS in register @ref RG_TRX_STATUS */
#define SR_CCA_STATUS                   0x01, 0x40, 6
#define SR_reserved_01_3                0x01, 0x20, 5
/** Access parameters for sub-register TRX_STATUS in register @ref RG_TRX_STATUS */
#define SR_TRX_STATUS                   0x01, 0x1f, 0
#define CCA_STATUS_BIT                  (0x40)
/** Constants for sub-register SR_TRX_STATUS */
// see tal_internal.h: tal_trx_status_t

/** Address for register TRX_STATE */
#define RG_TRX_STATE                    (0x02)
/** Access parameters for sub-register TRAC_STATUS in register @ref RG_TRX_STATE */
#define SR_TRAC_STATUS                  0x02, 0xe0, 5
/** Constant TRAC_SUCCESS for sub-register @ref SR_TRAC_STATUS */
#define TRAC_SUCCESS                    (0)
#define TRAC_SUCCESS_DATA_PENDING       (1)
#define TRAC_CHANNEL_ACCESS_FAILURE     (3)
#define TRAC_NO_ACK                     (5)
#define TRAC_INVALID                    (7)

/** Access parameters for sub-register TRX_CMD in register @ref RG_TRX_STATE */
#define SR_TRX_CMD                      0x02, 0x1f, 0

/** Address for register TRX_CTRL_0 */
#define RG_TRX_CTRL_0                   (0x03)
/** Access parameters for sub-registers in register @ref RG_TRX_CTRL_0 */
#define SR_PAD_IO                       0x03, 0xc0, 6
/** Access parameters for sub-register PAD_IO_CLKM in register @ref RG_TRX_CTRL_0 */
#define SR_PAD_IO_CLKM                  0x03, 0x30, 4
/** Constants for sub-register @ref SR_PAD_IO_CLKM */
#define CLKM_2mA                        (0)
#define CLKM_4mA                        (1)
#define CLKM_6mA                        (2)
#define CLKM_8mA                        (3)
/** Access parameters for sub-register CLKM_SHA_SEL in register @ref RG_TRX_CTRL_0 */
#define SR_CLKM_SHA_SEL                 0x03, 0x08, 3
/** Access parameters for sub-register CLKM_CTRL in register @ref RG_TRX_CTRL_0 */
#define SR_CLKM_CTRL                    0x03, 0x07, 0
/** Constants for sub-register @ref SR_CLKM_CTRL */
#define CLKM_no_clock                   (0)
#define CLKM_1MHz                       (1)
#define CLKM_2MHz                       (2)
#define CLKM_4MHz                       (3)
#define CLKM_8MHz                       (4)
#define CLKM_16MHz                      (5)

/** Address for register PHY_TX_PWR */
#define RG_PHY_TX_PWR                   (0x05)
/** Access parameters for sub-register TX_AUTO_CRC_ON in register @ref RG_PHY_TX_PWR */
#define TX_AUTO_CRC_ON                  0x80
#define SR_TX_AUTO_CRC_ON               0x05, 0x80, 7
#define SR_reserved_05_2                0x05, 0x70, 4

/** Access parameters for sub-register TX_PWR in register @ref RG_PHY_TX_PWR */
#define SR_TX_PWR                       0x05, 0x0f, 0

/** Address for register PHY_RSSI */
#define RG_PHY_RSSI                     (0x06)
/** Access parameters for sub-register RX_CRC_VALID in register @ref RG_PHY_RSSI */
#define SR_RX_CRC_VALID                 0x06, 0x80, 7
/** Constant CRC16_not_valid for sub-register @ref SR_RX_CRC_VALID */
#define CRC16_NOT_VALID                 (0)
/** Constant CRC16_valid for sub-register @ref SR_RX_CRC_VALID */
#define CRC16_VALID                     (1)
#define SR_reserved_06_2                0x06, 0x60, 5
/** Access parameters for sub-register RSSI in register @ref RG_PHY_RSSI */
#define SR_RSSI                         0x06, 0x1f, 0

/** Address for register PHY_ED_LEVEL */
#define RG_PHY_ED_LEVEL                 (0x07)
/** Access parameters for sub-register ED_LEVEL in register @ref RG_PHY_ED_LEVEL */
#define SR_ED_LEVEL                     0x07, 0xff, 0

/** Address for register PHY_CC_CCA */
#define RG_PHY_CC_CCA                   (0x08)
/** Access parameters for sub-registers in register @ref RG_PHY_CC_CCA */
#define SR_CCA_REQUEST                  0x08, 0x80, 7
#define SR_CCA_MODE                     0x08, 0x60, 5
#define SR_CHANNEL                      0x08, 0x1f, 0

/** Address for register CCA_THRES */
#define RG_CCA_THRES                    (0x09)
/** Access parameters for sub-registers in register @ref RG_CCA_THRES */
#define SR_CCA_CS_THRES                 0x09, 0xf0, 4
#define SR_CCA_ED_THRES                 0x09, 0x0f, 0

/** Offset for register SFD_VALUE */
#define RG_SFD_VALUE                    (0x0b)

/** Address for register IRQ_MASK */
#define RG_IRQ_MASK                     (0x0e)
/** Access parameters for sub-register IRQ_MASK in register @ref RG_IRQ_MASK */
#define SR_IRQ_MASK                     0x0e, 0xff, 0

/** Address for register IRQ_STATUS */
#define RG_IRQ_STATUS                   (0x0f)

/** Access parameters for sub-registers in register @ref RG_IRQ_STATUS */
#define SR_IRQ_7_BAT_LOW                0x0f, 0x80, 7
#define SR_IRQ_6_TRX_UR                 0x0f, 0x40, 6
#define SR_IRQ_5                        0x0f, 0x20, 5
#define SR_IRQ_4                        0x0f, 0x10, 4
#define SR_IRQ_3_TRX_END                0x0f, 0x08, 3
#define SR_IRQ_2_RX_START               0x0f, 0x04, 2
#define SR_IRQ_1_PLL_UNLOCK             0x0f, 0x02, 1
#define SR_IRQ_0_PLL_LOCK               0x0f, 0x01, 0

/** Address for register VREG_CTRL */
#define RG_VREG_CTRL                    (0x10)
/** Access parameters for sub-register AVREG_EXT in register @ref RG_VREG_CTRL */
#define SR_AVREG_EXT                    0x10, 0x80, 7
/** Access parameters for sub-register AVDD_OK in register @ref RG_VREG_CTRL */
#define SR_AVDD_OK                      0x10, 0x40, 6
/** Access parameters for sub-register AVREG_TRIM in register @ref RG_VREG_CTRL */
#define SR_AVREG_TRIM                   0x10, 0x30, 4
/** Constants for sub-register @ref SR_AVREG_TRIM */
#define AVREG_1_80V                     (0)
#define AVREG_1_75V                     (1)
#define AVREG_1_84V                     (2)
#define AVREG_1_88V                     (3)
/** Access parameters for sub-register DVREG_EXT in register @ref RG_VREG_CTRL */
#define SR_DVREG_EXT                    0x10, 0x08, 3
#define SR_DVDD_OK                      0x10, 0x04, 2
#define SR_DVREG_TRIM                   0x10, 0x03, 0
/** Constants for sub-register @ref SR_DVREG_TRIM */
#define DVREG_1_80V                     (0)
#define DVREG_1_75V                     (1)
#define DVREG_1_84V                     (2)
#define DVREG_1_88V                     (3)

/** Address for register BATMON */
#define RG_BATMON                       (0x11)
#define SR_reserved_11_1                0x11, 0xc0, 6
/** Access parameters for sub-registers in register @ref RG_BATMON */
#define SR_BATMON_OK                    0x11, 0x20, 5
#define SR_BATMON_HR                    0x11, 0x10, 4
#define SR_BATMON_VTH                   0x11, 0x0f, 0
/** Constant BATMON_BELOW_THRES for sub-register SR_BATMON_OK */
#define BATMON_BELOW_THRES              (0)
/** Constant BATMON_ABOVE_THRES for sub-register SR_BATMON_OK */
#define BATMON_ABOVE_THRES              (1)
/** Constant BATMON_LOW_RANGE for sub-register BATMON_HR in register RG_BATMON */
#define BATMON_LOW_RANGE                (0)
/** Constant BATMON_HIGH_RANGE for sub-register BATMON_HR in register RG_BATMON */
#define BATMON_HIGH_RANGE               (1)

/** Address for register XOSC_CTRL */
#define RG_XOSC_CTRL                    (0x12)
/** Access parameters for sub-registers in register @ref RG_XOSC_CTRL */
#define SR_XTAL_MODE                    0x12, 0xf0, 4
#define SR_XTAL_TRIM                    0x12, 0x0f, 0

/** Address for register FTN_CTRL */
#define RG_FTN_CTRL                     (0x18)
/** Access parameters for sub-register FTN_START in register @ref RG_FTN_CTRL */
#define SR_FTN_START                    0x18, 0x80, 7
#define SR_reserved_18_2                0x18, 0x40, 6
/** Access parameters for sub-register FTNV in register @ref RG_FTN_CTRL */
#define SR_FTNV                         0x18, 0x3f, 0

/** Address for register PLL_CF */
#define RG_PLL_CF                       (0x1a)
/** Access parameters for sub-register PLL_CF_START in register @ref RG_PLL_CF */
#define SR_PLL_CF_START                 0x1a, 0x80, 7
#define SR_reserved_1a_2                0x1a, 0x70, 4
/** Access parameters for sub-register PLL_CF in register @ref RG_PLL_CF */
#define SR_PLL_CF                       0x1a, 0x0f, 0

/** Address for register PLL_DCU */
#define RG_PLL_DCU                      (0x1b)
/** Access parameters for sub-register PLL_DCU_START in register @ref RG_PLL_DCU */
#define SR_PLL_DCU_START                0x1b, 0x80, 7
#define SR_reserved_1b_2                0x1b, 0x40, 6
/** Access parameters for sub-register PLL_DCUW in register @ref RG_PLL_DCU */
#define SR_PLL_DCUW                     0x1b, 0x3f, 0

/** Address for register PART_NUM */
#define RG_PART_NUM                     (0x1c)
/** Access parameters for sub-register PART_NUM in register @ref RG_PART_NUM */
#define SR_PART_NUM                     0x1c, 0xff, 0
/** Constant RF230 for sub-register @ref SR_PART_NUM */
#define AT86RF230                       (2)

/** Address for register VERSION_NUM */
#define RG_VERSION_NUM                  (0x1d)
/** Constant RF230 Rev B for @ref VERSION_NUM */
#define AT86RF230_REV_B                1//2
/** Access parameters for sub-register VERSION_NUM in register @ref RG_VERSION_NUM */
#define SR_VERSION_NUM                  0x1d, 0xff, 0

/** Address for register MAN_ID_0 */
#define RG_MAN_ID_0                     (0x1e)
/** Access parameters for sub-registers in register @ref RG_MAN_ID_0 */
#define SR_MAN_ID_0                     0x1e, 0xff, 0
/** Address for register MAN_ID_1 */
#define RG_MAN_ID_1                     (0x1f)
/** Access parameters for sub-registers in register @ref RG_MAN_ID_1 */
#define SR_MAN_ID_1                     0x1f, 0xff, 0

/** Address for register SHORT_ADDR_0 */
#define RG_SHORT_ADDR_0                 (0x20)
/** Access parameters for sub-register SHORT_ADDR_0 in register @ref RG_SHORT_ADDR_0 */
#define SR_SHORT_ADDR_0                 0x20, 0xff, 0
/** Address for register SHORT_ADDR_1 */
#define RG_SHORT_ADDR_1                 (0x21)
/** Access parameters for sub-register SHORT_ADDR_1 in register @ref RG_SHORT_ADDR_1 */
#define SR_SHORT_ADDR_1                 0x21, 0xff, 0

/** Address for register PAN_ID_0 */
#define RG_PAN_ID_0                     (0x22)
/** Access parameters for sub-register PAN_ID_0 in register @ref RG_PAN_ID_0 */
#define SR_PAN_ID_0                     0x22, 0xff, 0
/** Address for register PAN_ID_1 */
#define RG_PAN_ID_1                     (0x23)
/** Access parameters for sub-register PAN_ID_1 in register @ref RG_PAN_ID_1 */
#define SR_PAN_ID_1                     0x23, 0xff, 0

/** Address for register IEEE_ADDR_0 */
#define RG_IEEE_ADDR_0                  (0x24)
/** Access parameters for sub-register IEEE_ADDR_0 in register @ref RG_IEEE_ADDR_0 */
#define SR_IEEE_ADDR_0                  0x24, 0xff, 0
/** Address for register IEEE_ADDR_1 */
#define RG_IEEE_ADDR_1                  (0x25)
/** Access parameters for sub-register IEEE_ADDR_1 in register @ref RG_IEEE_ADDR_1 */
#define SR_IEEE_ADDR_1                  0x25, 0xff, 0
/** Address for register IEEE_ADDR_2 */
#define RG_IEEE_ADDR_2                  (0x26)
/** Access parameters for sub-register IEEE_ADDR_2 in register @ref RG_IEEE_ADDR_2 */
#define SR_IEEE_ADDR_2                  0x26, 0xff, 0
/** Address for register IEEE_ADDR_3 */
#define RG_IEEE_ADDR_3                  (0x27)
/** Access parameters for sub-register IEEE_ADDR_3 in register @ref RG_IEEE_ADDR_3 */
#define SR_IEEE_ADDR_3                  0x27, 0xff, 0
/** Address for register IEEE_ADDR_4 */
#define RG_IEEE_ADDR_4                  (0x28)
/** Access parameters for sub-register IEEE_ADDR_4 in register @ref RG_IEEE_ADDR_4 */
#define SR_IEEE_ADDR_4                  0x28, 0xff, 0
/** Address for register IEEE_ADDR_5 */
#define RG_IEEE_ADDR_5                  (0x29)
/** Access parameters for sub-register IEEE_ADDR_5 in register @ref RG_IEEE_ADDR_5 */
#define SR_IEEE_ADDR_5                  0x29, 0xff, 0
/** Address for register IEEE_ADDR_6 */
#define RG_IEEE_ADDR_6                  (0x2a)
/** Access parameters for sub-register IEEE_ADDR_6 in register @ref RG_IEEE_ADDR_6 */
#define SR_IEEE_ADDR_6                  0x2a, 0xff, 0
/** Address for register IEEE_ADDR_7 */
#define RG_IEEE_ADDR_7                  (0x2b)
/** Access parameters for sub-register IEEE_ADDR_7 in register @ref RG_IEEE_ADDR_7 */
#define SR_IEEE_ADDR_7                  0x2b, 0xff, 0

/** Address for register XAH_CTRL */
#define RG_XAH_CTRL                     (0x2c)
/** Access parameters for sub-register MAX_FRAME_RETRIES in register @ref RG_XAH_CTRL */
#define SR_MAX_FRAME_RETRIES            0x2c, 0xf0, 4
/** Access parameters for sub-register MAX_CSMA_RETRIES in register @ref RG_XAH_CTRL */
#define SR_MAX_CSMA_RETRIES             0x2c, 0x0e, 1
#define SR_reserved_2c_3                0x2c, 0x01, 0

/** Address for register CSMA_SEED_0 */
#define RG_CSMA_SEED_0                  (0x2d)
/** Access parameters for sub-register CSMA_SEED_0 in register @ref RG_CSMA_SEED_0 */
#define SR_CSMA_SEED_0                  0x2d, 0xff, 0

/** Address for register CSMA_SEED_1 */
#define RG_CSMA_SEED_1                  (0x2e)
/** Access parameters for sub-register MIN_BE in register @ref RG_CSMA_SEED_1 */
#define SR_MIN_BE                       0x2e, 0xc0, 6
/** Access parameters for sub-register AACK_SET_PD in register @ref RG_CSMA_SEED_1 */
#define SR_AACK_SET_PD                  0x2e, 0x20, 5
#define SR_reserved_2e_3                0x2e, 0x10, 4
/** Access parameters for sub-register I_AM_COORD in register @ref RG_CSMA_SEED_1 */
#define SR_I_AM_COORD                   0x2e, 0x08, 3
/** Access parameters for sub-register CSMA_SEED_1 in register @ref RG_CSMA_SEED_1 */
#define SR_CSMA_SEED_1                  0x2e, 0x07, 0

/** RSSI Base Value */
#define RSSI_BASE_VAL                    (-91)

/** Constant TX_AUTO_CRC_ENABLE for sub-register TX_AUTO_CRC_ON */
#define TX_AUTO_CRC_ENABLE   (1)

/** Constant TX_AUTO_CRC_ENABLE for sub-register TX_AUTO_CRC_ON */
#define TX_AUTO_CRC_DISABLE   (0)

/** Constant PD_ACK_BIT_SET_ENABLE for sub-register AACK_SET_PD in register RG_CSMA_SEED_1 */
#define PD_ACK_BIT_SET_ENABLE      (1)

/** Constant PD_ACK_BIT_SET_DISABLE for sub-register AACK_SET_PD in register RG_CSMA_SEED_1 */
#define PD_ACK_BIT_SET_DISABLE      (0)


/*
 * TAL PIB default values
*/
#define TAL_ACK_WAIT_DURATION_DEFAULT       (0x36)//Default value of ack wait duration
#define TAL_CURRENT_PAGE_DEFAULT            (0x00)//Default value of custom TAL PIB channel page
#define TAL_MAX_FRAME_DURATION_DEFAULT      (MAX_FRAME_DURATION)//Default value of maximum number of symbols in a frame
#define TAL_SHR_DURATION_DEFAULT            (NO_OF_SYMBOLS_PREAMBLE_SFD)//Default value of duration of the synchronization header 
																		//(SHR) in symbols for the current PHY
#define TAL_SYMBOLS_PER_OCTET_DEFAULT       (SYMBOLS_PER_OCTET)//Default value of number of symbols per octet for the current PHY
#define TAL_MAXBE_DEFAULT                   (0x05)//Default value of maximum backoff exponent used while performing csma ca
#define TAL_MAXFRAMERETRIES_DEFAULT         (0x03)//Default value of PIB attribute macMaxFrameRetries
#define TAL_MAX_CSMA_BACKOFFS_DEFAULT       (0x04)//Default value of maximum csma ca backoffs
#define TAL_MINBE_DEFAULT                   (0x03)//Default value of minimum backoff exponent used while performing csma ca
#define TAL_PANID_BC_DEFAULT                (0xFFFF)//Value of a broadcast PAN ID
#define TAL_SHORT_ADDRESS_DEFAULT           (0xFFFF)//Default value of short address
#define TAL_CURRENT_CHANNEL_DEFAULT         (0x0B)//Default value of current channel in TAL
#define TAL_PIB_PROMISCUOUS_MODE_DEFAULT    (false)//Default value of promiscuous mode in TAL
#define TAL_TRANSMIT_POWER_DEFAULT          (TX_PWR_TOLERANCE | 0x03)//Default value of transmit power of transceiver: Use highest tx power
#define TAL_CCA_MODE_DEFAULT                (TRX_CCA_MODE1)//Default value CCA mode
#define TAL_BEACON_ORDER_DEFAULT            (15)//Default value beacon order set to 15
#define TAL_SUPERFRAME_ORDER_DEFAULT        (15)//Default value supeframe order set to 15 
#define TAL_BEACON_TX_TIME_DEFAULT          (0x00000000)//Default value of BeaconTxTime
#define TAL_BATTERY_LIFE_EXTENSION_DEFAULT  (false)//Default value of BatteryLifeExtension.
#define TAL_PAN_COORDINATOR_DEFAULT         (false)//Default value of PAN Coordiantor custom TAL PIB



/** Transceiver commands */
typedef enum trx_cmd
{
    CMD_NOP                           = (0),
    CMD_TX_START                      = (2),
    CMD_FORCE_TRX_OFF                 = (3),
    CMD_RX_ON                         = (6),
    CMD_TRX_OFF                       = (8),
    CMD_PLL_ON                        = (9),
    CMD_FORCE_PLL_ON                  = (10),
    CMD_RX_AACK_ON                    = (22),
    CMD_TX_ARET_ON                    = (25),
    CMD_TRX_SLEEP                     = (26)  /* not a register command */
} trx_cmd_t;


/** Transceiver interrupt reasons */
typedef enum trx_irq_reason
{
    TRX_NO_IRQ                      = (0x00),
    TRX_IRQ_4                       = (0x10),
    TRX_IRQ_5                       = (0x20),
    TRX_IRQ_BAT_LOW                 = (0x80),
    TRX_IRQ_PLL_LOCK                = (0x01),
    TRX_IRQ_PLL_UNLOCK              = (0x02),
    TRX_IRQ_RX_START                = (0x04),
    TRX_IRQ_TRX_END                 = (0x08),
    TRX_IRQ_TRX_UR                  = (0x40)
} trx_irq_reason_t;


/** Transceiver states */
typedef enum radio_hal_trx_status
{
    P_ON                            = 0,
    BUSY_RX                         = 1,
    BUSY_TX                         = 2,
    RX_ON                           = 6,
    TRX_OFF                         = 8,
    PLL_ON                          = 9,
    TRX_SLEEP                       = 15,
    BUSY_RX_AACK                    = 17,
    BUSY_TX_ARET                    = 18,
    RX_AACK_ON                      = 22,
    TX_ARET_ON                      = 25,
    RX_ON_NOCLK                     = 28,
    RX_AACK_ON_NOCLK                = 29,
    BUSY_RX_AACK_NOCLK              = 30,
    STATE_TRANSITION_IN_PROGRESS    = 31
} radio_hal_trx_status_t;

//Sleep mode for the radio
typedef enum sleep_mode
{
    SLEEP_MODE_1
} sleep_mode_t;

typedef enum radio_hal_state
{
    radio_hal_IDLE           = 0,
    radio_hal_TX_BEACON      = 1,
    radio_hal_TX_AUTO        = 2,
    radio_hal_TX_BASIC       = 3,
    radio_hal_SLEEP          = 4,
    radio_hal_SLOTTED_CSMA   = 5,
    radio_hal_ED             = 6
} radio_hal_state_t;


/*
 * TAL PIBs
 */
UINT8 radio_hal_pib_MaxCSMABackoffs;//The maximum number of back-offs the CSMA-CA algorithm will attempt before declaring a CSMA_CA failure.
UINT8 radio_hal_pib_MinBE;//* The minimum value of the backoff exponent BE in the CSMA-CA algorithm.
UINT16 radio_hal_pib_PANId;//16-bit PAN ID.
UINT16 radio_hal_pib_ShortAddress;//Node's 16-bit short address.
UINT64 radio_hal_pib_IeeeAddress;//Node's 64-bit (IEEE) address.
UINT8 radio_hal_pib_CurrentChannel;//Current RF channel to be used for all transmissions and receptions.
UINT32 radio_hal_pib_SupportedChannels;//Supported channels
UINT8 radio_hal_pib_CurrentPage;//Current channel page; supported: page 0
UINT16 radio_hal_pib_MaxFrameDuration;//Maximum number of symbols in a frame: = phySHRDuration + ceiling([aMaxPHYPacketSize + 1] x phySymbolsPerOctet)
UINT8 radio_hal_pib_SHRDuration;//Duration of the synchronization header (SHR) in symbols for the current PHY.
UINT8 radio_hal_pib_SymbolsPerOctet;//Number of symbols per octet for the current PHY.
UINT8 radio_hal_pib_MaxBE;//The maximum value of the backoff exponent BE in the CSMA-CA algorithm.
UINT8 radio_hal_pib_MaxFrameRetries;//The maximum number of retries allowed after a transmission failure.
UINT8 radio_hal_pib_TransmitPower;//Default value of transmit power of transceiver  using IEEE defined format of phyTransmitPower.
UINT8 radio_hal_pib_CCAMode; //CCA Mode.
BOOL radio_hal_pib_PrivatePanCoordinator;//Indicates if the node is a PAN coordinator or not.
//Global TAL variables, These variables are only to be used by the TAL internally.
radio_hal_state_t radio_hal_state;//Current state of the TAL state machine.
radio_hal_trx_status_t radio_hal_trx_status; //Current state of the transceiver.




/* === PROTOTYPES ========================================================== */
BOOL radio_hal_init();
BOOL radio_init();
void radio_sram_read(UINT8 addr, UINT8 *data, UINT8 length);
void radio_sram_write(UINT8 addr, UINT8 *data, UINT8 length);
void radio_bit_write(UINT8 reg_addr, UINT8 mask, UINT8 pos, UINT8 new_value);
UINT8 radio_bit_read(UINT8 address, UINT8 mask, UINT8 pos);
void radio_frame_write(UINT8* data, UINT8 length);
void radio_frame_read(UINT8* data, UINT8 length);
UINT8 radio_reg_read(UINT8 address);
void radio_reg_write(UINT8 address, UINT8 data);
void write_all_default_to_radio();
void init_radio_default();
void radio_wakeup();
BOOL radio_sleep(sleep_mode_t sleep_mode);
void radio_config_csma();
void generate_rand_seed();
BOOL radio_config();
BOOL radio_reset();
BOOL internal_radio_reset(BOOL default_values, BOOL rand_seed);
BOOL radio_hal_init();
void radio_irq_init(void (*irq_handler)());
void radio_irq_handler();
void (*irq_handler)();

#endif /* RADIO_H */

		