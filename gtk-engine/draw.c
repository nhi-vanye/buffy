/*
 * $Id: draw.c,v 1.2 1999/06/04 19:09:05 offer Exp $
 * 
 * Copyright (c) 1999 Richard Offer. All rights reserved 
 *                    <offer@sgi.com>
 *                    <richard@whitequeen.com>
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
 */
 
#include <math.h>
#include <gtk/gtk.h>

#include "buffy.h"


#define insens_stipple_width 2
#define insens_stipple_height 2
static unsigned char insens_stipple_bits[] = {
0x02, 0x01};


/* Takes the bg color and generates all the GCs needed for shadows */
static void     getShadowGCs(GdkWindow * win, GtkStyle * style, GtkStateType state);
/* set the clip area on all the internal GCs */
static void setClipArea(GdkRectangle *area); 




/* Wouldn't it be nice if :-
 * 
 * it was possible to combine engines, for example, this engine (like the real)
 * buffed Motif uses a lot of the Motif/Notif drawing code. This has to be
 * replicated here rather than simply calling the Notif entry points...
 */

GtkStyleClass   buffy_default_class =
{
	2,
	2,
	draw_hline,			/* NOTIF: ? */
	draw_vline,			/* NOTIF: ? */
	draw_shadow, 		/* draws the shadows around a given 'box' */
	draw_polygon, 		/* NOTIF: ? */
	draw_arrow, 		/* used in menus and scrollbars among others */
	draw_diamond, 		/* radio buttons (1-of-many) */
	/* draw_oval */ NULL,
	draw_string,
	draw_box, 			/* the generic routine */
	draw_flat_box,		/* NOTIF */
	draw_check,			/* toggle buttons (n-of-many) */
	draw_option,		/* NOTIF */
	/* draw_cross */ NULL,
	/* draw_ramp */ NULL,
	draw_tab,			/* used in the notebook */
	draw_shadow_gap,	/* draw a gap in the shadows (used in frame) */
	draw_box_gap,		/* NOTIF */
	draw_extension,		/* NOTIF: */
	draw_focus,			/* NOTIF: */
	draw_slider,		/* NOTIF: NOT used for scrollbars, only for scale */
	draw_handle			/* NOTIF: the toolbar tear off */
};

#define NGCS 6
GdkGC          *shadowGCs[] = {
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};


