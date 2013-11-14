#ifndef __KRAIT_ADC_H_
#define __KRAIT_ADC_H_

#include <tinyhal.h>

#define MSM_ADC_MAX_CHAN_STR 64

/* must be <= to the max buffer size in the modem implementation */
#define MSM_ADC_DEV_MAX_INFLIGHT 9

#define MSM_ADC_IOCTL_CODE		0x90

struct msm_adc_conversion {
	/* hwmon channel number - this is not equivalent to the DAL chan */
	UINT32 chan;
	/* returned result in ms */
	int result;
};

struct adc_chan_result {
	/* The channel number of the requesting/requested conversion */
	UINT32 chan;
	/* The pre-calibrated digital output of a given ADC relative to the
	ADC reference */
	INT32 adc_code;
	/* in units specific for a given ADC; most ADC uses reference voltage
	 *  but some ADC uses reference current.  This measurement here is
	 *  a number relative to a reference of a given ADC */
	INT64 measurement;
	/* The data meaningful for each individual channel whether it is
	 * voltage, current, temperature, etc. */
	INT64 physical;
};

#if 0
/*
 * Issue a blocking adc conversion request. Once the call returns, the data
 * can be found in the 'physical' field of adc_chan_result. This call will
 * return ENODATA if there is an invalid result returned by the modem driver.
 */
#define MSM_ADC_REQUEST			_IOWR(MSM_ADC_IOCTL_CODE, 1,	\
					     struct adc_chan_result)

/*
 * Issue a non-blocking adc conversion request. The results from this
 * request can be obtained by calling AIO_READ once the transfer is
 * completed. To verify completion, the blocking call AIO_POLL can be used.
 * If there are no slot resources, this call will return an error with errno
 * set to EWOULDBLOCK.
 */
#define MSM_ADC_AIO_REQUEST		_IOWR(MSM_ADC_IOCTL_CODE, 2,	\
					     struct adc_chan_result)

/*
 * Same non-blocking semantics as AIO_REQUEST, except this call will block
 * if there are no available slot resources. This call can fail with errno
 * set to EDEADLK if there are no resources and the file descriptor in question
 * has outstanding conversion requests already. This is done so the client
 * does not block on resources that can only be freed by reading the results --
 * effectively deadlocking the system. In this case, the client must read
 * pending results before proceeding to free up resources.
 */
#define MSM_ADC_AIO_REQUEST_BLOCK_RES	_IOWR(MSM_ADC_IOCTL_CODE, 3,	\
					     struct adc_chan_result)

/*
 * Returns the number of pending results that are associated with a particular
 * file descriptor. If there are no pending results, this call will block until
 * there is at least one. If there are no requests queued at all on this file
 * descriptor, this call will fail with EDEADLK. This is to prevent deadlock in
 * a single-threaded scenario where POLL would never return.
 */
#define MSM_ADC_AIO_POLL		_IOR(MSM_ADC_IOCTL_CODE, 4,	\
					     UINT32)

#define MSM_ADC_FLUID_INIT	_IOR(MSM_ADC_IOCTL_CODE, 5,	\
					     UINT32)

#define MSM_ADC_FLUID_DEINIT	_IOR(MSM_ADC_IOCTL_CODE, 6,	\
					     UINT32)

struct msm_adc_aio_result {
	UINT32 chan;
	int result;
};

/*
 * Read the results from an AIO / non-blocking conversion request. AIO_POLL
 * should be used before using this command to verify how many pending requests
 * are available for the file descriptor. This call will fail with errno set to
 * ENOMSG if there are no pending messages to be read at the time of the call.
 * The call will return ENODATA if there is an invalid result returned by the
 * modem driver.
 */
#define MSM_ADC_AIO_READ		_IOR(MSM_ADC_IOCTL_CODE, 5,	\
					     struct adc_chan_result)

struct msm_adc_lookup {
	/* channel name (input) */
	char name[MSM_ADC_MAX_CHAN_STR];
	/* local channel index (output) */
	UINT32 chan_idx;
};

/*
 * Look up a channel name and get back an index that can be used
 * as a parameter to the conversion request commands.
 */
#define MSM_ADC_LOOKUP			_IOWR(MSM_ADC_IOCTL_CODE, 6,	\
					     struct msm_adc_lookup)
