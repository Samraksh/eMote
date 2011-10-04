#include <tinyhal.h>
#include "stm32f10x.h"
#include <spi/stm32f10x_spi.h>
#include "spi/netmf_spi.h"
#include "rtc/stm32f10x_rtc.h"
#include "SPOT_Hardware.h"
#include "misc/misc.h"
#include "exti/stm32f10x_exti.h"
#include "pwr/stm32f10x_pwr.h"
#include "bkp/stm32f10x_bkp.h"

#define nx_struct struct
#define nx_union union
#define dbg(mode, format, ...) ((void)0)
#define dbg_clear(mode, format, ...) ((void)0)
#define dbg_active(mode) 0
//# 41 "/home/nived/CodeSourcery/Sourcery_G++_Lite/bin/../lib/gcc/arm-none-eabi/4.5.1/../../../../arm-none-eabi/include/stdint.h" 3
typedef signed char int8_t;
typedef unsigned char uint8_t;

typedef signed char int_least8_t;
typedef unsigned char uint_least8_t;

typedef signed short int16_t;
typedef unsigned short uint16_t;
//#line 67
typedef int16_t int_least16_t;
typedef uint16_t uint_least16_t;

typedef signed long int32_t;
typedef unsigned long uint32_t;
//#line 97
typedef int32_t int_least32_t;
typedef uint32_t uint_least32_t;
//#line 119
typedef signed long long int64_t;
typedef unsigned long long uint64_t;

typedef int64_t int_least64_t;
typedef uint64_t uint_least64_t;
//#line 159
typedef signed int int_fast8_t;
typedef unsigned int uint_fast8_t;

typedef signed int int_fast16_t;
typedef unsigned int uint_fast16_t;

typedef signed int int_fast32_t;
typedef unsigned int uint_fast32_t;
//#line 213
typedef int_least64_t int_fast64_t;
typedef uint_least64_t uint_fast64_t;

typedef long long int intmax_t;

typedef long long unsigned int uintmax_t;
//#line 243
typedef signed int intptr_t;
typedef unsigned int uintptr_t;
////# 323 "/home/nived/CodeSourcery/Sourcery_G++_Lite/bin/../lib/gcc/arm-none-eabi/4.5.1/include/stddef.h" 3
//typedef unsigned int wchar_t;
//# 274 "/home/nived/CodeSourcery/Sourcery_G++_Lite/bin/../lib/gcc/arm-none-eabi/4.5.1/../../../../arm-none-eabi/include/inttypes.h"
//#line 271
typedef struct __nesc_unnamed4242 {
	intmax_t quot;
	intmax_t rem;
} imaxdiv_t;
//# 385 "/usr/lib/ncc/nesc_nx.h"
typedef struct {
	char data[1];
}__attribute__((packed)) nx_int8_t;
typedef int8_t __nesc_nxbase_nx_int8_t;
typedef struct {
	char data[2];
}__attribute__((packed)) nx_int16_t;
typedef int16_t __nesc_nxbase_nx_int16_t;
typedef struct {
	char data[4];
}__attribute__((packed)) nx_int32_t;
typedef int32_t __nesc_nxbase_nx_int32_t;
typedef struct {
	char data[8];
}__attribute__((packed)) nx_int64_t;
typedef int64_t __nesc_nxbase_nx_int64_t;
typedef struct {
	char data[1];
}__attribute__((packed)) nx_uint8_t;
typedef uint8_t __nesc_nxbase_nx_uint8_t;
typedef struct {
	char data[2];
}__attribute__((packed)) nx_uint16_t;
typedef uint16_t __nesc_nxbase_nx_uint16_t;
typedef struct {
	char data[4];
}__attribute__((packed)) nx_uint32_t;
typedef uint32_t __nesc_nxbase_nx_uint32_t;
typedef struct {
	char data[8];
}__attribute__((packed)) nx_uint64_t;
typedef uint64_t __nesc_nxbase_nx_uint64_t;

typedef struct {
	char data[1];
}__attribute__((packed)) nxle_int8_t;
typedef int8_t __nesc_nxbase_nxle_int8_t;
typedef struct {
	char data[2];
}__attribute__((packed)) nxle_int16_t;
typedef int16_t __nesc_nxbase_nxle_int16_t;
typedef struct {
	char data[4];
}__attribute__((packed)) nxle_int32_t;
typedef int32_t __nesc_nxbase_nxle_int32_t;
typedef struct {
	char data[8];
}__attribute__((packed)) nxle_int64_t;
typedef int64_t __nesc_nxbase_nxle_int64_t;
typedef struct {
	char data[1];
}__attribute__((packed)) nxle_uint8_t;
typedef uint8_t __nesc_nxbase_nxle_uint8_t;
typedef struct {
	char data[2];
}__attribute__((packed)) nxle_uint16_t;
typedef uint16_t __nesc_nxbase_nxle_uint16_t;
typedef struct {
	char data[4];
}__attribute__((packed)) nxle_uint32_t;
typedef uint32_t __nesc_nxbase_nxle_uint32_t;
typedef struct {
	char data[8];
}__attribute__((packed)) nxle_uint64_t;
typedef uint64_t __nesc_nxbase_nxle_uint64_t;
//# 26 "/home/nived/CodeSourcery/Sourcery_G++_Lite/bin/../lib/gcc/arm-none-eabi/4.5.1/../../../../arm-none-eabi/include/machine/_default_types.h"
typedef signed char __int8_t;
typedef unsigned char __uint8_t;

typedef signed short __int16_t;
typedef unsigned short __uint16_t;

typedef __int16_t  __int_least16_t; typedef __uint16_t  __uint_least16_t;//# 58 "/home/nived/CodeSourcery/Sourcery_G++_Lite/bin/../lib/gcc/arm-none-eabi/4.5.1/../../../../arm-none-eabi/include/machine/_default_types.h" 3
typedef signedint __int32_t;
typedef unsignedint __uint32_t;
//#line 76
typedef __int32_t __int_least32_t;
typedef __uint32_t __uint_least32_t;
//#line 99
typedef signedlong long __int64_t;
typedef unsignedlong long __uint64_t;
//# 6 "/home/nived/CodeSourcery/Sourcery_G++_Lite/bin/../lib/gcc/arm-none-eabi/4.5.1/../../../../arm-none-eabi/include/sys/lock.h"
typedef int _LOCK_T;
typedef int _LOCK_RECURSIVE_T;
//# 16 "/home/nived/CodeSourcery/Sourcery_G++_Lite/bin/../lib/gcc/arm-none-eabi/4.5.1/../../../../arm-none-eabi/include/sys/_types.h"
typedef long _off_t;

typedef short __dev_t;

typedef unsigned short __uid_t;

typedef unsigned short __gid_t;

__extension__
//#line 36
typedef long long _off64_t;

typedef long _fpos_t;
//# 56 "/home/nived/CodeSourcery/Sourcery_G++_Lite/bin/../lib/gcc/arm-none-eabi/4.5.1/../../../../arm-none-eabi/include/sys/_types.h" 3
typedef int _ssize_t;
//# 352 "/home/nived/CodeSourcery/Sourcery_G++_Lite/bin/../lib/gcc/arm-none-eabi/4.5.1/include/stddef.h" 3
typedef unsigned int wint_t;
//# 75 "/home/nived/CodeSourcery/Sourcery_G++_Lite/bin/../lib/gcc/arm-none-eabi/4.5.1/../../../../arm-none-eabi/include/sys/_types.h"
//#line 67

/*
 typedef struct __nesc_unnamed4243 {

 int __count;
 union __nesc_unnamed4244 {

 wint_t __wch;
 unsigned char __wchb[4];
 } __value;
 } _mbstate_t;

 */

typedef _LOCK_RECURSIVE_T _flock_t;

typedef void *_iconv_t;
//# 21 "/home/nived/CodeSourcery/Sourcery_G++_Lite/bin/../lib/gcc/arm-none-eabi/4.5.1/../../../../arm-none-eabi/include/sys/reent.h"
typedef unsignedlong __ULong;
//# 37 "/home/nived/CodeSourcery/Sourcery_G++_Lite/bin/../lib/gcc/arm-none-eabi/4.5.1/../../../../arm-none-eabi/include/sys/reent.h" 3
struct_reent;
/*
 struct _Bigint {

 struct _Bigint *_next;
 int _k, _maxwds, _sign, _wds;
 __ULong _x[1];
 };
 */
/*
 struct __tm {

 int __tm_sec;
 int __tm_min;
 int __tm_hour;
 int __tm_mday;
 int __tm_mon;
 int __tm_year;
 int __tm_wday;
 int __tm_yday;
 int __tm_isdst;
 };







 struct _on_exit_args {
 void *_fnargs[32];
 void *_dso_handle[32];

 __ULong _fntypes;


 __ULong _is_cxa;
 };


 struct _atexit {
 struct _atexit *_next;
 int _ind;
 void (*_fns[32])(void );
 struct _on_exit_args *_on_exit_args_ptr;
 };
 //#line 105
 struct __sbuf {
 unsigned char *_base;
 int _size;
 };
 //#line 141
 struct __sFILE_fake {
 unsigned char *_p;
 int _r;
 int _w;
 short _flags;
 short _file;
 struct __sbuf _bf;
 int _lbfsize;

 struct _reent *_data;
 };
 //#line 179
 struct __sFILE {
 unsigned char *_p;
 int _r;
 int _w;
 short _flags;
 short _file;
 struct __sbuf _bf;
 int _lbfsize;


 struct _reent *_data;



 void *_cookie;

 int (*_read)(struct _reent *, void *, char *, int );

 int (*_write)(struct _reent *, void *, const char *, int );

 _fpos_t (*_seek)(struct _reent *, void *, _fpos_t , int );
 int (*_close)(struct _reent *, void *);


 struct __sbuf _ub;
 unsigned char *_up;
 int _ur;


 unsigned char _ubuf[3];
 unsigned char _nbuf[1];


 struct __sbuf _lb;


 int _blksize;
 int _offset;






 _flock_t _lock;

 _mbstate_t _mbstate;
 int _flags2;
 };
 //#line 286
 typedef struct __sFILE __FILE;



 struct _glue {

 struct _glue *_next;
 int _niobs;
 __FILE *_iobs;
 };
 //#line 318
 struct _rand48 {
 unsigned short _seed[3];
 unsigned short _mult[3];
 unsigned short _add;


 __extension__ unsigned long long _rand_next;
 };
 //#line 351
 struct _mprec {


 struct _Bigint *_result;
 int _result_k;
 struct _Bigint *_p5s;
 struct _Bigint **_freelist;
 };


 struct _misc_reent {


 char *_strtok_last;
 _mbstate_t _mblen_state;
 _mbstate_t _wctomb_state;
 _mbstate_t _mbtowc_state;
 char _l64a_buf[8];
 int _getdate_err;
 _mbstate_t _mbrlen_state;
 _mbstate_t _mbrtowc_state;
 _mbstate_t _mbsrtowcs_state;
 _mbstate_t _wcrtomb_state;
 _mbstate_t _wcsrtombs_state;
 };



 struct _reent {



 int _errno;




 __FILE *_stdin, *_stdout, *_stderr;

 int _inc;

 char *_emergency;

 int __sdidinit;

 int _current_category;
 const char *_current_locale;

 struct _mprec *_mp;

 void (*__cleanup)(struct _reent *);

 int _gamma_signgam;


 int _cvtlen;
 char *_cvtbuf;

 struct _rand48 *_r48;
 struct __tm *_localtime_buf;
 char *_asctime_buf;


 void (**_sig_func)(int );


 struct _atexit *_atexit;
 struct _atexit _atexit0;

 struct _glue __sglue;
 __FILE *__sf;
 struct _misc_reent *_misc;
 char *_signal_buf;
 };

 struct __sFILE_fake;
 struct __sFILE_fake;
 struct __sFILE_fake;
 //#line 840
 struct _reent;
 struct _reent;
 //# 211 "/home/nived/CodeSourcery/Sourcery_G++_Lite/bin/../lib/gcc/arm-none-eabi/4.5.1/include/stddef.h" 3
 typedef unsigned int size_t;
 //# 26 "/home/nived/CodeSourcery/Sourcery_G++_Lite/bin/../lib/gcc/arm-none-eabi/4.5.1/../../../../arm-none-eabi/include/string.h"
 void *memset(void *, int , size_t );
 //# 38 "/home/nived/CodeSourcery/Sourcery_G++_Lite/bin/../lib/gcc/arm-none-eabi/4.5.1/../../../../arm-none-eabi/include/stdlib.h"
 //#line 34
 typedef struct __nesc_unnamed4245 {

 int quot;
 int rem;
 } div_t;





 //#line 40
 typedef struct __nesc_unnamed4246 {

 long quot;
 long rem;
 } ldiv_t;






 //#line 47
 typedef struct __nesc_unnamed4247 {

 long long int quot;
 long long int rem;
 } lldiv_t;
 //# 28 "/home/nived/CodeSourcery/Sourcery_G++_Lite/bin/../lib/gcc/arm-none-eabi/4.5.1/include-fixed/math.h"
 union __dmath {

 double d;
 __ULong i[2];
 };

 union __fmath {

 float f;
 __ULong i[1];
 };


 union __ldmath {

 long double ld;
 __ULong i[4];
 };
 //# 162 "/home/nived/CodeSourcery/Sourcery_G++_Lite/bin/../lib/gcc/arm-none-eabi/4.5.1/include-fixed/math.h" 3
 typedef float float_t;
 typedef double double_t;
 //#line 515
 struct exception {


 int type;
 char *name;
 double arg1;
 double arg2;
 double retval;
 int err;
 };
 //#line 570
 enum __fdlibm_version {

 __fdlibm_ieee = -1,
 __fdlibm_svid,
 __fdlibm_xopen,
 __fdlibm_posix
 };




 enum __fdlibm_version;

 */
//# 149 "/home/nived/CodeSourcery/Sourcery_G++_Lite/bin/../lib/gcc/arm-none-eabi/4.5.1/include/stddef.h" 3
typedefint ptrdiff_t;
////# 40 "/opt/tinyos-2.x/tos/system/../lib/safe/include/annots_stage1.h"
struct __nesc_attr_safe {
};
//#line 41
struct __nesc_attr_unsafe {
};
////# 23 "/opt/tinyos-2.x/tos/system/tos.h"
//typedef uint8_t bool;
/*
 enum __nesc_unnamed4248 {
 //#line 24
 FALSE = 0, TRUE = 1
 };
 */

typedef nx_int8_t nx_bool;

struct __nesc_attr_atmostonce {
};
//#line 35
struct __nesc_attr_atleastonce {
};
//#line 36
struct __nesc_attr_exactlyonce {
};
//# 40 "/opt/tinyos-2.x/tos/types/TinyError.h"


enum __nesc_unnamed4249 {
	//  SUCCESS = 0,
	FAIL = 1,
	ESIZE = 2,
	ECANCEL = 3,
	EOFF = 4,
	EBUSY = 5,
	EINVAL = 6,
	ERETRY = 7,
	ERESERVE = 8,
	EALREADY = 9,
	ENOMEM = 10,
	ENOACK = 11,
	ELAST = 11
};

typedef uint8_t error_t;

typedef uint32_t __nesc_atomic_t;

// pOINT 0

enum __nesc_unnamed4266 {
	ATM128_CLK16_OFF = 0x0,
	ATM128_CLK16_NORMAL = 0x1,
	ATM128_CLK16_DIVIDE_8 = 0x2,
	ATM128_CLK16_DIVIDE_64 = 0x3,
	ATM128_CLK16_DIVIDE_256 = 0x4,
	ATM128_CLK16_DIVIDE_1024 = 0x5,
	ATM128_CLK16_EXTERNAL_FALL = 0x6,
	ATM128_CLK16_EXTERNAL_RISE = 0x7
};

__inline  __nesc_atomic_t  __nesc_atomic_start(void )  ;
//#line 56
__inline void __nesc_atomic_end(__nesc_atomic_t oldState);
//#line 75
static __inline void __nesc_enable_interrupt(void);
//#line 89
static __inline void __nesc_disable_interrupt(void);
//#line 105
typedef uint8_t mcu_power_t;
//# 29 "/opt/tinyos-2.x/tos/lib/timer/Timer.h"
typedef struct __nesc_unnamed4304 {
	//#line 29
	int notUsed;
}
//#line 29
TMilli;
typedef struct __nesc_unnamed4305 {
	//#line 30
	int notUsed;
}
//#line 30
T32khz;
typedef struct __nesc_unnamed4306_T {
	//#line 31
	int notUsed;
}
//#line 31
TMicro;

typedef TMicro TOne;
typedef TOne TRadio;

static inline error_t ecombine(error_t r1, error_t r2);

//# 68 "/opt/tinyos-2.x/tos/chips/stm32/fwlib/inc/stm32f10x_type.h"
/*
 typedef enum __nesc_unnamed4250 {
 //#line 68
 RESET = 0, SET = !RESET
 }
 //#line 68
 FlagStatus;
 */
//#line 68
/*
 typedef enum __nesc_unnamed4250 ITStatus;

 typedef enum __nesc_unnamed4251 {
 //#line 70
 DISABLE = 0, ENABLE = !DISABLE
 }
 //#line 70
 FunctionalState;
 */

/*
 typedef enum __nesc_unnamed4252 {
 //#line 73
 ERROR = 0, SUCCESS_fwlib = !ERROR
 }
 //#line 73
 ErrorStatus;
 */
//# 27 "/opt/tinyos-2.x/tos/chips/stm32/fwlib/inc/cortexm3_macro.h"
void __WFI(void);
//# 58 "/opt/tinyos-2.x/tos/chips/stm32/fwlib/inc/stm32f10x_map.h"
//#line 36
typedefnx_struct radio_count_msg {
	nx_uint16_t src;
	nx_uint16_t count;
	nx_uint16_t vals[50];
} __attribute__((packed)) radio_count_msg_t;

enum __nesc_unnamed4311 {
	AM_RADIO_COUNT_MSG = 239
};
// 43 "/opt/tinyos-2.x/tos/types/Leds.h"

/*
 enum __nesc_unnamed4312 {
 LEDS_LED0 = 1 << 0,
 LEDS_LED1 = 1 << 1,
 LEDS_LED2 = 1 << 2,
 LEDS_LED3 = 1 << 3,
 LEDS_LED4 = 1 << 4,
 LEDS_LED5 = 1 << 5,
 LEDS_LED6 = 1 << 6,
 LEDS_LED7 = 1 << 7
 };
 */
// 41 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230DriverLayer.h"
//line 38
typedefnx_struct rf230_header_t {

	nxle_uint8_t length;
} __attribute__((packed)) rf230_header_t;

//line 43
typedef struct rf230_metadata_t {

	uint8_t lqi;
	union {

		uint8_t power;
		uint8_t rssi;
	};
} rf230_metadata_t;

enum rf230_registers_enum {

	RF230_TRX_STATUS = 0x01,
	RF230_TRX_STATE = 0x02,
	RF230_TRX_CTRL_0 = 0x03,
	RF230_PHY_TX_PWR = 0x05,
	RF230_PHY_RSSI = 0x06,
	RF230_PHY_ED_LEVEL = 0x07,
	RF230_PHY_CC_CCA = 0x08,
	RF230_CCA_THRES = 0x09,
	RF230_IRQ_MASK = 0x0E,
	RF230_IRQ_STATUS = 0x0F,
	RF230_VREG_CTRL = 0x10,
	RF230_BATMON = 0x11,
	RF230_XOSC_CTRL = 0x12,
	RF230_PLL_CF = 0x1A,
	RF230_PLL_DCU = 0x1B,
	RF230_PART_NUM = 0x1C,
	RF230_VERSION_NUM = 0x1D,
	RF230_MAN_ID_0 = 0x1E,
	RF230_MAN_ID_1 = 0x1F,
	RF230_SHORT_ADDR_0 = 0x20,
	RF230_SHORT_ADDR_1 = 0x21,
	RF230_PAN_ID_0 = 0x22,
	RF230_PAN_ID_1 = 0x23,
	RF230_IEEE_ADDR_0 = 0x24,
	RF230_IEEE_ADDR_1 = 0x25,
	RF230_IEEE_ADDR_2 = 0x26,
	RF230_IEEE_ADDR_3 = 0x27,
	RF230_IEEE_ADDR_4 = 0x28,
	RF230_IEEE_ADDR_5 = 0x29,
	RF230_IEEE_ADDR_6 = 0x2A,
	RF230_IEEE_ADDR_7 = 0x2B,
	RF230_XAH_CTRL = 0x2C,
	RF230_CSMA_SEED_0 = 0x2D,
	RF230_CSMA_SEED_1 = 0x2E
};

enum rf230_trx_register_enums {