static void
draw_shadow(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, gchar * detail, gint x, gint y, gint width, gint height)
{

	gint            thick;
	gint            i;

	g_return_if_fail(style != NULL);
	g_return_if_fail(window != NULL);


	if ((width == -1) && (height == -1))
		gdk_window_get_size(window, &width, &height);
	else if (width == -1)
		gdk_window_get_size(window, &width, NULL);
	else if (height == -1)
		gdk_window_get_size(window, NULL, &height);

	getShadowGCs(window, style, state_type);


	if (area) {

		if ((shadow_type == GTK_SHADOW_IN) ||
		    (shadow_type == GTK_SHADOW_OUT)) {

			setClipArea(area);

		}
	}
	if ((GTK_IS_SCROLLBAR(widget) && DETAIL("trough")))
		thick = 2;
	else
		thick = 3;

	switch (shadow_type) {

	case GTK_SHADOW_NONE:
		break;
	case GTK_SHADOW_IN:
		/* lowlights */

		for (i = 0; i < thick; i++) {

			gdk_draw_line(window, shadowGCs[i + 3],
				      x + i, y + height - i, x + width - i, y + height - i);

			gdk_draw_line(window, shadowGCs[i + 3],
			x + width - i, y + height - i, x + width - i, y + i);

		}

		/* highlights */

		for (i = 0; i < thick; i++) {

			gdk_draw_line(window, shadowGCs[i],
				      x + i, y + height - i, x + i, y + i);

			gdk_draw_line(window, shadowGCs[i],
				      x + i, y + i, x + width - i, y + i);

		}


		break;


	case GTK_SHADOW_ETCHED_IN:
		gdk_draw_line(window, shadowGCs[0],
			      x, y + height, x, y);

		gdk_draw_line(window, shadowGCs[0],
			      x, y, x + width, y);

		gdk_draw_line(window, shadowGCs[4],
			      x + 1, y + height - 1, x + 1, y + 1);

		gdk_draw_line(window, shadowGCs[4],
			      x + 1, y + 1, x + width - 1, y + 1);

		gdk_draw_line(window, shadowGCs[0],
			      x, y + height, x + width, y + height);

		gdk_draw_line(window, shadowGCs[0],
			      x + width, y + height, x + width, y);

		gdk_draw_line(window, shadowGCs[4],
		      x + 1, y + height - 1, x + width - 1, y + height - 1);

		gdk_draw_line(window, shadowGCs[4],
		       x + width - 1, y + height - 1, x + width - 1, y + 1);

		break;

	case GTK_SHADOW_ETCHED_OUT:
		gdk_draw_line(window, shadowGCs[4],
			      x, y + height, x, y);

		gdk_draw_line(window, shadowGCs[4],
			      x, y, x + width, y);

		gdk_draw_line(window, shadowGCs[0],
			      x + 1, y + height - 1, x + 1, y + 1);

		gdk_draw_line(window, shadowGCs[0],
			      x + 1, y + 1, x + width - 1, y + 1);

		gdk_draw_line(window, shadowGCs[4],
			      x, y + height, x + width, y + height);

		gdk_draw_line(window, shadowGCs[4],
			      x + width, y + height, x + width, y);

		gdk_draw_line(window, shadowGCs[0],
		      x + 1, y + height - 1, x + width - 1, y + height - 1);

		gdk_draw_line(window, shadowGCs[0],
		       x + width - 1, y + height - 1, x + width - 1, y + 1);

		break;

	case GTK_SHADOW_OUT:

		/* highlights */

		for (i = 0; i < thick; i++) {

			gdk_draw_line(window, shadowGCs[i],
				      x + i, y + height - i, x + width - i, y + height - i);

			gdk_draw_line(window, shadowGCs[i],
			x + width - i, y + height - i, x + width - i, y + i);

		}

		/* lowlights */

		for (i = 0; i < thick; i++) {

			gdk_draw_line(window, shadowGCs[i + 3],
				      x + i, y + height - i, x + i, y + i);

			gdk_draw_line(window, shadowGCs[i + 3],
				      x + i, y + i, x + width - i, y + i);

		}


		break;
	}

	if (area) {

		if ((shadow_type == GTK_SHADOW_IN) || (shadow_type == GTK_SHADOW_OUT)) {

			setClipArea(NULL);

		}
	}
}


static void
draw_arrow(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, gchar * detail, GtkArrowType arrow_type, gint fill, gint x, gint y, gint width, gint height)
{

	gint half_width;
	gint half_height;
	GdkPoint points[3];
	int	margin=3;


	g_return_if_fail(style != NULL);
	g_return_if_fail(window != NULL);


	if ((width == -1) && (height == -1))
		gdk_window_get_size(window, &width, &height);
	else if (width == -1)
		gdk_window_get_size(window, &width, NULL);
	else if (height == -1)
		gdk_window_get_size(window, NULL, &height);


	half_width = (int) ( ( width / 2 ) +0.5);
	half_height = (int) ( ( height / 2 ) +0.5);


	getShadowGCs(window, style, state_type);


	if (GTK_IS_SCROLLBAR(widget)) {
		draw_box(style, window, state_type, GTK_SHADOW_OUT,
		 area, widget, detail, x - 1, y - 1, width + 2, height + 2);
	}
	
	if ( width < 15 || height < 15 )
		margin = 0;

	switch (arrow_type) {

	case GTK_ARROW_UP:

		if (fill) {
			points[0].x = x + half_width;
			points[0].y = y + margin;
			points[1].x = x + margin;
			points[1].y = y + height - margin;
			points[2].x = x + width - margin;
			points[2].y = y + height - margin;
		}
		break;

	case GTK_ARROW_DOWN:

		if (fill) {
			points[0].x = x + half_width;
			points[0].y = y + height - margin;
			points[1].x = x + margin;
			points[1].y = y + margin;
			points[2].x = x + width - margin;
			points[2].y = y + margin;
		}
		break;

	case GTK_ARROW_RIGHT:

		if (fill) {
			points[0].x = x + width - margin;
			points[0].y = y + half_height;
			points[1].x = x + margin;
			points[1].y = y + margin;
			points[2].x = x + margin;
			points[2].y = y + height - margin;
		}
		break;

	case GTK_ARROW_LEFT:

		if (fill) {
			points[0].x = x + margin;
			points[0].y = y + half_height;
			points[1].x = x + width - margin;
			points[1].y = y + margin;
			points[2].x = x + width - margin;
			points[2].y = y + height - margin;
		}
		break;
	}

	gdk_draw_polygon(window, shadowGCs[1], TRUE, points, 3);
}

