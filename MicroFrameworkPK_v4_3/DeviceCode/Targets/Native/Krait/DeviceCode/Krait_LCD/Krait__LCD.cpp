
#include<tinyhal.h>
#include "..\Krait.h"
#include "fbcon.h"
#include "Krait__LCD.h"
#include "splash.h"

extern struct fbcon_config *mipi_init(void);
extern void mipi_dsi_shutdown(void);
extern void display_image_on_screen(void);
extern void fbcon_putc(char c);
extern void clock_config(UINT32 ns, UINT32 md, UINT32 ns_addr, UINT32 md_addr);
extern void mdelay(unsigned msecs);
static struct fbcon_config *config = NULL;
static INT32 LCDWidth;
static INT32 LCDHeight;
static INT32 bitsPerPixel;
static UINT32 pixelClockDivider;
static INT32 orientation;

#define LDO_P_MASK (1 << 7)

#define LDO_2      (2)
#define LDO_8      (8  | LDO_P_MASK)
#define LDO_23     (23 | LDO_P_MASK)

enum
{
	LDO_VOLTAGE_1_2V = 0,
	LDO_VOLTAGE_1_8V = 1,
	LDO_VOLTAGE_3_0V = 2,
	LDO_VOLTAGE_ENTRIES
};

static UINT8 ldo_n_voltage_mult[LDO_VOLTAGE_ENTRIES] = {
	18, /* 1.2V */
	0,
	0,
	};

static UINT8 ldo_p_voltage_mult[LDO_VOLTAGE_ENTRIES] = {
	0,
	6, /* 1.8V */
	30, /* 3.0V */
	};


typedef struct
{
	UINT32 initialized;

	pm8921_read_func	read;
	pm8921_write_func	write;

} pm8921_dev_t;

static pm8921_dev_t *dev;
static pm8921_dev_t pmic;

struct pm8921_gpio {
	int direction;
	int output_buffer;
	int output_value;
	int pull;
	int vin_sel;
	int out_strength;
	int function;
	int inv_int_pol;
	int disable_pin;
};

void pm8921_init(pm8921_dev_t *pmic)
{
	ASSERT(pmic);
	ASSERT(pmic->read);
	ASSERT(pmic->write);

	dev = pmic;

	dev->initialized = 1;
}

int pm8921_gpio_config(int gpio, struct pm8921_gpio *param)
{
	int ret;
	UINT8 bank[6];
	UINT8 output_buf_config;
	UINT8 output_value;

	static UINT8 dir_map[] = {
		PM_GPIO_MODE_OFF,
		PM_GPIO_MODE_OUTPUT,
		PM_GPIO_MODE_INPUT,
		PM_GPIO_MODE_BOTH,
	};

	if (param == NULL) {
		hal_printf("Gpio Structure not defined");
	}

	/* Select banks and configure the gpio */
	bank[0] = PM_GPIO_WRITE |
		((param->vin_sel << PM_GPIO_VIN_SHIFT) &
			PM_GPIO_VIN_MASK) |
		PM_GPIO_MODE_ENABLE;

	/* bank1 */
	if ((param->direction & PM_GPIO_DIR_OUT) && param->output_buffer)
		output_buf_config = PM_GPIO_OUT_BUFFER_OPEN_DRAIN;
	else
		output_buf_config = 0;

	if ((param->direction & PM_GPIO_DIR_OUT) && param->output_value)
		output_value = 1;
	else
		output_value = 0;

	bank[1] = PM_GPIO_WRITE |
		((1 << PM_GPIO_BANK_SHIFT) & PM_GPIO_BANK_MASK) |
		((dir_map[param->direction] << PM_GPIO_MODE_SHIFT)
						& PM_GPIO_MODE_MASK) |
		output_buf_config |
		output_value;

	bank[2] = PM_GPIO_WRITE |
		((2 << PM_GPIO_BANK_SHIFT) & PM_GPIO_BANK_MASK) |
		((param->pull << PM_GPIO_PULL_SHIFT) &
			PM_GPIO_PULL_MASK);

	bank[3] = PM_GPIO_WRITE |
		((3 << PM_GPIO_BANK_SHIFT) & PM_GPIO_BANK_MASK) |
		((param->out_strength << PM_GPIO_OUT_STRENGTH_SHIFT) &
			PM_GPIO_OUT_STRENGTH_MASK) |
		(param->disable_pin ? PM_GPIO_PIN_DISABLE : PM_GPIO_PIN_ENABLE);

	bank[4] = PM_GPIO_WRITE |
		((4 << PM_GPIO_BANK_SHIFT) & PM_GPIO_BANK_MASK) |
		((param->function << PM_GPIO_FUNC_SHIFT) &
			PM_GPIO_FUNC_MASK);

	bank[5] = PM_GPIO_WRITE |
		((5 << PM_GPIO_BANK_SHIFT) & PM_GPIO_BANK_MASK) |
		(param->inv_int_pol ? 0 : PM_GPIO_NON_INT_POL_INV);

	ret = dev->write(bank, 6, GPIO_CNTL(gpio));
	if (ret) {
		hal_printf("Failed to write");
		return -1;
	}
	return 0;
}


