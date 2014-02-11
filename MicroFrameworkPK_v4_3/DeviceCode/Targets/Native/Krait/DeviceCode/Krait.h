/*
 *  Name : Krait.h
 *
 *  Author : {Mukundan.Sridharan,Nived.Sivadas}@Samraksh.com
 *
 *  Description : Contains structures of all drivers for the Krait
 *
 */

#ifndef _KRAIT_H_
#define _KRAIT_H_

#include <tinyhal.h>


#define	ETIME		62	/* Timer expired */

typedef UINT32 uint32_t;
typedef UINT8 uint8_t;
//from include/sys/types.h

#define __ALIGNED(x) __attribute__((aligned(x)))

//typedef uchar u_char;
typedef ushort u_short;
typedef uint u_int;
//typedef ulong u_long;

#ifndef _SIZE_T_DEFINED_
typedef unsigned long size_t;
#endif
//typedef long          ssize_t;
//typedef long long     off_t;

typedef int status_t;

typedef unsigned long uintptr_t;
typedef uintptr_t addr_t;

//#define ASSERT(x) if(!x){ while(1); }
#define ASSERT(x)

//typedef ulong intptr_t;
//typedef uintptr_t addr_t;
//typedef uintptr_t vaddr_t;
//typedef uintptr_t paddr_t;

typedef int kobj_id;

# define __user
# define __kernel
# define __safe
# define __force
# define __nocast
# define __iomem

//typedef ulong time_t;
//typedef unsigned long long bigtime_t;
//typedef struct cpumask { DECLARE_BITMAP(bits, NR_CPUS); } cpumask_t;

#define INFINITE_TIME ULONG_MAX

#define ARRAY_SIZE(x) (sizeof(x)/sizeof((x)[0]))

#define TIME_GTE(a, b) ((long)((a) - (b)) >= 0)
#define TIME_LTE(a, b) ((long)((a) - (b)) <= 0)
#define TIME_GT(a, b) ((long)((a) - (b)) > 0)
#define TIME_LT(a, b) ((long)((a) - (b)) < 0)

////Below taken from platfrom/msm8690/include/platform/iomap.h
#define MSM_IOMAP_BASE      0x00100000
#define MSM_IOMAP_END       0x28000000

#define MSM_IMEM_BASE       0x2A000000

#define MSM_SHARED_IMEM_BASE 0x2A03F000
#define RESTART_REASON_ADDR  (MSM_SHARED_IMEM_BASE + 0x65C)

#define MSM_SHARED_BASE     0x80000000

#define MSM_TCSR_BASE       0x1A400000
#define MSM_GIC_DIST_BASE   0x02000000
#define MSM_TMR_BASE        0x0200A000
#define MSM_GPT_BASE        (MSM_TMR_BASE + 0x04)
#define MSM_DGT_BASE        (MSM_TMR_BASE + 0x24)
#define SPSS_TIMER_STATUS   (MSM_TMR_BASE + 0x88)

#define GPT_REG(off)        (MSM_GPT_BASE + (off))
#define DGT_REG(off)        (MSM_DGT_BASE + (off))

#define GPT_MATCH_VAL        GPT_REG(0x0000)
#define GPT_COUNT_VAL        GPT_REG(0x0004)
#define GPT_ENABLE           GPT_REG(0x0008)
#define GPT_CLEAR            GPT_REG(0x000C)

#define DGT_MATCH_VAL        DGT_REG(0x0000)
#define DGT_COUNT_VAL        DGT_REG(0x0004)
#define DGT_ENABLE           DGT_REG(0x0008)
#define DGT_CLEAR            DGT_REG(0x000C)
#define DGT_CLK_CTL          DGT_REG(0x0010)

#define MSM_GIC_CPU_BASE    0x02002000
#define MSM_VIC_BASE        0x02080000
#define MSM_USB_BASE        0x12500000
#define TLMM_BASE_ADDR      0x00800000