static void
draw_diamond(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, gchar * detail, gint x, gint y, gint width, gint height)
{
	gint            half_width;
	gint            half_height;
	int             thick = 2;
	int             i;
	static GdkGC   *gc = NULL;
	BuffyThemeData *themeData = style->engine_data;

	g_return_if_fail(style != NULL);
	g_return_if_fail(window != NULL);

	if ((width == -1) && (height == -1))
		gdk_window_get_size(window, &width, &height);
	else if (width == -1)
		gdk_window_get_size(window, &width, NULL);
	else if (height == -1)
		gdk_window_get_size(window, NULL, &height);

	half_width = width / 2;
	half_height = height / 2;

	getShadowGCs(window, style, state_type);

	if (area) {
		setClipArea(area);
	}
	/* highlights */


	for (i = 0; i < thick; i++) {

		gdk_draw_line(window, shadowGCs[i + 3],
			      x + i, y + half_height,
			      x + half_width, y + i);
		gdk_draw_line(window, shadowGCs[i + 3],
			      x + half_width, y + i,
			      x + width - i, y + half_height);

	}

	/* lowlights */

	for (i = 0; i < thick; i++) {

		gdk_draw_line(window, shadowGCs[i],
			      x + i, y + half_height,
			      x + half_width, y + height - i);
		gdk_draw_line(window, shadowGCs[i],
			      x + half_width, y + height - i,
			      x + width - i, y + half_height);

	}

	if (shadow_type == GTK_SHADOW_IN) {

		GdkPoint        points[3];

		if (gc == NULL) {

			gc = gdk_gc_new(window);

		}
		if (themeData) {

			gdk_colormap_alloc_color(style->colormap,
					      &themeData->radioColor, 0, 1);
			gdk_gc_set_foreground(gc, &themeData->radioColor);

		}
		points[0].x = x + half_width;
		points[0].y = y + 2;
		points[1].x = x + half_width;
		points[1].y = y + half_height + 2;
		points[2].x = x + width - 4;
		points[2].y = y + (half_height / 2) + 2;

		gdk_draw_polygon(window, gc, TRUE, points, 3);

		gdk_draw_line(window, shadowGCs[2],
			      x + half_width + 1, y + half_height + 2,
			      x + width - 4, y + (half_height / 2) + 3);
	}

	if (area) {
		setClipArea(NULL);
	}
}

static void
draw_string(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GdkRectangle * area, GtkWidget * widget, gchar * detail, gint x, gint y, const gchar * string)
{

	g_return_if_fail(style != NULL);
	g_return_if_fail(window != NULL);


	if (state_type == GTK_STATE_INSENSITIVE) {

		GdkPixmap      *stipple = gdk_bitmap_create_from_data(window,
					      (gchar *) insens_stipple_bits,
						       insens_stipple_width,
						     insens_stipple_height);

		gdk_gc_set_stipple(style->fg_gc[state_type], stipple);
		gdk_gc_set_fill(style->fg_gc[state_type], GDK_STIPPLED);
	} else
		/* Don't ask... But it doesn't work otherwise. */
		gdk_gc_set_fill(style->fg_gc[state_type], GDK_SOLID);

	gdk_draw_string(window, style->font, style->fg_gc[state_type],
			x, y, string);

}