void panel_backlight_on(void)
{
	struct pm8921_gpio backlight_pwm = {
		PM_GPIO_DIR_OUT,
		0,
		0,
		PM_GPIO_PULL_NO,
		2,
		PM_GPIO_STRENGTH_HIGH,
		PM_GPIO_FUNC_1,
		0,
		0,
	};

	int rc = pm8921_gpio_config(PM_GPIO(24), &backlight_pwm);
	if (rc) {
		hal_printf("Failed to turn on panel backlight");
	}
}

void panel_backlight_off(void)
{
	struct pm8921_gpio backlight_pwm = {
		PM_GPIO_DIR_IN,
		0,
		0,
		PM_GPIO_PULL_NO,
		2,
		PM_GPIO_STRENGTH_HIGH,
		PM_GPIO_FUNC_1,
		0,
		0,
	};

	int rc = pm8921_gpio_config(PM_GPIO(24), &backlight_pwm);
	if (rc) {
		hal_printf("Failed to turn on panel backlight");
	} else hal_printf("Turned off panel backlight\r\n");
}

int pa1_ssbi2_read_bytes(unsigned char  *buffer, unsigned short length,
                                                unsigned short slave_addr)
{
    unsigned val = 0x0;
    unsigned temp = 0x0000;
    unsigned char *buf = buffer;
    unsigned short len = length;
    unsigned short addr = slave_addr;
    unsigned long timeout = SSBI_TIMEOUT_US;

    while(len)
    {
        val |= ((addr << PA1_SSBI2_REG_ADDR_SHIFT) |
		(PA1_SSBI2_CMD_READ << PA1_SSBI2_CMD_RDWRN_SHIFT));
        writel(val, PA1_SSBI2_CMD);
        while(!((temp = readl(PA1_SSBI2_RD_STATUS)) & (1 << PA1_SSBI2_TRANS_DONE_SHIFT))) {
            if (--timeout == 0) {
	        hal_printf("In device ready function : Timeout");
	        return 1;
	    }
	}
        len--;
        *buf++ = (temp & (PA1_SSBI2_REG_DATA_MASK << PA1_SSBI2_REG_DATA_SHIFT));
    }
    return 0;
}

int pa1_ssbi2_write_bytes(unsigned char  *buffer, unsigned short length,
                                                unsigned short slave_addr)
{
    unsigned val;
    unsigned char *buf = buffer;
    unsigned short len = length;
    unsigned short addr = slave_addr;
    unsigned temp = 0x00;
    unsigned char written_data1 = 0x00;
    unsigned long timeout = SSBI_TIMEOUT_US;
    //unsigned char written_data2 = 0x00;

    while(len)
    {
        temp = 0x00;
        written_data1 = 0x00;
        val = (addr << PA1_SSBI2_REG_ADDR_SHIFT) |
	  (PA1_SSBI2_CMD_WRITE << PA1_SSBI2_CMD_RDWRN_SHIFT) |
 (*buf & 0xFF);
        writel(val, PA1_SSBI2_CMD);
        while(!((temp = readl(PA1_SSBI2_RD_STATUS)) & (1 << PA1_SSBI2_TRANS_DONE_SHIFT))) {
            if (--timeout == 0) {
	        hal_printf("In device write function : Timeout");
	        return 1;
	    }
	}
        len--;
        buf++;
    }
    return 0;
}

