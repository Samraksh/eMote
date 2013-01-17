#include <tinyhal.h>
#include "fbcon.h"
#include "splash.h"
#include "font5x12.h"

struct pos {
	int x;
	int y;
};


static struct fbcon_config *config = NULL;

#define RGB565_BLACK		0x0000
#define RGB565_WHITE		0xffff

#define RGB888_BLACK            0x000000
#define RGB888_WHITE            0xffffff

#define FONT_WIDTH		5
#define FONT_HEIGHT		12

static UINT16			BGCOLOR;
static UINT16			FGCOLOR;

static struct pos		cur_pos;
static struct pos		max_pos;

static void fbcon_flush(void)
{
	if (config->update_start)
		config->update_start();
	if (config->update_done)
		while (!config->update_done());
}

static void fbcon_scroll_up(void)
{
	unsigned short *dst = (UINT16*) config->base;
	unsigned short *src = dst + (config->width * FONT_HEIGHT);
	unsigned count = config->width * (config->height - FONT_HEIGHT);

	while(count--) {
		*dst++ = *src++;
	}

	count = config->width * FONT_HEIGHT;
	while(count--) {
		*dst++ = BGCOLOR;
	}

	fbcon_flush();
}


static void fbcon_set_colors(unsigned bg, unsigned fg)
{
	BGCOLOR = bg;
	FGCOLOR = fg;
}

void fbcon_clear(void)
{
	unsigned count = config->width * config->height;
	memset(config->base, BGCOLOR, count * ((config->bpp) / 8));
}


void fbcon_setup(struct fbcon_config *_config)
{
	UINT32 bg;
	UINT32 fg;

	//ASSERT(_config);

	config = _config;

	switch (config->format) {
	case FB_FORMAT_RGB565:
		fg = RGB565_WHITE;
		bg = RGB565_BLACK;
		break;
        case FB_FORMAT_RGB888:
                fg = RGB888_WHITE;
                bg = RGB888_BLACK;
                break;
	default:
		hal_printf("unknown framebuffer pixel format\n");
		//ASSERT(0);
		while(1);
		break;
	}

	fbcon_set_colors(bg, fg);

	cur_pos.x = 0;
	cur_pos.y = 0;
	max_pos.x = config->width / (FONT_WIDTH+1);
	max_pos.y = (config->height - 1) / FONT_HEIGHT;
#if !DISPLAY_SPLASH_SCREEN
	fbcon_clear();
#endif
}



static void fbcon_drawglyph(UINT16 *pixels, UINT16 paint, unsigned stride,
			    unsigned *glyph)
{
	unsigned x, y, data;
	stride -= FONT_WIDTH;

	data = glyph[0];
	for (y = 0; y < (FONT_HEIGHT / 2); ++y) {
		for (x = 0; x < FONT_WIDTH; ++x) {
			if (data & 1)
				*pixels = paint;
			data >>= 1;
			pixels++;
		}
		pixels += stride;
	}

	data = glyph[1];
	for (y = 0; y < (FONT_HEIGHT / 2); y++) {
		for (x = 0; x < FONT_WIDTH; x++) {
			if (data & 1)
				*pixels = paint;
			data >>= 1;
			pixels++;
		}
		pixels += stride;
	}
}


void fbcon_putc(char c)
{
	UINT16 *pixels;

	/* ignore anything that happens before fbcon is initialized */
	if (!config)
		return;

	if((unsigned char)c > 127)
		return;
	if((unsigned char)c < 32) {
		if(c == '\n')
			goto newline;
		else if (c == '\r')
			cur_pos.x = 0;
		return;
	}

	pixels = (UINT16 *) config->base;
	pixels += cur_pos.y * FONT_HEIGHT * config->width;
	pixels += cur_pos.x * (FONT_WIDTH + 1);
	fbcon_drawglyph(pixels, FGCOLOR, config->stride,
			font5x12 + (c - 32) * 2);

	cur_pos.x++;
	if (cur_pos.x < max_pos.x)
		return;

newline:
	cur_pos.y++;
	cur_pos.x = 0;
	if(cur_pos.y >= max_pos.y) {
		cur_pos.y = max_pos.y - 1;
		fbcon_scroll_up();
	} else
		fbcon_flush();
}


void display_image_on_screen(void)
{
    unsigned i = 0;
    unsigned total_x = config->width;
    unsigned total_y = config->height;
    unsigned bytes_per_bpp = ((config->bpp) / 8);
    unsigned image_base = ((((total_y/2) - (SPLASH_IMAGE_WIDTH / 2) - 1) *
			    (config->width)) + (total_x/2 - (SPLASH_IMAGE_HEIGHT / 2)));
    fbcon_clear();

#if DISPLAY_TYPE_MIPI
    if (bytes_per_bpp == 3)
    {
        for (i = 0; i < SPLASH_IMAGE_WIDTH; i++)
        {
            memcpy (config->base + ((image_base + (i * (config->width))) * bytes_per_bpp),
		    imageBuffer_rgb888 + (i * SPLASH_IMAGE_HEIGHT * bytes_per_bpp),
		    SPLASH_IMAGE_HEIGHT * bytes_per_bpp);
	}
    }
    fbcon_flush();
#if DISPLAY_MIPI_PANEL_NOVATEK_BLUE
    if(is_cmd_mode_enabled())
        mipi_dsi_cmd_mode_trigger();
#endif

#else
    if (bytes_per_bpp == 2)
    {
        for (i = 0; i < SPLASH_IMAGE_WIDTH; i++)
        {
            memcpy (config->base + ((image_base + (i * (config->width))) * bytes_per_bpp),
		    imageBuffer + (i * SPLASH_IMAGE_HEIGHT * bytes_per_bpp),
		    SPLASH_IMAGE_HEIGHT * bytes_per_bpp);
	}
    }
    fbcon_flush();
#endif
}

