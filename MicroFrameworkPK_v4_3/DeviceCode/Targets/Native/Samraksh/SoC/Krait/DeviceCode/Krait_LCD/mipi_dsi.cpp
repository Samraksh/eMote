#include <tinyhal.h>
#include "mipi_dsi.h"
#include "display.h"
#include "fbcon.h"
#include "..\Krait.h"
#include "Krait__LCD.h"

 void dsb_mipi(void)
 {

	 __asm(
		 "dsb sy"
	 	 );

 }

 static int cmd_mode_status = 0;
 void mdp_start_dma(void);
 extern int mipi_dsi_phy_init(struct mipi_dsi_panel_config *pinfo);
 extern void config_mdt61_dsi_video_mode(void);
 extern void mdp_setup_mdt61_video_dsi_config();
 extern UINT32 ntohl(UINT32 input);

 extern void
 mdp_setup_dma_p_video_config(unsigned short pack_pattern,
 			     unsigned short img_width,
 			     unsigned short img_height,
 			     unsigned long input_img_addr,
 			     unsigned short img_width_full_size,
 			     unsigned char ystride);


 int mipi_dsi_cmds_tx(struct mipi_dsi_cmd *cmds, int count);
 int mipi_dsi_cmds_rx(char **rp, int len);

#if DISPLAY_MIPI_PANEL_HXMAX
UINT32 manufacture_id = 0xA5A5A5A5;
static char gaboot_ScreenID_str[30]={0};
#endif


#if 0
static struct fbcon_config mipi_fb_cfg = {
	.height = TSH_MDT61_MIPI_FB_HEIGHT,
	.width = TSH_MDT61_MIPI_FB_WIDTH,
	.stride = TSH_MDT61_MIPI_FB_WIDTH,
	.format = FB_FORMAT_RGB888,
	.bpp = 24,
	.update_start = NULL,
	.update_done = NULL,

};
#endif

static struct fbcon_config mipi_fb_cfg;

void init_mipi_fb_cfg()
{
	mipi_fb_cfg.height = TSH_MDT61_MIPI_FB_HEIGHT;
	mipi_fb_cfg.width = TSH_MDT61_MIPI_FB_WIDTH;
	mipi_fb_cfg.stride = TSH_MDT61_MIPI_FB_WIDTH;
	mipi_fb_cfg.format = FB_FORMAT_RGB888;
	mipi_fb_cfg.bpp = 24;
	mipi_fb_cfg.update_start = NULL;
	mipi_fb_cfg.update_done = NULL;
}

struct mipi_dsi_panel_config toshiba_mdt61_panel_info;

struct mipi_dsi_panel_config hxmax_panel_info;

void init_hxmax_panel_info()
{
	hxmax_panel_info.mode = MIPI_VIDEO_MODE;
	hxmax_panel_info.num_of_lanes = 2;
	hxmax_panel_info.dsi_phy_config = &mipi_dsi_sim_panel_phy_ctrl;
	hxmax_panel_info.panel_cmds = hx8363_panel_video_mode_cmds;
	hxmax_panel_info.num_of_panel_cmds = ARRAY_SIZE(hx8363_panel_video_mode_cmds);

}

void init_toshiba_mdt61_panel_info()
{
	toshiba_mdt61_panel_info.mode = MIPI_VIDEO_MODE;
	toshiba_mdt61_panel_info.num_of_lanes = 3;
	toshiba_mdt61_panel_info.dsi_phy_config = &mipi_dsi_toshiba_mdt61_panel_phy_ctrl;
	toshiba_mdt61_panel_info.panel_cmds = toshiba_mdt61_video_mode_cmds;
	toshiba_mdt61_panel_info.num_of_panel_cmds = ARRAY_SIZE(toshiba_mdt61_video_mode_cmds);

}

#if 0
struct mipi_dsi_panel_config toshiba_mdt61_panel_info = {
	mipi_dsi_panel_config::mode = MIPI_VIDEO_MODE,
	mipi_dsi_panel_config::num_of_lanes = 3,
	mipi_dsi_panel_config::dsi_phy_config = &mipi_dsi_toshiba_mdt61_panel_phy_ctrl,
	mipi_dsi_panel_config::panel_cmds = toshiba_mdt61_video_mode_cmds,
	mipi_dsi_panel_config::num_of_panel_cmds = ARRAY_SIZE(toshiba_mdt61_video_mode_cmds),
};
#endif