int pm8921_ldo_set_voltage(UINT32 ldo_id, UINT32 voltage)
{
	UINT8 mult;
	UINT8 val = 0;
	UINT32 ldo_number = (ldo_id & ~LDO_P_MASK);
	INT32 ret = 0;

	/* Find the voltage multiplying factor */
	if(ldo_id & LDO_P_MASK)
		mult = ldo_p_voltage_mult[voltage];
	else
		mult = ldo_n_voltage_mult[voltage];

	/* Program the TEST reg */
	if (ldo_id & LDO_P_MASK){
		/* Bank 2, only for p ldo, use 1.25V reference */
		val = 0x0;
		val |= ( 1 << PM8921_LDO_TEST_REG_RW );
		val |= ( 2 << PM8921_LDO_TEST_REG_BANK_SEL);
		ret = dev->write(&val, 1, PM8921_LDO_TEST_REG(ldo_number));
		if (ret) {
			hal_printf("Failed to write to PM8921 LDO Test Reg ret=%d.\n", ret);
			return -1;
		}

		/* Bank 4, only for p ldo, disable output range ext, normal capacitance */
		val = 0x0;
		val |= ( 1 << PM8921_LDO_TEST_REG_RW );
		val |= ( 4 << PM8921_LDO_TEST_REG_BANK_SEL);
		ret = dev->write(&val, 1, PM8921_LDO_TEST_REG(ldo_number));
		if (ret) {
			hal_printf("Failed to write to PM8921 LDO Test Reg ret=%d.\n", ret);
			return -1;
		}
	}

	/* Program the CTRL reg */
	val = 0x0;
	val |= ( 1 << PM8921_LDO_CTRL_REG_ENABLE);
	val |= ( 1 << PM8921_LDO_CTRL_REG_PULL_DOWN);
	val |= ( 0 << PM8921_LDO_CTRL_REG_POWER_MODE);
	val |= ( mult << PM8921_LDO_CTRL_REG_VOLTAGE);
	ret = dev->write(&val, 1, PM8921_LDO_CTRL_REG(ldo_number));
	if (ret) {
		hal_printf("Failed to write to PM8921 LDO Ctrl Reg ret=%d.\n", ret);
		return -1;
	}

	return 0;
}


void mipi_dsi_panel_power_on(void)
{

	/* Turn on LDO8 for lcd1 mipi vdd */
	pm8921_ldo_set_voltage(LDO_8, LDO_VOLTAGE_3_0V);

	/* Turn on LDO23 for lcd1 mipi vddio */
	pm8921_ldo_set_voltage(LDO_23, LDO_VOLTAGE_1_8V);

	/* Turn on LDO2 for vdda_mipi_dsi */
	pm8921_ldo_set_voltage(LDO_2, LDO_VOLTAGE_1_2V);

}

#if DISPLAY_MIPI_PANEL_HXMAX
 void mipi_panel_reset(void)
 {
        int rc = 0;

       struct pm8921_gpio gpio43_param_0 = {
               PM_GPIO_DIR_OUT,
               0,
               0,
               PM_GPIO_PULL_UP_31_5,
               2,
               PM_GPIO_STRENGTH_HIGH,
               PM_GPIO_FUNC_PAIRED,
               0,
               0,
       };
       struct pm8921_gpio gpio43_param_1 = {
               PM_GPIO_DIR_OUT,
               0,
               1,
               PM_GPIO_PULL_UP_31_5,
               2,
               PM_GPIO_STRENGTH_HIGH,
               PM_GPIO_FUNC_PAIRED,
               0,
               0,
       };
       rc = pm8921_gpio_config(PM_GPIO(43), &gpio43_param_1);
       if (rc)
       {
               hal_printf("FAIL pm8921_gpio_config(): rc=%d.\n", rc);
       }
        mdelay(100);
       rc = pm8921_gpio_config(PM_GPIO(43), &gpio43_param_0);
       if (rc)
       {
               hal_printf("FAIL pm8921_gpio_config(): rc=%d.\n", rc);
       }
        mdelay(100);
       rc = pm8921_gpio_config(PM_GPIO(43), &gpio43_param_1);
       if (rc)
       {
               hal_printf("FAIL pm8921_gpio_config(): rc=%d.\n", rc);
       }
        mdelay(150);
}
#else
void mipi_panel_reset(void)
{

	struct pm8921_gpio gpio43_param = {
		  PM_GPIO_DIR_OUT,
		  0,
		  1,
		  PM_GPIO_PULL_UP_30,
		  2,
		  PM_GPIO_STRENGTH_HIGH,
		  PM_GPIO_FUNC_PAIRED,
		  0,
		  0,
	};
	pm8921_gpio_config(PM_GPIO(43), &gpio43_param);
}
#endif