#endif

//#ifdef __KERNEL__
#define MSM_ADC_MAX_NUM_DEVS		3

enum {
	ADC_CONFIG_TYPE1,
	ADC_CONFIG_TYPE2,
	ADC_CONFIG_NONE = 0xffffffff
};

enum {
	ADC_CALIB_CONFIG_TYPE1,
	ADC_CALIB_CONFIG_TYPE2,
	ADC_CALIB_CONFIG_TYPE3,
	ADC_CALIB_CONFIG_TYPE4,
	ADC_CALIB_CONFIG_TYPE5,
	ADC_CALIB_CONFIG_TYPE6,
	ADC_CALIB_CONFIG_TYPE7,
	ADC_CALIB_CONFIG_NONE = 0xffffffff
};

enum {
	/* CHAN_PATH_TYPEn is specific for each ADC driver
	and can be used however way it wants*/
	CHAN_PATH_TYPE1,
	CHAN_PATH_TYPE2,
	CHAN_PATH_TYPE3,
	CHAN_PATH_TYPE4,
	CHAN_PATH_TYPE5,
	CHAN_PATH_TYPE6,
	CHAN_PATH_TYPE7,
	CHAN_PATH_TYPE8,
	CHAN_PATH_TYPE9,
	CHAN_PATH_TYPE10,
	CHAN_PATH_TYPE11,
	CHAN_PATH_TYPE12,
	CHAN_PATH_TYPE13,
	CHAN_PATH_TYPE14,
	CHAN_PATH_TYPE15,
	CHAN_PATH_TYPE16,
	/* A given channel connects directly to the ADC */
	CHAN_PATH_TYPE_NONE = 0xffffffff
};

#define CHANNEL_ADC_BATT_ID     0
#define CHANNEL_ADC_BATT_THERM  1
#define CHANNEL_ADC_BATT_AMON   2
#define CHANNEL_ADC_VBATT       3
#define CHANNEL_ADC_VCOIN       4
#define CHANNEL_ADC_VCHG        5
#define CHANNEL_ADC_CHG_MONITOR 6
#define CHANNEL_ADC_VPH_PWR     7
#define CHANNEL_ADC_USB_VBUS    8
#define CHANNEL_ADC_DIE_TEMP    9
#define CHANNEL_ADC_DIE_TEMP_4K 0xa
#define CHANNEL_ADC_XOTHERM     0xb
#define CHANNEL_ADC_XOTHERM_4K  0xc
#define CHANNEL_ADC_HDSET       0xd
#define CHANNEL_ADC_MSM_THERM	0xe
#define CHANNEL_ADC_625_REF	0xf
#define CHANNEL_ADC_1250_REF	0x10
#define CHANNEL_ADC_325_REF	0x11
#define CHANNEL_ADC_FSM_THERM	0x12
#define CHANNEL_ADC_PA_THERM	0x13

enum {
	CALIB_STARTED,
	CALIB_NOT_REQUIRED = 0xffffffff,
};

struct linear_graph {
	INT32 offset;
	INT32 dy; /* Slope numerator */
	INT32 dx; /* Slope denominator */
};

struct adc_map_pt {
	INT32 x;
	INT32 y;
};

struct adc_properties {
	UINT32 adc_reference; /* milli-voltage for this adc */
	UINT32 bitresolution;
	bool bipolar;
	UINT32 conversiontime;
};

struct chan_properties {
	UINT32 gain_numerator;
	UINT32 gain_denominator;
	struct linear_graph *adc_graph;
/* this maybe the same as adc_properties.ConversionTime
   if channel does not change the adc properties */
	UINT32 chan_conv_time;

};

struct msm_adc_channels {
	char *name;
	UINT32 channel_name;
	UINT32 adc_dev_instance;
	struct adc_access_fn *adc_access_fn;
	UINT32 chan_path_type;
	UINT32 adc_config_type;
	UINT32 adc_calib_type;
	INT32 (*chan_processor)(INT32, const struct adc_properties *,
		const struct chan_properties *, struct adc_chan_result *);

};