	RF230_CCA_DONE = 1 << 7,
	RF230_CCA_STATUS = 1 << 6,
	RF230_TRX_STATUS_MASK = 0x1F,
	RF230_P_ON = 0,
	RF230_BUSY_RX = 1,
	RF230_BUSY_TX = 2,
	RF230_RX_ON = 6,
	RF230_TRX_OFF = 8,
	RF230_PLL_ON = 9,
	RF230_SLEEP = 15,
	RF230_BUSY_RX_AACK = 17,
	RF230_BUSR_TX_ARET = 18,
	RF230_RX_AACK_ON = 22,
	RF230_TX_ARET_ON = 25,
	RF230_RX_ON_NOCLK = 28,
	RF230_AACK_ON_NOCLK = 29,
	RF230_BUSY_RX_AACK_NOCLK = 30,
	RF230_STATE_TRANSITION_IN_PROGRESS = 31,
	RF230_TRAC_STATUS_MASK = 0xE0,
	RF230_TRAC_SUCCESS = 0,
	RF230_TRAC_SUCCESS_DATA_PENDING = 1 << 5,
	RF230_TRAC_CHANNEL_ACCESS_FAILURE = 3 << 5,
	RF230_TRAC_NO_ACK = 5 << 5,
	RF230_TRAC_INVALID = 7 << 5,
	RF230_TRX_CMD_MASK = 0x1F,
	RF230_NOP = 0,
	RF230_TX_START = 2,
	RF230_FORCE_TRX_OFF = 3
};

enum rf230_phy_register_enums {

	RF230_TX_AUTO_CRC_ON = 1 << 7,
	RF230_TX_PWR_MASK = 0x0F,
	RF230_RSSI_MASK = 0x1F,
	RF230_CCA_REQUEST = 1 << 7,
	RF230_CCA_MODE_0 = 0 << 5,
	RF230_CCA_MODE_1 = 1 << 5,
	RF230_CCA_MODE_2 = 2 << 5,
	RF230_CCA_MODE_3 = 3 << 5,
	RF230_CHANNEL_DEFAULT = 11,
	RF230_CHANNEL_MASK = 0x1F,
	RF230_CCA_CS_THRES_SHIFT = 4,
	RF230_CCA_ED_THRES_SHIFT = 0
};

enum rf230_irq_register_enums {

	RF230_IRQ_BAT_LOW = 1 << 7,
	RF230_IRQ_TRX_UR = 1 << 6,
	RF230_IRQ_TRX_END = 1 << 3,
	RF230_IRQ_RX_START = 1 << 2,
	RF230_IRQ_PLL_UNLOCK = 1 << 1,
	RF230_IRQ_PLL_LOCK = 1 << 0
};

enum rf230_control_register_enums {

	RF230_AVREG_EXT = 1 << 7,
	RF230_AVDD_OK = 1 << 6,
	RF230_DVREG_EXT = 1 << 3,
	RF230_DVDD_OK = 1 << 2,
	RF230_BATMON_OK = 1 << 5,
	RF230_BATMON_VHR = 1 << 4,
	RF230_BATMON_VTH_MASK = 0x0F,
	RF230_XTAL_MODE_OFF = 0 << 4,
	RF230_XTAL_MODE_EXTERNAL = 4 << 4,
	RF230_XTAL_MODE_INTERNAL = 15 << 4
};

enum rf230_pll_register_enums {

	RF230_PLL_CF_START = 1 << 7, RF230_PLL_DCU_START = 1 << 7
};

enum rf230_spi_command_enums {

	RF230_CMD_REGISTER_READ = 0x80,
	RF230_CMD_REGISTER_WRITE = 0xC0,
	RF230_CMD_REGISTER_MASK = 0x3F,
	RF230_CMD_FRAME_READ = 0x20,
	RF230_CMD_FRAME_WRITE = 0x60,
	RF230_CMD_SRAM_READ = 0x00,
	RF230_CMD_SRAM_WRITE = 0x40
};
// 251 "/usr/lib/gcc/avr/3.4.6/../../../../avr/include/util/crc16.h"
//line 250
static __inline uint16_t _crc_ccitt_update(uint16_t __crc, uint8_t __data);
// 42 "/opt/tinyos-2.x/tos/platforms/iris/chips/rf230/RadioConfig.h"
enum __nesc_unnamed4313 {

	RF230_TRX_CTRL_0_VALUE = 0,

	RF230_CCA_MODE_VALUE = RF230_CCA_MODE_3,

	RF230_CCA_THRES_VALUE = 0xC7
};
//line 82
static __inline uint16_t RF230_CRCBYTE_COMMAND(uint16_t crc, uint8_t data);

// 45 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/TinyosNetworkLayer.h"
//line 42
typedefnx_struct network_header_t {

	nxle_uint8_t network;
} __attribute__((packed)) network_header_t;
// 45 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/Ieee154PacketLayer.h"
//line 38
typedefnx_struct ieee154_header_t {

	nxle_uint16_t fcf;
	nxle_uint8_t dsn;
	nxle_uint16_t destpan;
	nxle_uint16_t dest;
	nxle_uint16_t src;
} __attribute__((packed)) ieee154_header_t;

enum ieee154_fcf_enums {
	IEEE154_FCF_FRAME_TYPE = 0,
	IEEE154_FCF_SECURITY_ENABLED = 3,
	IEEE154_FCF_FRAME_PENDING = 4,
	IEEE154_FCF_ACK_REQ = 5,
	IEEE154_FCF_INTRAPAN = 6,
	IEEE154_FCF_DEST_ADDR_MODE = 10,
	IEEE154_FCF_SRC_ADDR_MODE = 14
};

enum ieee154_fcf_type_enums {
	IEEE154_TYPE_BEACON = 0,
	IEEE154_TYPE_DATA = 1,
	IEEE154_TYPE_ACK = 2,
	IEEE154_TYPE_MAC_CMD = 3,
	IEEE154_TYPE_MASK = 7
};

enum iee154_fcf_addr_mode_enums {
	IEEE154_ADDR_NONE = 0,
	IEEE154_ADDR_SHORT = 2,
	IEEE154_ADDR_EXT = 3,
	IEEE154_ADDR_MASK = 3
};
// 6 "/opt/tinyos-2.x/tos/types/AM.h"
typedef nx_uint8_t nx_am_id_t;
typedef nx_uint8_t nx_am_group_t;
typedef nx_uint16_t nx_am_addr_t;

typedef uint8_t am_id_t;
typedef uint8_t am_group_t;
typedef uint16_t am_addr_t;

enum __nesc_unnamed4314 {
	AM_BROADCAST_ADDR = 0xffff
};

enum __nesc_unnamed4315 {
	TOS_AM_GROUP = 0x22, TOS_AM_ADDRESS = 1
};
// 43 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/ActiveMessageLayer.h"
//line 40
typedefnx_struct activemessage_header_t {

	nx_am_id_t type;
} __attribute__((packed)) activemessage_header_t;
// 42 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/MetadataFlagsLayer.h"
//line 38
typedef struct flags_metadata_t {

	uint8_t flags;
} flags_metadata_t;
// 41 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/TimeStampingLayer.h"
//line 38
typedef struct timestamp_metadata_t {

	uint32_t timestamp;
} timestamp_metadata_t;
// 41 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/LowPowerListeningLayer.h"
//line 38
typedef struct lpl_metadata_t {

	uint16_t sleepint;
} lpl_metadata_t;
// 42 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/PacketLinkLayer.h"
//line 38
typedef struct link_metadata_t {

	uint16_t maxRetries;
	uint16_t retryDelay;
} link_metadata_t;
// 58 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230Radio.h"
//line 48
typedefnx_struct rf230packet_header_t {

	rf230_header_t rf230;
	ieee154_header_t ieee154;

	network_header_t network;

	activemessage_header_t am;
} __attribute__((packed))
rf230packet_header_t;

//line 60
typedefnx_struct rf230packet_footer_t {
} __attribute__((packed))

rf230packet_footer_t;
//line 76
//line 65
typedef struct rf230packet_metadata_t {

	link_metadata_t link;

	timestamp_metadata_t timestamp;
	flags_metadata_t flags;
	rf230_metadata_t rf230;
} rf230packet_metadata_t;
// 83 "/opt/tinyos-2.x/tos/lib/serial/Serial.h"
typedef uint8_t uart_id_t;

// Nived : Intialising the structure globally

SPI_InitTypeDef SPI_InitStructure;

enum __nesc_unnamed4316 {
	HDLC_FLAG_BYTE = 0x7e, HDLC_CTLESC_BYTE = 0x7d
};

enum __nesc_unnamed4317 {
	TOS_SERIAL_ACTIVE_MESSAGE_ID = 0,
	TOS_SERIAL_CC1000_ID = 1,
	TOS_SERIAL_802_15_4_ID = 2,
	TOS_SERIAL_UNKNOWN_ID = 255
};

enum __nesc_unnamed4318 {
	SERIAL_PROTO_ACK = 67,
	SERIAL_PROTO_PACKET_ACK = 68,
	SERIAL_PROTO_PACKET_NOACK = 69,
	SERIAL_PROTO_PACKET_UNKNOWN = 255
};
//line 121
//line 109
typedef struct radio_stats {
	uint8_t version;
	uint8_t flags;
	uint8_t reserved;
	uint8_t platform;
	uint16_t MTU;
	uint16_t radio_crc_fail;
	uint16_t radio_queue_drops;
	uint16_t serial_crc_fail;
	uint16_t serial_tx_fail;
	uint16_t serial_short_packets;
	uint16_t serial_proto_drops;
} radio_stats_t;

//line 123
typedefnx_struct serial_header {
	nx_am_addr_t dest;
	nx_am_addr_t src;
	nx_uint8_t length;
	nx_am_group_t group;
	nx_am_id_t type;
} __attribute__((packed)) serial_header_t;

//line 131
typedefnx_struct serial_packet {
	serial_header_t header;
	nx_uint8_t data[];
} __attribute__((packed)) serial_packet_t;

//line 136
typedefnx_struct serial_metadata {
	nx_uint8_t ack;
} __attribute__((packed)) serial_metadata_t;
// 43 "/opt/tinyos-2.x/tos/platforms/iris/platform_message.h"
//line 40
typedef union message_header {
	rf230packet_header_t rf230;
	serial_header_t serial;
} message_header_t;

//line 45
typedef union message_footer {
	rf230packet_footer_t rf230;
} message_footer_t;

//line 49
typedef union message_metadata {
	rf230packet_metadata_t rf230;
} message_metadata_t;
// 19 "/opt/tinyos-2.x/tos/types/message.h"
//line 14
typedefnx_struct message_t {
	nx_uint8_t header[sizeof(message_header_t )];
	nx_uint8_t data[120];
	nx_uint8_t footer[sizeof(message_footer_t )];
	nx_uint8_t metadata[sizeof(message_metadata_t )];
} __attribute__((packed)) message_t;
// 44 "/opt/tinyos-2.x/tos/types/Ieee154.h"
typedef uint16_t ieee154_panid_t;
typedef uint16_t ieee154_saddr_t;

enum __nesc_unnamed4319 {
	IEEE154_BROADCAST_ADDR = 0xffff
};