void pll1_enable(void)
{
	UINT32 val = 0;

	/* Reset MND divider */
	val |= (1 << 2);
	writel(val, MM_PLL1_MODE_REG);

	/* Use PLL -- Disable Bypass */
	val |= (1 << 1);
	writel(val, MM_PLL1_MODE_REG);

	/* Activate PLL out control */
	val |= 1;
	writel(val, MM_PLL1_MODE_REG);

	while (!readl(MM_PLL1_STATUS_REG)) ;
}

void config_mmss_clk(UINT32 ns,
		     UINT32 md,
		     UINT32 cc,
		     UINT32 ns_addr, UINT32 md_addr, UINT32 cc_addr)
{
	unsigned int val = 0;

	clock_config(ns, md, ns_addr, md_addr);

	/* Enable MND counter */
	val = cc | (1 << 5);
	val = val | readl(cc_addr);
	writel(val, cc_addr);

	/* Enable the root of the clock tree */
	val = 1 << 2;
	val = val | readl(cc_addr);
	writel(val, cc_addr);

	/* Enable the Pixel Clock */
	val = 1 << 0;
	val = val | readl(cc_addr);
	writel(val, cc_addr);

	/* Force On */
	val = 1 << 31;
	val = val | readl(cc_addr);
	writel(val, cc_addr);
}

void config_mdp_lut_clk(void)
{
	/* Force on */
	writel(MDP_LUT_VAL, MDP_LUT_CC_REG);
}

/* Turn on MDP related clocks and pll's for MDP */
void mdp_clock_init(void)
{
	/* Turn on the PLL1, as source for  MDP clock */
	pll1_enable();

	/* Turn on MDP clk */
	config_mmss_clk(MDP_NS_VAL, MDP_MD_VAL,
			MDP_CC_VAL, MDP_NS_REG, MDP_MD_REG, MDP_CC_REG);

	/* Seems to lose pixels without this from status 0x051E0048 */
	config_mdp_lut_clk();
}


/* Initialize all clocks needed by Display */
void mmss_clock_init(void)
{

#if DISPLAY_MIPI_PANEL_HXMAX
	/* Configure Pixel clock */
		config_mmss_clk(0x2003, 0x01fb, 0x05, PIXEL_NS_REG,
				PIXEL_MD_REG, PIXEL_CC_REG);

		/* Configure DSI clock */
		config_mmss_clk(0x03, 0x03fb, 0x05, DSI_NS_REG,
				DSI_MD_REG, DSI_CC_REG);

#else
	/* Configure Pixel clock */
	config_mmss_clk(PIXEL_NS_VAL, PIXEL_MD_VAL, PIXEL_CC_VAL, PIXEL_NS_REG,
			PIXEL_MD_REG, PIXEL_CC_REG);

	/* Configure DSI clock */
	config_mmss_clk(DSI_NS_VAL, DSI_MD_VAL, DSI_CC_VAL, DSI_NS_REG,
			DSI_MD_REG, DSI_CC_REG);

#endif
	/* Configure Byte clock */
	config_mmss_clk(BYTE_NS_VAL, 0x0, BYTE_CC_VAL, BYTE_NS_REG, 0x0,
			BYTE_CC_REG);

	/* Configure ESC clock */
	config_mmss_clk(ESC_NS_VAL, 0x0, ESC_CC_VAL, ESC_NS_REG, 0x0,
			ESC_CC_REG);
}

UINT32 PixelsPerWord()
{
    return ((8*sizeof(UINT32)) / bitsPerPixel);
}