static void
draw_box(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, gchar * detail, gint x, gint y, gint width, gint height)
{
	g_return_if_fail(style != NULL);
	g_return_if_fail(window != NULL);

	if ((width == -1) && (height == -1))
		gdk_window_get_size(window, &width, &height);
	else if (width == -1)
		gdk_window_get_size(window, &width, NULL);
	else if (height == -1)
		gdk_window_get_size(window, NULL, &height);

	getShadowGCs(window, style, state_type);

	if ( area)
		setClipArea(area);

	if (!style->bg_pixmap[state_type]) {

		if (area)
			gdk_gc_set_clip_rectangle(style->bg_gc[state_type], area);

		gdk_draw_rectangle(window, style->bg_gc[state_type], TRUE, 
				x, y, width, height);

		if (area)
			gdk_gc_set_clip_rectangle(style->bg_gc[state_type], NULL);


	}
	if (DETAIL("slider")) {
		if (GTK_IS_VSCROLLBAR(widget))
			gtk_paint_shadow(style, window, state_type, shadow_type, area,
			       widget, detail, x - 1, y, width + 2, height);
		else
			gtk_paint_shadow(style, window, state_type, shadow_type, area,
			       widget, detail, x, y - 1, width, height + 2);
	} 
	else if (DETAIL("trough"))
		gtk_paint_shadow(style, window, state_type, GTK_SHADOW_OUT, area,
				 widget, detail, x, y, width, height);
	
	else if ( DETAIL("tab") ) {
		int pos = GTK_NOTEBOOK(widget)->tab_pos;
		int i;
		int thick = 3;
		int hack;	

		switch (pos) {
		
			case GTK_POS_TOP:

		/* lowighlights */

				for (i = 0; i < thick; i++) {

					if ( state_type == GTK_STATE_ACTIVE )
						hack = +i;
					else
						hack = -i;
							
					gdk_draw_line(window, shadowGCs[i],
						x + width - i, y + height + hack, 
						x + width - i, y + i);
				}
				
		/* highlights */		
				for (i = 0; i < thick; i++) {

					if ( state_type == GTK_STATE_ACTIVE )
						hack = +i;
					else
						hack = -i;

					gdk_draw_line(window, shadowGCs[i + 3],
				      	x + i, y + i, x + width - i, y + i);

					gdk_draw_line(window, shadowGCs[i + 3],
				      	x + i, y + height + hack, x + i, y + i);
				}
				
				break;
			
			case GTK_POS_BOTTOM:

		/* lowighlights */

				for (i = 0; i < thick; i++) {

					if ( state_type == GTK_STATE_ACTIVE )
						hack = +i;
					else
						hack = -i;
							
					gdk_draw_line(window, shadowGCs[i],
						x + width - i, y + height + hack, 
						x + width - i, y + i);

					gdk_draw_line(window, shadowGCs[i],
				      	x + i, y + height - i, x + width - i, y + height - i);
				}
				
		/* highlights */		
				for (i = 0; i < thick; i++) {

					if ( state_type == GTK_STATE_ACTIVE )
						hack = +i;
					else
						hack = -i;

					gdk_draw_line(window, shadowGCs[i + 3],
				      	x + i, y + height + hack, x + i, y + i);
				}
				
				break;
			
			case GTK_POS_RIGHT:

		/* lowighlights */

				for (i = 0; i < thick; i++) {

					if ( state_type == GTK_STATE_ACTIVE )
						hack = +i;
					else
						hack = -i;
							
					gdk_draw_line(window, shadowGCs[i],
				      	x + i, y +height - i, x + width - i, y + height -i);

					gdk_draw_line(window, shadowGCs[i],
				      	x + width - i, y + height + hack, x + width -i, y + i);

				}
				
		/* highlights */		
				for (i = 0; i < thick; i++) {

					if ( state_type == GTK_STATE_ACTIVE )
						hack = +i;
					else
						hack = -i;

					gdk_draw_line(window, shadowGCs[i + 3],
				      	x + i, y + i, x + width - i, y + i);

				}
				
				break;

			case GTK_POS_LEFT:

		/* lowighlights */

				for (i = 0; i < thick; i++) {

					if ( state_type == GTK_STATE_ACTIVE )
						hack = +i;
					else
						hack = -i;
							
					gdk_draw_line(window, shadowGCs[i],
				      	x + i, y + height - i, x + width - i, y + height - i);

				}
				
		/* highlights */		
				for (i = 0; i < thick; i++) {

					if ( state_type == GTK_STATE_ACTIVE )
						hack = +i;
					else
						hack = -i;

					gdk_draw_line(window, shadowGCs[i + 3],
				      	x + i, y + i, x + width - i, y + i);

					gdk_draw_line(window, shadowGCs[i + 3],
				      	x + i, y + height -i, x + i, y + i);
				}
				
				break;
			
			
		}
	}
	else if (DETAIL("bar")) {

		gdk_draw_rectangle(window, style->bg_gc[state_type], TRUE,
				   x, y, width, height);

		if (GTK_IS_PROGRESS(widget) &&
		    !GTK_PROGRESS(widget)->activity_mode &&
		    !GTK_PROGRESS(widget)->show_text) {

			GdkPoint        points[3];

			points[0].x = x + width;
			points[0].y = y;
			points[1].x = x + width;
			points[1].y = y + height;
			points[2].x = x + width + height;
			points[2].y = y + height;

			gdk_draw_polygon(window, style->bg_gc[state_type], TRUE, points, 3);
		}
	} 
	else
		gtk_paint_shadow(style, window, state_type, shadow_type, area,
				 widget, detail, x, y, width, height);

	if (DETAIL("slider")) {
		if (GTK_IS_VSCROLLBAR(widget)) {
			int             lines = 3;

			if (height <= 12)
				lines = 0;

			if (height <= 24)
				lines = 1;

			if (lines) {
				gdk_draw_line(window, style->black_gc,
					      x + 2, y + (height / 2),
					   x + width - 2, y + (height / 2));

				gdk_draw_line(window, style->white_gc,
					      x + 2, y + (height / 2) - 1,
				       x + width - 2, y + (height / 2) - 1);

				if (lines > 1) {

					gdk_draw_line(window, style->black_gc,
						x + 2, y + (height / 2) + 4,
					x + width - 2, y + (height / 2) + 4);

					gdk_draw_line(window, style->white_gc,
					    x + 2, y + (height / 2) - 1 + 4,
						      x + width - 2, y + (height / 2) - 1 + 4);

					gdk_draw_line(window, style->black_gc,
						x + 2, y + (height / 2) - 4,
					x + width - 2, y + (height / 2) - 4);

					gdk_draw_line(window, style->white_gc,
					    x + 2, y + (height / 2) - 1 - 4,
						      x + width - 2, y + (height / 2) - 1 - 4);
				}
			}
		} else if (GTK_IS_HSCROLLBAR(widget)) {
			int             lines = 3;

			if (width <= 12)
				lines = 0;

			if (width <= 24)
				lines = 1;

			if (lines) {
				gdk_draw_line(window, style->black_gc,
					      x + (width / 2), y + 2,
					   x + (width / 2), y + height - 2);

				gdk_draw_line(window, style->white_gc,
					      x + (width / 2) - 1, y + 2,
				       x + (width / 2) - 1, y + height - 2);

				if (lines > 1) {

					gdk_draw_line(window, style->black_gc,
						 x + (width / 2) + 4, y + 2,
					x + (width / 2) + 4, y + height - 2);

					gdk_draw_line(window, style->white_gc,
					     x + (width / 2) - 1 + 4, y + 2,
						      x + (width / 2) - 1 + 4, y + height - 2);

					gdk_draw_line(window, style->black_gc,
						 x + (width / 2) - 4, y + 2,
					x + (width / 2) - 4, y + height - 2);

					gdk_draw_line(window, style->white_gc,
					     x + (width / 2) - 1 - 4, y + 2,
						      x + (width / 2) - 1 - 4, y + height - 2);

				}
			}
		}
	}

	if ( area )
		setClipArea(NULL);
}