/*
 typedef struct __nesc_unnamed4253 {

 volatile uint32_t SR;
 volatile uint32_t CR1;
 volatile uint32_t CR2;
 volatile uint32_t SMPR1;
 volatile uint32_t SMPR2;
 volatile uint32_t JOFR1;
 volatile uint32_t JOFR2;
 volatile uint32_t JOFR3;
 volatile uint32_t JOFR4;
 volatile uint32_t HTR;
 volatile uint32_t LTR;
 volatile uint32_t SQR1;
 volatile uint32_t SQR2;
 volatile uint32_t SQR3;
 volatile uint32_t JSQR;
 volatile uint32_t JDR1;
 volatile uint32_t JDR2;
 volatile uint32_t JDR3;
 volatile uint32_t JDR4;
 volatile uint32_t DR;
 } ADC_TypeDef;
 //#line 90
 //#line 61
 typedef struct __nesc_unnamed4254 {

 uint32_t RESERVED0;
 volatile uint16_t DR1;
 uint16_t RESERVED1;
 volatile uint16_t DR2;
 uint16_t RESERVED2;
 volatile uint16_t DR3;
 uint16_t RESERVED3;
 volatile uint16_t DR4;
 uint16_t RESERVED4;
 volatile uint16_t DR5;
 uint16_t RESERVED5;
 volatile uint16_t DR6;
 uint16_t RESERVED6;
 volatile uint16_t DR7;
 uint16_t RESERVED7;
 volatile uint16_t DR8;
 uint16_t RESERVED8;
 volatile uint16_t DR9;
 uint16_t RESERVED9;
 volatile uint16_t DR10;
 uint16_t RESERVED10;

 uint16_t RESERVED11;
 volatile uint16_t CR;
 uint16_t RESERVED12;
 volatile uint16_t CSR;
 uint16_t RESERVED13;
 } BKP_TypeDef;








 //#line 93
 typedef struct __nesc_unnamed4255 {

 volatile uint32_t TIR;
 volatile uint32_t TDTR;
 volatile uint32_t TDLR;
 volatile uint32_t TDHR;
 } CAN_TxMailBox_TypeDef;







 //#line 101
 typedef struct __nesc_unnamed4256 {

 volatile uint32_t RIR;
 volatile uint32_t RDTR;
 volatile uint32_t RDLR;
 volatile uint32_t RDHR;
 } CAN_FIFOMailBox_TypeDef;





 //#line 109
 typedef struct __nesc_unnamed4257 {

 volatile uint32_t FR0;
 volatile uint32_t FR1;
 } CAN_FilterRegister_TypeDef;
 //#line 139
 //#line 115
 typedef struct __nesc_unnamed4258 {

 volatile uint32_t MCR;
 volatile uint32_t MSR;
 volatile uint32_t TSR;
 volatile uint32_t RF0R;
 volatile uint32_t RF1R;
 volatile uint32_t IER;
 volatile uint32_t ESR;
 volatile uint32_t BTR;
 uint32_t RESERVED0[88];
 CAN_TxMailBox_TypeDef sTxMailBox[3];
 CAN_FIFOMailBox_TypeDef sFIFOMailBox[2];
 uint32_t RESERVED1[12];
 volatile uint32_t FMR;
 volatile uint32_t FM0R;
 uint32_t RESERVED2[1];
 volatile uint32_t FS0R;
 uint32_t RESERVED3[1];
 volatile uint32_t FFA0R;
 uint32_t RESERVED4[1];
 volatile uint32_t FA0R;
 uint32_t RESERVED5[8];
 CAN_FilterRegister_TypeDef sFilterRegister[14];
 } CAN_TypeDef;








 //#line 142
 typedef struct __nesc_unnamed4259 {

 volatile uint32_t CCR;
 volatile uint32_t CNDTR;
 volatile uint32_t CPAR;
 volatile uint32_t CMAR;
 } DMA_Channel_TypeDef;





 //#line 150
 typedef struct __nesc_unnamed4260 {

 volatile uint32_t ISR;
 volatile uint32_t IFCR;
 } DMA_TypeDef;










 //#line 157
 typedef struct __nesc_unnamed4261 {

 volatile uint32_t IMR;
 volatile uint32_t EMR;
 volatile uint32_t RTSR;
 volatile uint32_t FTSR;
 volatile uint32_t SWIER;
 volatile uint32_t PR;
 } EXTI_TypeDef;
 //#line 179
 //#line 168
 typedef struct __nesc_unnamed4262 {

 volatile uint32_t ACR;
 volatile uint32_t KEYR;
 volatile uint32_t OPTKEYR;
 volatile uint32_t SR;
 volatile uint32_t CR;
 volatile uint32_t AR;
 volatile uint32_t RESERVED;
 volatile uint32_t OBR;
 volatile uint32_t WRPR;
 } FLASH_TypeDef;
 //#line 191
 //#line 181
 typedef struct __nesc_unnamed4263 {

 volatile uint16_t RDP;
 volatile uint16_t USER;
 volatile uint16_t Data0;
 volatile uint16_t Data1;
 volatile uint16_t WRP0;
 volatile uint16_t WRP1;
 volatile uint16_t WRP2;
 volatile uint16_t WRP3;
 } OB_TypeDef;
 //#line 203
 //#line 194
 typedef struct __nesc_unnamed4264 {

 volatile uint32_t CRL;
 volatile uint32_t CRH;
 volatile uint32_t IDR;
 volatile uint32_t ODR;
 volatile uint32_t BSRR;
 volatile uint32_t BRR;
 volatile uint32_t LCKR;
 } GPIO_TypeDef;






 //#line 205
 typedef struct __nesc_unnamed4265 {

 volatile uint32_t EVCR;
 volatile uint32_t MAPR;
 volatile uint32_t EXTICR[4];
 } AFIO_TypeDef;
 //#line 233
 //#line 213
 typedef struct __nesc_unnamed4266 {

 volatile uint16_t CR1;
 uint16_t RESERVED0;
 volatile uint16_t CR2;
 uint16_t RESERVED1;
 volatile uint16_t OAR1;
 uint16_t RESERVED2;
 volatile uint16_t OAR2;
 uint16_t RESERVED3;
 volatile uint16_t DR;
 uint16_t RESERVED4;
 volatile uint16_t SR1;
 uint16_t RESERVED5;
 volatile uint16_t SR2;
 uint16_t RESERVED6;
 volatile uint16_t CCR;
 uint16_t RESERVED7;
 volatile uint16_t TRISE;
 uint16_t RESERVED8;
 } I2C_TypeDef;








 //#line 236
 typedef struct __nesc_unnamed4267 {

 volatile uint32_t KR;
 volatile uint32_t PR;
 volatile uint32_t RLR;
 volatile uint32_t SR;
 } IWDG_TypeDef;
 //#line 258
 //#line 245
 typedef struct __nesc_unnamed4268 {

 volatile uint32_t ISER[2];
 uint32_t RESERVED0[30];
 volatile uint32_t ICER[2];
 uint32_t RSERVED1[30];
 volatile uint32_t ISPR[2];
 uint32_t RESERVED2[30];
 volatile uint32_t ICPR[2];
 uint32_t RESERVED3[30];
 volatile uint32_t IABR[2];
 uint32_t RESERVED4[62];
 volatile uint32_t IPR[11];
 } NVIC_TypeDef;
 //#line 276
 //#line 260
 typedef struct __nesc_unnamed4269 {

 volatile const uint32_t CPUID;
 volatile uint32_t ICSR;
 volatile uint32_t VTOR;
 volatile uint32_t AIRCR;
 volatile uint32_t SCR;
 volatile uint32_t CCR;
 volatile uint32_t SHPR[3];
 volatile uint32_t SHCSR;
 volatile uint32_t CFSR;
 volatile uint32_t HFSR;
 volatile uint32_t DFSR;
 volatile uint32_t MMFAR;
 volatile uint32_t BFAR;
 volatile uint32_t AFSR;
 } SCB_TypeDef;






 //#line 279
 typedef struct __nesc_unnamed4270 {

 volatile uint32_t CR;
 volatile uint32_t CSR;
 } PWR_TypeDef;
 //#line 298
 //#line 286
 typedef struct __nesc_unnamed4271 {

 volatile uint32_t CR;
 volatile uint32_t CFGR;
 volatile uint32_t CIR;
 volatile uint32_t APB2RSTR;
 volatile uint32_t APB1RSTR;
 volatile uint32_t AHBENR;
 volatile uint32_t APB2ENR;
 volatile uint32_t APB1ENR;
 volatile uint32_t BDCR;
 volatile uint32_t CSR;
 } RCC_TypeDef;
 //#line 323
 //#line 301
 typedef struct __nesc_unnamed4272 {

 volatile uint16_t CRH;
 uint16_t RESERVED0;
 volatile uint16_t CRL;
 uint16_t RESERVED1;
 volatile uint16_t PRLH;
 uint16_t RESERVED2;
 volatile uint16_t PRLL;
 uint16_t RESERVED3;
 volatile uint16_t DIVH;
 uint16_t RESERVED4;
 volatile uint16_t DIVL;
 uint16_t RESERVED5;
 volatile uint16_t CNTH;
 uint16_t RESERVED6;
 volatile uint16_t CNTL;
 uint16_t RESERVED7;
 volatile uint16_t ALRH;
 uint16_t RESERVED8;
 volatile uint16_t ALRL;
 uint16_t RESERVED9;
 } RTC_TypeDef;
 //#line 342
 //#line 326
 typedef struct __nesc_unnamed4273 {

 volatile uint16_t CR1;
 uint16_t RESERVED0;
 volatile uint16_t CR2;
 uint16_t RESERVED1;
 volatile uint16_t SR;
 uint16_t RESERVED2;
 volatile uint16_t DR;
 uint16_t RESERVED3;
 volatile uint16_t CRCPR;
 uint16_t RESERVED4;
 volatile uint16_t RXCRCR;
 uint16_t RESERVED5;
 volatile uint16_t TXCRCR;
 uint16_t RESERVED6;
 } SPI_TypeDef;








 //#line 345
 typedef struct __nesc_unnamed4274 {

 volatile uint32_t CTRL;
 volatile uint32_t LOAD;
 volatile uint32_t VAL;
 volatile const uint32_t CALIB;
 } SysTick_TypeDef;
 //#line 396
 //#line 354
 typedef struct __nesc_unnamed4275 {

 volatile uint16_t CR1;
 uint16_t RESERVED0;
 volatile uint16_t CR2;
 uint16_t RESERVED1;
 volatile uint16_t SMCR;
 uint16_t RESERVED2;
 volatile uint16_t DIER;
 uint16_t RESERVED3;
 volatile uint16_t SR;
 uint16_t RESERVED4;
 volatile uint16_t EGR;
 uint16_t RESERVED5;
 volatile uint16_t CCMR1;
 uint16_t RESERVED6;
 volatile uint16_t CCMR2;
 uint16_t RESERVED7;
 volatile uint16_t CCER;
 uint16_t RESERVED8;
 volatile uint16_t CNT;
 uint16_t RESERVED9;
 volatile uint16_t PSC;
 uint16_t RESERVED10;
 volatile uint16_t ARR;
 uint16_t RESERVED11;
 volatile uint16_t RCR;
 uint16_t RESERVED12;
 volatile uint16_t CCR1;
 uint16_t RESERVED13;
 volatile uint16_t CCR2;
 uint16_t RESERVED14;
 volatile uint16_t CCR3;
 uint16_t RESERVED15;
 volatile uint16_t CCR4;
 uint16_t RESERVED16;
 volatile uint16_t BDTR;
 uint16_t RESERVED17;
 volatile uint16_t DCR;
 uint16_t RESERVED18;
 volatile uint16_t DMAR;
 uint16_t RESERVED19;
 } TIM1_TypeDef;
 //#line 437
 //#line 399
 typedef struct __nesc_unnamed4276 {

 volatile uint16_t CR1;
 uint16_t RESERVED0;
 volatile uint16_t CR2;
 uint16_t RESERVED1;
 volatile uint16_t SMCR;
 uint16_t RESERVED2;
 volatile uint16_t DIER;
 uint16_t RESERVED3;
 volatile uint16_t SR;
 uint16_t RESERVED4;
 volatile uint16_t EGR;
 uint16_t RESERVED5;
 volatile uint16_t CCMR1;
 uint16_t RESERVED6;
 volatile uint16_t CCMR2;
 uint16_t RESERVED7;
 volatile uint16_t CCER;
 uint16_t RESERVED8;
 volatile uint16_t CNT;
 uint16_t RESERVED9;
 volatile uint16_t PSC;
 uint16_t RESERVED10;
 volatile uint16_t ARR;
 uint16_t RESERVED11[3];
 volatile uint16_t CCR1;
 uint16_t RESERVED12;
 volatile uint16_t CCR2;
 uint16_t RESERVED13;
 volatile uint16_t CCR3;
 uint16_t RESERVED14;
 volatile uint16_t CCR4;
 uint16_t RESERVED15[3];
 volatile uint16_t DCR;
 uint16_t RESERVED16;
 volatile uint16_t DMAR;
 uint16_t RESERVED17;
 } TIM_TypeDef;
 //#line 456
 //#line 440
 typedef struct __nesc_unnamed4277 {

 volatile uint16_t SR;
 uint16_t RESERVED0;
 volatile uint16_t DR;
 uint16_t RESERVED1;
 volatile uint16_t BRR;
 uint16_t RESERVED2;
 volatile uint16_t CR1;
 uint16_t RESERVED3;
 volatile uint16_t CR2;
 uint16_t RESERVED4;
 volatile uint16_t CR3;
 uint16_t RESERVED5;
 volatile uint16_t GTPR;
 uint16_t RESERVED6;
 } USART_TypeDef;







 //#line 459
 typedef struct __nesc_unnamed4278 {

 volatile uint32_t CR;
 volatile uint32_t CFR;
 volatile uint32_t SR;
 } WWDG_TypeDef;
 //# 34 "/opt/tinyos-2.x/tos/chips/stm32/fwlib/inc/stm32f10x_adc.h"
 //#line 26
 typedef struct __nesc_unnamed4279 {

 uint32_t ADC_Mode;
 FunctionalState ADC_ScanConvMode;
 FunctionalState ADC_ContinuousConvMode;
 uint32_t ADC_ExternalTrigConv;
 uint32_t ADC_DataAlign;
 uint8_t ADC_NbrOfChannel;
 } ADC_InitTypeDef;
 //# 65 "/opt/tinyos-2.x/tos/chips/stm32/fwlib/inc/stm32f10x_bkp.h"
 void BKP_DeInit(void );
 //# 39 "/opt/tinyos-2.x/tos/chips/stm32/fwlib/inc/stm32f10x_can.h"
 //#line 26
 typedef struct __nesc_unnamed4280 {

 FunctionalState CAN_TTCM;
 FunctionalState CAN_ABOM;
 FunctionalState CAN_AWUM;
 FunctionalState CAN_NART;
 FunctionalState CAN_RFLM;
 FunctionalState CAN_TXFP;
 uint8_t CAN_Mode;
 uint8_t CAN_SJW;
 uint8_t CAN_BS1;
 uint8_t CAN_BS2;
 uint16_t CAN_Prescaler;
 } CAN_InitTypeDef;
 //#line 53
 //#line 42
 typedef struct __nesc_unnamed4281 {

 uint8_t CAN_FilterNumber;
 uint8_t CAN_FilterMode;
 uint8_t CAN_FilterScale;
 uint16_t CAN_FilterIdHigh;
 uint16_t CAN_FilterIdLow;
 uint16_t CAN_FilterMaskIdHigh;
 uint16_t CAN_FilterMaskIdLow;
 uint16_t CAN_FilterFIFOAssignment;
 FunctionalState CAN_FilterActivation;
 } CAN_FilterInitTypeDef;










 //#line 56
 typedef struct __nesc_unnamed4282 {

 uint32_t StdId;
 uint32_t ExtId;
 uint8_t IDE;
 uint8_t RTR;
 uint8_t DLC;
 uint8_t Data[8];
 } CanTxMsg;
 //#line 76
 //#line 67
 typedef struct __nesc_unnamed4283 {

 uint32_t StdId;
 uint32_t ExtId;
 uint8_t IDE;
 uint8_t RTR;
 uint8_t DLC;
 uint8_t Data[8];
 uint8_t FMI;
 } CanRxMsg;
 //# 39 "/opt/tinyos-2.x/tos/chips/stm32/fwlib/inc/stm32f10x_dma.h"
 //#line 26

 /*
 typedef struct __nesc_unnamed4284 {

 uint32_t DMA_PeripheralBaseAddr;
 uint32_t DMA_MemoryBaseAddr;
 uint32_t DMA_DIR;
 uint32_t DMA_BufferSize;
 uint32_t DMA_PeripheralInc;
 uint32_t DMA_MemoryInc;
 uint32_t DMA_PeripheralDataSize;
 uint32_t DMA_MemoryDataSize;
 uint32_t DMA_Mode;
 uint32_t DMA_Priority;
 uint32_t DMA_M2M;
 } DMA_InitTypeDef;
 //# 30 "/opt/tinyos-2.x/tos/chips/stm32/fwlib/inc/stm32f10x_exti.h"
 //#line 26
 typedef enum __nesc_unnamed4285 {

 EXTI_Mode_Interrupt = 0x00,
 EXTI_Mode_Event = 0x04
 } EXTIMode_TypeDef;









 //#line 35
 typedef enum __nesc_unnamed4286 {

 EXTI_Trigger_Rising = 0x08,
 EXTI_Trigger_Falling = 0x0C,
 EXTI_Trigger_Rising_Falling = 0x10
 } EXTITrigger_TypeDef;
 //#line 53
 //#line 47
 typedef struct __nesc_unnamed4287 {

 uint32_t EXTI_Line;
 EXTIMode_TypeDef EXTI_Mode;
 EXTITrigger_TypeDef EXTI_Trigger;
 FunctionalState EXTI_LineCmd;
 } EXTI_InitTypeDef;
 //#line 97

 void EXTI_Init(EXTI_InitTypeDef *EXTI_InitStruct);





 void EXTI_ClearITPendingBit(uint32_t EXTI_Line);
 //# 31 "/opt/tinyos-2.x/tos/chips/stm32/fwlib/inc/stm32f10x_gpio.h"
 //#line 26
 typedef enum __nesc_unnamed4288 {

 GPIO_Speed_10MHz = 1,
 GPIO_Speed_2MHz,
 GPIO_Speed_50MHz
 } GPIOSpeed_TypeDef;
 //#line 46
 //#line 37
 typedef enum __nesc_unnamed4289 {
 GPIO_Mode_AIN = 0x0,
 GPIO_Mode_IN_FLOATING = 0x04,
 GPIO_Mode_IPD = 0x28,
 GPIO_Mode_IPU = 0x48,
 GPIO_Mode_Out_OD = 0x14,
 GPIO_Mode_Out_PP = 0x10,
 GPIO_Mode_AF_OD = 0x1C,
 GPIO_Mode_AF_PP = 0x18
 } GPIOMode_TypeDef;
 //#line 59
 //#line 54
 typedef struct __nesc_unnamed4290 {

 uint16_t GPIO_Pin;
 GPIOSpeed_TypeDef GPIO_Speed;
 GPIOMode_TypeDef GPIO_Mode;
 } GPIO_InitTypeDef;





 //#line 62
 typedef enum __nesc_unnamed4291 {
 Bit_RESET = 0,
 Bit_SET
 } BitAction;
 //#line 192
 void GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_InitStruct);
 //# 34 "/opt/tinyos-2.x/tos/chips/stm32/fwlib/inc/stm32f10x_i2c.h"
 //#line 26
 typedef struct __nesc_unnamed4292 {

 uint16_t I2C_Mode;
 uint16_t I2C_DutyCycle;
 uint16_t I2C_OwnAddress1;
 uint16_t I2C_Ack;
 uint16_t I2C_AcknowledgedAddress;
 uint32_t I2C_ClockSpeed;
 } I2C_InitTypeDef;
 //# 32 "/opt/tinyos-2.x/tos/chips/stm32/fwlib/inc/stm32f10x_nvic.h"
 //#line 26
 typedef struct __nesc_unnamed4293 {

 uint8_t NVIC_IRQChannel;
 uint8_t NVIC_IRQChannelPreemptionPriority;
 uint8_t NVIC_IRQChannelSubPriority;
 FunctionalState NVIC_IRQChannelCmd;
 } NVIC_InitTypeDef;
 //#line 219
 void NVIC_PriorityGroupConfig(uint32_t NVIC_PriorityGroup);
 void NVIC_Init(NVIC_InitTypeDef *NVIC_InitStruct);
 //# 66 "/opt/tinyos-2.x/tos/chips/stm32/fwlib/inc/stm32f10x_pwr.h"
 void PWR_BackupAccessCmd(FunctionalState NewState);





 FlagStatus PWR_GetFlagStatus(uint32_t PWR_FLAG);
 void PWR_ClearFlag(uint32_t PWR_FLAG);
 //# 32 "/opt/tinyos-2.x/tos/chips/stm32/fwlib/inc/stm32f10x_rcc.h"
 //#line 25
 typedef struct __nesc_unnamed4294 {

 uint32_t SYSCLK_Frequency;
 uint32_t HCLK_Frequency;
 uint32_t PCLK1_Frequency;
 uint32_t PCLK2_Frequency;
 uint32_t ADCCLK_Frequency;
 } RCC_ClocksTypeDef;
 */

//#line 239
void RCC_HSEConfig(uint32_t RCC_HSE);

void RCC_HSICmd(FunctionalState NewState);
void RCC_PLLConfig(uint32_t RCC_PLLSource, uint32_t RCC_PLLMul);
void RCC_PLLCmd(FunctionalState NewState);
void RCC_SYSCLKConfig(uint32_t RCC_SYSCLKSource);

void RCC_HCLKConfig(uint32_t RCC_HCLK);
void RCC_PCLK1Config(uint32_t RCC_PCLK1);
void RCC_PCLK2Config(uint32_t RCC_PCLK2);

void RCC_USBCLKConfig(uint32_t RCC_USBCLKSource);
void RCC_ADCCLKConfig(uint32_t RCC_ADCCLK);
void RCC_LSEConfig(uint32_t RCC_LSE);

void RCC_RTCCLKConfig(uint32_t RCC_RTCCLKSource);
void RCC_RTCCLKCmd(FunctionalState NewState);

void RCC_AHBPeriphClockCmd(uint32_t RCC_AHBPeriph, FunctionalState NewState);
void RCC_APB2PeriphClockCmd(uint32_t RCC_APB2Periph, FunctionalState NewState);
void RCC_APB1PeriphClockCmd(uint32_t RCC_APB1Periph, FunctionalState NewState);

FlagStatus RCC_GetFlagStatus(uint8_t RCC_FLAG);
//# 53 "/opt/tinyos-2.x/tos/chips/stm32/fwlib/inc/stm32f10x_rtc.h"
void RTC_ITConfig(uint16_t RTC_IT, FunctionalState NewState);

uint32_t RTC_GetCounter(void);
void RTC_SetCounter(uint32_t CounterValue);
void RTC_SetPrescaler(uint32_t PrescalerValue);
void RTC_SetAlarm(uint32_t AlarmValue);

void RTC_WaitForLastTask(void);
void RTC_WaitForSynchro(void);

ITStatus RTC_GetITStatus(uint16_t RTC_IT);
void RTC_ClearITPendingBit(uint16_t RTC_IT);
//# 37 "/opt/tinyos-2.x/tos/chips/stm32/fwlib/inc/stm32f10x_spi.h"
//#line 26
/*
 typedef struct __nesc_unnamed4295 {

 uint16_t SPI_Direction;
 uint16_t SPI_Mode;
 uint16_t SPI_DataSize;
 uint16_t SPI_CPOL;
 uint16_t SPI_CPHA;
 uint16_t SPI_NSS;
 uint16_t SPI_BaudRatePrescaler;
 uint16_t SPI_FirstBit;
 uint16_t SPI_CRCPolynomial;
 } SPI_InitTypeDef;
 //# 34 "/opt/tinyos-2.x/tos/chips/stm32/fwlib/inc/stm32f10x_tim1.h"
 //#line 27
 typedef struct __nesc_unnamed4296 {

 uint16_t TIM1_Prescaler;
 uint16_t TIM1_CounterMode;
 uint16_t TIM1_Period;
 uint16_t TIM1_ClockDivision;
 uint8_t TIM1_RepetitionCounter;
 } TIM1_TimeBaseInitTypeDef;
 //#line 47
 //#line 37
 typedef struct __nesc_unnamed4297 {

 uint16_t TIM1_OCMode;
 uint16_t TIM1_OutputState;
 uint16_t TIM1_OutputNState;
 uint16_t TIM1_Pulse;
 uint16_t TIM1_OCPolarity;
 uint16_t TIM1_OCNPolarity;
 uint16_t TIM1_OCIdleState;
 uint16_t TIM1_OCNIdleState;
 } TIM1_OCInitTypeDef;









 //#line 50
 typedef struct __nesc_unnamed4298 {

 uint16_t TIM1_Channel;
 uint16_t TIM1_ICPolarity;
 uint16_t TIM1_ICSelection;
 uint16_t TIM1_ICPrescaler;
 uint8_t TIM1_ICFilter;
 } TIM1_ICInitTypeDef;
 //#line 69
 //#line 60
 typedef struct __nesc_unnamed4299 {

 uint16_t TIM1_OSSRState;
 uint16_t TIM1_OSSIState;
 uint16_t TIM1_LOCKLevel;
 uint16_t TIM1_DeadTime;
 uint16_t TIM1_Break;
 uint16_t TIM1_BreakPolarity;
 uint16_t TIM1_AutomaticOutput;
 } TIM1_BDTRInitTypeDef;
 //# 33 "/opt/tinyos-2.x/tos/chips/stm32/fwlib/inc/stm32f10x_tim.h"
 //#line 27
 typedef struct __nesc_unnamed4300 {

 uint16_t TIM_Period;
 uint16_t TIM_Prescaler;
 uint16_t TIM_ClockDivision;
 uint16_t TIM_CounterMode;
 } TIM_TimeBaseInitTypeDef;








 //#line 36
 typedef struct __nesc_unnamed4301 {

 uint16_t TIM_OCMode;
 uint16_t TIM_Channel;
 uint16_t TIM_Pulse;
 uint16_t TIM_OCPolarity;
 } TIM_OCInitTypeDef;



 */

//#line 45
/*
 typedef struct __nesc_unnamed4302 {

 uint16_t TIM_ICMode;
 uint16_t TIM_Channel;
 uint16_t TIM_ICPolarity;
 uint16_t TIM_ICSelection;
 uint16_t TIM_ICPrescaler;
 uint8_t TIM_ICFilter;
 } TIM_ICInitTypeDef;
 //# 38 "/opt/tinyos-2.x/tos/chips/stm32/fwlib/inc/stm32f10x_usart.h"
 //#line 26
 typedef struct __nesc_unnamed4303 {

 uint32_t USART_BaudRate;
 uint16_t USART_WordLength;
 uint16_t USART_StopBits;
 uint16_t USART_Parity;
 uint16_t USART_HardwareFlowControl;
 uint16_t USART_Mode;
 uint16_t USART_Clock;
 uint16_t USART_CPOL;
 uint16_t USART_CPHA;
 uint16_t USART_LastBit;
 } USART_InitTypeDef;
 */
//# 42 "/opt/tinyos-2.x/tos/chips/stm32/stm32exceptions.h"
void NMIException(void);

void HardFaultException(void);

void MemManageException(void);

void BusFaultException(void);

void UsageFaultException(void);

void __STM32ReservedException7(void);
void __STM32ReservedException8(void);
void __STM32ReservedException9(void);
void __STM32ReservedException10(void);

void SVCHandler(void);

void DebugMonitor(void);

void __STM32ReservedException13(void);

void PendSVC(void);

void SysTickHandler(void);

void WWDG_IRQHandler(void);

void PVD_IRQHandler(void);

void TAMPER_IRQHandler(void);

void RTC_IRQHandler(void);

void FLASH_IRQHandler(void);

void RCC_IRQHandler(void);

void EXTI0_IRQHandler(void);

void EXTI1_IRQHandler(void);

void EXTI2_IRQHandler(void);

void EXTI3_IRQHandler(void);

void EXTI4_IRQHandler(void);

void DMAChannel1_IRQHandler(void);

void DMAChannel2_IRQHandler(void);

void DMAChannel3_IRQHandler(void);

void DMAChannel4_IRQHandler(void);

void DMAChannel5_IRQHandler(void);

void DMAChannel6_IRQHandler(void);

void DMAChannel7_IRQHandler(void);

void ADC_IRQHandler(void);

void USB_HP_CAN_TX_IRQHandler(void);

void USB_LP_CAN_RX0_IRQHandler(void);

void CAN_RX1_IRQHandler(void);

void CAN_SCE_IRQHandler(void);

void EXTI9_5_IRQHandler(void);

void TIM1_BRK_IRQHandler(void);

void TIM1_UP_IRQHandler(void);

void TIM1_TRG_COM_IRQHandler(void);

void TIM1_CC_IRQHandler(void);

void TIM2_IRQHandler(void);

void TIM3_IRQHandler(void);

void TIM4_IRQHandler(void);

void I2C1_EV_IRQHandler(void);

void I2C1_ER_IRQHandler(void);

void I2C2_EV_IRQHandler(void);

void I2C2_ER_IRQHandler(void);

void SPI1_IRQHandler(void);

void SPI2_IRQHandler(void);

void USART1_IRQHandler(void);

void USART2_IRQHandler(void);

void USART3_IRQHandler(void);

void EXTI15_10_IRQHandler(void);

void USBWakeUp_IRQHandler(void);
//# 36 "/opt/tinyos-2.x/tos/chips/stm32/stm32hardware.h"
//# 32 "/opt/tinyos-2.x/tos/types/Leds.h"
enum __nesc_unnamed4307 {
	LEDS_LED0 = 1 << 0,
	LEDS_LED1 = 1 << 1,
	LEDS_LED2 = 1 << 2,
	LEDS_LED3 = 1 << 3,
	LEDS_LED4 = 1 << 4,
	LEDS_LED5 = 1 << 5,
	LEDS_LED6 = 1 << 6,
	LEDS_LED7 = 1 << 7
};
typedef TMilli BlinkC$Timer0$precision_tag;
typedef TMilli BlinkC$Timer1$precision_tag;
typedef TMilli BlinkC$Timer2$precision_tag;
typedef TMilli /*HilTimerMilliC.VirtTimersMilli32*/
		VirtualizeTimerC$0$precision_tag;
typedef/*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$precision_tag /*HilTimerMilliC.VirtTimersMilli32*/
		VirtualizeTimerC$0$TimerFrom$precision_tag;
typedef/*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$precision_tag /*HilTimerMilliC.VirtTimersMilli32*/
		VirtualizeTimerC$0$Timer$precision_tag;
typedef TMilli /*HilTimerMilliC.AlarmToTimerMilli32*/
		AlarmToTimerC$0$precision_tag;
typedef/*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$precision_tag /*HilTimerMilliC.AlarmToTimerMilli32*/
		AlarmToTimerC$0$Alarm$precision_tag;
typedef uint32_t /*HilTimerMilliC.AlarmToTimerMilli32*/
		AlarmToTimerC$0$Alarm$size_type;