static UINT32 mipi_himax_manufacture_id(void)
{
    char rec_buf[24];
    char *rp = rec_buf;
    UINT32 *lp, data;
    mipi_dsi_cmds_tx(himax_panel_manufacture_id_cmd, ARRAY_SIZE(himax_panel_manufacture_id_cmd));
    mipi_dsi_cmds_rx(&rp, 4);

    lp = (UINT32 *)rp;
    data = (UINT32)*lp;
    data = ntohl(data);
    //data = data >> 8;
    return data;
}

void mipi_dsi_panel_read_id_set_flag(void)
{
//        mipi_panel_reset();
#if 0
    gpio_set(TRULY_PANEL_RESET, 1);
    HAL_Time_Sleep_MicroSeconds(100000);
    gpio_set(TRULY_PANEL_RESET, 0);
    HAL_Time_Sleep_MicroSeconds(100000);
    gpio_set(TRULY_PANEL_RESET, 1);
    HAL_Time_Sleep_MicroSeconds(150000);
#endif
    manufacture_id =   mipi_himax_manufacture_id();

       hal_printf("manufacture_id:0x%08x\n", manufacture_id);

       if(0x01000a02 == manufacture_id)
       {
               //strcpy(gaboot_ScreenID_str," scr_id=8363\0");
               hal_printf("read correctly: 0x01000a02 gaboot_ScreenID_str: scr_id = 8363 \n");
       }
       else
       {
               //strcpy(gaboot_ScreenID_str," scr_id=35510000\0");
               hal_printf("not 8363 gaboot_ScreenID_str: scr_id-35510000s\n");
       }
}



struct mipi_dsi_panel_config *get_panel_info(void)
{
	return &hxmax_panel_info;
}

int mipi_dsi_video_config(unsigned short num_of_lanes)
{

	int status = 0;
	unsigned long ReadValue;
	unsigned long count = 0;
	unsigned long low_pwr_stop_mode = 0;	// low power mode 0x1111 start from
	// bit16, high spd mode 0x0
	unsigned char eof_bllp_pwr = 0x9;	// bit 12, 15, 1:low power stop mode or
	// let cmd mode eng send packets in hs
	// or lp mode
	unsigned short image_wd = mipi_fb_cfg.width;
	unsigned short image_ht = mipi_fb_cfg.height;
#if (!DISPLAY_MIPI_PANEL_TOSHIBA_MDT61) && (!DISPLAY_MIPI_PANEL_HXMAX)
	unsigned short display_wd = mipi_fb_cfg.width;
	unsigned short display_ht = mipi_fb_cfg.height;
	unsigned short hsync_porch_fp = MIPI_HSYNC_FRONT_PORCH_DCLK;
	unsigned short hsync_porch_bp = MIPI_HSYNC_BACK_PORCH_DCLK;
	unsigned short vsync_porch_fp = MIPI_VSYNC_FRONT_PORCH_LINES;
	unsigned short vsync_porch_bp = MIPI_VSYNC_BACK_PORCH_LINES;
	unsigned short hsync_width = MIPI_HSYNC_PULSE_WIDTH;
	unsigned short vsync_width = MIPI_VSYNC_PULSE_WIDTH;
	unsigned short dst_format = 0;
	unsigned short traffic_mode = 0;
#endif
	unsigned short pack_pattern = 0x12;	//BGR
	unsigned char ystride = 3;

	low_pwr_stop_mode = 0x1111;	// low pwr mode bit16:HSA, bit20:HBA,
	// bit24:HFP, bit28:PULSE MODE, need enough
	// time for swithc from LP to HS
	eof_bllp_pwr = 0x9;	// low power stop mode or let cmd mode eng send
	// packets in hs or lp mode

#if (DISPLAY_MIPI_PANEL_TOSHIBA_MDT61) || (DISPLAY_MIPI_PANEL_HXMAX)
	pack_pattern = 0x21;	//RGB
	config_mdt61_dsi_video_mode();


	/* Two functions make up mdp_setup_dma_p_video_mode with mdt61 panel functions */
	mdp_setup_dma_p_video_config(pack_pattern, image_wd, image_ht,
				     MIPI_FB_ADDR, image_wd, ystride);
	mdp_setup_mdt61_video_dsi_config();
#elif DISPLAY_MIPI_PANEL_RENESAS
	if (machine_is_7x25a()) {
		display_wd = REN_MIPI_FB_WIDTH_HVGA;
		display_ht = REN_MIPI_FB_HEIGHT_HVGA;
		image_wd = REN_MIPI_FB_WIDTH_HVGA;
		image_ht = REN_MIPI_FB_HEIGHT_HVGA;
		hsync_porch_fp = MIPI_HSYNC_FRONT_PORCH_DCLK_HVGA;
		hsync_porch_bp = MIPI_HSYNC_BACK_PORCH_DCLK_HVGA;
		vsync_porch_fp = MIPI_VSYNC_FRONT_PORCH_LINES_HVGA;
		vsync_porch_bp = MIPI_VSYNC_BACK_PORCH_LINES_HVGA;
		hsync_width = MIPI_HSYNC_PULSE_WIDTH_HVGA;
		vsync_width = MIPI_VSYNC_PULSE_WIDTH_HVGA;
	}

	pack_pattern = 0x21;	//RGB
	config_renesas_dsi_video_mode();

	status +=
	    mdp_setup_dma_p_video_mode(display_wd, display_ht, image_wd,
				       image_ht, hsync_porch_fp, hsync_porch_bp,
				       vsync_porch_fp, vsync_porch_bp,
				       hsync_width, vsync_width, MIPI_FB_ADDR,
				       image_wd, pack_pattern, ystride);
#else
	status +=
	    config_dsi_video_mode(display_wd, display_ht, image_wd, image_ht,
				  hsync_porch_fp, hsync_porch_bp,
				  vsync_porch_fp, vsync_porch_bp, hsync_width,
				  vsync_width, dst_format, traffic_mode,
				  num_of_lanes);

	status +=
	    mdp_setup_dma_p_video_mode(display_wd, display_ht, image_wd,
				       image_ht, hsync_porch_fp, hsync_porch_bp,
				       vsync_porch_fp, vsync_porch_bp,
				       hsync_width, vsync_width, MIPI_FB_ADDR,
				       image_wd, pack_pattern, ystride);
#endif

	ReadValue = readl(DSI_INT_CTRL) & 0x00010000;
	while (ReadValue != 0x00010000) {
		ReadValue = readl(DSI_INT_CTRL) & 0x00010000;
		count++;
		if (count > 0xffff) {
			status = FAIL;
			hal_printf("Video lane test failed\n");
			return status;
		}
	}

	hal_printf("Video lane tested successfully\n");
	return status;
}