static void
draw_check(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, gchar * detail, gint x, gint y, gint width, gint height)
{

	static GdkGC   *gc = NULL;
	BuffyThemeData *themeData = style->engine_data;
	
	/* buffy always draw checks as SHADOW_OUT independent of state */
	gtk_paint_box(style, window, state_type,
		      GTK_SHADOW_OUT, area, widget, detail,
		      x, y, width, height);

	if (shadow_type == GTK_SHADOW_IN && window) {
		/* we're 'on', so draw the 'check' */
		GdkSegment      lines[10];

		lines[0].x1 = x + 3;
		lines[0].y1 = y + 5;
		lines[0].x2 = x + 4;
		lines[0].y2 = y + 5;
		lines[1].x1 = x + 2;
		lines[1].y1 = y + 6;
		lines[1].x2 = x + 5;
		lines[1].y2 = y + 6;
		lines[2].x1 = x + 3;
		lines[2].y1 = y + 7;
		lines[2].x2 = x + 7;
		lines[2].y2 = y + 7;
		lines[3].x1 = x + 4;
		lines[3].y1 = y + 8;
		lines[3].x2 = x + 7;
		lines[3].y2 = y + 8;
		lines[4].x1 = x + 5;
		lines[4].y1 = y + 9;
		lines[4].x2 = x + 6;
		lines[4].y2 = y + 9;
		lines[5].x1 = x + 6;
		lines[5].y1 = y + 10;
		lines[5].x2 = x + 6;
		lines[5].y2 = y + 10;
		lines[6].x1 = x + 7;
		lines[6].y1 = y + 6;
		lines[6].x2 = x + 8;
		lines[6].y2 = y + 6;
		lines[7].x1 = x + 7;
		lines[7].y1 = y + 5;
		lines[7].x2 = x + 9;
		lines[7].y2 = y + 5;
		lines[8].x1 = x + 8;
		lines[8].y1 = y + 4;
		lines[8].x2 = x + 11;
		lines[8].y2 = y + 4;
		lines[9].x1 = x + 10;
		lines[9].y1 = y + 3;
		lines[9].x2 = x + 14;
		lines[9].y2 = y + 3;


		if (gc == NULL) {
			gc = gdk_gc_new(window);

		}
		if (themeData) {

			gdk_colormap_alloc_color(style->colormap,
					      &themeData->checkColor, 0, 1);
			gdk_gc_set_foreground(gc, &themeData->checkColor);
		}
		gdk_draw_segments(window, gc, lines, 10);
	}
}