typedef/*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$precision_tag /*HilTimerMilliC.AlarmToTimerMilli32*/
		AlarmToTimerC$0$Timer$precision_tag;
typedef TMilli STM32RtcC$LocalTime$precision_tag;
typedef TMilli STM32RtcC$Alarm$precision_tag;
typedef uint32_t STM32RtcC$Alarm$size_type;
typedef TMilli STM32RtcC$Counter$precision_tag;
typedef uint32_t STM32RtcC$Counter$size_type;
//# 51 "/opt/tinyos-2.x/tos/interfaces/Init.nc"
static error_t PlatformP$Init$init(void);
//# 31 "/opt/tinyos-2.x/tos/chips/stm32/HplSTM32Interrupt.nc"
static void PlatformP$Interrupt$fired(void);
//# 51 "/opt/tinyos-2.x/tos/interfaces/Init.nc"
static error_t MoteClockP$MoteClockInit$init(void);
//# 59 "/opt/tinyos-2.x/tos/interfaces/McuSleep.nc"
static void McuSleepC$McuSleep$sleep(void);
//# 51 "/opt/tinyos-2.x/tos/interfaces/Init.nc"
static error_t McuSleepC$McuSleepInit$init(void);
//# 56 "/opt/tinyos-2.x/tos/interfaces/TaskBasic.nc"
static error_t SchedulerBasicP$TaskBasic$postTask(
//# 45 "/opt/tinyos-2.x/tos/system/SchedulerBasicP.nc"
		uint8_t arg_0x404d8d50);
//# 64 "/opt/tinyos-2.x/tos/interfaces/TaskBasic.nc"
static void SchedulerBasicP$TaskBasic$default$runTask(
//# 45 "/opt/tinyos-2.x/tos/system/SchedulerBasicP.nc"
		uint8_t arg_0x404d8d50);
//# 46 "/opt/tinyos-2.x/tos/interfaces/Scheduler.nc"
static void SchedulerBasicP$Scheduler$init(void);
//#line 61
static void SchedulerBasicP$Scheduler$taskLoop(void);
//#line 54
static bool SchedulerBasicP$Scheduler$runNextTask(void);
//# 72 "/opt/tinyos-2.x/tos/lib/timer/Timer.nc"
static void BlinkC$Timer0$fired(void);
//# 49 "/opt/tinyos-2.x/tos/interfaces/Boot.nc"
static void BlinkC$Boot$booted(void);
//# 72 "/opt/tinyos-2.x/tos/lib/timer/Timer.nc"
static void BlinkC$Timer1$fired(void);
//#line 72
static void BlinkC$Timer2$fired(void);
//# 51 "/opt/tinyos-2.x/tos/interfaces/Init.nc"
static error_t LedsP$Init$init(void);
//# 56 "/opt/tinyos-2.x/tos/interfaces/Leds.nc"
static void LedsP$Leds$led0Toggle(void);
//#line 72
static void LedsP$Leds$led1Toggle(void);
//#line 89
static void LedsP$Leds$led2Toggle(void);
//# 31 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
static void /*HplSTM32GeneralIOC.PortC.Bit8*/
		HplSTM32GeneralIOPinP$40$IO$toggle(void);

static void /*HplSTM32GeneralIOC.PortC.Bit8*/
		HplSTM32GeneralIOPinP$40$IO$makeOutput(void);
//#line 29
static void /*HplSTM32GeneralIOC.PortC.Bit8*/HplSTM32GeneralIOPinP$40$IO$set(
		void);

static void /*HplSTM32GeneralIOC.PortC.Bit9*/
		HplSTM32GeneralIOPinP$41$IO$toggle(void);

static void /*HplSTM32GeneralIOC.PortC.Bit9*/
		HplSTM32GeneralIOPinP$41$IO$makeOutput(void);
//#line 29
static void /*HplSTM32GeneralIOC.PortC.Bit9*/HplSTM32GeneralIOPinP$41$IO$set(
		void);

static void /*HplSTM32GeneralIOC.PortC.Bit12*/
		HplSTM32GeneralIOPinP$44$IO$toggle(void);

static void /*HplSTM32GeneralIOC.PortC.Bit12*/
		HplSTM32GeneralIOPinP$44$IO$makeOutput(void);
//#line 29
static void /*HplSTM32GeneralIOC.PortC.Bit12*/HplSTM32GeneralIOPinP$44$IO$set(
		void);
//# 64 "/opt/tinyos-2.x/tos/interfaces/TaskBasic.nc"
static void /*HilTimerMilliC.VirtTimersMilli32*/
		VirtualizeTimerC$0$updateFromTimer$runTask(void);
//# 72 "/opt/tinyos-2.x/tos/lib/timer/Timer.nc"
static void /*HilTimerMilliC.VirtTimersMilli32*/
		VirtualizeTimerC$0$TimerFrom$fired(void);
//#line 72
static void /*HilTimerMilliC.VirtTimersMilli32*/
		VirtualizeTimerC$0$Timer$default$fired(
		//# 37 "/opt/tinyos-2.x/tos/lib/timer/VirtualizeTimerC.nc"
				uint8_t arg_0x4068b030);
//# 53 "/opt/tinyos-2.x/tos/lib/timer/Timer.nc"
static void /*HilTimerMilliC.VirtTimersMilli32*/
		VirtualizeTimerC$0$Timer$startPeriodic(
		//# 37 "/opt/tinyos-2.x/tos/lib/timer/VirtualizeTimerC.nc"
				uint8_t arg_0x4068b030,
				//# 53 "/opt/tinyos-2.x/tos/lib/timer/Timer.nc"
				uint32_t arg_0x40565120);
//# 64 "/opt/tinyos-2.x/tos/interfaces/TaskBasic.nc"
static void /*HilTimerMilliC.AlarmToTimerMilli32*/
		AlarmToTimerC$0$fired$runTask(void);
//# 67 "/opt/tinyos-2.x/tos/lib/timer/Alarm.nc"
static void /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Alarm$fired(
		void);
//# 125 "/opt/tinyos-2.x/tos/lib/timer/Timer.nc"
static uint32_t /*HilTimerMilliC.AlarmToTimerMilli32*/
		AlarmToTimerC$0$Timer$getNow(void);
//#line 118
static void /*HilTimerMilliC.AlarmToTimerMilli32*/
		AlarmToTimerC$0$Timer$startOneShotAt(uint32_t arg_0x40563360,
				uint32_t arg_0x405634f0);
//#line 67
static void /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Timer$stop(
		void);
//# 98 "/opt/tinyos-2.x/tos/lib/timer/Alarm.nc"
static STM32RtcC$Alarm$size_type STM32RtcC$Alarm$getNow(void);
//#line 92
static void STM32RtcC$Alarm$startAt(STM32RtcC$Alarm$size_type arg_0x406c1678,
		STM32RtcC$Alarm$size_type arg_0x406c1808);
//#line 105
static STM32RtcC$Alarm$size_type STM32RtcC$Alarm$getAlarm(void);
//#line 62
static void STM32RtcC$Alarm$stop(void);
//# 51 "/opt/tinyos-2.x/tos/interfaces/Init.nc"
static error_t STM32RtcC$Init$init(void);
//# 71 "/opt/tinyos-2.x/tos/lib/timer/Counter.nc"
static void STM32RtcC$Counter$default$overflow(void);
//# 51 "/opt/tinyos-2.x/tos/interfaces/Init.nc"
static error_t PlatformP$MoteInit$init(void);
//#line 51
static error_t PlatformP$MoteClockInit$init(void);
//#line 51
static error_t PlatformP$McuSleepInit$init(void);
//# 46 "/opt/tinyos-2.x/tos/platforms/stm32-p103/PlatformP.nc"
static inline error_t PlatformP$Init$init(void);
//#line 104
static inline void PlatformP$Interrupt$fired(void);
//# 37 "/opt/tinyos-2.x/tos/platforms/stm32-p103/MoteClockP.nc"
static inline error_t MoteClockP$MoteClockInit$init(void);
//# 43 "/opt/tinyos-2.x/tos/chips/stm32/McuSleepC.nc"
static inline error_t McuSleepC$McuSleepInit$init(void);
//#line 65
static inline void McuSleepC$McuSleep$sleep(void);
//#line 160
void RTCAlarm_IRQHandler(void);
//# 31 "/opt/tinyos-2.x/tos/chips/stm32/HplSTM32Interrupt.nc"
static void HplSTM32InterruptM$Irq$fired(void);
//# 50 "/opt/tinyos-2.x/tos/chips/stm32/HplSTM32InterruptM.nc"
void NMIException(void);

void HardFaultException(void);

void MemManageException(void);

void BusFaultException(void);

void UsageFaultException(void);

void __STM32ReservedException7(void);

void __STM32ReservedException8(void);

void __STM32ReservedException9(void);

void __STM32ReservedException10(void);

void SVCHandler(void);

void DebugMonitor(void);

void __STM32ReservedException13(void);

void PendSVC(void);

void SysTickHandler(void);

void WWDG_IRQHandler(void);

void PVD_IRQHandler(void);

void TAMPER_IRQHandler(void);
//#line 158
void FLASH_IRQHandler(void);

void RCC_IRQHandler(void);

void EXTI0_IRQHandler(void);

void EXTI1_IRQHandler(void);

void EXTI2_IRQHandler(void);

void EXTI3_IRQHandler(void);

void EXTI4_IRQHandler(void);

void DMAChannel1_IRQHandler(void);

void DMAChannel2_IRQHandler(void);

void DMAChannel3_IRQHandler(void);

void DMAChannel4_IRQHandler(void);

void DMAChannel5_IRQHandler(void);

void DMAChannel6_IRQHandler(void);

void DMAChannel7_IRQHandler(void);

void ADC_IRQHandler(void);

void USB_HP_CAN_TX_IRQHandler(void);

void USB_LP_CAN_RX0_IRQHandler(void);

void CAN_RX1_IRQHandler(void);

void CAN_SCE_IRQHandler(void);

void EXTI9_5_IRQHandler(void);

void TIM1_BRK_IRQHandler(void);

void TIM1_UP_IRQHandler(void);

void TIM1_TRG_COM_IRQHandler(void);

void TIM1_CC_IRQHandler(void);

void TIM2_IRQHandler(void);

void TIM3_IRQHandler(void);

void TIM4_IRQHandler(void);

void I2C1_EV_IRQHandler(void);

void I2C1_ER_IRQHandler(void);

void I2C2_EV_IRQHandler(void);

void I2C2_ER_IRQHandler(void);

void SPI1_IRQHandler(void);

void SPI2_IRQHandler(void);

void USART1_IRQHandler(void);

void USART2_IRQHandler(void);

void USART3_IRQHandler(void);

void EXTI15_10_IRQHandler(void);
//#line 389
void USBWakeUp_IRQHandler(void);

// Nived : Point 1


typedef uint16_t RandomMlcgC__SeedInit__parameter;
typedef TRadio TimeStampingLayerP__PacketTimeStampRadio__precision_tag;
typedef uint32_t TimeStampingLayerP__PacketTimeStampRadio__size_type;
typedef TRadio TimeStampingLayerP__LocalTimeRadio__precision_tag;
typedef TMilli TimeStampingLayerP__LocalTimeMilli__precision_tag;
typedef TMilli TimeStampingLayerP__PacketTimeStampMilli__precision_tag;
typedef uint32_t TimeStampingLayerP__PacketTimeStampMilli__size_type;
typedef TMicro RF230DriverLayerP__BusyWait__precision_tag;
typedef uint16_t RF230DriverLayerP__BusyWait__size_type;
typedef uint8_t RF230DriverLayerP__PacketRSSI__value_type;
typedef TRadio RF230DriverLayerP__PacketTimeStamp__precision_tag;
typedef uint32_t RF230DriverLayerP__PacketTimeStamp__size_type;
typedef TRadio RF230DriverLayerP__LocalTime__precision_tag;
typedef uint8_t RF230DriverLayerP__PacketTransmitPower__value_type;
typedef uint8_t RF230DriverLayerP__PacketTimeSyncOffset__value_type;
typedef uint8_t RF230DriverLayerP__PacketLinkQuality__value_type;
typedef uint16_t HplRF230P__Capture__size_type;

static void mf_gpio_set(int Pin);
static void mf_gpio_clr(int Pin);
static void mf_gpio_makeOutput(int Pin);
static void mf_gpio_makeInput(int Pin);

static error_t HplRF230P__PlatformInit__init(void);

static error_t RF230DriverLayerP__PlatformInit__init(void);

//# 51 "/opt/tinyos-2.x/tos/interfaces/Init.nc"
static error_t RealMainP$SoftwareInit$init(void);
//# 49 "/opt/tinyos-2.x/tos/interfaces/Boot.nc"
static void RealMainP$Boot$booted(void);
//# 51 "/opt/tinyos-2.x/tos/interfaces/Init.nc"
static error_t RealMainP$PlatformInit$init(void);
//# 46 "/opt/tinyos-2.x/tos/interfaces/Scheduler.nc"
static void RealMainP$Scheduler$init(void);
//#line 61
static void RealMainP$Scheduler$taskLoop(void);
//#line 54
static bool RealMainP$Scheduler$runNextTask(void);
//# 52 "/opt/tinyos-2.x/tos/system/RealMainP.nc"
int main_tinyos(void);
//# 64 "/opt/tinyos-2.x/tos/interfaces/TaskBasic.nc"
static void SchedulerBasicP$TaskBasic$runTask(
//# 45 "/opt/tinyos-2.x/tos/system/SchedulerBasicP.nc"
		uint8_t arg_0x404d8d50);
//# 59 "/opt/tinyos-2.x/tos/interfaces/McuSleep.nc"
static void SchedulerBasicP$McuSleep$sleep(void);
//# 50 "/opt/tinyos-2.x/tos/system/SchedulerBasicP.nc"
enum SchedulerBasicP$__nesc_unnamed4308 {

	SchedulerBasicP$NUM_TASKS = 12U, SchedulerBasicP$NO_TASK = 255
};

uint8_t SchedulerBasicP$m_head;
uint8_t SchedulerBasicP$m_tail;
uint8_t SchedulerBasicP$m_next[SchedulerBasicP$NUM_TASKS];

// Point 2 Nived : Inline function definition begins here

static inline error_t HplRF230P__PlatformInit__init(void);

static inline error_t RF230DriverLayerP__PlatformInit__init(void);

static __inline uint8_t SchedulerBasicP$popTask(void);
//#line 86
static inline bool SchedulerBasicP$isWaiting(uint8_t id);

static inline bool SchedulerBasicP$pushTask(uint8_t id);
//#line 113
static inline void SchedulerBasicP$Scheduler$init(void);

static bool SchedulerBasicP$Scheduler$runNextTask(void);
//#line 138
static inline void SchedulerBasicP$Scheduler$taskLoop(void);
//#line 159
static error_t SchedulerBasicP$TaskBasic$postTask(uint8_t id);

static inline void SchedulerBasicP$TaskBasic$default$runTask(uint8_t id);
//# 53 "/opt/tinyos-2.x/tos/lib/timer/Timer.nc"
static void BlinkC$Timer0$startPeriodic(uint32_t arg_0x40565120);
//#line 53
static void BlinkC$Timer1$startPeriodic(uint32_t arg_0x40565120);
//# 56 "/opt/tinyos-2.x/tos/interfaces/Leds.nc"
static void BlinkC$Leds$led0Toggle(void);
//#line 72
static void BlinkC$Leds$led1Toggle(void);
//#line 89
static void BlinkC$Leds$led2Toggle(void);
//# 53 "/opt/tinyos-2.x/tos/lib/timer/Timer.nc"
static void BlinkC$Timer2$startPeriodic(uint32_t arg_0x40565120);
//# 49 "BlinkC.nc"
static inline void BlinkC$Boot$booted(void);

static inline void BlinkC$Timer0$fired(void);

static inline void BlinkC$Timer1$fired(void);

static inline void BlinkC$Timer2$fired(void);
//# 31 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
static void LedsP$Led0$toggle(void);

static void LedsP$Led0$makeOutput(void);
//#line 29
static void LedsP$Led0$set(void);

static void LedsP$Led1$toggle(void);

static void LedsP$Led1$makeOutput(void);
//#line 29
static void LedsP$Led1$set(void);

static void LedsP$Led2$toggle(void);

static void LedsP$Led2$makeOutput(void);
//#line 29
static void LedsP$Led2$set(void);
//# 45 "/opt/tinyos-2.x/tos/system/LedsP.nc"
static inline error_t LedsP$Init$init(void);
//#line 73
static inline void LedsP$Leds$led0Toggle(void);
//#line 88
static inline void LedsP$Leds$led1Toggle(void);
//#line 103
static inline void LedsP$Leds$led2Toggle(void);
//# 47 "/opt/tinyos-2.x/tos/chips/stm32/gpio/HplSTM32GeneralIOPinP.nc"
static __inline void /*HplSTM32GeneralIOC.PortC.Bit8*/
		HplSTM32GeneralIOPinP$40$IO$set(void);

static inline void /*HplSTM32GeneralIOC.PortC.Bit8*/
		HplSTM32GeneralIOPinP$40$IO$toggle(void);
//#line 82
static __inline void /*HplSTM32GeneralIOC.PortC.Bit8*/
		HplSTM32GeneralIOPinP$40$IO$makeOutput(void);
//#line 47
static __inline void /*HplSTM32GeneralIOC.PortC.Bit9*/
		HplSTM32GeneralIOPinP$41$IO$set(void);

static inline void /*HplSTM32GeneralIOC.PortC.Bit9*/
		HplSTM32GeneralIOPinP$41$IO$toggle(void);
//#line 82
static __inline void /*HplSTM32GeneralIOC.PortC.Bit9*/
		HplSTM32GeneralIOPinP$41$IO$makeOutput(void);
//#line 47
static __inline void /*HplSTM32GeneralIOC.PortC.Bit12*/
		HplSTM32GeneralIOPinP$44$IO$set(void);

static inline void /*HplSTM32GeneralIOC.PortC.Bit12*/
		HplSTM32GeneralIOPinP$44$IO$toggle(void);
//#line 82
static __inline void /*HplSTM32GeneralIOC.PortC.Bit12*/
		HplSTM32GeneralIOPinP$44$IO$makeOutput(void);
//# 56 "/opt/tinyos-2.x/tos/interfaces/TaskBasic.nc"
static error_t /*HilTimerMilliC.VirtTimersMilli32*/
		VirtualizeTimerC$0$updateFromTimer$postTask(void);
//# 125 "/opt/tinyos-2.x/tos/lib/timer/Timer.nc"
static uint32_t /*HilTimerMilliC.VirtTimersMilli32*/
		VirtualizeTimerC$0$TimerFrom$getNow(void);
//#line 118
static void /*HilTimerMilliC.VirtTimersMilli32*/
		VirtualizeTimerC$0$TimerFrom$startOneShotAt(uint32_t arg_0x40563360,
				uint32_t arg_0x405634f0);
//#line 67
static void /*HilTimerMilliC.VirtTimersMilli32*/
		VirtualizeTimerC$0$TimerFrom$stop(void);

static void /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$Timer$fired(
//# 37 "/opt/tinyos-2.x/tos/lib/timer/VirtualizeTimerC.nc"
		uint8_t arg_0x4068b030);
