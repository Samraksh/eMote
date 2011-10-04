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
//# 323 "/home/nived/CodeSourcery/Sourcery_G++_Lite/bin/../lib/gcc/arm-none-eabi/4.5.1/include/stddef.h" 3
//typedef unsigned int wchar_t;
//# 274 "/home/nived/CodeSourcery/Sourcery_G++_Lite/bin/../lib/gcc/arm-none-eabi/4.5.1/../../../../arm-none-eabi/include/inttypes.h"
//#line 271
typedef struct __nesc_unnamed4242 {
  intmax_t quot;
  intmax_t rem;
} imaxdiv_t;
//# 235 "/usr/lib/ncc/nesc_nx.h"
static __inline uint8_t __nesc_ntoh_uint8(const void *source);




static __inline uint8_t __nesc_hton_uint8(void *target, uint8_t value);





static __inline uint8_t __nesc_ntoh_leuint8(const void *source);




static __inline uint8_t __nesc_hton_leuint8(void *target, uint8_t value);
//#line 276
static __inline uint16_t __nesc_ntoh_leuint16(const void *source);




static __inline uint16_t __nesc_hton_leuint16(void *target, uint16_t value);
//#line 294
static __inline uint32_t __nesc_ntoh_uint32(const void *source);






static __inline uint32_t __nesc_hton_uint32(void *target, uint32_t value);
//#line 326
static __inline int32_t __nesc_ntoh_int32(const void *source);
//#line 326
static __inline int32_t __nesc_hton_int32(void *target, int32_t value);
//#line 385
typedef struct { char data[1]; } __attribute__((packed)) nx_int8_t;typedef int8_t __nesc_nxbase_nx_int8_t  ;
typedef struct { char data[2]; } __attribute__((packed)) nx_int16_t;typedef int16_t __nesc_nxbase_nx_int16_t  ;
typedef struct { char data[4]; } __attribute__((packed)) nx_int32_t;typedef int32_t __nesc_nxbase_nx_int32_t  ;
typedef struct { char data[8]; } __attribute__((packed)) nx_int64_t;typedef int64_t __nesc_nxbase_nx_int64_t  ;
typedef struct { char data[1]; } __attribute__((packed)) nx_uint8_t;typedef uint8_t __nesc_nxbase_nx_uint8_t  ;
typedef struct { char data[2]; } __attribute__((packed)) nx_uint16_t;typedef uint16_t __nesc_nxbase_nx_uint16_t  ;
typedef struct { char data[4]; } __attribute__((packed)) nx_uint32_t;typedef uint32_t __nesc_nxbase_nx_uint32_t  ;
typedef struct { char data[8]; } __attribute__((packed)) nx_uint64_t;typedef uint64_t __nesc_nxbase_nx_uint64_t  ;


typedef struct { char data[1]; } __attribute__((packed)) nxle_int8_t;typedef int8_t __nesc_nxbase_nxle_int8_t  ;
typedef struct { char data[2]; } __attribute__((packed)) nxle_int16_t;typedef int16_t __nesc_nxbase_nxle_int16_t  ;
typedef struct { char data[4]; } __attribute__((packed)) nxle_int32_t;typedef int32_t __nesc_nxbase_nxle_int32_t  ;
typedef struct { char data[8]; } __attribute__((packed)) nxle_int64_t;typedef int64_t __nesc_nxbase_nxle_int64_t  ;
typedef struct { char data[1]; } __attribute__((packed)) nxle_uint8_t;typedef uint8_t __nesc_nxbase_nxle_uint8_t  ;
typedef struct { char data[2]; } __attribute__((packed)) nxle_uint16_t;typedef uint16_t __nesc_nxbase_nxle_uint16_t  ;
typedef struct { char data[4]; } __attribute__((packed)) nxle_uint32_t;typedef uint32_t __nesc_nxbase_nxle_uint32_t  ;
typedef struct { char data[8]; } __attribute__((packed)) nxle_uint64_t;typedef uint64_t __nesc_nxbase_nxle_uint64_t  ;
//# 26 "/home/nived/CodeSourcery/Sourcery_G++_Lite/bin/../lib/gcc/arm-none-eabi/4.5.1/../../../../arm-none-eabi/include/machine/_default_types.h"
typedef signed char __int8_t;
typedef unsigned char __uint8_t;








typedef signed short __int16_t;
typedef unsigned short __uint16_t;








typedef __int16_t __int_least16_t;
typedef __uint16_t __uint_least16_t;
//# 58 "/home/nived/CodeSourcery/Sourcery_G++_Lite/bin/../lib/gcc/arm-none-eabi/4.5.1/../../../../arm-none-eabi/include/machine/_default_types.h" 3
typedef signed int __int32_t;
typedef unsigned int __uint32_t;
//#line 76
typedef __int32_t __int_least32_t;
typedef __uint32_t __uint_least32_t;
//#line 99
typedef signed long long __int64_t;
typedef unsigned long long __uint64_t;
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
typedef struct __nesc_unnamed4243 {

  int __count;
  union __nesc_unnamed4244 {

    wint_t __wch;
    unsigned char __wchb[4];
  } __value;
} _mbstate_t;



typedef _LOCK_RECURSIVE_T _flock_t;




typedef void *_iconv_t;
//# 21 "/home/nived/CodeSourcery/Sourcery_G++_Lite/bin/../lib/gcc/arm-none-eabi/4.5.1/../../../../arm-none-eabi/include/sys/reent.h"
typedef unsigned long __ULong;
//# 37 "/home/nived/CodeSourcery/Sourcery_G++_Lite/bin/../lib/gcc/arm-none-eabi/4.5.1/../../../../arm-none-eabi/include/sys/reent.h" 3
struct _reent;






struct _Bigint {

  struct _Bigint *_next;
  int _k, _maxwds, _sign, _wds;
  __ULong _x[1];
};


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
//# 149 "/home/nived/CodeSourcery/Sourcery_G++_Lite/bin/../lib/gcc/arm-none-eabi/4.5.1/include/stddef.h" 3
typedef int ptrdiff_t;
//# 40 "/opt/tinyos-2.x/tos/system/../lib/safe/include/annots_stage1.h"
struct __nesc_attr_safe {
};
//#line 41
struct __nesc_attr_unsafe {
};
//# 23 "/opt/tinyos-2.x/tos/system/tos.h"
//typedef uint8_t bool;
enum __nesc_unnamed4248 {
//#line 24
  FALSE = 0, TRUE = 1
};
typedef nx_int8_t nx_bool;
uint16_t TOS_NODE_ID = 1;






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
  SUCCESS = 0, 
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

typedef uint8_t error_t  ;

static inline error_t ecombine(error_t r1, error_t r2)  ;
//# 408 "/opt/tinyos-2.x/tos/chips/stm32/fwlib/inc/stm32f10x_map.h"
//#line 143
typedef enum IRQn {


  NonMaskableInt_IRQn = -14, 
  MemoryManagement_IRQn = -12, 
  BusFault_IRQn = -11, 
  UsageFault_IRQn = -10, 
  SVCall_IRQn = -5, 
  DebugMonitor_IRQn = -4, 
  PendSV_IRQn = -2, 
  SysTick_IRQn = -1, 


  WWDG_IRQn = 0, 
  PVD_IRQn = 1, 
  TAMPER_IRQn = 2, 
  RTC_IRQn = 3, 
  FLASH_IRQn = 4, 
  RCC_IRQn = 5, 
  EXTI0_IRQn = 6, 
  EXTI1_IRQn = 7, 
  EXTI2_IRQn = 8, 
  EXTI3_IRQn = 9, 
  EXTI4_IRQn = 10, 
  DMA1_Channel1_IRQn = 11, 
  DMA1_Channel2_IRQn = 12, 
  DMA1_Channel3_IRQn = 13, 
  DMA1_Channel4_IRQn = 14, 
  DMA1_Channel5_IRQn = 15, 
  DMA1_Channel6_IRQn = 16, 
  DMA1_Channel7_IRQn = 17, 
//#line 274
  ADC1_2_IRQn = 18, 
  USB_HP_CAN1_TX_IRQn = 19, 
  USB_LP_CAN1_RX0_IRQn = 20, 
  CAN1_RX1_IRQn = 21, 
  CAN1_SCE_IRQn = 22, 
  EXTI9_5_IRQn = 23, 
  TIM1_BRK_IRQn = 24, 
  TIM1_UP_IRQn = 25, 
  TIM1_TRG_COM_IRQn = 26, 
  TIM1_CC_IRQn = 27, 
  TIM2_IRQn = 28, 
  TIM3_IRQn = 29, 
  TIM4_IRQn = 30, 
  I2C1_EV_IRQn = 31, 
  I2C1_ER_IRQn = 32, 
  I2C2_EV_IRQn = 33, 
  I2C2_ER_IRQn = 34, 
  SPI1_IRQn = 35, 
  SPI2_IRQn = 36, 
  USART1_IRQn = 37, 
  USART2_IRQn = 38, 
  USART3_IRQn = 39, 
  EXTI15_10_IRQn = 40, 
  RTCAlarm_IRQn = 41, 
  USBWakeUp_IRQn = 42, 
  TIM8_BRK_IRQn = 43, 
  TIM8_UP_IRQn = 44, 
  TIM8_TRG_COM_IRQn = 45, 
  TIM8_CC_IRQn = 46, 
  ADC3_IRQn = 47, 
  FSMC_IRQn = 48, 
  SDIO_IRQn = 49, 
  TIM5_IRQn = 50, 
  SPI3_IRQn = 51, 
  UART4_IRQn = 52, 
  UART5_IRQn = 53, 
  TIM6_IRQn = 54, 
  TIM7_IRQn = 55, 
  DMA2_Channel1_IRQn = 56, 
  DMA2_Channel2_IRQn = 57, 
  DMA2_Channel3_IRQn = 58, 
  DMA2_Channel4_5_IRQn = 59
} 
//#line 408
IRQn_Type;
//# 147 "/opt/tinyos-2.x/tos/chips/stm32/fwlib/inc/cortexm3_macro.h"
//#line 132
typedef struct __nesc_unnamed4250 {

  volatile uint32_t ISER[8];
  uint32_t RESERVED0[24];
  volatile uint32_t ICER[8];
  uint32_t RSERVED1[24];
  volatile uint32_t ISPR[8];
  uint32_t RESERVED2[24];
  volatile uint32_t ICPR[8];
  uint32_t RESERVED3[24];
  volatile uint32_t IABR[8];
  uint32_t RESERVED4[56];
  volatile uint8_t IP[240];
  uint32_t RESERVED5[644];
  volatile uint32_t STIR;
} NVIC_Type;
//#line 176
//#line 155
typedef struct __nesc_unnamed4251 {

  volatile const uint32_t CPUID;
  volatile uint32_t ICSR;
  volatile uint32_t VTOR;
  volatile uint32_t AIRCR;
  volatile uint32_t SCR;
  volatile uint32_t CCR;
  volatile uint8_t SHP[12];
  volatile uint32_t SHCSR;
  volatile uint32_t CFSR;
  volatile uint32_t HFSR;
  volatile uint32_t DFSR;
  volatile uint32_t MMFAR;
  volatile uint32_t BFAR;
  volatile uint32_t AFSR;
  volatile const uint32_t PFR[2];
  volatile const uint32_t DFR;
  volatile const uint32_t ADR;
  volatile const uint32_t MMFR[4];
  volatile const uint32_t ISAR[5];
} SCB_Type;
//#line 371
//#line 365
typedef struct __nesc_unnamed4252 {

  volatile uint32_t CTRL;
  volatile uint32_t LOAD;
  volatile uint32_t VAL;
  volatile const uint32_t CALIB;
} SysTick_Type;
//#line 444
//#line 410
typedef struct __nesc_unnamed4253 {

  volatile union __nesc_unnamed4254 {

    volatile uint8_t u8;
    volatile uint16_t u16;
    volatile uint32_t u32;
  } PORT[32];
  uint32_t RESERVED0[864];
  volatile uint32_t TER;
  uint32_t RESERVED1[15];
  volatile uint32_t TPR;
  uint32_t RESERVED2[15];
  volatile uint32_t TCR;
  uint32_t RESERVED3[29];
  volatile uint32_t IWR;
  volatile uint32_t IRR;
  volatile uint32_t IMCR;
  uint32_t RESERVED4[43];
  volatile uint32_t LAR;
  volatile uint32_t LSR;
  uint32_t RESERVED5[6];
  volatile const uint32_t PID4;
  volatile const uint32_t PID5;
  volatile const uint32_t PID6;
  volatile const uint32_t PID7;
  volatile const uint32_t PID0;
  volatile const uint32_t PID1;
  volatile const uint32_t PID2;
  volatile const uint32_t PID3;
  volatile const uint32_t CID0;
  volatile const uint32_t CID1;
  volatile const uint32_t CID2;
  volatile const uint32_t CID3;
} ITM_Type;
//#line 512
//#line 503
typedef struct __nesc_unnamed4255 {

  uint32_t RESERVED0;
  volatile const uint32_t ICTR;



  uint32_t RESERVED1;
} 
InterruptType_Type;
//#line 626
//#line 620
typedef struct __nesc_unnamed4256 {

  volatile uint32_t DHCSR;
  volatile uint32_t DCRSR;
  volatile uint32_t DCRDR;
  volatile uint32_t DEMCR;
} CoreDebug_Type;
//#line 1211
static __inline void __WFI(void);
//#line 1502
static __inline void NVIC_EnableIRQ(IRQn_Type IRQn);
//#line 1515
static __inline void NVIC_DisableIRQ(IRQn_Type IRQn);
//# 424 "/opt/tinyos-2.x/tos/chips/stm32/fwlib/inc/stm32f10x_map.h"
typedef int32_t s32;
typedef int16_t s16;
typedef int8_t s8;

typedef const int32_t sc32;
typedef const int16_t sc16;
typedef const int8_t sc8;

typedef volatile int32_t vs32;
typedef volatile int16_t vs16;
typedef volatile int8_t vs8;

typedef volatile const int32_t vsc32;
typedef volatile const int16_t vsc16;
typedef volatile const int8_t vsc8;

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef const uint32_t uc32;
typedef const uint16_t uc16;
typedef const uint8_t uc8;

typedef volatile uint32_t vu32;
typedef volatile uint16_t vu16;
typedef volatile uint8_t vu8;

typedef volatile const uint32_t vuc32;
typedef volatile const uint16_t vuc16;
typedef volatile const uint8_t vuc8;





typedef enum __nesc_unnamed4257 {
//#line 460
  RESET = 0, SET = !RESET
} 
//#line 460
FlagStatus;
//#line 460
typedef enum __nesc_unnamed4257 ITStatus;

typedef enum __nesc_unnamed4258 {
//#line 462
  DISABLE = 0, ENABLE = !DISABLE
} 
//#line 462
FunctionalState;


typedef enum __nesc_unnamed4259 {
//#line 465
  ERROR = 0, SUCCESS_fwlib = !ERROR
} 
//#line 465
ErrorStatus;
//#line 505
//#line 483
typedef struct __nesc_unnamed4260 {

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
//#line 604
//#line 511
typedef struct __nesc_unnamed4261 {

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
  volatile uint16_t RTCCR;
  uint16_t RESERVED11;
  volatile uint16_t CR;
  uint16_t RESERVED12;
  volatile uint16_t CSR;
  uint16_t RESERVED13[5];
  volatile uint16_t DR11;
  uint16_t RESERVED14;
  volatile uint16_t DR12;
  uint16_t RESERVED15;
  volatile uint16_t DR13;
  uint16_t RESERVED16;
  volatile uint16_t DR14;
  uint16_t RESERVED17;
  volatile uint16_t DR15;
  uint16_t RESERVED18;
  volatile uint16_t DR16;
  uint16_t RESERVED19;
  volatile uint16_t DR17;
  uint16_t RESERVED20;
  volatile uint16_t DR18;
  uint16_t RESERVED21;
  volatile uint16_t DR19;
  uint16_t RESERVED22;
  volatile uint16_t DR20;
  uint16_t RESERVED23;
  volatile uint16_t DR21;
  uint16_t RESERVED24;
  volatile uint16_t DR22;
  uint16_t RESERVED25;
  volatile uint16_t DR23;
  uint16_t RESERVED26;
  volatile uint16_t DR24;
  uint16_t RESERVED27;
  volatile uint16_t DR25;
  uint16_t RESERVED28;
  volatile uint16_t DR26;
  uint16_t RESERVED29;
  volatile uint16_t DR27;
  uint16_t RESERVED30;
  volatile uint16_t DR28;
  uint16_t RESERVED31;
  volatile uint16_t DR29;
  uint16_t RESERVED32;
  volatile uint16_t DR30;
  uint16_t RESERVED33;
  volatile uint16_t DR31;
  uint16_t RESERVED34;
  volatile uint16_t DR32;
  uint16_t RESERVED35;
  volatile uint16_t DR33;
  uint16_t RESERVED36;
  volatile uint16_t DR34;
  uint16_t RESERVED37;
  volatile uint16_t DR35;
  uint16_t RESERVED38;
  volatile uint16_t DR36;
  uint16_t RESERVED39;
  volatile uint16_t DR37;
  uint16_t RESERVED40;
  volatile uint16_t DR38;
  uint16_t RESERVED41;
  volatile uint16_t DR39;
  uint16_t RESERVED42;
  volatile uint16_t DR40;
  uint16_t RESERVED43;
  volatile uint16_t DR41;
  uint16_t RESERVED44;
  volatile uint16_t DR42;
  uint16_t RESERVED45;
} BKP_TypeDef;
//#line 616
//#line 610
typedef struct __nesc_unnamed4262 {

  volatile uint32_t TIR;
  volatile uint32_t TDTR;
  volatile uint32_t TDLR;
  volatile uint32_t TDHR;
} CAN_TxMailBox_TypeDef;
//#line 628
//#line 622
typedef struct __nesc_unnamed4263 {

  volatile uint32_t RIR;
  volatile uint32_t RDTR;
  volatile uint32_t RDLR;
  volatile uint32_t RDHR;
} CAN_FIFOMailBox_TypeDef;









//#line 634
typedef struct __nesc_unnamed4264 {

  volatile uint32_t FR1;
  volatile uint32_t FR2;
} CAN_FilterRegister_TypeDef;
//#line 672
//#line 644
typedef struct __nesc_unnamed4265 {

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
  volatile uint32_t FM1R;
  uint32_t RESERVED2;
  volatile uint32_t FS1R;
  uint32_t RESERVED3;
  volatile uint32_t FFA1R;
  uint32_t RESERVED4;
  volatile uint32_t FA1R;
  uint32_t RESERVED5[8];

  CAN_FilterRegister_TypeDef sFilterRegister[14];
} 


CAN_TypeDef;
//#line 686
//#line 677
typedef struct __nesc_unnamed4266 {

  volatile uint32_t CFGR;
  volatile uint32_t OAR;
  volatile uint32_t PRES;
  volatile uint32_t ESR;
  volatile uint32_t CSR;
  volatile uint32_t TXD;
  volatile uint32_t RXD;
} CEC_TypeDef;
//#line 699
//#line 692
typedef struct __nesc_unnamed4267 {

  volatile uint32_t DR;
  volatile uint8_t IDR;
  uint8_t RESERVED0;
  uint16_t RESERVED1;
  volatile uint32_t CR;
} CRC_TypeDef;
//#line 723
//#line 705
typedef struct __nesc_unnamed4268 {

  volatile uint32_t CR;
  volatile uint32_t SWTRIGR;
  volatile uint32_t DHR12R1;
  volatile uint32_t DHR12L1;
  volatile uint32_t DHR8R1;
  volatile uint32_t DHR12R2;
  volatile uint32_t DHR12L2;
  volatile uint32_t DHR8R2;
  volatile uint32_t DHR12RD;
  volatile uint32_t DHR12LD;
  volatile uint32_t DHR8RD;
  volatile uint32_t DOR1;
  volatile uint32_t DOR2;
} 


DAC_TypeDef;









//#line 729
typedef struct __nesc_unnamed4269 {

  volatile uint32_t IDCODE;
  volatile uint32_t CR;
} DBGMCU_TypeDef;
//#line 745
//#line 739
typedef struct __nesc_unnamed4270 {

  volatile uint32_t CCR;
  volatile uint32_t CNDTR;
  volatile uint32_t CPAR;
  volatile uint32_t CMAR;
} DMA_Channel_TypeDef;





//#line 747
typedef struct __nesc_unnamed4271 {

  volatile uint32_t ISR;
  volatile uint32_t IFCR;
} DMA_TypeDef;
//#line 822
//#line 757
typedef struct __nesc_unnamed4272 {

  volatile uint32_t MACCR;
  volatile uint32_t MACFFR;
  volatile uint32_t MACHTHR;
  volatile uint32_t MACHTLR;
  volatile uint32_t MACMIIAR;
  volatile uint32_t MACMIIDR;
  volatile uint32_t MACFCR;
  volatile uint32_t MACVLANTR;
  uint32_t RESERVED0[2];
  volatile uint32_t MACRWUFFR;
  volatile uint32_t MACPMTCSR;
  uint32_t RESERVED1[2];
  volatile uint32_t MACSR;
  volatile uint32_t MACIMR;
  volatile uint32_t MACA0HR;
  volatile uint32_t MACA0LR;
  volatile uint32_t MACA1HR;
  volatile uint32_t MACA1LR;
  volatile uint32_t MACA2HR;
  volatile uint32_t MACA2LR;
  volatile uint32_t MACA3HR;
  volatile uint32_t MACA3LR;
  uint32_t RESERVED2[40];
  volatile uint32_t MMCCR;
  volatile uint32_t MMCRIR;
  volatile uint32_t MMCTIR;
  volatile uint32_t MMCRIMR;
  volatile uint32_t MMCTIMR;
  uint32_t RESERVED3[14];
  volatile uint32_t MMCTGFSCCR;
  volatile uint32_t MMCTGFMSCCR;
  uint32_t RESERVED4[5];
  volatile uint32_t MMCTGFCR;
  uint32_t RESERVED5[10];
  volatile uint32_t MMCRFCECR;
  volatile uint32_t MMCRFAECR;
  uint32_t RESERVED6[10];
  volatile uint32_t MMCRGUFCR;
  uint32_t RESERVED7[334];
  volatile uint32_t PTPTSCR;
  volatile uint32_t PTPSSIR;
  volatile uint32_t PTPTSHR;
  volatile uint32_t PTPTSLR;
  volatile uint32_t PTPTSHUR;
  volatile uint32_t PTPTSLUR;
  volatile uint32_t PTPTSAR;
  volatile uint32_t PTPTTHR;
  volatile uint32_t PTPTTLR;
  uint32_t RESERVED8[567];
  volatile uint32_t DMABMR;
  volatile uint32_t DMATPDR;
  volatile uint32_t DMARPDR;
  volatile uint32_t DMARDLAR;
  volatile uint32_t DMATDLAR;
  volatile uint32_t DMASR;
  volatile uint32_t DMAOMR;
  volatile uint32_t DMAIER;
  volatile uint32_t DMAMFBOCR;
  uint32_t RESERVED9[9];
  volatile uint32_t DMACHTDR;
  volatile uint32_t DMACHRDR;
  volatile uint32_t DMACHTBAR;
  volatile uint32_t DMACHRBAR;
} ETH_TypeDef;
//#line 836
//#line 828
typedef struct __nesc_unnamed4273 {

  volatile uint32_t IMR;
  volatile uint32_t EMR;
  volatile uint32_t RTSR;
  volatile uint32_t FTSR;
  volatile uint32_t SWIER;
  volatile uint32_t PR;
} EXTI_TypeDef;
//#line 861
//#line 842
typedef struct __nesc_unnamed4274 {

  volatile uint32_t ACR;
  volatile uint32_t KEYR;
  volatile uint32_t OPTKEYR;
  volatile uint32_t SR;
  volatile uint32_t CR;
  volatile uint32_t AR;
  volatile uint32_t RESERVED;
  volatile uint32_t OBR;
  volatile uint32_t WRPR;
} 







FLASH_TypeDef;
//#line 877
//#line 867
typedef struct __nesc_unnamed4275 {

  volatile uint16_t RDP;
  volatile uint16_t USER;
  volatile uint16_t Data0;
  volatile uint16_t Data1;
  volatile uint16_t WRP0;
  volatile uint16_t WRP1;
  volatile uint16_t WRP2;
  volatile uint16_t WRP3;
} OB_TypeDef;








//#line 883
typedef struct __nesc_unnamed4276 {

  volatile uint32_t BTCR[8];
} FSMC_Bank1_TypeDef;








//#line 892
typedef struct __nesc_unnamed4277 {

  volatile uint32_t BWTR[7];
} FSMC_Bank1E_TypeDef;
//#line 909
//#line 901
typedef struct __nesc_unnamed4278 {

  volatile uint32_t PCR2;
  volatile uint32_t SR2;
  volatile uint32_t PMEM2;
  volatile uint32_t PATT2;
  uint32_t RESERVED0;
  volatile uint32_t ECCR2;
} FSMC_Bank2_TypeDef;
//#line 923
//#line 915
typedef struct __nesc_unnamed4279 {

  volatile uint32_t PCR3;
  volatile uint32_t SR3;
  volatile uint32_t PMEM3;
  volatile uint32_t PATT3;
  uint32_t RESERVED0;
  volatile uint32_t ECCR3;
} FSMC_Bank3_TypeDef;
//#line 936
//#line 929
typedef struct __nesc_unnamed4280 {

  volatile uint32_t PCR4;
  volatile uint32_t SR4;
  volatile uint32_t PMEM4;
  volatile uint32_t PATT4;
  volatile uint32_t PIO4;
} FSMC_Bank4_TypeDef;
//#line 951
//#line 942
typedef struct __nesc_unnamed4281 {

  volatile uint32_t CRL;
  volatile uint32_t CRH;
  volatile uint32_t IDR;
  volatile uint32_t ODR;
  volatile uint32_t BSRR;
  volatile uint32_t BRR;
  volatile uint32_t LCKR;
} GPIO_TypeDef;
//#line 964
//#line 957
typedef struct __nesc_unnamed4282 {

  volatile uint32_t EVCR;
  volatile uint32_t MAPR;
  volatile uint32_t EXTICR[4];
  uint32_t RESERVED0;
  volatile uint32_t MAPR2;
} AFIO_TypeDef;
//#line 989
//#line 969
typedef struct __nesc_unnamed4283 {

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
//#line 1001
//#line 995
typedef struct __nesc_unnamed4284 {

  volatile uint32_t KR;
  volatile uint32_t PR;
  volatile uint32_t RLR;
  volatile uint32_t SR;
} IWDG_TypeDef;









//#line 1007
typedef struct __nesc_unnamed4285 {

  volatile uint32_t CR;
  volatile uint32_t CSR;
} PWR_TypeDef;
//#line 1039
//#line 1017
typedef struct __nesc_unnamed4286 {

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
} 









RCC_TypeDef;
//#line 1067
//#line 1045
typedef struct __nesc_unnamed4287 {

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
//#line 1095
//#line 1073
typedef struct __nesc_unnamed4288 {

  volatile uint32_t POWER;
  volatile uint32_t CLKCR;
  volatile uint32_t ARG;
  volatile uint32_t CMD;
  volatile const uint32_t RESPCMD;
  volatile const uint32_t RESP1;
  volatile const uint32_t RESP2;
  volatile const uint32_t RESP3;
  volatile const uint32_t RESP4;
  volatile uint32_t DTIMER;
  volatile uint32_t DLEN;
  volatile uint32_t DCTRL;
  volatile const uint32_t DCOUNT;
  volatile const uint32_t STA;
  volatile uint32_t ICR;
  volatile uint32_t MASK;
  uint32_t RESERVED0[2];
  volatile const uint32_t FIFOCNT;
  uint32_t RESERVED1[13];
  volatile uint32_t FIFO;
} SDIO_TypeDef;
//#line 1121
//#line 1101
typedef struct __nesc_unnamed4289 {

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
  volatile uint16_t I2SCFGR;
  uint16_t RESERVED7;
  volatile uint16_t I2SPR;
  uint16_t RESERVED8;
} SPI_TypeDef;
//#line 1169
//#line 1127
typedef struct __nesc_unnamed4290 {

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
} TIM_TypeDef;
//#line 1191
//#line 1175
typedef struct __nesc_unnamed4291 {

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










//#line 1197
typedef struct __nesc_unnamed4292 {

  volatile uint32_t CR;
  volatile uint32_t CFR;
  volatile uint32_t SR;
} WWDG_TypeDef;
//# 42 "/opt/tinyos-2.x/tos/chips/stm32/stm32exceptions.h"
void NMIException(void );


void HardFaultException(void );


void MemManageException(void );


void BusFaultException(void );


void UsageFaultException(void );

void __STM32ReservedException7(void );
void __STM32ReservedException8(void );
void __STM32ReservedException9(void );
void __STM32ReservedException10(void );


void SVCHandler(void );


void DebugMonitor(void );

void __STM32ReservedException13(void );


void PendSVC(void );


void SysTickHandler(void );


void WWDG_IRQHandler(void );


void PVD_IRQHandler(void );


void TAMPER_IRQHandler(void );


void RTC_IRQHandler(void );


void FLASH_IRQHandler(void );


void RCC_IRQHandler(void );


void EXTI0_IRQHandler(void );


void EXTI1_IRQHandler(void );


void EXTI2_IRQHandler(void );


void EXTI3_IRQHandler(void );


void EXTI4_IRQHandler(void );


void DMAChannel1_IRQHandler(void );


void DMAChannel2_IRQHandler(void );


void DMAChannel3_IRQHandler(void );


void DMAChannel4_IRQHandler(void );


void DMAChannel5_IRQHandler(void );


void DMAChannel6_IRQHandler(void );


void DMAChannel7_IRQHandler(void );


void ADC_IRQHandler(void );


void USB_HP_CAN_TX_IRQHandler(void );


void USB_LP_CAN_RX0_IRQHandler(void );


void CAN_RX1_IRQHandler(void );


void CAN_SCE_IRQHandler(void );


void EXTI9_5_IRQHandler(void );


void TIM1_BRK_IRQHandler(void );


void TIM1_UP_IRQHandler(void );


void TIM1_TRG_COM_IRQHandler(void );


void TIM1_CC_IRQHandler(void );


void TIM2_IRQHandler(void );


void TIM3_IRQHandler(void );


void TIM4_IRQHandler(void );


void I2C1_EV_IRQHandler(void );


void I2C1_ER_IRQHandler(void );


void I2C2_EV_IRQHandler(void );


void I2C2_ER_IRQHandler(void );


void SPI1_IRQHandler(void );


void SPI2_IRQHandler(void );


void USART1_IRQHandler(void );


void USART2_IRQHandler(void );


void USART3_IRQHandler(void );


void EXTI15_10_IRQHandler(void );


void USBWakeUp_IRQHandler(void );
//# 36 "/opt/tinyos-2.x/tos/chips/stm32/stm32hardware.h"
typedef uint32_t __nesc_atomic_t;


__inline __nesc_atomic_t __nesc_atomic_start(void )  ;
//#line 58
__inline void __nesc_atomic_end(__nesc_atomic_t oldState)  ;
//#line 80
static __inline void __nesc_enable_interrupt(void);
//#line 95
static __inline void __nesc_disable_interrupt(void);
//#line 112
typedef uint8_t mcu_power_t  ;
//# 30 "/opt/tinyos-2.x/tos/chips/stm32/fwlib/inc/stm32f10x_exti.h"
//#line 26
typedef enum __nesc_unnamed4293 {

  EXTI_Mode_Interrupt = 0x00, 
  EXTI_Mode_Event = 0x04
} EXTIMode_TypeDef;









//#line 35
typedef enum __nesc_unnamed4294 {

  EXTI_Trigger_Rising = 0x08, 
  EXTI_Trigger_Falling = 0x0C, 
  EXTI_Trigger_Rising_Falling = 0x10
} EXTITrigger_TypeDef;
//#line 53
//#line 47
typedef struct __nesc_unnamed4295 {

  uint32_t EXTI_Line;
  EXTIMode_TypeDef EXTI_Mode;
  EXTITrigger_TypeDef EXTI_Trigger;
  FunctionalState EXTI_LineCmd;
} EXTI_InitTypeDef;
//#line 97
void EXTI_Init(EXTI_InitTypeDef *EXTI_InitStruct);





void EXTI_ClearITPendingBit(uint32_t EXTI_Line);
//# 67 "/opt/tinyos-2.x/tos/chips/stm32/fwlib/inc/stm32f10x_nvic.h"
//#line 49
typedef struct __nesc_unnamed4296 {

  uint8_t NVIC_IRQChannel;




  uint8_t NVIC_IRQChannelPreemptionPriority;



  uint8_t NVIC_IRQChannelSubPriority;



  FunctionalState NVIC_IRQChannelCmd;
} 

NVIC_InitTypeDef;
//#line 195
void NVIC_PriorityGroupConfig(uint32_t NVIC_PriorityGroup);
void NVIC_Init(NVIC_InitTypeDef *NVIC_InitStruct);
//# 52 "/opt/tinyos-2.x/tos/chips/stm32/fwlib/inc/stm32f10x_rcc.h"
//#line 45
typedef struct __nesc_unnamed4297 {

  uint32_t SYSCLK_Frequency;
  uint32_t HCLK_Frequency;
  uint32_t PCLK1_Frequency;
  uint32_t PCLK2_Frequency;
  uint32_t ADCCLK_Frequency;
} RCC_ClocksTypeDef;
//#line 647
void RCC_HSEConfig(uint32_t RCC_HSE);


void RCC_HSICmd(FunctionalState NewState);
void RCC_PLLConfig(uint32_t RCC_PLLSource, uint32_t RCC_PLLMul);
void RCC_PLLCmd(FunctionalState NewState);
//#line 666
void RCC_SYSCLKConfig(uint32_t RCC_SYSCLKSource);

void RCC_HCLKConfig(uint32_t RCC_SYSCLK);
void RCC_PCLK1Config(uint32_t RCC_HCLK);
void RCC_PCLK2Config(uint32_t RCC_HCLK);



void RCC_USBCLKConfig(uint32_t RCC_USBCLKSource);




void RCC_ADCCLKConfig(uint32_t RCC_PCLK2);






void RCC_LSEConfig(uint8_t RCC_LSE);

void RCC_RTCCLKConfig(uint32_t RCC_RTCCLKSource);
void RCC_RTCCLKCmd(FunctionalState NewState);

void RCC_AHBPeriphClockCmd(uint32_t RCC_AHBPeriph, FunctionalState NewState);
void RCC_APB2PeriphClockCmd(uint32_t RCC_APB2Periph, FunctionalState NewState);
void RCC_APB1PeriphClockCmd(uint32_t RCC_APB1Periph, FunctionalState NewState);










FlagStatus RCC_GetFlagStatus(uint8_t RCC_FLAG);
//# 31 "/opt/tinyos-2.x/tos/chips/stm32/fwlib/inc/stm32f10x_gpio.h"
//#line 26
typedef enum __nesc_unnamed4298 {

  GPIO_Speed_10MHz = 1, 
  GPIO_Speed_2MHz, 
  GPIO_Speed_50MHz
} GPIOSpeed_TypeDef;
//#line 46
//#line 37
typedef enum __nesc_unnamed4299 {
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
typedef struct __nesc_unnamed4300 {

  uint16_t GPIO_Pin;
  GPIOSpeed_TypeDef GPIO_Speed;
  GPIOMode_TypeDef GPIO_Mode;
} GPIO_InitTypeDef;





//#line 62
typedef enum __nesc_unnamed4301 {
  Bit_RESET = 0, 
  Bit_SET
} BitAction;
//#line 192
void GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_InitStruct);
//# 53 "/opt/tinyos-2.x/tos/chips/stm32/fwlib/inc/stm32f10x_rtc.h"
void RTC_ITConfig(uint16_t RTC_IT, FunctionalState NewState);


uint32_t RTC_GetCounter(void );
void RTC_SetCounter(uint32_t CounterValue);
void RTC_SetPrescaler(uint32_t PrescalerValue);
void RTC_SetAlarm(uint32_t AlarmValue);

void RTC_WaitForLastTask(void );
void RTC_WaitForSynchro(void );


ITStatus RTC_GetITStatus(uint16_t RTC_IT);
void RTC_ClearITPendingBit(uint16_t RTC_IT);
//# 66 "/opt/tinyos-2.x/tos/chips/stm32/fwlib/inc/stm32f10x_pwr.h"
void PWR_BackupAccessCmd(FunctionalState NewState);





FlagStatus PWR_GetFlagStatus(uint32_t PWR_FLAG);
void PWR_ClearFlag(uint32_t PWR_FLAG);
//# 32 "/opt/tinyos-2.x/tos/types/Leds.h"
enum __nesc_unnamed4302 {
  LEDS_LED0 = 1 << 0, 
  LEDS_LED1 = 1 << 1, 
  LEDS_LED2 = 1 << 2, 
  LEDS_LED3 = 1 << 3, 
  LEDS_LED4 = 1 << 4, 
  LEDS_LED5 = 1 << 5, 
  LEDS_LED6 = 1 << 6, 
  LEDS_LED7 = 1 << 7
};
//# 29 "/opt/tinyos-2.x/tos/lib/timer/Timer.h"
typedef struct __nesc_unnamed4303 {
//#line 29
  int notUsed;
} 
//#line 29
TMilli;
typedef struct __nesc_unnamed4304 {
//#line 30
  int notUsed;
} 
//#line 30
T32khz;
typedef struct __nesc_unnamed4305 {
//#line 31
  int notUsed;
} 
//#line 31
TMicro;
//# 33 "/opt/tinyos-2.x/tos/chips/stm32/fwlib/inc/stm32f10x_tim.h"
//#line 27
typedef struct __nesc_unnamed4306 {

  uint16_t TIM_Period;
  uint16_t TIM_Prescaler;
  uint16_t TIM_ClockDivision;
  uint16_t TIM_CounterMode;
} TIM_TimeBaseInitTypeDef;








//#line 36
typedef struct __nesc_unnamed4307 {

  uint16_t TIM_OCMode;
  uint16_t TIM_Channel;
  uint16_t TIM_Pulse;
  uint16_t TIM_OCPolarity;
} TIM_OCInitTypeDef;










//#line 45
typedef struct __nesc_unnamed4308 {

  uint16_t TIM_ICMode;
  uint16_t TIM_Channel;
  uint16_t TIM_ICPolarity;
  uint16_t TIM_ICSelection;
  uint16_t TIM_ICPrescaler;
  uint8_t TIM_ICFilter;
} TIM_ICInitTypeDef;
//#line 446
void TIM_ICInit(TIM_TypeDef *TIMx, TIM_ICInitTypeDef *TIM_ICInitStruct);



void TIM_Cmd(TIM_TypeDef *TIMx, FunctionalState NewState);
void TIM_ITConfig(TIM_TypeDef *TIMx, uint16_t TIM_IT, FunctionalState NewState);
//#line 501
void TIM_SetCompare1(TIM_TypeDef *TIMx, uint16_t Compare1);









uint16_t TIM_GetCapture2(TIM_TypeDef *TIMx);


uint16_t TIM_GetCounter(TIM_TypeDef *TIMx);
//# 65 "/opt/tinyos-2.x/tos/chips/stm32/fwlib/inc/stm32f10x_bkp.h"
void BKP_DeInit(void );
//# 8 "STMAppMessaging.h"
//#line 4
typedef nx_struct stm_app_msg {
  nx_uint8_t appId;
  nx_uint8_t length;
  nx_uint8_t data[100];
} __attribute__((packed)) stm_app_msg_t;


enum __nesc_unnamed4309 {
  AM_STM_APP_MSG = 100, 
  AM_STM_APP_MSG_HDR_LEN = 2
};
//# 34 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/IEEE154MessageLayer.h"
//#line 27
typedef nx_struct ieee154_header_t {

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
//# 30 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/LowpanNetworkLayer.h"
//#line 27
typedef nx_struct lowpan_header_t {

  nxle_uint8_t network;
} __attribute__((packed)) lowpan_header_t;
//# 30 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/ActiveMessageLayer.h"
//#line 27
typedef nx_struct activemessage_header_t {

  nxle_uint8_t type;
} __attribute__((packed)) activemessage_header_t;
//# 31 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/MetadataFlagsLayer.h"
//#line 27
typedef struct flags_metadata_t {


  uint8_t flags;
} flags_metadata_t;
//# 30 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230DriverLayer.h"
//#line 27
typedef nx_struct rf230_header_t {

  nxle_uint8_t length;
} __attribute__((packed)) rf230_header_t;









//#line 32
typedef struct rf230_metadata_t {

  uint8_t lqi;
  union  {

    uint8_t power;
    uint8_t rssi;
  } ;
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
  RF230_BUSY_RX_AACK = 16, 
  RF230_BUSR_TX_ARET = 17, 
  RF230_RX_AACK_ON = 22, 
  RF230_TX_ARET_ON = 25, 
  RF230_RX_ON_NOCLK = 28, 
  RF230_AACK_ON_NOCLK = 29, 
  RF230_BUSY_RX_AACK_NOCLK = 30, 
  RF230_STATE_TRANSITION_IN_PROGRESS = 31, 
  RF230_TRAC_STATUS_MASK = 0xE0, 
  RF230_TRAC_SUCCESS = 0, 
  RF230_TRAC_CHANNEL_ACCESS_FAILURE = 3 << 5, 
  RF230_TRAC_NO_ACK = 5 << 5, 
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

  RF230_PLL_CF_START = 1 << 7, 
  RF230_PLL_DCU_START = 1 << 7
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
//# 30 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/TimeStampingLayer.h"
//#line 27
typedef struct timestamp_metadata_t {

  uint32_t timestamp;
} timestamp_metadata_t;
//# 30 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/LowPowerListeningLayer.h"
//#line 27
typedef struct lpl_metadata_t {

  uint16_t sleepint;
} lpl_metadata_t;
//# 31 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/PacketLinkLayer.h"
//#line 27
typedef struct link_metadata_t {

  uint16_t maxRetries;
  uint16_t retryDelay;
} link_metadata_t;
//# 44 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230ActiveMessage.h"
//#line 36
typedef nx_struct rf230packet_header_t {

  rf230_header_t rf230;
  ieee154_header_t ieee154;

  lowpan_header_t lowpan;

  activemessage_header_t am;
} __attribute__((packed)) rf230packet_header_t;




//#line 46
typedef nx_struct rf230packet_footer_t {
} __attribute__((packed)) 

rf230packet_footer_t;
//#line 62
//#line 51
typedef struct rf230packet_metadata_t {







  timestamp_metadata_t timestamp;
  flags_metadata_t flags;
  rf230_metadata_t rf230;
} rf230packet_metadata_t;
//# 6 "/opt/tinyos-2.x/tos/types/AM.h"
typedef nx_uint8_t nx_am_id_t;
typedef nx_uint8_t nx_am_group_t;
typedef nx_uint16_t nx_am_addr_t;

typedef uint8_t am_id_t;
typedef uint8_t am_group_t;
typedef uint16_t am_addr_t;

enum __nesc_unnamed4310 {
  AM_BROADCAST_ADDR = 0xffff
};









enum __nesc_unnamed4311 {
  TOS_AM_GROUP = 0x22, 
  TOS_AM_ADDRESS = 1
};
//# 72 "/opt/tinyos-2.x/tos/lib/serial/Serial.h"
typedef uint8_t uart_id_t;



enum __nesc_unnamed4312 {
  HDLC_FLAG_BYTE = 0x7e, 
  HDLC_CTLESC_BYTE = 0x7d
};



enum __nesc_unnamed4313 {
  TOS_SERIAL_ACTIVE_MESSAGE_ID = 0, 
  TOS_SERIAL_CC1000_ID = 1, 
  TOS_SERIAL_802_15_4_ID = 2, 
  TOS_SERIAL_UNKNOWN_ID = 255
};


enum __nesc_unnamed4314 {
  SERIAL_PROTO_ACK = 67, 
  SERIAL_PROTO_PACKET_ACK = 68, 
  SERIAL_PROTO_PACKET_NOACK = 69, 
  SERIAL_PROTO_PACKET_UNKNOWN = 255
};
//#line 110
//#line 98
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







//#line 112
typedef nx_struct serial_header {
  nx_am_addr_t dest;
  nx_am_addr_t src;
  nx_uint8_t length;
  nx_am_group_t group;
  nx_am_id_t type;
} __attribute__((packed)) serial_header_t;




//#line 120
typedef nx_struct serial_packet {
  serial_header_t header;
  nx_uint8_t data[];
} __attribute__((packed)) serial_packet_t;



//#line 125
typedef nx_struct serial_metadata {
  nx_uint8_t ack;
} __attribute__((packed)) serial_metadata_t;
//# 32 "/opt/tinyos-2.x/tos/platforms/stm32-p103/platform_message.h"
//#line 29
typedef union message_header {
  rf230packet_header_t rf230;
  serial_header_t serial;
} message_header_t;



//#line 34
typedef union message_footer {
  rf230packet_footer_t rf230;
} message_footer_t;



//#line 38
typedef union message_metadata {
  rf230packet_metadata_t rf230;
} message_metadata_t;
//# 19 "/opt/tinyos-2.x/tos/types/message.h"
//#line 14
typedef nx_struct message_t {
  nx_uint8_t header[sizeof(message_header_t )];
  nx_uint8_t data[120];
  nx_uint8_t footer[sizeof(message_footer_t )];
  nx_uint8_t metadata[sizeof(message_metadata_t )];
} __attribute__((packed)) message_t;
//# 8 "/opt/tinyos-2.x/tos/platforms/stm32-p103/STM32Timer.h"
typedef struct __nesc_unnamed4315 {
} 
//#line 8
T64khz;
typedef struct __nesc_unnamed4316 {
} 
//#line 9
T128khz;
typedef struct __nesc_unnamed4317 {
} 
//#line 10
T2mhz;
typedef struct __nesc_unnamed4318 {
} 
//#line 11
T4mhz;

typedef TMicro TOne;
typedef TMicro TThree;
typedef uint32_t counter_one_overflow_t;
typedef uint16_t counter_three_overflow_t;
//# 30 "/opt/tinyos-2.x/tos/platforms/stm32-p103/chips/rf230/RadioConfig.h"
enum __nesc_unnamed4319 {





  RF230_TRX_CTRL_0_VALUE = 0, 





  RF230_CCA_MODE_VALUE = RF230_CCA_MODE_3, 





  RF230_CCA_THRES_VALUE = 0xC7
};
//#line 70
static __inline uint16_t RF230_CRCBYTE_COMMAND(uint16_t crc, uint8_t data);









typedef TOne TRadio;
//# 42 "/opt/tinyos-2.x/tos/chips/rf2xx/util/IEEE802154.h"
typedef uint16_t ieee154_panid_t;
typedef uint16_t ieee154_saddr_t;

enum __nesc_unnamed4320 {
  IEEE154_BROADCAST_ADDR = 0xffff
};
//# 28 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/TimeSyncMessageLayer.h"
typedef nx_int32_t timesync_relative_t;


typedef uint32_t timesync_absolute_t;
//# 80 "/opt/tinyos-2.x/tos/chips/stm32/fwlib/inc/stm32f10x_spi.h"
//#line 49
typedef struct __nesc_unnamed4321 {

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
//#line 106
//#line 86
typedef struct __nesc_unnamed4322 {


  uint16_t I2S_Mode;


  uint16_t I2S_Standard;


  uint16_t I2S_DataFormat;


  uint16_t I2S_MCLKOutput;


  uint32_t I2S_AudioFreq;


  uint16_t I2S_CPOL;
} 
I2S_InitTypeDef;
//#line 454
void SPI_Cmd(SPI_TypeDef *SPIx, FunctionalState NewState);



void SPI_I2S_SendData(SPI_TypeDef *SPIx, uint16_t Data);
uint16_t SPI_I2S_ReceiveData(SPI_TypeDef *SPIx);








FlagStatus SPI_I2S_GetFlagStatus(SPI_TypeDef *SPIx, uint16_t SPI_I2S_FLAG);
//# 11 "/opt/tinyos-2.x/tos/chips/stm32/spi/Stm32Spi.h"
enum __nesc_unnamed4323 {
  STM32_SPI_CLK_DIVIDE_4 = 0, 
  STM32_SPI_CLK_DIVIDE_16 = 1, 
  STM32_SPI_CLK_DIVIDE_64 = 2, 
  STM32_SPI_CLK_DIVIDE_128 = 3
};

SPI_InitTypeDef SPI_InitStructure;


extern uint16_t SPI_I2S_ReceiveData(SPI_TypeDef *SPIx);
//# 33 "/opt/tinyos-2.x/tos/types/Resource.h"
typedef uint8_t resource_client_id_t;
typedef TMilli /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$precision_tag;
typedef /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$precision_tag /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$TimerFrom$precision_tag;
typedef /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$precision_tag /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$Timer$precision_tag;
typedef TMilli /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$precision_tag;
typedef /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$precision_tag /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Alarm$precision_tag;
typedef uint32_t /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Alarm$size_type;
typedef /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$precision_tag /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Timer$precision_tag;
typedef TMilli STM32RtcC$LocalTime$precision_tag;
typedef TMilli STM32RtcC$Alarm$precision_tag;
typedef uint32_t STM32RtcC$Alarm$size_type;
typedef TMilli STM32RtcC$Counter$precision_tag;
typedef uint32_t STM32RtcC$Counter$size_type;
enum AMQueueP$__nesc_unnamed4324 {
  AMQueueP$NUM_CLIENTS = 1U
};
typedef TRadio RF230ActiveMessageP$PacketTimeStamp$precision_tag;
typedef uint32_t RF230ActiveMessageP$PacketTimeStamp$size_type;
typedef TRadio RadioAlarmP$Alarm$precision_tag;
typedef uint16_t RadioAlarmP$Alarm$size_type;
typedef uint16_t RandomMlcgC$SeedInit$parameter;
typedef TRadio TimeStampingLayerP$PacketTimeStampRadio$precision_tag;
typedef uint32_t TimeStampingLayerP$PacketTimeStampRadio$size_type;
typedef TRadio TimeStampingLayerP$LocalTimeRadio$precision_tag;
typedef TMilli TimeStampingLayerP$LocalTimeMilli$precision_tag;
typedef TMilli TimeStampingLayerP$PacketTimeStampMilli$precision_tag;
typedef uint32_t TimeStampingLayerP$PacketTimeStampMilli$size_type;
typedef TMicro RF230DriverLayerP$BusyWait$precision_tag;
typedef uint16_t RF230DriverLayerP$BusyWait$size_type;
typedef uint8_t RF230DriverLayerP$PacketRSSI$value_type;
typedef TRadio RF230DriverLayerP$PacketTimeStamp$precision_tag;
typedef uint32_t RF230DriverLayerP$PacketTimeStamp$size_type;
typedef TRadio RF230DriverLayerP$LocalTime$precision_tag;
typedef uint8_t RF230DriverLayerP$PacketTransmitPower$value_type;
typedef uint8_t RF230DriverLayerP$PacketTimeSyncOffset$value_type;
typedef uint8_t RF230DriverLayerP$PacketLinkQuality$value_type;
typedef uint16_t HplRF230P$Capture$size_type;
typedef uint16_t HplStm32Timer3P$CompareA$size_type;
typedef uint16_t HplStm32Timer3P$Capture$size_type;
typedef uint16_t HplStm32Timer3P$CompareC$size_type;
typedef uint16_t HplStm32Timer3P$Timer$timer_size;
typedef TOne /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$frequency_tag;
typedef uint16_t /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$timer_size;
typedef /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$frequency_tag /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$Alarm$precision_tag;
typedef /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$timer_size /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$Alarm$size_type;
typedef /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$timer_size /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$HplStm32Timer$timer_size;
typedef /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$timer_size /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$HplStm32Compare$size_type;
enum /*HplRF230C.AlarmC*/AlarmOne16C$0$__nesc_unnamed4325 {
  AlarmOne16C$0$COMPARE_ID = 0U
};
typedef TOne /*CounterOne16C.NCounter*/Stm32CounterC$0$frequency_tag;
typedef uint16_t /*CounterOne16C.NCounter*/Stm32CounterC$0$timer_size;
typedef /*CounterOne16C.NCounter*/Stm32CounterC$0$frequency_tag /*CounterOne16C.NCounter*/Stm32CounterC$0$Counter$precision_tag;
typedef /*CounterOne16C.NCounter*/Stm32CounterC$0$timer_size /*CounterOne16C.NCounter*/Stm32CounterC$0$Counter$size_type;
typedef /*CounterOne16C.NCounter*/Stm32CounterC$0$timer_size /*CounterOne16C.NCounter*/Stm32CounterC$0$Timer$timer_size;
typedef TMicro /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$to_precision_tag;
typedef uint32_t /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$to_size_type;
typedef TMicro /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$from_precision_tag;
typedef uint16_t /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$from_size_type;
typedef uint32_t /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$upper_count_type;
typedef /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$from_precision_tag /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$CounterFrom$precision_tag;
typedef /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$from_size_type /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$CounterFrom$size_type;
typedef /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$to_precision_tag /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$Counter$precision_tag;
typedef /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$to_size_type /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$Counter$size_type;
typedef TMicro /*LocalTimeMicroC.CounterToLocalTimeC*/CounterToLocalTimeC$0$precision_tag;
typedef /*LocalTimeMicroC.CounterToLocalTimeC*/CounterToLocalTimeC$0$precision_tag /*LocalTimeMicroC.CounterToLocalTimeC*/CounterToLocalTimeC$0$LocalTime$precision_tag;
typedef /*LocalTimeMicroC.CounterToLocalTimeC*/CounterToLocalTimeC$0$precision_tag /*LocalTimeMicroC.CounterToLocalTimeC*/CounterToLocalTimeC$0$Counter$precision_tag;
typedef uint32_t /*LocalTimeMicroC.CounterToLocalTimeC*/CounterToLocalTimeC$0$Counter$size_type;
typedef TMicro /*BusyWaitMicroC.Stm32BusyWaitMicro*/HalStm32BusyWaitM$0$precision_tag;
typedef /*BusyWaitMicroC.Stm32BusyWaitMicro*/HalStm32BusyWaitM$0$precision_tag /*BusyWaitMicroC.Stm32BusyWaitMicro*/HalStm32BusyWaitM$0$BusyWait$precision_tag;
typedef uint16_t /*BusyWaitMicroC.Stm32BusyWaitMicro*/HalStm32BusyWaitM$0$BusyWait$size_type;
typedef TMilli /*BusyWaitMicroC.Stm32BusyWaitMicro*/HalStm32BusyWaitM$0$OST$precision_tag;
typedef uint32_t /*BusyWaitMicroC.Stm32BusyWaitMicro*/HalStm32BusyWaitM$0$OST$size_type;
typedef TMilli STMTestC$Timer0$precision_tag;
//# 51 "/opt/tinyos-2.x/tos/interfaces/Init.nc"
static  error_t PlatformP$Init$init(void);
//# 31 "/opt/tinyos-2.x/tos/chips/stm32/HplSTM32Interrupt.nc"
static   void PlatformP$Interrupt$fired(void);
//# 51 "/opt/tinyos-2.x/tos/interfaces/Init.nc"
static  error_t MoteClockP$MoteClockInit$init(void);
//# 59 "/opt/tinyos-2.x/tos/interfaces/McuSleep.nc"
static   void McuSleepC$McuSleep$sleep(void);
//# 44 "/opt/tinyos-2.x/tos/interfaces/McuPowerState.nc"
static   void McuSleepC$McuPowerState$update(void);
//# 51 "/opt/tinyos-2.x/tos/interfaces/Init.nc"
static  error_t McuSleepC$McuSleepInit$init(void);
//# 56 "/opt/tinyos-2.x/tos/interfaces/TaskBasic.nc"
static   error_t SchedulerBasicP$TaskBasic$postTask(
//# 45 "/opt/tinyos-2.x/tos/system/SchedulerBasicP.nc"
uint8_t arg_0x4044e188);
//# 64 "/opt/tinyos-2.x/tos/interfaces/TaskBasic.nc"
static  void SchedulerBasicP$TaskBasic$default$runTask(
//# 45 "/opt/tinyos-2.x/tos/system/SchedulerBasicP.nc"
uint8_t arg_0x4044e188);
//# 46 "/opt/tinyos-2.x/tos/interfaces/Scheduler.nc"
static  void SchedulerBasicP$Scheduler$init(void);
//#line 61
static  void SchedulerBasicP$Scheduler$taskLoop(void);
//#line 54
static  bool SchedulerBasicP$Scheduler$runNextTask(void);
//# 51 "/opt/tinyos-2.x/tos/interfaces/Init.nc"
static  error_t LedsP$Init$init(void);
//# 56 "/opt/tinyos-2.x/tos/interfaces/Leds.nc"
static   void LedsP$Leds$led0Toggle(void);
//#line 72
static   void LedsP$Leds$led1Toggle(void);
//#line 89
static   void LedsP$Leds$led2Toggle(void);
//# 33 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
static   void /*HplSTM32GeneralIOC.PortA.Bit0*/HplSTM32GeneralIOPinP$0$IO$makeInput(void);
//#line 30
static   void /*HplSTM32GeneralIOC.PortA.Bit0*/HplSTM32GeneralIOPinP$0$IO$clr(void);




static   void /*HplSTM32GeneralIOC.PortA.Bit5*/HplSTM32GeneralIOPinP$5$IO$makeOutput(void);
//#line 33
static   void /*HplSTM32GeneralIOC.PortA.Bit6*/HplSTM32GeneralIOPinP$6$IO$makeInput(void);

static   void /*HplSTM32GeneralIOC.PortA.Bit6*/HplSTM32GeneralIOPinP$6$IO$makeOutput(void);
//#line 29
static   void /*HplSTM32GeneralIOC.PortA.Bit6*/HplSTM32GeneralIOPinP$6$IO$set(void);
static   void /*HplSTM32GeneralIOC.PortA.Bit6*/HplSTM32GeneralIOPinP$6$IO$clr(void);




static   void /*HplSTM32GeneralIOC.PortA.Bit7*/HplSTM32GeneralIOPinP$7$IO$makeOutput(void);
//#line 35
static   void /*HplSTM32GeneralIOC.PortB.Bit0*/HplSTM32GeneralIOPinP$16$IO$makeOutput(void);
//#line 29
static   void /*HplSTM32GeneralIOC.PortB.Bit0*/HplSTM32GeneralIOPinP$16$IO$set(void);
static   void /*HplSTM32GeneralIOC.PortB.Bit0*/HplSTM32GeneralIOPinP$16$IO$clr(void);




static   void /*HplSTM32GeneralIOC.PortB.Bit7*/HplSTM32GeneralIOPinP$23$IO$makeOutput(void);
//#line 29
static   void /*HplSTM32GeneralIOC.PortB.Bit7*/HplSTM32GeneralIOPinP$23$IO$set(void);
static   void /*HplSTM32GeneralIOC.PortB.Bit7*/HplSTM32GeneralIOPinP$23$IO$clr(void);
static   void /*HplSTM32GeneralIOC.PortC.Bit8*/HplSTM32GeneralIOPinP$40$IO$toggle(void);



static   void /*HplSTM32GeneralIOC.PortC.Bit8*/HplSTM32GeneralIOPinP$40$IO$makeOutput(void);
//#line 29
static   void /*HplSTM32GeneralIOC.PortC.Bit8*/HplSTM32GeneralIOPinP$40$IO$set(void);

static   void /*HplSTM32GeneralIOC.PortC.Bit9*/HplSTM32GeneralIOPinP$41$IO$toggle(void);



static   void /*HplSTM32GeneralIOC.PortC.Bit9*/HplSTM32GeneralIOPinP$41$IO$makeOutput(void);
//#line 29
static   void /*HplSTM32GeneralIOC.PortC.Bit9*/HplSTM32GeneralIOPinP$41$IO$set(void);

static   void /*HplSTM32GeneralIOC.PortC.Bit12*/HplSTM32GeneralIOPinP$44$IO$toggle(void);



static   void /*HplSTM32GeneralIOC.PortC.Bit12*/HplSTM32GeneralIOPinP$44$IO$makeOutput(void);
//#line 29
static   void /*HplSTM32GeneralIOC.PortC.Bit12*/HplSTM32GeneralIOPinP$44$IO$set(void);
//# 64 "/opt/tinyos-2.x/tos/interfaces/TaskBasic.nc"
static  void /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$updateFromTimer$runTask(void);
//# 72 "/opt/tinyos-2.x/tos/lib/timer/Timer.nc"
static  void /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$TimerFrom$fired(void);
//#line 72
static  void /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$Timer$default$fired(
//# 37 "/opt/tinyos-2.x/tos/lib/timer/VirtualizeTimerC.nc"
uint8_t arg_0x406329a0);
//# 64 "/opt/tinyos-2.x/tos/interfaces/TaskBasic.nc"
static  void /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$fired$runTask(void);
//# 67 "/opt/tinyos-2.x/tos/lib/timer/Alarm.nc"
static   void /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Alarm$fired(void);
//# 125 "/opt/tinyos-2.x/tos/lib/timer/Timer.nc"
static  uint32_t /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Timer$getNow(void);
//#line 118
static  void /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Timer$startOneShotAt(uint32_t arg_0x4060f9b8, uint32_t arg_0x4060fb48);
//#line 67
static  void /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Timer$stop(void);
//# 98 "/opt/tinyos-2.x/tos/lib/timer/Alarm.nc"
static   STM32RtcC$Alarm$size_type STM32RtcC$Alarm$getNow(void);
//#line 92
static   void STM32RtcC$Alarm$startAt(STM32RtcC$Alarm$size_type arg_0x40667e78, STM32RtcC$Alarm$size_type arg_0x40666030);
//#line 105
static   STM32RtcC$Alarm$size_type STM32RtcC$Alarm$getAlarm(void);
//#line 62
static   void STM32RtcC$Alarm$stop(void);
//# 51 "/opt/tinyos-2.x/tos/interfaces/Init.nc"
static  error_t STM32RtcC$Init$init(void);
//# 53 "/opt/tinyos-2.x/tos/lib/timer/Counter.nc"
static   STM32RtcC$Counter$size_type STM32RtcC$Counter$get(void);
//# 92 "/opt/tinyos-2.x/tos/interfaces/SplitControl.nc"
static  void STMAppMessagingP$AMControl$startDone(error_t arg_0x406deb10);
//#line 117
static  void STMAppMessagingP$AMControl$stopDone(error_t arg_0x406dd6e8);
//# 99 "/opt/tinyos-2.x/tos/interfaces/AMSend.nc"
static  void STMAppMessagingP$AMSend$sendDone(message_t *arg_0x407101c0, error_t arg_0x40710348);
//# 67 "/opt/tinyos-2.x/tos/interfaces/Receive.nc"
static  message_t *STMAppMessagingP$Receive$receive(message_t *arg_0x4070c340, void *arg_0x4070c4e0, uint8_t arg_0x4070c668);
//# 5 "STMAppMessaging.nc"
static  error_t STMAppMessagingP$STMAppMessaging$Send(uint8_t arg_0x406e4968, uint16_t arg_0x406e4af8, uint8_t *arg_0x406e4ca0, uint16_t arg_0x406e4e30);
//#line 3
static  void STMAppMessagingP$STMAppMessaging$Init(void);
//# 69 "/opt/tinyos-2.x/tos/interfaces/AMSend.nc"
static  error_t /*STMAppMessagingC.AMSenderC.AMQueueEntryP*/AMQueueEntryP$0$AMSend$send(am_addr_t arg_0x406fce00, message_t *arg_0x40711010, uint8_t arg_0x40711198);
//# 89 "/opt/tinyos-2.x/tos/interfaces/Send.nc"
static  void /*STMAppMessagingC.AMSenderC.AMQueueEntryP*/AMQueueEntryP$0$Send$sendDone(message_t *arg_0x40728e18, error_t arg_0x4073d010);
//# 99 "/opt/tinyos-2.x/tos/interfaces/AMSend.nc"
static  void /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$AMSend$sendDone(
//# 40 "/opt/tinyos-2.x/tos/system/AMQueueImplP.nc"
am_id_t arg_0x40746be8, 
//# 99 "/opt/tinyos-2.x/tos/interfaces/AMSend.nc"
message_t *arg_0x407101c0, error_t arg_0x40710348);
//# 64 "/opt/tinyos-2.x/tos/interfaces/Send.nc"
static  error_t /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$Send$send(
//# 38 "/opt/tinyos-2.x/tos/system/AMQueueImplP.nc"
uint8_t arg_0x40746288, 
//# 64 "/opt/tinyos-2.x/tos/interfaces/Send.nc"
message_t *arg_0x40729cb0, uint8_t arg_0x40729e38);
//#line 89
static  void /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$Send$default$sendDone(
//# 38 "/opt/tinyos-2.x/tos/system/AMQueueImplP.nc"
uint8_t arg_0x40746288, 
//# 89 "/opt/tinyos-2.x/tos/interfaces/Send.nc"
message_t *arg_0x40728e18, error_t arg_0x4073d010);
//# 64 "/opt/tinyos-2.x/tos/interfaces/TaskBasic.nc"
static  void /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$errorTask$runTask(void);
//#line 64
static  void /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$CancelTask$runTask(void);
//# 29 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/ActiveMessageConfig.nc"
static  am_addr_t RF230ActiveMessageP$ActiveMessageConfig$destination(message_t *arg_0x407c2eb8);








static  void RF230ActiveMessageP$ActiveMessageConfig$setSource(message_t *arg_0x407c1ee0, am_addr_t arg_0x407c0088);
//#line 32
static  void RF230ActiveMessageP$ActiveMessageConfig$setDestination(message_t *arg_0x407c1398, am_addr_t arg_0x407c1528);
//#line 44
static  void RF230ActiveMessageP$ActiveMessageConfig$setGroup(message_t *arg_0x407c0a38, am_group_t arg_0x407c0bc8);
//# 48 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioAlarm.nc"
static   void RF230ActiveMessageP$RadioAlarm$fired(void);
//# 35 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230DriverConfig.nc"
static   uint8_t RF230ActiveMessageP$RF230DriverConfig$maxPayloadLength(void);
//#line 29
static   uint8_t RF230ActiveMessageP$RF230DriverConfig$headerLength(message_t *arg_0x407a8228);
//#line 41
static   uint8_t RF230ActiveMessageP$RF230DriverConfig$metadataLength(message_t *arg_0x407a8ae0);






static   uint8_t RF230ActiveMessageP$RF230DriverConfig$headerPreloadLength(void);





static   bool RF230ActiveMessageP$RF230DriverConfig$requiresRssiCca(message_t *arg_0x407a74d8);
//# 75 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/SoftwareAckConfig.nc"
static   void RF230ActiveMessageP$SoftwareAckConfig$reportChannelError(void);
//#line 69
static   void RF230ActiveMessageP$SoftwareAckConfig$createAckPacket(message_t *arg_0x407a2678, message_t *arg_0x407a2828);
//#line 44
static   bool RF230ActiveMessageP$SoftwareAckConfig$requiresAckWait(message_t *arg_0x407a5df8);






static   bool RF230ActiveMessageP$SoftwareAckConfig$isAckPacket(message_t *arg_0x407a4380);






static   bool RF230ActiveMessageP$SoftwareAckConfig$verifyAckPacket(message_t *arg_0x407a4970, message_t *arg_0x407a4b20);
//#line 32
static   uint16_t RF230ActiveMessageP$SoftwareAckConfig$getAckTimeout(void);
//#line 64
static   bool RF230ActiveMessageP$SoftwareAckConfig$requiresAckReply(message_t *arg_0x407a2140);
//# 41 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/UniqueConfig.nc"
static   void RF230ActiveMessageP$UniqueConfig$setSequenceNumber(message_t *arg_0x407b8690, uint8_t arg_0x407b8818);





static   void RF230ActiveMessageP$UniqueConfig$reportChannelError(void);
//#line 31
static   uint8_t RF230ActiveMessageP$UniqueConfig$getSequenceNumber(message_t *arg_0x407a0c70);




static   am_addr_t RF230ActiveMessageP$UniqueConfig$getSender(message_t *arg_0x407b8190);
//# 35 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/RandomCollisionConfig.nc"
static   uint16_t RF230ActiveMessageP$RandomCollisionConfig$getCongestionBackoff(message_t *arg_0x407afb60);
//#line 29
static   uint16_t RF230ActiveMessageP$RandomCollisionConfig$getInitialBackoff(message_t *arg_0x407af630);










static   uint16_t RF230ActiveMessageP$RandomCollisionConfig$getMinimumBackoff(void);





static   uint16_t RF230ActiveMessageP$RandomCollisionConfig$getTransmitBarrier(message_t *arg_0x407ae450);
//# 48 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioAlarm.nc"
static   void RadioAlarmP$RadioAlarm$default$fired(
//# 31 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioAlarmP.nc"
uint8_t arg_0x4080dc60);
//# 38 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioAlarm.nc"
static   void RadioAlarmP$RadioAlarm$wait(
//# 31 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioAlarmP.nc"
uint8_t arg_0x4080dc60, 
//# 38 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioAlarm.nc"
uint16_t arg_0x407c7958);




static   void RadioAlarmP$RadioAlarm$cancel(
//# 31 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioAlarmP.nc"
uint8_t arg_0x4080dc60);
//# 33 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioAlarm.nc"
static   bool RadioAlarmP$RadioAlarm$isFree(
//# 31 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioAlarmP.nc"
uint8_t arg_0x4080dc60);
//# 53 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioAlarm.nc"
static   uint16_t RadioAlarmP$RadioAlarm$getNow(
//# 31 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioAlarmP.nc"
uint8_t arg_0x4080dc60);
//# 67 "/opt/tinyos-2.x/tos/lib/timer/Alarm.nc"
static   void RadioAlarmP$Alarm$fired(void);
//# 37 "/opt/tinyos-2.x/tos/chips/rf2xx/util/Tasklet.nc"
static   void RadioAlarmP$Tasklet$run(void);










static   void TaskletC$Tasklet$schedule(void);
//#line 61
static  void TaskletC$Tasklet$suspend(void);






static  void TaskletC$Tasklet$resume(void);
//# 43 "/opt/tinyos-2.x/tos/chips/rf2xx/util/BareSend.nc"
static  void ActiveMessageLayerP$SubSend$sendDone(message_t *arg_0x408221f8, error_t arg_0x40822380);
//# 31 "/opt/tinyos-2.x/tos/chips/rf2xx/util/BareReceive.nc"
static  message_t *ActiveMessageLayerP$SubReceive$receive(message_t *arg_0x4083b1f8);
//# 59 "/opt/tinyos-2.x/tos/interfaces/SendNotifier.nc"
static  void ActiveMessageLayerP$SendNotifier$default$aboutToSend(
//# 36 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/ActiveMessageLayerP.nc"
am_id_t arg_0x408357e0, 
//# 59 "/opt/tinyos-2.x/tos/interfaces/SendNotifier.nc"
am_addr_t arg_0x4078f478, message_t *arg_0x4078f628);
//# 67 "/opt/tinyos-2.x/tos/interfaces/Packet.nc"
static  uint8_t ActiveMessageLayerP$Packet$payloadLength(message_t *arg_0x4070aaf0);
//#line 115
static  void *ActiveMessageLayerP$Packet$getPayload(message_t *arg_0x40709ef0, uint8_t arg_0x40708088);
//#line 95
static  uint8_t ActiveMessageLayerP$Packet$maxPayloadLength(void);
//#line 83
static  void ActiveMessageLayerP$Packet$setPayloadLength(message_t *arg_0x40709190, uint8_t arg_0x40709318);
//# 38 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioPacket.nc"
static   uint8_t ActiveMessageLayerP$RadioPacket$payloadLength(message_t *arg_0x407de250);









static   uint8_t ActiveMessageLayerP$RadioPacket$maxPayloadLength(void);
//#line 43
static   void ActiveMessageLayerP$RadioPacket$setPayloadLength(message_t *arg_0x407de7a8, uint8_t arg_0x407de930);
//#line 32
static   uint8_t ActiveMessageLayerP$RadioPacket$headerLength(message_t *arg_0x407c9c68);
//# 69 "/opt/tinyos-2.x/tos/interfaces/AMSend.nc"
static  error_t ActiveMessageLayerP$AMSend$send(
//# 33 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/ActiveMessageLayerP.nc"
am_id_t arg_0x408372f8, 
//# 69 "/opt/tinyos-2.x/tos/interfaces/AMSend.nc"
am_addr_t arg_0x406fce00, message_t *arg_0x40711010, uint8_t arg_0x40711198);
//# 67 "/opt/tinyos-2.x/tos/interfaces/Receive.nc"
static  message_t *ActiveMessageLayerP$Snoop$default$receive(
//# 35 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/ActiveMessageLayerP.nc"
am_id_t arg_0x40835228, 
//# 67 "/opt/tinyos-2.x/tos/interfaces/Receive.nc"
message_t *arg_0x4070c340, void *arg_0x4070c4e0, uint8_t arg_0x4070c668);
//#line 67
static  message_t *ActiveMessageLayerP$Receive$default$receive(
//# 34 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/ActiveMessageLayerP.nc"
am_id_t arg_0x40837c28, 
//# 67 "/opt/tinyos-2.x/tos/interfaces/Receive.nc"
message_t *arg_0x4070c340, void *arg_0x4070c4e0, uint8_t arg_0x4070c668);
//# 57 "/opt/tinyos-2.x/tos/interfaces/AMPacket.nc"
static  am_addr_t ActiveMessageLayerP$AMPacket$address(void);









static  am_addr_t ActiveMessageLayerP$AMPacket$destination(message_t *arg_0x40722b50);
//#line 110
static  void ActiveMessageLayerP$AMPacket$setSource(message_t *arg_0x4071f030, am_addr_t arg_0x4071f1c0);
//#line 92
static  void ActiveMessageLayerP$AMPacket$setDestination(message_t *arg_0x40721768, am_addr_t arg_0x407218f8);
//#line 136
static  am_id_t ActiveMessageLayerP$AMPacket$type(message_t *arg_0x4071e0d0);
//#line 151
static  void ActiveMessageLayerP$AMPacket$setType(message_t *arg_0x4071e668, am_id_t arg_0x4071e7f0);
//#line 125
static  bool ActiveMessageLayerP$AMPacket$isForMe(message_t *arg_0x4071f978);
//#line 176
static  void ActiveMessageLayerP$AMPacket$setGroup(message_t *arg_0x4071d5a8, am_group_t arg_0x4071d738);







static  am_group_t ActiveMessageLayerP$AMPacket$localGroup(void);
//# 44 "/opt/tinyos-2.x/tos/system/ActiveMessageAddressC.nc"
static   am_addr_t ActiveMessageAddressC$amAddress(void);
//# 50 "/opt/tinyos-2.x/tos/interfaces/ActiveMessageAddress.nc"
static   am_addr_t ActiveMessageAddressC$ActiveMessageAddress$amAddress(void);




static   am_group_t ActiveMessageAddressC$ActiveMessageAddress$amGroup(void);
//# 43 "/opt/tinyos-2.x/tos/chips/rf2xx/util/BareSend.nc"
static  void LowpanNetworkLayerC$SubSend$sendDone(message_t *arg_0x408221f8, error_t arg_0x40822380);
//# 31 "/opt/tinyos-2.x/tos/chips/rf2xx/util/BareReceive.nc"
static  message_t *LowpanNetworkLayerC$SubReceive$receive(message_t *arg_0x4083b1f8);
//# 35 "/opt/tinyos-2.x/tos/chips/rf2xx/util/BareSend.nc"
static  error_t LowpanNetworkLayerC$Send$send(message_t *arg_0x40825b10);
//# 38 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioPacket.nc"
static   uint8_t LowpanNetworkLayerC$RadioPacket$payloadLength(message_t *arg_0x407de250);









static   uint8_t LowpanNetworkLayerC$RadioPacket$maxPayloadLength(void);
//#line 43
static   void LowpanNetworkLayerC$RadioPacket$setPayloadLength(message_t *arg_0x407de7a8, uint8_t arg_0x407de930);
//#line 32
static   uint8_t LowpanNetworkLayerC$RadioPacket$headerLength(message_t *arg_0x407c9c68);
//# 67 "/opt/tinyos-2.x/tos/interfaces/Receive.nc"
static  message_t *LowpanNetworkLayerC$NonTinyosReceive$default$receive(
//# 34 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/LowpanNetworkLayerC.nc"
uint8_t arg_0x40881068, 
//# 67 "/opt/tinyos-2.x/tos/interfaces/Receive.nc"
message_t *arg_0x4070c340, void *arg_0x4070c4e0, uint8_t arg_0x4070c668);
//# 43 "/opt/tinyos-2.x/tos/chips/rf2xx/util/BareSend.nc"
static  void IEEE154MessageLayerP$SubSend$sendDone(message_t *arg_0x408221f8, error_t arg_0x40822380);
//# 64 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/IEEE154MessageLayer.nc"
static   bool IEEE154MessageLayerP$IEEE154MessageLayer$isAckFrame(message_t *arg_0x407bae70);
//#line 109
static   uint8_t IEEE154MessageLayerP$IEEE154MessageLayer$getDSN(message_t *arg_0x407cf120);
//#line 58
static   void IEEE154MessageLayerP$IEEE154MessageLayer$createDataFrame(message_t *arg_0x407ba8c0);
//#line 114
static   void IEEE154MessageLayerP$IEEE154MessageLayer$setDSN(message_t *arg_0x407cf690, uint8_t arg_0x407cf818);
//#line 77
static   void IEEE154MessageLayerP$IEEE154MessageLayer$createAckReply(message_t *arg_0x407b99b8, message_t *arg_0x407b9b68);
//#line 145
static   void IEEE154MessageLayerP$IEEE154MessageLayer$setSrcAddr(message_t *arg_0x407cca48, uint16_t arg_0x407ccbd8);
//#line 140
static   uint16_t IEEE154MessageLayerP$IEEE154MessageLayer$getSrcAddr(message_t *arg_0x407cc538);
//#line 52
static   bool IEEE154MessageLayerP$IEEE154MessageLayer$isDataFrame(message_t *arg_0x407ba380);
//#line 83
static   bool IEEE154MessageLayerP$IEEE154MessageLayer$verifyAckReply(message_t *arg_0x407d1150, message_t *arg_0x407d1300);




static   bool IEEE154MessageLayerP$IEEE154MessageLayer$getAckRequired(message_t *arg_0x407d18a8);
//#line 151
static   bool IEEE154MessageLayerP$IEEE154MessageLayer$requiresAckWait(message_t *arg_0x407cb120);
//#line 135
static   void IEEE154MessageLayerP$IEEE154MessageLayer$setDestAddr(message_t *arg_0x407cde40, uint16_t arg_0x407cc010);
//#line 125
static   void IEEE154MessageLayerP$IEEE154MessageLayer$setDestPan(message_t *arg_0x407cd278, uint16_t arg_0x407cd408);
//#line 157
static   bool IEEE154MessageLayerP$IEEE154MessageLayer$requiresAckReply(message_t *arg_0x407cb688);
//#line 130
static   uint16_t IEEE154MessageLayerP$IEEE154MessageLayer$getDestAddr(message_t *arg_0x407cd930);
//# 38 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioPacket.nc"
static   uint8_t IEEE154MessageLayerP$RadioPacket$payloadLength(message_t *arg_0x407de250);









static   uint8_t IEEE154MessageLayerP$RadioPacket$maxPayloadLength(void);
//#line 43
static   void IEEE154MessageLayerP$RadioPacket$setPayloadLength(message_t *arg_0x407de7a8, uint8_t arg_0x407de930);
//#line 32
static   uint8_t IEEE154MessageLayerP$RadioPacket$headerLength(message_t *arg_0x407c9c68);
//# 35 "/opt/tinyos-2.x/tos/chips/rf2xx/util/BareSend.nc"
static  error_t IEEE154MessageLayerP$Send$send(message_t *arg_0x40825b10);
//# 85 "/opt/tinyos-2.x/tos/chips/rf2xx/util/Ieee154Send.nc"
static  void IEEE154MessageLayerP$Ieee154Send$default$sendDone(message_t *arg_0x40898aa8, error_t arg_0x40898c30);
//# 43 "/opt/tinyos-2.x/tos/chips/rf2xx/util/BareSend.nc"
static  void UniqueLayerP$SubSend$sendDone(message_t *arg_0x408221f8, error_t arg_0x40822380);
//# 42 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioReceive.nc"
static   message_t *UniqueLayerP$SubReceive$receive(message_t *arg_0x408eec28);
//#line 35
static   bool UniqueLayerP$SubReceive$header(message_t *arg_0x408ee550);
//# 35 "/opt/tinyos-2.x/tos/chips/rf2xx/util/BareSend.nc"
static  error_t UniqueLayerP$Send$send(message_t *arg_0x40825b10);
//# 69 "/opt/tinyos-2.x/tos/chips/rf2xx/util/Neighborhood.nc"
static   void UniqueLayerP$Neighborhood$evicted(uint8_t arg_0x40902630);
//# 51 "/opt/tinyos-2.x/tos/interfaces/Init.nc"
static  error_t UniqueLayerP$Init$init(void);
//# 35 "/opt/tinyos-2.x/tos/chips/rf2xx/util/NeighborhoodFlag.nc"
static   bool NeighborhoodP$NeighborhoodFlag$get(
//# 32 "/opt/tinyos-2.x/tos/chips/rf2xx/util/NeighborhoodP.nc"
uint8_t arg_0x4090e738, 
//# 35 "/opt/tinyos-2.x/tos/chips/rf2xx/util/NeighborhoodFlag.nc"
uint8_t arg_0x408ff610);




static   void NeighborhoodP$NeighborhoodFlag$set(
//# 32 "/opt/tinyos-2.x/tos/chips/rf2xx/util/NeighborhoodP.nc"
uint8_t arg_0x4090e738, 
//# 40 "/opt/tinyos-2.x/tos/chips/rf2xx/util/NeighborhoodFlag.nc"
uint8_t arg_0x408ffae8);
//# 60 "/opt/tinyos-2.x/tos/chips/rf2xx/util/Neighborhood.nc"
static   uint8_t NeighborhoodP$Neighborhood$insertNode(am_addr_t arg_0x40902010);
//# 51 "/opt/tinyos-2.x/tos/interfaces/Init.nc"
static  error_t NeighborhoodP$Init$init(void);
//# 83 "/opt/tinyos-2.x/tos/interfaces/SplitControl.nc"
static  error_t MessageBufferLayerP$SplitControl$start(void);
//# 51 "/opt/tinyos-2.x/tos/interfaces/Init.nc"
static  error_t MessageBufferLayerP$SoftwareInit$init(void);
//# 58 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioState.nc"
static   void MessageBufferLayerP$RadioState$done(void);
//# 35 "/opt/tinyos-2.x/tos/chips/rf2xx/util/BareSend.nc"
static  error_t MessageBufferLayerP$Send$send(message_t *arg_0x40825b10);
//# 64 "/opt/tinyos-2.x/tos/interfaces/TaskBasic.nc"
static  void MessageBufferLayerP$stateDoneTask$runTask(void);
//# 42 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioReceive.nc"
static   message_t *MessageBufferLayerP$RadioReceive$receive(message_t *arg_0x408eec28);
//#line 35
static   bool MessageBufferLayerP$RadioReceive$header(message_t *arg_0x408ee550);
//# 64 "/opt/tinyos-2.x/tos/interfaces/TaskBasic.nc"
static  void MessageBufferLayerP$sendTask$runTask(void);
//#line 64
static  void MessageBufferLayerP$deliverTask$runTask(void);
//# 52 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioSend.nc"
static   void MessageBufferLayerP$RadioSend$ready(void);
//#line 45
static   void MessageBufferLayerP$RadioSend$sendDone(error_t arg_0x40937850);
//# 37 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioChannel.nc"
static  void MessageBufferLayerP$RadioChannel$default$setChannelDone(void);
//# 37 "/opt/tinyos-2.x/tos/chips/rf2xx/util/Tasklet.nc"
static   void MessageBufferLayerP$Tasklet$run(void);
//# 52 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioSend.nc"
static   void RandomCollisionLayerP$SubSend$ready(void);
//#line 45
static   void RandomCollisionLayerP$SubSend$sendDone(error_t arg_0x40937850);
//# 42 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioReceive.nc"
static   message_t *RandomCollisionLayerP$SubReceive$receive(message_t *arg_0x408eec28);
//#line 35
static   bool RandomCollisionLayerP$SubReceive$header(message_t *arg_0x408ee550);
//# 48 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioAlarm.nc"
static   void RandomCollisionLayerP$RadioAlarm$fired(void);
//# 37 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioSend.nc"
static   error_t RandomCollisionLayerP$RadioSend$send(message_t *arg_0x40937190);
//# 64 "/opt/tinyos-2.x/tos/interfaces/TaskBasic.nc"
static  void RandomCollisionLayerP$calcNextRandom$runTask(void);
//# 41 "/opt/tinyos-2.x/tos/interfaces/Random.nc"
static   uint16_t RandomMlcgC$Random$rand16(void);
//#line 35
static   uint32_t RandomMlcgC$Random$rand32(void);
//# 51 "/opt/tinyos-2.x/tos/interfaces/Init.nc"
static  error_t RandomMlcgC$Init$init(void);
//# 52 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioSend.nc"
static   void SoftwareAckLayerP$SubSend$ready(void);
//#line 45
static   void SoftwareAckLayerP$SubSend$sendDone(error_t arg_0x40937850);
//# 42 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioReceive.nc"
static   message_t *SoftwareAckLayerP$SubReceive$receive(message_t *arg_0x408eec28);
//#line 35
static   bool SoftwareAckLayerP$SubReceive$header(message_t *arg_0x408ee550);
//# 48 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioAlarm.nc"
static   void SoftwareAckLayerP$RadioAlarm$fired(void);
//# 37 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioSend.nc"
static   error_t SoftwareAckLayerP$RadioSend$send(message_t *arg_0x40937190);
//# 38 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioPacket.nc"
static   uint8_t MetadataFlagsLayerC$RadioPacket$payloadLength(message_t *arg_0x407de250);









static   uint8_t MetadataFlagsLayerC$RadioPacket$maxPayloadLength(void);
//#line 43
static   void MetadataFlagsLayerC$RadioPacket$setPayloadLength(message_t *arg_0x407de7a8, uint8_t arg_0x407de930);
//#line 32
static   uint8_t MetadataFlagsLayerC$RadioPacket$headerLength(message_t *arg_0x407c9c68);
//#line 54
static   uint8_t MetadataFlagsLayerC$RadioPacket$metadataLength(message_t *arg_0x407dc1b0);
//# 44 "/opt/tinyos-2.x/tos/chips/rf2xx/util/PacketFlag.nc"
static   void MetadataFlagsLayerC$PacketFlag$clear(
//# 31 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/MetadataFlagsLayerC.nc"
uint8_t arg_0x409c9910, 
//# 44 "/opt/tinyos-2.x/tos/chips/rf2xx/util/PacketFlag.nc"
message_t *arg_0x409bbba0);
//#line 29
static   bool MetadataFlagsLayerC$PacketFlag$get(
//# 31 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/MetadataFlagsLayerC.nc"
uint8_t arg_0x409c9910, 
//# 29 "/opt/tinyos-2.x/tos/chips/rf2xx/util/PacketFlag.nc"
message_t *arg_0x409bca38);




static   void MetadataFlagsLayerC$PacketFlag$setValue(
//# 31 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/MetadataFlagsLayerC.nc"
uint8_t arg_0x409c9910, 
//# 34 "/opt/tinyos-2.x/tos/chips/rf2xx/util/PacketFlag.nc"
message_t *arg_0x409bb010, bool arg_0x409bb198);




static   void MetadataFlagsLayerC$PacketFlag$set(
//# 31 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/MetadataFlagsLayerC.nc"
uint8_t arg_0x409c9910, 
//# 39 "/opt/tinyos-2.x/tos/chips/rf2xx/util/PacketFlag.nc"
message_t *arg_0x409bb6a8);
//# 38 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioPacket.nc"
static   uint8_t TimeStampingLayerP$RadioPacket$payloadLength(message_t *arg_0x407de250);









static   uint8_t TimeStampingLayerP$RadioPacket$maxPayloadLength(void);
//#line 43
static   void TimeStampingLayerP$RadioPacket$setPayloadLength(message_t *arg_0x407de7a8, uint8_t arg_0x407de930);
//#line 32
static   uint8_t TimeStampingLayerP$RadioPacket$headerLength(message_t *arg_0x407c9c68);
//#line 54
static   uint8_t TimeStampingLayerP$RadioPacket$metadataLength(message_t *arg_0x407dc1b0);
//# 59 "/opt/tinyos-2.x/tos/interfaces/PacketTimeStamp.nc"
static   void TimeStampingLayerP$PacketTimeStampRadio$clear(message_t *arg_0x4076f710);







static   void TimeStampingLayerP$PacketTimeStampRadio$set(message_t *arg_0x4076fc38, TimeStampingLayerP$PacketTimeStampRadio$size_type arg_0x4076fdc8);
//# 41 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioCCA.nc"
static   void RF230DriverLayerP$RadioCCA$default$done(error_t arg_0x40953238);
//# 40 "/opt/tinyos-2.x/tos/chips/rf2xx/util/PacketField.nc"
static   void RF230DriverLayerP$PacketRSSI$clear(message_t *arg_0x40788d80);





static   void RF230DriverLayerP$PacketRSSI$set(message_t *arg_0x40787290, RF230DriverLayerP$PacketRSSI$value_type arg_0x40787420);
//# 51 "/opt/tinyos-2.x/tos/interfaces/Init.nc"
static  error_t RF230DriverLayerP$SoftwareInit$init(void);
//# 48 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioAlarm.nc"
static   void RF230DriverLayerP$RadioAlarm$fired(void);
//# 45 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioState.nc"
static   error_t RF230DriverLayerP$RadioState$turnOn(void);
//# 35 "/opt/tinyos-2.x/tos/chips/rf2xx/util/PacketField.nc"
static   RF230DriverLayerP$PacketTransmitPower$value_type RF230DriverLayerP$PacketTransmitPower$get(message_t *arg_0x407887f8);
//#line 29
static   bool RF230DriverLayerP$PacketTransmitPower$isSet(message_t *arg_0x407882e8);
//# 38 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioPacket.nc"
static   uint8_t RF230DriverLayerP$RadioPacket$payloadLength(message_t *arg_0x407de250);









static   uint8_t RF230DriverLayerP$RadioPacket$maxPayloadLength(void);
//#line 43
static   void RF230DriverLayerP$RadioPacket$setPayloadLength(message_t *arg_0x407de7a8, uint8_t arg_0x407de930);
//#line 32
static   uint8_t RF230DriverLayerP$RadioPacket$headerLength(message_t *arg_0x407c9c68);
//#line 54
static   uint8_t RF230DriverLayerP$RadioPacket$metadataLength(message_t *arg_0x407dc1b0);
//# 35 "/opt/tinyos-2.x/tos/chips/rf2xx/util/PacketField.nc"
static   RF230DriverLayerP$PacketTimeSyncOffset$value_type RF230DriverLayerP$PacketTimeSyncOffset$get(message_t *arg_0x407887f8);
//#line 29
static   bool RF230DriverLayerP$PacketTimeSyncOffset$isSet(message_t *arg_0x407882e8);
//# 51 "/opt/tinyos-2.x/tos/interfaces/Init.nc"
static  error_t RF230DriverLayerP$PlatformInit$init(void);
//# 92 "/opt/tinyos-2.x/tos/interfaces/Resource.nc"
static  void RF230DriverLayerP$SpiResource$granted(void);
//# 46 "/opt/tinyos-2.x/tos/chips/rf2xx/util/PacketField.nc"
static   void RF230DriverLayerP$PacketLinkQuality$set(message_t *arg_0x40787290, RF230DriverLayerP$PacketLinkQuality$value_type arg_0x40787420);
//# 37 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioSend.nc"
static   error_t RF230DriverLayerP$RadioSend$send(message_t *arg_0x40937190);
//# 50 "/opt/tinyos-2.x/tos/interfaces/GpioCapture.nc"
static   void RF230DriverLayerP$IRQ$captured(uint16_t arg_0x40a2e8c8);
//# 37 "/opt/tinyos-2.x/tos/chips/rf2xx/util/Tasklet.nc"
static   void RF230DriverLayerP$Tasklet$run(void);
//# 51 "/opt/tinyos-2.x/tos/chips/stm32/timer/HplStm32Capture.nc"
static   void HplRF230P$Capture$captured(HplRF230P$Capture$size_type arg_0x40ab0248);
//# 51 "/opt/tinyos-2.x/tos/interfaces/Init.nc"
static  error_t HplRF230P$PlatformInit$init(void);
//# 55 "/opt/tinyos-2.x/tos/interfaces/GpioCapture.nc"
static   void HplRF230P$IRQ$disable(void);
//#line 42
static   error_t HplRF230P$IRQ$captureRisingEdge(void);
//# 64 "/opt/tinyos-2.x/tos/interfaces/TaskBasic.nc"
static  void Stm32SpiP$zeroTask$runTask(void);
//# 71 "/opt/tinyos-2.x/tos/interfaces/SpiPacket.nc"
static   void Stm32SpiP$SpiPacket$default$sendDone(uint8_t *arg_0x40ac47e8, uint8_t *arg_0x40ac4990, uint16_t arg_0x40ac4b20, 
error_t arg_0x40ac4cb8);
//# 92 "/opt/tinyos-2.x/tos/interfaces/Resource.nc"
static  void Stm32SpiP$ResourceArbiter$granted(
//# 87 "/opt/tinyos-2.x/tos/chips/stm32/spi/Stm32SpiP.nc"
uint8_t arg_0x40af20f8);
//# 46 "/opt/tinyos-2.x/tos/interfaces/FastSpiByte.nc"
static   void Stm32SpiP$FastSpiByte$splitWrite(uint8_t arg_0x40a32250);
//#line 58
static   uint8_t Stm32SpiP$FastSpiByte$splitReadWrite(uint8_t arg_0x40a32b88);
//#line 52
static   uint8_t Stm32SpiP$FastSpiByte$splitRead(void);
//#line 65
static   uint8_t Stm32SpiP$FastSpiByte$write(uint8_t arg_0x40a310b0);
//# 110 "/opt/tinyos-2.x/tos/interfaces/Resource.nc"
static   error_t Stm32SpiP$Resource$release(
//# 83 "/opt/tinyos-2.x/tos/chips/stm32/spi/Stm32SpiP.nc"
uint8_t arg_0x40af5b80);
//# 87 "/opt/tinyos-2.x/tos/interfaces/Resource.nc"
static   error_t Stm32SpiP$Resource$immediateRequest(
//# 83 "/opt/tinyos-2.x/tos/chips/stm32/spi/Stm32SpiP.nc"
uint8_t arg_0x40af5b80);
//# 78 "/opt/tinyos-2.x/tos/interfaces/Resource.nc"
static   error_t Stm32SpiP$Resource$request(
//# 83 "/opt/tinyos-2.x/tos/chips/stm32/spi/Stm32SpiP.nc"
uint8_t arg_0x40af5b80);
//# 92 "/opt/tinyos-2.x/tos/interfaces/Resource.nc"
static  void Stm32SpiP$Resource$default$granted(
//# 83 "/opt/tinyos-2.x/tos/chips/stm32/spi/Stm32SpiP.nc"
uint8_t arg_0x40af5b80);
//# 118 "/opt/tinyos-2.x/tos/interfaces/Resource.nc"
static   uint8_t Stm32SpiP$Resource$isOwner(
//# 83 "/opt/tinyos-2.x/tos/chips/stm32/spi/Stm32SpiP.nc"
uint8_t arg_0x40af5b80);
//# 72 "/opt/tinyos-2.x/tos/chips/stm32/spi/Stm32Spi.nc"
static   void HplStm32SpiP$SPI$sleep(void);
//#line 66
static   void HplStm32SpiP$SPI$initMaster(void);
//#line 105
static   void HplStm32SpiP$SPI$setMasterBit(bool arg_0x40aea8d0);
//#line 96
static   void HplStm32SpiP$SPI$enableInterrupt(bool arg_0x40aeb128);
//#line 80
static   uint8_t HplStm32SpiP$SPI$read(void);
//#line 125
static   void HplStm32SpiP$SPI$setMasterDoubleSpeed(bool arg_0x40ae6358);
//#line 114
static   void HplStm32SpiP$SPI$setClock(uint8_t arg_0x40ae7168);
//#line 108
static   void HplStm32SpiP$SPI$setClockPolarity(bool arg_0x40ae8170);
//#line 86
static   void HplStm32SpiP$SPI$write(uint8_t arg_0x40aec6b8);
//#line 99
static   void HplStm32SpiP$SPI$enableSpi(bool arg_0x40aeb8f8);
//#line 111
static   void HplStm32SpiP$SPI$setClockPhase(bool arg_0x40ae8950);
//# 51 "/opt/tinyos-2.x/tos/interfaces/Init.nc"
static  error_t /*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$Init$init(void);
//# 69 "/opt/tinyos-2.x/tos/interfaces/ResourceQueue.nc"
static   error_t /*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$FcfsQueue$enqueue(resource_client_id_t arg_0x40b6a010);
//#line 43
static   bool /*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$FcfsQueue$isEmpty(void);








static   bool /*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$FcfsQueue$isEnqueued(resource_client_id_t arg_0x40b6b598);







static   resource_client_id_t /*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$FcfsQueue$dequeue(void);
//# 43 "/opt/tinyos-2.x/tos/interfaces/ResourceRequested.nc"
static   void /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$ResourceRequested$default$requested(
//# 52 "/opt/tinyos-2.x/tos/system/SimpleArbiterP.nc"
uint8_t arg_0x40b5f688);
//# 51 "/opt/tinyos-2.x/tos/interfaces/ResourceRequested.nc"
static   void /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$ResourceRequested$default$immediateRequested(
//# 52 "/opt/tinyos-2.x/tos/system/SimpleArbiterP.nc"
uint8_t arg_0x40b5f688);
//# 55 "/opt/tinyos-2.x/tos/interfaces/ResourceConfigure.nc"
static   void /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$ResourceConfigure$default$unconfigure(
//# 56 "/opt/tinyos-2.x/tos/system/SimpleArbiterP.nc"
uint8_t arg_0x40b5e188);
//# 49 "/opt/tinyos-2.x/tos/interfaces/ResourceConfigure.nc"
static   void /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$ResourceConfigure$default$configure(
//# 56 "/opt/tinyos-2.x/tos/system/SimpleArbiterP.nc"
uint8_t arg_0x40b5e188);
//# 110 "/opt/tinyos-2.x/tos/interfaces/Resource.nc"
static   error_t /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$Resource$release(
//# 51 "/opt/tinyos-2.x/tos/system/SimpleArbiterP.nc"
uint8_t arg_0x40b60c90);
//# 87 "/opt/tinyos-2.x/tos/interfaces/Resource.nc"
static   error_t /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$Resource$immediateRequest(
//# 51 "/opt/tinyos-2.x/tos/system/SimpleArbiterP.nc"
uint8_t arg_0x40b60c90);
//# 78 "/opt/tinyos-2.x/tos/interfaces/Resource.nc"
static   error_t /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$Resource$request(
//# 51 "/opt/tinyos-2.x/tos/system/SimpleArbiterP.nc"
uint8_t arg_0x40b60c90);
//# 118 "/opt/tinyos-2.x/tos/interfaces/Resource.nc"
static   uint8_t /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$Resource$isOwner(
//# 51 "/opt/tinyos-2.x/tos/system/SimpleArbiterP.nc"
uint8_t arg_0x40b60c90);
//# 80 "/opt/tinyos-2.x/tos/interfaces/ArbiterInfo.nc"
static   bool /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$ArbiterInfo$inUse(void);
//# 64 "/opt/tinyos-2.x/tos/interfaces/TaskBasic.nc"
static  void /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$grantedTask$runTask(void);
//# 53 "/opt/tinyos-2.x/tos/chips/stm32/timer/HplStm32Compare.nc"
static   void HplStm32Timer3P$CompareA$reset(void);
//#line 45
static   void HplStm32Timer3P$CompareA$set(HplStm32Timer3P$CompareA$size_type arg_0x40b83350);










static   void HplStm32Timer3P$CompareA$start(void);


static   void HplStm32Timer3P$CompareA$stop(void);
//# 79 "/opt/tinyos-2.x/tos/chips/stm32/timer/HplStm32Capture.nc"
static   void HplStm32Timer3P$Capture$setEdge(bool arg_0x40aae830);
//#line 38
static   HplStm32Timer3P$Capture$size_type HplStm32Timer3P$Capture$get(void);
//#line 55
static   void HplStm32Timer3P$Capture$reset(void);


static   void HplStm32Timer3P$Capture$start(void);


static   void HplStm32Timer3P$Capture$stop(void);
//# 49 "/opt/tinyos-2.x/tos/chips/stm32/timer/HplStm32Compare.nc"
static   void HplStm32Timer3P$CompareC$default$fired(void);
//# 78 "/opt/tinyos-2.x/tos/chips/stm32/timer/HplStm32Timer.nc"
static   bool HplStm32Timer3P$Timer$test(void);
//#line 52
static   HplStm32Timer3P$Timer$timer_size HplStm32Timer3P$Timer$get(void);
//# 98 "/opt/tinyos-2.x/tos/lib/timer/Alarm.nc"
static   /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$Alarm$size_type /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$Alarm$getNow(void);
//#line 92
static   void /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$Alarm$startAt(/*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$Alarm$size_type arg_0x40667e78, /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$Alarm$size_type arg_0x40666030);
//#line 55
static   void /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$Alarm$start(/*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$Alarm$size_type arg_0x40653ce0);






static   void /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$Alarm$stop(void);
//# 61 "/opt/tinyos-2.x/tos/chips/stm32/timer/HplStm32Timer.nc"
static   void /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$HplStm32Timer$overflow(void);
//# 49 "/opt/tinyos-2.x/tos/chips/stm32/timer/HplStm32Compare.nc"
static   void /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$HplStm32Compare$fired(void);
//# 53 "/opt/tinyos-2.x/tos/lib/timer/Counter.nc"
static   /*CounterOne16C.NCounter*/Stm32CounterC$0$Counter$size_type /*CounterOne16C.NCounter*/Stm32CounterC$0$Counter$get(void);






static   bool /*CounterOne16C.NCounter*/Stm32CounterC$0$Counter$isOverflowPending(void);
//# 61 "/opt/tinyos-2.x/tos/chips/stm32/timer/HplStm32Timer.nc"
static   void /*CounterOne16C.NCounter*/Stm32CounterC$0$Timer$overflow(void);
//# 71 "/opt/tinyos-2.x/tos/lib/timer/Counter.nc"
static   void /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$CounterFrom$overflow(void);
//#line 53
static   /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$Counter$size_type /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$Counter$get(void);
//# 50 "/opt/tinyos-2.x/tos/lib/timer/LocalTime.nc"
static   uint32_t /*LocalTimeMicroC.CounterToLocalTimeC*/CounterToLocalTimeC$0$LocalTime$get(void);
//# 71 "/opt/tinyos-2.x/tos/lib/timer/Counter.nc"
static   void /*LocalTimeMicroC.CounterToLocalTimeC*/CounterToLocalTimeC$0$Counter$overflow(void);
//# 55 "/opt/tinyos-2.x/tos/lib/timer/BusyWait.nc"
static   void /*BusyWaitMicroC.Stm32BusyWaitMicro*/HalStm32BusyWaitM$0$BusyWait$wait(/*BusyWaitMicroC.Stm32BusyWaitMicro*/HalStm32BusyWaitM$0$BusyWait$size_type arg_0x40a2bf20);
//# 71 "/opt/tinyos-2.x/tos/lib/timer/Counter.nc"
static   void /*BusyWaitMicroC.Stm32BusyWaitMicro*/HalStm32BusyWaitM$0$OST$overflow(void);
//# 72 "/opt/tinyos-2.x/tos/lib/timer/Timer.nc"
static  void STMTestC$Timer0$fired(void);
//# 49 "/opt/tinyos-2.x/tos/interfaces/Boot.nc"
static  void STMTestC$Boot$booted(void);
//# 7 "STMAppMessaging.nc"
static  void STMTestC$STMAppMessaging$Receive(uint8_t arg_0x406e3328, uint8_t *arg_0x406e34d0, uint16_t arg_0x406e3660);
//# 51 "/opt/tinyos-2.x/tos/interfaces/Init.nc"
static  error_t PlatformP$MoteInit$init(void);
//#line 51
static  error_t PlatformP$MoteClockInit$init(void);
//#line 51
static  error_t PlatformP$McuSleepInit$init(void);
//# 46 "/opt/tinyos-2.x/tos/platforms/stm32-p103/PlatformP.nc"
static inline  error_t PlatformP$Init$init(void);
//#line 104
static inline   void PlatformP$Interrupt$fired(void);
//# 37 "/opt/tinyos-2.x/tos/platforms/stm32-p103/MoteClockP.nc"
static inline  error_t MoteClockP$MoteClockInit$init(void);
//# 51 "/opt/tinyos-2.x/tos/chips/stm32/McuSleepC.nc"
static inline  error_t McuSleepC$McuSleepInit$init(void);
//#line 73
static inline   void McuSleepC$McuSleep$sleep(void);
//#line 158
static inline   void McuSleepC$McuPowerState$update(void);
//#line 170
void RTCAlarm_IRQHandler(void)   ;
//# 31 "/opt/tinyos-2.x/tos/chips/stm32/HplSTM32Interrupt.nc"
static   void HplSTM32InterruptM$Irq$fired(void);
//# 50 "/opt/tinyos-2.x/tos/chips/stm32/HplSTM32InterruptM.nc"
void NMIException(void)   ;






void HardFaultException(void)   ;





void MemManageException(void)   ;





void BusFaultException(void)   ;





void UsageFaultException(void)   ;




void __STM32ReservedException7(void)   ;




void __STM32ReservedException8(void)   ;




void __STM32ReservedException9(void)   ;




void __STM32ReservedException10(void)   ;





void SVCHandler(void)   ;





void DebugMonitor(void)   ;




void __STM32ReservedException13(void)   ;






void PendSVC(void)   ;





void SysTickHandler(void)   ;





void WWDG_IRQHandler(void)   ;





void PVD_IRQHandler(void)   ;





void TAMPER_IRQHandler(void)   ;
//#line 158
void FLASH_IRQHandler(void)   ;





void RCC_IRQHandler(void)   ;





void EXTI0_IRQHandler(void)   ;





void EXTI1_IRQHandler(void)   ;





void EXTI2_IRQHandler(void)   ;





void EXTI3_IRQHandler(void)   ;





void EXTI4_IRQHandler(void)   ;





void DMAChannel1_IRQHandler(void)   ;





void DMAChannel2_IRQHandler(void)   ;





void DMAChannel3_IRQHandler(void)   ;





void DMAChannel4_IRQHandler(void)   ;





void DMAChannel5_IRQHandler(void)   ;





void DMAChannel6_IRQHandler(void)   ;





void DMAChannel7_IRQHandler(void)   ;





void ADC_IRQHandler(void)   ;





void USB_HP_CAN_TX_IRQHandler(void)   ;





void USB_LP_CAN_RX0_IRQHandler(void)   ;





void CAN_RX1_IRQHandler(void)   ;





void CAN_SCE_IRQHandler(void)   ;





void EXTI9_5_IRQHandler(void)   ;





void TIM1_BRK_IRQHandler(void)   ;





void TIM1_UP_IRQHandler(void)   ;





void TIM1_TRG_COM_IRQHandler(void)   ;





void TIM1_CC_IRQHandler(void)   ;





void TIM2_IRQHandler(void)   ;





void TIM3_IRQHandler(void)   ;





void TIM4_IRQHandler(void)   ;





void I2C1_EV_IRQHandler(void)   ;





void I2C1_ER_IRQHandler(void)   ;





void I2C2_EV_IRQHandler(void)   ;





void I2C2_ER_IRQHandler(void)   ;





void SPI1_IRQHandler(void)   ;





void SPI2_IRQHandler(void)   ;





void USART1_IRQHandler(void)   ;





void USART2_IRQHandler(void)   ;





void USART3_IRQHandler(void)   ;





void EXTI15_10_IRQHandler(void)   ;
//#line 389
void USBWakeUp_IRQHandler(void)   ;
//# 51 "/opt/tinyos-2.x/tos/interfaces/Init.nc"
static  error_t RealMainP$SoftwareInit$init(void);
//# 49 "/opt/tinyos-2.x/tos/interfaces/Boot.nc"
static  void RealMainP$Boot$booted(void);
//# 51 "/opt/tinyos-2.x/tos/interfaces/Init.nc"
static  error_t RealMainP$PlatformInit$init(void);
//# 46 "/opt/tinyos-2.x/tos/interfaces/Scheduler.nc"
static  void RealMainP$Scheduler$init(void);
//#line 61
static  void RealMainP$Scheduler$taskLoop(void);
//#line 54
static  bool RealMainP$Scheduler$runNextTask(void);
//# 52 "/opt/tinyos-2.x/tos/system/RealMainP.nc"
int main(void)   ;
//# 64 "/opt/tinyos-2.x/tos/interfaces/TaskBasic.nc"
static  void SchedulerBasicP$TaskBasic$runTask(
//# 45 "/opt/tinyos-2.x/tos/system/SchedulerBasicP.nc"
uint8_t arg_0x4044e188);
//# 59 "/opt/tinyos-2.x/tos/interfaces/McuSleep.nc"
static   void SchedulerBasicP$McuSleep$sleep(void);
//# 50 "/opt/tinyos-2.x/tos/system/SchedulerBasicP.nc"
enum SchedulerBasicP$__nesc_unnamed4326 {

  SchedulerBasicP$NUM_TASKS = 10U, 
  SchedulerBasicP$NO_TASK = 255
};

uint8_t SchedulerBasicP$m_head;
uint8_t SchedulerBasicP$m_tail;
uint8_t SchedulerBasicP$m_next[SchedulerBasicP$NUM_TASKS];








static __inline uint8_t SchedulerBasicP$popTask(void);
//#line 86
static inline bool SchedulerBasicP$isWaiting(uint8_t id);




static inline bool SchedulerBasicP$pushTask(uint8_t id);
//#line 113
static inline  void SchedulerBasicP$Scheduler$init(void);









static  bool SchedulerBasicP$Scheduler$runNextTask(void);
//#line 138
static inline  void SchedulerBasicP$Scheduler$taskLoop(void);
//#line 159
static   error_t SchedulerBasicP$TaskBasic$postTask(uint8_t id);




static   void SchedulerBasicP$TaskBasic$default$runTask(uint8_t id);
//# 31 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
static   void LedsP$Led0$toggle(void);



static   void LedsP$Led0$makeOutput(void);
//#line 29
static   void LedsP$Led0$set(void);

static   void LedsP$Led1$toggle(void);



static   void LedsP$Led1$makeOutput(void);
//#line 29
static   void LedsP$Led1$set(void);

static   void LedsP$Led2$toggle(void);



static   void LedsP$Led2$makeOutput(void);
//#line 29
static   void LedsP$Led2$set(void);
//# 45 "/opt/tinyos-2.x/tos/system/LedsP.nc"
static inline  error_t LedsP$Init$init(void);
//#line 73
static inline   void LedsP$Leds$led0Toggle(void);
//#line 88
static inline   void LedsP$Leds$led1Toggle(void);
//#line 103
static inline   void LedsP$Leds$led2Toggle(void);
//# 53 "/opt/tinyos-2.x/tos/chips/stm32/gpio/HplSTM32GeneralIOPinP.nc"
static __inline   void /*HplSTM32GeneralIOC.PortA.Bit0*/HplSTM32GeneralIOPinP$0$IO$clr(void);
//#line 67
static __inline   void /*HplSTM32GeneralIOC.PortA.Bit0*/HplSTM32GeneralIOPinP$0$IO$makeInput(void);
//#line 82
static __inline   void /*HplSTM32GeneralIOC.PortA.Bit5*/HplSTM32GeneralIOPinP$5$IO$makeOutput(void);
//#line 47
static __inline   void /*HplSTM32GeneralIOC.PortA.Bit6*/HplSTM32GeneralIOPinP$6$IO$set(void);





static __inline   void /*HplSTM32GeneralIOC.PortA.Bit6*/HplSTM32GeneralIOPinP$6$IO$clr(void);
//#line 67
static __inline   void /*HplSTM32GeneralIOC.PortA.Bit6*/HplSTM32GeneralIOPinP$6$IO$makeInput(void);
//#line 82
static __inline   void /*HplSTM32GeneralIOC.PortA.Bit6*/HplSTM32GeneralIOPinP$6$IO$makeOutput(void);
//#line 82
static __inline   void /*HplSTM32GeneralIOC.PortA.Bit7*/HplSTM32GeneralIOPinP$7$IO$makeOutput(void);
//#line 47
static __inline   void /*HplSTM32GeneralIOC.PortB.Bit0*/HplSTM32GeneralIOPinP$16$IO$set(void);





static __inline   void /*HplSTM32GeneralIOC.PortB.Bit0*/HplSTM32GeneralIOPinP$16$IO$clr(void);
//#line 82
static __inline   void /*HplSTM32GeneralIOC.PortB.Bit0*/HplSTM32GeneralIOPinP$16$IO$makeOutput(void);
//#line 47
static __inline   void /*HplSTM32GeneralIOC.PortB.Bit7*/HplSTM32GeneralIOPinP$23$IO$set(void);





static __inline   void /*HplSTM32GeneralIOC.PortB.Bit7*/HplSTM32GeneralIOPinP$23$IO$clr(void);
//#line 82
static __inline   void /*HplSTM32GeneralIOC.PortB.Bit7*/HplSTM32GeneralIOPinP$23$IO$makeOutput(void);
//#line 47
static __inline   void /*HplSTM32GeneralIOC.PortC.Bit8*/HplSTM32GeneralIOPinP$40$IO$set(void);










static inline   void /*HplSTM32GeneralIOC.PortC.Bit8*/HplSTM32GeneralIOPinP$40$IO$toggle(void);
//#line 82
static __inline   void /*HplSTM32GeneralIOC.PortC.Bit8*/HplSTM32GeneralIOPinP$40$IO$makeOutput(void);
//#line 47
static __inline   void /*HplSTM32GeneralIOC.PortC.Bit9*/HplSTM32GeneralIOPinP$41$IO$set(void);










static inline   void /*HplSTM32GeneralIOC.PortC.Bit9*/HplSTM32GeneralIOPinP$41$IO$toggle(void);
//#line 82
static __inline   void /*HplSTM32GeneralIOC.PortC.Bit9*/HplSTM32GeneralIOPinP$41$IO$makeOutput(void);
//#line 47
static __inline   void /*HplSTM32GeneralIOC.PortC.Bit12*/HplSTM32GeneralIOPinP$44$IO$set(void);










static inline   void /*HplSTM32GeneralIOC.PortC.Bit12*/HplSTM32GeneralIOPinP$44$IO$toggle(void);
//#line 82
static __inline   void /*HplSTM32GeneralIOC.PortC.Bit12*/HplSTM32GeneralIOPinP$44$IO$makeOutput(void);
//# 56 "/opt/tinyos-2.x/tos/interfaces/TaskBasic.nc"
static   error_t /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$updateFromTimer$postTask(void);
//# 125 "/opt/tinyos-2.x/tos/lib/timer/Timer.nc"
static  uint32_t /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$TimerFrom$getNow(void);
//#line 118
static  void /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$TimerFrom$startOneShotAt(uint32_t arg_0x4060f9b8, uint32_t arg_0x4060fb48);
//#line 67
static  void /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$TimerFrom$stop(void);




static  void /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$Timer$fired(
//# 37 "/opt/tinyos-2.x/tos/lib/timer/VirtualizeTimerC.nc"
uint8_t arg_0x406329a0);
//#line 60
enum /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$__nesc_unnamed4327 {
//#line 60
  VirtualizeTimerC$0$updateFromTimer = 0U
};
//#line 60
typedef int /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$__nesc_sillytask_updateFromTimer[/*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$updateFromTimer];
//#line 42
enum /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$__nesc_unnamed4328 {

  VirtualizeTimerC$0$NUM_TIMERS = 1U, 
  VirtualizeTimerC$0$END_OF_LIST = 255
};








//#line 48
typedef struct /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$__nesc_unnamed4329 {

  uint32_t t0;
  uint32_t dt;
  bool isoneshot : 1;
  bool isrunning : 1;
  bool _reserved : 6;
} /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$Timer_t;

/*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$Timer_t /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$m_timers[/*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$NUM_TIMERS];




static void /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$fireTimers(uint32_t now);
//#line 89
static inline  void /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$updateFromTimer$runTask(void);
//#line 128
static inline  void /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$TimerFrom$fired(void);
//#line 193
static inline   void /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$Timer$default$fired(uint8_t num);
//# 56 "/opt/tinyos-2.x/tos/interfaces/TaskBasic.nc"
static   error_t /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$fired$postTask(void);
//# 98 "/opt/tinyos-2.x/tos/lib/timer/Alarm.nc"
static   /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Alarm$size_type /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Alarm$getNow(void);
//#line 92
static   void /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Alarm$startAt(/*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Alarm$size_type arg_0x40667e78, /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Alarm$size_type arg_0x40666030);
//#line 105
static   /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Alarm$size_type /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Alarm$getAlarm(void);
//#line 62
static   void /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Alarm$stop(void);
//# 72 "/opt/tinyos-2.x/tos/lib/timer/Timer.nc"
static  void /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Timer$fired(void);
//# 63 "/opt/tinyos-2.x/tos/lib/timer/AlarmToTimerC.nc"
enum /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$__nesc_unnamed4330 {
//#line 63
  AlarmToTimerC$0$fired = 1U
};
//#line 63
typedef int /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$__nesc_sillytask_fired[/*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$fired];
//#line 44
uint32_t /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$m_dt;
bool /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$m_oneshot;

static inline void /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$start(uint32_t t0, uint32_t dt, bool oneshot);
//#line 60
static inline  void /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Timer$stop(void);


static inline  void /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$fired$runTask(void);






static inline   void /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Alarm$fired(void);
//#line 82
static inline  void /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Timer$startOneShotAt(uint32_t t0, uint32_t dt);


static inline  uint32_t /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Timer$getNow(void);
//# 67 "/opt/tinyos-2.x/tos/lib/timer/Alarm.nc"
static   void STM32RtcC$Alarm$fired(void);
//# 71 "/opt/tinyos-2.x/tos/lib/timer/Counter.nc"
static   void STM32RtcC$Counter$overflow(void);
//# 45 "/opt/tinyos-2.x/tos/chips/stm32/timer/STM32RtcC.nc"
 uint32_t STM32RtcC$alarm;
bool STM32RtcC$running;

static inline void STM32RtcC$enableInterrupt(void);









static void STM32RtcC$disableInterrupt(void);
//#line 72
static inline  error_t STM32RtcC$Init$init(void);
//#line 178
static inline   void STM32RtcC$Alarm$stop(void);









static   void STM32RtcC$Alarm$startAt(uint32_t t0, uint32_t dt);
//#line 221
static inline   uint32_t STM32RtcC$Alarm$getNow(void);






static inline   uint32_t STM32RtcC$Alarm$getAlarm(void);




static inline   uint32_t STM32RtcC$Counter$get(void);
//#line 261
void RTC_IRQHandler(void)   ;
//# 83 "/opt/tinyos-2.x/tos/interfaces/SplitControl.nc"
static  error_t STMAppMessagingP$AMControl$start(void);
//# 69 "/opt/tinyos-2.x/tos/interfaces/AMSend.nc"
static  error_t STMAppMessagingP$AMSend$send(am_addr_t arg_0x406fce00, message_t *arg_0x40711010, uint8_t arg_0x40711198);
//# 115 "/opt/tinyos-2.x/tos/interfaces/Packet.nc"
static  void *STMAppMessagingP$Packet$getPayload(message_t *arg_0x40709ef0, uint8_t arg_0x40708088);
//#line 95
static  uint8_t STMAppMessagingP$Packet$maxPayloadLength(void);
//# 72 "/opt/tinyos-2.x/tos/interfaces/Leds.nc"
static   void STMAppMessagingP$Leds$led1Toggle(void);
//#line 89
static   void STMAppMessagingP$Leds$led2Toggle(void);
//# 7 "STMAppMessaging.nc"
static  void STMAppMessagingP$STMAppMessaging$Receive(uint8_t arg_0x406e3328, uint8_t *arg_0x406e34d0, uint16_t arg_0x406e3660);
//# 17 "STMAppMessagingP.nc"
message_t STMAppMessagingP$packet;
uint8_t STMAppMessagingP$radioStarted = 0;
uint8_t STMAppMessagingP$rcvdMsg[100];

static inline  void STMAppMessagingP$STMAppMessaging$Init(void);







static inline  void STMAppMessagingP$AMControl$startDone(error_t err);









static inline  void STMAppMessagingP$AMControl$stopDone(error_t err);


static inline  error_t STMAppMessagingP$STMAppMessaging$Send(uint8_t appId, uint16_t destId, uint8_t *dataBuf, uint16_t len);
//#line 68
static inline  void STMAppMessagingP$AMSend$sendDone(message_t *bufPtr, error_t error);



static inline  message_t *STMAppMessagingP$Receive$receive(message_t *bufPtr, void *payload, uint8_t len);
//# 99 "/opt/tinyos-2.x/tos/interfaces/AMSend.nc"
static  void /*STMAppMessagingC.AMSenderC.AMQueueEntryP*/AMQueueEntryP$0$AMSend$sendDone(message_t *arg_0x407101c0, error_t arg_0x40710348);
//# 64 "/opt/tinyos-2.x/tos/interfaces/Send.nc"
static  error_t /*STMAppMessagingC.AMSenderC.AMQueueEntryP*/AMQueueEntryP$0$Send$send(message_t *arg_0x40729cb0, uint8_t arg_0x40729e38);
//# 92 "/opt/tinyos-2.x/tos/interfaces/AMPacket.nc"
static  void /*STMAppMessagingC.AMSenderC.AMQueueEntryP*/AMQueueEntryP$0$AMPacket$setDestination(message_t *arg_0x40721768, am_addr_t arg_0x407218f8);
//#line 151
static  void /*STMAppMessagingC.AMSenderC.AMQueueEntryP*/AMQueueEntryP$0$AMPacket$setType(message_t *arg_0x4071e668, am_id_t arg_0x4071e7f0);
//# 45 "/opt/tinyos-2.x/tos/system/AMQueueEntryP.nc"
static inline  error_t /*STMAppMessagingC.AMSenderC.AMQueueEntryP*/AMQueueEntryP$0$AMSend$send(am_addr_t dest, 
message_t *msg, 
uint8_t len);









static inline  void /*STMAppMessagingC.AMSenderC.AMQueueEntryP*/AMQueueEntryP$0$Send$sendDone(message_t *m, error_t err);
//# 69 "/opt/tinyos-2.x/tos/interfaces/AMSend.nc"
static  error_t /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$AMSend$send(
//# 40 "/opt/tinyos-2.x/tos/system/AMQueueImplP.nc"
am_id_t arg_0x40746be8, 
//# 69 "/opt/tinyos-2.x/tos/interfaces/AMSend.nc"
am_addr_t arg_0x406fce00, message_t *arg_0x40711010, uint8_t arg_0x40711198);
//# 89 "/opt/tinyos-2.x/tos/interfaces/Send.nc"
static  void /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$Send$sendDone(
//# 38 "/opt/tinyos-2.x/tos/system/AMQueueImplP.nc"
uint8_t arg_0x40746288, 
//# 89 "/opt/tinyos-2.x/tos/interfaces/Send.nc"
message_t *arg_0x40728e18, error_t arg_0x4073d010);
//# 67 "/opt/tinyos-2.x/tos/interfaces/Packet.nc"
static  uint8_t /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$Packet$payloadLength(message_t *arg_0x4070aaf0);
//#line 83
static  void /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$Packet$setPayloadLength(message_t *arg_0x40709190, uint8_t arg_0x40709318);
//# 56 "/opt/tinyos-2.x/tos/interfaces/TaskBasic.nc"
static   error_t /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$errorTask$postTask(void);
//# 67 "/opt/tinyos-2.x/tos/interfaces/AMPacket.nc"
static  am_addr_t /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$AMPacket$destination(message_t *arg_0x40722b50);
//#line 136
static  am_id_t /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$AMPacket$type(message_t *arg_0x4071e0d0);
//# 118 "/opt/tinyos-2.x/tos/system/AMQueueImplP.nc"
enum /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$__nesc_unnamed4331 {
//#line 118
  AMQueueImplP$0$CancelTask = 2U
};
//#line 118
typedef int /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$__nesc_sillytask_CancelTask[/*AMQueueP.AMQueueImplP*/AMQueueImplP$0$CancelTask];
//#line 161
enum /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$__nesc_unnamed4332 {
//#line 161
  AMQueueImplP$0$errorTask = 3U
};
//#line 161
typedef int /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$__nesc_sillytask_errorTask[/*AMQueueP.AMQueueImplP*/AMQueueImplP$0$errorTask];
//#line 49
//#line 47
typedef struct /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$__nesc_unnamed4333 {
  message_t *msg;
} /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$queue_entry_t;

uint8_t /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$current = 1;
/*AMQueueP.AMQueueImplP*/AMQueueImplP$0$queue_entry_t /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$queue[1];
uint8_t /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$cancelMask[1 / 8 + 1];

static void /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$tryToSend(void);

static inline void /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$nextPacket(void);
//#line 82
static inline  error_t /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$Send$send(uint8_t clientId, message_t *msg, 
uint8_t len);
//#line 118
static inline  void /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$CancelTask$runTask(void);
//#line 155
static inline void /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$sendDone(uint8_t last, message_t *msg, error_t err);





static inline  void /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$errorTask$runTask(void);




static void /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$tryToSend(void);
//#line 181
static inline  void /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$AMSend$sendDone(am_id_t id, message_t *msg, error_t err);
//#line 207
static inline   void /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$Send$default$sendDone(uint8_t id, message_t *msg, error_t err);
//# 64 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/IEEE154MessageLayer.nc"
static   bool RF230ActiveMessageP$IEEE154MessageLayer$isAckFrame(message_t *arg_0x407bae70);
//#line 109
static   uint8_t RF230ActiveMessageP$IEEE154MessageLayer$getDSN(message_t *arg_0x407cf120);




static   void RF230ActiveMessageP$IEEE154MessageLayer$setDSN(message_t *arg_0x407cf690, uint8_t arg_0x407cf818);
//#line 77
static   void RF230ActiveMessageP$IEEE154MessageLayer$createAckReply(message_t *arg_0x407b99b8, message_t *arg_0x407b9b68);
//#line 145
static   void RF230ActiveMessageP$IEEE154MessageLayer$setSrcAddr(message_t *arg_0x407cca48, uint16_t arg_0x407ccbd8);
//#line 140
static   uint16_t RF230ActiveMessageP$IEEE154MessageLayer$getSrcAddr(message_t *arg_0x407cc538);
//#line 52
static   bool RF230ActiveMessageP$IEEE154MessageLayer$isDataFrame(message_t *arg_0x407ba380);
//#line 83
static   bool RF230ActiveMessageP$IEEE154MessageLayer$verifyAckReply(message_t *arg_0x407d1150, message_t *arg_0x407d1300);
//#line 151
static   bool RF230ActiveMessageP$IEEE154MessageLayer$requiresAckWait(message_t *arg_0x407cb120);
//#line 135
static   void RF230ActiveMessageP$IEEE154MessageLayer$setDestAddr(message_t *arg_0x407cde40, uint16_t arg_0x407cc010);
//#line 125
static   void RF230ActiveMessageP$IEEE154MessageLayer$setDestPan(message_t *arg_0x407cd278, uint16_t arg_0x407cd408);
//#line 157
static   bool RF230ActiveMessageP$IEEE154MessageLayer$requiresAckReply(message_t *arg_0x407cb688);
//#line 130
static   uint16_t RF230ActiveMessageP$IEEE154MessageLayer$getDestAddr(message_t *arg_0x407cd930);
//# 53 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioAlarm.nc"
static   uint16_t RF230ActiveMessageP$RadioAlarm$getNow(void);
//# 63 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230ActiveMessageP.nc"
static inline   uint8_t RF230ActiveMessageP$RF230DriverConfig$headerLength(message_t *msg);




static inline   uint8_t RF230ActiveMessageP$RF230DriverConfig$maxPayloadLength(void);




static inline   uint8_t RF230ActiveMessageP$RF230DriverConfig$metadataLength(message_t *msg);




static inline   uint8_t RF230ActiveMessageP$RF230DriverConfig$headerPreloadLength(void);





static inline   bool RF230ActiveMessageP$RF230DriverConfig$requiresRssiCca(message_t *msg);






static inline   bool RF230ActiveMessageP$SoftwareAckConfig$requiresAckWait(message_t *msg);




static inline   bool RF230ActiveMessageP$SoftwareAckConfig$isAckPacket(message_t *msg);




static inline   bool RF230ActiveMessageP$SoftwareAckConfig$verifyAckPacket(message_t *data, message_t *ack);









static inline   bool RF230ActiveMessageP$SoftwareAckConfig$requiresAckReply(message_t *msg);




static inline   void RF230ActiveMessageP$SoftwareAckConfig$createAckPacket(message_t *data, message_t *ack);








static inline   uint16_t RF230ActiveMessageP$SoftwareAckConfig$getAckTimeout(void);




static inline   void RF230ActiveMessageP$SoftwareAckConfig$reportChannelError(void);








static inline   uint8_t RF230ActiveMessageP$UniqueConfig$getSequenceNumber(message_t *msg);




static inline   void RF230ActiveMessageP$UniqueConfig$setSequenceNumber(message_t *msg, uint8_t dsn);




static inline   am_addr_t RF230ActiveMessageP$UniqueConfig$getSender(message_t *msg);




static inline   void RF230ActiveMessageP$UniqueConfig$reportChannelError(void);








static inline  am_addr_t RF230ActiveMessageP$ActiveMessageConfig$destination(message_t *msg);




static inline  void RF230ActiveMessageP$ActiveMessageConfig$setDestination(message_t *msg, am_addr_t addr);









static inline  void RF230ActiveMessageP$ActiveMessageConfig$setSource(message_t *msg, am_addr_t addr);









static inline  void RF230ActiveMessageP$ActiveMessageConfig$setGroup(message_t *msg, am_group_t grp);
//#line 202
enum RF230ActiveMessageP$__nesc_unnamed4334 {

  RF230ActiveMessageP$TRAFFIC_UPDATE_PERIOD = 100, 
  RF230ActiveMessageP$TRAFFIC_MAX_BYTES = (uint16_t )(RF230ActiveMessageP$TRAFFIC_UPDATE_PERIOD * 1000UL / 32)
};
//#line 242
static inline   uint16_t RF230ActiveMessageP$RandomCollisionConfig$getMinimumBackoff(void);




static inline   uint16_t RF230ActiveMessageP$RandomCollisionConfig$getInitialBackoff(message_t *msg);




static inline   uint16_t RF230ActiveMessageP$RandomCollisionConfig$getCongestionBackoff(message_t *msg);




static inline   uint16_t RF230ActiveMessageP$RandomCollisionConfig$getTransmitBarrier(message_t *msg);
//#line 273
static inline   void RF230ActiveMessageP$RadioAlarm$fired(void);
//# 48 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioAlarm.nc"
static   void RadioAlarmP$RadioAlarm$fired(
//# 31 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioAlarmP.nc"
uint8_t arg_0x4080dc60);
//# 98 "/opt/tinyos-2.x/tos/lib/timer/Alarm.nc"
static   RadioAlarmP$Alarm$size_type RadioAlarmP$Alarm$getNow(void);
//#line 55
static   void RadioAlarmP$Alarm$start(RadioAlarmP$Alarm$size_type arg_0x40653ce0);






static   void RadioAlarmP$Alarm$stop(void);
//# 48 "/opt/tinyos-2.x/tos/chips/rf2xx/util/Tasklet.nc"
static   void RadioAlarmP$Tasklet$schedule(void);
//# 43 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioAlarmP.nc"
 uint8_t RadioAlarmP$state;
enum RadioAlarmP$__nesc_unnamed4335 {

  RadioAlarmP$STATE_READY = 0, 
  RadioAlarmP$STATE_WAIT = 1, 
  RadioAlarmP$STATE_FIRED = 2
};

 uint8_t RadioAlarmP$alarm;

static inline   void RadioAlarmP$Alarm$fired(void);










static __inline   uint16_t RadioAlarmP$RadioAlarm$getNow(uint8_t id);




static inline   void RadioAlarmP$Tasklet$run(void);








static inline    void RadioAlarmP$RadioAlarm$default$fired(uint8_t id);



static __inline   bool RadioAlarmP$RadioAlarm$isFree(uint8_t id);




static inline   void RadioAlarmP$RadioAlarm$wait(uint8_t id, uint16_t timeout);








static inline   void RadioAlarmP$RadioAlarm$cancel(uint8_t id);
//# 37 "/opt/tinyos-2.x/tos/chips/rf2xx/util/Tasklet.nc"
static   void TaskletC$Tasklet$run(void);
//# 61 "/opt/tinyos-2.x/tos/chips/rf2xx/util/TaskletC.nc"
uint8_t TaskletC$state;

static void TaskletC$doit(void);
//#line 83
static __inline  void TaskletC$Tasklet$suspend(void);




static  void TaskletC$Tasklet$resume(void);
//#line 101
static   void TaskletC$Tasklet$schedule(void);
//# 35 "/opt/tinyos-2.x/tos/chips/rf2xx/util/BareSend.nc"
static  error_t ActiveMessageLayerP$SubSend$send(message_t *arg_0x40825b10);
//# 59 "/opt/tinyos-2.x/tos/interfaces/SendNotifier.nc"
static  void ActiveMessageLayerP$SendNotifier$aboutToSend(
//# 36 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/ActiveMessageLayerP.nc"
am_id_t arg_0x408357e0, 
//# 59 "/opt/tinyos-2.x/tos/interfaces/SendNotifier.nc"
am_addr_t arg_0x4078f478, message_t *arg_0x4078f628);
//# 99 "/opt/tinyos-2.x/tos/interfaces/AMSend.nc"
static  void ActiveMessageLayerP$AMSend$sendDone(
//# 33 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/ActiveMessageLayerP.nc"
am_id_t arg_0x408372f8, 
//# 99 "/opt/tinyos-2.x/tos/interfaces/AMSend.nc"
message_t *arg_0x407101c0, error_t arg_0x40710348);
//# 67 "/opt/tinyos-2.x/tos/interfaces/Receive.nc"
static  message_t *ActiveMessageLayerP$Snoop$receive(
//# 35 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/ActiveMessageLayerP.nc"
am_id_t arg_0x40835228, 
//# 67 "/opt/tinyos-2.x/tos/interfaces/Receive.nc"
message_t *arg_0x4070c340, void *arg_0x4070c4e0, uint8_t arg_0x4070c668);
//# 38 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioPacket.nc"
static   uint8_t ActiveMessageLayerP$SubPacket$payloadLength(message_t *arg_0x407de250);









static   uint8_t ActiveMessageLayerP$SubPacket$maxPayloadLength(void);
//#line 43
static   void ActiveMessageLayerP$SubPacket$setPayloadLength(message_t *arg_0x407de7a8, uint8_t arg_0x407de930);
//#line 32
static   uint8_t ActiveMessageLayerP$SubPacket$headerLength(message_t *arg_0x407c9c68);
//# 67 "/opt/tinyos-2.x/tos/interfaces/Receive.nc"
static  message_t *ActiveMessageLayerP$Receive$receive(
//# 34 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/ActiveMessageLayerP.nc"
am_id_t arg_0x40837c28, 
//# 67 "/opt/tinyos-2.x/tos/interfaces/Receive.nc"
message_t *arg_0x4070c340, void *arg_0x4070c4e0, uint8_t arg_0x4070c668);
//# 29 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/ActiveMessageConfig.nc"
static  am_addr_t ActiveMessageLayerP$Config$destination(message_t *arg_0x407c2eb8);








static  void ActiveMessageLayerP$Config$setSource(message_t *arg_0x407c1ee0, am_addr_t arg_0x407c0088);
//#line 32
static  void ActiveMessageLayerP$Config$setDestination(message_t *arg_0x407c1398, am_addr_t arg_0x407c1528);
//#line 44
static  void ActiveMessageLayerP$Config$setGroup(message_t *arg_0x407c0a38, am_group_t arg_0x407c0bc8);
//# 50 "/opt/tinyos-2.x/tos/interfaces/ActiveMessageAddress.nc"
static   am_addr_t ActiveMessageLayerP$ActiveMessageAddress$amAddress(void);




static   am_group_t ActiveMessageLayerP$ActiveMessageAddress$amGroup(void);
//# 51 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/ActiveMessageLayerP.nc"
static inline activemessage_header_t *ActiveMessageLayerP$getHeader(message_t *msg);




static inline void *ActiveMessageLayerP$getPayload(message_t *msg);






static  error_t ActiveMessageLayerP$AMSend$send(am_id_t id, am_addr_t addr, message_t *msg, uint8_t len);
//#line 79
static __inline  void ActiveMessageLayerP$SubSend$sendDone(message_t *msg, error_t error);
//#line 103
static inline   void ActiveMessageLayerP$SendNotifier$default$aboutToSend(am_id_t id, am_addr_t addr, message_t *msg);





static inline  message_t *ActiveMessageLayerP$SubReceive$receive(message_t *msg);
//#line 122
static inline   message_t *ActiveMessageLayerP$Receive$default$receive(am_id_t id, message_t *msg, void *payload, uint8_t len);




static inline   message_t *ActiveMessageLayerP$Snoop$default$receive(am_id_t id, message_t *msg, void *payload, uint8_t len);






static __inline  am_addr_t ActiveMessageLayerP$AMPacket$address(void);




static __inline  am_group_t ActiveMessageLayerP$AMPacket$localGroup(void);




static __inline  bool ActiveMessageLayerP$AMPacket$isForMe(message_t *msg);





static __inline  am_addr_t ActiveMessageLayerP$AMPacket$destination(message_t *msg);




static __inline  void ActiveMessageLayerP$AMPacket$setDestination(message_t *msg, am_addr_t addr);









static __inline  void ActiveMessageLayerP$AMPacket$setSource(message_t *msg, am_addr_t addr);




static __inline  am_id_t ActiveMessageLayerP$AMPacket$type(message_t *msg);




static __inline  void ActiveMessageLayerP$AMPacket$setType(message_t *msg, am_id_t type);









static __inline  void ActiveMessageLayerP$AMPacket$setGroup(message_t *msg, am_group_t grp);










static   uint8_t ActiveMessageLayerP$RadioPacket$headerLength(message_t *msg);




static inline   uint8_t ActiveMessageLayerP$RadioPacket$payloadLength(message_t *msg);




static inline   void ActiveMessageLayerP$RadioPacket$setPayloadLength(message_t *msg, uint8_t length);




static inline   uint8_t ActiveMessageLayerP$RadioPacket$maxPayloadLength(void);
//#line 233
static inline  uint8_t ActiveMessageLayerP$Packet$payloadLength(message_t *msg);




static inline  void ActiveMessageLayerP$Packet$setPayloadLength(message_t *msg, uint8_t len);




static inline  uint8_t ActiveMessageLayerP$Packet$maxPayloadLength(void);




static  void *ActiveMessageLayerP$Packet$getPayload(message_t *msg, uint8_t len);
//# 51 "/opt/tinyos-2.x/tos/system/ActiveMessageAddressC.nc"
am_addr_t ActiveMessageAddressC$addr = TOS_AM_ADDRESS;


am_group_t ActiveMessageAddressC$group = TOS_AM_GROUP;






static inline   am_addr_t ActiveMessageAddressC$ActiveMessageAddress$amAddress(void);
//#line 82
static inline   am_group_t ActiveMessageAddressC$ActiveMessageAddress$amGroup(void);
//#line 95
static   am_addr_t ActiveMessageAddressC$amAddress(void);
//# 35 "/opt/tinyos-2.x/tos/chips/rf2xx/util/BareSend.nc"
static  error_t LowpanNetworkLayerC$SubSend$send(message_t *arg_0x40825b10);







static  void LowpanNetworkLayerC$Send$sendDone(message_t *arg_0x408221f8, error_t arg_0x40822380);
//# 67 "/opt/tinyos-2.x/tos/interfaces/Receive.nc"
static  message_t *LowpanNetworkLayerC$NonTinyosReceive$receive(
//# 34 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/LowpanNetworkLayerC.nc"
uint8_t arg_0x40881068, 
//# 67 "/opt/tinyos-2.x/tos/interfaces/Receive.nc"
message_t *arg_0x4070c340, void *arg_0x4070c4e0, uint8_t arg_0x4070c668);
//# 38 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioPacket.nc"
static   uint8_t LowpanNetworkLayerC$SubPacket$payloadLength(message_t *arg_0x407de250);









static   uint8_t LowpanNetworkLayerC$SubPacket$maxPayloadLength(void);
//#line 43
static   void LowpanNetworkLayerC$SubPacket$setPayloadLength(message_t *arg_0x407de7a8, uint8_t arg_0x407de930);
//#line 32
static   uint8_t LowpanNetworkLayerC$SubPacket$headerLength(message_t *arg_0x407c9c68);
//# 31 "/opt/tinyos-2.x/tos/chips/rf2xx/util/BareReceive.nc"
static  message_t *LowpanNetworkLayerC$Receive$receive(message_t *arg_0x4083b1f8);
//# 47 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/LowpanNetworkLayerC.nc"
static inline lowpan_header_t *LowpanNetworkLayerC$getHeader(message_t *msg);




static inline void *LowpanNetworkLayerC$getPayload(message_t *msg);








static inline  error_t LowpanNetworkLayerC$Send$send(message_t *msg);










static inline  void LowpanNetworkLayerC$SubSend$sendDone(message_t *msg, error_t error);




static inline  message_t *LowpanNetworkLayerC$SubReceive$receive(message_t *msg);









static inline   message_t *LowpanNetworkLayerC$NonTinyosReceive$default$receive(uint8_t network, message_t *msg, void *payload, uint8_t len);






static inline   uint8_t LowpanNetworkLayerC$RadioPacket$headerLength(message_t *msg);




static inline   uint8_t LowpanNetworkLayerC$RadioPacket$payloadLength(message_t *msg);




static inline   void LowpanNetworkLayerC$RadioPacket$setPayloadLength(message_t *msg, uint8_t length);




static inline   uint8_t LowpanNetworkLayerC$RadioPacket$maxPayloadLength(void);
//# 35 "/opt/tinyos-2.x/tos/chips/rf2xx/util/BareSend.nc"
static  error_t IEEE154MessageLayerP$SubSend$send(message_t *arg_0x40825b10);







static  void IEEE154MessageLayerP$Send$sendDone(message_t *arg_0x408221f8, error_t arg_0x40822380);
//# 50 "/opt/tinyos-2.x/tos/interfaces/ActiveMessageAddress.nc"
static   am_addr_t IEEE154MessageLayerP$ActiveMessageAddress$amAddress(void);
//# 38 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioPacket.nc"
static   uint8_t IEEE154MessageLayerP$SubPacket$payloadLength(message_t *arg_0x407de250);









static   uint8_t IEEE154MessageLayerP$SubPacket$maxPayloadLength(void);
//#line 43
static   void IEEE154MessageLayerP$SubPacket$setPayloadLength(message_t *arg_0x407de7a8, uint8_t arg_0x407de930);
//#line 32
static   uint8_t IEEE154MessageLayerP$SubPacket$headerLength(message_t *arg_0x407c9c68);
//# 85 "/opt/tinyos-2.x/tos/chips/rf2xx/util/Ieee154Send.nc"
static  void IEEE154MessageLayerP$Ieee154Send$sendDone(message_t *arg_0x40898aa8, error_t arg_0x40898c30);
//# 53 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/IEEE154MessageLayerP.nc"
enum IEEE154MessageLayerP$__nesc_unnamed4336 {

  IEEE154MessageLayerP$IEEE154_DATA_FRAME_MASK = (((IEEE154_TYPE_MASK << IEEE154_FCF_FRAME_TYPE)
   | (1 << IEEE154_FCF_INTRAPAN))
   | (IEEE154_ADDR_MASK << IEEE154_FCF_DEST_ADDR_MODE))
   | (IEEE154_ADDR_MASK << IEEE154_FCF_SRC_ADDR_MODE), 

  IEEE154MessageLayerP$IEEE154_DATA_FRAME_VALUE = (((IEEE154_TYPE_DATA << IEEE154_FCF_FRAME_TYPE)
   | (1 << IEEE154_FCF_INTRAPAN))
   | (IEEE154_ADDR_SHORT << IEEE154_FCF_DEST_ADDR_MODE))
   | (IEEE154_ADDR_SHORT << IEEE154_FCF_SRC_ADDR_MODE), 

  IEEE154MessageLayerP$IEEE154_ACK_FRAME_LENGTH = 3, 
  IEEE154MessageLayerP$IEEE154_ACK_FRAME_MASK = IEEE154_TYPE_MASK << IEEE154_FCF_FRAME_TYPE, 
  IEEE154MessageLayerP$IEEE154_ACK_FRAME_VALUE = IEEE154_TYPE_ACK << IEEE154_FCF_FRAME_TYPE
};

static inline ieee154_header_t *IEEE154MessageLayerP$getHeader(message_t *msg);
//#line 90
static   bool IEEE154MessageLayerP$IEEE154MessageLayer$isDataFrame(message_t *msg);




static inline   void IEEE154MessageLayerP$IEEE154MessageLayer$createDataFrame(message_t *msg);




static   bool IEEE154MessageLayerP$IEEE154MessageLayer$isAckFrame(message_t *msg);










static inline   void IEEE154MessageLayerP$IEEE154MessageLayer$createAckReply(message_t *data, message_t *ack);








static inline   bool IEEE154MessageLayerP$IEEE154MessageLayer$verifyAckReply(message_t *data, message_t *ack);







static inline   bool IEEE154MessageLayerP$IEEE154MessageLayer$getAckRequired(message_t *msg);
//#line 154
static inline   uint8_t IEEE154MessageLayerP$IEEE154MessageLayer$getDSN(message_t *msg);




static inline   void IEEE154MessageLayerP$IEEE154MessageLayer$setDSN(message_t *msg, uint8_t dsn);









static inline   void IEEE154MessageLayerP$IEEE154MessageLayer$setDestPan(message_t *msg, uint16_t pan);




static   uint16_t IEEE154MessageLayerP$IEEE154MessageLayer$getDestAddr(message_t *msg);




static inline   void IEEE154MessageLayerP$IEEE154MessageLayer$setDestAddr(message_t *msg, uint16_t addr);




static inline   uint16_t IEEE154MessageLayerP$IEEE154MessageLayer$getSrcAddr(message_t *msg);




static inline   void IEEE154MessageLayerP$IEEE154MessageLayer$setSrcAddr(message_t *msg, uint16_t addr);




static inline   bool IEEE154MessageLayerP$IEEE154MessageLayer$requiresAckWait(message_t *msg);






static   bool IEEE154MessageLayerP$IEEE154MessageLayer$requiresAckReply(message_t *msg);
//#line 262
static   uint8_t IEEE154MessageLayerP$RadioPacket$headerLength(message_t *msg);




static inline   uint8_t IEEE154MessageLayerP$RadioPacket$payloadLength(message_t *msg);




static   void IEEE154MessageLayerP$RadioPacket$setPayloadLength(message_t *msg, uint8_t length);




static inline   uint8_t IEEE154MessageLayerP$RadioPacket$maxPayloadLength(void);
//#line 359
static inline   void IEEE154MessageLayerP$Ieee154Send$default$sendDone(message_t *msg, error_t error);









static inline  error_t IEEE154MessageLayerP$Send$send(message_t *msg);
//#line 383
static inline  void IEEE154MessageLayerP$SubSend$sendDone(message_t *msg, error_t error);
//# 35 "/opt/tinyos-2.x/tos/chips/rf2xx/util/BareSend.nc"
static  error_t UniqueLayerP$SubSend$send(message_t *arg_0x40825b10);
//# 35 "/opt/tinyos-2.x/tos/chips/rf2xx/util/NeighborhoodFlag.nc"
static   bool UniqueLayerP$NeighborhoodFlag$get(uint8_t arg_0x408ff610);




static   void UniqueLayerP$NeighborhoodFlag$set(uint8_t arg_0x408ffae8);
//# 43 "/opt/tinyos-2.x/tos/chips/rf2xx/util/BareSend.nc"
static  void UniqueLayerP$Send$sendDone(message_t *arg_0x408221f8, error_t arg_0x40822380);
//# 60 "/opt/tinyos-2.x/tos/chips/rf2xx/util/Neighborhood.nc"
static   uint8_t UniqueLayerP$Neighborhood$insertNode(am_addr_t arg_0x40902010);
//# 42 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioReceive.nc"
static   message_t *UniqueLayerP$RadioReceive$receive(message_t *arg_0x408eec28);
//#line 35
static   bool UniqueLayerP$RadioReceive$header(message_t *arg_0x408ee550);
//# 41 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/UniqueConfig.nc"
static   void UniqueLayerP$UniqueConfig$setSequenceNumber(message_t *arg_0x407b8690, uint8_t arg_0x407b8818);





static   void UniqueLayerP$UniqueConfig$reportChannelError(void);
//#line 31
static   uint8_t UniqueLayerP$UniqueConfig$getSequenceNumber(message_t *arg_0x407a0c70);




static   am_addr_t UniqueLayerP$UniqueConfig$getSender(message_t *arg_0x407b8190);
//# 50 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/UniqueLayerP.nc"
uint8_t UniqueLayerP$sequenceNumber;

static inline  error_t UniqueLayerP$Init$init(void);





static inline  error_t UniqueLayerP$Send$send(message_t *msg);










static inline  void UniqueLayerP$SubSend$sendDone(message_t *msg, error_t error);




static inline   bool UniqueLayerP$SubReceive$header(message_t *msg);





 uint8_t UniqueLayerP$receivedNumbers[5];

static inline   message_t *UniqueLayerP$SubReceive$receive(message_t *msg);
//#line 105
static inline   void UniqueLayerP$Neighborhood$evicted(uint8_t index);
//# 69 "/opt/tinyos-2.x/tos/chips/rf2xx/util/Neighborhood.nc"
static   void NeighborhoodP$Neighborhood$evicted(uint8_t arg_0x40902630);
//# 38 "/opt/tinyos-2.x/tos/chips/rf2xx/util/NeighborhoodP.nc"
 am_addr_t NeighborhoodP$nodes[5];
 uint8_t NeighborhoodP$ages[5];
 uint8_t NeighborhoodP$flags[5];
 uint8_t NeighborhoodP$time;
 uint8_t NeighborhoodP$last;

static inline  error_t NeighborhoodP$Init$init(void);
//#line 83
static inline  uint8_t  NeighborhoodP$Neighborhood$insertNode(am_addr_t node);
//#line 147
static __inline   bool NeighborhoodP$NeighborhoodFlag$get(uint8_t bit, uint8_t index);




static __inline   void NeighborhoodP$NeighborhoodFlag$set(uint8_t bit, uint8_t index);
//# 92 "/opt/tinyos-2.x/tos/interfaces/SplitControl.nc"
static  void MessageBufferLayerP$SplitControl$startDone(error_t arg_0x406deb10);
//#line 117
static  void MessageBufferLayerP$SplitControl$stopDone(error_t arg_0x406dd6e8);
//# 45 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioState.nc"
static   error_t MessageBufferLayerP$RadioState$turnOn(void);
//# 43 "/opt/tinyos-2.x/tos/chips/rf2xx/util/BareSend.nc"
static  void MessageBufferLayerP$Send$sendDone(message_t *arg_0x408221f8, error_t arg_0x40822380);
//# 56 "/opt/tinyos-2.x/tos/interfaces/TaskBasic.nc"
static   error_t MessageBufferLayerP$stateDoneTask$postTask(void);
//#line 56
static   error_t MessageBufferLayerP$sendTask$postTask(void);
//# 31 "/opt/tinyos-2.x/tos/chips/rf2xx/util/BareReceive.nc"
static  message_t *MessageBufferLayerP$Receive$receive(message_t *arg_0x4083b1f8);
//# 56 "/opt/tinyos-2.x/tos/interfaces/TaskBasic.nc"
static   error_t MessageBufferLayerP$deliverTask$postTask(void);
//# 37 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioSend.nc"
static   error_t MessageBufferLayerP$RadioSend$send(message_t *arg_0x40937190);
//# 37 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioChannel.nc"
static  void MessageBufferLayerP$RadioChannel$setChannelDone(void);
//# 61 "/opt/tinyos-2.x/tos/chips/rf2xx/util/Tasklet.nc"
static  void MessageBufferLayerP$Tasklet$suspend(void);






static  void MessageBufferLayerP$Tasklet$resume(void);
//# 125 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/MessageBufferLayerP.nc"
enum MessageBufferLayerP$__nesc_unnamed4337 {
//#line 125
  MessageBufferLayerP$stateDoneTask = 4U
};
//#line 125
typedef int MessageBufferLayerP$__nesc_sillytask_stateDoneTask[MessageBufferLayerP$stateDoneTask];
//#line 170
enum MessageBufferLayerP$__nesc_unnamed4338 {
//#line 170
  MessageBufferLayerP$sendTask = 5U
};
//#line 170
typedef int MessageBufferLayerP$__nesc_sillytask_sendTask[MessageBufferLayerP$sendTask];
//#line 280
enum MessageBufferLayerP$__nesc_unnamed4339 {
//#line 280
  MessageBufferLayerP$deliverTask = 6U
};
//#line 280
typedef int MessageBufferLayerP$__nesc_sillytask_deliverTask[MessageBufferLayerP$deliverTask];
//#line 51
 uint8_t MessageBufferLayerP$state;
enum MessageBufferLayerP$__nesc_unnamed4340 {

  MessageBufferLayerP$STATE_READY = 0, 
  MessageBufferLayerP$STATE_TX_PENDING = 1, 
  MessageBufferLayerP$STATE_TX_SEND = 2, 
  MessageBufferLayerP$STATE_TX_DONE = 3, 
  MessageBufferLayerP$STATE_TURN_ON = 4, 
  MessageBufferLayerP$STATE_TURN_OFF = 5, 
  MessageBufferLayerP$STATE_CHANNEL = 6
};

static  error_t MessageBufferLayerP$SplitControl$start(void);
//#line 125
static inline  void MessageBufferLayerP$stateDoneTask$runTask(void);
//#line 144
static inline   void MessageBufferLayerP$RadioState$done(void);
//#line 157
static inline   void MessageBufferLayerP$RadioChannel$default$setChannelDone(void);





message_t *MessageBufferLayerP$txMsg;
error_t MessageBufferLayerP$txError;
uint8_t MessageBufferLayerP$retries;


enum MessageBufferLayerP$__nesc_unnamed4341 {
//#line 168
  MessageBufferLayerP$MAX_RETRIES = 5
};
static inline  void MessageBufferLayerP$sendTask$runTask(void);
//#line 201
static   void MessageBufferLayerP$RadioSend$sendDone(error_t error);







static inline  error_t MessageBufferLayerP$Send$send(message_t *msg);
//#line 222
static inline   void MessageBufferLayerP$RadioSend$ready(void);





static inline   void MessageBufferLayerP$Tasklet$run(void);
//#line 249
enum MessageBufferLayerP$__nesc_unnamed4342 {

  MessageBufferLayerP$RECEIVE_QUEUE_SIZE = 3
};

message_t MessageBufferLayerP$receiveQueueData[MessageBufferLayerP$RECEIVE_QUEUE_SIZE];
message_t *MessageBufferLayerP$receiveQueue[MessageBufferLayerP$RECEIVE_QUEUE_SIZE];

uint8_t MessageBufferLayerP$receiveQueueHead;
uint8_t MessageBufferLayerP$receiveQueueSize;

static inline  error_t MessageBufferLayerP$SoftwareInit$init(void);









static inline   bool MessageBufferLayerP$RadioReceive$header(message_t *msg);









static inline  void MessageBufferLayerP$deliverTask$runTask(void);
//#line 309
static inline   message_t *MessageBufferLayerP$RadioReceive$receive(message_t *msg);
//# 37 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioSend.nc"
static   error_t RandomCollisionLayerP$SubSend$send(message_t *arg_0x40937190);
//# 38 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioAlarm.nc"
static   void RandomCollisionLayerP$RadioAlarm$wait(uint16_t arg_0x407c7958);
//#line 33
static   bool RandomCollisionLayerP$RadioAlarm$isFree(void);
//#line 53
static   uint16_t RandomCollisionLayerP$RadioAlarm$getNow(void);
//# 41 "/opt/tinyos-2.x/tos/interfaces/Random.nc"
static   uint16_t RandomCollisionLayerP$Random$rand16(void);
//# 35 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/RandomCollisionConfig.nc"
static   uint16_t RandomCollisionLayerP$Config$getCongestionBackoff(message_t *arg_0x407afb60);
//#line 29
static   uint16_t RandomCollisionLayerP$Config$getInitialBackoff(message_t *arg_0x407af630);










static   uint16_t RandomCollisionLayerP$Config$getMinimumBackoff(void);





static   uint16_t RandomCollisionLayerP$Config$getTransmitBarrier(message_t *arg_0x407ae450);
//# 42 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioReceive.nc"
static   message_t *RandomCollisionLayerP$RadioReceive$receive(message_t *arg_0x408eec28);
//#line 35
static   bool RandomCollisionLayerP$RadioReceive$header(message_t *arg_0x408ee550);
//# 52 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioSend.nc"
static   void RandomCollisionLayerP$RadioSend$ready(void);
//#line 45
static   void RandomCollisionLayerP$RadioSend$sendDone(error_t arg_0x40937850);
//# 56 "/opt/tinyos-2.x/tos/interfaces/TaskBasic.nc"
static   error_t RandomCollisionLayerP$calcNextRandom$postTask(void);
//# 67 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/RandomCollisionLayerP.nc"
enum RandomCollisionLayerP$__nesc_unnamed4343 {
//#line 67
  RandomCollisionLayerP$calcNextRandom = 7U
};
//#line 67
typedef int RandomCollisionLayerP$__nesc_sillytask_calcNextRandom[RandomCollisionLayerP$calcNextRandom];
//#line 46
 uint8_t RandomCollisionLayerP$state;
enum RandomCollisionLayerP$__nesc_unnamed4344 {

  RandomCollisionLayerP$STATE_READY = 0, 
  RandomCollisionLayerP$STATE_TX_PENDING_FIRST = 1, 
  RandomCollisionLayerP$STATE_TX_PENDING_SECOND = 2, 
  RandomCollisionLayerP$STATE_TX_SENDING = 3, 

  RandomCollisionLayerP$STATE_BARRIER = 0x80
};

 message_t *RandomCollisionLayerP$txMsg;
 uint16_t RandomCollisionLayerP$txBarrier;

static inline   void RandomCollisionLayerP$SubSend$ready(void);





uint16_t RandomCollisionLayerP$nextRandom;
static inline  void RandomCollisionLayerP$calcNextRandom$runTask(void);





static uint16_t RandomCollisionLayerP$getBackoff(uint16_t maxBackoff);
//#line 87
static inline   error_t RandomCollisionLayerP$RadioSend$send(message_t *msg);
//#line 99
static inline   void RandomCollisionLayerP$RadioAlarm$fired(void);
//#line 144
static inline   void RandomCollisionLayerP$SubSend$sendDone(error_t error);







static inline   bool RandomCollisionLayerP$SubReceive$header(message_t *msg);




static inline   message_t *RandomCollisionLayerP$SubReceive$receive(message_t *msg);
//# 41 "/opt/tinyos-2.x/tos/system/RandomMlcgC.nc"
uint32_t RandomMlcgC$seed;


static inline  error_t RandomMlcgC$Init$init(void);
//#line 58
static   uint32_t RandomMlcgC$Random$rand32(void);
//#line 78
static inline   uint16_t RandomMlcgC$Random$rand16(void);
//# 37 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioSend.nc"
static   error_t SoftwareAckLayerP$SubSend$send(message_t *arg_0x40937190);
//# 38 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioAlarm.nc"
static   void SoftwareAckLayerP$RadioAlarm$wait(uint16_t arg_0x407c7958);




static   void SoftwareAckLayerP$RadioAlarm$cancel(void);
//#line 33
static   bool SoftwareAckLayerP$RadioAlarm$isFree(void);
//# 44 "/opt/tinyos-2.x/tos/chips/rf2xx/util/PacketFlag.nc"
static   void SoftwareAckLayerP$AckReceivedFlag$clear(message_t *arg_0x409bbba0);
//#line 34
static   void SoftwareAckLayerP$AckReceivedFlag$setValue(message_t *arg_0x409bb010, bool arg_0x409bb198);
//# 42 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioReceive.nc"
static   message_t *SoftwareAckLayerP$RadioReceive$receive(message_t *arg_0x408eec28);
//#line 35
static   bool SoftwareAckLayerP$RadioReceive$header(message_t *arg_0x408ee550);
//# 75 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/SoftwareAckConfig.nc"
static   void SoftwareAckLayerP$SoftwareAckConfig$reportChannelError(void);
//#line 69
static   void SoftwareAckLayerP$SoftwareAckConfig$createAckPacket(message_t *arg_0x407a2678, message_t *arg_0x407a2828);
//#line 44
static   bool SoftwareAckLayerP$SoftwareAckConfig$requiresAckWait(message_t *arg_0x407a5df8);






static   bool SoftwareAckLayerP$SoftwareAckConfig$isAckPacket(message_t *arg_0x407a4380);






static   bool SoftwareAckLayerP$SoftwareAckConfig$verifyAckPacket(message_t *arg_0x407a4970, message_t *arg_0x407a4b20);
//#line 32
static   uint16_t SoftwareAckLayerP$SoftwareAckConfig$getAckTimeout(void);
//#line 64
static   bool SoftwareAckLayerP$SoftwareAckConfig$requiresAckReply(message_t *arg_0x407a2140);
//# 52 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioSend.nc"
static   void SoftwareAckLayerP$RadioSend$ready(void);
//#line 45
static   void SoftwareAckLayerP$RadioSend$sendDone(error_t arg_0x40937850);
//# 49 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/SoftwareAckLayerP.nc"
 uint8_t SoftwareAckLayerP$state;
enum SoftwareAckLayerP$__nesc_unnamed4345 {

  SoftwareAckLayerP$STATE_READY = 0, 
  SoftwareAckLayerP$STATE_DATA_SEND = 1, 
  SoftwareAckLayerP$STATE_ACK_WAIT = 2, 
  SoftwareAckLayerP$STATE_ACK_SEND = 3
};

 message_t *SoftwareAckLayerP$txMsg;
 message_t SoftwareAckLayerP$ackMsg;

static inline   void SoftwareAckLayerP$SubSend$ready(void);





static inline   error_t SoftwareAckLayerP$RadioSend$send(message_t *msg);
//#line 86
static inline   void SoftwareAckLayerP$SubSend$sendDone(error_t error);
//#line 113
static inline   void SoftwareAckLayerP$RadioAlarm$fired(void);









static inline   bool SoftwareAckLayerP$SubReceive$header(message_t *msg);







static inline   message_t *SoftwareAckLayerP$SubReceive$receive(message_t *msg);
//# 38 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioPacket.nc"
static   uint8_t MetadataFlagsLayerC$SubPacket$payloadLength(message_t *arg_0x407de250);









static   uint8_t MetadataFlagsLayerC$SubPacket$maxPayloadLength(void);
//#line 43
static   void MetadataFlagsLayerC$SubPacket$setPayloadLength(message_t *arg_0x407de7a8, uint8_t arg_0x407de930);
//#line 32
static   uint8_t MetadataFlagsLayerC$SubPacket$headerLength(message_t *arg_0x407c9c68);
//#line 54
static   uint8_t MetadataFlagsLayerC$SubPacket$metadataLength(message_t *arg_0x407dc1b0);
//# 43 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/MetadataFlagsLayerC.nc"
static inline flags_metadata_t *MetadataFlagsLayerC$getMeta(message_t *msg);






static   bool MetadataFlagsLayerC$PacketFlag$get(uint8_t bit, message_t *msg);




static   void MetadataFlagsLayerC$PacketFlag$set(uint8_t bit, message_t *msg);






static inline   void MetadataFlagsLayerC$PacketFlag$clear(uint8_t bit, message_t *msg);






static inline   void MetadataFlagsLayerC$PacketFlag$setValue(uint8_t bit, message_t *msg, bool value);









static inline   uint8_t MetadataFlagsLayerC$RadioPacket$headerLength(message_t *msg);




static inline   uint8_t MetadataFlagsLayerC$RadioPacket$payloadLength(message_t *msg);




static inline   void MetadataFlagsLayerC$RadioPacket$setPayloadLength(message_t *msg, uint8_t length);




static inline   uint8_t MetadataFlagsLayerC$RadioPacket$maxPayloadLength(void);




static inline   uint8_t MetadataFlagsLayerC$RadioPacket$metadataLength(message_t *msg);
//# 44 "/opt/tinyos-2.x/tos/chips/rf2xx/util/PacketFlag.nc"
static   void TimeStampingLayerP$TimeStampFlag$clear(message_t *arg_0x409bbba0);
//#line 39
static   void TimeStampingLayerP$TimeStampFlag$set(message_t *arg_0x409bb6a8);
//# 38 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioPacket.nc"
static   uint8_t TimeStampingLayerP$SubPacket$payloadLength(message_t *arg_0x407de250);









static   uint8_t TimeStampingLayerP$SubPacket$maxPayloadLength(void);
//#line 43
static   void TimeStampingLayerP$SubPacket$setPayloadLength(message_t *arg_0x407de7a8, uint8_t arg_0x407de930);
//#line 32
static   uint8_t TimeStampingLayerP$SubPacket$headerLength(message_t *arg_0x407c9c68);
//#line 54
static   uint8_t TimeStampingLayerP$SubPacket$metadataLength(message_t *arg_0x407dc1b0);
//# 49 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/TimeStampingLayerP.nc"
static inline timestamp_metadata_t *TimeStampingLayerP$getMeta(message_t *msg);
//#line 66
static inline   void TimeStampingLayerP$PacketTimeStampRadio$clear(message_t *msg);




static inline   void TimeStampingLayerP$PacketTimeStampRadio$set(message_t *msg, uint32_t value);
//#line 105
static inline   uint8_t TimeStampingLayerP$RadioPacket$headerLength(message_t *msg);




static inline   uint8_t TimeStampingLayerP$RadioPacket$payloadLength(message_t *msg);




static inline   void TimeStampingLayerP$RadioPacket$setPayloadLength(message_t *msg, uint8_t length);




static inline   uint8_t TimeStampingLayerP$RadioPacket$maxPayloadLength(void);




static inline   uint8_t TimeStampingLayerP$RadioPacket$metadataLength(message_t *msg);
//# 41 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioCCA.nc"
static   void RF230DriverLayerP$RadioCCA$done(error_t arg_0x40953238);
//# 55 "/opt/tinyos-2.x/tos/lib/timer/BusyWait.nc"
static   void RF230DriverLayerP$BusyWait$wait(RF230DriverLayerP$BusyWait$size_type arg_0x40a2bf20);
//# 44 "/opt/tinyos-2.x/tos/chips/rf2xx/util/PacketFlag.nc"
static   void RF230DriverLayerP$RSSIFlag$clear(message_t *arg_0x409bbba0);
//#line 39
static   void RF230DriverLayerP$RSSIFlag$set(message_t *arg_0x409bb6a8);
//# 59 "/opt/tinyos-2.x/tos/interfaces/PacketTimeStamp.nc"
static   void RF230DriverLayerP$PacketTimeStamp$clear(message_t *arg_0x4076f710);







static   void RF230DriverLayerP$PacketTimeStamp$set(message_t *arg_0x4076fc38, RF230DriverLayerP$PacketTimeStamp$size_type arg_0x4076fdc8);
//# 35 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
static   void RF230DriverLayerP$RSTN$makeOutput(void);
//#line 29
static   void RF230DriverLayerP$RSTN$set(void);
static   void RF230DriverLayerP$RSTN$clr(void);
//# 38 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioAlarm.nc"
static   void RF230DriverLayerP$RadioAlarm$wait(uint16_t arg_0x407c7958);
//#line 33
static   bool RF230DriverLayerP$RadioAlarm$isFree(void);
//#line 53
static   uint16_t RF230DriverLayerP$RadioAlarm$getNow(void);
//# 58 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioState.nc"
static   void RF230DriverLayerP$RadioState$done(void);
//# 50 "/opt/tinyos-2.x/tos/lib/timer/LocalTime.nc"
static   uint32_t RF230DriverLayerP$LocalTime$get(void);
//# 35 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
static   void RF230DriverLayerP$SLP_TR$makeOutput(void);
//#line 29
static   void RF230DriverLayerP$SLP_TR$set(void);
static   void RF230DriverLayerP$SLP_TR$clr(void);
//# 35 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230DriverConfig.nc"
static   uint8_t RF230DriverLayerP$Config$maxPayloadLength(void);
//#line 29
static   uint8_t RF230DriverLayerP$Config$headerLength(message_t *arg_0x407a8228);
//#line 41
static   uint8_t RF230DriverLayerP$Config$metadataLength(message_t *arg_0x407a8ae0);






static   uint8_t RF230DriverLayerP$Config$headerPreloadLength(void);





static   bool RF230DriverLayerP$Config$requiresRssiCca(message_t *arg_0x407a74d8);
//# 42 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioReceive.nc"
static   message_t *RF230DriverLayerP$RadioReceive$receive(message_t *arg_0x408eec28);
//#line 35
static   bool RF230DriverLayerP$RadioReceive$header(message_t *arg_0x408ee550);
//# 110 "/opt/tinyos-2.x/tos/interfaces/Resource.nc"
static   error_t RF230DriverLayerP$SpiResource$release(void);
//#line 87
static   error_t RF230DriverLayerP$SpiResource$immediateRequest(void);
//#line 78
static   error_t RF230DriverLayerP$SpiResource$request(void);
//#line 118
static   bool RF230DriverLayerP$SpiResource$isOwner(void);
//# 35 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
static   void RF230DriverLayerP$SELN$makeOutput(void);
//#line 29
static   void RF230DriverLayerP$SELN$set(void);
static   void RF230DriverLayerP$SELN$clr(void);
//# 52 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioSend.nc"
static   void RF230DriverLayerP$RadioSend$ready(void);
//#line 45
static   void RF230DriverLayerP$RadioSend$sendDone(error_t arg_0x40937850);
//# 55 "/opt/tinyos-2.x/tos/interfaces/GpioCapture.nc"
static   void RF230DriverLayerP$IRQ$disable(void);
//#line 42
static   error_t RF230DriverLayerP$IRQ$captureRisingEdge(void);
//# 46 "/opt/tinyos-2.x/tos/interfaces/FastSpiByte.nc"
static   void RF230DriverLayerP$FastSpiByte$splitWrite(uint8_t arg_0x40a32250);
//#line 58
static   uint8_t RF230DriverLayerP$FastSpiByte$splitReadWrite(uint8_t arg_0x40a32b88);
//#line 52
static   uint8_t RF230DriverLayerP$FastSpiByte$splitRead(void);
//#line 65
static   uint8_t RF230DriverLayerP$FastSpiByte$write(uint8_t arg_0x40a310b0);
//# 29 "/opt/tinyos-2.x/tos/chips/rf2xx/util/PacketFlag.nc"
static   bool RF230DriverLayerP$TimeSyncFlag$get(message_t *arg_0x409bca38);
//#line 44
static   void RF230DriverLayerP$TransmitPowerFlag$clear(message_t *arg_0x409bbba0);
//#line 29
static   bool RF230DriverLayerP$TransmitPowerFlag$get(message_t *arg_0x409bca38);
//# 48 "/opt/tinyos-2.x/tos/chips/rf2xx/util/Tasklet.nc"
static   void RF230DriverLayerP$Tasklet$schedule(void);
//# 83 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230DriverLayerP.nc"
static inline rf230_header_t *RF230DriverLayerP$getHeader(message_t *msg);




static void *RF230DriverLayerP$getPayload(message_t *msg);




static rf230_metadata_t *RF230DriverLayerP$getMeta(message_t *msg);






 uint8_t RF230DriverLayerP$state;
enum RF230DriverLayerP$__nesc_unnamed4346 {

  RF230DriverLayerP$STATE_P_ON = 0, 
  RF230DriverLayerP$STATE_SLEEP = 1, 
  RF230DriverLayerP$STATE_SLEEP_2_TRX_OFF = 2, 
  RF230DriverLayerP$STATE_TRX_OFF = 3, 
  RF230DriverLayerP$STATE_TRX_OFF_2_RX_ON = 4, 
  RF230DriverLayerP$STATE_RX_ON = 5, 
  RF230DriverLayerP$STATE_BUSY_TX_2_RX_ON = 6, 
  RF230DriverLayerP$STATE_PLL_ON_2_RX_ON = 7
};

 uint8_t RF230DriverLayerP$cmd;
enum RF230DriverLayerP$__nesc_unnamed4347 {

  RF230DriverLayerP$CMD_NONE = 0, 
  RF230DriverLayerP$CMD_TURNOFF = 1, 
  RF230DriverLayerP$CMD_STANDBY = 2, 
  RF230DriverLayerP$CMD_TURNON = 3, 
  RF230DriverLayerP$CMD_TRANSMIT = 4, 
  RF230DriverLayerP$CMD_RECEIVE = 5, 
  RF230DriverLayerP$CMD_CCA = 6, 
  RF230DriverLayerP$CMD_CHANNEL = 7, 
  RF230DriverLayerP$CMD_SIGNAL_DONE = 8, 
  RF230DriverLayerP$CMD_DOWNLOAD = 9
};

 bool RF230DriverLayerP$radioIrq;

 uint8_t RF230DriverLayerP$txPower;
 uint8_t RF230DriverLayerP$channel;

 message_t *RF230DriverLayerP$rxMsg;
message_t RF230DriverLayerP$rxMsgBuffer;

uint16_t RF230DriverLayerP$capturedTime;

 uint8_t RF230DriverLayerP$rssiClear;
 uint8_t RF230DriverLayerP$rssiBusy;



static __inline void RF230DriverLayerP$writeRegister(uint8_t reg, uint8_t value);
//#line 155
static __inline uint8_t RF230DriverLayerP$readRegister(uint8_t reg);
//#line 171
enum RF230DriverLayerP$__nesc_unnamed4348 {

  RF230DriverLayerP$SLEEP_WAKEUP_TIME = (uint16_t )(880 * (7372800UL / 1 / 32) * (1 << 2) / 1000000UL), 
  RF230DriverLayerP$CCA_REQUEST_TIME = (uint16_t )(140 * (7372800UL / 1 / 32) * (1 << 2) / 1000000UL), 

  RF230DriverLayerP$TX_SFD_DELAY = (uint16_t )(176 * (7372800UL / 1 / 32) * (1 << 2) / 1000000UL), 
  RF230DriverLayerP$RX_SFD_DELAY = (uint16_t )(8 * (7372800UL / 1 / 32) * (1 << 2) / 1000000UL)
};

static inline   void RF230DriverLayerP$RadioAlarm$fired(void);
//#line 206
static inline  error_t RF230DriverLayerP$PlatformInit$init(void);
//#line 224
static inline  error_t RF230DriverLayerP$SoftwareInit$init(void);





static inline void RF230DriverLayerP$initRadio(void);
//#line 258
static inline  void RF230DriverLayerP$SpiResource$granted(void);
//#line 272
static bool RF230DriverLayerP$isSpiAcquired(void);
//#line 312
static __inline void RF230DriverLayerP$changeChannel(void);
//#line 330
static __inline void RF230DriverLayerP$changeState(void);
//#line 400
static inline   error_t RF230DriverLayerP$RadioState$turnOn(void);
//#line 417
static   error_t RF230DriverLayerP$RadioSend$send(message_t *msg);
//#line 574
static inline    void RF230DriverLayerP$RadioCCA$default$done(error_t error);



static __inline void RF230DriverLayerP$downloadMessage(void);
//#line 659
static inline   void RF230DriverLayerP$IRQ$captured(uint16_t time);
//#line 672
static inline void RF230DriverLayerP$serviceRadio(void);
//#line 824
static inline   void RF230DriverLayerP$Tasklet$run(void);
//#line 854
static inline   uint8_t RF230DriverLayerP$RadioPacket$headerLength(message_t *msg);




static   uint8_t RF230DriverLayerP$RadioPacket$payloadLength(message_t *msg);




static   void RF230DriverLayerP$RadioPacket$setPayloadLength(message_t *msg, uint8_t length);








static inline   uint8_t RF230DriverLayerP$RadioPacket$maxPayloadLength(void);






static inline   uint8_t RF230DriverLayerP$RadioPacket$metadataLength(message_t *msg);
//#line 892
static inline   bool RF230DriverLayerP$PacketTransmitPower$isSet(message_t *msg);




static inline   uint8_t RF230DriverLayerP$PacketTransmitPower$get(message_t *msg);
//#line 925
static inline   void RF230DriverLayerP$PacketRSSI$clear(message_t *msg);




static inline   void RF230DriverLayerP$PacketRSSI$set(message_t *msg, uint8_t value);










static inline   bool RF230DriverLayerP$PacketTimeSyncOffset$isSet(message_t *msg);




static inline   uint8_t RF230DriverLayerP$PacketTimeSyncOffset$get(message_t *msg);
//#line 980
static inline   void RF230DriverLayerP$PacketLinkQuality$set(message_t *msg, uint8_t value);
//# 33 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
static   void HplRF230P$PortCLKM$makeInput(void);
//#line 30
static   void HplRF230P$PortCLKM$clr(void);
//# 79 "/opt/tinyos-2.x/tos/chips/stm32/timer/HplStm32Capture.nc"
static   void HplRF230P$Capture$setEdge(bool arg_0x40aae830);
//#line 38
static   HplRF230P$Capture$size_type HplRF230P$Capture$get(void);
//#line 55
static   void HplRF230P$Capture$reset(void);


static   void HplRF230P$Capture$start(void);


static   void HplRF230P$Capture$stop(void);
//# 33 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
static   void HplRF230P$PortIRQ$makeInput(void);
//#line 30
static   void HplRF230P$PortIRQ$clr(void);
//# 50 "/opt/tinyos-2.x/tos/interfaces/GpioCapture.nc"
static   void HplRF230P$IRQ$captured(uint16_t arg_0x40a2e8c8);
//# 42 "/opt/tinyos-2.x/tos/platforms/stm32-p103/chips/rf230/HplRF230P.nc"
static inline  error_t HplRF230P$PlatformInit$init(void);










static inline   void HplRF230P$Capture$captured(uint16_t time);









static inline   error_t HplRF230P$IRQ$captureRisingEdge(void);
//#line 78
static inline   void HplRF230P$IRQ$disable(void);
//# 71 "/opt/tinyos-2.x/tos/interfaces/SpiPacket.nc"
static   void Stm32SpiP$SpiPacket$sendDone(uint8_t *arg_0x40ac47e8, uint8_t *arg_0x40ac4990, uint16_t arg_0x40ac4b20, 
error_t arg_0x40ac4cb8);
//# 110 "/opt/tinyos-2.x/tos/interfaces/Resource.nc"
static   error_t Stm32SpiP$ResourceArbiter$release(
//# 87 "/opt/tinyos-2.x/tos/chips/stm32/spi/Stm32SpiP.nc"
uint8_t arg_0x40af20f8);
//# 87 "/opt/tinyos-2.x/tos/interfaces/Resource.nc"
static   error_t Stm32SpiP$ResourceArbiter$immediateRequest(
//# 87 "/opt/tinyos-2.x/tos/chips/stm32/spi/Stm32SpiP.nc"
uint8_t arg_0x40af20f8);
//# 78 "/opt/tinyos-2.x/tos/interfaces/Resource.nc"
static   error_t Stm32SpiP$ResourceArbiter$request(
//# 87 "/opt/tinyos-2.x/tos/chips/stm32/spi/Stm32SpiP.nc"
uint8_t arg_0x40af20f8);
//# 118 "/opt/tinyos-2.x/tos/interfaces/Resource.nc"
static   bool Stm32SpiP$ResourceArbiter$isOwner(
//# 87 "/opt/tinyos-2.x/tos/chips/stm32/spi/Stm32SpiP.nc"
uint8_t arg_0x40af20f8);
//# 72 "/opt/tinyos-2.x/tos/chips/stm32/spi/Stm32Spi.nc"
static   void Stm32SpiP$Spi$sleep(void);
//#line 66
static   void Stm32SpiP$Spi$initMaster(void);
//#line 96
static   void Stm32SpiP$Spi$enableInterrupt(bool arg_0x40aeb128);
//#line 80
static   uint8_t Stm32SpiP$Spi$read(void);
//#line 125
static   void Stm32SpiP$Spi$setMasterDoubleSpeed(bool arg_0x40ae6358);
//#line 114
static   void Stm32SpiP$Spi$setClock(uint8_t arg_0x40ae7168);
//#line 108
static   void Stm32SpiP$Spi$setClockPolarity(bool arg_0x40ae8170);
//#line 86
static   void Stm32SpiP$Spi$write(uint8_t arg_0x40aec6b8);
//#line 99
static   void Stm32SpiP$Spi$enableSpi(bool arg_0x40aeb8f8);
//#line 111
static   void Stm32SpiP$Spi$setClockPhase(bool arg_0x40ae8950);
//# 92 "/opt/tinyos-2.x/tos/interfaces/Resource.nc"
static  void Stm32SpiP$Resource$granted(
//# 83 "/opt/tinyos-2.x/tos/chips/stm32/spi/Stm32SpiP.nc"
uint8_t arg_0x40af5b80);
//# 80 "/opt/tinyos-2.x/tos/interfaces/ArbiterInfo.nc"
static   bool Stm32SpiP$ArbiterInfo$inUse(void);
//# 44 "/opt/tinyos-2.x/tos/interfaces/McuPowerState.nc"
static   void Stm32SpiP$McuPowerState$update(void);
//# 239 "/opt/tinyos-2.x/tos/chips/stm32/spi/Stm32SpiP.nc"
enum Stm32SpiP$__nesc_unnamed4349 {
//#line 239
  Stm32SpiP$zeroTask = 8U
};
//#line 239
typedef int Stm32SpiP$__nesc_sillytask_zeroTask[Stm32SpiP$zeroTask];
//#line 93
uint16_t Stm32SpiP$len;
uint8_t *Stm32SpiP$txBuffer;
uint8_t *Stm32SpiP$rxBuffer;
uint16_t Stm32SpiP$pos;

enum Stm32SpiP$__nesc_unnamed4350 {
  Stm32SpiP$SPI_IDLE, 
  Stm32SpiP$SPI_BUSY, 
  Stm32SpiP$SPI_ATOMIC_SIZE = 10
};





static void Stm32SpiP$startSpi(void);
//#line 122
static inline void Stm32SpiP$stopSpi(void);
//#line 140
static __inline   void Stm32SpiP$FastSpiByte$splitWrite(uint8_t data);



static __inline   uint8_t Stm32SpiP$FastSpiByte$splitRead(void);





static __inline   uint8_t Stm32SpiP$FastSpiByte$splitReadWrite(uint8_t data);










static __inline   uint8_t Stm32SpiP$FastSpiByte$write(uint8_t data);
//#line 239
static inline  void Stm32SpiP$zeroTask$runTask(void);
//#line 294
static inline    
//#line 293
void Stm32SpiP$SpiPacket$default$sendDone(
uint8_t *_txbuffer, uint8_t *_rxbuffer, 
uint16_t _length, error_t _success);
//#line 337
static inline   error_t Stm32SpiP$Resource$immediateRequest(uint8_t id);







static   error_t Stm32SpiP$Resource$request(uint8_t id);








static   error_t Stm32SpiP$Resource$release(uint8_t id);









static inline   uint8_t Stm32SpiP$Resource$isOwner(uint8_t id);



static inline  void Stm32SpiP$ResourceArbiter$granted(uint8_t id);



static inline   void Stm32SpiP$Resource$default$granted(uint8_t id);
//# 33 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
static   void HplStm32SpiP$MISO$makeInput(void);

static   void HplStm32SpiP$SCK$makeOutput(void);
//# 44 "/opt/tinyos-2.x/tos/interfaces/McuPowerState.nc"
static   void HplStm32SpiP$Mcu$update(void);
//# 35 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
static   void HplStm32SpiP$MOSI$makeOutput(void);
//# 29 "/opt/tinyos-2.x/tos/chips/stm32/spi/HplStm32SpiP.nc"
static inline   void HplStm32SpiP$SPI$initMaster(void);
//#line 44
static inline   void HplStm32SpiP$SPI$sleep(void);



static inline   uint8_t HplStm32SpiP$SPI$read(void);
static inline   void HplStm32SpiP$SPI$write(uint8_t d);
//#line 70
static inline   void HplStm32SpiP$SPI$enableInterrupt(bool enabled);
//#line 103
static inline   void HplStm32SpiP$SPI$enableSpi(bool enabled);
//#line 133
static inline   void HplStm32SpiP$SPI$setMasterBit(bool isMaster);
//#line 148
static inline   void HplStm32SpiP$SPI$setClockPolarity(bool highWhenIdle);
//#line 164
static inline   void HplStm32SpiP$SPI$setClockPhase(bool sampleOnTrailing);
//#line 183
static inline   void HplStm32SpiP$SPI$setClock(uint8_t v);
//#line 197
static inline   void HplStm32SpiP$SPI$setMasterDoubleSpeed(bool on);
//# 39 "/opt/tinyos-2.x/tos/system/FcfsResourceQueueC.nc"
enum /*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$__nesc_unnamed4351 {
//#line 39
  FcfsResourceQueueC$0$NO_ENTRY = 0xFF
};
uint8_t /*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$resQ[1U];
uint8_t /*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$qHead = /*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$NO_ENTRY;
uint8_t /*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$qTail = /*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$NO_ENTRY;

static inline  error_t /*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$Init$init(void);




static inline   bool /*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$FcfsQueue$isEmpty(void);



static inline   bool /*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$FcfsQueue$isEnqueued(resource_client_id_t id);



static inline   resource_client_id_t /*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$FcfsQueue$dequeue(void);
//#line 72
static inline   error_t /*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$FcfsQueue$enqueue(resource_client_id_t id);
//# 43 "/opt/tinyos-2.x/tos/interfaces/ResourceRequested.nc"
static   void /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$ResourceRequested$requested(
//# 52 "/opt/tinyos-2.x/tos/system/SimpleArbiterP.nc"
uint8_t arg_0x40b5f688);
//# 51 "/opt/tinyos-2.x/tos/interfaces/ResourceRequested.nc"
static   void /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$ResourceRequested$immediateRequested(
//# 52 "/opt/tinyos-2.x/tos/system/SimpleArbiterP.nc"
uint8_t arg_0x40b5f688);
//# 55 "/opt/tinyos-2.x/tos/interfaces/ResourceConfigure.nc"
static   void /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$ResourceConfigure$unconfigure(
//# 56 "/opt/tinyos-2.x/tos/system/SimpleArbiterP.nc"
uint8_t arg_0x40b5e188);
//# 49 "/opt/tinyos-2.x/tos/interfaces/ResourceConfigure.nc"
static   void /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$ResourceConfigure$configure(
//# 56 "/opt/tinyos-2.x/tos/system/SimpleArbiterP.nc"
uint8_t arg_0x40b5e188);
//# 69 "/opt/tinyos-2.x/tos/interfaces/ResourceQueue.nc"
static   error_t /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$Queue$enqueue(resource_client_id_t arg_0x40b6a010);
//#line 43
static   bool /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$Queue$isEmpty(void);
//#line 60
static   resource_client_id_t /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$Queue$dequeue(void);
//# 92 "/opt/tinyos-2.x/tos/interfaces/Resource.nc"
static  void /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$Resource$granted(
//# 51 "/opt/tinyos-2.x/tos/system/SimpleArbiterP.nc"
uint8_t arg_0x40b60c90);
//# 56 "/opt/tinyos-2.x/tos/interfaces/TaskBasic.nc"
static   error_t /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$grantedTask$postTask(void);
//# 69 "/opt/tinyos-2.x/tos/system/SimpleArbiterP.nc"
enum /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$__nesc_unnamed4352 {
//#line 69
  SimpleArbiterP$0$grantedTask = 9U
};
//#line 69
typedef int /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$__nesc_sillytask_grantedTask[/*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$grantedTask];
//#line 62
enum /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$__nesc_unnamed4353 {
//#line 62
  SimpleArbiterP$0$RES_IDLE = 0, SimpleArbiterP$0$RES_GRANTING = 1, SimpleArbiterP$0$RES_BUSY = 2
};
//#line 63
enum /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$__nesc_unnamed4354 {
//#line 63
  SimpleArbiterP$0$NO_RES = 0xFF
};
uint8_t /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$state = /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$RES_IDLE;
 uint8_t /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$resId = /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$NO_RES;
 uint8_t /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$reqResId;



static inline   error_t /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$Resource$request(uint8_t id);
//#line 84
static inline   error_t /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$Resource$immediateRequest(uint8_t id);
//#line 97
static inline   error_t /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$Resource$release(uint8_t id);
//#line 124
static   bool /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$ArbiterInfo$inUse(void);
//#line 148
static inline   uint8_t /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$Resource$isOwner(uint8_t id);






static inline  void /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$grantedTask$runTask(void);
//#line 167
static inline    void /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$ResourceRequested$default$requested(uint8_t id);

static inline    void /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$ResourceRequested$default$immediateRequested(uint8_t id);

static inline    void /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$ResourceConfigure$default$configure(uint8_t id);

static inline    void /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$ResourceConfigure$default$unconfigure(uint8_t id);
//# 49 "/opt/tinyos-2.x/tos/chips/stm32/timer/HplStm32Compare.nc"
static   void HplStm32Timer3P$CompareA$fired(void);
//# 51 "/opt/tinyos-2.x/tos/chips/stm32/timer/HplStm32Capture.nc"
static   void HplStm32Timer3P$Capture$captured(HplStm32Timer3P$Capture$size_type arg_0x40ab0248);
//# 49 "/opt/tinyos-2.x/tos/chips/stm32/timer/HplStm32Compare.nc"
static   void HplStm32Timer3P$CompareC$fired(void);
//# 61 "/opt/tinyos-2.x/tos/chips/stm32/timer/HplStm32Timer.nc"
static   void HplStm32Timer3P$Timer$overflow(void);
//# 26 "/opt/tinyos-2.x/tos/chips/stm32/timer/HplStm32Timer3P.nc"
static inline   uint16_t HplStm32Timer3P$Timer$get(void);







static inline   void HplStm32Timer3P$Capture$setEdge(bool up);
//#line 47
static inline   void HplStm32Timer3P$Capture$reset(void);
static inline   void HplStm32Timer3P$CompareA$reset(void);



static inline   void HplStm32Timer3P$Capture$start(void);
static inline   void HplStm32Timer3P$CompareA$start(void);



static inline   void HplStm32Timer3P$Capture$stop(void);
static inline   void HplStm32Timer3P$CompareA$stop(void);



static inline   bool HplStm32Timer3P$Timer$test(void);
//#line 102
static inline   void HplStm32Timer3P$CompareA$set(uint16_t t);



static inline   uint16_t HplStm32Timer3P$Capture$get(void);








void SIG_OUTPUT_COMPARE1A(void) __attribute((interrupt))   ;


static inline    void HplStm32Timer3P$CompareC$default$fired(void);
void SIG_OUTPUT_COMPARE1C(void) __attribute((interrupt))   ;



void SIG_INPUT_CAPTURE1(void) __attribute((interrupt))   ;



void SIG_OVERFLOW1(void) __attribute((interrupt))   ;
//# 67 "/opt/tinyos-2.x/tos/lib/timer/Alarm.nc"
static   void /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$Alarm$fired(void);
//# 52 "/opt/tinyos-2.x/tos/chips/stm32/timer/HplStm32Timer.nc"
static   /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$HplStm32Timer$timer_size /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$HplStm32Timer$get(void);
//# 53 "/opt/tinyos-2.x/tos/chips/stm32/timer/HplStm32Compare.nc"
static   void /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$HplStm32Compare$reset(void);
//#line 45
static   void /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$HplStm32Compare$set(/*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$HplStm32Compare$size_type arg_0x40b83350);










static   void /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$HplStm32Compare$start(void);


static   void /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$HplStm32Compare$stop(void);
//# 53 "/opt/tinyos-2.x/tos/chips/stm32/timer/Stm32AlarmC.nc"
static inline   /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$timer_size /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$Alarm$getNow(void);
//#line 65
static inline   void /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$Alarm$stop(void);



static inline   void /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$Alarm$start(/*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$timer_size dt);




static   void /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$Alarm$startAt(/*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$timer_size t0, /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$timer_size dt);
//#line 110
static inline   void /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$HplStm32Compare$fired(void);






static inline   void /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$HplStm32Timer$overflow(void);
//# 71 "/opt/tinyos-2.x/tos/lib/timer/Counter.nc"
static   void /*CounterOne16C.NCounter*/Stm32CounterC$0$Counter$overflow(void);
//# 78 "/opt/tinyos-2.x/tos/chips/stm32/timer/HplStm32Timer.nc"
static   bool /*CounterOne16C.NCounter*/Stm32CounterC$0$Timer$test(void);
//#line 52
static   /*CounterOne16C.NCounter*/Stm32CounterC$0$Timer$timer_size /*CounterOne16C.NCounter*/Stm32CounterC$0$Timer$get(void);
//# 41 "/opt/tinyos-2.x/tos/chips/stm32/timer/Stm32CounterC.nc"
static inline   /*CounterOne16C.NCounter*/Stm32CounterC$0$timer_size /*CounterOne16C.NCounter*/Stm32CounterC$0$Counter$get(void);




static inline   bool /*CounterOne16C.NCounter*/Stm32CounterC$0$Counter$isOverflowPending(void);









static inline   void /*CounterOne16C.NCounter*/Stm32CounterC$0$Timer$overflow(void);
//# 53 "/opt/tinyos-2.x/tos/lib/timer/Counter.nc"
static   /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$CounterFrom$size_type /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$CounterFrom$get(void);






static   bool /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$CounterFrom$isOverflowPending(void);










static   void /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$Counter$overflow(void);
//# 56 "/opt/tinyos-2.x/tos/lib/timer/TransformCounterC.nc"
/*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$upper_count_type /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$m_upper;

enum /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$__nesc_unnamed4355 {

  TransformCounterC$0$LOW_SHIFT_RIGHT = 0, 
  TransformCounterC$0$HIGH_SHIFT_LEFT = 8 * sizeof(/*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$from_size_type ) - /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$LOW_SHIFT_RIGHT, 
  TransformCounterC$0$NUM_UPPER_BITS = 8 * sizeof(/*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$to_size_type ) - 8 * sizeof(/*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$from_size_type ) + 0, 



  TransformCounterC$0$OVERFLOW_MASK = /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$NUM_UPPER_BITS ? ((/*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$upper_count_type )2 << (/*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$NUM_UPPER_BITS - 1)) - 1 : 0
};

static   /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$to_size_type /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$Counter$get(void);
//#line 122
static inline   void /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$CounterFrom$overflow(void);
//# 53 "/opt/tinyos-2.x/tos/lib/timer/Counter.nc"
static   /*LocalTimeMicroC.CounterToLocalTimeC*/CounterToLocalTimeC$0$Counter$size_type /*LocalTimeMicroC.CounterToLocalTimeC*/CounterToLocalTimeC$0$Counter$get(void);
//# 42 "/opt/tinyos-2.x/tos/lib/timer/CounterToLocalTimeC.nc"
static inline   uint32_t /*LocalTimeMicroC.CounterToLocalTimeC*/CounterToLocalTimeC$0$LocalTime$get(void);




static inline   void /*LocalTimeMicroC.CounterToLocalTimeC*/CounterToLocalTimeC$0$Counter$overflow(void);
//# 53 "/opt/tinyos-2.x/tos/lib/timer/Counter.nc"
static   /*BusyWaitMicroC.Stm32BusyWaitMicro*/HalStm32BusyWaitM$0$OST$size_type /*BusyWaitMicroC.Stm32BusyWaitMicro*/HalStm32BusyWaitM$0$OST$get(void);
//# 56 "/opt/tinyos-2.x/tos/chips/stm32/timer/HalStm32BusyWaitM.nc"
static   void /*BusyWaitMicroC.Stm32BusyWaitMicro*/HalStm32BusyWaitM$0$BusyWait$wait(uint16_t dt);









static inline   void /*BusyWaitMicroC.Stm32BusyWaitMicro*/HalStm32BusyWaitM$0$OST$overflow(void);
//# 56 "/opt/tinyos-2.x/tos/interfaces/Leds.nc"
static   void STMTestC$Leds$led0Toggle(void);
//# 5 "STMAppMessaging.nc"
static  error_t STMTestC$STMAppMessaging$Send(uint8_t arg_0x406e4968, uint16_t arg_0x406e4af8, uint8_t *arg_0x406e4ca0, uint16_t arg_0x406e4e30);
//#line 3
static  void STMTestC$STMAppMessaging$Init(void);
//# 21 "STMTestC.nc"
uint8_t STMTestC$packet[10];
uint8_t STMTestC$myAppID = 0x34;
uint8_t STMTestC$counter = 1;

static inline  void STMTestC$Boot$booted(void);
//#line 39
static inline  void STMTestC$Timer0$fired(void);
//#line 54
static inline  void STMTestC$STMAppMessaging$Receive(uint8_t appID, uint8_t *payload, uint16_t len);
//# 80 "/opt/tinyos-2.x/tos/chips/stm32/stm32hardware.h"
static __inline void __nesc_enable_interrupt(void)
//#line 80
{
  uint32_t statusReg = 0;









   __asm volatile ("CPSIE I");
  return;
}

//#line 39
 __inline __nesc_atomic_t __nesc_atomic_start(void )
{
  uint32_t result = 0;
  uint32_t temp = 0;

  __nesc_enable_interrupt();










  return result;
}

 __inline void __nesc_atomic_end(__nesc_atomic_t oldState)
{
  uint32_t statusReg = 0;


  __nesc_disable_interrupt();
//#line 77
  return;
}

//# 56 "/opt/tinyos-2.x/tos/interfaces/TaskBasic.nc"
inline static   error_t /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$fired$postTask(void){
//#line 56
  unsigned char result;
//#line 56

//#line 56
  result = SchedulerBasicP$TaskBasic$postTask(/*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$fired);
//#line 56

//#line 56
  return result;
//#line 56
}
//#line 56
//# 70 "/opt/tinyos-2.x/tos/lib/timer/AlarmToTimerC.nc"
static inline   void /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Alarm$fired(void)
{
//#line 71
  /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$fired$postTask();
}

//# 67 "/opt/tinyos-2.x/tos/lib/timer/Alarm.nc"
inline static   void STM32RtcC$Alarm$fired(void){
//#line 67
  /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Alarm$fired();
//#line 67
}
//#line 67
//# 86 "/opt/tinyos-2.x/tos/system/SchedulerBasicP.nc"
static inline bool SchedulerBasicP$isWaiting(uint8_t id)
{
  return SchedulerBasicP$m_next[id] != SchedulerBasicP$NO_TASK || SchedulerBasicP$m_tail == id;
}

static inline bool SchedulerBasicP$pushTask(uint8_t id)
{
  if (!SchedulerBasicP$isWaiting(id)) 
    {
      if (SchedulerBasicP$m_head == SchedulerBasicP$NO_TASK) 
        {
          SchedulerBasicP$m_head = id;
          SchedulerBasicP$m_tail = id;
        }
      else 
        {
          SchedulerBasicP$m_next[SchedulerBasicP$m_tail] = id;
          SchedulerBasicP$m_tail = id;
        }
      return TRUE;
    }
  else 
    {
      return FALSE;
    }
}

//# 66 "/opt/tinyos-2.x/tos/chips/stm32/timer/HalStm32BusyWaitM.nc"
static inline   void /*BusyWaitMicroC.Stm32BusyWaitMicro*/HalStm32BusyWaitM$0$OST$overflow(void)
//#line 66
{
  return;
}

//# 71 "/opt/tinyos-2.x/tos/lib/timer/Counter.nc"
inline static   void STM32RtcC$Counter$overflow(void){
//#line 71
  /*BusyWaitMicroC.Stm32BusyWaitMicro*/HalStm32BusyWaitM$0$OST$overflow();
//#line 71
}
//#line 71
//# 104 "/opt/tinyos-2.x/tos/platforms/stm32-p103/PlatformP.nc"
static inline   void PlatformP$Interrupt$fired(void)
{
}

//# 31 "/opt/tinyos-2.x/tos/chips/stm32/HplSTM32Interrupt.nc"
inline static   void HplSTM32InterruptM$Irq$fired(void){
//#line 31
  PlatformP$Interrupt$fired();
//#line 31
}
//#line 31
//# 113 "/opt/tinyos-2.x/tos/system/SchedulerBasicP.nc"
static inline  void SchedulerBasicP$Scheduler$init(void)
{
  /* atomic removed: atomic calls only */
  {
    memset((void *)SchedulerBasicP$m_next, SchedulerBasicP$NO_TASK, sizeof SchedulerBasicP$m_next);
    SchedulerBasicP$m_head = SchedulerBasicP$NO_TASK;
    SchedulerBasicP$m_tail = SchedulerBasicP$NO_TASK;
  }
}

//# 46 "/opt/tinyos-2.x/tos/interfaces/Scheduler.nc"
inline static  void RealMainP$Scheduler$init(void){
//#line 46
  SchedulerBasicP$Scheduler$init();
//#line 46
}
//#line 46
//# 58 "/opt/tinyos-2.x/tos/types/TinyError.h"
static inline  error_t ecombine(error_t r1, error_t r2)




{
  return r1 == r2 ? r1 : FAIL;
}

//# 47 "/opt/tinyos-2.x/tos/chips/stm32/gpio/HplSTM32GeneralIOPinP.nc"
static __inline   void /*HplSTM32GeneralIOC.PortC.Bit9*/HplSTM32GeneralIOPinP$41$IO$set(void)
//#line 47
{
  GPIO_TypeDef *port = (GPIO_TypeDef *)1073811456U;

//#line 49
  * (volatile int32_t *)((uint32_t )0x42000000 + ((uint32_t )& port->ODR - (uint32_t )0x40000000) * 32 + 9 * 4) = 1;
}

//# 29 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
inline static   void LedsP$Led2$set(void){
//#line 29
  /*HplSTM32GeneralIOC.PortC.Bit9*/HplSTM32GeneralIOPinP$41$IO$set();
//#line 29
}
//#line 29
//# 47 "/opt/tinyos-2.x/tos/chips/stm32/gpio/HplSTM32GeneralIOPinP.nc"
static __inline   void /*HplSTM32GeneralIOC.PortC.Bit8*/HplSTM32GeneralIOPinP$40$IO$set(void)
//#line 47
{
  GPIO_TypeDef *port = (GPIO_TypeDef *)1073811456U;

//#line 49
  * (volatile int32_t *)((uint32_t )0x42000000 + ((uint32_t )& port->ODR - (uint32_t )0x40000000) * 32 + 8 * 4) = 1;
}

//# 29 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
inline static   void LedsP$Led1$set(void){
//#line 29
  /*HplSTM32GeneralIOC.PortC.Bit8*/HplSTM32GeneralIOPinP$40$IO$set();
//#line 29
}
//#line 29
//# 47 "/opt/tinyos-2.x/tos/chips/stm32/gpio/HplSTM32GeneralIOPinP.nc"
static __inline   void /*HplSTM32GeneralIOC.PortC.Bit12*/HplSTM32GeneralIOPinP$44$IO$set(void)
//#line 47
{
  GPIO_TypeDef *port = (GPIO_TypeDef *)1073811456U;

//#line 49
  * (volatile int32_t *)((uint32_t )0x42000000 + ((uint32_t )& port->ODR - (uint32_t )0x40000000) * 32 + 12 * 4) = 1;
}

//# 29 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
inline static   void LedsP$Led0$set(void){
//#line 29
  /*HplSTM32GeneralIOC.PortC.Bit12*/HplSTM32GeneralIOPinP$44$IO$set();
//#line 29
}
//#line 29
//# 82 "/opt/tinyos-2.x/tos/chips/stm32/gpio/HplSTM32GeneralIOPinP.nc"
static __inline   void /*HplSTM32GeneralIOC.PortC.Bit9*/HplSTM32GeneralIOPinP$41$IO$makeOutput(void)
//#line 82
{
  GPIO_TypeDef *port = (GPIO_TypeDef *)1073811456U;

  GPIO_InitTypeDef gpioi = { 
  (uint16_t )1 << 9, 
  GPIO_Speed_10MHz, 
  GPIO_Mode_Out_PP };

  GPIO_Init(port, &gpioi);
}

//# 35 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
inline static   void LedsP$Led2$makeOutput(void){
//#line 35
  /*HplSTM32GeneralIOC.PortC.Bit9*/HplSTM32GeneralIOPinP$41$IO$makeOutput();
//#line 35
}
//#line 35
//# 82 "/opt/tinyos-2.x/tos/chips/stm32/gpio/HplSTM32GeneralIOPinP.nc"
static __inline   void /*HplSTM32GeneralIOC.PortC.Bit8*/HplSTM32GeneralIOPinP$40$IO$makeOutput(void)
//#line 82
{
  GPIO_TypeDef *port = (GPIO_TypeDef *)1073811456U;

  GPIO_InitTypeDef gpioi = { 
  (uint16_t )1 << 8, 
  GPIO_Speed_10MHz, 
  GPIO_Mode_Out_PP };

  GPIO_Init(port, &gpioi);
}

//# 35 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
inline static   void LedsP$Led1$makeOutput(void){
//#line 35
  /*HplSTM32GeneralIOC.PortC.Bit8*/HplSTM32GeneralIOPinP$40$IO$makeOutput();
//#line 35
}
//#line 35
//# 82 "/opt/tinyos-2.x/tos/chips/stm32/gpio/HplSTM32GeneralIOPinP.nc"
static __inline   void /*HplSTM32GeneralIOC.PortC.Bit12*/HplSTM32GeneralIOPinP$44$IO$makeOutput(void)
//#line 82
{
  GPIO_TypeDef *port = (GPIO_TypeDef *)1073811456U;

  GPIO_InitTypeDef gpioi = { 
  (uint16_t )1 << 12, 
  GPIO_Speed_10MHz, 
  GPIO_Mode_Out_PP };

  GPIO_Init(port, &gpioi);
}

//# 35 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
inline static   void LedsP$Led0$makeOutput(void){
//#line 35
  /*HplSTM32GeneralIOC.PortC.Bit12*/HplSTM32GeneralIOPinP$44$IO$makeOutput();
//#line 35
}
//#line 35
//# 45 "/opt/tinyos-2.x/tos/system/LedsP.nc"
static inline  error_t LedsP$Init$init(void)
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
inline static  error_t PlatformP$MoteInit$init(void){
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
//# 51 "/opt/tinyos-2.x/tos/chips/stm32/McuSleepC.nc"
static inline  error_t McuSleepC$McuSleepInit$init(void)
//#line 51
{
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;




  EXTI_ClearITPendingBit((uint32_t )0x20000);
  EXTI_InitStructure.EXTI_Line = (uint32_t )0x20000;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_Init(&EXTI_InitStructure);


  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  return SUCCESS;
}

//# 51 "/opt/tinyos-2.x/tos/interfaces/Init.nc"
inline static  error_t PlatformP$McuSleepInit$init(void){
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
static inline  error_t MoteClockP$MoteClockInit$init(void)
{

  RCC_HSICmd(ENABLE);


  while (RCC_GetFlagStatus((uint8_t )0x21) == RESET) ;
  RCC_SYSCLKConfig((uint32_t )0x00000000);


  RCC_HSEConfig((uint32_t )0x00010000);


  while (RCC_GetFlagStatus((uint8_t )0x31) == RESET) ;


  RCC_PLLConfig((uint32_t )0x00010000, (uint32_t )0x001C0000);


  RCC_PLLCmd(ENABLE);


  while (RCC_GetFlagStatus((uint8_t )0x39) == RESET) ;


  RCC_USBCLKConfig((uint8_t )0x00);
  RCC_ADCCLKConfig((uint32_t )0x0000C000);
  RCC_PCLK2Config((uint32_t )0x00000000);
  RCC_PCLK1Config((uint32_t )0x00000400);
  RCC_HCLKConfig((uint32_t )0x00000000);


  * (volatile uint32_t *)0x40022000 = 0x12;


  RCC_SYSCLKConfig((uint32_t )0x00000002);

  return SUCCESS;
}

//# 51 "/opt/tinyos-2.x/tos/interfaces/Init.nc"
inline static  error_t PlatformP$MoteClockInit$init(void){
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
static inline  error_t PlatformP$Init$init(void)
//#line 46
{

  GPIO_InitTypeDef gpioa = { 
  (uint16_t )0xFFFF, 
  GPIO_Speed_2MHz, 
  GPIO_Mode_Out_OD };


  GPIO_Init((GPIO_TypeDef *)((uint32_t )0x40000000 + 0x10000 + 0x0800), &gpioa);
  ((GPIO_TypeDef *)((uint32_t )0x40000000 + 0x10000 + 0x0800))->ODR = 0;
  GPIO_Init((GPIO_TypeDef *)((uint32_t )0x40000000 + 0x10000 + 0x0C00), &gpioa);
  ((GPIO_TypeDef *)((uint32_t )0x40000000 + 0x10000 + 0x0C00))->ODR = 0;
  GPIO_Init((GPIO_TypeDef *)((uint32_t )0x40000000 + 0x10000 + 0x1000), &gpioa);
  ((GPIO_TypeDef *)((uint32_t )0x40000000 + 0x10000 + 0x1000))->ODR = 0;
  GPIO_Init((GPIO_TypeDef *)((uint32_t )0x40000000 + 0x10000 + 0x1400), &gpioa);
  ((GPIO_TypeDef *)((uint32_t )0x40000000 + 0x10000 + 0x1400))->ODR = 0;

  * (volatile unsigned long *)0xE000ED14 = * (volatile unsigned long *)0xE000ED14 | 0x200;



  PlatformP$MoteClockInit$init();
  PlatformP$McuSleepInit$init();

  RCC_APB2PeriphClockCmd((uint32_t )0x00000001, ENABLE);
  RCC_APB2PeriphClockCmd((uint32_t )0x00000010 | (uint32_t )0x00000004, ENABLE);

  PlatformP$MoteInit$init();




  return SUCCESS;
}

//# 47 "/opt/tinyos-2.x/tos/chips/stm32/gpio/HplSTM32GeneralIOPinP.nc"
static __inline   void /*HplSTM32GeneralIOC.PortA.Bit6*/HplSTM32GeneralIOPinP$6$IO$set(void)
//#line 47
{
  GPIO_TypeDef *port = (GPIO_TypeDef *)1073809408U;

//#line 49
  * (volatile int32_t *)((uint32_t )0x42000000 + ((uint32_t )& port->ODR - (uint32_t )0x40000000) * 32 + 6 * 4) = 1;
}

//# 29 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
inline static   void RF230DriverLayerP$RSTN$set(void){
//#line 29
  /*HplSTM32GeneralIOC.PortA.Bit6*/HplSTM32GeneralIOPinP$6$IO$set();
//#line 29
}
//#line 29
//# 82 "/opt/tinyos-2.x/tos/chips/stm32/gpio/HplSTM32GeneralIOPinP.nc"
static __inline   void /*HplSTM32GeneralIOC.PortA.Bit6*/HplSTM32GeneralIOPinP$6$IO$makeOutput(void)
//#line 82
{
  GPIO_TypeDef *port = (GPIO_TypeDef *)1073809408U;

  GPIO_InitTypeDef gpioi = { 
  (uint16_t )1 << 6, 
  GPIO_Speed_10MHz, 
  GPIO_Mode_Out_PP };

  GPIO_Init(port, &gpioi);
}

//# 35 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
inline static   void RF230DriverLayerP$RSTN$makeOutput(void){
//#line 35
  /*HplSTM32GeneralIOC.PortA.Bit6*/HplSTM32GeneralIOPinP$6$IO$makeOutput();
//#line 35
}
//#line 35
//# 53 "/opt/tinyos-2.x/tos/chips/stm32/gpio/HplSTM32GeneralIOPinP.nc"
static __inline   void /*HplSTM32GeneralIOC.PortB.Bit7*/HplSTM32GeneralIOPinP$23$IO$clr(void)
//#line 53
{
  GPIO_TypeDef *port = (GPIO_TypeDef *)1073810432U;

//#line 55
  * (volatile int32_t *)((uint32_t )0x42000000 + ((uint32_t )& port->ODR - (uint32_t )0x40000000) * 32 + 7 * 4) = 0;
}

//# 30 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
inline static   void RF230DriverLayerP$SLP_TR$clr(void){
//#line 30
  /*HplSTM32GeneralIOC.PortB.Bit7*/HplSTM32GeneralIOPinP$23$IO$clr();
//#line 30
}
//#line 30
//# 82 "/opt/tinyos-2.x/tos/chips/stm32/gpio/HplSTM32GeneralIOPinP.nc"
static __inline   void /*HplSTM32GeneralIOC.PortB.Bit7*/HplSTM32GeneralIOPinP$23$IO$makeOutput(void)
//#line 82
{
  GPIO_TypeDef *port = (GPIO_TypeDef *)1073810432U;

  GPIO_InitTypeDef gpioi = { 
  (uint16_t )1 << 7, 
  GPIO_Speed_10MHz, 
  GPIO_Mode_Out_PP };

  GPIO_Init(port, &gpioi);
}

//# 35 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
inline static   void RF230DriverLayerP$SLP_TR$makeOutput(void){
//#line 35
  /*HplSTM32GeneralIOC.PortB.Bit7*/HplSTM32GeneralIOPinP$23$IO$makeOutput();
//#line 35
}
//#line 35
//# 47 "/opt/tinyos-2.x/tos/chips/stm32/gpio/HplSTM32GeneralIOPinP.nc"
static __inline   void /*HplSTM32GeneralIOC.PortB.Bit0*/HplSTM32GeneralIOPinP$16$IO$set(void)
//#line 47
{
  GPIO_TypeDef *port = (GPIO_TypeDef *)1073810432U;

//#line 49
  * (volatile int32_t *)((uint32_t )0x42000000 + ((uint32_t )& port->ODR - (uint32_t )0x40000000) * 32 + 0 * 4) = 1;
}

//# 29 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
inline static   void RF230DriverLayerP$SELN$set(void){
//#line 29
  /*HplSTM32GeneralIOC.PortB.Bit0*/HplSTM32GeneralIOPinP$16$IO$set();
//#line 29
}
//#line 29
//# 82 "/opt/tinyos-2.x/tos/chips/stm32/gpio/HplSTM32GeneralIOPinP.nc"
static __inline   void /*HplSTM32GeneralIOC.PortB.Bit0*/HplSTM32GeneralIOPinP$16$IO$makeOutput(void)
//#line 82
{
  GPIO_TypeDef *port = (GPIO_TypeDef *)1073810432U;

  GPIO_InitTypeDef gpioi = { 
  (uint16_t )1 << 0, 
  GPIO_Speed_10MHz, 
  GPIO_Mode_Out_PP };

  GPIO_Init(port, &gpioi);
}

//# 35 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
inline static   void RF230DriverLayerP$SELN$makeOutput(void){
//#line 35
  /*HplSTM32GeneralIOC.PortB.Bit0*/HplSTM32GeneralIOPinP$16$IO$makeOutput();
//#line 35
}
//#line 35
//# 206 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230DriverLayerP.nc"
static inline  error_t RF230DriverLayerP$PlatformInit$init(void)
{
  RF230DriverLayerP$SELN$makeOutput();
  RF230DriverLayerP$SELN$set();
  RF230DriverLayerP$SLP_TR$makeOutput();
  RF230DriverLayerP$SLP_TR$clr();
  RF230DriverLayerP$RSTN$makeOutput();
  RF230DriverLayerP$RSTN$set();

  RF230DriverLayerP$rxMsg = &RF230DriverLayerP$rxMsgBuffer;


  RF230DriverLayerP$rssiClear = 0;
  RF230DriverLayerP$rssiBusy = 90;

  return SUCCESS;
}

//# 57 "/opt/tinyos-2.x/tos/chips/stm32/timer/HplStm32Timer3P.nc"
static inline   void HplStm32Timer3P$Capture$stop(void)
//#line 57
{
}

//# 61 "/opt/tinyos-2.x/tos/chips/stm32/timer/HplStm32Capture.nc"
inline static   void HplRF230P$Capture$stop(void){
//#line 61
  HplStm32Timer3P$Capture$stop();
//#line 61
}
//#line 61
//# 53 "/opt/tinyos-2.x/tos/chips/stm32/gpio/HplSTM32GeneralIOPinP.nc"
static __inline   void /*HplSTM32GeneralIOC.PortA.Bit0*/HplSTM32GeneralIOPinP$0$IO$clr(void)
//#line 53
{
  GPIO_TypeDef *port = (GPIO_TypeDef *)1073809408U;

//#line 55
  * (volatile int32_t *)((uint32_t )0x42000000 + ((uint32_t )& port->ODR - (uint32_t )0x40000000) * 32 + 0 * 4) = 0;
}

//# 30 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
inline static   void HplRF230P$PortIRQ$clr(void){
//#line 30
  /*HplSTM32GeneralIOC.PortA.Bit0*/HplSTM32GeneralIOPinP$0$IO$clr();
//#line 30
}
//#line 30
//# 67 "/opt/tinyos-2.x/tos/chips/stm32/gpio/HplSTM32GeneralIOPinP.nc"
static __inline   void /*HplSTM32GeneralIOC.PortA.Bit0*/HplSTM32GeneralIOPinP$0$IO$makeInput(void)
//#line 67
{
  GPIO_TypeDef *port = (GPIO_TypeDef *)1073809408U;

  GPIO_InitTypeDef gpioi = { 
  (uint16_t )1 << 0, 
  GPIO_Speed_10MHz, 
  GPIO_Mode_IN_FLOATING };

  GPIO_Init(port, &gpioi);
}

//# 33 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
inline static   void HplRF230P$PortIRQ$makeInput(void){
//#line 33
  /*HplSTM32GeneralIOC.PortA.Bit0*/HplSTM32GeneralIOPinP$0$IO$makeInput();
//#line 33
}
//#line 33
//# 53 "/opt/tinyos-2.x/tos/chips/stm32/gpio/HplSTM32GeneralIOPinP.nc"
static __inline   void /*HplSTM32GeneralIOC.PortA.Bit6*/HplSTM32GeneralIOPinP$6$IO$clr(void)
//#line 53
{
  GPIO_TypeDef *port = (GPIO_TypeDef *)1073809408U;

//#line 55
  * (volatile int32_t *)((uint32_t )0x42000000 + ((uint32_t )& port->ODR - (uint32_t )0x40000000) * 32 + 6 * 4) = 0;
}

//# 30 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
inline static   void HplRF230P$PortCLKM$clr(void){
//#line 30
  /*HplSTM32GeneralIOC.PortA.Bit6*/HplSTM32GeneralIOPinP$6$IO$clr();
//#line 30
}
//#line 30
//# 67 "/opt/tinyos-2.x/tos/chips/stm32/gpio/HplSTM32GeneralIOPinP.nc"
static __inline   void /*HplSTM32GeneralIOC.PortA.Bit6*/HplSTM32GeneralIOPinP$6$IO$makeInput(void)
//#line 67
{
  GPIO_TypeDef *port = (GPIO_TypeDef *)1073809408U;

  GPIO_InitTypeDef gpioi = { 
  (uint16_t )1 << 6, 
  GPIO_Speed_10MHz, 
  GPIO_Mode_IN_FLOATING };

  GPIO_Init(port, &gpioi);
}

//# 33 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
inline static   void HplRF230P$PortCLKM$makeInput(void){
//#line 33
  /*HplSTM32GeneralIOC.PortA.Bit6*/HplSTM32GeneralIOPinP$6$IO$makeInput();
//#line 33
}
//#line 33
//# 42 "/opt/tinyos-2.x/tos/platforms/stm32-p103/chips/rf230/HplRF230P.nc"
static inline  error_t HplRF230P$PlatformInit$init(void)
{
  HplRF230P$PortCLKM$makeInput();
  HplRF230P$PortCLKM$clr();
  HplRF230P$PortIRQ$makeInput();
  HplRF230P$PortIRQ$clr();
  HplRF230P$Capture$stop();

  return SUCCESS;
}

//# 51 "/opt/tinyos-2.x/tos/interfaces/Init.nc"
inline static  error_t RealMainP$PlatformInit$init(void){
//#line 51
  unsigned char result;
//#line 51

//#line 51
  result = HplRF230P$PlatformInit$init();
//#line 51
  result = ecombine(result, RF230DriverLayerP$PlatformInit$init());
//#line 51
  result = ecombine(result, PlatformP$Init$init());
//#line 51

//#line 51
  return result;
//#line 51
}
//#line 51
//# 54 "/opt/tinyos-2.x/tos/interfaces/Scheduler.nc"
inline static  bool RealMainP$Scheduler$runNextTask(void){
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
//# 48 "/opt/tinyos-2.x/tos/chips/rf2xx/util/Tasklet.nc"
inline static   void RF230DriverLayerP$Tasklet$schedule(void){
//#line 48
  TaskletC$Tasklet$schedule();
//#line 48
}
//#line 48
//# 110 "/opt/tinyos-2.x/tos/interfaces/Resource.nc"
inline static   error_t RF230DriverLayerP$SpiResource$release(void){
//#line 110
  unsigned char result;
//#line 110

//#line 110
  result = Stm32SpiP$Resource$release(0U);
//#line 110

//#line 110
  return result;
//#line 110
}
//#line 110
//# 47 "/opt/tinyos-2.x/tos/chips/stm32/gpio/HplSTM32GeneralIOPinP.nc"
static __inline   void /*HplSTM32GeneralIOC.PortB.Bit7*/HplSTM32GeneralIOPinP$23$IO$set(void)
//#line 47
{
  GPIO_TypeDef *port = (GPIO_TypeDef *)1073810432U;

//#line 49
  * (volatile int32_t *)((uint32_t )0x42000000 + ((uint32_t )& port->ODR - (uint32_t )0x40000000) * 32 + 7 * 4) = 1;
}

//# 29 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
inline static   void RF230DriverLayerP$SLP_TR$set(void){
//#line 29
  /*HplSTM32GeneralIOC.PortB.Bit7*/HplSTM32GeneralIOPinP$23$IO$set();
//#line 29
}
//#line 29
//# 48 "/opt/tinyos-2.x/tos/chips/stm32/spi/HplStm32SpiP.nc"
static inline   uint8_t HplStm32SpiP$SPI$read(void)
//#line 48
{
//#line 48
  return SPI_I2S_ReceiveData((SPI_TypeDef *)((uint32_t )0x40000000 + 0x10000 + 0x3000));
}

//# 80 "/opt/tinyos-2.x/tos/chips/stm32/spi/Stm32Spi.nc"
inline static   uint8_t Stm32SpiP$Spi$read(void){
//#line 80
  unsigned char result;
//#line 80

//#line 80
  result = HplStm32SpiP$SPI$read();
//#line 80

//#line 80
  return result;
//#line 80
}
//#line 80
//# 144 "/opt/tinyos-2.x/tos/chips/stm32/spi/Stm32SpiP.nc"
static __inline   uint8_t Stm32SpiP$FastSpiByte$splitRead(void)
//#line 144
{
  while (SPI_I2S_GetFlagStatus((SPI_TypeDef *)((uint32_t )0x40000000 + 0x10000 + 0x3000), (uint16_t )0x0001) == RESET) 
    ;
  return Stm32SpiP$Spi$read();
}

//# 52 "/opt/tinyos-2.x/tos/interfaces/FastSpiByte.nc"
inline static   uint8_t RF230DriverLayerP$FastSpiByte$splitRead(void){
//#line 52
  unsigned char result;
//#line 52

//#line 52
  result = Stm32SpiP$FastSpiByte$splitRead();
//#line 52

//#line 52
  return result;
//#line 52
}
//#line 52
//# 49 "/opt/tinyos-2.x/tos/chips/stm32/spi/HplStm32SpiP.nc"
static inline   void HplStm32SpiP$SPI$write(uint8_t d)
//#line 49
{
//#line 49
  SPI_I2S_SendData((SPI_TypeDef *)((uint32_t )0x40000000 + 0x10000 + 0x3000), d);
}

//# 86 "/opt/tinyos-2.x/tos/chips/stm32/spi/Stm32Spi.nc"
inline static   void Stm32SpiP$Spi$write(uint8_t arg_0x40aec6b8){
//#line 86
  HplStm32SpiP$SPI$write(arg_0x40aec6b8);
//#line 86
}
//#line 86
//# 150 "/opt/tinyos-2.x/tos/chips/stm32/spi/Stm32SpiP.nc"
static __inline   uint8_t Stm32SpiP$FastSpiByte$splitReadWrite(uint8_t data)
//#line 150
{
  uint8_t b;

  while (SPI_I2S_GetFlagStatus((SPI_TypeDef *)((uint32_t )0x40000000 + 0x10000 + 0x3000), (uint16_t )0x0001) == RESET) 
    ;
  b = Stm32SpiP$Spi$read();
  Stm32SpiP$Spi$write(data);

  return b;
}

//# 58 "/opt/tinyos-2.x/tos/interfaces/FastSpiByte.nc"
inline static   uint8_t RF230DriverLayerP$FastSpiByte$splitReadWrite(uint8_t arg_0x40a32b88){
//#line 58
  unsigned char result;
//#line 58

//#line 58
  result = Stm32SpiP$FastSpiByte$splitReadWrite(arg_0x40a32b88);
//#line 58

//#line 58
  return result;
//#line 58
}
//#line 58
//# 140 "/opt/tinyos-2.x/tos/chips/stm32/spi/Stm32SpiP.nc"
static __inline   void Stm32SpiP$FastSpiByte$splitWrite(uint8_t data)
//#line 140
{
  Stm32SpiP$Spi$write(data);
}

//# 46 "/opt/tinyos-2.x/tos/interfaces/FastSpiByte.nc"
inline static   void RF230DriverLayerP$FastSpiByte$splitWrite(uint8_t arg_0x40a32250){
//#line 46
  Stm32SpiP$FastSpiByte$splitWrite(arg_0x40a32250);
//#line 46
}
//#line 46
//# 53 "/opt/tinyos-2.x/tos/chips/stm32/gpio/HplSTM32GeneralIOPinP.nc"
static __inline   void /*HplSTM32GeneralIOC.PortB.Bit0*/HplSTM32GeneralIOPinP$16$IO$clr(void)
//#line 53
{
  GPIO_TypeDef *port = (GPIO_TypeDef *)1073810432U;

//#line 55
  * (volatile int32_t *)((uint32_t )0x42000000 + ((uint32_t )& port->ODR - (uint32_t )0x40000000) * 32 + 0 * 4) = 0;
}

//# 30 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
inline static   void RF230DriverLayerP$SELN$clr(void){
//#line 30
  /*HplSTM32GeneralIOC.PortB.Bit0*/HplSTM32GeneralIOPinP$16$IO$clr();
//#line 30
}
//#line 30
//# 143 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230DriverLayerP.nc"
static __inline void RF230DriverLayerP$writeRegister(uint8_t reg, uint8_t value)
{
  for (; 0; ) ;
  for (; 0; ) ;

  RF230DriverLayerP$SELN$clr();
  RF230DriverLayerP$FastSpiByte$splitWrite(RF230_CMD_REGISTER_WRITE | reg);
  RF230DriverLayerP$FastSpiByte$splitReadWrite(value);
  RF230DriverLayerP$FastSpiByte$splitRead();
  RF230DriverLayerP$SELN$set();
}

//# 55 "/opt/tinyos-2.x/tos/lib/timer/BusyWait.nc"
inline static   void RF230DriverLayerP$BusyWait$wait(RF230DriverLayerP$BusyWait$size_type arg_0x40a2bf20){
//#line 55
  /*BusyWaitMicroC.Stm32BusyWaitMicro*/HalStm32BusyWaitM$0$BusyWait$wait(arg_0x40a2bf20);
//#line 55
}
//#line 55
//# 30 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
inline static   void RF230DriverLayerP$RSTN$clr(void){
//#line 30
  /*HplSTM32GeneralIOC.PortA.Bit6*/HplSTM32GeneralIOPinP$6$IO$clr();
//#line 30
}
//#line 30
//# 230 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230DriverLayerP.nc"
static inline void RF230DriverLayerP$initRadio(void)
{
  RF230DriverLayerP$BusyWait$wait(510);

  RF230DriverLayerP$RSTN$clr();
  RF230DriverLayerP$SLP_TR$clr();
  RF230DriverLayerP$BusyWait$wait(6);
  RF230DriverLayerP$RSTN$set();

  RF230DriverLayerP$writeRegister(RF230_TRX_CTRL_0, RF230_TRX_CTRL_0_VALUE);
  RF230DriverLayerP$writeRegister(RF230_TRX_STATE, RF230_TRX_OFF);

  RF230DriverLayerP$BusyWait$wait(510);

  RF230DriverLayerP$writeRegister(RF230_IRQ_MASK, ((RF230_IRQ_TRX_UR | RF230_IRQ_PLL_LOCK) | RF230_IRQ_TRX_END) | RF230_IRQ_RX_START);
  RF230DriverLayerP$writeRegister(RF230_CCA_THRES, RF230_CCA_THRES_VALUE);
  RF230DriverLayerP$writeRegister(RF230_PHY_TX_PWR, RF230_TX_AUTO_CRC_ON | (0 & RF230_TX_PWR_MASK));

  RF230DriverLayerP$txPower = 0 & RF230_TX_PWR_MASK;
  RF230DriverLayerP$channel = 11 & RF230_CHANNEL_MASK;
  RF230DriverLayerP$writeRegister(RF230_PHY_CC_CCA, RF230_CCA_MODE_VALUE | RF230DriverLayerP$channel);

  RF230DriverLayerP$SLP_TR$set();
  RF230DriverLayerP$state = RF230DriverLayerP$STATE_SLEEP;
}



static inline  void RF230DriverLayerP$SpiResource$granted(void)
{
  RF230DriverLayerP$SELN$makeOutput();
  RF230DriverLayerP$SELN$set();

  if (RF230DriverLayerP$state == RF230DriverLayerP$STATE_P_ON) 
    {
      RF230DriverLayerP$initRadio();
      RF230DriverLayerP$SpiResource$release();
    }
  else {
    RF230DriverLayerP$Tasklet$schedule();
    }
}

//# 372 "/opt/tinyos-2.x/tos/chips/stm32/spi/Stm32SpiP.nc"
static inline   void Stm32SpiP$Resource$default$granted(uint8_t id)
//#line 372
{
}

//# 92 "/opt/tinyos-2.x/tos/interfaces/Resource.nc"
inline static  void Stm32SpiP$Resource$granted(uint8_t arg_0x40af5b80){
//#line 92
  switch (arg_0x40af5b80) {
//#line 92
    case 0U:
//#line 92
      RF230DriverLayerP$SpiResource$granted();
//#line 92
      break;
//#line 92
    default:
//#line 92
      Stm32SpiP$Resource$default$granted(arg_0x40af5b80);
//#line 92
      break;
//#line 92
    }
//#line 92
}
//#line 92
//# 368 "/opt/tinyos-2.x/tos/chips/stm32/spi/Stm32SpiP.nc"
static inline  void Stm32SpiP$ResourceArbiter$granted(uint8_t id)
//#line 368
{
  Stm32SpiP$Resource$granted(id);
}

//# 92 "/opt/tinyos-2.x/tos/interfaces/Resource.nc"
inline static  void /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$Resource$granted(uint8_t arg_0x40b60c90){
//#line 92
  Stm32SpiP$ResourceArbiter$granted(arg_0x40b60c90);
//#line 92
}
//#line 92
//# 171 "/opt/tinyos-2.x/tos/system/SimpleArbiterP.nc"
static inline    void /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$ResourceConfigure$default$configure(uint8_t id)
//#line 171
{
}

//# 49 "/opt/tinyos-2.x/tos/interfaces/ResourceConfigure.nc"
inline static   void /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$ResourceConfigure$configure(uint8_t arg_0x40b5e188){
//#line 49
    /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$ResourceConfigure$default$configure(arg_0x40b5e188);
//#line 49
}
//#line 49
//# 155 "/opt/tinyos-2.x/tos/system/SimpleArbiterP.nc"
static inline  void /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$grantedTask$runTask(void)
//#line 155
{
  { __nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
//#line 156
    {
      /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$resId = /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$reqResId;
      /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$state = /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$RES_BUSY;
    }
//#line 159
    __nesc_atomic_end(__nesc_atomic); }
  /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$ResourceConfigure$configure(/*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$resId);
  /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$Resource$granted(/*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$resId);
}

//# 221 "/opt/tinyos-2.x/tos/chips/stm32/timer/STM32RtcC.nc"
static inline   uint32_t STM32RtcC$Alarm$getNow(void)
{
  uint32_t c;

//#line 224
  c = RTC_GetCounter();
  return c;
}






static inline   uint32_t STM32RtcC$Counter$get(void)
{
  return STM32RtcC$Alarm$getNow();
}

//# 53 "/opt/tinyos-2.x/tos/lib/timer/Counter.nc"
inline static   /*BusyWaitMicroC.Stm32BusyWaitMicro*/HalStm32BusyWaitM$0$OST$size_type /*BusyWaitMicroC.Stm32BusyWaitMicro*/HalStm32BusyWaitM$0$OST$get(void){
//#line 53
  unsigned long result;
//#line 53

//#line 53
  result = STM32RtcC$Counter$get();
//#line 53

//#line 53
  return result;
//#line 53
}
//#line 53
//# 173 "/opt/tinyos-2.x/tos/system/SimpleArbiterP.nc"
static inline    void /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$ResourceConfigure$default$unconfigure(uint8_t id)
//#line 173
{
}

//# 55 "/opt/tinyos-2.x/tos/interfaces/ResourceConfigure.nc"
inline static   void /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$ResourceConfigure$unconfigure(uint8_t arg_0x40b5e188){
//#line 55
    /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$ResourceConfigure$default$unconfigure(arg_0x40b5e188);
//#line 55
}
//#line 55
//# 56 "/opt/tinyos-2.x/tos/interfaces/TaskBasic.nc"
inline static   error_t /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$grantedTask$postTask(void){
//#line 56
  unsigned char result;
//#line 56

//#line 56
  result = SchedulerBasicP$TaskBasic$postTask(/*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$grantedTask);
//#line 56

//#line 56
  return result;
//#line 56
}
//#line 56
//# 58 "/opt/tinyos-2.x/tos/system/FcfsResourceQueueC.nc"
static inline   resource_client_id_t /*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$FcfsQueue$dequeue(void)
//#line 58
{
  /* atomic removed: atomic calls only */
//#line 59
  {
    if (/*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$qHead != /*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$NO_ENTRY) {
        uint8_t id = /*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$qHead;

//#line 62
        /*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$qHead = /*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$resQ[/*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$qHead];
        if (/*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$qHead == /*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$NO_ENTRY) {
          /*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$qTail = /*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$NO_ENTRY;
          }
//#line 65
        /*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$resQ[id] = /*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$NO_ENTRY;
        {
          unsigned char __nesc_temp = 
//#line 66
          id;

//#line 66
          return __nesc_temp;
        }
      }
//#line 68
    {
      unsigned char __nesc_temp = 
//#line 68
      /*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$NO_ENTRY;

//#line 68
      return __nesc_temp;
    }
  }
}

//# 60 "/opt/tinyos-2.x/tos/interfaces/ResourceQueue.nc"
inline static   resource_client_id_t /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$Queue$dequeue(void){
//#line 60
  unsigned char result;
//#line 60

//#line 60
  result = /*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$FcfsQueue$dequeue();
//#line 60

//#line 60
  return result;
//#line 60
}
//#line 60
//# 50 "/opt/tinyos-2.x/tos/system/FcfsResourceQueueC.nc"
static inline   bool /*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$FcfsQueue$isEmpty(void)
//#line 50
{
  return /*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$qHead == /*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$NO_ENTRY;
}

//# 43 "/opt/tinyos-2.x/tos/interfaces/ResourceQueue.nc"
inline static   bool /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$Queue$isEmpty(void){
//#line 43
  unsigned char result;
//#line 43

//#line 43
  result = /*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$FcfsQueue$isEmpty();
//#line 43

//#line 43
  return result;
//#line 43
}
//#line 43
//# 97 "/opt/tinyos-2.x/tos/system/SimpleArbiterP.nc"
static inline   error_t /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$Resource$release(uint8_t id)
//#line 97
{
  bool released = FALSE;

//#line 99
  { __nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
//#line 99
    {
      if (/*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$state == /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$RES_BUSY && /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$resId == id) {
          if (/*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$Queue$isEmpty() == FALSE) {
              /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$resId = /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$NO_RES;
              /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$reqResId = /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$Queue$dequeue();
              /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$state = /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$RES_GRANTING;
              /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$grantedTask$postTask();
            }
          else {
              /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$resId = /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$NO_RES;
              /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$state = /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$RES_IDLE;
            }
          released = TRUE;
        }
    }
//#line 113
    __nesc_atomic_end(__nesc_atomic); }
  if (released == TRUE) {
      /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$ResourceConfigure$unconfigure(id);
      return SUCCESS;
    }
  return FAIL;
}

//# 110 "/opt/tinyos-2.x/tos/interfaces/Resource.nc"
inline static   error_t Stm32SpiP$ResourceArbiter$release(uint8_t arg_0x40af20f8){
//#line 110
  unsigned char result;
//#line 110

//#line 110
  result = /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$Resource$release(arg_0x40af20f8);
//#line 110

//#line 110
  return result;
//#line 110
}
//#line 110
//# 80 "/opt/tinyos-2.x/tos/interfaces/ArbiterInfo.nc"
inline static   bool Stm32SpiP$ArbiterInfo$inUse(void){
//#line 80
  unsigned char result;
//#line 80

//#line 80
  result = /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$ArbiterInfo$inUse();
//#line 80

//#line 80
  return result;
//#line 80
}
//#line 80
//# 158 "/opt/tinyos-2.x/tos/chips/stm32/McuSleepC.nc"
static inline   void McuSleepC$McuPowerState$update(void)
//#line 158
{

  return;
}

//# 44 "/opt/tinyos-2.x/tos/interfaces/McuPowerState.nc"
inline static   void Stm32SpiP$McuPowerState$update(void){
//#line 44
  McuSleepC$McuPowerState$update();
//#line 44
}
//#line 44
//# 44 "/opt/tinyos-2.x/tos/chips/stm32/spi/HplStm32SpiP.nc"
static inline   void HplStm32SpiP$SPI$sleep(void)
//#line 44
{
}

//# 72 "/opt/tinyos-2.x/tos/chips/stm32/spi/Stm32Spi.nc"
inline static   void Stm32SpiP$Spi$sleep(void){
//#line 72
  HplStm32SpiP$SPI$sleep();
//#line 72
}
//#line 72
//# 103 "/opt/tinyos-2.x/tos/chips/stm32/spi/HplStm32SpiP.nc"
static inline   void HplStm32SpiP$SPI$enableSpi(bool enabled)
//#line 103
{
  if (enabled) {


      SPI_Cmd((SPI_TypeDef *)((uint32_t )0x40000000 + 0x10000 + 0x3000), ENABLE);
    }
  else {


      SPI_Cmd((SPI_TypeDef *)((uint32_t )0x40000000 + 0x10000 + 0x3000), DISABLE);
    }
}

//# 99 "/opt/tinyos-2.x/tos/chips/stm32/spi/Stm32Spi.nc"
inline static   void Stm32SpiP$Spi$enableSpi(bool arg_0x40aeb8f8){
//#line 99
  HplStm32SpiP$SPI$enableSpi(arg_0x40aeb8f8);
//#line 99
}
//#line 99
//# 122 "/opt/tinyos-2.x/tos/chips/stm32/spi/Stm32SpiP.nc"
static inline void Stm32SpiP$stopSpi(void)
//#line 122
{
  Stm32SpiP$Spi$enableSpi(FALSE);
  /* atomic removed: atomic calls only */
//#line 124
  {
    Stm32SpiP$Spi$sleep();
  }
  Stm32SpiP$McuPowerState$update();
}

//# 273 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230ActiveMessageP.nc"
static inline   void RF230ActiveMessageP$RadioAlarm$fired(void)
{
}

//# 45 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioSend.nc"
inline static   void RandomCollisionLayerP$RadioSend$sendDone(error_t arg_0x40937850){
//#line 45
  MessageBufferLayerP$RadioSend$sendDone(arg_0x40937850);
//#line 45
}
//#line 45
//# 252 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230ActiveMessageP.nc"
static inline   uint16_t RF230ActiveMessageP$RandomCollisionConfig$getCongestionBackoff(message_t *msg)
{
  return (uint16_t )(2240 * (7372800UL / 1 / 32) * (1 << 2) / 1000000UL);
}

//# 35 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/RandomCollisionConfig.nc"
inline static   uint16_t RandomCollisionLayerP$Config$getCongestionBackoff(message_t *arg_0x407afb60){
//#line 35
  unsigned short result;
//#line 35

//#line 35
  result = RF230ActiveMessageP$RandomCollisionConfig$getCongestionBackoff(arg_0x407afb60);
//#line 35

//#line 35
  return result;
//#line 35
}
//#line 35
//# 73 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230ActiveMessageP.nc"
static inline   uint8_t RF230ActiveMessageP$RF230DriverConfig$metadataLength(message_t *msg)
{
  return 0;
}

//# 41 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230DriverConfig.nc"
inline static   uint8_t RF230DriverLayerP$Config$metadataLength(message_t *arg_0x407a8ae0){
//#line 41
  unsigned char result;
//#line 41

//#line 41
  result = RF230ActiveMessageP$RF230DriverConfig$metadataLength(arg_0x407a8ae0);
//#line 41

//#line 41
  return result;
//#line 41
}
//#line 41
//# 880 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230DriverLayerP.nc"
static inline   uint8_t RF230DriverLayerP$RadioPacket$metadataLength(message_t *msg)
{
  return RF230DriverLayerP$Config$metadataLength(msg) + sizeof(rf230_metadata_t );
}

//# 54 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioPacket.nc"
inline static   uint8_t MetadataFlagsLayerC$SubPacket$metadataLength(message_t *arg_0x407dc1b0){
//#line 54
  unsigned char result;
//#line 54

//#line 54
  result = RF230DriverLayerP$RadioPacket$metadataLength(arg_0x407dc1b0);
//#line 54

//#line 54
  return result;
//#line 54
}
//#line 54
//# 99 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/MetadataFlagsLayerC.nc"
static inline   uint8_t MetadataFlagsLayerC$RadioPacket$metadataLength(message_t *msg)
{
  return MetadataFlagsLayerC$SubPacket$metadataLength(msg) + sizeof(flags_metadata_t );
}

//#line 43
static inline flags_metadata_t *MetadataFlagsLayerC$getMeta(message_t *msg)
{
  return (flags_metadata_t *) ((void *)msg + sizeof(message_t ) - MetadataFlagsLayerC$RadioPacket$metadataLength(msg));
}

//#line 62
static inline   void MetadataFlagsLayerC$PacketFlag$clear(uint8_t bit, message_t *msg)
{
  for (; 0; ) ;

  MetadataFlagsLayerC$getMeta(msg)->flags &= ~(1 << bit);
}

//# 44 "/opt/tinyos-2.x/tos/chips/rf2xx/util/PacketFlag.nc"
inline static   void SoftwareAckLayerP$AckReceivedFlag$clear(message_t *arg_0x409bbba0){
//#line 44
  MetadataFlagsLayerC$PacketFlag$clear(0U, arg_0x409bbba0);
//#line 44
}
//#line 44
//# 37 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioSend.nc"
inline static   error_t SoftwareAckLayerP$SubSend$send(message_t *arg_0x40937190){
//#line 37
  unsigned char result;
//#line 37

//#line 37
  result = RF230DriverLayerP$RadioSend$send(arg_0x40937190);
//#line 37

//#line 37
  return result;
//#line 37
}
//#line 37
//# 67 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/SoftwareAckLayerP.nc"
static inline   error_t SoftwareAckLayerP$RadioSend$send(message_t *msg)
{
  error_t error;

  if (SoftwareAckLayerP$state == SoftwareAckLayerP$STATE_READY) 
    {
      if ((error = SoftwareAckLayerP$SubSend$send(msg)) == SUCCESS) 
        {
          SoftwareAckLayerP$AckReceivedFlag$clear(msg);
          SoftwareAckLayerP$state = SoftwareAckLayerP$STATE_DATA_SEND;
          SoftwareAckLayerP$txMsg = msg;
        }
    }
  else {
    error = EBUSY;
    }
  return error;
}

//# 37 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioSend.nc"
inline static   error_t RandomCollisionLayerP$SubSend$send(message_t *arg_0x40937190){
//#line 37
  unsigned char result;
//#line 37

//#line 37
  result = SoftwareAckLayerP$RadioSend$send(arg_0x40937190);
//#line 37

//#line 37
  return result;
//#line 37
}
//#line 37
//# 56 "/opt/tinyos-2.x/tos/interfaces/TaskBasic.nc"
inline static   error_t MessageBufferLayerP$sendTask$postTask(void){
//#line 56
  unsigned char result;
//#line 56

//#line 56
  result = SchedulerBasicP$TaskBasic$postTask(MessageBufferLayerP$sendTask);
//#line 56

//#line 56
  return result;
//#line 56
}
//#line 56
//# 222 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/MessageBufferLayerP.nc"
static inline   void MessageBufferLayerP$RadioSend$ready(void)
{
  if (MessageBufferLayerP$state == MessageBufferLayerP$STATE_TX_PENDING) {
    MessageBufferLayerP$sendTask$postTask();
    }
}

//# 52 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioSend.nc"
inline static   void RandomCollisionLayerP$RadioSend$ready(void){
//#line 52
  MessageBufferLayerP$RadioSend$ready();
//#line 52
}
//#line 52
//# 26 "/opt/tinyos-2.x/tos/chips/stm32/timer/HplStm32Timer3P.nc"
static inline   uint16_t HplStm32Timer3P$Timer$get(void)
//#line 26
{
//#line 26
  return TIM_GetCounter((TIM_TypeDef *)((uint32_t )0x40000000 + 0x0400));
}

//# 52 "/opt/tinyos-2.x/tos/chips/stm32/timer/HplStm32Timer.nc"
inline static   /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$HplStm32Timer$timer_size /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$HplStm32Timer$get(void){
//#line 52
  unsigned short result;
//#line 52

//#line 52
  result = HplStm32Timer3P$Timer$get();
//#line 52

//#line 52
  return result;
//#line 52
}
//#line 52
//# 53 "/opt/tinyos-2.x/tos/chips/stm32/timer/Stm32AlarmC.nc"
static inline   /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$timer_size /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$Alarm$getNow(void)
//#line 53
{
  return /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$HplStm32Timer$get();
}

//# 98 "/opt/tinyos-2.x/tos/lib/timer/Alarm.nc"
inline static   RadioAlarmP$Alarm$size_type RadioAlarmP$Alarm$getNow(void){
//#line 98
  unsigned short result;
//#line 98

//#line 98
  result = /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$Alarm$getNow();
//#line 98

//#line 98
  return result;
//#line 98
}
//#line 98
//# 64 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioAlarmP.nc"
static __inline   uint16_t RadioAlarmP$RadioAlarm$getNow(uint8_t id)
{
  return RadioAlarmP$Alarm$getNow();
}

//# 53 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioAlarm.nc"
inline static   uint16_t RandomCollisionLayerP$RadioAlarm$getNow(void){
//#line 53
  unsigned short result;
//#line 53

//#line 53
  result = RadioAlarmP$RadioAlarm$getNow(1U);
//#line 53

//#line 53
  return result;
//#line 53
}
//#line 53
//# 99 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/RandomCollisionLayerP.nc"
static inline   void RandomCollisionLayerP$RadioAlarm$fired(void)
{
  error_t error;
  int16_t delay;

  for (; 0; ) ;

  delay = (int16_t )RandomCollisionLayerP$txBarrier - RandomCollisionLayerP$RadioAlarm$getNow();

  if (RandomCollisionLayerP$state == RandomCollisionLayerP$STATE_BARRIER) 
    {
      RandomCollisionLayerP$state = RandomCollisionLayerP$STATE_READY;

      RandomCollisionLayerP$RadioSend$ready();
      return;
    }
  else {
//#line 115
    if (RandomCollisionLayerP$state & RandomCollisionLayerP$STATE_BARRIER && delay > 0) {
      error = EBUSY;
      }
    else {
//#line 118
      error = RandomCollisionLayerP$SubSend$send(RandomCollisionLayerP$txMsg);
      }
    }
//#line 120
  if (error != SUCCESS) 
    {
      if ((RandomCollisionLayerP$state & ~RandomCollisionLayerP$STATE_BARRIER) == RandomCollisionLayerP$STATE_TX_PENDING_FIRST) 
        {
          RandomCollisionLayerP$state = (RandomCollisionLayerP$state & RandomCollisionLayerP$STATE_BARRIER) | RandomCollisionLayerP$STATE_TX_PENDING_SECOND;
          RandomCollisionLayerP$RadioAlarm$wait(RandomCollisionLayerP$getBackoff(RandomCollisionLayerP$Config$getCongestionBackoff(RandomCollisionLayerP$txMsg)));
        }
      else 
        {
          if (RandomCollisionLayerP$state & RandomCollisionLayerP$STATE_BARRIER && delay > 0) 
            {
              RandomCollisionLayerP$state = RandomCollisionLayerP$STATE_BARRIER;
              RandomCollisionLayerP$RadioAlarm$wait(delay);
            }
          else {
            RandomCollisionLayerP$state = RandomCollisionLayerP$STATE_READY;
            }
          RandomCollisionLayerP$RadioSend$sendDone(error);
        }
    }
  else {
    RandomCollisionLayerP$state = RandomCollisionLayerP$STATE_TX_SENDING;
    }
}

//#line 144
static inline   void RandomCollisionLayerP$SubSend$sendDone(error_t error)
{
  for (; 0; ) ;

  RandomCollisionLayerP$state = RandomCollisionLayerP$STATE_READY;
  RandomCollisionLayerP$RadioSend$sendDone(error);
}

//# 45 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioSend.nc"
inline static   void SoftwareAckLayerP$RadioSend$sendDone(error_t arg_0x40937850){
//#line 45
  RandomCollisionLayerP$SubSend$sendDone(arg_0x40937850);
//#line 45
}
//#line 45
//# 130 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230ActiveMessageP.nc"
static inline   void RF230ActiveMessageP$SoftwareAckConfig$reportChannelError(void)
{
}

//# 75 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/SoftwareAckConfig.nc"
inline static   void SoftwareAckLayerP$SoftwareAckConfig$reportChannelError(void){
//#line 75
  RF230ActiveMessageP$SoftwareAckConfig$reportChannelError();
//#line 75
}
//#line 75
//# 113 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/SoftwareAckLayerP.nc"
static inline   void SoftwareAckLayerP$RadioAlarm$fired(void)
{
  for (; 0; ) ;

  SoftwareAckLayerP$SoftwareAckConfig$reportChannelError();

  SoftwareAckLayerP$state = SoftwareAckLayerP$STATE_READY;
  SoftwareAckLayerP$RadioSend$sendDone(SUCCESS);
}

//# 574 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230DriverLayerP.nc"
static inline    void RF230DriverLayerP$RadioCCA$default$done(error_t error)
//#line 574
{
}

//# 41 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioCCA.nc"
inline static   void RF230DriverLayerP$RadioCCA$done(error_t arg_0x40953238){
//#line 41
  RF230DriverLayerP$RadioCCA$default$done(arg_0x40953238);
//#line 41
}
//#line 41
//# 155 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230DriverLayerP.nc"
static __inline uint8_t RF230DriverLayerP$readRegister(uint8_t reg)
{
  for (; 0; ) ;
  for (; 0; ) ;

  RF230DriverLayerP$SELN$clr();
  RF230DriverLayerP$FastSpiByte$splitWrite(RF230_CMD_REGISTER_READ | reg);
  RF230DriverLayerP$FastSpiByte$splitReadWrite(0);
  reg = RF230DriverLayerP$FastSpiByte$splitRead();
  RF230DriverLayerP$SELN$set();

  return reg;
}

//#line 180
static inline   void RF230DriverLayerP$RadioAlarm$fired(void)
{
  if (RF230DriverLayerP$state == RF230DriverLayerP$STATE_SLEEP_2_TRX_OFF) {
    RF230DriverLayerP$state = RF230DriverLayerP$STATE_TRX_OFF;
    }
  else {
//#line 184
    if (RF230DriverLayerP$cmd == RF230DriverLayerP$CMD_CCA) 
      {
        uint8_t cca;

        for (; 0; ) ;

        RF230DriverLayerP$cmd = RF230DriverLayerP$CMD_NONE;
        cca = RF230DriverLayerP$readRegister(RF230_TRX_STATUS);

        for (; 0; ) ;

        RF230DriverLayerP$RadioCCA$done(cca & RF230_CCA_DONE ? cca & RF230_CCA_STATUS ? SUCCESS : EBUSY : FAIL);
      }
    else {
      for (; 0; ) ;
      }
    }
  RF230DriverLayerP$Tasklet$schedule();
}

//# 78 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioAlarmP.nc"
static inline    void RadioAlarmP$RadioAlarm$default$fired(uint8_t id)
{
}

//# 48 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioAlarm.nc"
inline static   void RadioAlarmP$RadioAlarm$fired(uint8_t arg_0x4080dc60){
//#line 48
  switch (arg_0x4080dc60) {
//#line 48
    case 0U:
//#line 48
      RF230ActiveMessageP$RadioAlarm$fired();
//#line 48
      break;
//#line 48
    case 1U:
//#line 48
      RandomCollisionLayerP$RadioAlarm$fired();
//#line 48
      break;
//#line 48
    case 2U:
//#line 48
      SoftwareAckLayerP$RadioAlarm$fired();
//#line 48
      break;
//#line 48
    case 3U:
//#line 48
      RF230DriverLayerP$RadioAlarm$fired();
//#line 48
      break;
//#line 48
    default:
//#line 48
      RadioAlarmP$RadioAlarm$default$fired(arg_0x4080dc60);
//#line 48
      break;
//#line 48
    }
//#line 48
}
//#line 48
//# 69 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioAlarmP.nc"
static inline   void RadioAlarmP$Tasklet$run(void)
{
  if (RadioAlarmP$state == RadioAlarmP$STATE_FIRED) 
    {
      RadioAlarmP$state = RadioAlarmP$STATE_READY;
      RadioAlarmP$RadioAlarm$fired(RadioAlarmP$alarm);
    }
}

//# 228 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/MessageBufferLayerP.nc"
static inline   void MessageBufferLayerP$Tasklet$run(void)
{
}

//# 82 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioAlarmP.nc"
static __inline   bool RadioAlarmP$RadioAlarm$isFree(uint8_t id)
{
  return RadioAlarmP$state == RadioAlarmP$STATE_READY;
}

//# 33 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioAlarm.nc"
inline static   bool RandomCollisionLayerP$RadioAlarm$isFree(void){
//#line 33
  unsigned char result;
//#line 33

//#line 33
  result = RadioAlarmP$RadioAlarm$isFree(1U);
//#line 33

//#line 33
  return result;
//#line 33
}
//#line 33
//# 60 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/RandomCollisionLayerP.nc"
static inline   void RandomCollisionLayerP$SubSend$ready(void)
{
  if (RandomCollisionLayerP$state == RandomCollisionLayerP$STATE_READY && RandomCollisionLayerP$RadioAlarm$isFree()) {
    RandomCollisionLayerP$RadioSend$ready();
    }
}

//# 52 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioSend.nc"
inline static   void SoftwareAckLayerP$RadioSend$ready(void){
//#line 52
  RandomCollisionLayerP$SubSend$ready();
//#line 52
}
//#line 52
//# 61 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/SoftwareAckLayerP.nc"
static inline   void SoftwareAckLayerP$SubSend$ready(void)
{
  if (SoftwareAckLayerP$state == SoftwareAckLayerP$STATE_READY) {
    SoftwareAckLayerP$RadioSend$ready();
    }
}

//# 52 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioSend.nc"
inline static   void RF230DriverLayerP$RadioSend$ready(void){
//#line 52
  SoftwareAckLayerP$SubSend$ready();
//#line 52
}
//#line 52
//# 56 "/opt/tinyos-2.x/tos/interfaces/TaskBasic.nc"
inline static   error_t MessageBufferLayerP$stateDoneTask$postTask(void){
//#line 56
  unsigned char result;
//#line 56

//#line 56
  result = SchedulerBasicP$TaskBasic$postTask(MessageBufferLayerP$stateDoneTask);
//#line 56

//#line 56
  return result;
//#line 56
}
//#line 56
//# 144 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/MessageBufferLayerP.nc"
static inline   void MessageBufferLayerP$RadioState$done(void)
{
  MessageBufferLayerP$stateDoneTask$postTask();
}

//# 58 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioState.nc"
inline static   void RF230DriverLayerP$RadioState$done(void){
//#line 58
  MessageBufferLayerP$RadioState$done();
//#line 58
}
//#line 58
//# 312 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230DriverLayerP.nc"
static __inline void RF230DriverLayerP$changeChannel(void)
{
  for (; 0; ) ;
  for (; 0; ) ;

  if (RF230DriverLayerP$isSpiAcquired()) 
    {
      RF230DriverLayerP$writeRegister(RF230_PHY_CC_CCA, RF230_CCA_MODE_VALUE | RF230DriverLayerP$channel);

      if (RF230DriverLayerP$state == RF230DriverLayerP$STATE_RX_ON) {
        RF230DriverLayerP$state = RF230DriverLayerP$STATE_TRX_OFF_2_RX_ON;
        }
      else {
//#line 324
        RF230DriverLayerP$cmd = RF230DriverLayerP$CMD_SIGNAL_DONE;
        }
    }
}

//# 78 "/opt/tinyos-2.x/tos/platforms/stm32-p103/chips/rf230/HplRF230P.nc"
static inline   void HplRF230P$IRQ$disable(void)
{
  HplRF230P$Capture$stop();
}

//# 55 "/opt/tinyos-2.x/tos/interfaces/GpioCapture.nc"
inline static   void RF230DriverLayerP$IRQ$disable(void){
//#line 55
  HplRF230P$IRQ$disable();
//#line 55
}
//#line 55
//# 52 "/opt/tinyos-2.x/tos/chips/stm32/timer/HplStm32Timer3P.nc"
static inline   void HplStm32Timer3P$Capture$start(void)
//#line 52
{
}

//# 58 "/opt/tinyos-2.x/tos/chips/stm32/timer/HplStm32Capture.nc"
inline static   void HplRF230P$Capture$start(void){
//#line 58
  HplStm32Timer3P$Capture$start();
//#line 58
}
//#line 58
//# 47 "/opt/tinyos-2.x/tos/chips/stm32/timer/HplStm32Timer3P.nc"
static inline   void HplStm32Timer3P$Capture$reset(void)
//#line 47
{
}

//# 55 "/opt/tinyos-2.x/tos/chips/stm32/timer/HplStm32Capture.nc"
inline static   void HplRF230P$Capture$reset(void){
//#line 55
  HplStm32Timer3P$Capture$reset();
//#line 55
}
//#line 55
//# 34 "/opt/tinyos-2.x/tos/chips/stm32/timer/HplStm32Timer3P.nc"
static inline   void HplStm32Timer3P$Capture$setEdge(bool up)
//#line 34
{
}

//# 79 "/opt/tinyos-2.x/tos/chips/stm32/timer/HplStm32Capture.nc"
inline static   void HplRF230P$Capture$setEdge(bool arg_0x40aae830){
//#line 79
  HplStm32Timer3P$Capture$setEdge(arg_0x40aae830);
//#line 79
}
//#line 79
//# 63 "/opt/tinyos-2.x/tos/platforms/stm32-p103/chips/rf230/HplRF230P.nc"
static inline   error_t HplRF230P$IRQ$captureRisingEdge(void)
{
  HplRF230P$Capture$setEdge(TRUE);
  HplRF230P$Capture$reset();
  HplRF230P$Capture$start();

  return SUCCESS;
}

//# 42 "/opt/tinyos-2.x/tos/interfaces/GpioCapture.nc"
inline static   error_t RF230DriverLayerP$IRQ$captureRisingEdge(void){
//#line 42
  unsigned char result;
//#line 42

//#line 42
  result = HplRF230P$IRQ$captureRisingEdge();
//#line 42

//#line 42
  return result;
//#line 42
}
//#line 42
//# 69 "/opt/tinyos-2.x/tos/chips/stm32/timer/Stm32AlarmC.nc"
static inline   void /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$Alarm$start(/*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$timer_size dt)
{
  /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$Alarm$startAt(/*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$HplStm32Timer$get(), dt);
}

//# 55 "/opt/tinyos-2.x/tos/lib/timer/Alarm.nc"
inline static   void RadioAlarmP$Alarm$start(RadioAlarmP$Alarm$size_type arg_0x40653ce0){
//#line 55
  /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$Alarm$start(arg_0x40653ce0);
//#line 55
}
//#line 55
//# 87 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioAlarmP.nc"
static inline   void RadioAlarmP$RadioAlarm$wait(uint8_t id, uint16_t timeout)
{
  for (; 0; ) ;

  RadioAlarmP$alarm = id;
  RadioAlarmP$state = RadioAlarmP$STATE_WAIT;
  RadioAlarmP$Alarm$start(timeout);
}

//# 38 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioAlarm.nc"
inline static   void RF230DriverLayerP$RadioAlarm$wait(uint16_t arg_0x407c7958){
//#line 38
  RadioAlarmP$RadioAlarm$wait(3U, arg_0x407c7958);
//#line 38
}
//#line 38
//#line 33
inline static   bool RF230DriverLayerP$RadioAlarm$isFree(void){
//#line 33
  unsigned char result;
//#line 33

//#line 33
  result = RadioAlarmP$RadioAlarm$isFree(3U);
//#line 33

//#line 33
  return result;
//#line 33
}
//#line 33
//# 330 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230DriverLayerP.nc"
static __inline void RF230DriverLayerP$changeState(void)
{

  if ((
//#line 332
  RF230DriverLayerP$cmd == RF230DriverLayerP$CMD_STANDBY || RF230DriverLayerP$cmd == RF230DriverLayerP$CMD_TURNON)
   && RF230DriverLayerP$state == RF230DriverLayerP$STATE_SLEEP && RF230DriverLayerP$RadioAlarm$isFree()) 
    {
      RF230DriverLayerP$SLP_TR$clr();

      RF230DriverLayerP$RadioAlarm$wait(RF230DriverLayerP$SLEEP_WAKEUP_TIME);
      RF230DriverLayerP$state = RF230DriverLayerP$STATE_SLEEP_2_TRX_OFF;
    }
  else {
//#line 340
    if (RF230DriverLayerP$cmd == RF230DriverLayerP$CMD_TURNON && RF230DriverLayerP$state == RF230DriverLayerP$STATE_TRX_OFF && RF230DriverLayerP$isSpiAcquired()) 
      {
        for (; 0; ) ;

        RF230DriverLayerP$readRegister(RF230_IRQ_STATUS);
        RF230DriverLayerP$IRQ$captureRisingEdge();


        RF230DriverLayerP$writeRegister(RF230_PHY_CC_CCA, RF230_CCA_MODE_VALUE | RF230DriverLayerP$channel);

        RF230DriverLayerP$writeRegister(RF230_TRX_STATE, RF230_RX_ON);
        RF230DriverLayerP$state = RF230DriverLayerP$STATE_TRX_OFF_2_RX_ON;
      }
    else {
      if ((
//#line 353
      RF230DriverLayerP$cmd == RF230DriverLayerP$CMD_TURNOFF || RF230DriverLayerP$cmd == RF230DriverLayerP$CMD_STANDBY)
       && RF230DriverLayerP$state == RF230DriverLayerP$STATE_RX_ON && RF230DriverLayerP$isSpiAcquired()) 
        {
          RF230DriverLayerP$writeRegister(RF230_TRX_STATE, RF230_FORCE_TRX_OFF);

          RF230DriverLayerP$IRQ$disable();
          RF230DriverLayerP$radioIrq = FALSE;

          RF230DriverLayerP$state = RF230DriverLayerP$STATE_TRX_OFF;
        }
      }
    }
//#line 364
  if (RF230DriverLayerP$cmd == RF230DriverLayerP$CMD_TURNOFF && RF230DriverLayerP$state == RF230DriverLayerP$STATE_TRX_OFF) 
    {
      RF230DriverLayerP$SLP_TR$set();
      RF230DriverLayerP$state = RF230DriverLayerP$STATE_SLEEP;
      RF230DriverLayerP$cmd = RF230DriverLayerP$CMD_SIGNAL_DONE;
    }
  else {
//#line 370
    if (RF230DriverLayerP$cmd == RF230DriverLayerP$CMD_STANDBY && RF230DriverLayerP$state == RF230DriverLayerP$STATE_TRX_OFF) {
      RF230DriverLayerP$cmd = RF230DriverLayerP$CMD_SIGNAL_DONE;
      }
    }
}

//# 251 "/usr/lib/ncc/nesc_nx.h"
static __inline uint8_t __nesc_hton_leuint8(void *target, uint8_t value)
//#line 251
{
  uint8_t *base = (uint8_t *) target;

//#line 253
  base[0] = value;
  return value;
}

//# 56 "/opt/tinyos-2.x/tos/interfaces/TaskBasic.nc"
inline static   error_t MessageBufferLayerP$deliverTask$postTask(void){
//#line 56
  unsigned char result;
//#line 56

//#line 56
  result = SchedulerBasicP$TaskBasic$postTask(MessageBufferLayerP$deliverTask);
//#line 56

//#line 56
  return result;
//#line 56
}
//#line 56
//# 309 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/MessageBufferLayerP.nc"
static inline   message_t *MessageBufferLayerP$RadioReceive$receive(message_t *msg)
{
  message_t *m;

  { __nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
    {
      if (MessageBufferLayerP$receiveQueueSize >= MessageBufferLayerP$RECEIVE_QUEUE_SIZE) {
        m = msg;
        }
      else {
          uint8_t index = MessageBufferLayerP$receiveQueueHead + MessageBufferLayerP$receiveQueueSize;

//#line 320
          if (index >= MessageBufferLayerP$RECEIVE_QUEUE_SIZE) {
            index -= MessageBufferLayerP$RECEIVE_QUEUE_SIZE;
            }
          m = MessageBufferLayerP$receiveQueue[index];
          MessageBufferLayerP$receiveQueue[index] = msg;

          ++MessageBufferLayerP$receiveQueueSize;
          MessageBufferLayerP$deliverTask$postTask();
        }
    }
//#line 329
    __nesc_atomic_end(__nesc_atomic); }

  return m;
}

//# 42 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioReceive.nc"
inline static   message_t *UniqueLayerP$RadioReceive$receive(message_t *arg_0x408eec28){
//#line 42
  nx_struct message_t *result;
//#line 42

//#line 42
  result = MessageBufferLayerP$RadioReceive$receive(arg_0x408eec28);
//#line 42

//#line 42
  return result;
//#line 42
}
//#line 42
//# 152 "/opt/tinyos-2.x/tos/chips/rf2xx/util/NeighborhoodP.nc"
static __inline   void NeighborhoodP$NeighborhoodFlag$set(uint8_t bit, uint8_t index)
{
  NeighborhoodP$flags[index] |= 1 << bit;
}

//# 40 "/opt/tinyos-2.x/tos/chips/rf2xx/util/NeighborhoodFlag.nc"
inline static   void UniqueLayerP$NeighborhoodFlag$set(uint8_t arg_0x408ffae8){
//#line 40
  NeighborhoodP$NeighborhoodFlag$set(0U, arg_0x408ffae8);
//#line 40
}
//#line 40
//# 154 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230ActiveMessageP.nc"
static inline   void RF230ActiveMessageP$UniqueConfig$reportChannelError(void)
{
}

//# 47 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/UniqueConfig.nc"
inline static   void UniqueLayerP$UniqueConfig$reportChannelError(void){
//#line 47
  RF230ActiveMessageP$UniqueConfig$reportChannelError();
//#line 47
}
//#line 47
//# 147 "/opt/tinyos-2.x/tos/chips/rf2xx/util/NeighborhoodP.nc"
static __inline   bool NeighborhoodP$NeighborhoodFlag$get(uint8_t bit, uint8_t index)
{
  return NeighborhoodP$flags[index] & (1 << bit);
}

//# 35 "/opt/tinyos-2.x/tos/chips/rf2xx/util/NeighborhoodFlag.nc"
inline static   bool UniqueLayerP$NeighborhoodFlag$get(uint8_t arg_0x408ff610){
//#line 35
  unsigned char result;
//#line 35

//#line 35
  result = NeighborhoodP$NeighborhoodFlag$get(0U, arg_0x408ff610);
//#line 35

//#line 35
  return result;
//#line 35
}
//#line 35
//# 246 "/usr/lib/ncc/nesc_nx.h"
static __inline uint8_t __nesc_ntoh_leuint8(const void *source)
//#line 246
{
  const uint8_t *base =(const uint8_t *) source;

//#line 248
  return base[0];
}

//# 63 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230ActiveMessageP.nc"
static inline   uint8_t RF230ActiveMessageP$RF230DriverConfig$headerLength(message_t *msg)
{
  return (unsigned int )& ((message_t *)0)->data - sizeof(rf230packet_header_t );
}

//# 29 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230DriverConfig.nc"
inline static   uint8_t RF230DriverLayerP$Config$headerLength(message_t *arg_0x407a8228){
//#line 29
  unsigned char result;
//#line 29

//#line 29
  result = RF230ActiveMessageP$RF230DriverConfig$headerLength(arg_0x407a8228);
//#line 29

//#line 29
  return result;
//#line 29
}
//#line 29
//# 854 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230DriverLayerP.nc"
static inline   uint8_t RF230DriverLayerP$RadioPacket$headerLength(message_t *msg)
{
  return RF230DriverLayerP$Config$headerLength(msg) + sizeof(rf230_header_t );
}

//# 32 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioPacket.nc"
inline static   uint8_t MetadataFlagsLayerC$SubPacket$headerLength(message_t *arg_0x407c9c68){
//#line 32
  unsigned char result;
//#line 32

//#line 32
  result = RF230DriverLayerP$RadioPacket$headerLength(arg_0x407c9c68);
//#line 32

//#line 32
  return result;
//#line 32
}
//#line 32
//# 79 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/MetadataFlagsLayerC.nc"
static inline   uint8_t MetadataFlagsLayerC$RadioPacket$headerLength(message_t *msg)
{
  return MetadataFlagsLayerC$SubPacket$headerLength(msg);
}

//# 32 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioPacket.nc"
inline static   uint8_t TimeStampingLayerP$SubPacket$headerLength(message_t *arg_0x407c9c68){
//#line 32
  unsigned char result;
//#line 32

//#line 32
  result = MetadataFlagsLayerC$RadioPacket$headerLength(arg_0x407c9c68);
//#line 32

//#line 32
  return result;
//#line 32
}
//#line 32
//# 105 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/TimeStampingLayerP.nc"
static inline   uint8_t TimeStampingLayerP$RadioPacket$headerLength(message_t *msg)
{
  return TimeStampingLayerP$SubPacket$headerLength(msg);
}

//# 32 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioPacket.nc"
inline static   uint8_t IEEE154MessageLayerP$SubPacket$headerLength(message_t *arg_0x407c9c68){
//#line 32
  unsigned char result;
//#line 32

//#line 32
  result = TimeStampingLayerP$RadioPacket$headerLength(arg_0x407c9c68);
//#line 32

//#line 32
  return result;
//#line 32
}
//#line 32
//# 70 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/IEEE154MessageLayerP.nc"
static inline ieee154_header_t *IEEE154MessageLayerP$getHeader(message_t *msg)
{
  return (ieee154_header_t *) ((void *)msg + IEEE154MessageLayerP$SubPacket$headerLength(msg));
}

//#line 154
static inline   uint8_t IEEE154MessageLayerP$IEEE154MessageLayer$getDSN(message_t *msg)
{
  return __nesc_ntoh_leuint8((unsigned char *)&IEEE154MessageLayerP$getHeader(msg)->dsn);
}

//# 109 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/IEEE154MessageLayer.nc"
inline static   uint8_t RF230ActiveMessageP$IEEE154MessageLayer$getDSN(message_t *arg_0x407cf120){
//#line 109
  unsigned char result;
//#line 109

//#line 109
  result = IEEE154MessageLayerP$IEEE154MessageLayer$getDSN(arg_0x407cf120);
//#line 109

//#line 109
  return result;
//#line 109
}
//#line 109
//# 139 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230ActiveMessageP.nc"
static inline   uint8_t RF230ActiveMessageP$UniqueConfig$getSequenceNumber(message_t *msg)
{
  return RF230ActiveMessageP$IEEE154MessageLayer$getDSN(msg);
}

//# 31 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/UniqueConfig.nc"
inline static   uint8_t UniqueLayerP$UniqueConfig$getSequenceNumber(message_t *arg_0x407a0c70){
//#line 31
  unsigned char result;
//#line 31

//#line 31
  result = RF230ActiveMessageP$UniqueConfig$getSequenceNumber(arg_0x407a0c70);
//#line 31

//#line 31
  return result;
//#line 31
}
//#line 31
//# 276 "/usr/lib/ncc/nesc_nx.h"
static __inline uint16_t __nesc_ntoh_leuint16(const void *source)
//#line 276
{
  const uint8_t *base = (const uint8_t *) source;

//#line 278
  return ((uint16_t )base[1] << 8) | base[0];
}

//# 184 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/IEEE154MessageLayerP.nc"
static inline   uint16_t IEEE154MessageLayerP$IEEE154MessageLayer$getSrcAddr(message_t *msg)
{
  return __nesc_ntoh_leuint16((unsigned char *)&IEEE154MessageLayerP$getHeader(msg)->src);
}

//# 140 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/IEEE154MessageLayer.nc"
inline static   uint16_t RF230ActiveMessageP$IEEE154MessageLayer$getSrcAddr(message_t *arg_0x407cc538){
//#line 140
  unsigned short result;
//#line 140

//#line 140
  result = IEEE154MessageLayerP$IEEE154MessageLayer$getSrcAddr(arg_0x407cc538);
//#line 140

//#line 140
  return result;
//#line 140
}
//#line 140
//# 149 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230ActiveMessageP.nc"
static inline   am_addr_t RF230ActiveMessageP$UniqueConfig$getSender(message_t *msg)
{
  return RF230ActiveMessageP$IEEE154MessageLayer$getSrcAddr(msg);
}

//# 36 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/UniqueConfig.nc"
inline static   am_addr_t UniqueLayerP$UniqueConfig$getSender(message_t *arg_0x407b8190){
//#line 36
  unsigned short result;
//#line 36

//#line 36
  result = RF230ActiveMessageP$UniqueConfig$getSender(arg_0x407b8190);
//#line 36

//#line 36
  return result;
//#line 36
}
//#line 36
//# 105 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/UniqueLayerP.nc"
static inline   void UniqueLayerP$Neighborhood$evicted(uint8_t index)
//#line 105
{
}

//# 69 "/opt/tinyos-2.x/tos/chips/rf2xx/util/Neighborhood.nc"
inline static   void NeighborhoodP$Neighborhood$evicted(uint8_t arg_0x40902630){
//#line 69
  UniqueLayerP$Neighborhood$evicted(arg_0x40902630);
//#line 69
}
//#line 69
//# 83 "/opt/tinyos-2.x/tos/chips/rf2xx/util/NeighborhoodP.nc"
static inline  uint8_t  NeighborhoodP$Neighborhood$insertNode(am_addr_t node)
{
  uint8_t i;
  uint8_t maxAge;

  if (NeighborhoodP$nodes[NeighborhoodP$last] == node) 
    {
      if (NeighborhoodP$ages[NeighborhoodP$last] == NeighborhoodP$time) {
        return NeighborhoodP$last;
        }
      NeighborhoodP$ages[NeighborhoodP$last] = ++NeighborhoodP$time;
      maxAge = 0x80;
    }
  else 
    {
      uint8_t oldest = 0;

//#line 99
      maxAge = 0;

      for (i = 0; i < 5; ++i) 
        {
          uint8_t age;

          if (NeighborhoodP$nodes[i] == node) 
            {
              NeighborhoodP$last = i;
              if (NeighborhoodP$ages[i] == NeighborhoodP$time) {
                return i;
                }
              NeighborhoodP$ages[i] = ++NeighborhoodP$time;
              maxAge = 0x80;
              break;
            }

          age = NeighborhoodP$time - NeighborhoodP$ages[i];
          if (age > maxAge) 
            {
              maxAge = age;
              oldest = i;
            }
        }

      if (i == 5) 
        {
          NeighborhoodP$Neighborhood$evicted(oldest);

          NeighborhoodP$last = oldest;
          NeighborhoodP$nodes[oldest] = node;
          NeighborhoodP$ages[oldest] = ++NeighborhoodP$time;
          NeighborhoodP$flags[oldest] = 0;
        }
    }

  if ((NeighborhoodP$time & 0x7F) == 0x7F && maxAge >= 0x7F) 
    {
      for (i = 0; i < 5; ++i) 
        {
          if ((NeighborhoodP$ages[i] | 0x7F) != NeighborhoodP$time) {
            NeighborhoodP$ages[i] = NeighborhoodP$time & 0x80;
            }
        }
    }
  return NeighborhoodP$last;
}

//# 60 "/opt/tinyos-2.x/tos/chips/rf2xx/util/Neighborhood.nc"
inline static   uint8_t UniqueLayerP$Neighborhood$insertNode(am_addr_t arg_0x40902010){
//#line 60
  unsigned char result;
//#line 60

//#line 60
  result = NeighborhoodP$Neighborhood$insertNode(arg_0x40902010);
//#line 60

//#line 60
  return result;
//#line 60
}
//#line 60
//# 82 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/UniqueLayerP.nc"
static inline   message_t *UniqueLayerP$SubReceive$receive(message_t *msg)
{
  uint8_t index = UniqueLayerP$Neighborhood$insertNode(UniqueLayerP$UniqueConfig$getSender(msg));
  uint8_t dsn = UniqueLayerP$UniqueConfig$getSequenceNumber(msg);

  if (UniqueLayerP$NeighborhoodFlag$get(index)) 
    {
      uint8_t diff = dsn - UniqueLayerP$receivedNumbers[index];

      if (diff == 0) 
        {
          UniqueLayerP$UniqueConfig$reportChannelError();
          return msg;
        }
    }
  else {
    UniqueLayerP$NeighborhoodFlag$set(index);
    }
  UniqueLayerP$receivedNumbers[index] = dsn;

  return UniqueLayerP$RadioReceive$receive(msg);
}

//# 42 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioReceive.nc"
inline static   message_t *RandomCollisionLayerP$RadioReceive$receive(message_t *arg_0x408eec28){
//#line 42
  nx_struct message_t *result;
//#line 42

//#line 42
  result = UniqueLayerP$SubReceive$receive(arg_0x408eec28);
//#line 42

//#line 42
  return result;
//#line 42
}
//#line 42
//# 157 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/IEEE154MessageLayer.nc"
inline static   bool RF230ActiveMessageP$IEEE154MessageLayer$requiresAckReply(message_t *arg_0x407cb688){
//#line 157
  unsigned char result;
//#line 157

//#line 157
  result = IEEE154MessageLayerP$IEEE154MessageLayer$requiresAckReply(arg_0x407cb688);
//#line 157

//#line 157
  return result;
//#line 157
}
//#line 157
//# 53 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioAlarm.nc"
inline static   uint16_t RF230ActiveMessageP$RadioAlarm$getNow(void){
//#line 53
  unsigned short result;
//#line 53

//#line 53
  result = RadioAlarmP$RadioAlarm$getNow(0U);
//#line 53

//#line 53
  return result;
//#line 53
}
//#line 53
//# 257 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230ActiveMessageP.nc"
static inline   uint16_t RF230ActiveMessageP$RandomCollisionConfig$getTransmitBarrier(message_t *msg)
{
  uint16_t time;


  time = RF230ActiveMessageP$RadioAlarm$getNow();


  if (RF230ActiveMessageP$IEEE154MessageLayer$requiresAckReply(msg)) {
    time += (uint16_t )(32 * (-5 + 16 + 11 + 5) * (7372800UL / 1 / 32) * (1 << 2) / 1000000UL);
    }
  else {
//#line 268
    time += (uint16_t )(32 * (-5 + 5) * (7372800UL / 1 / 32) * (1 << 2) / 1000000UL);
    }
  return time;
}

//# 46 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/RandomCollisionConfig.nc"
inline static   uint16_t RandomCollisionLayerP$Config$getTransmitBarrier(message_t *arg_0x407ae450){
//#line 46
  unsigned short result;
//#line 46

//#line 46
  result = RF230ActiveMessageP$RandomCollisionConfig$getTransmitBarrier(arg_0x407ae450);
//#line 46

//#line 46
  return result;
//#line 46
}
//#line 46
//# 157 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/RandomCollisionLayerP.nc"
static inline   message_t *RandomCollisionLayerP$SubReceive$receive(message_t *msg)
{
  int16_t delay;

  RandomCollisionLayerP$txBarrier = RandomCollisionLayerP$Config$getTransmitBarrier(msg);
  delay = RandomCollisionLayerP$txBarrier - RandomCollisionLayerP$RadioAlarm$getNow();

  if (delay > 0) 
    {
      if (RandomCollisionLayerP$state == RandomCollisionLayerP$STATE_READY) 
        {
          RandomCollisionLayerP$RadioAlarm$wait(delay);
          RandomCollisionLayerP$state = RandomCollisionLayerP$STATE_BARRIER;
        }
      else {
        RandomCollisionLayerP$state |= RandomCollisionLayerP$STATE_BARRIER;
        }
    }
  return RandomCollisionLayerP$RadioReceive$receive(msg);
}

//# 42 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioReceive.nc"
inline static   message_t *SoftwareAckLayerP$RadioReceive$receive(message_t *arg_0x408eec28){
//#line 42
  nx_struct message_t *result;
//#line 42

//#line 42
  result = RandomCollisionLayerP$SubReceive$receive(arg_0x408eec28);
//#line 42

//#line 42
  return result;
//#line 42
}
//#line 42
//# 281 "/usr/lib/ncc/nesc_nx.h"
static __inline uint16_t __nesc_hton_leuint16(void *target, uint16_t value)
//#line 281
{
  uint8_t *base = (uint8_t *) target;

//#line 283
  base[0] = value;
  base[1] = value >> 8;
  return value;
}

//# 43 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioPacket.nc"
inline static   void MetadataFlagsLayerC$SubPacket$setPayloadLength(message_t *arg_0x407de7a8, uint8_t arg_0x407de930){
//#line 43
  RF230DriverLayerP$RadioPacket$setPayloadLength(arg_0x407de7a8, arg_0x407de930);
//#line 43
}
//#line 43
//# 89 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/MetadataFlagsLayerC.nc"
static inline   void MetadataFlagsLayerC$RadioPacket$setPayloadLength(message_t *msg, uint8_t length)
{
  MetadataFlagsLayerC$SubPacket$setPayloadLength(msg, length);
}

//# 43 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioPacket.nc"
inline static   void TimeStampingLayerP$SubPacket$setPayloadLength(message_t *arg_0x407de7a8, uint8_t arg_0x407de930){
//#line 43
  MetadataFlagsLayerC$RadioPacket$setPayloadLength(arg_0x407de7a8, arg_0x407de930);
//#line 43
}
//#line 43
//# 115 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/TimeStampingLayerP.nc"
static inline   void TimeStampingLayerP$RadioPacket$setPayloadLength(message_t *msg, uint8_t length)
{
  TimeStampingLayerP$SubPacket$setPayloadLength(msg, length);
}

//# 43 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioPacket.nc"
inline static   void IEEE154MessageLayerP$SubPacket$setPayloadLength(message_t *arg_0x407de7a8, uint8_t arg_0x407de930){
//#line 43
  TimeStampingLayerP$RadioPacket$setPayloadLength(arg_0x407de7a8, arg_0x407de930);
//#line 43
}
//#line 43
//# 111 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/IEEE154MessageLayerP.nc"
static inline   void IEEE154MessageLayerP$IEEE154MessageLayer$createAckReply(message_t *data, message_t *ack)
{
  ieee154_header_t *header = IEEE154MessageLayerP$getHeader(ack);

//#line 114
  IEEE154MessageLayerP$SubPacket$setPayloadLength(ack, IEEE154MessageLayerP$IEEE154_ACK_FRAME_LENGTH);

  __nesc_hton_leuint16((unsigned char *)&header->fcf, IEEE154MessageLayerP$IEEE154_ACK_FRAME_VALUE);
  __nesc_hton_leuint8((unsigned char *)&header->dsn, __nesc_ntoh_leuint8((unsigned char *)&IEEE154MessageLayerP$getHeader(data)->dsn));
}

//# 77 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/IEEE154MessageLayer.nc"
inline static   void RF230ActiveMessageP$IEEE154MessageLayer$createAckReply(message_t *arg_0x407b99b8, message_t *arg_0x407b9b68){
//#line 77
  IEEE154MessageLayerP$IEEE154MessageLayer$createAckReply(arg_0x407b99b8, arg_0x407b9b68);
//#line 77
}
//#line 77
//# 116 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230ActiveMessageP.nc"
static inline   void RF230ActiveMessageP$SoftwareAckConfig$createAckPacket(message_t *data, message_t *ack)
{
  RF230ActiveMessageP$IEEE154MessageLayer$createAckReply(data, ack);
}

//# 69 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/SoftwareAckConfig.nc"
inline static   void SoftwareAckLayerP$SoftwareAckConfig$createAckPacket(message_t *arg_0x407a2678, message_t *arg_0x407a2828){
//#line 69
  RF230ActiveMessageP$SoftwareAckConfig$createAckPacket(arg_0x407a2678, arg_0x407a2828);
//#line 69
}
//#line 69
//# 111 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230ActiveMessageP.nc"
static inline   bool RF230ActiveMessageP$SoftwareAckConfig$requiresAckReply(message_t *msg)
{
  return RF230ActiveMessageP$IEEE154MessageLayer$requiresAckReply(msg);
}

//# 64 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/SoftwareAckConfig.nc"
inline static   bool SoftwareAckLayerP$SoftwareAckConfig$requiresAckReply(message_t *arg_0x407a2140){
//#line 64
  unsigned char result;
//#line 64

//#line 64
  result = RF230ActiveMessageP$SoftwareAckConfig$requiresAckReply(arg_0x407a2140);
//#line 64

//#line 64
  return result;
//#line 64
}
//#line 64
//# 69 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/MetadataFlagsLayerC.nc"
static inline   void MetadataFlagsLayerC$PacketFlag$setValue(uint8_t bit, message_t *msg, bool value)
{
  if (value) {
    MetadataFlagsLayerC$PacketFlag$set(bit, msg);
    }
  else {
//#line 74
    MetadataFlagsLayerC$PacketFlag$clear(bit, msg);
    }
}

//# 34 "/opt/tinyos-2.x/tos/chips/rf2xx/util/PacketFlag.nc"
inline static   void SoftwareAckLayerP$AckReceivedFlag$setValue(message_t *arg_0x409bb010, bool arg_0x409bb198){
//#line 34
  MetadataFlagsLayerC$PacketFlag$setValue(0U, arg_0x409bb010, arg_0x409bb198);
//#line 34
}
//#line 34
//# 58 "/opt/tinyos-2.x/tos/chips/stm32/timer/HplStm32Timer3P.nc"
static inline   void HplStm32Timer3P$CompareA$stop(void)
//#line 58
{
}

//# 59 "/opt/tinyos-2.x/tos/chips/stm32/timer/HplStm32Compare.nc"
inline static   void /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$HplStm32Compare$stop(void){
//#line 59
  HplStm32Timer3P$CompareA$stop();
//#line 59
}
//#line 59
//# 65 "/opt/tinyos-2.x/tos/chips/stm32/timer/Stm32AlarmC.nc"
static inline   void /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$Alarm$stop(void)
//#line 65
{
  /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$HplStm32Compare$stop();
}

//# 62 "/opt/tinyos-2.x/tos/lib/timer/Alarm.nc"
inline static   void RadioAlarmP$Alarm$stop(void){
//#line 62
  /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$Alarm$stop();
//#line 62
}
//#line 62
//# 96 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioAlarmP.nc"
static inline   void RadioAlarmP$RadioAlarm$cancel(uint8_t id)
{
  for (; 0; ) ;
  for (; 0; ) ;

  RadioAlarmP$Alarm$stop();
  RadioAlarmP$state = RadioAlarmP$STATE_READY;
}

//# 43 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioAlarm.nc"
inline static   void SoftwareAckLayerP$RadioAlarm$cancel(void){
//#line 43
  RadioAlarmP$RadioAlarm$cancel(2U);
//#line 43
}
//#line 43
//# 64 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/IEEE154MessageLayer.nc"
inline static   bool RF230ActiveMessageP$IEEE154MessageLayer$isAckFrame(message_t *arg_0x407bae70){
//#line 64
  unsigned char result;
//#line 64

//#line 64
  result = IEEE154MessageLayerP$IEEE154MessageLayer$isAckFrame(arg_0x407bae70);
//#line 64

//#line 64
  return result;
//#line 64
}
//#line 64
//# 96 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230ActiveMessageP.nc"
static inline   bool RF230ActiveMessageP$SoftwareAckConfig$isAckPacket(message_t *msg)
{
  return RF230ActiveMessageP$IEEE154MessageLayer$isAckFrame(msg);
}

//# 51 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/SoftwareAckConfig.nc"
inline static   bool SoftwareAckLayerP$SoftwareAckConfig$isAckPacket(message_t *arg_0x407a4380){
//#line 51
  unsigned char result;
//#line 51

//#line 51
  result = RF230ActiveMessageP$SoftwareAckConfig$isAckPacket(arg_0x407a4380);
//#line 51

//#line 51
  return result;
//#line 51
}
//#line 51
//# 131 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/SoftwareAckLayerP.nc"
static inline   message_t *SoftwareAckLayerP$SubReceive$receive(message_t *msg)
{
  bool ack = SoftwareAckLayerP$SoftwareAckConfig$isAckPacket(msg);

  for (; 0; ) ;

  if (SoftwareAckLayerP$state == SoftwareAckLayerP$STATE_ACK_WAIT) 
    {
      for (; 0; ) ;

      SoftwareAckLayerP$RadioAlarm$cancel();
      SoftwareAckLayerP$AckReceivedFlag$setValue(SoftwareAckLayerP$txMsg, ack);

      SoftwareAckLayerP$state = SoftwareAckLayerP$STATE_READY;
      SoftwareAckLayerP$RadioSend$sendDone(SUCCESS);
    }

  if (ack) {
    return msg;
    }
  if (SoftwareAckLayerP$SoftwareAckConfig$requiresAckReply(msg)) 
    {
      SoftwareAckLayerP$SoftwareAckConfig$createAckPacket(msg, &SoftwareAckLayerP$ackMsg);


      if (SoftwareAckLayerP$SubSend$send(&SoftwareAckLayerP$ackMsg) == SUCCESS) {
        SoftwareAckLayerP$state = SoftwareAckLayerP$STATE_ACK_SEND;
        }
      else {
//#line 159
        for (; 0; ) ;
        }
    }
  return SoftwareAckLayerP$RadioReceive$receive(msg);
}

//# 42 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioReceive.nc"
inline static   message_t *RF230DriverLayerP$RadioReceive$receive(message_t *arg_0x408eec28){
//#line 42
  nx_struct message_t *result;
//#line 42

//#line 42
  result = SoftwareAckLayerP$SubReceive$receive(arg_0x408eec28);
//#line 42

//#line 42
  return result;
//#line 42
}
//#line 42
//# 980 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230DriverLayerP.nc"
static inline   void RF230DriverLayerP$PacketLinkQuality$set(message_t *msg, uint8_t value)
{
  RF230DriverLayerP$getMeta(msg)->lqi = value;
}

//# 70 "/opt/tinyos-2.x/tos/platforms/stm32-p103/chips/rf230/RadioConfig.h"
static __inline uint16_t RF230_CRCBYTE_COMMAND(uint16_t crc, uint8_t data)
{

  return 0;
}

//# 270 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/MessageBufferLayerP.nc"
static inline   bool MessageBufferLayerP$RadioReceive$header(message_t *msg)
{
  bool notFull;


  { __nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
//#line 275
    notFull = MessageBufferLayerP$receiveQueueSize < MessageBufferLayerP$RECEIVE_QUEUE_SIZE;
//#line 275
    __nesc_atomic_end(__nesc_atomic); }

  return notFull;
}

//# 35 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioReceive.nc"
inline static   bool UniqueLayerP$RadioReceive$header(message_t *arg_0x408ee550){
//#line 35
  unsigned char result;
//#line 35

//#line 35
  result = MessageBufferLayerP$RadioReceive$header(arg_0x408ee550);
//#line 35

//#line 35
  return result;
//#line 35
}
//#line 35
//# 74 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/UniqueLayerP.nc"
static inline   bool UniqueLayerP$SubReceive$header(message_t *msg)
{

  return UniqueLayerP$RadioReceive$header(msg);
}

//# 35 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioReceive.nc"
inline static   bool RandomCollisionLayerP$RadioReceive$header(message_t *arg_0x408ee550){
//#line 35
  unsigned char result;
//#line 35

//#line 35
  result = UniqueLayerP$SubReceive$header(arg_0x408ee550);
//#line 35

//#line 35
  return result;
//#line 35
}
//#line 35
//# 152 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/RandomCollisionLayerP.nc"
static inline   bool RandomCollisionLayerP$SubReceive$header(message_t *msg)
{
  return RandomCollisionLayerP$RadioReceive$header(msg);
}

//# 35 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioReceive.nc"
inline static   bool SoftwareAckLayerP$RadioReceive$header(message_t *arg_0x408ee550){
//#line 35
  unsigned char result;
//#line 35

//#line 35
  result = RandomCollisionLayerP$SubReceive$header(arg_0x408ee550);
//#line 35

//#line 35
  return result;
//#line 35
}
//#line 35
//# 120 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/IEEE154MessageLayerP.nc"
static inline   bool IEEE154MessageLayerP$IEEE154MessageLayer$verifyAckReply(message_t *data, message_t *ack)
{
  ieee154_header_t *header = IEEE154MessageLayerP$getHeader(ack);

  return __nesc_ntoh_leuint8((unsigned char *)&header->dsn) == __nesc_ntoh_leuint8((unsigned char *)&IEEE154MessageLayerP$getHeader(data)->dsn)
   && (__nesc_ntoh_leuint16((unsigned char *)&header->fcf) & IEEE154MessageLayerP$IEEE154_ACK_FRAME_MASK) == IEEE154MessageLayerP$IEEE154_ACK_FRAME_VALUE;
}

//# 83 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/IEEE154MessageLayer.nc"
inline static   bool RF230ActiveMessageP$IEEE154MessageLayer$verifyAckReply(message_t *arg_0x407d1150, message_t *arg_0x407d1300){
//#line 83
  unsigned char result;
//#line 83

//#line 83
  result = IEEE154MessageLayerP$IEEE154MessageLayer$verifyAckReply(arg_0x407d1150, arg_0x407d1300);
//#line 83

//#line 83
  return result;
//#line 83
}
//#line 83
//# 101 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230ActiveMessageP.nc"
static inline   bool RF230ActiveMessageP$SoftwareAckConfig$verifyAckPacket(message_t *data, message_t *ack)
{
  return RF230ActiveMessageP$IEEE154MessageLayer$verifyAckReply(data, ack);
}

//# 58 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/SoftwareAckConfig.nc"
inline static   bool SoftwareAckLayerP$SoftwareAckConfig$verifyAckPacket(message_t *arg_0x407a4970, message_t *arg_0x407a4b20){
//#line 58
  unsigned char result;
//#line 58

//#line 58
  result = RF230ActiveMessageP$SoftwareAckConfig$verifyAckPacket(arg_0x407a4970, arg_0x407a4b20);
//#line 58

//#line 58
  return result;
//#line 58
}
//#line 58
//# 123 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/SoftwareAckLayerP.nc"
static inline   bool SoftwareAckLayerP$SubReceive$header(message_t *msg)
{
  if (SoftwareAckLayerP$SoftwareAckConfig$isAckPacket(msg)) {
    return SoftwareAckLayerP$state == SoftwareAckLayerP$STATE_ACK_WAIT && SoftwareAckLayerP$SoftwareAckConfig$verifyAckPacket(SoftwareAckLayerP$txMsg, msg);
    }
  else {
//#line 128
    return SoftwareAckLayerP$RadioReceive$header(msg);
    }
}

//# 35 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioReceive.nc"
inline static   bool RF230DriverLayerP$RadioReceive$header(message_t *arg_0x408ee550){
//#line 35
  unsigned char result;
//#line 35

//#line 35
  result = SoftwareAckLayerP$SubReceive$header(arg_0x408ee550);
//#line 35

//#line 35
  return result;
//#line 35
}
//#line 35
//# 78 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230ActiveMessageP.nc"
static inline   uint8_t RF230ActiveMessageP$RF230DriverConfig$headerPreloadLength(void)
{

  return 7;
}

//# 48 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230DriverConfig.nc"
inline static   uint8_t RF230DriverLayerP$Config$headerPreloadLength(void){
//#line 48
  unsigned char result;
//#line 48

//#line 48
  result = RF230ActiveMessageP$RF230DriverConfig$headerPreloadLength();
//#line 48

//#line 48
  return result;
//#line 48
}
//#line 48
//# 83 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230DriverLayerP.nc"
static inline rf230_header_t *RF230DriverLayerP$getHeader(message_t *msg)
{
  return (rf230_header_t *) ((void *)msg + RF230DriverLayerP$Config$headerLength(msg));
}

//# 68 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230ActiveMessageP.nc"
static inline   uint8_t RF230ActiveMessageP$RF230DriverConfig$maxPayloadLength(void)
{
  return sizeof(rf230packet_header_t ) + 120;
}

//# 35 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230DriverConfig.nc"
inline static   uint8_t RF230DriverLayerP$Config$maxPayloadLength(void){
//#line 35
  unsigned char result;
//#line 35

//#line 35
  result = RF230ActiveMessageP$RF230DriverConfig$maxPayloadLength();
//#line 35

//#line 35
  return result;
//#line 35
}
//#line 35
//# 873 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230DriverLayerP.nc"
static inline   uint8_t RF230DriverLayerP$RadioPacket$maxPayloadLength(void)
{
  for (; 0; ) ;

  return RF230DriverLayerP$Config$maxPayloadLength() - sizeof(rf230_header_t );
}

//# 161 "/opt/tinyos-2.x/tos/chips/stm32/spi/Stm32SpiP.nc"
static __inline   uint8_t Stm32SpiP$FastSpiByte$write(uint8_t data)
//#line 161
{
  Stm32SpiP$Spi$write(data);
  while (SPI_I2S_GetFlagStatus((SPI_TypeDef *)((uint32_t )0x40000000 + 0x10000 + 0x3000), (uint16_t )0x0001) == RESET) 
    ;
  return Stm32SpiP$Spi$read();
}

//# 65 "/opt/tinyos-2.x/tos/interfaces/FastSpiByte.nc"
inline static   uint8_t RF230DriverLayerP$FastSpiByte$write(uint8_t arg_0x40a310b0){
//#line 65
  unsigned char result;
//#line 65

//#line 65
  result = Stm32SpiP$FastSpiByte$write(arg_0x40a310b0);
//#line 65

//#line 65
  return result;
//#line 65
}
//#line 65
//# 578 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230DriverLayerP.nc"
static __inline void RF230DriverLayerP$downloadMessage(void)
{
  uint8_t length;
  uint16_t crc;

  RF230DriverLayerP$SELN$clr();
  RF230DriverLayerP$FastSpiByte$write(RF230_CMD_FRAME_READ);


  length = RF230DriverLayerP$FastSpiByte$write(0);


  if (length >= 3 && length <= RF230DriverLayerP$RadioPacket$maxPayloadLength() + 2) 
    {
      uint8_t read;
      uint8_t *data;


      RF230DriverLayerP$FastSpiByte$splitWrite(0);

      data = (uint8_t *) RF230DriverLayerP$getPayload(RF230DriverLayerP$rxMsg);
      __nesc_hton_leuint8((unsigned char *)&RF230DriverLayerP$getHeader(RF230DriverLayerP$rxMsg)->length, length);
      crc = 0;


      length -= 2;

      read = RF230DriverLayerP$Config$headerPreloadLength();
      if (length < read) {
        read = length;
        }
      length -= read;

      do {
          crc = RF230_CRCBYTE_COMMAND(crc, * data++ = RF230DriverLayerP$FastSpiByte$splitReadWrite(0));
        }
      while (--read != 0);

      if (RF230DriverLayerP$RadioReceive$header(RF230DriverLayerP$rxMsg)) 
        {
          while (length-- != 0) 
            crc = RF230_CRCBYTE_COMMAND(crc, * data++ = RF230DriverLayerP$FastSpiByte$splitReadWrite(0));

          crc = RF230_CRCBYTE_COMMAND(crc, RF230DriverLayerP$FastSpiByte$splitReadWrite(0));
          crc = RF230_CRCBYTE_COMMAND(crc, RF230DriverLayerP$FastSpiByte$splitReadWrite(0));

          RF230DriverLayerP$PacketLinkQuality$set(RF230DriverLayerP$rxMsg, RF230DriverLayerP$FastSpiByte$splitRead());
        }
      else {
        crc = 1;
        }
    }
  else {
//#line 630
    crc = 1;
    }
  RF230DriverLayerP$SELN$set();
  RF230DriverLayerP$state = RF230DriverLayerP$STATE_RX_ON;
//#line 650
  RF230DriverLayerP$cmd = RF230DriverLayerP$CMD_NONE;


  if (crc == 0) {
    RF230DriverLayerP$rxMsg = RF230DriverLayerP$RadioReceive$receive(RF230DriverLayerP$rxMsg);
    }
}

//# 125 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230ActiveMessageP.nc"
static inline   uint16_t RF230ActiveMessageP$SoftwareAckConfig$getAckTimeout(void)
{
  return (uint16_t )(800 * (7372800UL / 1 / 32) * (1 << 2) / 1000000UL);
}

//# 32 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/SoftwareAckConfig.nc"
inline static   uint16_t SoftwareAckLayerP$SoftwareAckConfig$getAckTimeout(void){
//#line 32
  unsigned short result;
//#line 32

//#line 32
  result = RF230ActiveMessageP$SoftwareAckConfig$getAckTimeout();
//#line 32

//#line 32
  return result;
//#line 32
}
//#line 32
//# 38 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioAlarm.nc"
inline static   void SoftwareAckLayerP$RadioAlarm$wait(uint16_t arg_0x407c7958){
//#line 38
  RadioAlarmP$RadioAlarm$wait(2U, arg_0x407c7958);
//#line 38
}
//#line 38
//#line 33
inline static   bool SoftwareAckLayerP$RadioAlarm$isFree(void){
//#line 33
  unsigned char result;
//#line 33

//#line 33
  result = RadioAlarmP$RadioAlarm$isFree(2U);
//#line 33

//#line 33
  return result;
//#line 33
}
//#line 33
//# 128 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/IEEE154MessageLayerP.nc"
static inline   bool IEEE154MessageLayerP$IEEE154MessageLayer$getAckRequired(message_t *msg)
{
  return __nesc_ntoh_leuint16((unsigned char *)&IEEE154MessageLayerP$getHeader(msg)->fcf) & (1 << IEEE154_FCF_ACK_REQ);
}

//#line 194
static inline   bool IEEE154MessageLayerP$IEEE154MessageLayer$requiresAckWait(message_t *msg)
{
  return IEEE154MessageLayerP$IEEE154MessageLayer$getAckRequired(msg)
   && IEEE154MessageLayerP$IEEE154MessageLayer$isDataFrame(msg)
   && IEEE154MessageLayerP$IEEE154MessageLayer$getDestAddr(msg) != 0xFFFF;
}

//# 151 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/IEEE154MessageLayer.nc"
inline static   bool RF230ActiveMessageP$IEEE154MessageLayer$requiresAckWait(message_t *arg_0x407cb120){
//#line 151
  unsigned char result;
//#line 151

//#line 151
  result = IEEE154MessageLayerP$IEEE154MessageLayer$requiresAckWait(arg_0x407cb120);
//#line 151

//#line 151
  return result;
//#line 151
}
//#line 151
//# 91 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230ActiveMessageP.nc"
static inline   bool RF230ActiveMessageP$SoftwareAckConfig$requiresAckWait(message_t *msg)
{
  return RF230ActiveMessageP$IEEE154MessageLayer$requiresAckWait(msg);
}

//# 44 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/SoftwareAckConfig.nc"
inline static   bool SoftwareAckLayerP$SoftwareAckConfig$requiresAckWait(message_t *arg_0x407a5df8){
//#line 44
  unsigned char result;
//#line 44

//#line 44
  result = RF230ActiveMessageP$SoftwareAckConfig$requiresAckWait(arg_0x407a5df8);
//#line 44

//#line 44
  return result;
//#line 44
}
//#line 44
//# 86 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/SoftwareAckLayerP.nc"
static inline   void SoftwareAckLayerP$SubSend$sendDone(error_t error)
{
  if (SoftwareAckLayerP$state == SoftwareAckLayerP$STATE_ACK_SEND) 
    {

      for (; 0; ) ;

      SoftwareAckLayerP$state = SoftwareAckLayerP$STATE_READY;
    }
  else 
    {
      for (; 0; ) ;
      for (; 0; ) ;

      if (error == SUCCESS && SoftwareAckLayerP$SoftwareAckConfig$requiresAckWait(SoftwareAckLayerP$txMsg) && SoftwareAckLayerP$RadioAlarm$isFree()) 
        {
          SoftwareAckLayerP$RadioAlarm$wait(SoftwareAckLayerP$SoftwareAckConfig$getAckTimeout());
          SoftwareAckLayerP$state = SoftwareAckLayerP$STATE_ACK_WAIT;
        }
      else 
        {
          SoftwareAckLayerP$state = SoftwareAckLayerP$STATE_READY;
          SoftwareAckLayerP$RadioSend$sendDone(error);
        }
    }
}

//# 45 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioSend.nc"
inline static   void RF230DriverLayerP$RadioSend$sendDone(error_t arg_0x40937850){
//#line 45
  SoftwareAckLayerP$SubSend$sendDone(arg_0x40937850);
//#line 45
}
//#line 45
//# 44 "/opt/tinyos-2.x/tos/chips/rf2xx/util/PacketFlag.nc"
inline static   void TimeStampingLayerP$TimeStampFlag$clear(message_t *arg_0x409bbba0){
//#line 44
  MetadataFlagsLayerC$PacketFlag$clear(1U, arg_0x409bbba0);
//#line 44
}
//#line 44
//# 66 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/TimeStampingLayerP.nc"
static inline   void TimeStampingLayerP$PacketTimeStampRadio$clear(message_t *msg)
{
  TimeStampingLayerP$TimeStampFlag$clear(msg);
}

//# 59 "/opt/tinyos-2.x/tos/interfaces/PacketTimeStamp.nc"
inline static   void RF230DriverLayerP$PacketTimeStamp$clear(message_t *arg_0x4076f710){
//#line 59
  TimeStampingLayerP$PacketTimeStampRadio$clear(arg_0x4076f710);
//#line 59
}
//#line 59
//# 125 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/TimeStampingLayerP.nc"
static inline   uint8_t TimeStampingLayerP$RadioPacket$metadataLength(message_t *msg)
{
  return TimeStampingLayerP$SubPacket$metadataLength(msg) + sizeof(timestamp_metadata_t );
}

//#line 49
static inline timestamp_metadata_t *TimeStampingLayerP$getMeta(message_t *msg)
{
  return (timestamp_metadata_t *) ((void *)msg + sizeof(message_t ) - TimeStampingLayerP$RadioPacket$metadataLength(msg));
}

//# 39 "/opt/tinyos-2.x/tos/chips/rf2xx/util/PacketFlag.nc"
inline static   void TimeStampingLayerP$TimeStampFlag$set(message_t *arg_0x409bb6a8){
//#line 39
  MetadataFlagsLayerC$PacketFlag$set(1U, arg_0x409bb6a8);
//#line 39
}
//#line 39
//# 71 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/TimeStampingLayerP.nc"
static inline   void TimeStampingLayerP$PacketTimeStampRadio$set(message_t *msg, uint32_t value)
{
  TimeStampingLayerP$TimeStampFlag$set(msg);
  TimeStampingLayerP$getMeta(msg)->timestamp = value;
}

//# 67 "/opt/tinyos-2.x/tos/interfaces/PacketTimeStamp.nc"
inline static   void RF230DriverLayerP$PacketTimeStamp$set(message_t *arg_0x4076fc38, RF230DriverLayerP$PacketTimeStamp$size_type arg_0x4076fdc8){
//#line 67
  TimeStampingLayerP$PacketTimeStampRadio$set(arg_0x4076fc38, arg_0x4076fdc8);
//#line 67
}
//#line 67
//# 53 "/opt/tinyos-2.x/tos/lib/timer/Counter.nc"
inline static   /*LocalTimeMicroC.CounterToLocalTimeC*/CounterToLocalTimeC$0$Counter$size_type /*LocalTimeMicroC.CounterToLocalTimeC*/CounterToLocalTimeC$0$Counter$get(void){
//#line 53
  unsigned long result;
//#line 53

//#line 53
  result = /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$Counter$get();
//#line 53

//#line 53
  return result;
//#line 53
}
//#line 53
//# 42 "/opt/tinyos-2.x/tos/lib/timer/CounterToLocalTimeC.nc"
static inline   uint32_t /*LocalTimeMicroC.CounterToLocalTimeC*/CounterToLocalTimeC$0$LocalTime$get(void)
{
  return /*LocalTimeMicroC.CounterToLocalTimeC*/CounterToLocalTimeC$0$Counter$get();
}

//# 50 "/opt/tinyos-2.x/tos/lib/timer/LocalTime.nc"
inline static   uint32_t RF230DriverLayerP$LocalTime$get(void){
//#line 50
  unsigned long result;
//#line 50

//#line 50
  result = /*LocalTimeMicroC.CounterToLocalTimeC*/CounterToLocalTimeC$0$LocalTime$get();
//#line 50

//#line 50
  return result;
//#line 50
}
//#line 50
//# 44 "/opt/tinyos-2.x/tos/chips/rf2xx/util/PacketFlag.nc"
inline static   void RF230DriverLayerP$RSSIFlag$clear(message_t *arg_0x409bbba0){
//#line 44
  MetadataFlagsLayerC$PacketFlag$clear(3U, arg_0x409bbba0);
//#line 44
}
//#line 44
//# 925 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230DriverLayerP.nc"
static inline   void RF230DriverLayerP$PacketRSSI$clear(message_t *msg)
{
  RF230DriverLayerP$RSSIFlag$clear(msg);
}

//# 39 "/opt/tinyos-2.x/tos/chips/rf2xx/util/PacketFlag.nc"
inline static   void RF230DriverLayerP$RSSIFlag$set(message_t *arg_0x409bb6a8){
//#line 39
  MetadataFlagsLayerC$PacketFlag$set(3U, arg_0x409bb6a8);
//#line 39
}
//#line 39





inline static   void RF230DriverLayerP$TransmitPowerFlag$clear(message_t *arg_0x409bbba0){
//#line 44
  MetadataFlagsLayerC$PacketFlag$clear(2U, arg_0x409bbba0);
//#line 44
}
//#line 44
//# 930 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230DriverLayerP.nc"
static inline   void RF230DriverLayerP$PacketRSSI$set(message_t *msg, uint8_t value)
{

  RF230DriverLayerP$TransmitPowerFlag$clear(msg);

  RF230DriverLayerP$RSSIFlag$set(msg);
  RF230DriverLayerP$getMeta(msg)->rssi = value;
}

//#line 672
static inline void RF230DriverLayerP$serviceRadio(void)
{
  if (RF230DriverLayerP$isSpiAcquired()) 
    {
      uint16_t time;
      uint32_t time32;
      uint8_t irq;
      uint8_t temp;

      { __nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
//#line 681
        time = RF230DriverLayerP$capturedTime;
//#line 681
        __nesc_atomic_end(__nesc_atomic); }
      RF230DriverLayerP$radioIrq = FALSE;
      irq = RF230DriverLayerP$readRegister(RF230_IRQ_STATUS);
//#line 703
      if (irq & RF230_IRQ_PLL_LOCK) 
        {
          if (RF230DriverLayerP$cmd == RF230DriverLayerP$CMD_TURNON || RF230DriverLayerP$cmd == RF230DriverLayerP$CMD_CHANNEL) 
            {
              for (; 0; ) ;

              RF230DriverLayerP$state = RF230DriverLayerP$STATE_RX_ON;
              RF230DriverLayerP$cmd = RF230DriverLayerP$CMD_SIGNAL_DONE;
            }
          else {
//#line 712
            if (RF230DriverLayerP$cmd == RF230DriverLayerP$CMD_TRANSMIT) 
              {
                for (; 0; ) ;
              }
            else {
              for (; 0; ) ;
              }
            }
        }
//#line 720
      if (irq & RF230_IRQ_RX_START) 
        {
          if (RF230DriverLayerP$cmd == RF230DriverLayerP$CMD_CCA) 
            {
              RF230DriverLayerP$RadioCCA$done(FAIL);
              RF230DriverLayerP$cmd = RF230DriverLayerP$CMD_NONE;
            }

          if (RF230DriverLayerP$cmd == RF230DriverLayerP$CMD_NONE) 
            {
              for (; 0; ) ;


              if (irq == RF230_IRQ_RX_START) 
                {
                  temp = RF230DriverLayerP$readRegister(RF230_PHY_RSSI) & RF230_RSSI_MASK;
                  RF230DriverLayerP$rssiBusy += temp - (RF230DriverLayerP$rssiBusy >> 2);

                  RF230DriverLayerP$PacketRSSI$set(RF230DriverLayerP$rxMsg, temp);
                }
              else 
                {
                  RF230DriverLayerP$PacketRSSI$clear(RF230DriverLayerP$rxMsg);
                }










              if (irq == RF230_IRQ_RX_START) 
                {
                  time32 = RF230DriverLayerP$LocalTime$get();
                  time32 += (int16_t )(time - RF230DriverLayerP$RX_SFD_DELAY) - (int16_t )time32;
                  RF230DriverLayerP$PacketTimeStamp$set(RF230DriverLayerP$rxMsg, time32);
                }
              else {
                RF230DriverLayerP$PacketTimeStamp$clear(RF230DriverLayerP$rxMsg);
                }
              RF230DriverLayerP$cmd = RF230DriverLayerP$CMD_RECEIVE;
            }
          else {
            for (; 0; ) ;
            }
        }
      if (irq & RF230_IRQ_TRX_END) 
        {
          if (RF230DriverLayerP$cmd == RF230DriverLayerP$CMD_TRANSMIT) 
            {
              for (; 0; ) ;

              RF230DriverLayerP$state = RF230DriverLayerP$STATE_RX_ON;
              RF230DriverLayerP$cmd = RF230DriverLayerP$CMD_NONE;
              RF230DriverLayerP$RadioSend$sendDone(SUCCESS);


              for (; 0; ) ;
            }
          else {
//#line 782
            if (RF230DriverLayerP$cmd == RF230DriverLayerP$CMD_RECEIVE) 
              {
                for (; 0; ) ;






                if (RF230DriverLayerP$state == RF230DriverLayerP$STATE_PLL_ON_2_RX_ON) 
                  {
                    for (; 0; ) ;

                    RF230DriverLayerP$writeRegister(RF230_TRX_STATE, RF230_RX_ON);
                    RF230DriverLayerP$state = RF230DriverLayerP$STATE_RX_ON;
                  }
                else 
                  {

                    RF230DriverLayerP$rssiClear += (RF230DriverLayerP$readRegister(RF230_PHY_RSSI) & RF230_RSSI_MASK) - (RF230DriverLayerP$rssiClear >> 2);
                  }

                RF230DriverLayerP$cmd = RF230DriverLayerP$CMD_DOWNLOAD;
              }
            else {
              for (; 0; ) ;
              }
            }
        }
    }
}











static inline   void RF230DriverLayerP$Tasklet$run(void)
{
  if (RF230DriverLayerP$radioIrq) {
    RF230DriverLayerP$serviceRadio();
    }
  if (RF230DriverLayerP$cmd != RF230DriverLayerP$CMD_NONE) 
    {
      if (RF230DriverLayerP$cmd == RF230DriverLayerP$CMD_DOWNLOAD) {
        RF230DriverLayerP$downloadMessage();
        }
      else {
//#line 833
        if (RF230DriverLayerP$CMD_TURNOFF <= RF230DriverLayerP$cmd && RF230DriverLayerP$cmd <= RF230DriverLayerP$CMD_TURNON) {
          RF230DriverLayerP$changeState();
          }
        else {
//#line 835
          if (RF230DriverLayerP$cmd == RF230DriverLayerP$CMD_CHANNEL) {
            RF230DriverLayerP$changeChannel();
            }
          }
        }
//#line 838
      if (RF230DriverLayerP$cmd == RF230DriverLayerP$CMD_SIGNAL_DONE) 
        {
          RF230DriverLayerP$cmd = RF230DriverLayerP$CMD_NONE;
          RF230DriverLayerP$RadioState$done();
        }
    }

  if (RF230DriverLayerP$cmd == RF230DriverLayerP$CMD_NONE && RF230DriverLayerP$state == RF230DriverLayerP$STATE_RX_ON && !RF230DriverLayerP$radioIrq) {
    RF230DriverLayerP$RadioSend$ready();
    }
  if (RF230DriverLayerP$cmd == RF230DriverLayerP$CMD_NONE) {
    RF230DriverLayerP$SpiResource$release();
    }
}

//# 37 "/opt/tinyos-2.x/tos/chips/rf2xx/util/Tasklet.nc"
inline static   void TaskletC$Tasklet$run(void){
//#line 37
  RF230DriverLayerP$Tasklet$run();
//#line 37
  MessageBufferLayerP$Tasklet$run();
//#line 37
  RadioAlarmP$Tasklet$run();
//#line 37
}
//#line 37
//# 148 "/opt/tinyos-2.x/tos/system/SimpleArbiterP.nc"
static inline   uint8_t /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$Resource$isOwner(uint8_t id)
//#line 148
{
  { __nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
//#line 149
    {
      if (/*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$resId == id && /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$state == /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$RES_BUSY) {
          unsigned char __nesc_temp = 
//#line 150
          TRUE;

          {
//#line 150
            __nesc_atomic_end(__nesc_atomic); 
//#line 150
            return __nesc_temp;
          }
        }
      else 
//#line 151
        {
          unsigned char __nesc_temp = 
//#line 151
          FALSE;

          {
//#line 151
            __nesc_atomic_end(__nesc_atomic); 
//#line 151
            return __nesc_temp;
          }
        }
    }
//#line 154
    __nesc_atomic_end(__nesc_atomic); }
}

//# 118 "/opt/tinyos-2.x/tos/interfaces/Resource.nc"
inline static   bool Stm32SpiP$ResourceArbiter$isOwner(uint8_t arg_0x40af20f8){
//#line 118
  unsigned char result;
//#line 118

//#line 118
  result = /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$Resource$isOwner(arg_0x40af20f8);
//#line 118

//#line 118
  return result;
//#line 118
}
//#line 118
//# 364 "/opt/tinyos-2.x/tos/chips/stm32/spi/Stm32SpiP.nc"
static inline   uint8_t Stm32SpiP$Resource$isOwner(uint8_t id)
//#line 364
{
  return Stm32SpiP$ResourceArbiter$isOwner(id);
}

//# 118 "/opt/tinyos-2.x/tos/interfaces/Resource.nc"
inline static   bool RF230DriverLayerP$SpiResource$isOwner(void){
//#line 118
  unsigned char result;
//#line 118

//#line 118
  result = Stm32SpiP$Resource$isOwner(0U);
//#line 118

//#line 118
  return result;
//#line 118
}
//#line 118
//# 169 "/opt/tinyos-2.x/tos/system/SimpleArbiterP.nc"
static inline    void /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$ResourceRequested$default$immediateRequested(uint8_t id)
//#line 169
{
}

//# 51 "/opt/tinyos-2.x/tos/interfaces/ResourceRequested.nc"
inline static   void /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$ResourceRequested$immediateRequested(uint8_t arg_0x40b5f688){
//#line 51
    /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$ResourceRequested$default$immediateRequested(arg_0x40b5f688);
//#line 51
}
//#line 51
//# 84 "/opt/tinyos-2.x/tos/system/SimpleArbiterP.nc"
static inline   error_t /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$Resource$immediateRequest(uint8_t id)
//#line 84
{
  /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$ResourceRequested$immediateRequested(/*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$resId);
  { __nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
//#line 86
    {
      if (/*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$state == /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$RES_IDLE) {
          /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$state = /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$RES_BUSY;
          /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$resId = id;
          /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$ResourceConfigure$configure(/*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$resId);
          {
            unsigned char __nesc_temp = 
//#line 91
            SUCCESS;

            {
//#line 91
              __nesc_atomic_end(__nesc_atomic); 
//#line 91
              return __nesc_temp;
            }
          }
        }
//#line 93
      {
        unsigned char __nesc_temp = 
//#line 93
        FAIL;

        {
//#line 93
          __nesc_atomic_end(__nesc_atomic); 
//#line 93
          return __nesc_temp;
        }
      }
    }
//#line 96
    __nesc_atomic_end(__nesc_atomic); }
}

//# 87 "/opt/tinyos-2.x/tos/interfaces/Resource.nc"
inline static   error_t Stm32SpiP$ResourceArbiter$immediateRequest(uint8_t arg_0x40af20f8){
//#line 87
  unsigned char result;
//#line 87

//#line 87
  result = /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$Resource$immediateRequest(arg_0x40af20f8);
//#line 87

//#line 87
  return result;
//#line 87
}
//#line 87
//# 337 "/opt/tinyos-2.x/tos/chips/stm32/spi/Stm32SpiP.nc"
static inline   error_t Stm32SpiP$Resource$immediateRequest(uint8_t id)
//#line 337
{
  error_t result = Stm32SpiP$ResourceArbiter$immediateRequest(id);

//#line 339
  if (result == SUCCESS) {
      Stm32SpiP$startSpi();
    }
  return result;
}

//# 87 "/opt/tinyos-2.x/tos/interfaces/Resource.nc"
inline static   error_t RF230DriverLayerP$SpiResource$immediateRequest(void){
//#line 87
  unsigned char result;
//#line 87

//#line 87
  result = Stm32SpiP$Resource$immediateRequest(0U);
//#line 87

//#line 87
  return result;
//#line 87
}
//#line 87
//# 133 "/opt/tinyos-2.x/tos/chips/stm32/spi/HplStm32SpiP.nc"
static inline   void HplStm32SpiP$SPI$setMasterBit(bool isMaster)
//#line 133
{
  if (isMaster) {

      SPI_InitStructure.SPI_Mode = (uint16_t )0x0104;
    }
  else {
      SPI_InitStructure.SPI_Mode = (uint16_t )0x0000;
    }
}

//# 82 "/opt/tinyos-2.x/tos/chips/stm32/gpio/HplSTM32GeneralIOPinP.nc"
static __inline   void /*HplSTM32GeneralIOC.PortA.Bit5*/HplSTM32GeneralIOPinP$5$IO$makeOutput(void)
//#line 82
{
  GPIO_TypeDef *port = (GPIO_TypeDef *)1073809408U;

  GPIO_InitTypeDef gpioi = { 
  (uint16_t )1 << 5, 
  GPIO_Speed_10MHz, 
  GPIO_Mode_Out_PP };

  GPIO_Init(port, &gpioi);
}

//# 35 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
inline static   void HplStm32SpiP$SCK$makeOutput(void){
//#line 35
  /*HplSTM32GeneralIOC.PortA.Bit5*/HplSTM32GeneralIOPinP$5$IO$makeOutput();
//#line 35
}
//#line 35
//#line 33
inline static   void HplStm32SpiP$MISO$makeInput(void){
//#line 33
  /*HplSTM32GeneralIOC.PortA.Bit6*/HplSTM32GeneralIOPinP$6$IO$makeInput();
//#line 33
}
//#line 33
//# 82 "/opt/tinyos-2.x/tos/chips/stm32/gpio/HplSTM32GeneralIOPinP.nc"
static __inline   void /*HplSTM32GeneralIOC.PortA.Bit7*/HplSTM32GeneralIOPinP$7$IO$makeOutput(void)
//#line 82
{
  GPIO_TypeDef *port = (GPIO_TypeDef *)1073809408U;

  GPIO_InitTypeDef gpioi = { 
  (uint16_t )1 << 7, 
  GPIO_Speed_10MHz, 
  GPIO_Mode_Out_PP };

  GPIO_Init(port, &gpioi);
}

//# 35 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
inline static   void HplStm32SpiP$MOSI$makeOutput(void){
//#line 35
  /*HplSTM32GeneralIOC.PortA.Bit7*/HplSTM32GeneralIOPinP$7$IO$makeOutput();
//#line 35
}
//#line 35
//# 29 "/opt/tinyos-2.x/tos/chips/stm32/spi/HplStm32SpiP.nc"
static inline   void HplStm32SpiP$SPI$initMaster(void)
//#line 29
{
  HplStm32SpiP$MOSI$makeOutput();
  HplStm32SpiP$MISO$makeInput();
  HplStm32SpiP$SCK$makeOutput();
  HplStm32SpiP$SPI$setMasterBit(TRUE);
}

//# 66 "/opt/tinyos-2.x/tos/chips/stm32/spi/Stm32Spi.nc"
inline static   void Stm32SpiP$Spi$initMaster(void){
//#line 66
  HplStm32SpiP$SPI$initMaster();
//#line 66
}
//#line 66
//# 44 "/opt/tinyos-2.x/tos/interfaces/McuPowerState.nc"
inline static   void HplStm32SpiP$Mcu$update(void){
//#line 44
  McuSleepC$McuPowerState$update();
//#line 44
}
//#line 44
//# 1515 "/opt/tinyos-2.x/tos/chips/stm32/fwlib/inc/cortexm3_macro.h"
static __inline void NVIC_DisableIRQ(IRQn_Type IRQn)
{
  ((NVIC_Type *)(0xE000E000 + 0x0100))->ICER[(uint32_t )IRQn >> 5] = 1 << ((uint32_t )IRQn & 0x1F);
}

//#line 1502
static __inline void NVIC_EnableIRQ(IRQn_Type IRQn)
{
  ((NVIC_Type *)(0xE000E000 + 0x0100))->ISER[(uint32_t )IRQn >> 5] = 1 << ((uint32_t )IRQn & 0x1F);
}

//# 70 "/opt/tinyos-2.x/tos/chips/stm32/spi/HplStm32SpiP.nc"
static inline   void HplStm32SpiP$SPI$enableInterrupt(bool enabled)
//#line 70
{

  if (enabled) 
    {

      NVIC_EnableIRQ(SPI1_IRQn);
      HplStm32SpiP$Mcu$update();
    }
  else 
    {
      NVIC_DisableIRQ(SPI1_IRQn);
      HplStm32SpiP$Mcu$update();
    }
}

//# 96 "/opt/tinyos-2.x/tos/chips/stm32/spi/Stm32Spi.nc"
inline static   void Stm32SpiP$Spi$enableInterrupt(bool arg_0x40aeb128){
//#line 96
  HplStm32SpiP$SPI$enableInterrupt(arg_0x40aeb128);
//#line 96
}
//#line 96
//# 197 "/opt/tinyos-2.x/tos/chips/stm32/spi/HplStm32SpiP.nc"
static inline   void HplStm32SpiP$SPI$setMasterDoubleSpeed(bool on)
//#line 197
{
  if (on) {
      SPI_InitStructure.SPI_Direction = (uint16_t )0x0000;
    }
  else 
    {
    }
}

//# 125 "/opt/tinyos-2.x/tos/chips/stm32/spi/Stm32Spi.nc"
inline static   void Stm32SpiP$Spi$setMasterDoubleSpeed(bool arg_0x40ae6358){
//#line 125
  HplStm32SpiP$SPI$setMasterDoubleSpeed(arg_0x40ae6358);
//#line 125
}
//#line 125
//# 148 "/opt/tinyos-2.x/tos/chips/stm32/spi/HplStm32SpiP.nc"
static inline   void HplStm32SpiP$SPI$setClockPolarity(bool highWhenIdle)
//#line 148
{
  if (highWhenIdle) {
    }
  else 
    {
      SPI_InitStructure.SPI_CPOL = (uint16_t )0x0000;
    }
}

//# 108 "/opt/tinyos-2.x/tos/chips/stm32/spi/Stm32Spi.nc"
inline static   void Stm32SpiP$Spi$setClockPolarity(bool arg_0x40ae8170){
//#line 108
  HplStm32SpiP$SPI$setClockPolarity(arg_0x40ae8170);
//#line 108
}
//#line 108
//# 164 "/opt/tinyos-2.x/tos/chips/stm32/spi/HplStm32SpiP.nc"
static inline   void HplStm32SpiP$SPI$setClockPhase(bool sampleOnTrailing)
//#line 164
{
  if (sampleOnTrailing) {
    }
  else 
    {
      SPI_InitStructure.SPI_CPHA = (uint16_t )0x0001;
    }
}

//# 111 "/opt/tinyos-2.x/tos/chips/stm32/spi/Stm32Spi.nc"
inline static   void Stm32SpiP$Spi$setClockPhase(bool arg_0x40ae8950){
//#line 111
  HplStm32SpiP$SPI$setClockPhase(arg_0x40ae8950);
//#line 111
}
//#line 111
//# 183 "/opt/tinyos-2.x/tos/chips/stm32/spi/HplStm32SpiP.nc"
static inline   void HplStm32SpiP$SPI$setClock(uint8_t v)
//#line 183
{


  SPI_InitStructure.SPI_BaudRatePrescaler = (uint16_t )0x0008;
}

//# 114 "/opt/tinyos-2.x/tos/chips/stm32/spi/Stm32Spi.nc"
inline static   void Stm32SpiP$Spi$setClock(uint8_t arg_0x40ae7168){
//#line 114
  HplStm32SpiP$SPI$setClock(arg_0x40ae7168);
//#line 114
}
//#line 114
//# 54 "/opt/tinyos-2.x/tos/system/FcfsResourceQueueC.nc"
static inline   bool /*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$FcfsQueue$isEnqueued(resource_client_id_t id)
//#line 54
{
  return /*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$resQ[id] != /*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$NO_ENTRY || /*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$qTail == id;
}

//#line 72
static inline   error_t /*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$FcfsQueue$enqueue(resource_client_id_t id)
//#line 72
{
  /* atomic removed: atomic calls only */
//#line 73
  {
    if (!/*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$FcfsQueue$isEnqueued(id)) {
        if (/*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$qHead == /*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$NO_ENTRY) {
          /*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$qHead = id;
          }
        else {
//#line 78
          /*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$resQ[/*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$qTail] = id;
          }
//#line 79
        /*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$qTail = id;
        {
          unsigned char __nesc_temp = 
//#line 80
          SUCCESS;

//#line 80
          return __nesc_temp;
        }
      }
//#line 82
    {
      unsigned char __nesc_temp = 
//#line 82
      EBUSY;

//#line 82
      return __nesc_temp;
    }
  }
}

//# 69 "/opt/tinyos-2.x/tos/interfaces/ResourceQueue.nc"
inline static   error_t /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$Queue$enqueue(resource_client_id_t arg_0x40b6a010){
//#line 69
  unsigned char result;
//#line 69

//#line 69
  result = /*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$FcfsQueue$enqueue(arg_0x40b6a010);
//#line 69

//#line 69
  return result;
//#line 69
}
//#line 69
//# 167 "/opt/tinyos-2.x/tos/system/SimpleArbiterP.nc"
static inline    void /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$ResourceRequested$default$requested(uint8_t id)
//#line 167
{
}

//# 43 "/opt/tinyos-2.x/tos/interfaces/ResourceRequested.nc"
inline static   void /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$ResourceRequested$requested(uint8_t arg_0x40b5f688){
//#line 43
    /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$ResourceRequested$default$requested(arg_0x40b5f688);
//#line 43
}
//#line 43
//# 71 "/opt/tinyos-2.x/tos/system/SimpleArbiterP.nc"
static inline   error_t /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$Resource$request(uint8_t id)
//#line 71
{
  /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$ResourceRequested$requested(/*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$resId);
  { __nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
//#line 73
    {
      if (/*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$state == /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$RES_IDLE) {
          /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$state = /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$RES_GRANTING;
          /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$reqResId = id;
          /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$grantedTask$postTask();
          {
            unsigned char __nesc_temp = 
//#line 78
            SUCCESS;

            {
//#line 78
              __nesc_atomic_end(__nesc_atomic); 
//#line 78
              return __nesc_temp;
            }
          }
        }
//#line 80
      {
        unsigned char __nesc_temp = 
//#line 80
        /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$Queue$enqueue(id);

        {
//#line 80
          __nesc_atomic_end(__nesc_atomic); 
//#line 80
          return __nesc_temp;
        }
      }
    }
//#line 83
    __nesc_atomic_end(__nesc_atomic); }
}

//# 78 "/opt/tinyos-2.x/tos/interfaces/Resource.nc"
inline static   error_t Stm32SpiP$ResourceArbiter$request(uint8_t arg_0x40af20f8){
//#line 78
  unsigned char result;
//#line 78

//#line 78
  result = /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$Resource$request(arg_0x40af20f8);
//#line 78

//#line 78
  return result;
//#line 78
}
//#line 78
//# 52 "/opt/tinyos-2.x/tos/chips/stm32/timer/HplStm32Timer.nc"
inline static   /*CounterOne16C.NCounter*/Stm32CounterC$0$Timer$timer_size /*CounterOne16C.NCounter*/Stm32CounterC$0$Timer$get(void){
//#line 52
  unsigned short result;
//#line 52

//#line 52
  result = HplStm32Timer3P$Timer$get();
//#line 52

//#line 52
  return result;
//#line 52
}
//#line 52
//# 41 "/opt/tinyos-2.x/tos/chips/stm32/timer/Stm32CounterC.nc"
static inline   /*CounterOne16C.NCounter*/Stm32CounterC$0$timer_size /*CounterOne16C.NCounter*/Stm32CounterC$0$Counter$get(void)
{
  return /*CounterOne16C.NCounter*/Stm32CounterC$0$Timer$get();
}

//# 53 "/opt/tinyos-2.x/tos/lib/timer/Counter.nc"
inline static   /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$CounterFrom$size_type /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$CounterFrom$get(void){
//#line 53
  unsigned short result;
//#line 53

//#line 53
  result = /*CounterOne16C.NCounter*/Stm32CounterC$0$Counter$get();
//#line 53

//#line 53
  return result;
//#line 53
}
//#line 53
//# 62 "/opt/tinyos-2.x/tos/chips/stm32/timer/HplStm32Timer3P.nc"
static inline   bool HplStm32Timer3P$Timer$test(void)
//#line 62
{

  return TRUE;
}

//# 78 "/opt/tinyos-2.x/tos/chips/stm32/timer/HplStm32Timer.nc"
inline static   bool /*CounterOne16C.NCounter*/Stm32CounterC$0$Timer$test(void){
//#line 78
  unsigned char result;
//#line 78

//#line 78
  result = HplStm32Timer3P$Timer$test();
//#line 78

//#line 78
  return result;
//#line 78
}
//#line 78
//# 46 "/opt/tinyos-2.x/tos/chips/stm32/timer/Stm32CounterC.nc"
static inline   bool /*CounterOne16C.NCounter*/Stm32CounterC$0$Counter$isOverflowPending(void)
{
  return /*CounterOne16C.NCounter*/Stm32CounterC$0$Timer$test();
}

//# 60 "/opt/tinyos-2.x/tos/lib/timer/Counter.nc"
inline static   bool /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$CounterFrom$isOverflowPending(void){
//#line 60
  unsigned char result;
//#line 60

//#line 60
  result = /*CounterOne16C.NCounter*/Stm32CounterC$0$Counter$isOverflowPending();
//#line 60

//#line 60
  return result;
//#line 60
}
//#line 60
//# 102 "/opt/tinyos-2.x/tos/chips/stm32/timer/HplStm32Timer3P.nc"
static inline   void HplStm32Timer3P$CompareA$set(uint16_t t)
//#line 102
{
//#line 102
  TIM_SetCompare1((TIM_TypeDef *)((uint32_t )0x40000000 + 0x0400), t);
}

//# 45 "/opt/tinyos-2.x/tos/chips/stm32/timer/HplStm32Compare.nc"
inline static   void /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$HplStm32Compare$set(/*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$HplStm32Compare$size_type arg_0x40b83350){
//#line 45
  HplStm32Timer3P$CompareA$set(arg_0x40b83350);
//#line 45
}
//#line 45
//# 48 "/opt/tinyos-2.x/tos/chips/stm32/timer/HplStm32Timer3P.nc"
static inline   void HplStm32Timer3P$CompareA$reset(void)
//#line 48
{
}

//# 53 "/opt/tinyos-2.x/tos/chips/stm32/timer/HplStm32Compare.nc"
inline static   void /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$HplStm32Compare$reset(void){
//#line 53
  HplStm32Timer3P$CompareA$reset();
//#line 53
}
//#line 53
//# 53 "/opt/tinyos-2.x/tos/chips/stm32/timer/HplStm32Timer3P.nc"
static inline   void HplStm32Timer3P$CompareA$start(void)
//#line 53
{
}

//# 56 "/opt/tinyos-2.x/tos/chips/stm32/timer/HplStm32Compare.nc"
inline static   void /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$HplStm32Compare$start(void){
//#line 56
  HplStm32Timer3P$CompareA$start();
//#line 56
}
//#line 56
//# 61 "/opt/tinyos-2.x/tos/system/ActiveMessageAddressC.nc"
static inline   am_addr_t ActiveMessageAddressC$ActiveMessageAddress$amAddress(void)
//#line 61
{
  return ActiveMessageAddressC$amAddress();
}

//# 50 "/opt/tinyos-2.x/tos/interfaces/ActiveMessageAddress.nc"
inline static   am_addr_t IEEE154MessageLayerP$ActiveMessageAddress$amAddress(void){
//#line 50
  unsigned short result;
//#line 50

//#line 50
  result = ActiveMessageAddressC$ActiveMessageAddress$amAddress();
//#line 50

//#line 50
  return result;
//#line 50
}
//#line 50
//# 29 "/opt/tinyos-2.x/tos/chips/rf2xx/util/PacketFlag.nc"
inline static   bool RF230DriverLayerP$TransmitPowerFlag$get(message_t *arg_0x409bca38){
//#line 29
  unsigned char result;
//#line 29

//#line 29
  result = MetadataFlagsLayerC$PacketFlag$get(2U, arg_0x409bca38);
//#line 29

//#line 29
  return result;
//#line 29
}
//#line 29
//# 892 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230DriverLayerP.nc"
static inline   bool RF230DriverLayerP$PacketTransmitPower$isSet(message_t *msg)
{
  return RF230DriverLayerP$TransmitPowerFlag$get(msg);
}

static inline   uint8_t RF230DriverLayerP$PacketTransmitPower$get(message_t *msg)
{
  return RF230DriverLayerP$getMeta(msg)->power;
}

//# 52 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/IEEE154MessageLayer.nc"
inline static   bool RF230ActiveMessageP$IEEE154MessageLayer$isDataFrame(message_t *arg_0x407ba380){
//#line 52
  unsigned char result;
//#line 52

//#line 52
  result = IEEE154MessageLayerP$IEEE154MessageLayer$isDataFrame(arg_0x407ba380);
//#line 52

//#line 52
  return result;
//#line 52
}
//#line 52
//# 84 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230ActiveMessageP.nc"
static inline   bool RF230ActiveMessageP$RF230DriverConfig$requiresRssiCca(message_t *msg)
{
  return RF230ActiveMessageP$IEEE154MessageLayer$isDataFrame(msg);
}

//# 54 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230DriverConfig.nc"
inline static   bool RF230DriverLayerP$Config$requiresRssiCca(message_t *arg_0x407a74d8){
//#line 54
  unsigned char result;
//#line 54

//#line 54
  result = RF230ActiveMessageP$RF230DriverConfig$requiresRssiCca(arg_0x407a74d8);
//#line 54

//#line 54
  return result;
//#line 54
}
//#line 54
//# 29 "/opt/tinyos-2.x/tos/chips/rf2xx/util/PacketFlag.nc"
inline static   bool RF230DriverLayerP$TimeSyncFlag$get(message_t *arg_0x409bca38){
//#line 29
  unsigned char result;
//#line 29

//#line 29
  result = MetadataFlagsLayerC$PacketFlag$get(4U, arg_0x409bca38);
//#line 29

//#line 29
  return result;
//#line 29
}
//#line 29
//# 941 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230DriverLayerP.nc"
static inline   bool RF230DriverLayerP$PacketTimeSyncOffset$isSet(message_t *msg)
{
  return RF230DriverLayerP$TimeSyncFlag$get(msg);
}

static inline   uint8_t RF230DriverLayerP$PacketTimeSyncOffset$get(message_t *msg)
{
  return RF230DriverLayerP$RadioPacket$headerLength(msg) + RF230DriverLayerP$RadioPacket$payloadLength(msg) - sizeof(timesync_absolute_t );
}

//# 53 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioAlarm.nc"
inline static   uint16_t RF230DriverLayerP$RadioAlarm$getNow(void){
//#line 53
  unsigned short result;
//#line 53

//#line 53
  result = RadioAlarmP$RadioAlarm$getNow(3U);
//#line 53

//#line 53
  return result;
//#line 53
}
//#line 53
//# 301 "/usr/lib/ncc/nesc_nx.h"
static __inline uint32_t __nesc_hton_uint32(void *target, uint32_t value)
//#line 301
{
  uint8_t *base = (uint8_t *) target;

//#line 303
  base[3] = value;
  base[2] = value >> 8;
  base[1] = value >> 16;
  base[0] = value >> 24;
  return value;
}

//#line 326
static __inline int32_t __nesc_hton_int32(void *target, int32_t value)
//#line 326
{
//#line 326
  __nesc_hton_uint32(target, value);
//#line 326
  return value;
}

//#line 294
static __inline uint32_t __nesc_ntoh_uint32(const void *source)
//#line 294
{
  const uint8_t *base = (const uint8_t *) source;

//#line 296
  return ((((uint32_t )base[0] << 24) | (
  (uint32_t )base[1] << 16)) | (
  (uint32_t )base[2] << 8)) | base[3];
}

//#line 326
static __inline int32_t __nesc_ntoh_int32(const void *source)
//#line 326
{
//#line 326
  return __nesc_ntoh_uint32(source);
}

//# 56 "/opt/tinyos-2.x/tos/interfaces/TaskBasic.nc"
inline static   error_t RandomCollisionLayerP$calcNextRandom$postTask(void){
//#line 56
  unsigned char result;
//#line 56

//#line 56
  result = SchedulerBasicP$TaskBasic$postTask(RandomCollisionLayerP$calcNextRandom);
//#line 56

//#line 56
  return result;
//#line 56
}
//#line 56
//# 242 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230ActiveMessageP.nc"
static inline   uint16_t RF230ActiveMessageP$RandomCollisionConfig$getMinimumBackoff(void)
{
  return (uint16_t )(320 * (7372800UL / 1 / 32) * (1 << 2) / 1000000UL);
}

//# 40 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/RandomCollisionConfig.nc"
inline static   uint16_t RandomCollisionLayerP$Config$getMinimumBackoff(void){
//#line 40
  unsigned short result;
//#line 40

//#line 40
  result = RF230ActiveMessageP$RandomCollisionConfig$getMinimumBackoff();
//#line 40

//#line 40
  return result;
//#line 40
}
//#line 40
//# 294 "/opt/tinyos-2.x/tos/chips/stm32/spi/Stm32SpiP.nc"
static inline    
//#line 293
void Stm32SpiP$SpiPacket$default$sendDone(
uint8_t *_txbuffer, uint8_t *_rxbuffer, 
uint16_t _length, error_t _success)
//#line 295
{
}

//# 71 "/opt/tinyos-2.x/tos/interfaces/SpiPacket.nc"
inline static   void Stm32SpiP$SpiPacket$sendDone(uint8_t *arg_0x40ac47e8, uint8_t *arg_0x40ac4990, uint16_t arg_0x40ac4b20, error_t arg_0x40ac4cb8){
//#line 71
  Stm32SpiP$SpiPacket$default$sendDone(arg_0x40ac47e8, arg_0x40ac4990, arg_0x40ac4b20, arg_0x40ac4cb8);
//#line 71
}
//#line 71
//# 239 "/opt/tinyos-2.x/tos/chips/stm32/spi/Stm32SpiP.nc"
static inline  void Stm32SpiP$zeroTask$runTask(void)
//#line 239
{
  uint16_t myLen;
  uint8_t *rx;
  uint8_t *tx;

  { __nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
//#line 244
    {
      myLen = Stm32SpiP$len;
      rx = Stm32SpiP$rxBuffer;
      tx = Stm32SpiP$txBuffer;
      Stm32SpiP$rxBuffer = (uint8_t *)0;
      Stm32SpiP$txBuffer = (uint8_t *)0;
      Stm32SpiP$len = 0;
      Stm32SpiP$pos = 0;
      Stm32SpiP$SpiPacket$sendDone(tx, rx, myLen, SUCCESS);
    }
//#line 253
    __nesc_atomic_end(__nesc_atomic); }
}

//# 78 "/opt/tinyos-2.x/tos/system/RandomMlcgC.nc"
static inline   uint16_t RandomMlcgC$Random$rand16(void)
//#line 78
{
  return (uint16_t )RandomMlcgC$Random$rand32();
}

//# 41 "/opt/tinyos-2.x/tos/interfaces/Random.nc"
inline static   uint16_t RandomCollisionLayerP$Random$rand16(void){
//#line 41
  unsigned short result;
//#line 41

//#line 41
  result = RandomMlcgC$Random$rand16();
//#line 41

//#line 41
  return result;
//#line 41
}
//#line 41
//# 67 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/RandomCollisionLayerP.nc"
static inline  void RandomCollisionLayerP$calcNextRandom$runTask(void)
{
  uint16_t a = RandomCollisionLayerP$Random$rand16();

//#line 70
  { __nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
//#line 70
    RandomCollisionLayerP$nextRandom = a;
//#line 70
    __nesc_atomic_end(__nesc_atomic); }
}

//# 38 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioPacket.nc"
inline static   uint8_t MetadataFlagsLayerC$SubPacket$payloadLength(message_t *arg_0x407de250){
//#line 38
  unsigned char result;
//#line 38

//#line 38
  result = RF230DriverLayerP$RadioPacket$payloadLength(arg_0x407de250);
//#line 38

//#line 38
  return result;
//#line 38
}
//#line 38
//# 84 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/MetadataFlagsLayerC.nc"
static inline   uint8_t MetadataFlagsLayerC$RadioPacket$payloadLength(message_t *msg)
{
  return MetadataFlagsLayerC$SubPacket$payloadLength(msg);
}

//# 38 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioPacket.nc"
inline static   uint8_t TimeStampingLayerP$SubPacket$payloadLength(message_t *arg_0x407de250){
//#line 38
  unsigned char result;
//#line 38

//#line 38
  result = MetadataFlagsLayerC$RadioPacket$payloadLength(arg_0x407de250);
//#line 38

//#line 38
  return result;
//#line 38
}
//#line 38
//# 110 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/TimeStampingLayerP.nc"
static inline   uint8_t TimeStampingLayerP$RadioPacket$payloadLength(message_t *msg)
{
  return TimeStampingLayerP$SubPacket$payloadLength(msg);
}

//# 38 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioPacket.nc"
inline static   uint8_t IEEE154MessageLayerP$SubPacket$payloadLength(message_t *arg_0x407de250){
//#line 38
  unsigned char result;
//#line 38

//#line 38
  result = TimeStampingLayerP$RadioPacket$payloadLength(arg_0x407de250);
//#line 38

//#line 38
  return result;
//#line 38
}
//#line 38
//# 267 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/IEEE154MessageLayerP.nc"
static inline   uint8_t IEEE154MessageLayerP$RadioPacket$payloadLength(message_t *msg)
{
  return IEEE154MessageLayerP$SubPacket$payloadLength(msg) - sizeof(ieee154_header_t );
}

//# 38 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioPacket.nc"
inline static   uint8_t LowpanNetworkLayerC$SubPacket$payloadLength(message_t *arg_0x407de250){
//#line 38
  unsigned char result;
//#line 38

//#line 38
  result = IEEE154MessageLayerP$RadioPacket$payloadLength(arg_0x407de250);
//#line 38

//#line 38
  return result;
//#line 38
}
//#line 38
//# 99 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/LowpanNetworkLayerC.nc"
static inline   uint8_t LowpanNetworkLayerC$RadioPacket$payloadLength(message_t *msg)
{
  return LowpanNetworkLayerC$SubPacket$payloadLength(msg) - sizeof(lowpan_header_t );
}

//# 32 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioPacket.nc"
inline static   uint8_t LowpanNetworkLayerC$SubPacket$headerLength(message_t *arg_0x407c9c68){
//#line 32
  unsigned char result;
//#line 32

//#line 32
  result = IEEE154MessageLayerP$RadioPacket$headerLength(arg_0x407c9c68);
//#line 32

//#line 32
  return result;
//#line 32
}
//#line 32
//# 94 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/LowpanNetworkLayerC.nc"
static inline   uint8_t LowpanNetworkLayerC$RadioPacket$headerLength(message_t *msg)
{
  return LowpanNetworkLayerC$SubPacket$headerLength(msg) + sizeof(lowpan_header_t );
}

//#line 52
static inline void *LowpanNetworkLayerC$getPayload(message_t *msg)
{
  return (void *)msg + LowpanNetworkLayerC$RadioPacket$headerLength(msg);
}

//#line 87
static inline   message_t *LowpanNetworkLayerC$NonTinyosReceive$default$receive(uint8_t network, message_t *msg, void *payload, uint8_t len)
{
  return msg;
}

//# 67 "/opt/tinyos-2.x/tos/interfaces/Receive.nc"
inline static  message_t *LowpanNetworkLayerC$NonTinyosReceive$receive(uint8_t arg_0x40881068, message_t *arg_0x4070c340, void *arg_0x4070c4e0, uint8_t arg_0x4070c668){
//#line 67
  nx_struct message_t *result;
//#line 67

//#line 67
    result = LowpanNetworkLayerC$NonTinyosReceive$default$receive(arg_0x40881068, arg_0x4070c340, arg_0x4070c4e0, arg_0x4070c668);
//#line 67

//#line 67
  return result;
//#line 67
}
//#line 67
//# 127 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/ActiveMessageLayerP.nc"
static inline   message_t *ActiveMessageLayerP$Snoop$default$receive(am_id_t id, message_t *msg, void *payload, uint8_t len)
{
  return msg;
}

//# 67 "/opt/tinyos-2.x/tos/interfaces/Receive.nc"
inline static  message_t *ActiveMessageLayerP$Snoop$receive(am_id_t arg_0x40835228, message_t *arg_0x4070c340, void *arg_0x4070c4e0, uint8_t arg_0x4070c668){
//#line 67
  nx_struct message_t *result;
//#line 67

//#line 67
    result = ActiveMessageLayerP$Snoop$default$receive(arg_0x40835228, arg_0x4070c340, arg_0x4070c4e0, arg_0x4070c668);
//#line 67

//#line 67
  return result;
//#line 67
}
//#line 67
//# 235 "/usr/lib/ncc/nesc_nx.h"
static __inline uint8_t __nesc_ntoh_uint8(const void *source)
//#line 235
{
  const uint8_t *base = (const uint8_t *) source;

//#line 237
  return base[0];
}

//# 54 "STMTestC.nc"
static inline  void STMTestC$STMAppMessaging$Receive(uint8_t appID, uint8_t *payload, uint16_t len)
//#line 54
{
}

//# 7 "STMAppMessaging.nc"
inline static  void STMAppMessagingP$STMAppMessaging$Receive(uint8_t arg_0x406e3328, uint8_t *arg_0x406e34d0, uint16_t arg_0x406e3660){
//#line 7
  STMTestC$STMAppMessaging$Receive(arg_0x406e3328, arg_0x406e34d0, arg_0x406e3660);
//#line 7
}
//#line 7
//# 72 "STMAppMessagingP.nc"
static inline  message_t *STMAppMessagingP$Receive$receive(message_t *bufPtr, void *payload, uint8_t len)
//#line 72
{

  uint8_t i;
//#line 74
  uint8_t payloadLen;
  stm_app_msg_t *rcm = (stm_app_msg_t *)payload;

  payloadLen = len - AM_STM_APP_MSG_HDR_LEN;

  for (i = 0; i < payloadLen; i++) 
    STMAppMessagingP$rcvdMsg[i] = __nesc_ntoh_uint8((unsigned char *)&rcm->data[i]);


  STMAppMessagingP$STMAppMessaging$Receive(__nesc_ntoh_uint8((unsigned char *)&rcm->appId), &STMAppMessagingP$rcvdMsg[0], payloadLen);

  return bufPtr;
}

//# 122 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/ActiveMessageLayerP.nc"
static inline   message_t *ActiveMessageLayerP$Receive$default$receive(am_id_t id, message_t *msg, void *payload, uint8_t len)
{
  return msg;
}

//# 67 "/opt/tinyos-2.x/tos/interfaces/Receive.nc"
inline static  message_t *ActiveMessageLayerP$Receive$receive(am_id_t arg_0x40837c28, message_t *arg_0x4070c340, void *arg_0x4070c4e0, uint8_t arg_0x4070c668){
//#line 67
  nx_struct message_t *result;
//#line 67

//#line 67
  switch (arg_0x40837c28) {
//#line 67
    case 100:
//#line 67
      result = STMAppMessagingP$Receive$receive(arg_0x4070c340, arg_0x4070c4e0, arg_0x4070c668);
//#line 67
      break;
//#line 67
    default:
//#line 67
      result = ActiveMessageLayerP$Receive$default$receive(arg_0x40837c28, arg_0x4070c340, arg_0x4070c4e0, arg_0x4070c668);
//#line 67
      break;
//#line 67
    }
//#line 67

//#line 67
  return result;
//#line 67
}
//#line 67
//# 50 "/opt/tinyos-2.x/tos/interfaces/ActiveMessageAddress.nc"
inline static   am_addr_t ActiveMessageLayerP$ActiveMessageAddress$amAddress(void){
//#line 50
  unsigned short result;
//#line 50

//#line 50
  result = ActiveMessageAddressC$ActiveMessageAddress$amAddress();
//#line 50

//#line 50
  return result;
//#line 50
}
//#line 50
//# 134 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/ActiveMessageLayerP.nc"
static __inline  am_addr_t ActiveMessageLayerP$AMPacket$address(void)
{
  return ActiveMessageLayerP$ActiveMessageAddress$amAddress();
}

//# 130 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/IEEE154MessageLayer.nc"
inline static   uint16_t RF230ActiveMessageP$IEEE154MessageLayer$getDestAddr(message_t *arg_0x407cd930){
//#line 130
  unsigned short result;
//#line 130

//#line 130
  result = IEEE154MessageLayerP$IEEE154MessageLayer$getDestAddr(arg_0x407cd930);
//#line 130

//#line 130
  return result;
//#line 130
}
//#line 130
//# 163 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230ActiveMessageP.nc"
static inline  am_addr_t RF230ActiveMessageP$ActiveMessageConfig$destination(message_t *msg)
{
  return RF230ActiveMessageP$IEEE154MessageLayer$getDestAddr(msg);
}

//# 29 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/ActiveMessageConfig.nc"
inline static  am_addr_t ActiveMessageLayerP$Config$destination(message_t *arg_0x407c2eb8){
//#line 29
  unsigned short result;
//#line 29

//#line 29
  result = RF230ActiveMessageP$ActiveMessageConfig$destination(arg_0x407c2eb8);
//#line 29

//#line 29
  return result;
//#line 29
}
//#line 29
//# 150 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/ActiveMessageLayerP.nc"
static __inline  am_addr_t ActiveMessageLayerP$AMPacket$destination(message_t *msg)
{
  return ActiveMessageLayerP$Config$destination(msg);
}

//#line 144
static __inline  bool ActiveMessageLayerP$AMPacket$isForMe(message_t *msg)
{
  am_addr_t addr = ActiveMessageLayerP$AMPacket$destination(msg);

//#line 147
  return addr == ActiveMessageLayerP$AMPacket$address() || addr == AM_BROADCAST_ADDR;
}

//# 38 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioPacket.nc"
inline static   uint8_t ActiveMessageLayerP$SubPacket$payloadLength(message_t *arg_0x407de250){
//#line 38
  unsigned char result;
//#line 38

//#line 38
  result = LowpanNetworkLayerC$RadioPacket$payloadLength(arg_0x407de250);
//#line 38

//#line 38
  return result;
//#line 38
}
//#line 38
//# 201 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/ActiveMessageLayerP.nc"
static inline   uint8_t ActiveMessageLayerP$RadioPacket$payloadLength(message_t *msg)
{
  return ActiveMessageLayerP$SubPacket$payloadLength(msg) - sizeof(activemessage_header_t );
}

//#line 233
static inline  uint8_t ActiveMessageLayerP$Packet$payloadLength(message_t *msg)
{
  return ActiveMessageLayerP$RadioPacket$payloadLength(msg);
}

//#line 56
static inline void *ActiveMessageLayerP$getPayload(message_t *msg)
{
  return (void *)msg + ActiveMessageLayerP$RadioPacket$headerLength(msg);
}

//# 32 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioPacket.nc"
inline static   uint8_t ActiveMessageLayerP$SubPacket$headerLength(message_t *arg_0x407c9c68){
//#line 32
  unsigned char result;
//#line 32

//#line 32
  result = LowpanNetworkLayerC$RadioPacket$headerLength(arg_0x407c9c68);
//#line 32

//#line 32
  return result;
//#line 32
}
//#line 32
//# 51 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/ActiveMessageLayerP.nc"
static inline activemessage_header_t *ActiveMessageLayerP$getHeader(message_t *msg)
{
  return (activemessage_header_t *) ((void *)msg + ActiveMessageLayerP$SubPacket$headerLength(msg));
}

//#line 170
static __inline  am_id_t ActiveMessageLayerP$AMPacket$type(message_t *msg)
{
  return __nesc_ntoh_leuint8((unsigned char *)&ActiveMessageLayerP$getHeader(msg)->type);
}

//#line 109
static inline  message_t *ActiveMessageLayerP$SubReceive$receive(message_t *msg)
{
  am_id_t id = ActiveMessageLayerP$AMPacket$type(msg);
  void *payload = ActiveMessageLayerP$getPayload(msg);
  uint8_t len = ActiveMessageLayerP$Packet$payloadLength(msg);

  msg = ActiveMessageLayerP$AMPacket$isForMe(msg) ? 
  ActiveMessageLayerP$Receive$receive(id, msg, payload, len) : 
  ActiveMessageLayerP$Snoop$receive(id, msg, payload, len);

  return msg;
}

//# 31 "/opt/tinyos-2.x/tos/chips/rf2xx/util/BareReceive.nc"
inline static  message_t *LowpanNetworkLayerC$Receive$receive(message_t *arg_0x4083b1f8){
//#line 31
  nx_struct message_t *result;
//#line 31

//#line 31
  result = ActiveMessageLayerP$SubReceive$receive(arg_0x4083b1f8);
//#line 31

//#line 31
  return result;
//#line 31
}
//#line 31
//# 47 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/LowpanNetworkLayerC.nc"
static inline lowpan_header_t *LowpanNetworkLayerC$getHeader(message_t *msg)
{
  return (lowpan_header_t *) ((void *)msg + LowpanNetworkLayerC$SubPacket$headerLength(msg));
}

//#line 77
static inline  message_t *LowpanNetworkLayerC$SubReceive$receive(message_t *msg)
{
  uint8_t network = __nesc_ntoh_leuint8((unsigned char *)&LowpanNetworkLayerC$getHeader(msg)->network);

//#line 80
  if (network == 0x3f) {
    return LowpanNetworkLayerC$Receive$receive(msg);
    }
  else {
//#line 83
    return LowpanNetworkLayerC$NonTinyosReceive$receive(network, msg, 
    LowpanNetworkLayerC$getPayload(msg), LowpanNetworkLayerC$RadioPacket$payloadLength(msg));
    }
}

//# 31 "/opt/tinyos-2.x/tos/chips/rf2xx/util/BareReceive.nc"
inline static  message_t *MessageBufferLayerP$Receive$receive(message_t *arg_0x4083b1f8){
//#line 31
  nx_struct message_t *result;
//#line 31

//#line 31
  result = LowpanNetworkLayerC$SubReceive$receive(arg_0x4083b1f8);
//#line 31

//#line 31
  return result;
//#line 31
}
//#line 31
//# 280 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/MessageBufferLayerP.nc"
static inline  void MessageBufferLayerP$deliverTask$runTask(void)
{

  for (; ; ) 
    {
      message_t *msg;

      { __nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
        {
          if (MessageBufferLayerP$receiveQueueSize == 0) {
            {
//#line 290
              __nesc_atomic_end(__nesc_atomic); 
//#line 290
              return;
            }
            }
//#line 292
          msg = MessageBufferLayerP$receiveQueue[MessageBufferLayerP$receiveQueueHead];
        }
//#line 293
        __nesc_atomic_end(__nesc_atomic); }

      msg = MessageBufferLayerP$Receive$receive(msg);

      { __nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
        {
          MessageBufferLayerP$receiveQueue[MessageBufferLayerP$receiveQueueHead] = msg;

          if (++MessageBufferLayerP$receiveQueueHead >= MessageBufferLayerP$RECEIVE_QUEUE_SIZE) {
            MessageBufferLayerP$receiveQueueHead = 0;
            }
          --MessageBufferLayerP$receiveQueueSize;
        }
//#line 305
        __nesc_atomic_end(__nesc_atomic); }
    }
}

//# 68 "STMAppMessagingP.nc"
static inline  void STMAppMessagingP$AMSend$sendDone(message_t *bufPtr, error_t error)
//#line 68
{
}

//# 99 "/opt/tinyos-2.x/tos/interfaces/AMSend.nc"
inline static  void /*STMAppMessagingC.AMSenderC.AMQueueEntryP*/AMQueueEntryP$0$AMSend$sendDone(message_t *arg_0x407101c0, error_t arg_0x40710348){
//#line 99
  STMAppMessagingP$AMSend$sendDone(arg_0x407101c0, arg_0x40710348);
//#line 99
}
//#line 99
//# 57 "/opt/tinyos-2.x/tos/system/AMQueueEntryP.nc"
static inline  void /*STMAppMessagingC.AMSenderC.AMQueueEntryP*/AMQueueEntryP$0$Send$sendDone(message_t *m, error_t err)
//#line 57
{
  /*STMAppMessagingC.AMSenderC.AMQueueEntryP*/AMQueueEntryP$0$AMSend$sendDone(m, err);
}

//# 207 "/opt/tinyos-2.x/tos/system/AMQueueImplP.nc"
static inline   void /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$Send$default$sendDone(uint8_t id, message_t *msg, error_t err)
//#line 207
{
}

//# 89 "/opt/tinyos-2.x/tos/interfaces/Send.nc"
inline static  void /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$Send$sendDone(uint8_t arg_0x40746288, message_t *arg_0x40728e18, error_t arg_0x4073d010){
//#line 89
  switch (arg_0x40746288) {
//#line 89
    case 0U:
//#line 89
      /*STMAppMessagingC.AMSenderC.AMQueueEntryP*/AMQueueEntryP$0$Send$sendDone(arg_0x40728e18, arg_0x4073d010);
//#line 89
      break;
//#line 89
    default:
//#line 89
      /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$Send$default$sendDone(arg_0x40746288, arg_0x40728e18, arg_0x4073d010);
//#line 89
      break;
//#line 89
    }
//#line 89
}
//#line 89
//# 155 "/opt/tinyos-2.x/tos/system/AMQueueImplP.nc"
static inline void /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$sendDone(uint8_t last, message_t *msg, error_t err)
//#line 155
{
  /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$queue[last].msg = (message_t *)0;
  /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$tryToSend();
  /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$Send$sendDone(last, msg, err);
}

//#line 181
static inline  void /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$AMSend$sendDone(am_id_t id, message_t *msg, error_t err)
//#line 181
{





  if (/*AMQueueP.AMQueueImplP*/AMQueueImplP$0$current >= 1) {
      return;
    }
  if (/*AMQueueP.AMQueueImplP*/AMQueueImplP$0$queue[/*AMQueueP.AMQueueImplP*/AMQueueImplP$0$current].msg == msg) {
      /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$sendDone(/*AMQueueP.AMQueueImplP*/AMQueueImplP$0$current, msg, err);
    }
  else {

      ;
    }
}

//# 99 "/opt/tinyos-2.x/tos/interfaces/AMSend.nc"
inline static  void ActiveMessageLayerP$AMSend$sendDone(am_id_t arg_0x408372f8, message_t *arg_0x407101c0, error_t arg_0x40710348){
//#line 99
  /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$AMSend$sendDone(arg_0x408372f8, arg_0x407101c0, arg_0x40710348);
//#line 99
}
//#line 99
//# 79 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/ActiveMessageLayerP.nc"
static __inline  void ActiveMessageLayerP$SubSend$sendDone(message_t *msg, error_t error)
{
  ActiveMessageLayerP$AMSend$sendDone(ActiveMessageLayerP$AMPacket$type(msg), msg, error);
}

//# 43 "/opt/tinyos-2.x/tos/chips/rf2xx/util/BareSend.nc"
inline static  void LowpanNetworkLayerC$Send$sendDone(message_t *arg_0x408221f8, error_t arg_0x40822380){
//#line 43
  ActiveMessageLayerP$SubSend$sendDone(arg_0x408221f8, arg_0x40822380);
//#line 43
}
//#line 43
//# 72 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/LowpanNetworkLayerC.nc"
static inline  void LowpanNetworkLayerC$SubSend$sendDone(message_t *msg, error_t error)
{
  LowpanNetworkLayerC$Send$sendDone(msg, error);
}

//# 43 "/opt/tinyos-2.x/tos/chips/rf2xx/util/BareSend.nc"
inline static  void IEEE154MessageLayerP$Send$sendDone(message_t *arg_0x408221f8, error_t arg_0x40822380){
//#line 43
  LowpanNetworkLayerC$SubSend$sendDone(arg_0x408221f8, arg_0x40822380);
//#line 43
}
//#line 43
//# 359 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/IEEE154MessageLayerP.nc"
static inline   void IEEE154MessageLayerP$Ieee154Send$default$sendDone(message_t *msg, error_t error)
{
}

//# 85 "/opt/tinyos-2.x/tos/chips/rf2xx/util/Ieee154Send.nc"
inline static  void IEEE154MessageLayerP$Ieee154Send$sendDone(message_t *arg_0x40898aa8, error_t arg_0x40898c30){
//#line 85
  IEEE154MessageLayerP$Ieee154Send$default$sendDone(arg_0x40898aa8, arg_0x40898c30);
//#line 85
}
//#line 85
//# 383 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/IEEE154MessageLayerP.nc"
static inline  void IEEE154MessageLayerP$SubSend$sendDone(message_t *msg, error_t error)
{

  IEEE154MessageLayerP$Ieee154Send$sendDone(msg, error);
  IEEE154MessageLayerP$Send$sendDone(msg, error);
}

//# 43 "/opt/tinyos-2.x/tos/chips/rf2xx/util/BareSend.nc"
inline static  void UniqueLayerP$Send$sendDone(message_t *arg_0x408221f8, error_t arg_0x40822380){
//#line 43
  IEEE154MessageLayerP$SubSend$sendDone(arg_0x408221f8, arg_0x40822380);
//#line 43
}
//#line 43
//# 69 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/UniqueLayerP.nc"
static inline  void UniqueLayerP$SubSend$sendDone(message_t *msg, error_t error)
{
  UniqueLayerP$Send$sendDone(msg, error);
}

//# 43 "/opt/tinyos-2.x/tos/chips/rf2xx/util/BareSend.nc"
inline static  void MessageBufferLayerP$Send$sendDone(message_t *arg_0x408221f8, error_t arg_0x40822380){
//#line 43
  UniqueLayerP$SubSend$sendDone(arg_0x408221f8, arg_0x40822380);
//#line 43
}
//#line 43
//# 68 "/opt/tinyos-2.x/tos/chips/rf2xx/util/Tasklet.nc"
inline static  void MessageBufferLayerP$Tasklet$resume(void){
//#line 68
  TaskletC$Tasklet$resume();
//#line 68
}
//#line 68
//# 247 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230ActiveMessageP.nc"
static inline   uint16_t RF230ActiveMessageP$RandomCollisionConfig$getInitialBackoff(message_t *msg)
{
  return (uint16_t )(9920 * (7372800UL / 1 / 32) * (1 << 2) / 1000000UL);
}

//# 29 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/RandomCollisionConfig.nc"
inline static   uint16_t RandomCollisionLayerP$Config$getInitialBackoff(message_t *arg_0x407af630){
//#line 29
  unsigned short result;
//#line 29

//#line 29
  result = RF230ActiveMessageP$RandomCollisionConfig$getInitialBackoff(arg_0x407af630);
//#line 29

//#line 29
  return result;
//#line 29
}
//#line 29
//# 87 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/RandomCollisionLayerP.nc"
static inline   error_t RandomCollisionLayerP$RadioSend$send(message_t *msg)
{
  if (RandomCollisionLayerP$state != RandomCollisionLayerP$STATE_READY || !RandomCollisionLayerP$RadioAlarm$isFree()) {
    return EBUSY;
    }
  RandomCollisionLayerP$txMsg = msg;
  RandomCollisionLayerP$state = RandomCollisionLayerP$STATE_TX_PENDING_FIRST;
  RandomCollisionLayerP$RadioAlarm$wait(RandomCollisionLayerP$getBackoff(RandomCollisionLayerP$Config$getInitialBackoff(msg)));

  return SUCCESS;
}

//# 37 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioSend.nc"
inline static   error_t MessageBufferLayerP$RadioSend$send(message_t *arg_0x40937190){
//#line 37
  unsigned char result;
//#line 37

//#line 37
  result = RandomCollisionLayerP$RadioSend$send(arg_0x40937190);
//#line 37

//#line 37
  return result;
//#line 37
}
//#line 37
//# 83 "/opt/tinyos-2.x/tos/chips/rf2xx/util/TaskletC.nc"
static __inline  void TaskletC$Tasklet$suspend(void)
{
  { __nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
//#line 85
    ++TaskletC$state;
//#line 85
    __nesc_atomic_end(__nesc_atomic); }
}

//# 61 "/opt/tinyos-2.x/tos/chips/rf2xx/util/Tasklet.nc"
inline static  void MessageBufferLayerP$Tasklet$suspend(void){
//#line 61
  TaskletC$Tasklet$suspend();
//#line 61
}
//#line 61
//# 170 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/MessageBufferLayerP.nc"
static inline  void MessageBufferLayerP$sendTask$runTask(void)
{
  error_t error;

  for (; 0; ) ;

  { __nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
//#line 176
    error = MessageBufferLayerP$txError;
//#line 176
    __nesc_atomic_end(__nesc_atomic); }
  if ((MessageBufferLayerP$state == MessageBufferLayerP$STATE_TX_SEND && error == SUCCESS) || ++MessageBufferLayerP$retries > MessageBufferLayerP$MAX_RETRIES) {
    MessageBufferLayerP$state = MessageBufferLayerP$STATE_TX_DONE;
    }
  else {
      MessageBufferLayerP$Tasklet$suspend();

      error = MessageBufferLayerP$RadioSend$send(MessageBufferLayerP$txMsg);
      if (error == SUCCESS) {
        MessageBufferLayerP$state = MessageBufferLayerP$STATE_TX_SEND;
        }
      else {
//#line 186
        if (MessageBufferLayerP$retries == MessageBufferLayerP$MAX_RETRIES) {
          MessageBufferLayerP$state = MessageBufferLayerP$STATE_TX_DONE;
          }
        else {
//#line 189
          MessageBufferLayerP$state = MessageBufferLayerP$STATE_TX_PENDING;
          }
        }
//#line 191
      MessageBufferLayerP$Tasklet$resume();
    }

  if (MessageBufferLayerP$state == MessageBufferLayerP$STATE_TX_DONE) 
    {
      MessageBufferLayerP$state = MessageBufferLayerP$STATE_READY;
      MessageBufferLayerP$Send$sendDone(MessageBufferLayerP$txMsg, error);
    }
}

//# 57 "/opt/tinyos-2.x/tos/system/AMQueueImplP.nc"
static inline void /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$nextPacket(void)
//#line 57
{
  uint8_t i;

//#line 59
  /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$current = (/*AMQueueP.AMQueueImplP*/AMQueueImplP$0$current + 1) % 1;
  for (i = 0; i < 1; i++) {
      if (/*AMQueueP.AMQueueImplP*/AMQueueImplP$0$queue[/*AMQueueP.AMQueueImplP*/AMQueueImplP$0$current].msg == (void *)0 || 
      /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$cancelMask[/*AMQueueP.AMQueueImplP*/AMQueueImplP$0$current / 8] & (1 << /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$current % 8)) 
        {
          /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$current = (/*AMQueueP.AMQueueImplP*/AMQueueImplP$0$current + 1) % 1;
        }
      else {
          break;
        }
    }
  if (i >= 1) {
//#line 70
    /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$current = 1;
    }
}

//# 67 "/opt/tinyos-2.x/tos/interfaces/Packet.nc"
inline static  uint8_t /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$Packet$payloadLength(message_t *arg_0x4070aaf0){
//#line 67
  unsigned char result;
//#line 67

//#line 67
  result = ActiveMessageLayerP$Packet$payloadLength(arg_0x4070aaf0);
//#line 67

//#line 67
  return result;
//#line 67
}
//#line 67
//# 189 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/IEEE154MessageLayerP.nc"
static inline   void IEEE154MessageLayerP$IEEE154MessageLayer$setSrcAddr(message_t *msg, uint16_t addr)
{
  __nesc_hton_leuint16((unsigned char *)&IEEE154MessageLayerP$getHeader(msg)->src, addr);
}

//# 145 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/IEEE154MessageLayer.nc"
inline static   void RF230ActiveMessageP$IEEE154MessageLayer$setSrcAddr(message_t *arg_0x407cca48, uint16_t arg_0x407ccbd8){
//#line 145
  IEEE154MessageLayerP$IEEE154MessageLayer$setSrcAddr(arg_0x407cca48, arg_0x407ccbd8);
//#line 145
}
//#line 145
//# 178 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230ActiveMessageP.nc"
static inline  void RF230ActiveMessageP$ActiveMessageConfig$setSource(message_t *msg, am_addr_t addr)
{
  RF230ActiveMessageP$IEEE154MessageLayer$setSrcAddr(msg, addr);
}

//# 38 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/ActiveMessageConfig.nc"
inline static  void ActiveMessageLayerP$Config$setSource(message_t *arg_0x407c1ee0, am_addr_t arg_0x407c0088){
//#line 38
  RF230ActiveMessageP$ActiveMessageConfig$setSource(arg_0x407c1ee0, arg_0x407c0088);
//#line 38
}
//#line 38
//# 165 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/ActiveMessageLayerP.nc"
static __inline  void ActiveMessageLayerP$AMPacket$setSource(message_t *msg, am_addr_t addr)
{
  ActiveMessageLayerP$Config$setSource(msg, addr);
}

//# 169 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/IEEE154MessageLayerP.nc"
static inline   void IEEE154MessageLayerP$IEEE154MessageLayer$setDestPan(message_t *msg, uint16_t pan)
{
  __nesc_hton_leuint16((unsigned char *)&IEEE154MessageLayerP$getHeader(msg)->destpan, pan);
}

//# 125 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/IEEE154MessageLayer.nc"
inline static   void RF230ActiveMessageP$IEEE154MessageLayer$setDestPan(message_t *arg_0x407cd278, uint16_t arg_0x407cd408){
//#line 125
  IEEE154MessageLayerP$IEEE154MessageLayer$setDestPan(arg_0x407cd278, arg_0x407cd408);
//#line 125
}
//#line 125
//# 188 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230ActiveMessageP.nc"
static inline  void RF230ActiveMessageP$ActiveMessageConfig$setGroup(message_t *msg, am_group_t grp)
{
  RF230ActiveMessageP$IEEE154MessageLayer$setDestPan(msg, grp);
}

//# 44 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/ActiveMessageConfig.nc"
inline static  void ActiveMessageLayerP$Config$setGroup(message_t *arg_0x407c0a38, am_group_t arg_0x407c0bc8){
//#line 44
  RF230ActiveMessageP$ActiveMessageConfig$setGroup(arg_0x407c0a38, arg_0x407c0bc8);
//#line 44
}
//#line 44
//# 185 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/ActiveMessageLayerP.nc"
static __inline  void ActiveMessageLayerP$AMPacket$setGroup(message_t *msg, am_group_t grp)
{
  ActiveMessageLayerP$Config$setGroup(msg, grp);
}

//# 82 "/opt/tinyos-2.x/tos/system/ActiveMessageAddressC.nc"
static inline   am_group_t ActiveMessageAddressC$ActiveMessageAddress$amGroup(void)
//#line 82
{
  am_group_t myGroup;

//#line 84
  { __nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
//#line 84
    myGroup = ActiveMessageAddressC$group;
//#line 84
    __nesc_atomic_end(__nesc_atomic); }
  return myGroup;
}

//# 55 "/opt/tinyos-2.x/tos/interfaces/ActiveMessageAddress.nc"
inline static   am_group_t ActiveMessageLayerP$ActiveMessageAddress$amGroup(void){
//#line 55
  unsigned char result;
//#line 55

//#line 55
  result = ActiveMessageAddressC$ActiveMessageAddress$amGroup();
//#line 55

//#line 55
  return result;
//#line 55
}
//#line 55
//# 139 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/ActiveMessageLayerP.nc"
static __inline  am_group_t ActiveMessageLayerP$AMPacket$localGroup(void)
{
  return ActiveMessageLayerP$ActiveMessageAddress$amGroup();
}

//#line 103
static inline   void ActiveMessageLayerP$SendNotifier$default$aboutToSend(am_id_t id, am_addr_t addr, message_t *msg)
{
}

//# 59 "/opt/tinyos-2.x/tos/interfaces/SendNotifier.nc"
inline static  void ActiveMessageLayerP$SendNotifier$aboutToSend(am_id_t arg_0x408357e0, am_addr_t arg_0x4078f478, message_t *arg_0x4078f628){
//#line 59
    ActiveMessageLayerP$SendNotifier$default$aboutToSend(arg_0x408357e0, arg_0x4078f478, arg_0x4078f628);
//#line 59
}
//#line 59
//# 209 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/MessageBufferLayerP.nc"
static inline  error_t MessageBufferLayerP$Send$send(message_t *msg)
{
  if (MessageBufferLayerP$state != MessageBufferLayerP$STATE_READY) {
    return EBUSY;
    }
  MessageBufferLayerP$txMsg = msg;
  MessageBufferLayerP$state = MessageBufferLayerP$STATE_TX_PENDING;
  MessageBufferLayerP$retries = 0;
  MessageBufferLayerP$sendTask$postTask();

  return SUCCESS;
}

//# 35 "/opt/tinyos-2.x/tos/chips/rf2xx/util/BareSend.nc"
inline static  error_t UniqueLayerP$SubSend$send(message_t *arg_0x40825b10){
//#line 35
  unsigned char result;
//#line 35

//#line 35
  result = MessageBufferLayerP$Send$send(arg_0x40825b10);
//#line 35

//#line 35
  return result;
//#line 35
}
//#line 35
//# 159 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/IEEE154MessageLayerP.nc"
static inline   void IEEE154MessageLayerP$IEEE154MessageLayer$setDSN(message_t *msg, uint8_t dsn)
{
  __nesc_hton_leuint8((unsigned char *)&IEEE154MessageLayerP$getHeader(msg)->dsn, dsn);
}

//# 114 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/IEEE154MessageLayer.nc"
inline static   void RF230ActiveMessageP$IEEE154MessageLayer$setDSN(message_t *arg_0x407cf690, uint8_t arg_0x407cf818){
//#line 114
  IEEE154MessageLayerP$IEEE154MessageLayer$setDSN(arg_0x407cf690, arg_0x407cf818);
//#line 114
}
//#line 114
//# 144 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230ActiveMessageP.nc"
static inline   void RF230ActiveMessageP$UniqueConfig$setSequenceNumber(message_t *msg, uint8_t dsn)
{
  RF230ActiveMessageP$IEEE154MessageLayer$setDSN(msg, dsn);
}

//# 41 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/UniqueConfig.nc"
inline static   void UniqueLayerP$UniqueConfig$setSequenceNumber(message_t *arg_0x407b8690, uint8_t arg_0x407b8818){
//#line 41
  RF230ActiveMessageP$UniqueConfig$setSequenceNumber(arg_0x407b8690, arg_0x407b8818);
//#line 41
}
//#line 41
//# 58 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/UniqueLayerP.nc"
static inline  error_t UniqueLayerP$Send$send(message_t *msg)
{
  UniqueLayerP$UniqueConfig$setSequenceNumber(msg, ++UniqueLayerP$sequenceNumber);
  return UniqueLayerP$SubSend$send(msg);
}

//# 35 "/opt/tinyos-2.x/tos/chips/rf2xx/util/BareSend.nc"
inline static  error_t IEEE154MessageLayerP$SubSend$send(message_t *arg_0x40825b10){
//#line 35
  unsigned char result;
//#line 35

//#line 35
  result = UniqueLayerP$Send$send(arg_0x40825b10);
//#line 35

//#line 35
  return result;
//#line 35
}
//#line 35
//# 95 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/IEEE154MessageLayerP.nc"
static inline   void IEEE154MessageLayerP$IEEE154MessageLayer$createDataFrame(message_t *msg)
{
  __nesc_hton_leuint16((unsigned char *)&IEEE154MessageLayerP$getHeader(msg)->fcf, IEEE154MessageLayerP$IEEE154_DATA_FRAME_VALUE);
}

//#line 369
static inline  error_t IEEE154MessageLayerP$Send$send(message_t *msg)
{

  if (!IEEE154MessageLayerP$IEEE154MessageLayer$isDataFrame(msg)) {
    IEEE154MessageLayerP$IEEE154MessageLayer$createDataFrame(msg);
    }
  return IEEE154MessageLayerP$SubSend$send(msg);
}

//# 35 "/opt/tinyos-2.x/tos/chips/rf2xx/util/BareSend.nc"
inline static  error_t LowpanNetworkLayerC$SubSend$send(message_t *arg_0x40825b10){
//#line 35
  unsigned char result;
//#line 35

//#line 35
  result = IEEE154MessageLayerP$Send$send(arg_0x40825b10);
//#line 35

//#line 35
  return result;
//#line 35
}
//#line 35
//# 61 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/LowpanNetworkLayerC.nc"
static inline  error_t LowpanNetworkLayerC$Send$send(message_t *msg)
{
  __nesc_hton_leuint8((unsigned char *)&LowpanNetworkLayerC$getHeader(msg)->network, 0x3f);
  return LowpanNetworkLayerC$SubSend$send(msg);
}

//# 35 "/opt/tinyos-2.x/tos/chips/rf2xx/util/BareSend.nc"
inline static  error_t ActiveMessageLayerP$SubSend$send(message_t *arg_0x40825b10){
//#line 35
  unsigned char result;
//#line 35

//#line 35
  result = LowpanNetworkLayerC$Send$send(arg_0x40825b10);
//#line 35

//#line 35
  return result;
//#line 35
}
//#line 35
//# 56 "/opt/tinyos-2.x/tos/interfaces/TaskBasic.nc"
inline static   error_t /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$errorTask$postTask(void){
//#line 56
  unsigned char result;
//#line 56

//#line 56
  result = SchedulerBasicP$TaskBasic$postTask(/*AMQueueP.AMQueueImplP*/AMQueueImplP$0$errorTask);
//#line 56

//#line 56
  return result;
//#line 56
}
//#line 56
//# 157 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/MessageBufferLayerP.nc"
static inline   void MessageBufferLayerP$RadioChannel$default$setChannelDone(void)
{
}

//# 37 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioChannel.nc"
inline static  void MessageBufferLayerP$RadioChannel$setChannelDone(void){
//#line 37
  MessageBufferLayerP$RadioChannel$default$setChannelDone();
//#line 37
}
//#line 37
//# 39 "STMAppMessagingP.nc"
static inline  void STMAppMessagingP$AMControl$stopDone(error_t err)
//#line 39
{
}

//# 117 "/opt/tinyos-2.x/tos/interfaces/SplitControl.nc"
inline static  void MessageBufferLayerP$SplitControl$stopDone(error_t arg_0x406dd6e8){
//#line 117
  STMAppMessagingP$AMControl$stopDone(arg_0x406dd6e8);
//#line 117
}
//#line 117
//#line 83
inline static  error_t STMAppMessagingP$AMControl$start(void){
//#line 83
  unsigned char result;
//#line 83

//#line 83
  result = MessageBufferLayerP$SplitControl$start();
//#line 83

//#line 83
  return result;
//#line 83
}
//#line 83
//# 29 "STMAppMessagingP.nc"
static inline  void STMAppMessagingP$AMControl$startDone(error_t err)
//#line 29
{
  if (err == SUCCESS) {
      STMAppMessagingP$radioStarted = 1;
    }
  else 
    {
      STMAppMessagingP$AMControl$start();
    }
}

//# 92 "/opt/tinyos-2.x/tos/interfaces/SplitControl.nc"
inline static  void MessageBufferLayerP$SplitControl$startDone(error_t arg_0x406deb10){
//#line 92
  STMAppMessagingP$AMControl$startDone(arg_0x406deb10);
//#line 92
}
//#line 92
//# 125 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/MessageBufferLayerP.nc"
static inline  void MessageBufferLayerP$stateDoneTask$runTask(void)
{
  uint8_t s;

  s = MessageBufferLayerP$state;


  MessageBufferLayerP$state = MessageBufferLayerP$STATE_READY;

  if (s == MessageBufferLayerP$STATE_TURN_ON) {
    MessageBufferLayerP$SplitControl$startDone(SUCCESS);
    }
  else {
//#line 136
    if (s == MessageBufferLayerP$STATE_TURN_OFF) {
      MessageBufferLayerP$SplitControl$stopDone(SUCCESS);
      }
    else {
//#line 138
      if (s == MessageBufferLayerP$STATE_CHANNEL) {
        MessageBufferLayerP$RadioChannel$setChannelDone();
        }
      else {
//#line 141
        MessageBufferLayerP$state = s;
        }
      }
    }
}

//# 400 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230DriverLayerP.nc"
static inline   error_t RF230DriverLayerP$RadioState$turnOn(void)
{
  if (RF230DriverLayerP$cmd != RF230DriverLayerP$CMD_NONE || (RF230DriverLayerP$state == RF230DriverLayerP$STATE_SLEEP && !RF230DriverLayerP$RadioAlarm$isFree())) {
    return EBUSY;
    }
  else {
//#line 404
    if (RF230DriverLayerP$state == RF230DriverLayerP$STATE_RX_ON) {
      return EALREADY;
      }
    }
//#line 407
  RF230DriverLayerP$cmd = RF230DriverLayerP$CMD_TURNON;
  RF230DriverLayerP$Tasklet$schedule();

  return SUCCESS;
}

//# 45 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioState.nc"
inline static   error_t MessageBufferLayerP$RadioState$turnOn(void){
//#line 45
  unsigned char result;
//#line 45

//#line 45
  result = RF230DriverLayerP$RadioState$turnOn();
//#line 45

//#line 45
  return result;
//#line 45
}
//#line 45
//# 118 "/opt/tinyos-2.x/tos/system/AMQueueImplP.nc"
static inline  void /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$CancelTask$runTask(void)
//#line 118
{
  uint8_t i;
//#line 119
  uint8_t j;
//#line 119
  uint8_t mask;
//#line 119
  uint8_t last;
  message_t *msg;

//#line 121
  for (i = 0; i < 1 / 8 + 1; i++) {
      if (/*AMQueueP.AMQueueImplP*/AMQueueImplP$0$cancelMask[i]) {
          for (mask = 1, j = 0; j < 8; j++) {
              if (/*AMQueueP.AMQueueImplP*/AMQueueImplP$0$cancelMask[i] & mask) {
                  last = i * 8 + j;
                  msg = /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$queue[last].msg;
                  /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$queue[last].msg = (message_t *)0;
                  /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$cancelMask[i] &= ~mask;
                  /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$Send$sendDone(last, msg, ECANCEL);
                }
              mask <<= 1;
            }
        }
    }
}

//#line 161
static inline  void /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$errorTask$runTask(void)
//#line 161
{
  /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$sendDone(/*AMQueueP.AMQueueImplP*/AMQueueImplP$0$current, /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$queue[/*AMQueueP.AMQueueImplP*/AMQueueImplP$0$current].msg, FAIL);
}

//# 98 "/opt/tinyos-2.x/tos/lib/timer/Alarm.nc"
inline static   /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Alarm$size_type /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Alarm$getNow(void){
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
static inline  uint32_t /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Timer$getNow(void)
{
//#line 86
  return /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Alarm$getNow();
}

//# 125 "/opt/tinyos-2.x/tos/lib/timer/Timer.nc"
inline static  uint32_t /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$TimerFrom$getNow(void){
//#line 125
  unsigned long result;
//#line 125

//#line 125
  result = /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Timer$getNow();
//#line 125

//#line 125
  return result;
//#line 125
}
//#line 125
//# 128 "/opt/tinyos-2.x/tos/lib/timer/VirtualizeTimerC.nc"
static inline  void /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$TimerFrom$fired(void)
{
  /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$fireTimers(/*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$TimerFrom$getNow());
}

//# 72 "/opt/tinyos-2.x/tos/lib/timer/Timer.nc"
inline static  void /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Timer$fired(void){
//#line 72
  /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$TimerFrom$fired();
//#line 72
}
//#line 72
//# 228 "/opt/tinyos-2.x/tos/chips/stm32/timer/STM32RtcC.nc"
static inline   uint32_t STM32RtcC$Alarm$getAlarm(void)
{
  return STM32RtcC$alarm;
}

//# 105 "/opt/tinyos-2.x/tos/lib/timer/Alarm.nc"
inline static   /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Alarm$size_type /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Alarm$getAlarm(void){
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
//#line 92
inline static   void /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Alarm$startAt(/*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Alarm$size_type arg_0x40667e78, /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Alarm$size_type arg_0x40666030){
//#line 92
  STM32RtcC$Alarm$startAt(arg_0x40667e78, arg_0x40666030);
//#line 92
}
//#line 92
//# 47 "/opt/tinyos-2.x/tos/lib/timer/AlarmToTimerC.nc"
static inline void /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$start(uint32_t t0, uint32_t dt, bool oneshot)
{
  /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$m_dt = dt;
  /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$m_oneshot = oneshot;
  /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Alarm$startAt(t0, dt);
}










static inline  void /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$fired$runTask(void)
{
  if (/*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$m_oneshot == FALSE) {
    /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$start(/*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Alarm$getAlarm(), /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$m_dt, FALSE);
    }
//#line 67
  /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Timer$fired();
}

//# 48 "/opt/tinyos-2.x/tos/chips/stm32/timer/STM32RtcC.nc"
static inline void STM32RtcC$enableInterrupt(void)
{

  { __nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
//#line 51
    {
      RTC_ITConfig((uint16_t )0x0002, ENABLE);
      RTC_WaitForLastTask();
      STM32RtcC$running = TRUE;
    }
//#line 55
    __nesc_atomic_end(__nesc_atomic); }
}

//# 58 "/opt/tinyos-2.x/tos/chips/stm32/gpio/HplSTM32GeneralIOPinP.nc"
static inline   void /*HplSTM32GeneralIOC.PortC.Bit12*/HplSTM32GeneralIOPinP$44$IO$toggle(void)
//#line 58
{
  GPIO_TypeDef *port = (GPIO_TypeDef *)1073811456U;



  * (volatile int32_t *)((uint32_t )0x42000000 + ((uint32_t )& port->ODR - (uint32_t )0x40000000) * 32 + 12 * 4) ^= 1;
}

//# 31 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
inline static   void LedsP$Led0$toggle(void){
//#line 31
  /*HplSTM32GeneralIOC.PortC.Bit12*/HplSTM32GeneralIOPinP$44$IO$toggle();
//#line 31
}
//#line 31
//# 73 "/opt/tinyos-2.x/tos/system/LedsP.nc"
static inline   void LedsP$Leds$led0Toggle(void)
//#line 73
{
  LedsP$Led0$toggle();
  ;
//#line 75
  ;
}

//# 56 "/opt/tinyos-2.x/tos/interfaces/Leds.nc"
inline static   void STMTestC$Leds$led0Toggle(void){
//#line 56
  LedsP$Leds$led0Toggle();
//#line 56
}
//#line 56
//# 240 "/usr/lib/ncc/nesc_nx.h"
static __inline uint8_t __nesc_hton_uint8(void *target, uint8_t value)
//#line 240
{
  uint8_t *base = (uint8_t *) target;

//#line 242
  base[0] = value;
  return value;
}

//# 58 "/opt/tinyos-2.x/tos/chips/stm32/gpio/HplSTM32GeneralIOPinP.nc"
static inline   void /*HplSTM32GeneralIOC.PortC.Bit9*/HplSTM32GeneralIOPinP$41$IO$toggle(void)
//#line 58
{
  GPIO_TypeDef *port = (GPIO_TypeDef *)1073811456U;



  * (volatile int32_t *)((uint32_t )0x42000000 + ((uint32_t )& port->ODR - (uint32_t )0x40000000) * 32 + 9 * 4) ^= 1;
}

//# 31 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
inline static   void LedsP$Led2$toggle(void){
//#line 31
  /*HplSTM32GeneralIOC.PortC.Bit9*/HplSTM32GeneralIOPinP$41$IO$toggle();
//#line 31
}
//#line 31
//# 103 "/opt/tinyos-2.x/tos/system/LedsP.nc"
static inline   void LedsP$Leds$led2Toggle(void)
//#line 103
{
  LedsP$Led2$toggle();
  ;
//#line 105
  ;
}

//# 89 "/opt/tinyos-2.x/tos/interfaces/Leds.nc"
inline static   void STMAppMessagingP$Leds$led2Toggle(void){
//#line 89
  LedsP$Leds$led2Toggle();
//#line 89
}
//#line 89
//# 58 "/opt/tinyos-2.x/tos/chips/stm32/gpio/HplSTM32GeneralIOPinP.nc"
static inline   void /*HplSTM32GeneralIOC.PortC.Bit8*/HplSTM32GeneralIOPinP$40$IO$toggle(void)
//#line 58
{
  GPIO_TypeDef *port = (GPIO_TypeDef *)1073811456U;



  * (volatile int32_t *)((uint32_t )0x42000000 + ((uint32_t )& port->ODR - (uint32_t )0x40000000) * 32 + 8 * 4) ^= 1;
}

//# 31 "/opt/tinyos-2.x/tos/interfaces/GeneralIO.nc"
inline static   void LedsP$Led1$toggle(void){
//#line 31
  /*HplSTM32GeneralIOC.PortC.Bit8*/HplSTM32GeneralIOPinP$40$IO$toggle();
//#line 31
}
//#line 31
//# 88 "/opt/tinyos-2.x/tos/system/LedsP.nc"
static inline   void LedsP$Leds$led1Toggle(void)
//#line 88
{
  LedsP$Led1$toggle();
  ;
//#line 90
  ;
}

//# 72 "/opt/tinyos-2.x/tos/interfaces/Leds.nc"
inline static   void STMAppMessagingP$Leds$led1Toggle(void){
//#line 72
  LedsP$Leds$led1Toggle();
//#line 72
}
//#line 72
//# 69 "/opt/tinyos-2.x/tos/interfaces/AMSend.nc"
inline static  error_t /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$AMSend$send(am_id_t arg_0x40746be8, am_addr_t arg_0x406fce00, message_t *arg_0x40711010, uint8_t arg_0x40711198){
//#line 69
  unsigned char result;
//#line 69

//#line 69
  result = ActiveMessageLayerP$AMSend$send(arg_0x40746be8, arg_0x406fce00, arg_0x40711010, arg_0x40711198);
//#line 69

//#line 69
  return result;
//#line 69
}
//#line 69
//# 67 "/opt/tinyos-2.x/tos/interfaces/AMPacket.nc"
inline static  am_addr_t /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$AMPacket$destination(message_t *arg_0x40722b50){
//#line 67
  unsigned short result;
//#line 67

//#line 67
  result = ActiveMessageLayerP$AMPacket$destination(arg_0x40722b50);
//#line 67

//#line 67
  return result;
//#line 67
}
//#line 67
//#line 136
inline static  am_id_t /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$AMPacket$type(message_t *arg_0x4071e0d0){
//#line 136
  unsigned char result;
//#line 136

//#line 136
  result = ActiveMessageLayerP$AMPacket$type(arg_0x4071e0d0);
//#line 136

//#line 136
  return result;
//#line 136
}
//#line 136
//# 43 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioPacket.nc"
inline static   void LowpanNetworkLayerC$SubPacket$setPayloadLength(message_t *arg_0x407de7a8, uint8_t arg_0x407de930){
//#line 43
  IEEE154MessageLayerP$RadioPacket$setPayloadLength(arg_0x407de7a8, arg_0x407de930);
//#line 43
}
//#line 43
//# 104 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/LowpanNetworkLayerC.nc"
static inline   void LowpanNetworkLayerC$RadioPacket$setPayloadLength(message_t *msg, uint8_t length)
{
  LowpanNetworkLayerC$SubPacket$setPayloadLength(msg, length + sizeof(lowpan_header_t ));
}

//# 43 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioPacket.nc"
inline static   void ActiveMessageLayerP$SubPacket$setPayloadLength(message_t *arg_0x407de7a8, uint8_t arg_0x407de930){
//#line 43
  LowpanNetworkLayerC$RadioPacket$setPayloadLength(arg_0x407de7a8, arg_0x407de930);
//#line 43
}
//#line 43
//# 206 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/ActiveMessageLayerP.nc"
static inline   void ActiveMessageLayerP$RadioPacket$setPayloadLength(message_t *msg, uint8_t length)
{
  ActiveMessageLayerP$SubPacket$setPayloadLength(msg, length + sizeof(activemessage_header_t ));
}

//#line 238
static inline  void ActiveMessageLayerP$Packet$setPayloadLength(message_t *msg, uint8_t len)
{
  ActiveMessageLayerP$RadioPacket$setPayloadLength(msg, len);
}

//# 83 "/opt/tinyos-2.x/tos/interfaces/Packet.nc"
inline static  void /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$Packet$setPayloadLength(message_t *arg_0x40709190, uint8_t arg_0x40709318){
//#line 83
  ActiveMessageLayerP$Packet$setPayloadLength(arg_0x40709190, arg_0x40709318);
//#line 83
}
//#line 83
//# 82 "/opt/tinyos-2.x/tos/system/AMQueueImplP.nc"
static inline  error_t /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$Send$send(uint8_t clientId, message_t *msg, 
uint8_t len)
//#line 83
{
  if (clientId >= 1) {
      return FAIL;
    }
  if (/*AMQueueP.AMQueueImplP*/AMQueueImplP$0$queue[clientId].msg != (message_t *)0) {
      return EBUSY;
    }
  ;

  /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$queue[clientId].msg = msg;
  /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$Packet$setPayloadLength(msg, len);

  if (/*AMQueueP.AMQueueImplP*/AMQueueImplP$0$current >= 1) {
      error_t err;
      am_id_t amId = /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$AMPacket$type(msg);
      am_addr_t dest = /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$AMPacket$destination(msg);

      ;
      /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$current = clientId;

      err = /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$AMSend$send(amId, dest, msg, len);
      if (err != SUCCESS) {
          ;
          /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$current = 1;
          /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$queue[clientId].msg = (void *)0;
        }

      return err;
    }
  else {
      ;
    }
  return SUCCESS;
}

//# 64 "/opt/tinyos-2.x/tos/interfaces/Send.nc"
inline static  error_t /*STMAppMessagingC.AMSenderC.AMQueueEntryP*/AMQueueEntryP$0$Send$send(message_t *arg_0x40729cb0, uint8_t arg_0x40729e38){
//#line 64
  unsigned char result;
//#line 64

//#line 64
  result = /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$Send$send(0U, arg_0x40729cb0, arg_0x40729e38);
//#line 64

//#line 64
  return result;
//#line 64
}
//#line 64
//# 175 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/ActiveMessageLayerP.nc"
static __inline  void ActiveMessageLayerP$AMPacket$setType(message_t *msg, am_id_t type)
{
  __nesc_hton_leuint8((unsigned char *)&ActiveMessageLayerP$getHeader(msg)->type, type);
}

//# 151 "/opt/tinyos-2.x/tos/interfaces/AMPacket.nc"
inline static  void /*STMAppMessagingC.AMSenderC.AMQueueEntryP*/AMQueueEntryP$0$AMPacket$setType(message_t *arg_0x4071e668, am_id_t arg_0x4071e7f0){
//#line 151
  ActiveMessageLayerP$AMPacket$setType(arg_0x4071e668, arg_0x4071e7f0);
//#line 151
}
//#line 151
//# 179 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/IEEE154MessageLayerP.nc"
static inline   void IEEE154MessageLayerP$IEEE154MessageLayer$setDestAddr(message_t *msg, uint16_t addr)
{
  __nesc_hton_leuint16((unsigned char *)&IEEE154MessageLayerP$getHeader(msg)->dest, addr);
}

//# 135 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/IEEE154MessageLayer.nc"
inline static   void RF230ActiveMessageP$IEEE154MessageLayer$setDestAddr(message_t *arg_0x407cde40, uint16_t arg_0x407cc010){
//#line 135
  IEEE154MessageLayerP$IEEE154MessageLayer$setDestAddr(arg_0x407cde40, arg_0x407cc010);
//#line 135
}
//#line 135
//# 168 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230ActiveMessageP.nc"
static inline  void RF230ActiveMessageP$ActiveMessageConfig$setDestination(message_t *msg, am_addr_t addr)
{
  RF230ActiveMessageP$IEEE154MessageLayer$setDestAddr(msg, addr);
}

//# 32 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/ActiveMessageConfig.nc"
inline static  void ActiveMessageLayerP$Config$setDestination(message_t *arg_0x407c1398, am_addr_t arg_0x407c1528){
//#line 32
  RF230ActiveMessageP$ActiveMessageConfig$setDestination(arg_0x407c1398, arg_0x407c1528);
//#line 32
}
//#line 32
//# 155 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/ActiveMessageLayerP.nc"
static __inline  void ActiveMessageLayerP$AMPacket$setDestination(message_t *msg, am_addr_t addr)
{
  ActiveMessageLayerP$Config$setDestination(msg, addr);
}

//# 92 "/opt/tinyos-2.x/tos/interfaces/AMPacket.nc"
inline static  void /*STMAppMessagingC.AMSenderC.AMQueueEntryP*/AMQueueEntryP$0$AMPacket$setDestination(message_t *arg_0x40721768, am_addr_t arg_0x407218f8){
//#line 92
  ActiveMessageLayerP$AMPacket$setDestination(arg_0x40721768, arg_0x407218f8);
//#line 92
}
//#line 92
//# 45 "/opt/tinyos-2.x/tos/system/AMQueueEntryP.nc"
static inline  error_t /*STMAppMessagingC.AMSenderC.AMQueueEntryP*/AMQueueEntryP$0$AMSend$send(am_addr_t dest, 
message_t *msg, 
uint8_t len)
//#line 47
{
  /*STMAppMessagingC.AMSenderC.AMQueueEntryP*/AMQueueEntryP$0$AMPacket$setDestination(msg, dest);
  /*STMAppMessagingC.AMSenderC.AMQueueEntryP*/AMQueueEntryP$0$AMPacket$setType(msg, 100);
  return /*STMAppMessagingC.AMSenderC.AMQueueEntryP*/AMQueueEntryP$0$Send$send(msg, len);
}

//# 69 "/opt/tinyos-2.x/tos/interfaces/AMSend.nc"
inline static  error_t STMAppMessagingP$AMSend$send(am_addr_t arg_0x406fce00, message_t *arg_0x40711010, uint8_t arg_0x40711198){
//#line 69
  unsigned char result;
//#line 69

//#line 69
  result = /*STMAppMessagingC.AMSenderC.AMQueueEntryP*/AMQueueEntryP$0$AMSend$send(arg_0x406fce00, arg_0x40711010, arg_0x40711198);
//#line 69

//#line 69
  return result;
//#line 69
}
//#line 69
//# 94 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/MetadataFlagsLayerC.nc"
static inline   uint8_t MetadataFlagsLayerC$RadioPacket$maxPayloadLength(void)
{
  return MetadataFlagsLayerC$SubPacket$maxPayloadLength();
}

//# 48 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioPacket.nc"
inline static   uint8_t TimeStampingLayerP$SubPacket$maxPayloadLength(void){
//#line 48
  unsigned char result;
//#line 48

//#line 48
  result = MetadataFlagsLayerC$RadioPacket$maxPayloadLength();
//#line 48

//#line 48
  return result;
//#line 48
}
//#line 48
//# 120 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/TimeStampingLayerP.nc"
static inline   uint8_t TimeStampingLayerP$RadioPacket$maxPayloadLength(void)
{
  return TimeStampingLayerP$SubPacket$maxPayloadLength();
}

//# 48 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioPacket.nc"
inline static   uint8_t IEEE154MessageLayerP$SubPacket$maxPayloadLength(void){
//#line 48
  unsigned char result;
//#line 48

//#line 48
  result = TimeStampingLayerP$RadioPacket$maxPayloadLength();
//#line 48

//#line 48
  return result;
//#line 48
}
//#line 48
//# 277 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/IEEE154MessageLayerP.nc"
static inline   uint8_t IEEE154MessageLayerP$RadioPacket$maxPayloadLength(void)
{
  return IEEE154MessageLayerP$SubPacket$maxPayloadLength() - sizeof(ieee154_header_t );
}

//# 48 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioPacket.nc"
inline static   uint8_t LowpanNetworkLayerC$SubPacket$maxPayloadLength(void){
//#line 48
  unsigned char result;
//#line 48

//#line 48
  result = IEEE154MessageLayerP$RadioPacket$maxPayloadLength();
//#line 48

//#line 48
  return result;
//#line 48
}
//#line 48
//# 109 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/LowpanNetworkLayerC.nc"
static inline   uint8_t LowpanNetworkLayerC$RadioPacket$maxPayloadLength(void)
{
  return LowpanNetworkLayerC$SubPacket$maxPayloadLength() - sizeof(lowpan_header_t );
}

//# 48 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioPacket.nc"
inline static   uint8_t ActiveMessageLayerP$SubPacket$maxPayloadLength(void){
//#line 48
  unsigned char result;
//#line 48

//#line 48
  result = LowpanNetworkLayerC$RadioPacket$maxPayloadLength();
//#line 48

//#line 48
  return result;
//#line 48
}
//#line 48
//# 211 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/ActiveMessageLayerP.nc"
static inline   uint8_t ActiveMessageLayerP$RadioPacket$maxPayloadLength(void)
{
  return ActiveMessageLayerP$SubPacket$maxPayloadLength() - sizeof(activemessage_header_t );
}

//#line 243
static inline  uint8_t ActiveMessageLayerP$Packet$maxPayloadLength(void)
{
  return ActiveMessageLayerP$RadioPacket$maxPayloadLength();
}

//# 95 "/opt/tinyos-2.x/tos/interfaces/Packet.nc"
inline static  uint8_t STMAppMessagingP$Packet$maxPayloadLength(void){
//#line 95
  unsigned char result;
//#line 95

//#line 95
  result = ActiveMessageLayerP$Packet$maxPayloadLength();
//#line 95

//#line 95
  return result;
//#line 95
}
//#line 95
//#line 115
inline static  void *STMAppMessagingP$Packet$getPayload(message_t *arg_0x40709ef0, uint8_t arg_0x40708088){
//#line 115
  void *result;
//#line 115

//#line 115
  result = ActiveMessageLayerP$Packet$getPayload(arg_0x40709ef0, arg_0x40708088);
//#line 115

//#line 115
  return result;
//#line 115
}
//#line 115
//# 42 "STMAppMessagingP.nc"
static inline  error_t STMAppMessagingP$STMAppMessaging$Send(uint8_t appId, uint16_t destId, uint8_t *dataBuf, uint16_t len)
//#line 42
{

  uint8_t i;

  stm_app_msg_t *newmsg = (stm_app_msg_t *)STMAppMessagingP$Packet$getPayload(&STMAppMessagingP$packet, (uint8_t)0);

  if (STMAppMessagingP$Packet$maxPayloadLength() < sizeof(stm_app_msg_t )) {
      return FAIL;
    }

  __nesc_hton_uint8((unsigned char *)&newmsg->appId, appId);
  __nesc_hton_uint8((unsigned char *)&newmsg->length, len);
  for (i = 0; i < len; i++) 
    __nesc_hton_uint8((unsigned char *)&newmsg->data[i], dataBuf[i]);

  if (STMAppMessagingP$AMSend$send(destId, &STMAppMessagingP$packet, AM_STM_APP_MSG_HDR_LEN + len) == SUCCESS) {
      STMAppMessagingP$Leds$led1Toggle();
      return SUCCESS;
    }
  else {

    STMAppMessagingP$Leds$led2Toggle();
    }
  return FAIL;
}

//# 5 "STMAppMessaging.nc"
inline static  error_t STMTestC$STMAppMessaging$Send(uint8_t arg_0x406e4968, uint16_t arg_0x406e4af8, uint8_t *arg_0x406e4ca0, uint16_t arg_0x406e4e30){
//#line 5
  unsigned char result;
//#line 5

//#line 5
  result = STMAppMessagingP$STMAppMessaging$Send(arg_0x406e4968, arg_0x406e4af8, arg_0x406e4ca0, arg_0x406e4e30);
//#line 5

//#line 5
  return result;
//#line 5
}
//#line 5
//# 39 "STMTestC.nc"
static inline  void STMTestC$Timer0$fired(void)
//#line 39
{

  uint8_t i;

//#line 42
  for (i = 0; i < STMTestC$counter; i++) 
    STMTestC$packet[i] = STMTestC$counter;

  STMTestC$STMAppMessaging$Send(STMTestC$myAppID, AM_BROADCAST_ADDR, &STMTestC$packet[0], STMTestC$counter);

  STMTestC$counter++;
  if (STMTestC$counter == 10) {
    STMTestC$counter = 1;
    }
  STMTestC$Leds$led0Toggle();
}

//# 193 "/opt/tinyos-2.x/tos/lib/timer/VirtualizeTimerC.nc"
static inline   void /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$Timer$default$fired(uint8_t num)
{
}

//# 72 "/opt/tinyos-2.x/tos/lib/timer/Timer.nc"
inline static  void /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$Timer$fired(uint8_t arg_0x406329a0){
//#line 72
  switch (arg_0x406329a0) {
//#line 72
    case 0U:
//#line 72
      STMTestC$Timer0$fired();
//#line 72
      break;
//#line 72
    default:
//#line 72
      /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$Timer$default$fired(arg_0x406329a0);
//#line 72
      break;
//#line 72
    }
//#line 72
}
//#line 72
//# 56 "/opt/tinyos-2.x/tos/interfaces/TaskBasic.nc"
inline static   error_t /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$updateFromTimer$postTask(void){
//#line 56
  unsigned char result;
//#line 56

//#line 56
  result = SchedulerBasicP$TaskBasic$postTask(/*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$updateFromTimer);
//#line 56

//#line 56
  return result;
//#line 56
}
//#line 56
//# 82 "/opt/tinyos-2.x/tos/lib/timer/AlarmToTimerC.nc"
static inline  void /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Timer$startOneShotAt(uint32_t t0, uint32_t dt)
{
//#line 83
  /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$start(t0, dt, TRUE);
}

//# 118 "/opt/tinyos-2.x/tos/lib/timer/Timer.nc"
inline static  void /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$TimerFrom$startOneShotAt(uint32_t arg_0x4060f9b8, uint32_t arg_0x4060fb48){
//#line 118
  /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Timer$startOneShotAt(arg_0x4060f9b8, arg_0x4060fb48);
//#line 118
}
//#line 118
//# 178 "/opt/tinyos-2.x/tos/chips/stm32/timer/STM32RtcC.nc"
static inline   void STM32RtcC$Alarm$stop(void)
{
  STM32RtcC$disableInterrupt();
}

//# 62 "/opt/tinyos-2.x/tos/lib/timer/Alarm.nc"
inline static   void /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Alarm$stop(void){
//#line 62
  STM32RtcC$Alarm$stop();
//#line 62
}
//#line 62
//# 60 "/opt/tinyos-2.x/tos/lib/timer/AlarmToTimerC.nc"
static inline  void /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Timer$stop(void)
{
//#line 61
  /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Alarm$stop();
}

//# 67 "/opt/tinyos-2.x/tos/lib/timer/Timer.nc"
inline static  void /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$TimerFrom$stop(void){
//#line 67
  /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$Timer$stop();
//#line 67
}
//#line 67
//# 89 "/opt/tinyos-2.x/tos/lib/timer/VirtualizeTimerC.nc"
static inline  void /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$updateFromTimer$runTask(void)
{




  uint32_t now = /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$TimerFrom$getNow();
  int32_t min_remaining = (1UL << 31) - 1;
  bool min_remaining_isset = FALSE;
  uint8_t num;

  /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$TimerFrom$stop();

  for (num = 0; num < /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$NUM_TIMERS; num++) 
    {
      /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$Timer_t *timer = &/*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$m_timers[num];

      if (timer->isrunning) 
        {
          uint32_t elapsed = now - timer->t0;
          int32_t remaining = timer->dt - elapsed;

          if (remaining < min_remaining) 
            {
              min_remaining = remaining;
              min_remaining_isset = TRUE;
            }
        }
    }

  if (min_remaining_isset) 
    {
      if (min_remaining <= 0) {
        /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$fireTimers(now);
        }
      else {
//#line 124
        /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$TimerFrom$startOneShotAt(now, min_remaining);
        }
    }
}

//# 72 "/opt/tinyos-2.x/tos/chips/stm32/timer/STM32RtcC.nc"
static inline  error_t STM32RtcC$Init$init(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;


  TIM_ICInitTypeDef TIM_ICInitStructure;


  NVIC_PriorityGroupConfig((uint32_t )0x600);


  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);


  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);


  RCC_APB1PeriphClockCmd((uint32_t )0x10000000 | (uint32_t )0x08000000, ENABLE);

  RCC_APB1PeriphClockCmd((uint32_t )0x00000002, ENABLE);

  RCC_APB2PeriphClockCmd((uint32_t )0x00000004, ENABLE);



  GPIO_InitStructure.GPIO_Pin = (uint16_t )0x0080;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init((GPIO_TypeDef *)((uint32_t )0x40000000 + 0x10000 + 0x0800), &GPIO_InitStructure);


  PWR_BackupAccessCmd(ENABLE);


  BKP_DeInit();


  RCC_LSEConfig((uint8_t )0x01);

  while (RCC_GetFlagStatus((uint8_t )0x41) == RESET) {
    }

  RCC_RTCCLKConfig((uint32_t )0x00000100);


  RCC_RTCCLKCmd(ENABLE);


  RTC_WaitForSynchro();


  RTC_WaitForLastTask();


  RTC_SetPrescaler(31);


  RTC_WaitForLastTask();


  RTC_SetCounter(0x0);


  RTC_WaitForLastTask();


  RTC_ITConfig((uint16_t )0x0004, ENABLE);


  RTC_WaitForLastTask();




  TIM_ICInitStructure.TIM_Channel = (uint16_t )0x0001;
  TIM_ICInitStructure.TIM_ICPolarity = (uint16_t )0x0000;
  TIM_ICInitStructure.TIM_ICSelection = (uint16_t )0x0001;
  TIM_ICInitStructure.TIM_ICPrescaler = (uint16_t )0x0000;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;

  TIM_ICInit((TIM_TypeDef *)((uint32_t )0x40000000 + 0x0400), &TIM_ICInitStructure);

  TIM_Cmd((TIM_TypeDef *)((uint32_t )0x40000000 + 0x0400), ENABLE);

  TIM_ITConfig((TIM_TypeDef *)((uint32_t )0x40000000 + 0x0400), (uint16_t )0x0004, ENABLE);
  /* atomic removed: atomic calls only */
  STM32RtcC$alarm = 0;

  return SUCCESS;
}

//# 52 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/UniqueLayerP.nc"
static inline  error_t UniqueLayerP$Init$init(void)
{
  UniqueLayerP$sequenceNumber = TOS_NODE_ID << 4;
  return SUCCESS;
}

//# 44 "/opt/tinyos-2.x/tos/chips/rf2xx/util/NeighborhoodP.nc"
static inline  error_t NeighborhoodP$Init$init(void)
{
  uint8_t i;

  for (i = 0; i < 5; ++i) 
    NeighborhoodP$nodes[i] = AM_BROADCAST_ADDR;

  return SUCCESS;
}

//# 260 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/MessageBufferLayerP.nc"
static inline  error_t MessageBufferLayerP$SoftwareInit$init(void)
{
  uint8_t i;

  for (i = 0; i < MessageBufferLayerP$RECEIVE_QUEUE_SIZE; ++i) 
    MessageBufferLayerP$receiveQueue[i] = MessageBufferLayerP$receiveQueueData + i;

  return SUCCESS;
}

//# 44 "/opt/tinyos-2.x/tos/system/RandomMlcgC.nc"
static inline  error_t RandomMlcgC$Init$init(void)
//#line 44
{
  /* atomic removed: atomic calls only */
//#line 45
  RandomMlcgC$seed = (uint32_t )(TOS_NODE_ID + 1);

  return SUCCESS;
}

//# 78 "/opt/tinyos-2.x/tos/interfaces/Resource.nc"
inline static   error_t RF230DriverLayerP$SpiResource$request(void){
//#line 78
  unsigned char result;
//#line 78

//#line 78
  result = Stm32SpiP$Resource$request(0U);
//#line 78

//#line 78
  return result;
//#line 78
}
//#line 78
//# 224 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230DriverLayerP.nc"
static inline  error_t RF230DriverLayerP$SoftwareInit$init(void)
{

  return RF230DriverLayerP$SpiResource$request();
}

//# 45 "/opt/tinyos-2.x/tos/system/FcfsResourceQueueC.nc"
static inline  error_t /*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$Init$init(void)
//#line 45
{
  memset(/*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$resQ, /*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$NO_ENTRY, sizeof /*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$resQ);
  return SUCCESS;
}

//# 51 "/opt/tinyos-2.x/tos/interfaces/Init.nc"
inline static  error_t RealMainP$SoftwareInit$init(void){
//#line 51
  unsigned char result;
//#line 51

//#line 51
  result = /*Stm32SpiC.Arbiter.Queue*/FcfsResourceQueueC$0$Init$init();
//#line 51
  result = ecombine(result, RF230DriverLayerP$SoftwareInit$init());
//#line 51
  result = ecombine(result, RandomMlcgC$Init$init());
//#line 51
  result = ecombine(result, MessageBufferLayerP$SoftwareInit$init());
//#line 51
  result = ecombine(result, NeighborhoodP$Init$init());
//#line 51
  result = ecombine(result, UniqueLayerP$Init$init());
//#line 51
  result = ecombine(result, STM32RtcC$Init$init());
//#line 51

//#line 51
  return result;
//#line 51
}
//#line 51
//# 21 "STMAppMessagingP.nc"
static inline  void STMAppMessagingP$STMAppMessaging$Init(void)
//#line 21
{
  if (STMAppMessagingP$radioStarted == 0) {
    STMAppMessagingP$AMControl$start();
    }
  else 
//#line 24
    {
    }
}

//# 3 "STMAppMessaging.nc"
inline static  void STMTestC$STMAppMessaging$Init(void){
//#line 3
  STMAppMessagingP$STMAppMessaging$Init();
//#line 3
}
//#line 3
//# 25 "STMTestC.nc"
static inline  void STMTestC$Boot$booted(void)
//#line 25
{
  STMTestC$STMAppMessaging$Init();
}

//# 49 "/opt/tinyos-2.x/tos/interfaces/Boot.nc"
inline static  void RealMainP$Boot$booted(void){
//#line 49
  STMTestC$Boot$booted();
//#line 49
}
//#line 49
//# 1211 "/opt/tinyos-2.x/tos/chips/stm32/fwlib/inc/cortexm3_macro.h"
static __inline void __WFI(void)
//#line 1211
{
//#line 1211
   __asm volatile ("wfi");}

//# 73 "/opt/tinyos-2.x/tos/chips/stm32/McuSleepC.nc"
static inline   void McuSleepC$McuSleep$sleep(void)
//#line 73
{

  uint16_t i = 0;

//#line 98
  RCC_APB2PeriphClockCmd((uint32_t )0x00000001 | (uint32_t )0x00000004, DISABLE);
  RCC_AHBPeriphClockCmd((uint32_t )0x00000010 | (uint32_t )0x00000004, DISABLE);


  RCC_HSICmd(ENABLE);

  while (RCC_GetFlagStatus((uint8_t )0x21) == RESET) ;
  RCC_SYSCLKConfig((uint32_t )0x00000000);

  RCC_PLLCmd(DISABLE);

  RCC_HSEConfig((uint32_t )0x00000000);


  RCC_HCLKConfig((uint32_t )0x000000C0);






  __WFI();
//#line 134
  RCC_HCLKConfig((uint32_t )0x00000000);

  RCC_HSEConfig((uint32_t )0x00010000);


  while (RCC_GetFlagStatus((uint8_t )0x31) == RESET) ;


  RCC_PLLConfig((uint32_t )0x00010000, (uint32_t )0x001C0000);


  RCC_PLLCmd(ENABLE);


  while (RCC_GetFlagStatus((uint8_t )0x39) == RESET) ;
  RCC_SYSCLKConfig((uint32_t )0x00000002);

  RCC_APB2PeriphClockCmd(((uint32_t )0x00000001 | (uint32_t )0x00000010) | (uint32_t )0x00000004, ENABLE);
  RCC_AHBPeriphClockCmd((uint32_t )0x00000010 | (uint32_t )0x00000004, ENABLE);


  return;
}

//# 59 "/opt/tinyos-2.x/tos/interfaces/McuSleep.nc"
inline static   void SchedulerBasicP$McuSleep$sleep(void){
//#line 59
  McuSleepC$McuSleep$sleep();
//#line 59
}
//#line 59
//# 67 "/opt/tinyos-2.x/tos/system/SchedulerBasicP.nc"
static __inline uint8_t SchedulerBasicP$popTask(void)
{
  if (SchedulerBasicP$m_head != SchedulerBasicP$NO_TASK) 
    {
      uint8_t id = SchedulerBasicP$m_head;

//#line 72
      SchedulerBasicP$m_head = SchedulerBasicP$m_next[SchedulerBasicP$m_head];
      if (SchedulerBasicP$m_head == SchedulerBasicP$NO_TASK) 
        {
          SchedulerBasicP$m_tail = SchedulerBasicP$NO_TASK;
        }
      SchedulerBasicP$m_next[id] = SchedulerBasicP$NO_TASK;
      return id;
    }
  else 
    {
      return SchedulerBasicP$NO_TASK;
    }
}

//#line 138
static inline  void SchedulerBasicP$Scheduler$taskLoop(void)
{
  for (; ; ) 
    {
      uint8_t nextTask;

      { __nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
        {
          while ((nextTask = SchedulerBasicP$popTask()) == SchedulerBasicP$NO_TASK) 
            {
              SchedulerBasicP$McuSleep$sleep();
            }
        }
//#line 150
        __nesc_atomic_end(__nesc_atomic); }
      SchedulerBasicP$TaskBasic$runTask(nextTask);
    }
}

//# 61 "/opt/tinyos-2.x/tos/interfaces/Scheduler.nc"
inline static  void RealMainP$Scheduler$taskLoop(void){
//#line 61
  SchedulerBasicP$Scheduler$taskLoop();
//#line 61
}
//#line 61
//# 48 "/opt/tinyos-2.x/tos/chips/rf2xx/util/Tasklet.nc"
inline static   void RadioAlarmP$Tasklet$schedule(void){
//#line 48
  TaskletC$Tasklet$schedule();
//#line 48
}
//#line 48
//# 53 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioAlarmP.nc"
static inline   void RadioAlarmP$Alarm$fired(void)
{
  { __nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
    {
      if (RadioAlarmP$state == RadioAlarmP$STATE_WAIT) {
        RadioAlarmP$state = RadioAlarmP$STATE_FIRED;
        }
    }
//#line 60
    __nesc_atomic_end(__nesc_atomic); }
  RadioAlarmP$Tasklet$schedule();
}

//# 67 "/opt/tinyos-2.x/tos/lib/timer/Alarm.nc"
inline static   void /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$Alarm$fired(void){
//#line 67
  RadioAlarmP$Alarm$fired();
//#line 67
}
//#line 67
//# 110 "/opt/tinyos-2.x/tos/chips/stm32/timer/Stm32AlarmC.nc"
static inline   void /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$HplStm32Compare$fired(void)
//#line 110
{
  /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$HplStm32Compare$stop();
  ;
  __nesc_enable_interrupt();
  /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$Alarm$fired();
}

//# 49 "/opt/tinyos-2.x/tos/chips/stm32/timer/HplStm32Compare.nc"
inline static   void HplStm32Timer3P$CompareA$fired(void){
//#line 49
  /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$HplStm32Compare$fired();
//#line 49
}
//#line 49
//# 118 "/opt/tinyos-2.x/tos/chips/stm32/timer/HplStm32Timer3P.nc"
static inline    void HplStm32Timer3P$CompareC$default$fired(void)
//#line 118
{
}

//# 49 "/opt/tinyos-2.x/tos/chips/stm32/timer/HplStm32Compare.nc"
inline static   void HplStm32Timer3P$CompareC$fired(void){
//#line 49
  HplStm32Timer3P$CompareC$default$fired();
//#line 49
}
//#line 49
//# 659 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230DriverLayerP.nc"
static inline   void RF230DriverLayerP$IRQ$captured(uint16_t time)
{
  for (; 0; ) ;

  { __nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
    {
      RF230DriverLayerP$capturedTime = time;
      RF230DriverLayerP$radioIrq = TRUE;
    }
//#line 667
    __nesc_atomic_end(__nesc_atomic); }

  RF230DriverLayerP$Tasklet$schedule();
}

//# 50 "/opt/tinyos-2.x/tos/interfaces/GpioCapture.nc"
inline static   void HplRF230P$IRQ$captured(uint16_t arg_0x40a2e8c8){
//#line 50
  RF230DriverLayerP$IRQ$captured(arg_0x40a2e8c8);
//#line 50
}
//#line 50
//# 106 "/opt/tinyos-2.x/tos/chips/stm32/timer/HplStm32Timer3P.nc"
static inline   uint16_t HplStm32Timer3P$Capture$get(void)
//#line 106
{
//#line 106
  return TIM_GetCapture2((TIM_TypeDef *)((uint32_t )0x40000000 + 0x0400));
}

//# 38 "/opt/tinyos-2.x/tos/chips/stm32/timer/HplStm32Capture.nc"
inline static   HplRF230P$Capture$size_type HplRF230P$Capture$get(void){
//#line 38
  unsigned short result;
//#line 38

//#line 38
  result = HplStm32Timer3P$Capture$get();
//#line 38

//#line 38
  return result;
//#line 38
}
//#line 38
//# 53 "/opt/tinyos-2.x/tos/platforms/stm32-p103/chips/rf230/HplRF230P.nc"
static inline   void HplRF230P$Capture$captured(uint16_t time)
{
  time = HplRF230P$Capture$get();
  HplRF230P$IRQ$captured(time);
}

//# 51 "/opt/tinyos-2.x/tos/chips/stm32/timer/HplStm32Capture.nc"
inline static   void HplStm32Timer3P$Capture$captured(HplStm32Timer3P$Capture$size_type arg_0x40ab0248){
//#line 51
  HplRF230P$Capture$captured(arg_0x40ab0248);
//#line 51
}
//#line 51
//# 117 "/opt/tinyos-2.x/tos/chips/stm32/timer/Stm32AlarmC.nc"
static inline   void /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$HplStm32Timer$overflow(void)
//#line 117
{
}

//# 47 "/opt/tinyos-2.x/tos/lib/timer/CounterToLocalTimeC.nc"
static inline   void /*LocalTimeMicroC.CounterToLocalTimeC*/CounterToLocalTimeC$0$Counter$overflow(void)
{
}

//# 71 "/opt/tinyos-2.x/tos/lib/timer/Counter.nc"
inline static   void /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$Counter$overflow(void){
//#line 71
  /*LocalTimeMicroC.CounterToLocalTimeC*/CounterToLocalTimeC$0$Counter$overflow();
//#line 71
}
//#line 71
//# 122 "/opt/tinyos-2.x/tos/lib/timer/TransformCounterC.nc"
static inline   void /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$CounterFrom$overflow(void)
{
  { __nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
    {
      /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$m_upper++;
      if ((/*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$m_upper & /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$OVERFLOW_MASK) == 0) {
        /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$Counter$overflow();
        }
    }
//#line 130
    __nesc_atomic_end(__nesc_atomic); }
}

//# 71 "/opt/tinyos-2.x/tos/lib/timer/Counter.nc"
inline static   void /*CounterOne16C.NCounter*/Stm32CounterC$0$Counter$overflow(void){
//#line 71
  /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$CounterFrom$overflow();
//#line 71
}
//#line 71
//# 56 "/opt/tinyos-2.x/tos/chips/stm32/timer/Stm32CounterC.nc"
static inline   void /*CounterOne16C.NCounter*/Stm32CounterC$0$Timer$overflow(void)
{
  /*CounterOne16C.NCounter*/Stm32CounterC$0$Counter$overflow();
}

//# 61 "/opt/tinyos-2.x/tos/chips/stm32/timer/HplStm32Timer.nc"
inline static   void HplStm32Timer3P$Timer$overflow(void){
//#line 61
  /*CounterOne16C.NCounter*/Stm32CounterC$0$Timer$overflow();
//#line 61
  /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$HplStm32Timer$overflow();
//#line 61
}
//#line 61
//# 95 "/opt/tinyos-2.x/tos/chips/stm32/stm32hardware.h"
static __inline void __nesc_disable_interrupt(void)
//#line 95
{
  uint32_t statusReg = 0;









   __asm volatile ("CPSID I");
  return;
}

//# 170 "/opt/tinyos-2.x/tos/chips/stm32/McuSleepC.nc"
  void RTCAlarm_IRQHandler(void)
{
  if (RTC_GetITStatus((uint16_t )0x0002) != RESET) 
    {

      EXTI_ClearITPendingBit((uint32_t )0x20000);


      if (PWR_GetFlagStatus((uint32_t )0x00000001) != RESET) 
        {

          PWR_ClearFlag((uint32_t )0x00000001);
        }
    }
  RTC_IRQHandler();
}

//# 261 "/opt/tinyos-2.x/tos/chips/stm32/timer/STM32RtcC.nc"
  void RTC_IRQHandler(void)
{
  if (RTC_GetITStatus((uint16_t )0x0002) != RESET) 
    {


      STM32RtcC$Alarm$stop();
      STM32RtcC$Alarm$fired();
    }
  if (RTC_GetITStatus((uint16_t )0x0004) != RESET) 
    {
      RTC_ClearITPendingBit((uint16_t )0x0004);
      RTC_WaitForLastTask();
      STM32RtcC$Counter$overflow();
    }
}

//#line 58
static void STM32RtcC$disableInterrupt(void)
{

  { __nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
//#line 61
    {
      RTC_ClearITPendingBit((uint16_t )0x0002);
      RTC_WaitForLastTask();
      RTC_ITConfig((uint16_t )0x0002, DISABLE);
      RTC_WaitForLastTask();
      STM32RtcC$running = FALSE;
    }
//#line 67
    __nesc_atomic_end(__nesc_atomic); }
}

//# 159 "/opt/tinyos-2.x/tos/system/SchedulerBasicP.nc"
static   error_t SchedulerBasicP$TaskBasic$postTask(uint8_t id)
{
  { __nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
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
    __nesc_atomic_end(__nesc_atomic); }
}

//# 50 "/opt/tinyos-2.x/tos/chips/stm32/HplSTM32InterruptM.nc"
  void NMIException(void)
{
  while (1) ;
//#line 52
  ;
  HplSTM32InterruptM$Irq$fired();
}


  void HardFaultException(void)
{
  while (1) ;
//#line 59
  ;
}


  void MemManageException(void)
{
  while (1) ;
//#line 65
  ;
}


  void BusFaultException(void)
{
  while (1) ;
//#line 71
  ;
}


  void UsageFaultException(void)
{
  while (1) ;
//#line 77
  ;
}

  void __STM32ReservedException7(void)
{
  while (1) ;
//#line 82
  ;
}

  void __STM32ReservedException8(void)
{
  while (1) ;
//#line 87
  ;
}

  void __STM32ReservedException9(void)
{
  while (1) ;
//#line 92
  ;
}

  void __STM32ReservedException10(void)
{
  while (1) ;
//#line 97
  ;
}


  void SVCHandler(void)
{
  while (1) ;
//#line 103
  ;
}


  void DebugMonitor(void)
{
  while (1) ;
//#line 109
  ;
}

  void __STM32ReservedException13(void)
{
  while (1) ;
//#line 114
  ;
}



  void PendSVC(void)
{
  while (1) ;
//#line 121
  ;
}


  void SysTickHandler(void)
{
  while (1) ;
//#line 127
  ;
}


  void WWDG_IRQHandler(void)
{
  while (1) ;
//#line 133
  ;
}


  void PVD_IRQHandler(void)
{
  while (1) ;
//#line 139
  ;
}


  void TAMPER_IRQHandler(void)
{
  while (1) ;
//#line 145
  ;
}











  void FLASH_IRQHandler(void)
{
  while (1) ;
//#line 160
  ;
}


  void RCC_IRQHandler(void)
{
  while (1) ;
//#line 166
  ;
}


  void EXTI0_IRQHandler(void)
{
  while (1) ;
//#line 172
  ;
}


  void EXTI1_IRQHandler(void)
{
  while (1) ;
//#line 178
  ;
}


  void EXTI2_IRQHandler(void)
{
  while (1) ;
//#line 184
  ;
}


  void EXTI3_IRQHandler(void)
{
  while (1) ;
//#line 190
  ;
}


  void EXTI4_IRQHandler(void)
{
  while (1) ;
//#line 196
  ;
}


  void DMAChannel1_IRQHandler(void)
{
  while (1) ;
//#line 202
  ;
}


  void DMAChannel2_IRQHandler(void)
{
  while (1) ;
//#line 208
  ;
}


  void DMAChannel3_IRQHandler(void)
{
  while (1) ;
//#line 214
  ;
}


  void DMAChannel4_IRQHandler(void)
{
  while (1) ;
//#line 220
  ;
}


  void DMAChannel5_IRQHandler(void)
{
  while (1) ;
//#line 226
  ;
}


  void DMAChannel6_IRQHandler(void)
{
  while (1) ;
//#line 232
  ;
}


  void DMAChannel7_IRQHandler(void)
{
  while (1) ;
//#line 238
  ;
}


  void ADC_IRQHandler(void)
{
  while (1) ;
//#line 244
  ;
}


  void USB_HP_CAN_TX_IRQHandler(void)
{
  while (1) ;
//#line 250
  ;
}


  void USB_LP_CAN_RX0_IRQHandler(void)
{
  while (1) ;
//#line 256
  ;
}


  void CAN_RX1_IRQHandler(void)
{
  while (1) ;
//#line 262
  ;
}


  void CAN_SCE_IRQHandler(void)
{
  while (1) ;
//#line 268
  ;
}


  void EXTI9_5_IRQHandler(void)
{
  while (1) ;
//#line 274
  ;
}


  void TIM1_BRK_IRQHandler(void)
{
  while (1) ;
//#line 280
  ;
}


  void TIM1_UP_IRQHandler(void)
{
  while (1) ;
//#line 286
  ;
}


  void TIM1_TRG_COM_IRQHandler(void)
{
  while (1) ;
//#line 292
  ;
}


  void TIM1_CC_IRQHandler(void)
{
  while (1) ;
//#line 298
  ;
}


  void TIM2_IRQHandler(void)
{
  while (1) ;
//#line 304
  ;
}


  void TIM3_IRQHandler(void)
{
  while (1) ;
//#line 310
  ;
}


  void TIM4_IRQHandler(void)
{
  while (1) ;
//#line 316
  ;
}


  void I2C1_EV_IRQHandler(void)
{
  while (1) ;
//#line 322
  ;
}


  void I2C1_ER_IRQHandler(void)
{
  while (1) ;
//#line 328
  ;
}


  void I2C2_EV_IRQHandler(void)
{
  while (1) ;
//#line 334
  ;
}


  void I2C2_ER_IRQHandler(void)
{
  while (1) ;
//#line 340
  ;
}


  void SPI1_IRQHandler(void)
{
  while (1) ;
//#line 346
  ;
}


  void SPI2_IRQHandler(void)
{
  while (1) ;
//#line 352
  ;
}


  void USART1_IRQHandler(void)
{
  while (1) ;
//#line 358
  ;
}


  void USART2_IRQHandler(void)
{
  while (1) ;
//#line 364
  ;
}


  void USART3_IRQHandler(void)
{
  while (1) ;
//#line 370
  ;
}


  void EXTI15_10_IRQHandler(void)
{
  while (1) ;
//#line 376
  ;
}











  void USBWakeUp_IRQHandler(void)
{
  while (1) ;
//#line 391
  ;
}

//# 52 "/opt/tinyos-2.x/tos/system/RealMainP.nc"
  int main(void)
//#line 52
{
  { __nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
    {





      {
      }
//#line 60
      ;

      RealMainP$Scheduler$init();





      RealMainP$PlatformInit$init();
      while (RealMainP$Scheduler$runNextTask()) ;





      RealMainP$SoftwareInit$init();
      while (RealMainP$Scheduler$runNextTask()) ;
    }
//#line 77
    __nesc_atomic_end(__nesc_atomic); }


  __nesc_enable_interrupt();

  RealMainP$Boot$booted();


  RealMainP$Scheduler$taskLoop();




  return -1;
}

//# 123 "/opt/tinyos-2.x/tos/system/SchedulerBasicP.nc"
static  bool SchedulerBasicP$Scheduler$runNextTask(void)
{
  uint8_t nextTask;

  /* atomic removed: atomic calls only */
//#line 127
  {
    nextTask = SchedulerBasicP$popTask();
    if (nextTask == SchedulerBasicP$NO_TASK) 
      {
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

//#line 164
static   void SchedulerBasicP$TaskBasic$default$runTask(uint8_t id)
{
}

//# 64 "/opt/tinyos-2.x/tos/interfaces/TaskBasic.nc"
static  void SchedulerBasicP$TaskBasic$runTask(uint8_t arg_0x4044e188){
//#line 64
  switch (arg_0x4044e188) {
//#line 64
    case /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$updateFromTimer:
//#line 64
      /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$updateFromTimer$runTask();
//#line 64
      break;
//#line 64
    case /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$fired:
//#line 64
      /*HilTimerMilliC.AlarmToTimerMilli32*/AlarmToTimerC$0$fired$runTask();
//#line 64
      break;
//#line 64
    case /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$CancelTask:
//#line 64
      /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$CancelTask$runTask();
//#line 64
      break;
//#line 64
    case /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$errorTask:
//#line 64
      /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$errorTask$runTask();
//#line 64
      break;
//#line 64
    case MessageBufferLayerP$stateDoneTask:
//#line 64
      MessageBufferLayerP$stateDoneTask$runTask();
//#line 64
      break;
//#line 64
    case MessageBufferLayerP$sendTask:
//#line 64
      MessageBufferLayerP$sendTask$runTask();
//#line 64
      break;
//#line 64
    case MessageBufferLayerP$deliverTask:
//#line 64
      MessageBufferLayerP$deliverTask$runTask();
//#line 64
      break;
//#line 64
    case RandomCollisionLayerP$calcNextRandom:
//#line 64
      RandomCollisionLayerP$calcNextRandom$runTask();
//#line 64
      break;
//#line 64
    case Stm32SpiP$zeroTask:
//#line 64
      Stm32SpiP$zeroTask$runTask();
//#line 64
      break;
//#line 64
    case /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$grantedTask:
//#line 64
      /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$grantedTask$runTask();
//#line 64
      break;
//#line 64
    default:
//#line 64
      SchedulerBasicP$TaskBasic$default$runTask(arg_0x4044e188);
//#line 64
      break;
//#line 64
    }
//#line 64
}
//#line 64
//# 56 "/opt/tinyos-2.x/tos/chips/stm32/timer/HalStm32BusyWaitM.nc"
static   void /*BusyWaitMicroC.Stm32BusyWaitMicro*/HalStm32BusyWaitM$0$BusyWait$wait(uint16_t dt)
//#line 56
{
  uint32_t dCounts;

//#line 58
  { __nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
//#line 58
    {
      uint32_t t0 = /*BusyWaitMicroC.Stm32BusyWaitMicro*/HalStm32BusyWaitM$0$OST$get();

//#line 60
      dCounts = dt * 4 * 13;
      dCounts >>= 2;
      while (/*BusyWaitMicroC.Stm32BusyWaitMicro*/HalStm32BusyWaitM$0$OST$get() - t0 < dCounts) ;
    }
//#line 63
    __nesc_atomic_end(__nesc_atomic); }
}

//# 354 "/opt/tinyos-2.x/tos/chips/stm32/spi/Stm32SpiP.nc"
static   error_t Stm32SpiP$Resource$release(uint8_t id)
//#line 354
{
  error_t error = Stm32SpiP$ResourceArbiter$release(id);

//#line 356
  { __nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
//#line 356
    {
      if (!Stm32SpiP$ArbiterInfo$inUse()) {
          Stm32SpiP$stopSpi();
        }
    }
//#line 360
    __nesc_atomic_end(__nesc_atomic); }
  return error;
}

//# 124 "/opt/tinyos-2.x/tos/system/SimpleArbiterP.nc"
static   bool /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$ArbiterInfo$inUse(void)
//#line 124
{
  /* atomic removed: atomic calls only */
//#line 125
  {
    if (/*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$state == /*Stm32SpiC.Arbiter.Arbiter*/SimpleArbiterP$0$RES_IDLE) 
      {
        unsigned char __nesc_temp = 
//#line 127
        FALSE;

//#line 127
        return __nesc_temp;
      }
  }
//#line 129
  return TRUE;
}

//# 101 "/opt/tinyos-2.x/tos/chips/rf2xx/util/TaskletC.nc"
static   void TaskletC$Tasklet$schedule(void)
{
  { __nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
    {
      if (TaskletC$state != 0) 
        {
          TaskletC$state |= 0x80;
          {
//#line 108
            __nesc_atomic_end(__nesc_atomic); 
//#line 108
            return;
          }
        }
      TaskletC$state = 1;
    }
//#line 112
    __nesc_atomic_end(__nesc_atomic); }

  TaskletC$doit();
}

//#line 63
static void TaskletC$doit(void)
{
  for (; ; ) 
    {
      TaskletC$Tasklet$run();

      { __nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
        {
          if (TaskletC$state == 1) 
            {
              TaskletC$state = 0;
              {
//#line 74
                __nesc_atomic_end(__nesc_atomic); 
//#line 74
                return;
              }
            }
          for (; 0; ) ;
          TaskletC$state = 1;
        }
//#line 79
        __nesc_atomic_end(__nesc_atomic); }
    }
}

//# 272 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230DriverLayerP.nc"
static bool RF230DriverLayerP$isSpiAcquired(void)
{
  if (RF230DriverLayerP$SpiResource$isOwner()) {
    return TRUE;
    }
  if (RF230DriverLayerP$SpiResource$immediateRequest() == SUCCESS) 
    {
      RF230DriverLayerP$SELN$makeOutput();
      RF230DriverLayerP$SELN$set();

      return TRUE;
    }

  RF230DriverLayerP$SpiResource$request();
  return FALSE;
}

//# 108 "/opt/tinyos-2.x/tos/chips/stm32/spi/Stm32SpiP.nc"
static void Stm32SpiP$startSpi(void)
//#line 108
{
  Stm32SpiP$Spi$enableSpi(FALSE);
  { __nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
//#line 110
    {
      Stm32SpiP$Spi$initMaster();
      Stm32SpiP$Spi$enableInterrupt(FALSE);
      Stm32SpiP$Spi$setMasterDoubleSpeed(TRUE);
      Stm32SpiP$Spi$setClockPolarity(FALSE);
      Stm32SpiP$Spi$setClockPhase(FALSE);
      Stm32SpiP$Spi$setClock(0);
      Stm32SpiP$Spi$enableSpi(TRUE);
    }
//#line 118
    __nesc_atomic_end(__nesc_atomic); }
  Stm32SpiP$McuPowerState$update();
}

//#line 345
static   error_t Stm32SpiP$Resource$request(uint8_t id)
//#line 345
{
  { __nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
//#line 346
    {
      if (!Stm32SpiP$ArbiterInfo$inUse()) {
          Stm32SpiP$startSpi();
        }
    }
//#line 350
    __nesc_atomic_end(__nesc_atomic); }
  return Stm32SpiP$ResourceArbiter$request(id);
}

//# 55 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/MetadataFlagsLayerC.nc"
static   void MetadataFlagsLayerC$PacketFlag$set(uint8_t bit, message_t *msg)
{
  for (; 0; ) ;

  MetadataFlagsLayerC$getMeta(msg)->flags |= 1 << bit;
}

//# 93 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230DriverLayerP.nc"
static rf230_metadata_t *RF230DriverLayerP$getMeta(message_t *msg)
{
  return (rf230_metadata_t *) ((void *)msg + sizeof(message_t ) - RF230DriverLayerP$RadioPacket$metadataLength(msg));
}

//# 69 "/opt/tinyos-2.x/tos/lib/timer/TransformCounterC.nc"
static   /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$to_size_type /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$Counter$get(void)
{
  /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$to_size_type rv = 0;

//#line 72
  { __nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
    {
      /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$upper_count_type high = /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$m_upper;
      /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$from_size_type low = /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$CounterFrom$get();

//#line 76
      if (/*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$CounterFrom$isOverflowPending()) 
        {






          high++;
          low = /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$CounterFrom$get();
        }
      {
        /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$to_size_type high_to = high;
        /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$to_size_type low_to = low >> /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$LOW_SHIFT_RIGHT;

//#line 90
        rv = (high_to << /*LocalTimeMicroC.TransformCounterC*/TransformCounterC$0$HIGH_SHIFT_LEFT) | low_to;
      }
    }
//#line 92
    __nesc_atomic_end(__nesc_atomic); }
  return rv;
}

//# 54 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioPacket.nc"
static   uint8_t TimeStampingLayerP$SubPacket$metadataLength(message_t *arg_0x407dc1b0){
//#line 54
  unsigned char result;
//#line 54

//#line 54
  result = MetadataFlagsLayerC$RadioPacket$metadataLength(arg_0x407dc1b0);
//#line 54

//#line 54
  return result;
//#line 54
}
//#line 54
//# 90 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/IEEE154MessageLayerP.nc"
static   bool IEEE154MessageLayerP$IEEE154MessageLayer$isDataFrame(message_t *msg)
{
  return (__nesc_ntoh_leuint16((unsigned char *)&IEEE154MessageLayerP$getHeader(msg)->fcf) & IEEE154MessageLayerP$IEEE154_DATA_FRAME_MASK) == IEEE154MessageLayerP$IEEE154_DATA_FRAME_VALUE;
}

//#line 174
static   uint16_t IEEE154MessageLayerP$IEEE154MessageLayer$getDestAddr(message_t *msg)
{
  return __nesc_ntoh_leuint16((unsigned char *)&IEEE154MessageLayerP$getHeader(msg)->dest);
}

//# 74 "/opt/tinyos-2.x/tos/chips/stm32/timer/Stm32AlarmC.nc"
static   void /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$Alarm$startAt(/*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$timer_size t0, /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$timer_size dt)
//#line 74
{






  { __nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
    {
      /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$timer_size now;
//#line 83
      /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$timer_size elapsed;
//#line 83
      /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$timer_size expires;


      ;

      now = /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$HplStm32Timer$get();
      elapsed = now + 3 - t0;
      if (elapsed >= dt) {
        expires = now + 3;
        }
      else {
//#line 93
        expires = t0 + dt;
        }



      if (expires == 0) {
        expires = 1;
        }



      /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$HplStm32Compare$set(expires - 1);
      /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$HplStm32Compare$reset();
      /*HplRF230C.AlarmC.NAlarm*/Stm32AlarmC$0$HplStm32Compare$start();
    }
//#line 107
    __nesc_atomic_end(__nesc_atomic); }
}

//# 201 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/MessageBufferLayerP.nc"
static   void MessageBufferLayerP$RadioSend$sendDone(error_t error)
{
  for (; 0; ) ;

  { __nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
//#line 205
    MessageBufferLayerP$txError = error;
//#line 205
    __nesc_atomic_end(__nesc_atomic); }
  MessageBufferLayerP$sendTask$postTask();
}

//# 88 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230DriverLayerP.nc"
static void *RF230DriverLayerP$getPayload(message_t *msg)
{
  return (void *)msg + RF230DriverLayerP$RadioPacket$headerLength(msg);
}

//# 100 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/IEEE154MessageLayerP.nc"
static   bool IEEE154MessageLayerP$IEEE154MessageLayer$isAckFrame(message_t *msg)
{
  return (__nesc_ntoh_leuint16((unsigned char *)&IEEE154MessageLayerP$getHeader(msg)->fcf) & IEEE154MessageLayerP$IEEE154_ACK_FRAME_MASK) == IEEE154MessageLayerP$IEEE154_ACK_FRAME_VALUE;
}

//#line 201
static   bool IEEE154MessageLayerP$IEEE154MessageLayer$requiresAckReply(message_t *msg)
{
  return IEEE154MessageLayerP$IEEE154MessageLayer$getAckRequired(msg)
   && IEEE154MessageLayerP$IEEE154MessageLayer$isDataFrame(msg)
   && IEEE154MessageLayerP$IEEE154MessageLayer$getDestAddr(msg) == IEEE154MessageLayerP$ActiveMessageAddress$amAddress();
}

//# 95 "/opt/tinyos-2.x/tos/system/ActiveMessageAddressC.nc"
static   am_addr_t ActiveMessageAddressC$amAddress(void)
//#line 95
{
  am_addr_t myAddr;

//#line 97
  { __nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
//#line 97
    myAddr = ActiveMessageAddressC$addr;
//#line 97
    __nesc_atomic_end(__nesc_atomic); }
  return myAddr;
}

//# 864 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230DriverLayerP.nc"
static   void RF230DriverLayerP$RadioPacket$setPayloadLength(message_t *msg, uint8_t length)
{
  for (; 0; ) ;
  for (; 0; ) ;


  __nesc_hton_leuint8((unsigned char *)&RF230DriverLayerP$getHeader(msg)->length, length + 2);
}

//#line 417
static   error_t RF230DriverLayerP$RadioSend$send(message_t *msg)
{
  uint16_t time;
  uint8_t length;
  uint8_t *data;
  uint8_t header;
  uint32_t time32;
  void *timesync;

  if (((RF230DriverLayerP$cmd != RF230DriverLayerP$CMD_NONE || RF230DriverLayerP$state != RF230DriverLayerP$STATE_RX_ON) || !RF230DriverLayerP$isSpiAcquired()) || RF230DriverLayerP$radioIrq) {
    return EBUSY;
    }
  length = (RF230DriverLayerP$PacketTransmitPower$isSet(msg) ? 
  RF230DriverLayerP$PacketTransmitPower$get(msg) : 0) & RF230_TX_PWR_MASK;

  if (length != RF230DriverLayerP$txPower) 
    {
      RF230DriverLayerP$txPower = length;
      RF230DriverLayerP$writeRegister(RF230_PHY_TX_PWR, RF230_TX_AUTO_CRC_ON | RF230DriverLayerP$txPower);
    }


  if (
//#line 438
  RF230DriverLayerP$Config$requiresRssiCca(msg)
   && (RF230DriverLayerP$readRegister(RF230_PHY_RSSI) & RF230_RSSI_MASK) > (RF230DriverLayerP$rssiClear + RF230DriverLayerP$rssiBusy) >> 3) {
    return EBUSY;
    }
  RF230DriverLayerP$writeRegister(RF230_TRX_STATE, RF230_PLL_ON);


  time32 = RF230DriverLayerP$LocalTime$get();
  timesync = RF230DriverLayerP$PacketTimeSyncOffset$isSet(msg) ? (void *)msg + RF230DriverLayerP$PacketTimeSyncOffset$get(msg) : 0;


  if ((RF230DriverLayerP$readRegister(RF230_TRX_STATUS) & RF230_TRX_STATUS_MASK) != RF230_PLL_ON) 
    {
      for (; 0; ) ;

      RF230DriverLayerP$state = RF230DriverLayerP$STATE_PLL_ON_2_RX_ON;
      return EBUSY;
    }


  { __nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
    {
      RF230DriverLayerP$SLP_TR$set();
      time = RF230DriverLayerP$RadioAlarm$getNow() + RF230DriverLayerP$TX_SFD_DELAY;
    }
//#line 462
    __nesc_atomic_end(__nesc_atomic); }
  RF230DriverLayerP$SLP_TR$clr();


  for (; 0; ) ;

  RF230DriverLayerP$SELN$clr();
  RF230DriverLayerP$FastSpiByte$splitWrite(RF230_CMD_FRAME_WRITE);

  data = RF230DriverLayerP$getPayload(msg);
  length = __nesc_ntoh_leuint8((unsigned char *)&RF230DriverLayerP$getHeader(msg)->length);


  RF230DriverLayerP$FastSpiByte$splitReadWrite(length);


  length -= 2;

  header = RF230DriverLayerP$Config$headerPreloadLength();
  if (header > length) {
    header = length;
    }
  length -= header;


  do {
      RF230DriverLayerP$FastSpiByte$splitReadWrite(* data++);
    }
  while (--header != 0);










  time32 += (int16_t )time - (int16_t )time32;

  if (timesync != 0) {
    __nesc_hton_int32((unsigned char *)&* (timesync_relative_t *)timesync, * (timesync_absolute_t *)timesync - time32);
    }
  while (length-- != 0) 
    RF230DriverLayerP$FastSpiByte$splitReadWrite(* data++);


  RF230DriverLayerP$FastSpiByte$splitRead();
  RF230DriverLayerP$SELN$set();
//#line 526
  RF230DriverLayerP$writeRegister(RF230_TRX_STATE, RF230_RX_ON);
//#line 541
  if (timesync != 0) {
    * (timesync_absolute_t *)timesync = __nesc_ntoh_int32((unsigned char *)&* (timesync_relative_t *)timesync) + time32;
    }
  RF230DriverLayerP$PacketTimeStamp$set(msg, time32);


  RF230DriverLayerP$state = RF230DriverLayerP$STATE_BUSY_TX_2_RX_ON;
  RF230DriverLayerP$cmd = RF230DriverLayerP$CMD_TRANSMIT;

  return SUCCESS;
}

//# 50 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/MetadataFlagsLayerC.nc"
static   bool MetadataFlagsLayerC$PacketFlag$get(uint8_t bit, message_t *msg)
{
  return MetadataFlagsLayerC$getMeta(msg)->flags & (1 << bit);
}

//# 859 "/opt/tinyos-2.x/tos/chips/rf2xx/rf230/RF230DriverLayerP.nc"
static   uint8_t RF230DriverLayerP$RadioPacket$payloadLength(message_t *msg)
{
  return __nesc_ntoh_leuint8((unsigned char *)&RF230DriverLayerP$getHeader(msg)->length) - 2;
}

//# 38 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioAlarm.nc"
static   void RandomCollisionLayerP$RadioAlarm$wait(uint16_t arg_0x407c7958){
//#line 38
  RadioAlarmP$RadioAlarm$wait(1U, arg_0x407c7958);
//#line 38
}
//#line 38
//# 73 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/RandomCollisionLayerP.nc"
static uint16_t RandomCollisionLayerP$getBackoff(uint16_t maxBackoff)
{
  uint16_t a;

  { __nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
    {
      a = RandomCollisionLayerP$nextRandom;
      RandomCollisionLayerP$nextRandom += 273;
    }
//#line 81
    __nesc_atomic_end(__nesc_atomic); }
  RandomCollisionLayerP$calcNextRandom$postTask();

  return a % maxBackoff + RandomCollisionLayerP$Config$getMinimumBackoff();
}

//# 58 "/opt/tinyos-2.x/tos/system/RandomMlcgC.nc"
static   uint32_t RandomMlcgC$Random$rand32(void)
//#line 58
{
  uint32_t mlcg;
//#line 59
  uint32_t p;
//#line 59
  uint32_t q;
  uint64_t tmpseed;

//#line 61
  { __nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
    {
      tmpseed = (uint64_t )33614U * (uint64_t )RandomMlcgC$seed;
      q = tmpseed;
      q = q >> 1;
      p = tmpseed >> 32;
      mlcg = p + q;
      if (mlcg & 0x80000000) {
          mlcg = mlcg & 0x7FFFFFFF;
          mlcg++;
        }
      RandomMlcgC$seed = mlcg;
    }
//#line 73
    __nesc_atomic_end(__nesc_atomic); }
  return mlcg;
}

//# 262 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/IEEE154MessageLayerP.nc"
static   uint8_t IEEE154MessageLayerP$RadioPacket$headerLength(message_t *msg)
{
  return IEEE154MessageLayerP$SubPacket$headerLength(msg) + sizeof(ieee154_header_t );
}

//# 196 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/ActiveMessageLayerP.nc"
static   uint8_t ActiveMessageLayerP$RadioPacket$headerLength(message_t *msg)
{
  return ActiveMessageLayerP$SubPacket$headerLength(msg) + sizeof(activemessage_header_t );
}

//# 88 "/opt/tinyos-2.x/tos/chips/rf2xx/util/TaskletC.nc"
static  void TaskletC$Tasklet$resume(void)
{
  { __nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
    {
      if (--TaskletC$state != 0x80) {
        {
//#line 93
          __nesc_atomic_end(__nesc_atomic); 
//#line 93
          return;
        }
        }
//#line 95
      TaskletC$state = 1;
    }
//#line 96
    __nesc_atomic_end(__nesc_atomic); }

  TaskletC$doit();
}

//# 166 "/opt/tinyos-2.x/tos/system/AMQueueImplP.nc"
static void /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$tryToSend(void)
//#line 166
{
  /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$nextPacket();
  if (/*AMQueueP.AMQueueImplP*/AMQueueImplP$0$current < 1) {
      error_t nextErr;
      message_t *nextMsg = /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$queue[/*AMQueueP.AMQueueImplP*/AMQueueImplP$0$current].msg;
      am_id_t nextId = /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$AMPacket$type(nextMsg);
      am_addr_t nextDest = /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$AMPacket$destination(nextMsg);
      uint8_t len = /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$Packet$payloadLength(nextMsg);

//#line 174
      nextErr = /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$AMSend$send(nextId, nextDest, nextMsg, len);
      if (nextErr != SUCCESS) {
          /*AMQueueP.AMQueueImplP*/AMQueueImplP$0$errorTask$postTask();
        }
    }
}

//# 63 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/ActiveMessageLayerP.nc"
static  error_t ActiveMessageLayerP$AMSend$send(am_id_t id, am_addr_t addr, message_t *msg, uint8_t len)
{
  if (len > ActiveMessageLayerP$Packet$maxPayloadLength()) {
    return EINVAL;
    }
  ActiveMessageLayerP$Packet$setPayloadLength(msg, len);
  ActiveMessageLayerP$AMPacket$setSource(msg, ActiveMessageLayerP$AMPacket$address());
  ActiveMessageLayerP$AMPacket$setGroup(msg, ActiveMessageLayerP$AMPacket$localGroup());
  ActiveMessageLayerP$AMPacket$setType(msg, id);
  ActiveMessageLayerP$AMPacket$setDestination(msg, addr);

  ActiveMessageLayerP$SendNotifier$aboutToSend(id, addr, msg);

  return ActiveMessageLayerP$SubSend$send(msg);
}

//# 48 "/opt/tinyos-2.x/tos/chips/rf2xx/util/RadioPacket.nc"
static   uint8_t MetadataFlagsLayerC$SubPacket$maxPayloadLength(void){
//#line 48
  unsigned char result;
//#line 48

//#line 48
  result = RF230DriverLayerP$RadioPacket$maxPayloadLength();
//#line 48

//#line 48
  return result;
//#line 48
}
//#line 48
//# 272 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/IEEE154MessageLayerP.nc"
static   void IEEE154MessageLayerP$RadioPacket$setPayloadLength(message_t *msg, uint8_t length)
{
  IEEE154MessageLayerP$SubPacket$setPayloadLength(msg, length + sizeof(ieee154_header_t ));
}

//# 63 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/MessageBufferLayerP.nc"
static  error_t MessageBufferLayerP$SplitControl$start(void)
{
  error_t error;

  MessageBufferLayerP$Tasklet$suspend();

  if (MessageBufferLayerP$state != MessageBufferLayerP$STATE_READY) {
    error = EBUSY;
    }
  else {
//#line 72
    error = MessageBufferLayerP$RadioState$turnOn();
    }
  if (error == SUCCESS) {
    MessageBufferLayerP$state = MessageBufferLayerP$STATE_TURN_ON;
    }
  MessageBufferLayerP$Tasklet$resume();

  return error;
}

//# 188 "/opt/tinyos-2.x/tos/chips/stm32/timer/STM32RtcC.nc"
static   void STM32RtcC$Alarm$startAt(uint32_t t0, uint32_t dt)
{
  {
    uint32_t now = STM32RtcC$Alarm$getNow();
    uint32_t elapsed = now - t0;

//#line 193
    now = RTC_GetCounter();
    if (elapsed >= dt) 
      {

        RTC_SetAlarm(now + 1);
        { __nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
//#line 198
          STM32RtcC$alarm = now + 1;
//#line 198
          __nesc_atomic_end(__nesc_atomic); }
        RTC_WaitForLastTask();
      }
    else 
      {
        uint32_t remaining = dt - elapsed;

//#line 204
        if (remaining <= 1) 
          {
            RTC_SetAlarm(now + 1);
            { __nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
//#line 207
              STM32RtcC$alarm = now + 1;
//#line 207
              __nesc_atomic_end(__nesc_atomic); }
            RTC_WaitForLastTask();
          }
        else 
          {
            RTC_SetAlarm(now + remaining);
            { __nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
//#line 213
              STM32RtcC$alarm = now + remaining;
//#line 213
              __nesc_atomic_end(__nesc_atomic); }
            RTC_WaitForLastTask();
          }
      }
    STM32RtcC$enableInterrupt();
  }
}

//# 62 "/opt/tinyos-2.x/tos/lib/timer/VirtualizeTimerC.nc"
static void /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$fireTimers(uint32_t now)
{
  uint8_t num;

  for (num = 0; num < /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$NUM_TIMERS; num++) 
    {
      /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$Timer_t *timer = &/*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$m_timers[num];

      if (timer->isrunning) 
        {
          uint32_t elapsed = now - timer->t0;

          if (elapsed >= timer->dt) 
            {
              if (timer->isoneshot) {
                timer->isrunning = FALSE;
                }
              else {
//#line 79
                timer->t0 += timer->dt;
                }
              /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$Timer$fired(num);
              break;
            }
        }
    }
  /*HilTimerMilliC.VirtTimersMilli32*/VirtualizeTimerC$0$updateFromTimer$postTask();
}

//# 248 "/opt/tinyos-2.x/tos/chips/rf2xx/layers/ActiveMessageLayerP.nc"
static  void *ActiveMessageLayerP$Packet$getPayload(message_t *msg, uint8_t len)
{
  if (len > ActiveMessageLayerP$RadioPacket$maxPayloadLength()) {
    return (void *)0;
    }
  return (void *)msg + ActiveMessageLayerP$RadioPacket$headerLength(msg);
}

//# 115 "/opt/tinyos-2.x/tos/chips/stm32/timer/HplStm32Timer3P.nc"
__attribute((interrupt))   void SIG_OUTPUT_COMPARE1A(void)
//#line 115
{
  HplStm32Timer3P$CompareA$fired();
}

__attribute((interrupt))   void SIG_OUTPUT_COMPARE1C(void)
//#line 119
{
  HplStm32Timer3P$CompareC$fired();
}

__attribute((interrupt))   void SIG_INPUT_CAPTURE1(void)
//#line 123
{
  HplStm32Timer3P$Capture$captured(HplStm32Timer3P$Timer$get());
}

__attribute((interrupt))   void SIG_OVERFLOW1(void)
//#line 127
{
  HplStm32Timer3P$Timer$overflow();
}

