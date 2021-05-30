/*
 * $Id: buffy.h,v 1.2 1999/06/04 19:09:04 offer Exp $
 * 
 * Copyright (c) 1999 Richard Offer. All rights reserved.
 *                    Home:  	<richard@whitequeen.com> 
 *                    Work: 	<offer@sgi.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL 
 * Richard Offer, Silicon Graphics Inc. BE LIABLE FOR ANY CLAIM, DAMAGES OR 
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR 
 * OTHER DEALINGS IN THE SOFTWARE.
 * 
 * 
 * Except as contained in this notice, the name of Richard Offer, or Silicon 
 * Graphics Inc. shall not be used in advertising or otherwise to promote the 
 * sale, use or other dealings in this Software without prior written 
 * authorization from both Richard Offer and Silicon Graphics Inc.
 * 

 * 
 */

#ifndef _THEME_BUFFY_H
#define _THEME_BUFFY_H

#define DEFAULT_SCROLLBARTHICKNESS 16
#define DEFAULT_CHECK_SIZE 16
#define DEFAULT_RADIO_SIZE 16

#define DETAIL(x) ( (detail) && (! strcmp(detail, x)) )

typedef struct _BuffyThemeRc {

	gint	scrollbar_thickness;
	GdkColor check_color;
	GdkColor radio_color;

} BuffyThemeRc;


typedef struct _BuffyThemeData {

	gint	scrollbarThickness;
	GdkColor checkColor;
	GdkColor radioColor;

} BuffyThemeData;

enum {
	TOKEN_SCROLLBARTHICKNESS = G_TOKEN_LAST + 1,
	TOKEN_CHECK_COLOR,
	TOKEN_RADIO_COLOR,
	
};

enum {
	SCROLLBAR_THICKNESS =0,
	CHECK_COLOR,
	RADIO_COLOR,
};


void     draw_hline(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GdkRectangle * area, GtkWidget * widget, gchar * detail, gint x1, gint x2, gint y);

void     draw_vline(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GdkRectangle * area, GtkWidget * widget, gchar * detail, gint y1, gint y2, gint x);

void     draw_shadow(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, gchar * detail, gint x, gint y, gint width, gint height);

void     draw_polygon(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, gchar * detail, GdkPoint * point, gint npoints, gint fill);

void     draw_arrow(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, gchar * detail, GtkArrowType arrow_type, gint fill, gint x, gint y, gint width, gint height);

void     draw_diamond(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, gchar * detail, gint x, gint y, gint width, gint height);


void     draw_string(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GdkRectangle * area, GtkWidget * widget, gchar * detail, gint x, gint y, const gchar * string);

void     draw_box(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, gchar * detail, gint x, gint y, gint width, gint height);

void     draw_flat_box(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, gchar * detail, gint x, gint y, gint width, gint height);

void     draw_check(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, gchar * detail, gint x, gint y, gint width, gint height);

void     draw_option(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, gchar * detail, gint x, gint y, gint width, gint height);


void     draw_tab(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, gchar * detail, gint x, gint y, gint width, gint height);

void     draw_shadow_gap(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, gchar * detail, gint x, gint y, gint width, gint height, GtkPositionType gap_side, gint gap_x, gint gap_width);

void     draw_box_gap(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, gchar * detail, gint x, gint y, gint width, gint height, GtkPositionType gap_side, gint gap_x, gint gap_width);

void     draw_extension(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, gchar * detail, gint x, gint y, gint width, gint height, GtkPositionType gap_side);

void     draw_focus(GtkStyle * style, GdkWindow * window, GdkRectangle * area, GtkWidget * widget, gchar * detail, gint x, gint y, gint width, gint height);

void     draw_slider(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, gchar * detail, gint x, gint y, gint width, gint height, GtkOrientation orientation);

void     draw_handle(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, gchar * detail, gint x, gint y, gint width, gint height, GtkOrientation orientation);


#endif /* _THEME_BUFFY_H */