int dsi_cmd_dma_trigger_for_panel()
{
	unsigned long ReadValue;
	unsigned long count = 0;
	int status = 0;

	writel(0x03030303, DSI_INT_CTRL);
	writel(0x1, DSI_CMD_MODE_DMA_SW_TRIGGER);
	dsb_mipi();
	ReadValue = readl(DSI_INT_CTRL) & 0x00000001;
	while (ReadValue != 0x00000001) {
		ReadValue = readl(DSI_INT_CTRL) & 0x00000001;
		count++;
		if (count > 0xffff) {
			status = FAIL;
			hal_printf("Panel CMD: command mode dma test failed\n");
			return status;
		}
	}

	writel((readl(DSI_INT_CTRL) | 0x01000001), DSI_INT_CTRL);
	//hal_printf("Panel CMD: command mode dma tested successfully\n");
	return status;
}

int mipi_dsi_cmds_rx(char **rp, int len)
{
	UINT32 *lp, data;
	char *dp;
	int i, off, cnt;
	int rlen, res;

	if (len <= 2)
		rlen = 4;	/* short read */
	else
		rlen = MIPI_DSI_MRPS + 6;	/* 4 bytes header + 2 bytes crc */

	if (rlen > MIPI_DSI_REG_LEN) {
		return 0;
	}

	res = rlen & 0x03;

	rlen += res;		/* 4 byte align */
	lp = (UINT32 *) (*rp);

	cnt = rlen;
	cnt += 3;
	cnt >>= 2;

	if (cnt > 4)
		cnt = 4;	/* 4 x 32 bits registers only */

	off = 0x068;		/* DSI_RDBK_DATA0 */
	off += ((cnt - 1) * 4);

	for (i = 0; i < cnt; i++) {
		data = (UINT32) readl(MIPI_DSI_BASE + off);
		*lp++ = ntohl(data);	/* to network byte order */
		off -= 4;
	}

	if (len > 2) {
		/*First 4 bytes + paded bytes will be header next len bytes would be payload */
		for (i = 0; i < len; i++) {
			dp = *rp;
			dp[i] = dp[4 + res + i];
		}
	}

	return len;
}


