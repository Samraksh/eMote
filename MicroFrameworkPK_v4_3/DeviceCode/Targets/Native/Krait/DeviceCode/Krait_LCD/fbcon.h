/*
 * 	Name 	: 	fbcon.h
 *
 *  Author  :   nived.sivadas@samraksh.com
 *
 *  Description : contains header for the lcd module
 */

#ifndef __DEV_FBCON_H
#define __DEV_FBCON_H

#define FB_FORMAT_RGB565 0
#define FB_FORMAT_RGB888 1

#define RGB565_BLACK		0x0000
#define RGB565_WHITE		0xffff

#define RGB888_BLACK            0x000000
#define RGB888_WHITE            0xffffff

#define FONT_WIDTH		5
#define FONT_HEIGHT		12

struct fbcon_config {
	void		*base;
	unsigned	width;
	unsigned	height;
	unsigned	stride;
	unsigned	bpp;
	unsigned	format;

	void		(*update_start)(void);
	int		(*update_done)(void);
};

//static void fbcon_flush(void);
void fbcon_setup(struct fbcon_config *cfg);
void fbcon_putc(char c);
void fbcon_clear(void);
struct fbcon_config* get_fbcon_config(void);

#endif /* __DEV_FBCON_H */
