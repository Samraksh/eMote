#ifndef _PMIC_DEFINES_H_
#define _PMIC_DEFINES_H_ 1
/**
 * @file: pmic_defines.h
 * @brief: definitions from Little Kernel and Android drivers.
 */

// Little Kernel
// dev/ssbi/include/dev/ssbi.h
#define SSBI_TIMEOUT_US                 100
#define PA1_SSBI2_CMD                   0x00500000
#define PA1_SSBI2_RD_STATUS             0x00500004
#define PA1_SSBI2_REG_ADDR_SHIFT        8
#define PA1_SSBI2_CMD_RDWRN_SHIFT       24
#define PA1_SSBI2_TRANS_DONE_SHIFT      27
#define PA1_SSBI2_REG_DATA_MASK         0xFF
#define PA1_SSBI2_REG_DATA_SHIFT        0
#define PA1_SSBI2_CMD_READ              1
#define PA1_SSBI2_CMD_WRITE             0

// dev/pmic/pm8921/pm8921_hw.h
#define PM8921_SLEEP_CTRL_REG           0x10A

// dev/pmic/pm8921/pm8921_hw.h
#define SLEEP_CTRL_SMPL_EN_MASK          0x04
#define SLEEP_CTRL_SMPL_EN_RESET         0x04
#define SLEEP_CTRL_SMPL_EN_PWR_OFF       0x00

// dev/pmic/pm8921/pm8921_hw.h
#define PM8921_PON_CTRL_1_REG            0x01C
#define PON_CTRL_1_PULL_UP_MASK          0xE0
#define PON_CTRL_1_USB_PWR_EN            0x10
#define PON_CTRL_1_WD_EN_MASK            0x08
#define PON_CTRL_1_WD_EN_RESET           0x08
#define PON_CTRL_1_WD_EN_PWR_OFF         0x00

#endif