int mipi_dsi_cmds_tx(struct mipi_dsi_cmd *cmds, int count)
{
	int ret = 0;
	struct mipi_dsi_cmd *cm;
	int i = 0;
	char pload[256];
	UINT32 off;

	/* Align pload at 8 byte boundry */
	off = (UINT32) pload;
	off &= 0x07;
	if (off)
		off = 8 - off;
	off += (UINT32) pload;

	cm = cmds;
	for (i = 0; i < count; i++) {
		memcpy((void *)off, (cm->payload), cm->size);
		writel(off, DSI_DMA_CMD_OFFSET);
		writel(cm->size, DSI_DMA_CMD_LENGTH);	// reg 0x48 for this build
		dsb_mipi();
		ret += dsi_cmd_dma_trigger_for_panel();
		//writel(0x1, DSI_CMD_MODE_DMA_SW_TRIGGER);
		//dsb_mipi();
		HAL_Time_Sleep_MicroSeconds(80);
		cm++;
	}
	return ret;
}


int mipi_dsi_panel_initialize(struct mipi_dsi_panel_config *pinfo)
{
	unsigned char DMA_STREAM1 = 0;	// for mdp display processor path
	unsigned char EMBED_MODE1 = 1;	// from frame buffer
	unsigned char POWER_MODE2 = 1;	// from frame buffer
	unsigned char PACK_TYPE1 = 1;	// long packet
	unsigned char VC1 = 0;
	unsigned char DT1 = 0;	// non embedded mode
	unsigned short WC1 = 0;	// for non embedded mode only
	int status = 0;
	unsigned char DLNx_EN;

	switch (pinfo->num_of_lanes) {
	default:
	case 1:
		DLNx_EN = 1;	// 1 lane
		break;
	case 2:
		DLNx_EN = 3;	// 2 lane
		break;
	case 3:
		DLNx_EN = 7;	// 3 lane
		break;
	}

	writel(0x0001, DSI_SOFT_RESET);
	writel(0x0000, DSI_SOFT_RESET);

	writel((0 << 16) | 0x3f, DSI_CLK_CTRL);	/* Turn on all DSI Clks */
	writel(DMA_STREAM1 << 8 | 0x04, DSI_TRIG_CTRL);	// reg 0x80 dma trigger: sw
	// trigger 0x4; dma stream1

	writel(0 << 30 | DLNx_EN << 4 | 0x105, DSI_CTRL);	// reg 0x00 for this
	// build
	writel(EMBED_MODE1 << 28 | POWER_MODE2 << 26
	       | PACK_TYPE1 << 24 | VC1 << 22 | DT1 << 16 | WC1,
	       DSI_COMMAND_MODE_DMA_CTRL);

	// Start the dma
	//mdp_start_dma();
	HAL_Time_Sleep_MicroSeconds(30000);

	mipi_dsi_panel_read_id_set_flag();

	status = mipi_dsi_cmds_tx(pinfo->panel_cmds, pinfo->num_of_panel_cmds);

	return status;
}


void mipi_dsi_shutdown(void)
{
	int i;
	//hal_printf("mdp_shutdown()\r\n");
	
	//mdp_shutdown();
	//writel(0x00000000, MDP_DSI_VIDEO_EN);
	//mdelay(60);
	//writel(0x00000000, MDP_INTR_ENABLE);
	//writel(0x00000003, MDP_OVERLAYPROC0_CFG);

	//writel(0x01010101, DSI_INT_CTRL);
	//writel(0x13FF3BFF, DSI_ERR_INT_MASK0);
	// Disable branch clocks 
	writel(0x0, BYTE_CC_REG);
	writel(0x0, PIXEL_CC_REG);
	writel(0x0, ESC_CC_REG);
	// Disable root clock 
	writel(0x0, DSI_CC_REG);
	writel(0, DSI_CLK_CTRL);
	writel(0, DSI_CTRL);
	writel(0, DSIPHY_PLL_CTRL(0));
	
}

struct fbcon_config *mipi_init(void)
{
	int status = 0;
	int i;

	init_mipi_fb_cfg();

	init_hxmax_panel_info();

	struct mipi_dsi_panel_config *panel_info = get_panel_info();


	mipi_dsi_phy_init(panel_info);

	HAL_Time_Sleep_MicroSeconds(3000);
	
	status += mipi_dsi_panel_initialize(panel_info);

	mipi_fb_cfg.base = (void *) MIPI_FB_ADDR;

	if (panel_info->mode == MIPI_VIDEO_MODE)
		status += mipi_dsi_video_config(panel_info->num_of_lanes);

	if (panel_info->mode == MIPI_CMD_MODE)
		cmd_mode_status = 1;

	return &mipi_fb_cfg;
}
