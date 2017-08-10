#include "..\Krait.h"
#include "mdp4.h"

void
mdp_setup_dma_p_video_config(unsigned short pack_pattern,
			     unsigned short img_width,
			     unsigned short img_height,
			     unsigned long input_img_addr,
			     unsigned short img_width_full_size,
			     unsigned char ystride)
{
	hal_printf("MDP4.2 Setup for DSI Video Mode\n");

	// ----- programming MDP_AXI_RDMASTER_CONFIG --------
	/* MDP_AXI_RDMASTER_CONFIG set all master to read from AXI port 0, that's
	   the only port connected */
	//TODO: Seems to still work without this
	writel(0x00290000, MDP_AXI_RDMASTER_CONFIG);
	writel(0x00000004, MDP_AXI_WRMASTER_CONFIG);
	writel(0x00007777, MDP_MAX_RD_PENDING_CMD_CONFIG);

	/* Set up CMD_INTF_SEL, VIDEO_INTF_SEL, EXT_INTF_SEL, SEC_INTF_SEL, PRIM_INTF_SEL */
	writel(0x00000049, MDP_DISP_INTF_SEL);

	/* DMA P */
	writel(0x0000000b, MDP_OVERLAYPROC0_CFG);

	/* RGB 888 */
	writel(pack_pattern << 8 | 0xbf | (0 << 25), MDP_DMA_P_CONFIG);

	writel(0x0, MDP_DMA_P_OUT_XY);

	writel(img_height << 16 | img_width, MDP_DMA_P_SIZE);

	writel(input_img_addr, MDP_DMA_P_BUF_ADDR);

	writel(img_width_full_size * ystride, MDP_DMA_P_BUF_Y_STRIDE);
}

void mdp_start_dma(void)
{
        writel(0x00000001, MDP_DMA_P_START);
}