struct msm_adc_platform_data {
	struct msm_adc_channels *channel;
	UINT32 num_chan_supported;
	UINT32 num_adc;
	UINT32 chan_per_adc;
	char **dev_names;
	UINT32 target_hw;
	UINT32 gpio_config;
	u32 (*adc_gpio_enable) (int);
	u32 (*adc_gpio_disable) (int);
	u32 (*adc_fluid_enable) (void);
	u32 (*adc_fluid_disable) (void);
};

enum hw_type {
	MSM_7x30,
	MSM_8x60,
	FSM_9xxx,
};

enum epm_gpio_config {
	MPROC_CONFIG,
	APROC_CONFIG
};

enum adc_request {
	START_OF_CONV,
	END_OF_CONV,
	START_OF_CALIBRATION,
	END_OF_CALIBRATION,
};

struct adc_dev_spec {
	UINT32			hwmon_dev_idx;
	struct dal_dev_spec {
		UINT32		dev_idx;
		UINT32		chan_idx;
	} dal;
};

struct dal_conv_request {
	struct dal_dev_spec		target;
	void				*cb_h;
};

struct dal_adc_result {
	UINT32			status;
	UINT32			token;
	UINT32			dev_idx;
	UINT32			chan_idx;
	int				physical;
	UINT32			percent;
	UINT32			microvolts;
	UINT32			reserved;
};

struct dal_conv_slot {
	void				*cb_h;
	struct dal_adc_result		result;
	struct completion		comp;
	struct list_head		list;
	UINT32			idx;
	UINT32			chan_idx;
	bool				blocking;
	struct msm_client_data		*client;
};

struct dal_translation {
	UINT32			dal_dev_idx;
	UINT32			hwmon_dev_idx;
	UINT32			hwmon_start;
	UINT32			hwmon_end;
};

struct msm_client_data {
	struct list_head		complete_list;
	bool				online;
	INT32				adc_chan;
	UINT32			num_complete;
	UINT32			num_outstanding;
	wait_queue_head_t		data_wait;
	wait_queue_head_t		outst_wait;
	struct mutex lock;
};

struct adc_conv_slot {
	void				*cb_h;
	union {
		struct adc_chan_result		result;
		struct dal_adc_result		dal_result;
	} conv;
	struct completion		comp;
	struct completion		*compk;
	struct list_head		list;
	UINT32			idx;
	enum adc_request		adc_request;
	bool				blocking;
	struct msm_client_data		*client;
	struct work_struct		work;
	struct chan_properties		chan_properties;
	UINT32			chan_path;
	UINT32			chan_adc_config;
	UINT32			chan_adc_calib;
};

struct adc_access_fn {
	INT32 (*adc_select_chan_and_start_conv)(UINT32,
				struct adc_conv_slot*);
	INT32 (*adc_read_adc_code)(UINT32 dev_instance, INT32 *data);
	struct adc_properties *(*adc_get_properties)(UINT32 dev_instance);
	void (*adc_slot_request)(UINT32 dev_instance,
				struct adc_conv_slot **);
	void (*adc_restore_slot)(UINT32 dev_instance,
				struct adc_conv_slot *slot);
	INT32 (*adc_calibrate)(UINT32 dev_instance, struct adc_conv_slot*,
		int *);
};

void msm_adc_wq_work(struct work_struct *work);
void msm_adc_conv_cb(void *context, u32 param, void *evt_buf, u32 len);
#ifdef CONFIG_SENSORS_MSM_ADC
INT32 adc_channel_open(UINT32 channel, void **h);
INT32 adc_channel_close(void *h);
INT32 adc_channel_request_conv(void *h, struct completion *conv_complete_evt);
INT32 adc_channel_read_result(void *h, struct adc_chan_result *chan_result);
#else
static INT32 adc_channel_open(UINT32 channel, void **h)
{
	hal_printf("%s.not supported.\n", __func__);
	return -ENODEV;
}
static INT32 adc_channel_close(void *h)
{
	hal_printf("%s.not supported.\n", __func__);
	return -ENODEV;
}
static INT32
adc_channel_request_conv(void *h, struct completion *conv_complete_evt)
{
	hal_printf("%s.not supported.\n", __func__);
	return -ENODEV;
}
static INT32
adc_channel_read_result(void *h, struct adc_chan_result *chan_result)
{
	hal_printf("%s.not supported.\n", __func__);
	return -ENODEV;
}

#endif /* CONFIG_SENSORS_MSM_ADC */
//#endif
#endif