#define TCSR_WDOG_CFG       0x30
#define MSM_WDT0_RST        (MSM_TMR_BASE + 0x38)
#define MSM_WDT0_EN         (MSM_TMR_BASE + 0x40)
#define MSM_WDT0_BT         (MSM_TMR_BASE + 0x4C)
#define MSM_PSHOLD_CTL_SU   (TLMM_BASE_ADDR + 0x820)

#define MSM_SDC1_BASE       0x12400000
#define MSM_SDC2_BASE       0x12140000
#define MSM_SDC3_BASE       0x12180000
#define MSM_SDC4_BASE       0x121C0000

#define GPIO_CONFIG_ADDR(x) (TLMM_BASE_ADDR + 0x1000 + (x)*0x10)
#define GPIO_IN_OUT_ADDR(x) (TLMM_BASE_ADDR + 0x1004 + (x)*0x10)

// Turns out GSBI_BASE and QUP_BASE are lies. Check with datasheet.
#define GSBI_BASE(id)         ((id) <= 7 ? (0x16000000 + (((id)-1) << 20)) : \
                                           (0x1A000000 + (((id)-8) << 20)))
#define GSBI_UART_DM_BASE(id) (GSBI_BASE(id) + 0x40000)
#define QUP_BASE(id)          (GSBI_BASE(id) + 0x80000)

#define EBI2_CHIP_SELECT_CFG0   0x1A100000
#define EBI2_XMEM_CS3_CFG1      0x1A110034

#define CLK_CTL_BASE             0x00900000
#define SDC_MD(n)               (CLK_CTL_BASE + 0x2828 + (32 * ((n) - 1)))
#define SDC_NS(n)               (CLK_CTL_BASE + 0x282C + (32 * ((n) - 1)))
#define USB_HS1_HCLK_CTL        (CLK_CTL_BASE + 0x2900)
#define USB_HS1_XCVR_FS_CLK_MD  (CLK_CTL_BASE + 0x2908)
#define USB_HS1_XCVR_FS_CLK_NS  (CLK_CTL_BASE + 0x290C)
#define GSBIn_HCLK_CTL(n)       (CLK_CTL_BASE + 0x29C0 + (32 * ((n) - 1)))
#define GSBIn_HCLK_FS(n)        (CLK_CTL_BASE + 0x29C4 + (32 * ((n) - 1)))
#define GSBIn_QUP_APPS_MD(n)    (CLK_CTL_BASE + 0x29C8 + (32 * ((n) - 1)))
#define GSBIn_QUP_APPS_NS(n)    (CLK_CTL_BASE + 0x29CC + (32 * ((n) - 1)))
#define GSBIn_UART_APPS_MD(n)   (CLK_CTL_BASE + 0x29D0 + (32 * ((n) - 1)))
#define GSBIn_UART_APPS_NS(n)   (CLK_CTL_BASE + 0x29D4 + (32 * ((n) - 1)))
#define MSM_BOOT_PLL8_STATUS    (CLK_CTL_BASE + 0x3158)
#define MSM_BOOT_PLL_ENABLE_SC0 (CLK_CTL_BASE + 0x34C0)
#define CE1_HCLK_CTL            (CLK_CTL_BASE + 0x2720)
#define CE1_CORE_CLK_CTL        (CLK_CTL_BASE + 0x2724)

#define MSM_MMSS_CLK_CTL_BASE 0x04000000

#define MIPI_DSI_BASE                         (0x04700000)
#define REG_DSI(off)                          (MIPI_DSI_BASE + (off))

#define DSIPHY_REGULATOR_BASE                 (0x500)
#define DSIPHY_TIMING_BASE                    (0x440)
#define DSIPHY_CTRL_BASE                      (0x470)
#define DSIPHY_PLL_BASE                       (0x200)
#define DSIPHY_STRENGTH_BASE                  (0x480)