UINT32 WidthInWords()
{
	return ((LCDWidth + (PixelsPerWord() - 1)) / PixelsPerWord());
}

void display_init(void)
{
	struct fbcon_config *fb_cfg;

	mipi_dsi_panel_power_on();
	mipi_panel_reset();

	mdp_clock_init();
	mmss_clock_init();

	fb_cfg = mipi_init();
	fbcon_setup(fb_cfg);
}


BOOL LCD_Initialize()
{
	pmic.read = (pm8921_read_func) & pa1_ssbi2_read_bytes;
	pmic.write = (pm8921_write_func) & pa1_ssbi2_write_bytes;

	pm8921_init(&pmic);


	display_init();

	display_image_on_screen();

	panel_backlight_on();

	config = get_fbcon_config();
	LCDWidth = config->width;
	LCDHeight = config->height;
	bitsPerPixel = config->bpp;
	pixelClockDivider = 0;
	orientation = 0;

	hal_printf("Initialized.\r\n");
}

BOOL LCD_Uninitialize()
{
	//hal_printf("LCD_Unit\r\n");
	panel_backlight_off();
    return TRUE;
}

void LCD_Clear()
{
	hal_printf("LCD_Clear() called\r\n");
	unsigned count = LCDWidth * LCDHeight;
	memset(config->base, RGB888_BLACK, count * (bitsPerPixel / 8));
}

void LCD_WriteFormattedChar ( unsigned char c )
{
}

#define FONT_GETBIT(_x,_y,_data,_widthInWords) (((_data[((_x)/32) + (_y)*(_widthInWords)])>>((_x)%32)) & 0x1)
void LCD_WriteChar ( unsigned char c, int row, int col)
{
	// convert to LCD pixel coordinates
    row *= Font_Height();
    col *= Font_Width();

    if(row > (LCDHeight - Font_Height())) return;
    if(col > (LCDWidth  - Font_Width() )) return;

    const UINT8* font = Font_GetGlyph( c );

    UINT16* ScreenBuffer = (UINT16*)config->base;

    for(int y = 0; y < Font_Height(); y++)
    {
        for(int x = 0; x < Font_Width(); x+=2)
        {
           /* UINT32 val = 0;
            // the font data is mirrored
            if(FONT_GETBIT( Font_Width() -  x   , y, font, 1 )) val |= 0x07e0;
            if(FONT_GETBIT( Font_Width() - (x+1), y, font, 1 )) val |= 0x07e00000;
            
            ScreenBuffer[(row+y)*WidthInWords() + (col+x)/2] = val;
            //config->base[(row+y)*WidthInWords() + (col+x)/2] = val;*/
        }
    }

}

INT32 LCD_GetWidth()
{
    return LCDWidth;
}

INT32 LCD_GetHeight()
{
    return LCDHeight;
}

INT32 LCD_GetBitsPerPixel()
{
    return bitsPerPixel;
}

INT32 LCD_GetOrientation()
{
    return orientation;
}

void LCD_PowerSave ( BOOL On )
{
}

UINT32 LCD_ConvertColor(UINT32 color)
{
}

void LCD_BitBltEx ( int x, int y, int width, int height, char data[])
{
	hal_printf("BitBltEx char\r\n");

	ASSERT((x >= 0) && ((x+width)  <= LCDWidth));
    ASSERT((y >= 0) && ((y+height) <= LCDHeight));
    ASSERT(LCDWidth % 4 == 0);

	unsigned i = 0;
    unsigned bytes_per_bpp = ((config->bpp) / 8);
	unsigned image_base = y*LCDWidth + x;
    
    for (i = 0; i < height; i++)
    {
    	memcpy (config->base + ((image_base + (i * (config->width))) * bytes_per_bpp),
		data + (i * width * bytes_per_bpp),
		width * bytes_per_bpp);
	}

    if (config->update_start)
		config->update_start();
	if (config->update_done)
		while (!config->update_done());
}