static void
draw_shadow_gap(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, gchar * detail, gint x, gint y, gint width, gint height, GtkPositionType gap_side, gint gap_x, gint gap_width)
{
	GdkRectangle    rect;
	BuffyThemeData *themeData = style->engine_data;
	static GdkGC *gc = NULL;
	
	if ( gc == NULL )
		gc = gdk_gc_new(window);

	g_return_if_fail(style != NULL);
	g_return_if_fail(window != NULL);

	gtk_paint_shadow(style, window, state_type, shadow_type, area, widget, 
			detail, x, y, width, height);

	switch (gap_side) {
	case GTK_POS_TOP:
		rect.x = x + gap_x;
		rect.y = y;
		rect.width = gap_width;
		rect.height = 2;
		break;
	case GTK_POS_BOTTOM:
		rect.x = x + gap_x;
		rect.y = y + height - 2;
		rect.width = gap_width;
		rect.height = 2;
		break;
	case GTK_POS_LEFT:
		rect.x = x;
		rect.y = y + gap_x;
		rect.width = 2;
		rect.height = gap_width;
		break;
	case GTK_POS_RIGHT:
		rect.x = x + width - 2;
		rect.y = y + gap_x;
		rect.width = 2;
		rect.height = gap_width;
		break;
	}

	gdk_gc_set_clip_rectangle(style->bg_gc[GTK_STATE_NORMAL], &rect);

	gdk_draw_rectangle(window,
			style->bg_gc[GTK_STATE_NORMAL],
			TRUE,
			rect.x, rect.y, rect.width, rect.height);

	gdk_gc_set_clip_rectangle(style->bg_gc[GTK_STATE_NORMAL], NULL);
}