/* Range 0 - 4 */
#define DSIPHY_REGULATOR_CTRL(x) REG_DSI(DSIPHY_REGULATOR_BASE + (x) * 4)
/* Range 0 - 11 */
#define DSIPHY_TIMING_CTRL(x)    REG_DSI(DSIPHY_TIMING_BASE + (x) * 4)
/* Range 0 - 3 */
#define DSIPHY_CTRL(x)           REG_DSI(DSIPHY_CTRL_BASE + (x) * 4)
/* Range 0 - 2 */
#define DSIPHY_STRENGTH_CTRL(x)  REG_DSI(DSIPHY_STRENGTH_BASE + (x) * 4)
/* Range 0 - 19 */
#define DSIPHY_PLL_CTRL(x)       REG_DSI(DSIPHY_PLL_BASE + (x) * 4)

#define MDP_BASE                              (0x05100000)
#define REG_MDP(off)                          (MDP_BASE + (off))

//TODO: Where does this go?
#define MMSS_SFPB_GPREG                       (0x05700058)

#define CE1_CRYPTO4_BASE                      (0x18500000)
#define MSM_CRYPTO_BASE                       CE1_CRYPTO4_BASE

#define container_of(ptr, type, member) ({			\
	const typeof(((type *)0)->member) * __mptr = (ptr);	\
	(type *)((char *)__mptr - offsetof(type, member)); })



//from include/reg.h
/* low level macros for accessing memory mapped hardware registers */
#define REG64(addr) ((volatile UINT64 *)(addr))
#define REG32(addr) ((volatile UINT32 *)(addr))
#define REG16(addr) ((volatile UINT16 *)(addr))
#define REG8(addr) ((volatile UINT8 *)(addr))

#define RMWREG64(addr, startbit, width, val) *REG64(addr) = (*REG64(addr) & ~(((1<<(width)) - 1) << (startbit))) | ((val) << (startbit))
#define RMWREG32(addr, startbit, width, val) *REG32(addr) = (*REG32(addr) & ~(((1<<(width)) - 1) << (startbit))) | ((val) << (startbit))
#define RMWREG16(addr, startbit, width, val) *REG16(addr) = (*REG16(addr) & ~(((1<<(width)) - 1) << (startbit))) | ((val) << (startbit))
#define RMWREG8(addr, startbit, width, val) *REG8(addr) = (*REG8(addr) & ~(((1<<(width)) - 1) << (startbit))) | ((val) << (startbit))

#define writel(v, a) (*REG32(a) = (v))
#define readl(a) (*REG32(a))

#define writeb(v, a) (*REG8(a) = (v))
#define readb(a) (*REG8(a))

#define __raw_writeb writeb
#define __raw_writew writew
#define __raw_writel writel

#define VERIFY_PERCPU_PTR(__p) ({			\
	__verify_pcpu_ptr((__p));			\
	(typeof(*(__p)) __kernel __force *)(__p);	\
})

#define per_cpu(var, cpu)	(*((void)(cpu), VERIFY_PERCPU_PTR(&(var))))
#define __get_cpu_var(var)	(*VERIFY_PERCPU_PTR(&(var)))
#define __raw_get_cpu_var(var)	(*VERIFY_PERCPU_PTR(&(var)))
#define this_cpu_ptr(ptr)	per_cpu_ptr(ptr, 0)
#define __this_cpu_ptr(ptr)	this_cpu_ptr(ptr)

# define __chk_io_ptr(x) (void)0

#define __raw_readb_no_log(a)		(__chk_io_ptr(a), *(volatile unsigned char __force  *)(a))
#define __raw_readw_no_log(a)		(__chk_io_ptr(a), *(volatile unsigned short __force *)(a))
#define __raw_readl_no_log(a)		(__chk_io_ptr(a), *(volatile unsigned int __force *)(a))

//extern void __chk_user_ptr(const volatile void __user *);
//extern void __chk_io_ptr(const volatile void __iomem *);

static inline UINT32 __raw_readl(const volatile void __iomem *addr)
{
	return *(__force volatile UINT32 *)addr;
}

#define MSM_GCC_BASE	0x02082000

#define mb() __asm__ __volatile__ ("": : :"memory")

#define IO_CONCAT(a,b)	_IO_CONCAT(a,b)
#define _IO_CONCAT(a,b)	a ## _ ## b



enum handler_return {
	INT_NO_RESCHEDULE = 0,
	INT_RESCHEDULE,
};

#endif
