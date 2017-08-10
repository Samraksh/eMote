#ifndef _KRAIT_CLOCK_H_
#define _KRAIT_CLOCK_H_

#include "..\Krait_USART\gsbi.h"


/* NS/MD value for MMC */
#define SDC_CLK_NS_400KHZ    0x00440040
#define SDC_CLK_MD_400KHZ    0x00010043

#define SDC_CLK_NS_48MHZ     0x00FE005B
#define SDC_CLK_MD_48MHZ     0x000100FD

/* NS/MD value for UART */
#define UART_DM_CLK_NS_115200  0xFFE40040
#define UART_DM_CLK_MD_115200  0x0002FFE2




// This is the corresponding clock source structure in linux found in clocksource.h that the Krait_clock structure is
// mirroring
#if 0
struct clocksource {
	/*
	 * Hotpath data, fits in a single cache line when the
	 * clocksource itself is cacheline aligned.
	 */
	cycle_t (*read)(struct clocksource *cs);
	cycle_t cycle_last;
	cycle_t mask;
	u32 mult;
	u32 shift;
	u64 max_idle_ns;

#ifdef CONFIG_IA64
	void *fsys_mmio;        /* used by fsyscall asm code */
#define CLKSRC_FSYS_MMIO_SET(mmio, addr)      ((mmio) = (addr))
#else
#define CLKSRC_FSYS_MMIO_SET(mmio, addr)      do { } while (0)
#endif
	const char *name;
	struct list_head list;
	int rating;
	cycle_t (*vread)(void);
	int (*enable)(struct clocksource *cs);
	void (*disable)(struct clocksource *cs);
	unsigned long flags;
	void (*suspend)(struct clocksource *cs);
	void (*resume)(struct clocksource *cs);

#ifdef CONFIG_CLOCKSOURCE_WATCHDOG
	/* Watchdog related data, used by the framework */
	struct list_head wd_list;
	cycle_t cs_last;
	cycle_t wd_last;
#endif
} ____cacheline_aligned;


/**
 * struct clock_event_device - clock event device descriptor
 * @event_handler:	Assigned by the framework to be called by the low
 *			level handler of the event source
 * @set_next_event:	set next event function
 * @next_event:		local storage for the next event in oneshot mode
 * @max_delta_ns:	maximum delta value in ns
 * @min_delta_ns:	minimum delta value in ns
 * @mult:		nanosecond to cycles multiplier
 * @shift:		nanoseconds to cycles divisor (power of two)
 * @mode:		operating mode assigned by the management code
 * @features:		features
 * @retries:		number of forced programming retries
 * @set_mode:		set mode function
 * @broadcast:		function to broadcast events
 * @min_delta_ticks:	minimum delta value in ticks stored for reconfiguration
 * @max_delta_ticks:	maximum delta value in ticks stored for reconfiguration
 * @name:		ptr to clock event name
 * @rating:		variable to rate clock event devices
 * @irq:		IRQ number (only for non CPU local devices)
 * @cpumask:		cpumask to indicate for which CPUs this device works
 * @list:		list head for the management code
 */
struct clock_event_device {
	void			(*event_handler)(struct clock_event_device *);
	int			(*set_next_event)(unsigned long evt,
						  struct clock_event_device *);
	ktime_t			next_event;
	u64			max_delta_ns;
	u64			min_delta_ns;
	u32			mult;
	u32			shift;
	enum clock_event_mode	mode;
	unsigned int		features;
	unsigned long		retries;

	void			(*broadcast)(const struct cpumask *mask);
	void			(*set_mode)(enum clock_event_mode mode,
					    struct clock_event_device *);
	unsigned long		min_delta_ticks;
	unsigned long		max_delta_ticks;

	const char		*name;
	int			rating;
	int			irq;
	const struct cpumask	*cpumask;
	struct list_head	list;
} ____cacheline_aligned;

// Hijakced from ktime.h
union ktime {
	s64	tv64;
#if BITS_PER_LONG != 64 && !defined(CONFIG_KTIME_SCALAR)
	struct {
# ifdef __BIG_ENDIAN
	s32	sec, nsec;
# else
	s32	nsec, sec;
# endif
	} tv;
#endif
};

typedef union ktime ktime_t;		/* Kill this */

#endif

#define HALT		0	/* Bit pol: 1 = halted */
#define NOCHECK		1	/* No bit to check, do nothing */
#define HALT_VOTED	2	/* Bit pol: 1 = halted; delay on disable */
#define ENABLE		3	/* Bit pol: 1 = running */
#define ENABLE_VOTED	4	/* Bit pol: 1 = running; delay on disable */
#define DELAY		5	/* No bit to check, just delay */

union ktime {
	INT64	tv64;
#if BITS_PER_LONG != 64 && !defined(CONFIG_KTIME_SCALAR)
	struct {
# ifdef __BIG_ENDIAN
	s32	sec, nsec;
# else
	INT32	nsec, sec;
# endif
	} tv;
#endif
};

typedef union ktime ktime_t;		/* Kill this */

// Hijacked from clockchips.h
/* Clock event mode commands */
enum clock_event_mode {
	CLOCK_EVT_MODE_UNUSED = 0,
	CLOCK_EVT_MODE_SHUTDOWN,
	CLOCK_EVT_MODE_PERIODIC,
	CLOCK_EVT_MODE_ONESHOT,
	CLOCK_EVT_MODE_RESUME,
};