static void
draw_box_gap(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, gchar * detail, gint x, gint y, gint width, gint height, GtkPositionType gap_side, gint gap_x, gint gap_width)
{
	GdkRectangle    rect;

	g_return_if_fail(style != NULL);
	g_return_if_fail(window != NULL);

	gtk_paint_box(style, window, state_type, shadow_type, area, widget, detail,
		      x, y, width, height);

	switch (gap_side) {
	case GTK_POS_TOP:
		rect.x = x + gap_x;
		rect.y = y;
		rect.width = gap_width ;
		rect.height = 3;
		break;
	case GTK_POS_BOTTOM:
		rect.x = x + gap_x;
		rect.y = y + height - 3;
		rect.width = gap_width ;
		rect.height = 3;
		break;
	case GTK_POS_LEFT:
		rect.x = x;
		rect.y = y + gap_x ;
		rect.width = 3;
		rect.height = gap_width ;
		break;
	case GTK_POS_RIGHT:
		rect.x = x + width ;
		rect.y = y + gap_x ;
		rect.width = 3;
		rect.height = gap_width ;
		break;
	}
	
	gdk_gc_set_clip_rectangle(style->bg_gc[GTK_STATE_NORMAL], &rect);
	gdk_draw_rectangle(window,
			style->bg_gc[GTK_STATE_NORMAL],
			TRUE,
			rect.x, rect.y, rect.width, rect.height);

	gdk_gc_set_clip_rectangle(style->bg_gc[GTK_STATE_NORMAL], NULL);
}



static void
setClipArea(GdkRectangle *area)
{
	int i=0;

	
	for (i = 0; i < NGCS; i++) {

		if ( shadowGCs[i] != NULL) {
			gdk_gc_set_clip_rectangle(shadowGCs[i], NULL);
		}
	}

}

static void
getShadowGCs(GdkWindow * win, GtkStyle * style, GtkStateType state)
{
	int             i;
	GdkColor        c;
	/* these factors are a rough guess by visual inspection */
	float           fac[5][6] = {
		{0.25, 0.5, 0.75, 0.5, 1.33, 1.5,},	/* NORMAL */
		{0.25, 0.5, 0.75, 0.5, 1.33, 1.5,},	/* ACTIVE TODO */
		{0.18, 0.37, 0.56, 0.18, 1.1, 1.1,},	/* PRELIGHT */
		{0.25, 0.5, 0.75, 0.5, 1.33, 1.5,},	/* SELECTED TODO */
		{0.25, 0.5, 0.75, 0.5, 1.33, 1.5,},	/* INSENSITIVE TODO */
	};

	for (i = 0; i < NGCS; i++) {

		if (shadowGCs[i] == NULL) {

			shadowGCs[i] = gdk_gc_new(win);
		}
		c.red = style->bg[state].red * fac[state][i];
		c.green = style->bg[state].green * fac[state][i];
		c.blue = style->bg[state].blue * fac[state][i];

		gdk_colormap_alloc_color(style->colormap, &c, 0, 1);
		gdk_gc_set_foreground(shadowGCs[i], &c);
	}
}