void LCD_BitBltEx ( int x, int y, int width, int height, UINT32 data[])
{
	hal_printf("BitBltEx uint32\r\n");

	//ASSERT((x >= 0) && ((x+width)  <= LCDWidth));
    //ASSERT((y >= 0) && ((y+height) <= LCDHeight));
    //ASSERT(LCDWidth % 4 == 0);

	//unsigned i = 0;
    //unsigned bytes_per_bpp = ((config->bpp) / 8);
	//unsigned image_base = y*LCDWidth + x;
    
	hal_printf("copying bitmap data\r\n");

	int i, j;
	unsigned bytes_per_bpp = ((config->bpp) / 8);
	UINT32* StartOfLine_src = &data[0];
	char* StartOfLine_dst;
	UINT32 val;

    const int screenWidth = config->width;
	
	if ( (width%2) == 0){
		for (i = 0; i < height; i++){
			StartOfLine_src = data + (i * width / 2);
			StartOfLine_dst =(char*) ((int)config->base + ((((y * screenWidth) + x) + (i * screenWidth)) * bytes_per_bpp));
			// todo height must be factor of 2
			for (j = 0; j < (width / 2); j++){
				val = *StartOfLine_src++;
				*StartOfLine_dst++ = (val & 0x001F) << 3;
				*StartOfLine_dst++ = (val & 0x07C0) >> 3;
				*StartOfLine_dst++ = (val & 0xF800) >> 8;
				*StartOfLine_dst++ = (val & 0x001F0000) >> 13;
				*StartOfLine_dst++ = (val & 0x07C00000) >> 19;
				*StartOfLine_dst++ = (val & 0xF8000000) >> 24;
			}
		}
	} else {
		for (i = 0; i < height; i++){
			StartOfLine_src = data + (i * (((width-1) / 2) + 1));
			StartOfLine_dst =(char*) ((int)config->base + ((((y * screenWidth) + x) + (i * screenWidth)) * bytes_per_bpp));
			// todo height must be factor of 2
			for (j = 0; j < ((width-1) / 2); j++){
				val = *StartOfLine_src++;
				*StartOfLine_dst++ = (val & 0x001F) << 3;
				*StartOfLine_dst++ = (val & 0x07C0) >> 3;
				*StartOfLine_dst++ = (val & 0xF800) >> 8;
				*StartOfLine_dst++ = (val & 0x001F0000) >> 13;
				*StartOfLine_dst++ = (val & 0x07C00000) >> 19;
				*StartOfLine_dst++ = (val & 0xF8000000) >> 24;
			}
			// dealing with extra pixel on end
			val = *StartOfLine_src++;
			*StartOfLine_dst++ = (val & 0x001F) << 3;
			*StartOfLine_dst++ = (val & 0x07C0) >> 3;
			*StartOfLine_dst++ = (val & 0xF800) >> 8;
		}
	}
	
    /*StartOfLine_src += (y * screenWidth) + x;
    StartOfLine_dst += (y * screenWidth) + x;

    UINT32* src;
    char* dst;
    INT32   Xcnt;

    while( height-- )
    {   
        src = StartOfLine_src;
        dst = StartOfLine_dst;
        Xcnt = width;

        while( Xcnt-- )
        {
            UINT32 val = *src++;
			*dst++ = (val & 0xf800) >> 11;
			*dst++ = (val & 0x07C0) >> 6;
			*dst++ = (val & 0x001F);
        }
        
        StartOfLine_src += screenWidth;
        StartOfLine_dst += screenWidth;
    }*/
    /*for (i = 0; i < width; i++)
    {
    	memcpy (config->base + ((image_base + (i * (config->width))) * bytes_per_bpp),
		data + (i * height * bytes_per_bpp),
		height * bytes_per_bpp);
	}*/

    if (config->update_start)
		config->update_start();
	if (config->update_done)
		while (!config->update_done());
}

void LCD_BitBlt(int width, int height, int widthInWords, UINT32 data[], BOOL fUseDelta)
{
	hal_printf("BitBlt uint32\r\n");
	LCD_BitBltEx( 0, 0, width, height, data );
	//LCD_BitBltEx( 10, 40, width, height, penguin );
}

void LCD_BitBlt(int width, int height, int widthInWords, char data[], BOOL fUseDelta)
{
	hal_printf("BitBlt char\r\n");
	LCD_BitBltEx( 0, 0, width, height, data );
	//LCD_BitBltEx( 10, 40, width, height, penguin );
}

UINT32* LCD_GetFrameBuffer()
{
	return (UINT32*) config;
}


