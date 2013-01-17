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

void fbcon_setup(struct fbcon_config *cfg);
void fbcon_putc(char c);
void fbcon_clear(void);
struct fbcon_config* fbcon_display(void);

#endif /* __DEV_FBCON_H */
