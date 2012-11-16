#ifndef _KRAIT__LCD_H_
#define _KRAIT__LCD_H_

// Clock
#define ESC_NS_VAL            0x00001000
#define ESC_CC_VAL            0x00000004

#define MDP_NS_VAL            0x3F3FC008
#define MDP_MD_VAL            0x000001FB
#define MDP_CC_VAL            0x00000500

#define MDP_LUT_VAL           0x00000001

#define DSI_NS_VAL            0xFA000003
#define DSI_MD_VAL            0x000003FB
#define DSI_CC_VAL            0x00000080

#define BYTE_NS_VAL           0x00000001
#define BYTE_CC_VAL           0x00000004

#define MSM_MMSS_CLK_CTL_BASE 0x04000000

#define REG_MM(off)     (MSM_MMSS_CLK_CTL_BASE + (off))

#define MM_PLL1_MODE_REG        REG_MM(0x031C)
#define MM_PLL1_STATUS_REG      REG_MM(0x0334)
#define MM_PLL1_CONFIG_REG      REG_MM(0x032C)

#define MDP_CC_REG              REG_MM(0xC0)
#define MDP_MD_REG              REG_MM(0xC4)
#define MDP_NS_REG              REG_MM(0xD0)

#define MDP_LUT_CC_REG          REG_MM(0x016C)

#define ESC_CC_REG              REG_MM(0xCC)
#define ESC_NS_REG              REG_MM(0x11C)

#define BYTE_CC_REG             REG_MM(0x90)
#define BYTE_NS_REG             REG_MM(0xB0)


#define PIXEL_NS_VAL          0x00F80003
#define PIXEL_MD_VAL          0x000001FB
#define PIXEL_CC_VAL          0x00000080

#define PIXEL_CC_REG            REG_MM(0x130)
#define PIXEL_MD_REG            REG_MM(0x134)
#define PIXEL_NS_REG            REG_MM(0x138)
#define DSI_NS_REG              REG_MM(0x54)
#define DSI_MD_REG              REG_MM(0x50)
#define DSI_CC_REG              REG_MM(0x4C)


// end of clock related stuff

/* Bit offsets LDO Test register */
#define PM8921_LDO_TEST_REG_BANK_SEL             4
#define PM8921_LDO_TEST_REG_RW                   7
#define PM8921_LDO_TEST_REG_BANK2_RANGE_SEL      2
#define PM8921_LDO_TEST_REG_BANK2_FINE_STEP      1
#define PM8921_LDO_TEST_REG_BANK4_RANGE_EXT      0

#define PM_GPIO_FUNC_NORMAL     0
#define PM_GPIO_FUNC_PAIRED     1
#define PM_GPIO_FUNC_1          2
#define PM_GPIO_FUNC_2          3


#define PM_GPIO_PULL_UP_30      0
#define PM_GPIO_PULL_UP_31_5    2

#define PM8921_LDO_CTRL_REG_ENABLE        7
#define PM8921_LDO_CTRL_REG_PULL_DOWN     6
#define PM8921_LDO_CTRL_REG_POWER_MODE    5
#define PM8921_LDO_CTRL_REG_VOLTAGE       0

#define PM8921_LDO_REG_BASE                   0x0AE
#define PM8921_LDO_CTRL_REG(id)               (PM8921_LDO_REG_BASE + (2 * (id-1)))
#define PM8921_LDO_TEST_REG(id)               (PM8921_LDO_CTRL_REG(id) + 1)

#define PM_GPIO_MODE_OFF                 3
#define PM_GPIO_MODE_OUTPUT              2
#define PM_GPIO_MODE_INPUT               0
#define PM_GPIO_MODE_BOTH                1

#define PM_GPIO_VIN_MASK                 0x0E
#define PM_GPIO_VIN_SHIFT                1
#define PM_GPIO_MODE_ENABLE              0x01

#define PM_GPIO(_x)                    ((_x) - 1)

#define PM_GPIO_DIR_OUT         0x01
#define PM_GPIO_DIR_IN          0x02
#define PM_GPIO_PULL_NO         5
#define PM_GPIO_STRENGTH_HIGH   1
#define PM_GPIO_FUNC_1          2


#define PM_GPIO_BANK_MASK                0x70
#define PM_GPIO_BANK_SHIFT               4
#define PM_GPIO_WRITE                    0x80

#define PM_GPIO_MODE_MASK                0x0C
#define PM_GPIO_MODE_SHIFT               2
#define PM_GPIO_OUT_BUFFER_OPEN_DRAIN    0x02
#define PM_GPIO_OUT_INVERT               0x01

#define PM_GPIO_PULL_MASK                0x0E
#define PM_GPIO_PULL_SHIFT               1

#define PM_GPIO_OUT_STRENGTH_MASK        0x0C
#define PM_GPIO_OUT_STRENGTH_SHIFT       2
#define PM_GPIO_PIN_ENABLE               0x00
#define PM_GPIO_PIN_DISABLE              0x01

#define PM_GPIO_FUNC_MASK                0x0E
#define PM_GPIO_FUNC_SHIFT               1

#define PM_GPIO_NON_INT_POL_INV          0x08

#define GPIO_CNTL_BASE                        0x150
#define GPIO_CNTL(n)                          (GPIO_CNTL_BASE + n)

#define SSBI_TIMEOUT_US			100

#define PA1_SSBI2_REG_ADDR_SHIFT        8
#define PA1_SSBI2_CMD_RDWRN_SHIFT       24
#define PA1_SSBI2_TRANS_DONE_SHIFT      27

#define PA1_SSBI2_CMD_READ              1
#define PA1_SSBI2_CMD_WRITE             0

#define PA1_SSBI2_REG_DATA_MASK         0xFF
#define PA1_SSBI2_REG_DATA_SHIFT        0

typedef int (*pm8921_read_func)(UINT8 *data, UINT32 length, UINT32 addr);
typedef int (*pm8921_write_func)(UINT8 *data, UINT32 length, UINT32 addr);

#define PA1_SSBI2_CMD                   0x00500000
#define PA1_SSBI2_RD_STATUS             0x00500004



#endif