struct KraitClockEventDevice
{
	void (*event_handler)(struct KraitClockEventDevice *);
	INT32  (*set_next_event)(UINT64 evt, struct KraitClockEventDevice *);

	ktime_t			next_event;
	UINT64				max_delta_ns;
	UINT64			min_delta_ns;
	UINT32			mult;
	UINT32			shift;
	enum clock_event_mode	mode;
	UINT32		features;
	UINT64		retries;

	void			(*broadcast)(const struct cpumask *mask);
	void			(*set_mode)(enum clock_event_mode mode,
						    struct clock_event_device *);
	UINT64		min_delta_ticks;
	UINT64		max_delta_ticks;

	const char		*name;
	INT32			rating;
	INT32			irq;
	// Not sure what these attributes are designed for, will add them in when the answer is clear
	//const struct cpumask	*cpumask;
	//struct list_head	list;

};

// Unfortunately since we dont have access to a datasheet, there is no way to determine
// what the role of the individual items in this structure is
struct KraitClock
{
		UINT64 (*read)(struct KraitClock *cs);
		UINT64 cycle_last;
		UINT64 mask;
		UINT32 mult;
		UINT32 shift;
		UINT64 max_idle_ns;

#define CLKSRC_FSYS_MMIO_SET(mmio, addr)      do { } while (0)

		const char* name;
		// Unsure what the role of the list is
		// Commenting out for now, we dont have a list in MF atleast at the HAL level
		// struct list_head list;
		int rating;
		UINT64 (*vread)(void);
		INT32 (*enable)(struct KraitClock* cs);
		void (*disable)(struct KraitClock* cs);
		UINT64 flags;
		void (*suspend)(struct KraitClock *cs);
		void (*resume)(struct KraitClock *cs);

};


enum clk_reset_action {
	CLK_RESET_DEASSERT	= 0,
	CLK_RESET_ASSERT	= 1
};

// Generic clock structure definition

struct clk_ops {
	int (*enable)(struct clk *clk);
	void (*disable)(struct clk *clk);
	void (*auto_off)(struct clk *clk);
	void (*enable_hwcg)(struct clk *clk);
	void (*disable_hwcg)(struct clk *clk);
	int (*in_hwcg_mode)(struct clk *clk);
	int (*handoff)(struct clk *clk);
	int (*reset)(struct clk *clk, enum clk_reset_action action);
	int (*set_rate)(struct clk *clk, unsigned long rate);
	int (*set_max_rate)(struct clk *clk, unsigned long rate);
	int (*set_flags)(struct clk *clk, unsigned flags);
	unsigned long (*get_rate)(struct clk *clk);
	int (*list_rate)(struct clk *clk, unsigned n);
	int (*is_enabled)(struct clk *clk);
	long (*round_rate)(struct clk *clk, unsigned long rate);
	int (*set_parent)(struct clk *clk, struct clk *parent);
	struct clk *(*get_parent)(struct clk *clk);
	bool (*is_local)(struct clk *clk);
};


// Removing some of the attributes because a lot of support needed like linked list is not available here
struct clk {
	UINT32 flags;
	struct clk_ops *ops;
	const char *dbg_name;
	//struct clk *depends;
	//struct clk_vdd_class *vdd_class;
	//unsigned long fmax[MAX_VDD_LEVELS];
	unsigned long rate;

	//struct list_head children;
	//struct list_head siblings;
#ifdef CONFIG_CLOCK_MAP
	unsigned id;
#endif

	unsigned count;
	//spinlock_t lock;
};


struct branch {
	void *ctl_reg;
	UINT32 en_mask;

	void *hwcg_reg;
	UINT32 hwcg_mask;

	void *halt_reg;
	UINT16 halt_check;
	UINT16 halt_bit;

	void *reset_reg;
	UINT32 reset_mask;

	void *retain_reg;
	UINT32 retain_mask;
};

struct clk_freq_tbl {
	UINT32	freq_hz;
	struct clk	*src_clk;
	UINT32	md_val;
	UINT32	ns_val;
	UINT32	ctl_val;
	UINT32	mnd_en_mask;
	unsigned	sys_vdd;
	void		*extra_freq_data;
};

struct rcg_clk {
	BOOL		enabled;
	void		*ns_reg;
	void		*md_reg;

	UINT32	root_en_mask;
	UINT32	ns_mask;
	UINT32	ctl_mask;

	void		*bank_info;
	void   (*set_rate)(struct rcg_clk *, struct clk_freq_tbl *);

	struct clk_freq_tbl *freq_tbl;
	struct clk_freq_tbl *current_freq;

	struct branch	b;
	struct clk	c;
};


/**
 * struct clk
 * @count: enable refcount
 * @lock: protects clk_enable()/clk_disable() path and @count
 * @depends: non-direct parent of clock to enable when this clock is enabled
 * @vdd_class: voltage scaling requirement class
 * @fmax: maximum frequency in Hz supported at each voltage level
 */

#define BVAL(msb, lsb, val)	(((val) << lsb) & BM(msb, lsb))

#define NS(n_msb, n_lsb, n, m, mde_lsb, d_msb, d_lsb, d, s_msb, s_lsb, s) \
		(BVAL(n_msb, n_lsb, ~(n-m)) \
		| (BVAL((mde_lsb+1), mde_lsb, MN_MODE_DUAL_EDGE) * !!(n)) \
		| BVAL(d_msb, d_lsb, (d-1)) | BVAL(s_msb, s_lsb, s))


#endif