//#line 60
enum /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$__nesc_unnamed4309 {
	//#line 60
	VirtualizeTimerC$0$updateFromTimer = 0U
};
//#line 60
typedef int /*HilTimerMilliC.VirtTimersMilli32*/
		VirtualizeTimerC$0$__nesc_sillytask_updateFromTimer[/*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$updateFromTimer];
//#line 42
enum /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$__nesc_unnamed4310 {

	VirtualizeTimerC$0$NUM_TIMERS = 3U, VirtualizeTimerC$0$END_OF_LIST = 255
};

//#line 48
typedef struct /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$__nesc_unnamed4311 {

	uint32_t t0;
	uint32_t dt;
	bool isoneshot :1;
	bool isrunning :1;
	bool _reserved :6;
} /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$Timer_t;

/*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$Timer_t /*HilTimerMilliC.VirtTimersMilli32*/
		VirtualizeTimerC$0$m_timers[/*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$NUM_TIMERS];

static void /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$fireTimers(
		uint32_t now);
//#line 89
static void /*HilTimerMilliC.VirtTimersMilli32*/
		VirtualizeTimerC$0$updateFromTimer$runTask(void);
//#line 128
static inline void /*HilTimerMilliC.VirtTimersMilli32*/
		VirtualizeTimerC$0$TimerFrom$fired(void);

static inline void /*HilTimerMilliC.VirtTimersMilli32*/
		VirtualizeTimerC$0$startTimer(uint8_t num, uint32_t t0, uint32_t dt,
				bool isoneshot);

static void /*HilTimerMilliC.VirtTimersMilli32*/
		VirtualizeTimerC$0$Timer$startPeriodic(uint8_t num, uint32_t dt);
//#line 193
static inline void /*HilTimerMilliC.VirtTimersMilli32*/
		VirtualizeTimerC$0$Timer$default$fired(uint8_t num);
//# 56 "/opt/tinyos-2.x/tos/interfaces/TaskBasic.nc"
static error_t /*HilTimerMilliC.AlarmToTimerMilli32*/
		AlarmToTimerC$0$fired$postTask(void);
//# 98 "/opt/tinyos-2.x/tos/lib/timer/Alarm.nc"
static/*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Alarm$size_type /*HilTimerMilliC.AlarmToTimerMilli32*/
		AlarmToTimerC$0$Alarm$getNow(void);
//#line 92
static void /*HilTimerMilliC.AlarmToTimerMilli32*/
		AlarmToTimerC$0$Alarm$startAt(
				/*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Alarm$size_type arg_0x406c1678, /*HilTimerMilliC.AlarmToTimerMilli32*/
				AlarmToTimerC$0$Alarm$size_type arg_0x406c1808);
//#line 105
static/*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Alarm$size_type /*HilTimerMilliC.AlarmToTimerMilli32*/
		AlarmToTimerC$0$Alarm$getAlarm(void);
//#line 62
static void /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Alarm$stop(
		void);
//# 72 "/opt/tinyos-2.x/tos/lib/timer/Timer.nc"
static void /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Timer$fired(
		void);
//# 63 "/opt/tinyos-2.x/tos/lib/timer/AlarmToTimerC.nc"
enum /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$__nesc_unnamed4312 {
	//#line 63
	AlarmToTimerC$0$fired = 1U
};
//#line 63
typedef int /*HilTimerMilliC.AlarmToTimerMilli32*/
		AlarmToTimerC$0$__nesc_sillytask_fired[/*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$fired];
//#line 44
uint32_t /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$m_dt;
bool /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$m_oneshot;

static inline void /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$start(
		uint32_t t0, uint32_t dt, bool oneshot);
//#line 60
static inline void /*HilTimerMilliC.AlarmToTimerMilli32*/
		AlarmToTimerC$0$Timer$stop(void);

static void /*HilTimerMilliC.AlarmToTimerMilli32*/
		AlarmToTimerC$0$fired$runTask(void);

static inline void /*HilTimerMilliC.AlarmToTimerMilli32*/
		AlarmToTimerC$0$Alarm$fired(void);
//#line 82
static inline void /*HilTimerMilliC.AlarmToTimerMilli32*/
		AlarmToTimerC$0$Timer$startOneShotAt(uint32_t t0, uint32_t dt);

static inline uint32_t /*HilTimerMilliC.AlarmToTimerMilli32*/
		AlarmToTimerC$0$Timer$getNow(void);
//# 67 "/opt/tinyos-2.x/tos/lib/timer/Alarm.nc"
static void STM32RtcC$Alarm$fired(void);
//# 71 "/opt/tinyos-2.x/tos/lib/timer/Counter.nc"
static void STM32RtcC$Counter$overflow(void);
//# 42 "/opt/tinyos-2.x/tos/chips/stm32/timer/STM32RtcC.nc"

// Radio Glue Code

static void
		MessageBufferLayerP__SplitControl__startDone(error_t arg_0x7eac3198);
//line 138
static void MessageBufferLayerP__SplitControl__stopDone(error_t arg_0x7eac3d48);
// 56 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioState.nc"
static error_t MessageBufferLayerP__RadioState__turnOn(void);
// 54 "/opt/tinyos-2.x/tos/chips/rf2xx/util/BareSend.nc"
static void MessageBufferLayerP__Send__sendDone(message_t *arg_0x7e797010,
		error_t arg_0x7e797198);
// 67 "/opt/tinyos-2.x/tos/interfaces/TaskBasic.nc"
static error_t MessageBufferLayerP__stateDoneTask__postTask(void);
//line 67
static error_t MessageBufferLayerP__sendTask__postTask(void);
// 42 "/opt/tinyos-2.x/tos/chips/rf2xx/util/BareReceive.nc"
static message_t *MessageBufferLayerP__Receive__receive(
		message_t *arg_0x7e7a0ec0);
// 67 "/opt/tinyos-2.x/tos/interfaces/TaskBasic.nc"
static error_t MessageBufferLayerP__deliverTask__postTask(void);
// 48 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioSend.nc"
static error_t MessageBufferLayerP__RadioSend__send(message_t *arg_0x7e390380);
// 48 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioChannel.nc"
static void MessageBufferLayerP__RadioChannel__setChannelDone(void);
// 72 "/opt/tinyos-2.x/tos/chips/rf2xx/util/Tasklet.nc"
static void MessageBufferLayerP__Tasklet__suspend(void);

static void MessageBufferLayerP__Tasklet__resume(void);
// 144 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/MessageBufferLayerP.nc"
enum MessageBufferLayerP____nesc_unnamed4347 {
	//line 144
	MessageBufferLayerP__stateDoneTask = 6U
};
//line 144
typedef int
		MessageBufferLayerP____nesc_sillytask_stateDoneTask[MessageBufferLayerP__stateDoneTask];
//line 189
enum MessageBufferLayerP____nesc_unnamed4348 {
	//line 189
	MessageBufferLayerP__sendTask = 7U
};
//line 189
typedef int
		MessageBufferLayerP____nesc_sillytask_sendTask[MessageBufferLayerP__sendTask];
//line 322
enum MessageBufferLayerP____nesc_unnamed4349 {
	//line 322
	MessageBufferLayerP__deliverTask = 8U
};
//line 322
typedef int
		MessageBufferLayerP____nesc_sillytask_deliverTask[MessageBufferLayerP__deliverTask];
//line 63
uint8_t MessageBufferLayerP__state;
enum MessageBufferLayerP____nesc_unnamed4350 {

	MessageBufferLayerP__STATE_READY = 0,
	MessageBufferLayerP__STATE_TX_PENDING = 1,
	MessageBufferLayerP__STATE_TX_RETRY = 2,
	MessageBufferLayerP__STATE_TX_SEND = 3,
	MessageBufferLayerP__STATE_TX_DONE = 4,
	MessageBufferLayerP__STATE_TURN_ON = 5,
	MessageBufferLayerP__STATE_TURN_OFF = 6,
	MessageBufferLayerP__STATE_CHANNEL = 7
};

static error_t MessageBufferLayerP__SplitControl__start(void);
//line 144
static inline void MessageBufferLayerP__stateDoneTask__runTask(void);
//line 163
static inline void MessageBufferLayerP__RadioState__done(void);
//line 176
static inline void MessageBufferLayerP__RadioChannel__default__setChannelDone(
		void);

message_t *MessageBufferLayerP__txMsg;
error_t MessageBufferLayerP__txError;
uint8_t MessageBufferLayerP__retries;

enum MessageBufferLayerP____nesc_unnamed4351 {
	//line 187
	MessageBufferLayerP__MAX_RETRIES = 5
};
static inline void MessageBufferLayerP__sendTask__runTask(void);
//line 217
static void MessageBufferLayerP__RadioSend__sendDone(error_t error);
//line 230
static error_t MessageBufferLayerP__Send__send(message_t *msg);
//line 252
static void MessageBufferLayerP__RadioSend__ready(void);

static inline void MessageBufferLayerP__Tasklet__run(void);
//line 291
enum MessageBufferLayerP____nesc_unnamed4352 {

	MessageBufferLayerP__RECEIVE_QUEUE_SIZE = 3
};

message_t
		MessageBufferLayerP__receiveQueueData[MessageBufferLayerP__RECEIVE_QUEUE_SIZE];
message_t
		*MessageBufferLayerP__receiveQueue[MessageBufferLayerP__RECEIVE_QUEUE_SIZE];

uint8_t MessageBufferLayerP__receiveQueueHead;
uint8_t MessageBufferLayerP__receiveQueueSize;

static inline error_t MessageBufferLayerP__SoftwareInit__init(void);

static inline bool MessageBufferLayerP__RadioReceive__header(message_t *msg);

static inline void MessageBufferLayerP__deliverTask__runTask(void);
//line 351
static inline message_t *MessageBufferLayerP__RadioReceive__receive(
		message_t *msg);
// 48 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioSend.nc"
static error_t RandomCollisionLayerP__SubSend__send(message_t *arg_0x7e390380);
// 49 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioAlarm.nc"
static void RandomCollisionLayerP__RadioAlarm__wait(uint16_t arg_0x7e8481b8);
//line 44
static bool RandomCollisionLayerP__RadioAlarm__isFree(void);
//line 64
static uint16_t RandomCollisionLayerP__RadioAlarm__getNow(void);
// 52 "/opt/tinyos-2.x/tos/interfaces/Random.nc"
static uint16_t RandomCollisionLayerP__Random__rand16(void);
// 46 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/RandomCollisionConfig.nc"
static uint16_t RandomCollisionLayerP__Config__getCongestionBackoff(
		message_t *arg_0x7e885670);
//line 40
static uint16_t RandomCollisionLayerP__Config__getInitialBackoff(
		message_t *arg_0x7e885140);

static uint16_t RandomCollisionLayerP__Config__getMinimumBackoff(void);

static uint16_t RandomCollisionLayerP__Config__getTransmitBarrier(
		message_t *arg_0x7e884010);
// 53 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioReceive.nc"
static message_t *RandomCollisionLayerP__RadioReceive__receive(
		message_t *arg_0x7e5fbea8);
//line 46
static bool RandomCollisionLayerP__RadioReceive__header(
		message_t *arg_0x7e5fb7d0);
// 63 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioSend.nc"
static void RandomCollisionLayerP__RadioSend__ready(void);
//line 56
static void RandomCollisionLayerP__RadioSend__sendDone(error_t arg_0x7e390a40);
// 67 "/opt/tinyos-2.x/tos/interfaces/TaskBasic.nc"
static error_t RandomCollisionLayerP__calcNextRandom__postTask(void);
// 78 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/RandomCollisionLayerP.nc"
enum RandomCollisionLayerP____nesc_unnamed4353 {
	//line 78
	RandomCollisionLayerP__calcNextRandom = 9U
};
//line 78
typedef int
		RandomCollisionLayerP____nesc_sillytask_calcNextRandom[RandomCollisionLayerP__calcNextRandom];
//line 57
uint8_t RandomCollisionLayerP__state;
enum RandomCollisionLayerP____nesc_unnamed4354 {

	RandomCollisionLayerP__STATE_READY = 0,
	RandomCollisionLayerP__STATE_TX_PENDING_FIRST = 1,
	RandomCollisionLayerP__STATE_TX_PENDING_SECOND = 2,
	RandomCollisionLayerP__STATE_TX_SENDING = 3,

	RandomCollisionLayerP__STATE_BARRIER = 0x80
};

message_t *RandomCollisionLayerP__txMsg;
uint16_t RandomCollisionLayerP__txBarrier;

static inline void RandomCollisionLayerP__SubSend__ready(void);

uint16_t RandomCollisionLayerP__nextRandom;
static inline void RandomCollisionLayerP__calcNextRandom__runTask(void);

static uint16_t RandomCollisionLayerP__getBackoff(uint16_t maxBackoff);
//line 98
static inline error_t RandomCollisionLayerP__RadioSend__send(message_t *msg);
//line 110
static inline void RandomCollisionLayerP__RadioAlarm__fired(void);
//line 155
static inline void RandomCollisionLayerP__SubSend__sendDone(error_t error);

static inline bool RandomCollisionLayerP__SubReceive__header(message_t *msg);

static inline message_t *RandomCollisionLayerP__SubReceive__receive(
		message_t *msg);
// 52 "/opt/tinyos-2.x/tos/system/RandomMlcgC.nc"
uint32_t RandomMlcgC__seed;

static inline error_t RandomMlcgC__Init__init(void);
//line 69
static uint32_t RandomMlcgC__Random__rand32(void);
//line 89
static inline uint16_t RandomMlcgC__Random__rand16(void);
// 48 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioSend.nc"
static error_t SoftwareAckLayerP__SubSend__send(message_t *arg_0x7e390380);
// 49 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioAlarm.nc"
static void SoftwareAckLayerP__RadioAlarm__wait(uint16_t arg_0x7e8481b8);

static void SoftwareAckLayerP__RadioAlarm__cancel(void);
//line 44
static bool SoftwareAckLayerP__RadioAlarm__isFree(void);
// 55 "/opt/tinyos-2.x/tos/chips/rf2xx/util/PacketFlag.nc"
static void
		SoftwareAckLayerP__AckReceivedFlag__clear(message_t *arg_0x7e309868);
//line 40
static bool SoftwareAckLayerP__AckReceivedFlag__get(message_t *arg_0x7e30a758);

static void SoftwareAckLayerP__AckReceivedFlag__setValue(
		message_t *arg_0x7e30ac60, bool arg_0x7e30ade8);
// 53 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioReceive.nc"
static message_t *SoftwareAckLayerP__RadioReceive__receive(
		message_t *arg_0x7e5fbea8);
//line 46
static bool SoftwareAckLayerP__RadioReceive__header(message_t *arg_0x7e5fb7d0);
// 86 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/SoftwareAckConfig.nc"
static void SoftwareAckLayerP__SoftwareAckConfig__reportChannelError(void);
//line 80
static void SoftwareAckLayerP__SoftwareAckConfig__createAckPacket(
		message_t *arg_0x7e8a3678, message_t *arg_0x7e8a3828);
//line 55
static bool SoftwareAckLayerP__SoftwareAckConfig__requiresAckWait(
		message_t *arg_0x7e8a6d68);

static bool SoftwareAckLayerP__SoftwareAckConfig__isAckPacket(
		message_t *arg_0x7e8a4380);

static bool SoftwareAckLayerP__SoftwareAckConfig__verifyAckPacket(
		message_t *arg_0x7e8a4970, message_t *arg_0x7e8a4b20);
//line 43
static uint16_t SoftwareAckLayerP__SoftwareAckConfig__getAckTimeout(void);

static void SoftwareAckLayerP__SoftwareAckConfig__setAckRequired(
		message_t *arg_0x7e8a6698, bool arg_0x7e8a6820);
//line 75
static bool SoftwareAckLayerP__SoftwareAckConfig__requiresAckReply(
		message_t *arg_0x7e8a3140);
// 63 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioSend.nc"
static void SoftwareAckLayerP__RadioSend__ready(void);
//line 56
static void SoftwareAckLayerP__RadioSend__sendDone(error_t arg_0x7e390a40);
// 60 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/SoftwareAckLayerP.nc"
uint8_t SoftwareAckLayerP__state;
enum SoftwareAckLayerP____nesc_unnamed4355 {

	SoftwareAckLayerP__STATE_READY = 0,
	SoftwareAckLayerP__STATE_DATA_SEND = 1,
	SoftwareAckLayerP__STATE_ACK_WAIT = 2,
	SoftwareAckLayerP__STATE_ACK_SEND = 3
};

message_t *SoftwareAckLayerP__txMsg;
message_t SoftwareAckLayerP__ackMsg;

static inline void SoftwareAckLayerP__SubSend__ready(void);

static inline error_t SoftwareAckLayerP__RadioSend__send(message_t *msg);
//line 97
static inline void SoftwareAckLayerP__SubSend__sendDone(error_t error);
//line 124
static inline void SoftwareAckLayerP__RadioAlarm__fired(void);

static inline bool SoftwareAckLayerP__SubReceive__header(message_t *msg);

static inline message_t *SoftwareAckLayerP__SubReceive__receive(message_t *msg);
//line 178
static inline error_t SoftwareAckLayerP__PacketAcknowledgements__requestAck(
		message_t *msg);
//line 192
static inline bool SoftwareAckLayerP__PacketAcknowledgements__wasAcked(
		message_t *msg);
// 49 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioPacket.nc"
static uint8_t MetadataFlagsLayerC__SubPacket__payloadLength(
		message_t *arg_0x7e845a50);

static uint8_t MetadataFlagsLayerC__SubPacket__maxPayloadLength(void);
//line 54
static void MetadataFlagsLayerC__SubPacket__setPayloadLength(
		message_t *arg_0x7e843010, uint8_t arg_0x7e843198);
//line 43
static uint8_t MetadataFlagsLayerC__SubPacket__headerLength(
		message_t *arg_0x7e845478);
//line 65
static uint8_t MetadataFlagsLayerC__SubPacket__metadataLength(
		message_t *arg_0x7e843a08);
// 54 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/MetadataFlagsLayerC.nc"
static inline flags_metadata_t *MetadataFlagsLayerC__getMeta(message_t *msg);

static bool MetadataFlagsLayerC__PacketFlag__get(uint8_t bit, message_t *msg);

static void MetadataFlagsLayerC__PacketFlag__set(uint8_t bit, message_t *msg);

static inline void MetadataFlagsLayerC__PacketFlag__clear(uint8_t bit,
		message_t *msg);

static inline void MetadataFlagsLayerC__PacketFlag__setValue(uint8_t bit,
		message_t *msg, bool value);

static inline uint8_t MetadataFlagsLayerC__RadioPacket__headerLength(
		message_t *msg);

static inline uint8_t MetadataFlagsLayerC__RadioPacket__payloadLength(
		message_t *msg);

static inline void MetadataFlagsLayerC__RadioPacket__setPayloadLength(
		message_t *msg, uint8_t length);

static inline uint8_t MetadataFlagsLayerC__RadioPacket__maxPayloadLength(void);

static inline uint8_t MetadataFlagsLayerC__RadioPacket__metadataLength(
		message_t *msg);
// 55 "/opt/tinyos-2.x/tos/chips/rf2xx/util/PacketFlag.nc"
static void TimeStampingLayerP__TimeStampFlag__clear(message_t *arg_0x7e309868);
//line 50
static void TimeStampingLayerP__TimeStampFlag__set(message_t *arg_0x7e309370);
// 49 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioPacket.nc"
static uint8_t TimeStampingLayerP__SubPacket__payloadLength(
		message_t *arg_0x7e845a50);

static uint8_t TimeStampingLayerP__SubPacket__maxPayloadLength(void);
//line 54
static void TimeStampingLayerP__SubPacket__setPayloadLength(
		message_t *arg_0x7e843010, uint8_t arg_0x7e843198);
//line 43
static uint8_t TimeStampingLayerP__SubPacket__headerLength(
		message_t *arg_0x7e845478);
//line 65
static uint8_t TimeStampingLayerP__SubPacket__metadataLength(
		message_t *arg_0x7e843a08);
// 60 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/TimeStampingLayerP.nc"
static inline timestamp_metadata_t *TimeStampingLayerP__getMeta(message_t *msg);
//line 77
static inline void TimeStampingLayerP__PacketTimeStampRadio__clear(
		message_t *msg);

static inline void TimeStampingLayerP__PacketTimeStampRadio__set(
		message_t *msg, uint32_t value);
//line 116
static inline uint8_t TimeStampingLayerP__RadioPacket__headerLength(
		message_t *msg);

static inline uint8_t TimeStampingLayerP__RadioPacket__payloadLength(
		message_t *msg);

static inline void TimeStampingLayerP__RadioPacket__setPayloadLength(
		message_t *msg, uint8_t length);

static inline uint8_t TimeStampingLayerP__RadioPacket__maxPayloadLength(void);

static inline uint8_t TimeStampingLayerP__RadioPacket__metadataLength(
		message_t *msg);
// 52 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioCCA.nc"
static void RF230DriverLayerP__RadioCCA__done(error_t arg_0x7e357460);
// 66 "/opt/tinyos-2.x/tos/lib/timer/BusyWait.nc"
static void RF230DriverLayerP__BusyWait__wait(
		RF230DriverLayerP__BusyWait__size_type arg_0x7e235010);
// 55 "/opt/tinyos-2.x/tos/chips/rf2xx/util/PacketFlag.nc"
static void RF230DriverLayerP__RSSIFlag__clear(message_t *arg_0x7e309868);
//line 50
static void RF230DriverLayerP__RSSIFlag__set(message_t *arg_0x7e309370);
// 70 "/opt/tinyos-2.x/tos/interfaces/PacketTimeStamp.nc"
static void
		RF230DriverLayerP__PacketTimeStamp__clear(message_t *arg_0x7e945638);

static void RF230DriverLayerP__PacketTimeStamp__set(message_t *arg_0x7e945b60,
		RF230DriverLayerP__PacketTimeStamp__size_type arg_0x7e945cf0);
// 46 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
static void RF230DriverLayerP__RSTN__makeOutput(void);
//line 40
static void RF230DriverLayerP__RSTN__set(void);
static void RF230DriverLayerP__RSTN__clr(void);
// 49 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioAlarm.nc"
static void RF230DriverLayerP__RadioAlarm__wait(uint16_t arg_0x7e8481b8);
//line 44
static bool RF230DriverLayerP__RadioAlarm__isFree(void);
//line 64
static uint16_t RF230DriverLayerP__RadioAlarm__getNow(void);
// 69 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioState.nc"
static void RF230DriverLayerP__RadioState__done(void);
// 61 "/opt/tinyos-2.x/tos/lib/timer/LocalTime.nc"
static uint32_t RF230DriverLayerP__LocalTime__get(void);
// 46 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
static void RF230DriverLayerP__SLP_TR__makeOutput(void);
//line 40
static void RF230DriverLayerP__SLP_TR__set(void);
static void RF230DriverLayerP__SLP_TR__clr(void);
// 46 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230DriverConfig.nc"
static uint8_t RF230DriverLayerP__Config__maxPayloadLength(void);
//line 40
static uint8_t RF230DriverLayerP__Config__headerLength(
		message_t *arg_0x7e8b13e8);
//line 52
static uint8_t RF230DriverLayerP__Config__metadataLength(
		message_t *arg_0x7e8b1ca0);

static uint8_t RF230DriverLayerP__Config__headerPreloadLength(void);

static bool RF230DriverLayerP__Config__requiresRssiCca(
		message_t *arg_0x7e8b0658);
// 53 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioReceive.nc"
static message_t *RF230DriverLayerP__RadioReceive__receive(
		message_t *arg_0x7e5fbea8);
//line 46
static bool RF230DriverLayerP__RadioReceive__header(message_t *arg_0x7e5fb7d0);
// 120 "/opt/tinyos-2.x/tos/interfaces/Resource.nc"
static error_t RF230DriverLayerP__SpiResource__release(void);
//line 97
static error_t RF230DriverLayerP__SpiResource__immediateRequest(void);
//line 88
static error_t RF230DriverLayerP__SpiResource__request(void);
//line 128
static bool RF230DriverLayerP__SpiResource__isOwner(void);
// 46 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
static void RF230DriverLayerP__SELN__makeOutput(void);
//line 40
static void RF230DriverLayerP__SELN__set(void);
static void RF230DriverLayerP__SELN__clr(void);
// 63 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioSend.nc"
static void RF230DriverLayerP__RadioSend__ready(void);
//line 56
static void RF230DriverLayerP__RadioSend__sendDone(error_t arg_0x7e390a40);
// 66 "/opt/tinyos-2.x/tos/interfaces/GpioCapture.nc"
static void RF230DriverLayerP__IRQ__disable(void);
//line 53
static error_t RF230DriverLayerP__IRQ__captureRisingEdge(void);
// 57 "/opt/tinyos-2.x/tos/interfaces/FastSpiByte.nc"
static void RF230DriverLayerP__FastSpiByte__splitWrite(uint8_t arg_0x7e23f530);
//line 69
static uint8_t RF230DriverLayerP__FastSpiByte__splitReadWrite(
		uint8_t arg_0x7e23fe68);
//line 63
static uint8_t RF230DriverLayerP__FastSpiByte__splitRead(void);
//line 76
static uint8_t RF230DriverLayerP__FastSpiByte__write(uint8_t arg_0x7e23e3d8);
// 40 "/opt/tinyos-2.x/tos/chips/rf2xx/util/PacketFlag.nc"
static bool RF230DriverLayerP__TimeSyncFlag__get(message_t *arg_0x7e30a758);
//line 55
static void RF230DriverLayerP__TransmitPowerFlag__clear(
		message_t *arg_0x7e309868);
//line 40
static bool
		RF230DriverLayerP__TransmitPowerFlag__get(message_t *arg_0x7e30a758);
// 59 "/opt/tinyos-2.x/tos/chips/rf2xx/util/Tasklet.nc"
static void RF230DriverLayerP__Tasklet__schedule(void);
// 94 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230DriverLayerP.nc"
static inline rf230_header_t *RF230DriverLayerP__getHeader(message_t *msg);

static void *RF230DriverLayerP__getPayload(message_t *msg);

static rf230_metadata_t *RF230DriverLayerP__getMeta(message_t *msg);

uint8_t RF230DriverLayerP__state;
enum RF230DriverLayerP____nesc_unnamed4356 {

	RF230DriverLayerP__STATE_P_ON = 0,
	RF230DriverLayerP__STATE_SLEEP = 1,
	RF230DriverLayerP__STATE_SLEEP_2_TRX_OFF = 2,
	RF230DriverLayerP__STATE_TRX_OFF = 3,
	RF230DriverLayerP__STATE_TRX_OFF_2_RX_ON = 4,
	RF230DriverLayerP__STATE_RX_ON = 5,
	RF230DriverLayerP__STATE_BUSY_TX_2_RX_ON = 6,
	RF230DriverLayerP__STATE_PLL_ON_2_RX_ON = 7
};

uint8_t RF230DriverLayerP__cmd;
enum RF230DriverLayerP____nesc_unnamed4357 {

	RF230DriverLayerP__CMD_NONE = 0,
	RF230DriverLayerP__CMD_TURNOFF = 1,
	RF230DriverLayerP__CMD_STANDBY = 2,
	RF230DriverLayerP__CMD_TURNON = 3,
	RF230DriverLayerP__CMD_TRANSMIT = 4,
	RF230DriverLayerP__CMD_RECEIVE = 5,
	RF230DriverLayerP__CMD_CCA = 6,
	RF230DriverLayerP__CMD_CHANNEL = 7,
	RF230DriverLayerP__CMD_SIGNAL_DONE = 8,
	RF230DriverLayerP__CMD_DOWNLOAD = 9
};

bool RF230DriverLayerP__radioIrq;

uint8_t RF230DriverLayerP__txPower;
uint8_t RF230DriverLayerP__channel;

message_t *RF230DriverLayerP__rxMsg;
message_t RF230DriverLayerP__rxMsgBuffer;

uint16_t RF230DriverLayerP__capturedTime;

uint8_t RF230DriverLayerP__rssiClear;
uint8_t RF230DriverLayerP__rssiBusy;

enum __nesc_unnamed4306 {
	MICA_PRESCALER_ONE = ATM128_CLK16_DIVIDE_8,
	MICA_DIVIDE_ONE_FOR_32KHZ_LOG2 = 5,
	MICA_PRESCALER_THREE = ATM128_CLK16_DIVIDE_8,
	MICA_DIVIDE_THREE_FOR_MICRO_LOG2 = 0,
	EXT_STANDBY_T0_THRESHOLD = 12
};

static __inline void RF230DriverLayerP__writeRegister(uint8_t reg,
		uint8_t value);
//line 166
static __inline uint8_t RF230DriverLayerP__readRegister(uint8_t reg);
//line 182
enum RF230DriverLayerP____nesc_unnamed4358 {

	RF230DriverLayerP__SLEEP_WAKEUP_TIME = (uint16_t) (880 * (73728UL / 8 / 32)
			* (1 << MICA_DIVIDE_ONE_FOR_32KHZ_LOG2) / 10000UL),
	RF230DriverLayerP__CCA_REQUEST_TIME = (uint16_t) (140 * (73728UL / 8 / 32)
			* (1 << MICA_DIVIDE_ONE_FOR_32KHZ_LOG2) / 10000UL),

	RF230DriverLayerP__TX_SFD_DELAY = (uint16_t) (176 * (73728UL / 8 / 32) * (1
			<< MICA_DIVIDE_ONE_FOR_32KHZ_LOG2) / 10000UL),
	RF230DriverLayerP__RX_SFD_DELAY = (uint16_t) (8 * (73728UL / 8 / 32) * (1
			<< MICA_DIVIDE_ONE_FOR_32KHZ_LOG2) / 10000UL)
};

static inline void RF230DriverLayerP__RadioAlarm__fired(void);
//line 217
static inline error_t RF230DriverLayerP__PlatformInit__init(void);
//line 235
static inline error_t RF230DriverLayerP__SoftwareInit__init(void);

static inline void RF230DriverLayerP__initRadio(void);
//line 269
static inline void RF230DriverLayerP__SpiResource__granted(void);
//line 283
static bool RF230DriverLayerP__isSpiAcquired(void);
//line 323
static __inline void RF230DriverLayerP__changeChannel(void);
//line 341
static __inline void RF230DriverLayerP__changeState(void);
//line 411
static inline error_t RF230DriverLayerP__RadioState__turnOn(void);
//line 428
static error_t RF230DriverLayerP__RadioSend__send(message_t *msg);
//line 586
static inline void RF230DriverLayerP__RadioCCA__default__done(error_t error);

static __inline void RF230DriverLayerP__downloadMessage(void);
//line 672
static inline void RF230DriverLayerP__IRQ__captured(uint16_t time);
//line 685
static inline void RF230DriverLayerP__serviceRadio(void);
//line 843
static inline void RF230DriverLayerP__Tasklet__run(void);
//line 873
static inline uint8_t RF230DriverLayerP__RadioPacket__headerLength(
		message_t *msg);

static uint8_t RF230DriverLayerP__RadioPacket__payloadLength(message_t *msg);

static void RF230DriverLayerP__RadioPacket__setPayloadLength(message_t *msg,
		uint8_t length);

static inline uint8_t RF230DriverLayerP__RadioPacket__maxPayloadLength(void);

static inline uint8_t RF230DriverLayerP__RadioPacket__metadataLength(
		message_t *msg);
//line 911
static inline bool
		RF230DriverLayerP__PacketTransmitPower__isSet(message_t *msg);

static inline uint8_t RF230DriverLayerP__PacketTransmitPower__get(
		message_t *msg);
//line 944
static inline void RF230DriverLayerP__PacketRSSI__clear(message_t *msg);

static inline void RF230DriverLayerP__PacketRSSI__set(message_t *msg,
		uint8_t value);

static inline bool RF230DriverLayerP__PacketTimeSyncOffset__isSet(
		message_t *msg);

static inline uint8_t RF230DriverLayerP__PacketTimeSyncOffset__get(
		message_t *msg);
//line 999
static inline void RF230DriverLayerP__PacketLinkQuality__set(message_t *msg,
		uint8_t value);
// 44 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
static void HplRF230P__PortCLKM__makeInput(void);
//line 41
static void HplRF230P__PortCLKM__clr(void);
// 88 "/opt/tinyos-2.x/tos/chips/atm128/timer/HplAtm128Capture.nc"
static void HplRF230P__Capture__setEdge(bool arg_0x7e153988);
//line 47
static HplRF230P__Capture__size_type HplRF230P__Capture__get(void);
//line 64
static void HplRF230P__Capture__reset(void);

static void HplRF230P__Capture__start(void);

static void HplRF230P__Capture__stop(void);
// 44 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
static void HplRF230P__PortIRQ__makeInput(void);
//line 41
static void HplRF230P__PortIRQ__clr(void);
// 61 "/opt/tinyos-2.x/tos/interfaces/GpioCapture.nc"
static void HplRF230P__IRQ__captured(uint16_t arg_0x7e23aa30);
// 53 "/opt/tinyos-2.x/tos/platforms/iris/chips/rf230/HplRF230P.nc"
static inline error_t HplRF230P__PlatformInit__init(void);

static inline void HplRF230P__Capture__captured(uint16_t time);

uint32_t STM32RtcC$alarm;
bool STM32RtcC$running;

static inline void STM32RtcC$enableInterrupt(void);

static void STM32RtcC$disableInterrupt(void);
//#line 69
static inline error_t STM32RtcC$Init$init(void);
//#line 137
static inline void STM32RtcC$Alarm$stop(void);

static void STM32RtcC$Alarm$startAt(uint32_t t0, uint32_t dt);
//#line 180
static inline uint32_t STM32RtcC$Alarm$getNow(void);

static inline uint32_t STM32RtcC$Alarm$getAlarm(void);
//#line 213
static inline void STM32RtcC$Counter$default$overflow(void);

void RTC_IRQHandler(void);

// Point 3 Nived : Glue Code for the Radio Stack Begins here

static inline error_t ecombine(error_t r1, error_t r2)

{
	return r1 == r2 ? r1 : FAIL;
}

inline static void mf_gpio_makeInput(int Pin) {
	CPU_GPIO_EnableInputPin((GPIO_PIN) Pin, false, NULL, (GPIO_INT_EDGE) 0,
			(GPIO_RESISTOR) 0);
}

inline static void mf_gpio_makeOutput(int Pin) {
	CPU_GPIO_EnableOutputPin((GPIO_PIN) Pin, false);
}

inline static void mf_gpio_clr(int Pin) {
	CPU_GPIO_SetPinState((GPIO_PIN) Pin, false);
}

inline static void mf_gpio_set(int Pin) {
	CPU_GPIO_SetPinState((GPIO_PIN) Pin, true);
}

static inline error_t HplRF230P__PlatformInit__init(void) {
	/*
	 HplRF230P__PortCLKM__makeInput();
	 HplRF230P__PortCLKM__clr();
	 HplRF230P__PortIRQ__makeInput();
	 HplRF230P__PortIRQ__clr();
	 HplRF230P__Capture__stop();
	 */

	//mf_gpio_makeInput(15);
	//mf_gpio_clr(15);
	mf_gpio_makeInput(0);
	mf_gpio_clr(0);
	// Need to implement
	//HplRF230P__Capture__stop();

	return SUCCESS;
}

inline static void RF230DriverLayerP__SELN__set(void) {
	//line 40
	///*HplAtm128GeneralIOC.PortB.Bit0*/HplAtm128GeneralIOPinP$8__IO__set();
	mf_gpio_set(10);

	mf_gpio_clr(88);
	mf_gpio_set(88);
	//line 40
}

inline static void RF230DriverLayerP__SELN__makeOutput(void) {
	//line 46
	///*HplAtm128GeneralIOC.PortB.Bit0*/HplAtm128GeneralIOPinP$8__IO__makeOutput();
	// Nived : SELN make Output
	mf_gpio_makeOutput(10);

	//line 46
}

inline static void RF230DriverLayerP__SELN__clr(void) {
	//line 41
	///*HplAtm128GeneralIOC.PortB.Bit0*/HplAtm128GeneralIOPinP$8__IO__clr();
	mf_gpio_clr(10);

	//line 41
}

inline static void RF230DriverLayerP__SLP_TR__makeOutput(void) {
	//line 46
	// /*HplAtm128GeneralIOC.PortB.Bit7*/HplAtm128GeneralIOPinP$15__IO__makeOutput();
	mf_gpio_makeOutput(14);
	//line 46
}

inline static void RF230DriverLayerP__SLP_TR__clr(void) {
	//line 41
	///*HplAtm128GeneralIOC.PortB.Bit7*/HplAtm128GeneralIOPinP$15__IO__clr();
	mf_gpio_clr(14);
	//line 41
}

inline static void RF230DriverLayerP__SLP_TR__set(void) {
	//line 40
	///*HplAtm128GeneralIOC.PortB.Bit7*/HplAtm128GeneralIOPinP$15__IO__set();
	mf_gpio_set(14);
	//line 40
}

inline static void RF230DriverLayerP__RSTN__makeOutput(void) {
	//line 46
	///*HplAtm128GeneralIOC.PortA.Bit6*/HplAtm128GeneralIOPinP$6__IO__makeOutput();
	mf_gpio_makeOutput(12);
	//line 46
}

inline static void RF230DriverLayerP__RSTN__set(void) {
	//line 40
	///*HplAtm128GeneralIOC.PortA.Bit6*/HplAtm128GeneralIOPinP$6__IO__set();
	mf_gpio_set(12);
	//line 40
}

inline static void RF230DriverLayerP__RSTN__clr(void) {
	//line 41
	///*HplAtm128GeneralIOC.PortA.Bit6*/HplAtm128GeneralIOPinP$6__IO__clr();
	mf_gpio_clr(12);
	//line 41
}

static inline error_t RF230DriverLayerP__PlatformInit__init(void) {

	RF230DriverLayerP__SELN__makeOutput();
	RF230DriverLayerP__SELN__set();
	RF230DriverLayerP__SLP_TR__makeOutput();
	RF230DriverLayerP__SLP_TR__clr();
	RF230DriverLayerP__RSTN__makeOutput();
	RF230DriverLayerP__RSTN__set();

	// Nived : Changing the pins to the values below. Lets see how this works

	/*
	 // SELN
	 mf_gpio_makeOutput(17);
	 mf_gpio_set(17);
	 // SLP_TR
	 mf_gpio_makeOutput(18);
	 mf_gpio_clr(18);
	 // RSTN
	 mf_gpio_makeOutput(19);
	 mf_gpio_set(19);
	 */

	RF230DriverLayerP__rxMsg = &RF230DriverLayerP__rxMsgBuffer;

	RF230DriverLayerP__rssiClear = 0;
	RF230DriverLayerP__rssiBusy = 90;

	return SUCCESS;
}

//# 39 "/opt/tinyos-2.x/tos/chips/stm32/stm32hardware.h"
__inline  __nesc_atomic_t  __nesc_atomic_start(void )
{
uint32_t result = 0;
uint32_t temp = 0;

return result;
}

__inline void __nesc_atomic_end(__nesc_atomic_t oldState) {
	uint32_t statusReg = 0;

	//#line 72
	return;
}

//# 56 "/opt/tinyos-2.x/tos/interfaces/TaskBasic.nc"
inline static error_t /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$fired$postTask(
		void) {
	//#line 56
	unsigned char result;
	//#line 56

	//#line 56
	result = SchedulerBasicP$TaskBasic$postTask(
			/*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$fired);
	//#line 56

	//#line 56
	return result;
	//#line 56
}
//#line 56
//# 70 "/opt/tinyos-2.x/tos/lib/timer/AlarmToTimerC.nc"
static inline void /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Alarm$fired(
		void) {
	//#line 71
	/*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$fired$postTask();
}

//# 67 "/opt/tinyos-2.x/tos/lib/timer/Alarm.nc"
inline static void STM32RtcC$Alarm$fired(void) {
	//#line 67
	/*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Alarm$fired();
	//#line 67
}
//#line 67
//# 86 "/opt/tinyos-2.x/tos/system/SchedulerBasicP.nc"
static inline bool SchedulerBasicP$isWaiting(uint8_t id) {
	return SchedulerBasicP$m_next[id] != SchedulerBasicP$NO_TASK
			|| SchedulerBasicP$m_tail == id;
}

static inline bool SchedulerBasicP$pushTask(uint8_t id) {
	if (!SchedulerBasicP$isWaiting(id)) {
		if (SchedulerBasicP$m_head == SchedulerBasicP$NO_TASK) {
			SchedulerBasicP$m_head = id;
			SchedulerBasicP$m_tail = id;
		} else {
			SchedulerBasicP$m_next[SchedulerBasicP$m_tail] = id;
			SchedulerBasicP$m_tail = id;
		}
		return TRUE;
	} else {
		return FALSE;
	}
}

//# 213 "/opt/tinyos-2.x/tos/chips/stm32/timer/STM32RtcC.nc"
static inline void STM32RtcC$Counter$default$overflow(void)
//#line 213
{
	return;
}

//# 71 "/opt/tinyos-2.x/tos/lib/timer/Counter.nc"
inline static void STM32RtcC$Counter$overflow(void) {
	//#line 71
	STM32RtcC$Counter$default$overflow();
	//#line 71
}
//#line 71
//# 104 "/opt/tinyos-2.x/tos/platforms/stm32-p103/PlatformP.nc"
static inline void PlatformP$Interrupt$fired(void) {
}

//# 31 "/opt/tinyos-2.x/tos/chips/stm32/HplSTM32Interrupt.nc"
inline static void HplSTM32InterruptM$Irq$fired(void) {
	//#line 31
	PlatformP$Interrupt$fired();
	//#line 31
}
//#line 31
//# 113 "/opt/tinyos-2.x/tos/system/SchedulerBasicP.nc"
static inline void SchedulerBasicP$Scheduler$init(void) {
	/* atomic removed: atomic calls only */
	{
		memset((void *) SchedulerBasicP$m_next, SchedulerBasicP$NO_TASK,
				sizeof SchedulerBasicP$m_next);
		SchedulerBasicP$m_head = SchedulerBasicP$NO_TASK;
		SchedulerBasicP$m_tail = SchedulerBasicP$NO_TASK;
	}
}

//# 46 "/opt/tinyos-2.x/tos/interfaces/Scheduler.nc"
inline static void RealMainP$Scheduler$init(void) {
	//#line 46
	SchedulerBasicP$Scheduler$init();
	//#line 46
}
//#line 46
//# 47 "/opt/tinyos-2.x/tos/chips/stm32/gpio/HplSTM32GeneralIOPinP.nc"
static __inline void /*HplSTM32GeneralIOC.PortC.Bit9*/HplSTM32GeneralIOPinP$41$IO$set(
		void)
//#line 47
{
	GPIO_TypeDef *port = (GPIO_TypeDef *) 1073811456U;

	//#line 49
	*(volatile int32_t *) ((uint32_t) 0x42000000 + ((uint32_t) &port->ODR
			- (uint32_t) 0x40000000) * 32 + 9 * 4) = 1;
}

//# 29 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
inline static void LedsP$Led2$set(void) {
	//#line 29
	/*HplSTM32GeneralIOC.PortC.Bit9*/HplSTM32GeneralIOPinP$41$IO$set();
	//#line 29
}
//#line 29
//# 47 "/opt/tinyos-2.x/tos/chips/stm32/gpio/HplSTM32GeneralIOPinP.nc"
static __inline void /*HplSTM32GeneralIOC.PortC.Bit8*/HplSTM32GeneralIOPinP$40$IO$set(
		void)
//#line 47
{
	GPIO_TypeDef *port = (GPIO_TypeDef *) 1073811456U;

	//#line 49
	*(volatile int32_t *) ((uint32_t) 0x42000000 + ((uint32_t) &port->ODR
			- (uint32_t) 0x40000000) * 32 + 8 * 4) = 1;
}

//# 29 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
inline static void LedsP$Led1$set(void) {
	//#line 29
	/*HplSTM32GeneralIOC.PortC.Bit8*/HplSTM32GeneralIOPinP$40$IO$set();
	//#line 29
}
//#line 29
//# 47 "/opt/tinyos-2.x/tos/chips/stm32/gpio/HplSTM32GeneralIOPinP.nc"
static __inline void /*HplSTM32GeneralIOC.PortC.Bit12*/HplSTM32GeneralIOPinP$44$IO$set(
		void)
//#line 47
{
	GPIO_TypeDef *port = (GPIO_TypeDef *) 1073811456U;

	//#line 49
	*(volatile int32_t *) ((uint32_t) 0x42000000 + ((uint32_t) &port->ODR
			- (uint32_t) 0x40000000) * 32 + 12 * 4) = 1;
}

//# 29 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
inline static void LedsP$Led0$set(void) {
	//#line 29
	/*HplSTM32GeneralIOC.PortC.Bit12*/HplSTM32GeneralIOPinP$44$IO$set();
	//#line 29
}
//#line 29
//# 82 "/opt/tinyos-2.x/tos/chips/stm32/gpio/HplSTM32GeneralIOPinP.nc"
static __inline void /*HplSTM32GeneralIOC.PortC.Bit9*/HplSTM32GeneralIOPinP$41$IO$makeOutput(
		void)
//#line 82
{
	GPIO_TypeDef *port = (GPIO_TypeDef *) 1073811456U;

	GPIO_InitTypeDef gpioi = { (uint16_t) 1 << 9, GPIO_Speed_10MHz,
			GPIO_Mode_Out_PP };

	GPIO_Init(port, &gpioi);
}

//# 35 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
inline static void LedsP$Led2$makeOutput(void) {
	//#line 35
	/*HplSTM32GeneralIOC.PortC.Bit9*/HplSTM32GeneralIOPinP$41$IO$makeOutput();
	//#line 35
}
//#line 35
//# 82 "/opt/tinyos-2.x/tos/chips/stm32/gpio/HplSTM32GeneralIOPinP.nc"
static __inline void /*HplSTM32GeneralIOC.PortC.Bit8*/HplSTM32GeneralIOPinP$40$IO$makeOutput(
		void)
//#line 82
{
	GPIO_TypeDef *port = (GPIO_TypeDef *) 1073811456U;

	GPIO_InitTypeDef gpioi = { (uint16_t) 1 << 8, GPIO_Speed_10MHz,
			GPIO_Mode_Out_PP };

	GPIO_Init(port, &gpioi);
}

//# 35 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
inline static void LedsP$Led1$makeOutput(void) {
	//#line 35
	/*HplSTM32GeneralIOC.PortC.Bit8*/HplSTM32GeneralIOPinP$40$IO$makeOutput();
	//#line 35
}
//#line 35
//# 82 "/opt/tinyos-2.x/tos/chips/stm32/gpio/HplSTM32GeneralIOPinP.nc"
static __inline void /*HplSTM32GeneralIOC.PortC.Bit12*/HplSTM32GeneralIOPinP$44$IO$makeOutput(
		void)
//#line 82
{
	GPIO_TypeDef *port = (GPIO_TypeDef *) 1073811456U;

	GPIO_InitTypeDef gpioi = { (uint16_t) 1 << 12, GPIO_Speed_10MHz,
			GPIO_Mode_Out_PP };

	GPIO_Init(port, &gpioi);
}

//# 35 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
inline static void LedsP$Led0$makeOutput(void) {
	//#line 35
	/*HplSTM32GeneralIOC.PortC.Bit12*/HplSTM32GeneralIOPinP$44$IO$makeOutput();
	//#line 35
}
//#line 35
//# 45 "/opt/tinyos-2.x/tos/system/LedsP.nc"
static inline error_t LedsP$Init$init(void)
//#line 45
{
	/* atomic removed: atomic calls only */
	//#line 46
	{
		;
		LedsP$Led0$makeOutput();
		LedsP$Led1$makeOutput();
		LedsP$Led2$makeOutput();
		LedsP$Led0$set();
		LedsP$Led1$set();
		LedsP$Led2$set();
	}
	return SUCCESS;
}

//# 51 "/opt/tinyos-2.x/tos/interfaces/Init.nc"
inline static error_t PlatformP$MoteInit$init(void) {
	//#line 51
	unsigned char result;
	//#line 51

	//#line 51
	result = LedsP$Init$init();
	//#line 51

	//#line 51
	return result;
	//#line 51
}
//#line 51
//# 43 "/opt/tinyos-2.x/tos/chips/stm32/McuSleepC.nc"
static inline error_t McuSleepC$McuSleepInit$init(void)
//#line 43
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	EXTI_ClearITPendingBit((uint32_t) 0x20000);
	EXTI_InitStructure.EXTI_Line = (uint32_t) 0x20000;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = (uint8_t) 0x29;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	return SUCCESS;
}

//# 51 "/opt/tinyos-2.x/tos/interfaces/Init.nc"
inline static error_t PlatformP$McuSleepInit$init(void) {
	//#line 51
	unsigned char result;
	//#line 51

	//#line 51
	result = McuSleepC$McuSleepInit$init();
	//#line 51

	//#line 51
	return result;
	//#line 51
}
//#line 51
//# 37 "/opt/tinyos-2.x/tos/platforms/stm32-p103/MoteClockP.nc"
static inline error_t MoteClockP$MoteClockInit$init(void) {

	RCC_HSICmd(ENABLE);

	while (RCC_GetFlagStatus((uint8_t) 0x20) == RESET)
		;
	RCC_SYSCLKConfig((uint32_t) 0x00000000);

	RCC_HSEConfig((uint32_t) 0x00010000);

	while (RCC_GetFlagStatus((uint8_t) 0x31) == RESET)
		;

	RCC_PLLConfig((uint32_t) 0x00010000, (uint32_t) 0x001C0000);

	RCC_PLLCmd(ENABLE);

	while (RCC_GetFlagStatus((uint8_t) 0x39) == RESET)
		;

	RCC_USBCLKConfig((uint8_t) 0x00);
	RCC_ADCCLKConfig((uint32_t) 0x0000C000);
	RCC_PCLK2Config((uint32_t) 0x00000000);
	RCC_PCLK1Config((uint32_t) 0x00000400);
	RCC_HCLKConfig((uint32_t) 0x00000000);

	*(volatile uint32_t *) 0x40022000 = 0x12;

	RCC_SYSCLKConfig((uint32_t) 0x00000002);

	return SUCCESS;
}

//# 51 "/opt/tinyos-2.x/tos/interfaces/Init.nc"
inline static error_t PlatformP$MoteClockInit$init(void) {
	//#line 51
	unsigned char result;
	//#line 51

	//#line 51
	result = MoteClockP$MoteClockInit$init();
	//#line 51

	//#line 51
	return result;
	//#line 51
}
//#line 51
//# 46 "/opt/tinyos-2.x/tos/platforms/stm32-p103/PlatformP.nc"
static inline error_t PlatformP$Init$init(void)
//#line 46
{

	GPIO_InitTypeDef gpioa = { (uint16_t) 0xFFFF, GPIO_Speed_2MHz,
			GPIO_Mode_Out_OD };

	GPIO_Init((GPIO_TypeDef *) ((uint32_t) 0x40000000 + 0x10000 + 0x0800),
			&gpioa);
	((GPIO_TypeDef *) ((uint32_t) 0x40000000 + 0x10000 + 0x0800))->ODR = 0;
	GPIO_Init((GPIO_TypeDef *) ((uint32_t) 0x40000000 + 0x10000 + 0x0C00),
			&gpioa);
	((GPIO_TypeDef *) ((uint32_t) 0x40000000 + 0x10000 + 0x0C00))->ODR = 0;
	GPIO_Init((GPIO_TypeDef *) ((uint32_t) 0x40000000 + 0x10000 + 0x1000),
			&gpioa);
	((GPIO_TypeDef *) ((uint32_t) 0x40000000 + 0x10000 + 0x1000))->ODR = 0;
	GPIO_Init((GPIO_TypeDef *) ((uint32_t) 0x40000000 + 0x10000 + 0x1400),
			&gpioa);
	((GPIO_TypeDef *) ((uint32_t) 0x40000000 + 0x10000 + 0x1400))->ODR = 0;

	*(volatile unsigned long *) 0xE000ED14
			= *(volatile unsigned long *) 0xE000ED14 | 0x200;

	PlatformP$MoteClockInit$init();
	PlatformP$McuSleepInit$init();

	RCC_APB2PeriphClockCmd((uint32_t) 0x00000001, ENABLE);
	RCC_APB2PeriphClockCmd((uint32_t) 0x00000010 | (uint32_t) 0x00000004,
			ENABLE);

	PlatformP$MoteInit$init();

	return SUCCESS;
}

//# 51 "/opt/tinyos-2.x/tos/interfaces/Init.nc"
inline static error_t RealMainP$PlatformInit$init(void) {
	//#line 51
	unsigned char result;
	//#line 51

	//#line 51

	// Nived : I have added the iris platform Init for the radio stack init

	result = HplRF230P__PlatformInit__init();

	result = ecombine(result, RF230DriverLayerP__PlatformInit__init());

	result = ecombine(result, PlatformP$Init$init());

	//result = PlatformP$Init$init();
	//#line 51

	//#line 51
	return result;
	//#line 51
}
//#line 51
//# 54 "/opt/tinyos-2.x/tos/interfaces/Scheduler.nc"
inline static bool RealMainP$Scheduler$runNextTask(void) {
	//#line 54
	unsigned char result;
	//#line 54

	//#line 54
	result = SchedulerBasicP$Scheduler$runNextTask();
	//#line 54

	//#line 54
	return result;
	//#line 54
}
//#line 54
//# 45 "/opt/tinyos-2.x/tos/chips/stm32/timer/STM32RtcC.nc"
static inline void STM32RtcC$enableInterrupt(void) {

	{
		__nesc_atomic_t  __nesc_atomic = __nesc_atomic_start();
		//#line 48
		{
			RTC_ITConfig((uint16_t) 0x0002, ENABLE);
			RTC_WaitForLastTask();
			STM32RtcC$running = TRUE;
		}
		//#line 52
		__nesc_atomic_end(__nesc_atomic);
	}
}

//#line 187
static inline uint32_t STM32RtcC$Alarm$getAlarm(void) {
	return STM32RtcC$alarm;
}

//# 105 "/opt/tinyos-2.x/tos/lib/timer/Alarm.nc"
inline static/*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Alarm$size_type /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Alarm$getAlarm(
		void) {
	//#line 105
	unsigned long result;
	//#line 105

	//#line 105
	result = STM32RtcC$Alarm$getAlarm();
	//#line 105

	//#line 105
	return result;
	//#line 105
}
//#line 105
//# 180 "/opt/tinyos-2.x/tos/chips/stm32/timer/STM32RtcC.nc"
static inline uint32_t STM32RtcC$Alarm$getNow(void) {
	uint32_t c;

	//#line 183
	c = RTC_GetCounter();
	return c;
}

//# 98 "/opt/tinyos-2.x/tos/lib/timer/Alarm.nc"
inline static/*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Alarm$size_type /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Alarm$getNow(
		void) {
	//#line 98
	unsigned long result;
	//#line 98

	//#line 98
	result = STM32RtcC$Alarm$getNow();
	//#line 98

	//#line 98
	return result;
	//#line 98
}
//#line 98
//# 85 "/opt/tinyos-2.x/tos/lib/timer/AlarmToTimerC.nc"
static inline uint32_t /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Timer$getNow(
		void) {
	//#line 86
	return /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Alarm$getNow();
}

//# 125 "/opt/tinyos-2.x/tos/lib/timer/Timer.nc"
inline static uint32_t /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$TimerFrom$getNow(
		void) {
	//#line 125
	unsigned long result;
	//#line 125

	//#line 125
	result
			= /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Timer$getNow();
	//#line 125

	//#line 125
	return result;
	//#line 125
}
//#line 125
//# 128 "/opt/tinyos-2.x/tos/lib/timer/VirtualizeTimerC.nc"
static inline void /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$TimerFrom$fired(
		void) {
	/*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$fireTimers(
			/*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$TimerFrom$getNow());
}

//# 72 "/opt/tinyos-2.x/tos/lib/timer/Timer.nc"
inline static void /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Timer$fired(
		void) {
	//#line 72
	/*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$TimerFrom$fired();
	//#line 72
}
//#line 72
//# 58 "/opt/tinyos-2.x/tos/chips/stm32/gpio/HplSTM32GeneralIOPinP.nc"
static inline void /*HplSTM32GeneralIOC.PortC.Bit12*/HplSTM32GeneralIOPinP$44$IO$toggle(
		void)
//#line 58
{
	GPIO_TypeDef *port = (GPIO_TypeDef *) 1073811456U;

	*(volatile int32_t *) ((uint32_t) 0x42000000 + ((uint32_t) &port->ODR
			- (uint32_t) 0x40000000) * 32 + 12 * 4) ^= 1;
}

//# 31 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
inline static void LedsP$Led0$toggle(void) {
	//#line 31
	/*HplSTM32GeneralIOC.PortC.Bit12*/HplSTM32GeneralIOPinP$44$IO$toggle();
	//#line 31
}
//#line 31
//# 73 "/opt/tinyos-2.x/tos/system/LedsP.nc"
static inline void LedsP$Leds$led0Toggle(void)
//#line 73
{
	LedsP$Led0$toggle();;
	//#line 75
	;
}

//# 56 "/opt/tinyos-2.x/tos/interfaces/Leds.nc"
inline static void BlinkC$Leds$led0Toggle(void) {
	//#line 56
	LedsP$Leds$led0Toggle();
	//#line 56
}
//#line 56
//# 56 "BlinkC.nc"
static inline void BlinkC$Timer0$fired(void) {
	;
	BlinkC$Leds$led0Toggle();
}

//# 58 "/opt/tinyos-2.x/tos/chips/stm32/gpio/HplSTM32GeneralIOPinP.nc"
static inline void /*HplSTM32GeneralIOC.PortC.Bit8*/HplSTM32GeneralIOPinP$40$IO$toggle(
		void)
//#line 58
{
	GPIO_TypeDef *port = (GPIO_TypeDef *) 1073811456U;

	*(volatile int32_t *) ((uint32_t) 0x42000000 + ((uint32_t) &port->ODR
			- (uint32_t) 0x40000000) * 32 + 8 * 4) ^= 1;
}

//# 31 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
inline static void LedsP$Led1$toggle(void) {
	//#line 31
	/*HplSTM32GeneralIOC.PortC.Bit8*/HplSTM32GeneralIOPinP$40$IO$toggle();
	//#line 31
}
//#line 31
//# 88 "/opt/tinyos-2.x/tos/system/LedsP.nc"
static inline void LedsP$Leds$led1Toggle(void)
//#line 88
{
	LedsP$Led1$toggle();;
	//#line 90
	;
}

//# 72 "/opt/tinyos-2.x/tos/interfaces/Leds.nc"
inline static void BlinkC$Leds$led1Toggle(void) {
	//#line 72
	LedsP$Leds$led1Toggle();
	//#line 72
}
//#line 72
//# 62 "BlinkC.nc"
static inline void BlinkC$Timer1$fired(void) {
	;
	BlinkC$Leds$led1Toggle();
}

//# 58 "/opt/tinyos-2.x/tos/chips/stm32/gpio/HplSTM32GeneralIOPinP.nc"
static inline void /*HplSTM32GeneralIOC.PortC.Bit9*/HplSTM32GeneralIOPinP$41$IO$toggle(
		void)
//#line 58
{
	GPIO_TypeDef *port = (GPIO_TypeDef *) 1073811456U;

	*(volatile int32_t *) ((uint32_t) 0x42000000 + ((uint32_t) &port->ODR
			- (uint32_t) 0x40000000) * 32 + 9 * 4) ^= 1;
}

//# 31 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
inline static void LedsP$Led2$toggle(void) {
	//#line 31
	/*HplSTM32GeneralIOC.PortC.Bit9*/HplSTM32GeneralIOPinP$41$IO$toggle();
	//#line 31
}
//#line 31
//# 103 "/opt/tinyos-2.x/tos/system/LedsP.nc"
static inline void LedsP$Leds$led2Toggle(void)
//#line 103
{
	LedsP$Led2$toggle();;
	//#line 105
	;
}

//# 89 "/opt/tinyos-2.x/tos/interfaces/Leds.nc"
inline static void BlinkC$Leds$led2Toggle(void) {
	//#line 89
	LedsP$Leds$led2Toggle();
	//#line 89
}
//#line 89
//# 68 "BlinkC.nc"
static inline void BlinkC$Timer2$fired(void) {
	;
	BlinkC$Leds$led2Toggle();
}

//# 193 "/opt/tinyos-2.x/tos/lib/timer/VirtualizeTimerC.nc"
static inline void /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$Timer$default$fired(
		uint8_t num) {
}

//# 72 "/opt/tinyos-2.x/tos/lib/timer/Timer.nc"
inline static void /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$Timer$fired(
		uint8_t arg_0x4068b030) {
	//#line 72
	switch (arg_0x4068b030) {
	//#line 72
	case 0U:
		//#line 72
		BlinkC$Timer0$fired();
		//#line 72
		break;
		//#line 72
	case 1U:
		//#line 72
		BlinkC$Timer1$fired();
		//#line 72
		break;
		//#line 72
	case 2U:
		//#line 72
		BlinkC$Timer2$fired();
		//#line 72
		break;
		//#line 72
	default:
		//#line 72
		/*HilTimerMilliC.VirtTimersMilli32*/
		VirtualizeTimerC$0$Timer$default$fired(arg_0x4068b030);
		//#line 72
		break;
		//#line 72
	}
	//#line 72
}
//#line 72
//# 137 "/opt/tinyos-2.x/tos/chips/stm32/timer/STM32RtcC.nc"
static inline void STM32RtcC$Alarm$stop(void) {
	STM32RtcC$disableInterrupt();
}

//# 62 "/opt/tinyos-2.x/tos/lib/timer/Alarm.nc"
inline static void /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Alarm$stop(
		void) {
	//#line 62
	STM32RtcC$Alarm$stop();
	//#line 62
}
//#line 62
//# 60 "/opt/tinyos-2.x/tos/lib/timer/AlarmToTimerC.nc"
static inline void /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Timer$stop(
		void) {
	//#line 61
	/*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Alarm$stop();
}

//# 67 "/opt/tinyos-2.x/tos/lib/timer/Timer.nc"
inline static void /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$TimerFrom$stop(
		void) {
	//#line 67
	/*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Timer$stop();
	//#line 67
}
//#line 67
//# 92 "/opt/tinyos-2.x/tos/lib/timer/Alarm.nc"
inline static void /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Alarm$startAt(
		/*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Alarm$size_type arg_0x406c1678, /*HilTimerMilliC.AlarmToTimerMilli32*/
		AlarmToTimerC$0$Alarm$size_type arg_0x406c1808) {
	//#line 92
	STM32RtcC$Alarm$startAt(arg_0x406c1678, arg_0x406c1808);
	//#line 92
}
//#line 92
//# 47 "/opt/tinyos-2.x/tos/lib/timer/AlarmToTimerC.nc"
static inline void /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$start(
		uint32_t t0, uint32_t dt, bool oneshot) {
	/*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$m_dt = dt;
	/*HilTimerMilliC.AlarmToTimerMilli32*/
	AlarmToTimerC$0$m_oneshot = oneshot;
	/*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Alarm$startAt(t0, dt);
}

//#line 82
static inline void /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Timer$startOneShotAt(
		uint32_t t0, uint32_t dt) {
	//#line 83
	/*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$start(t0, dt, TRUE);
}

//# 118 "/opt/tinyos-2.x/tos/lib/timer/Timer.nc"
inline static void /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$TimerFrom$startOneShotAt(
		uint32_t arg_0x40563360, uint32_t arg_0x405634f0) {
	//#line 118
	/*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Timer$startOneShotAt(
			arg_0x40563360, arg_0x405634f0);
	//#line 118
}
//#line 118
//# 69 "/opt/tinyos-2.x/tos/chips/stm32/timer/STM32RtcC.nc"
static inline error_t STM32RtcC$Init$init(void) {
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_PriorityGroupConfig((uint32_t) 0x600);

	NVIC_InitStructure.NVIC_IRQChannel = (uint8_t) 0x03;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	RCC_APB1PeriphClockCmd((uint32_t) 0x10000000 | (uint32_t) 0x08000000,
			ENABLE);

	PWR_BackupAccessCmd(ENABLE);

	BKP_DeInit();

	RCC_LSEConfig((uint8_t) 0x01);

	while (RCC_GetFlagStatus((uint8_t) 0x41) == RESET) {
	}

	RCC_RTCCLKConfig((uint32_t) 0x00000100);

	RCC_RTCCLKCmd(ENABLE);

	RTC_WaitForSynchro();

	RTC_WaitForLastTask();

	RTC_SetPrescaler(31);

	RTC_WaitForLastTask();

	RTC_SetCounter(0x0);

	RTC_WaitForLastTask();

	RTC_ITConfig((uint16_t) 0x0004, ENABLE);

	RTC_WaitForLastTask();
	/* atomic removed: atomic calls only */
	STM32RtcC$alarm = 0;

	return SUCCESS;
}

//# 51 "/opt/tinyos-2.x/tos/interfaces/Init.nc"
inline static error_t RealMainP$SoftwareInit$init(void) {
	//#line 51
	unsigned char result;
	//#line 51

	//#line 51
	result = STM32RtcC$Init$init();
	//#line 51

	//#line 51
	return result;
	//#line 51
}
//#line 51
//# 75 "/opt/tinyos-2.x/tos/chips/stm32/stm32hardware.h"
static __inline void __nesc_enable_interrupt(void)
//#line 75
{
	uint32_t statusReg = 0;

	return;
}

//# 53 "/opt/tinyos-2.x/tos/lib/timer/Timer.nc"
inline static void BlinkC$Timer2$startPeriodic(uint32_t arg_0x40565120) {
	//#line 53
	/*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$Timer$startPeriodic(
			2U, arg_0x40565120);
	//#line 53
}
//#line 53
inline static void BlinkC$Timer1$startPeriodic(uint32_t arg_0x40565120) {
	//#line 53
	/*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$Timer$startPeriodic(
			1U, arg_0x40565120);
	//#line 53
}
//#line 53
inline static void BlinkC$Timer0$startPeriodic(uint32_t arg_0x40565120) {
	//#line 53
	/*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$Timer$startPeriodic(
			0U, arg_0x40565120);
	//#line 53
}
//#line 53
//# 49 "BlinkC.nc"
static inline void BlinkC$Boot$booted(void) {
	BlinkC$Timer0$startPeriodic(250);
	BlinkC$Timer1$startPeriodic(500);
	BlinkC$Timer2$startPeriodic(1000);
}

//# 49 "/opt/tinyos-2.x/tos/interfaces/Boot.nc"
inline static void RealMainP$Boot$booted(void) {
	//#line 49
	BlinkC$Boot$booted();
	//#line 49
}
//#line 49
//# 56 "/opt/tinyos-2.x/tos/interfaces/TaskBasic.nc"
inline static error_t /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$updateFromTimer$postTask(
		void) {
	//#line 56
	unsigned char result;
	//#line 56

	//#line 56
	result
			= SchedulerBasicP$TaskBasic$postTask(
					/*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$updateFromTimer);
	//#line 56

	//#line 56
	return result;
	//#line 56
}
//#line 56
//# 133 "/opt/tinyos-2.x/tos/lib/timer/VirtualizeTimerC.nc"
static inline void /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$startTimer(
		uint8_t num, uint32_t t0, uint32_t dt, bool isoneshot) {
	/*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$Timer_t
			*timer =
					&/*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$m_timers[num];

	//#line 136
	timer->t0 = t0;
	timer->dt = dt;
	timer->isoneshot = isoneshot;
	timer->isrunning = TRUE;
	/*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$updateFromTimer$postTask();
}

//# 164 "/opt/tinyos-2.x/tos/system/SchedulerBasicP.nc"
static inline void SchedulerBasicP$TaskBasic$default$runTask(uint8_t id) {
}

//# 64 "/opt/tinyos-2.x/tos/interfaces/TaskBasic.nc"
inline static void SchedulerBasicP$TaskBasic$runTask(uint8_t arg_0x404d8d50) {
	//#line 64
	switch (arg_0x404d8d50) {
	//#line 64
	case /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$updateFromTimer:
		//#line 64
		/*HilTimerMilliC.VirtTimersMilli32*/
		VirtualizeTimerC$0$updateFromTimer$runTask();
		//#line 64
		break;
		//#line 64
	case /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$fired:
		//#line 64
		/*HilTimerMilliC.AlarmToTimerMilli32*/
		AlarmToTimerC$0$fired$runTask();
		//#line 64
		break;
		//#line 64
	default:
		//#line 64
		SchedulerBasicP$TaskBasic$default$runTask(arg_0x404d8d50);
		//#line 64
		break;
		//#line 64
	}
	//#line 64
}
//#line 64
//# 65 "/opt/tinyos-2.x/tos/chips/stm32/McuSleepC.nc"
static inline void McuSleepC$McuSleep$sleep(void)
//#line 65
{
	//#line 87
	RCC_APB2PeriphClockCmd((uint32_t) 0x00000001 | (uint32_t) 0x00000004,
			DISABLE);
	RCC_AHBPeriphClockCmd((uint32_t) 0x00000010 | (uint32_t) 0x00000004,
			DISABLE);

	RCC_HSICmd(ENABLE);

	while (RCC_GetFlagStatus((uint8_t) 0x20) == RESET)
		;
	RCC_SYSCLKConfig((uint32_t) 0x00000000);

	RCC_PLLCmd(DISABLE);

	RCC_HSEConfig((uint32_t) 0x00000000);

	RCC_HCLKConfig((uint32_t) 0x000000C0);

	__WFI();
	//#line 123
	RCC_HCLKConfig((uint32_t) 0x00000000);

	RCC_HSEConfig((uint32_t) 0x00010000);

	while (RCC_GetFlagStatus((uint8_t) 0x31) == RESET)
		;

	RCC_PLLConfig((uint32_t) 0x00010000, (uint32_t) 0x001C0000);

	RCC_PLLCmd(ENABLE);

	while (RCC_GetFlagStatus((uint8_t) 0x39) == RESET)
		;
	RCC_SYSCLKConfig((uint32_t) 0x00000002);

	RCC_APB2PeriphClockCmd(((uint32_t) 0x00000001 | (uint32_t) 0x00000010)
			| (uint32_t) 0x00000004, ENABLE);
	RCC_AHBPeriphClockCmd((uint32_t) 0x00000010 | (uint32_t) 0x00000004, ENABLE);

	return;
}

//# 59 "/opt/tinyos-2.x/tos/interfaces/McuSleep.nc"
inline static void SchedulerBasicP$McuSleep$sleep(void) {
	//#line 59
	McuSleepC$McuSleep$sleep();
	//#line 59
}
//#line 59
//# 67 "/opt/tinyos-2.x/tos/system/SchedulerBasicP.nc"
static __inline uint8_t SchedulerBasicP$popTask(void) {
	if (SchedulerBasicP$m_head != SchedulerBasicP$NO_TASK) {
		uint8_t id = SchedulerBasicP$m_head;

		//#line 72
		SchedulerBasicP$m_head = SchedulerBasicP$m_next[SchedulerBasicP$m_head];
		if (SchedulerBasicP$m_head == SchedulerBasicP$NO_TASK) {
			SchedulerBasicP$m_tail = SchedulerBasicP$NO_TASK;
		}
		SchedulerBasicP$m_next[id] = SchedulerBasicP$NO_TASK;
		return id;
	} else {
		return SchedulerBasicP$NO_TASK;
	}
}

//#line 138
static inline void SchedulerBasicP$Scheduler$taskLoop(void) {
	for (;;) {
		uint8_t nextTask;

		{
			__nesc_atomic_t  __nesc_atomic = __nesc_atomic_start();
			{
				while ((nextTask = SchedulerBasicP$popTask())
						== SchedulerBasicP$NO_TASK) {
					SchedulerBasicP$McuSleep$sleep();
				}
			}
			//#line 150
			__nesc_atomic_end(__nesc_atomic);
		}
		SchedulerBasicP$TaskBasic$runTask(nextTask);
	}
}

//# 61 "/opt/tinyos-2.x/tos/interfaces/Scheduler.nc"
inline static void RealMainP$Scheduler$taskLoop(void) {
	//#line 61
	SchedulerBasicP$Scheduler$taskLoop();
	//#line 61
}
//#line 61
//# 89 "/opt/tinyos-2.x/tos/chips/stm32/stm32hardware.h"
static __inline void __nesc_disable_interrupt(void)
//#line 89
{
	uint32_t statusReg = 0;

	return;
}

//# 160 "/opt/tinyos-2.x/tos/chips/stm32/McuSleepC.nc"
void RTCAlarm_IRQHandler(void) {

	STM_EVAL_LEDToggle(LED3);

	if (RTC_GetITStatus((uint16_t) 0x0002) != RESET) {

		EXTI_ClearITPendingBit((uint32_t) 0x20000);

		if (PWR_GetFlagStatus((uint32_t) 0x00000001) != RESET) {

			PWR_ClearFlag((uint32_t) 0x00000001);
		}
	}
	RTC_IRQHandler();
}

//# 220 "/opt/tinyos-2.x/tos/chips/stm32/timer/STM32RtcC.nc"
void RTC_IRQHandler(void) {


	if (RTC_GetITStatus((uint16_t) 0x0002) != RESET) {

		STM32RtcC$Alarm$stop();
		STM32RtcC$Alarm$fired();
	}
	if (RTC_GetITStatus((uint16_t) 0x0004) != RESET) {
		RTC_ClearITPendingBit((uint16_t) 0x0004);
		RTC_WaitForLastTask();
		STM32RtcC$Counter$overflow();
	}
}

//#line 55
static void STM32RtcC$disableInterrupt(void) {

	{
		__nesc_atomic_t  __nesc_atomic = __nesc_atomic_start();
		//#line 58
		{
			RTC_ClearITPendingBit((uint16_t) 0x0002);
			RTC_WaitForLastTask();
			RTC_ITConfig((uint16_t) 0x0002, DISABLE);
			RTC_WaitForLastTask();
			STM32RtcC$running = FALSE;
		}
		//#line 64
		__nesc_atomic_end(__nesc_atomic);
	}
}

//# 159 "/opt/tinyos-2.x/tos/system/SchedulerBasicP.nc"
static error_t SchedulerBasicP$TaskBasic$postTask(uint8_t id) {
	{
		__nesc_atomic_t  __nesc_atomic = __nesc_atomic_start();
		//#line 161
		{
			//#line 161
			{
				unsigned char __nesc_temp =
				//#line 161
						SchedulerBasicP$pushTask(id) ? SUCCESS : EBUSY;

				{
					//#line 161
					__nesc_atomic_end(__nesc_atomic);
					//#line 161
					return __nesc_temp;
				}
			}
		}
		//#line 164
		__nesc_atomic_end(__nesc_atomic);
	}
}

//# 50 "/opt/tinyos-2.x/tos/chips/stm32/HplSTM32InterruptM.nc"
void NMIException(void) {
	while (1)
		;
	//#line 52
	;
	HplSTM32InterruptM$Irq$fired();
}

void HardFaultException(void) {
	while (1)
		;
	//#line 59
	;
}

void MemManageException(void) {
	while (1)
		;
	//#line 65
	;
}

void BusFaultException(void) {
	while (1)
		;
	//#line 71
	;
}

void UsageFaultException(void) {
	while (1)
		;
	//#line 77
	;
}

void __STM32ReservedException7(void) {
	while (1)
		;
	//#line 82
	;
}

void __STM32ReservedException8(void) {
	while (1)
		;
	//#line 87
	;
}

void __STM32ReservedException9(void) {
	while (1)
		;
	//#line 92
	;
}

void __STM32ReservedException10(void) {
	while (1)
		;
	//#line 97
	;
}

void SVCHandler(void) {
	while (1)
		;
	//#line 103
	;
}

void DebugMonitor(void) {
	while (1)
		;
	//#line 109
	;
}

void __STM32ReservedException13(void) {
	while (1)
		;
	//#line 114
	;
}

void PendSVC(void) {
	while (1)
		;
	//#line 121
	;
}

void SysTickHandler(void) {
	while (1)
		;
	//#line 127
	;
}

void WWDG_IRQHandler(void) {
	while (1)
		;
	//#line 133
	;
}

void PVD_IRQHandler(void) {
	while (1)
		;
	//#line 139
	;
}

void TAMPER_IRQHandler(void) {
	while (1)
		;
	//#line 145
	;
}

void FLASH_IRQHandler(void) {
	while (1)
		;
	//#line 160
	;
}

void RCC_IRQHandler(void) {
	while (1)
		;
	//#line 166
	;
}

void EXTI0_IRQHandler(void) {
	while (1)
		;
	//#line 172
	;
}

void EXTI1_IRQHandler(void) {
	while (1)
		;
	//#line 178
	;
}

void EXTI2_IRQHandler(void) {
	while (1)
		;
	//#line 184
	;
}

void EXTI3_IRQHandler(void) {
	while (1)
		;
	//#line 190
	;
}

void EXTI4_IRQHandler(void) {
	while (1)
		;
	//#line 196
	;
}

void DMAChannel1_IRQHandler(void) {
	while (1)
		;
	//#line 202
	;
}

void DMAChannel2_IRQHandler(void) {
	while (1)
		;
	//#line 208
	;
}

void DMAChannel3_IRQHandler(void) {
	while (1)
		;
	//#line 214
	;
}

void DMAChannel4_IRQHandler(void) {
	while (1)
		;
	//#line 220
	;
}

void DMAChannel5_IRQHandler(void) {
	while (1)
		;
	//#line 226
	;
}

void DMAChannel6_IRQHandler(void) {
	while (1)
		;
	//#line 232
	;
}

void DMAChannel7_IRQHandler(void) {
	while (1)
		;
	//#line 238
	;
}

void ADC_IRQHandler(void) {
	while (1)
		;
	//#line 244
	;
}

void USB_HP_CAN_TX_IRQHandler(void) {
	while (1)
		;
	//#line 250
	;
}

void USB_LP_CAN_RX0_IRQHandler(void) {
	while (1)
		;
	//#line 256
	;
}

void CAN_RX1_IRQHandler(void) {
	while (1)
		;
	//#line 262
	;
}

void CAN_SCE_IRQHandler(void) {
	while (1)
		;
	//#line 268
	;
}

void EXTI9_5_IRQHandler(void) {
	while (1)
		;
	//#line 274
	;
}

void TIM1_BRK_IRQHandler(void) {
	while (1)
		;
	//#line 280
	;
}

void TIM1_UP_IRQHandler(void) {
	while (1)
		;
	//#line 286
	;
}

void TIM1_TRG_COM_IRQHandler(void) {
	while (1)
		;
	//#line 292
	;
}

void TIM1_CC_IRQHandler(void) {
	while (1)
		;
	//#line 298
	;
}

void TIM2_IRQHandler(void) {
	while (1)
		;
	//#line 304
	;
}

void TIM3_IRQHandler(void) {
	while (1)
		;
	//#line 310
	;
}

void TIM4_IRQHandler(void) {
	while (1)
		;
	//#line 316
	;
}

void I2C1_EV_IRQHandler(void) {
	while (1)
		;
	//#line 322
	;
}

void I2C1_ER_IRQHandler(void) {
	while (1)
		;
	//#line 328
	;
}

void I2C2_EV_IRQHandler(void) {
	while (1)
		;
	//#line 334
	;
}

void I2C2_ER_IRQHandler(void) {
	while (1)
		;
	//#line 340
	;
}

void SPI1_IRQHandler(void) {
	while (1)
		;
	//#line 346
	;
}

void SPI2_IRQHandler(void) {
	while (1)
		;
	//#line 352
	;
}

void USART1_IRQHandler(void) {
	while (1)
		;
	//#line 358
	;
}

void USART2_IRQHandler(void) {
	while (1)
		;
	//#line 364
	;
}

void USART3_IRQHandler(void) {
	while (1)
		;
	//#line 370
	;
}

void EXTI15_10_IRQHandler(void) {
	while (1)
		;
	//#line 376
	;
}

void USBWakeUp_IRQHandler(void) {
	while (1)
		;
	//#line 391
	;
}

//# 52 "/opt/tinyos-2.x/tos/system/RealMainP.nc"
int main_tinyos(void)
//#line 52
{
	{
		__nesc_atomic_t  __nesc_atomic = __nesc_atomic_start();
		{

			{
			}
			//#line 60
			;

			RealMainP$Scheduler$init();

			RealMainP$PlatformInit$init();
			while (RealMainP$Scheduler$runNextTask())
				;

			RealMainP$SoftwareInit$init();
			while (RealMainP$Scheduler$runNextTask())
				;
		}
		//#line 77
		__nesc_atomic_end(__nesc_atomic);
	}

	__nesc_enable_interrupt();

	RealMainP$Boot$booted();

	RealMainP$Scheduler$taskLoop();

	return -1;
}

//# 123 "/opt/tinyos-2.x/tos/system/SchedulerBasicP.nc"
static bool SchedulerBasicP$Scheduler$runNextTask(void) {
	uint8_t nextTask;

	/* atomic removed: atomic calls only */
	//#line 127
	{
		nextTask = SchedulerBasicP$popTask();
		if (nextTask == SchedulerBasicP$NO_TASK) {
			{
				unsigned char __nesc_temp =
				//#line 131
						FALSE;

				//#line 131
				return __nesc_temp;
			}
		}
	}
	//#line 134
	SchedulerBasicP$TaskBasic$runTask(nextTask);
	return TRUE;
}

//# 63 "/opt/tinyos-2.x/tos/lib/timer/AlarmToTimerC.nc"
static void /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$fired$runTask(
		void) {
	if (/*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$m_oneshot
			== FALSE) {
		/*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$start(
				/*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Alarm$getAlarm(), /*HilTimerMilliC.AlarmToTimerMilli32*/
				AlarmToTimerC$0$m_dt, FALSE);
	}
	//#line 67
	/*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Timer$fired();
}

//# 147 "/opt/tinyos-2.x/tos/chips/stm32/timer/STM32RtcC.nc"
static void STM32RtcC$Alarm$startAt(uint32_t t0, uint32_t dt) {
	{
		uint32_t now = STM32RtcC$Alarm$getNow();
		uint32_t elapsed = now - t0;

		//#line 152
		now = RTC_GetCounter();
		if (elapsed >= dt) {

			RTC_SetAlarm(now + 1);
			{
				__nesc_atomic_t  __nesc_atomic = __nesc_atomic_start();
				//#line 157
				STM32RtcC$alarm = now + 1;
				//#line 157
				__nesc_atomic_end(__nesc_atomic);
			}
			RTC_WaitForLastTask();
		} else {
			uint32_t remaining = dt - elapsed;

			//#line 163
			if (remaining <= 1) {
				RTC_SetAlarm(now + 1);
				{
					__nesc_atomic_t  __nesc_atomic = __nesc_atomic_start();
					//#line 166
					STM32RtcC$alarm = now + 1;
					//#line 166
					__nesc_atomic_end(__nesc_atomic);
				}
				RTC_WaitForLastTask();
			} else {
				RTC_SetAlarm(now + remaining);
				{
					__nesc_atomic_t  __nesc_atomic = __nesc_atomic_start();
					//#line 172
					STM32RtcC$alarm = now + remaining;
					//#line 172
					__nesc_atomic_end(__nesc_atomic);
				}
				RTC_WaitForLastTask();
			}
		}
		STM32RtcC$enableInterrupt();
	}
}

//# 62 "/opt/tinyos-2.x/tos/lib/timer/VirtualizeTimerC.nc"
static void /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$fireTimers(
		uint32_t now) {
	uint8_t num;

	for (num = 0; num
			< /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$NUM_TIMERS; num++) {
		/*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$Timer_t
				*timer =
						&/*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$m_timers[num];

		if (timer->isrunning) {
			uint32_t elapsed = now - timer->t0;

			if (elapsed >= timer->dt) {
				if (timer->isoneshot) {
					timer->isrunning = FALSE;
				} else {
					//#line 79
					timer->t0 += timer->dt;
				}
				/*HilTimerMilliC.VirtTimersMilli32*/
				VirtualizeTimerC$0$Timer$fired(num);
				break;
			}
		}
	}
	/*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$updateFromTimer$postTask();
}

static void /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$updateFromTimer$runTask(
		void) {

	uint32_t now = /*HilTimerMilliC.VirtTimersMilli32*/
			VirtualizeTimerC$0$TimerFrom$getNow();
	int32_t min_remaining = (1UL << 31) - 1;
	bool min_remaining_isset = FALSE;
	uint8_t num;

	/*HilTimerMilliC.VirtTimersMilli32*/
	VirtualizeTimerC$0$TimerFrom$stop();

	for (num = 0; num
			< /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$NUM_TIMERS; num++) {
		/*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$Timer_t
				*timer =
						&/*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$m_timers[num];

		if (timer->isrunning) {
			uint32_t elapsed = now - timer->t0;
			int32_t remaining = timer->dt - elapsed;

			if (remaining < min_remaining) {
				min_remaining = remaining;
				min_remaining_isset = TRUE;
			}
		}
	}

	if (min_remaining_isset) {
		if (min_remaining <= 0) {
			/*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$fireTimers(
					now);
		} else {
			//#line 124
			/*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$TimerFrom$startOneShotAt(
					now, min_remaining);
		}
	}
}

//#line 143
static void /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$Timer$startPeriodic(
		uint8_t num, uint32_t dt) {
	/*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$startTimer(num, /*HilTimerMilliC.VirtTimersMilli32*/
			VirtualizeTimerC$0$TimerFrom$getNow(), dt, FALSE);
}

