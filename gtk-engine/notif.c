/*
 * $Id: notif.c,v 1.1 1999/06/04 19:09:05 offer Exp $
 * 
 * 
 * This code is of unknown copyright, it was taken directly from the Notif
 * theme engine, which is likely to be GPL, but there is no indication in 
 * the source files...
 * 
 * Go Figure.
 * 
 */
 
#include <math.h>
#include <gtk/gtk.h>

#include "buffy.h"

/* Wouldn't it be nice if :-
 * 
 * it was possible to combine engines, for example, this engine (like the real)
 * buffed Motif uses a lot of the Motif/Notif drawing code. This has to be
 * replicated here rather than simply calling the Notif entry points...
 */


void
draw_hline(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GdkRectangle * area, GtkWidget * widget, gchar * detail, gint x1, gint x2, gint y)
{
	gint            thickness_light;
	gint            thickness_dark;
	gint            i;

	g_return_if_fail(style != NULL);
	g_return_if_fail(window != NULL);

	thickness_light = style->klass->ythickness / 2;
	thickness_dark = style->klass->ythickness - thickness_light;

	if (area) {
		gdk_gc_set_clip_rectangle(style->light_gc[state_type], area);
		gdk_gc_set_clip_rectangle(style->dark_gc[state_type], area);
	}
	for (i = 0; i < thickness_dark; i++) {
		gdk_draw_line(window, style->light_gc[state_type], x2 - i - 1, y + i, x2, y + i);
		gdk_draw_line(window, style->dark_gc[state_type], x1, y + i, x2 - i - 1, y + i);
	}

	y += thickness_dark;
	for (i = 0; i < thickness_light; i++) {
		gdk_draw_line(window, style->dark_gc[state_type], x1, y + i, x1 + thickness_light - i - 1, y + i);
		gdk_draw_line(window, style->light_gc[state_type], x1 + thickness_light - i - 1, y + i, x2, y + i);
	}
	if (area) {
		gdk_gc_set_clip_rectangle(style->light_gc[state_type], NULL);
		gdk_gc_set_clip_rectangle(style->dark_gc[state_type], NULL);
	}
}

void
draw_vline(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GdkRectangle * area, GtkWidget * widget, gchar * detail, gint y1, gint y2, gint x)
{
	gint            thickness_light;
	gint            thickness_dark;
	gint            i;

	g_return_if_fail(style != NULL);
	g_return_if_fail(window != NULL);

	thickness_light = style->klass->xthickness / 2;
	thickness_dark = style->klass->xthickness - thickness_light;

	if (area) {
		gdk_gc_set_clip_rectangle(style->light_gc[state_type], area);
		gdk_gc_set_clip_rectangle(style->dark_gc[state_type], area);
	}
	for (i = 0; i < thickness_dark; i++) {
		gdk_draw_line(window, style->light_gc[state_type], x + i, y2 - i - 1, x + i, y2);
		gdk_draw_line(window, style->dark_gc[state_type], x + i, y1, x + i, y2 - i - 1);
	}

	x += thickness_dark;
	for (i = 0; i < thickness_light; i++) {
		gdk_draw_line(window, style->dark_gc[state_type], x + i, y1, x + i, y1 + thickness_light - i);
		gdk_draw_line(window, style->light_gc[state_type], x + i, y1 + thickness_light - i, x + i, y2);
	}
	if (area) {
		gdk_gc_set_clip_rectangle(style->light_gc[state_type], NULL);
		gdk_gc_set_clip_rectangle(style->dark_gc[state_type], NULL);
	}
}


void
draw_polygon(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, gchar * detail, GdkPoint * points, gint npoints, gint fill)
{
#ifndef M_PI
#define M_PI    3.14159265358979323846
#endif				/* M_PI */
#ifndef M_PI_4
#define M_PI_4  0.78539816339744830962
#endif				/* M_PI_4 */

	static const gdouble pi_over_4 = M_PI_4;
	static const gdouble pi_3_over_4 = M_PI_4 * 3;

	GdkGC          *gc1;
	GdkGC          *gc2;
	GdkGC          *gc3;
	GdkGC          *gc4;
	gdouble         angle;
	gint            xadjust;
	gint            yadjust;
	gint            i;

	g_return_if_fail(style != NULL);
	g_return_if_fail(window != NULL);
	g_return_if_fail(points != NULL);
	
	switch (shadow_type) {
	case GTK_SHADOW_IN:
		gc1 = style->light_gc[state_type];
		gc2 = style->dark_gc[state_type];
		gc3 = style->light_gc[state_type];
		gc4 = style->dark_gc[state_type];
		break;
	case GTK_SHADOW_ETCHED_IN:
		gc1 = style->light_gc[state_type];
		gc2 = style->dark_gc[state_type];
		gc3 = style->dark_gc[state_type];
		gc4 = style->light_gc[state_type];
		break;
	case GTK_SHADOW_OUT:
		gc1 = style->dark_gc[state_type];
		gc2 = style->light_gc[state_type];
		gc3 = style->dark_gc[state_type];
		gc4 = style->light_gc[state_type];
		break;
	case GTK_SHADOW_ETCHED_OUT:
		gc1 = style->dark_gc[state_type];
		gc2 = style->light_gc[state_type];
		gc3 = style->light_gc[state_type];
		gc4 = style->dark_gc[state_type];
		break;
	default:
		return;
	}

	if (area) {
		gdk_gc_set_clip_rectangle(gc1, area);
		gdk_gc_set_clip_rectangle(gc2, area);
		gdk_gc_set_clip_rectangle(gc3, area);
		gdk_gc_set_clip_rectangle(gc4, area);
	}
	if (fill)
		gdk_draw_polygon(window, style->bg_gc[state_type], TRUE, points, npoints);

	npoints--;

	for (i = 0; i < npoints; i++) {
		if ((points[i].x == points[i + 1].x) &&
		    (points[i].y == points[i + 1].y)) {
			angle = 0;
		} else {
			angle = atan2(points[i + 1].y - points[i].y,
				      points[i + 1].x - points[i].x);
		}

		if ((angle > -pi_3_over_4) && (angle < pi_over_4)) {
			if (angle > -pi_over_4) {
				xadjust = 0;
				yadjust = 1;
			} else {
				xadjust = 1;
				yadjust = 0;
			}

			gdk_draw_line(window, gc1,
			       points[i].x - xadjust, points[i].y - yadjust,
				      points[i + 1].x - xadjust, points[i + 1].y - yadjust);
			gdk_draw_line(window, gc3,
				      points[i].x, points[i].y,
				      points[i + 1].x, points[i + 1].y);
		} else {
			if ((angle < -pi_3_over_4) || (angle > pi_3_over_4)) {
				xadjust = 0;
				yadjust = 1;
			} else {
				xadjust = 1;
				yadjust = 0;
			}

			gdk_draw_line(window, gc4,
			       points[i].x + xadjust, points[i].y + yadjust,
				      points[i + 1].x + xadjust, points[i + 1].y + yadjust);
			gdk_draw_line(window, gc2,
				      points[i].x, points[i].y,
				      points[i + 1].x, points[i + 1].y);
		}
	}
	if (area) {
		gdk_gc_set_clip_rectangle(gc1, NULL);
		gdk_gc_set_clip_rectangle(gc2, NULL);
		gdk_gc_set_clip_rectangle(gc3, NULL);
		gdk_gc_set_clip_rectangle(gc4, NULL);
	}
}



void
draw_flat_box(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, gchar * detail, gint x, gint y, gint width, gint height)
{
	GdkGC          *gc1;

	g_return_if_fail(style != NULL);
	g_return_if_fail(window != NULL);

	if ((width == -1) && (height == -1))
		gdk_window_get_size(window, &width, &height);
	else if (width == -1)
		gdk_window_get_size(window, &width, NULL);
	else if (height == -1)
		gdk_window_get_size(window, NULL, &height);

	gc1 = style->bg_gc[state_type];

	if ( DETAIL("text")  && 
			(state_type == GTK_STATE_SELECTED))
		gc1 = style->bg_gc[GTK_STATE_SELECTED];
	else if ( DETAIL("viewportbin") )
		gc1 = style->bg_gc[GTK_STATE_NORMAL];

	if ((!style->bg_pixmap[state_type]) || 
			(gc1 != style->bg_gc[state_type]) || 
			(gdk_window_get_type(window) == GDK_WINDOW_PIXMAP)) {

		if (area) {
			gdk_gc_set_clip_rectangle(gc1, area);
		}

		gdk_draw_rectangle(window, gc1, TRUE,
				   x, y, width, height);

		if ( DETAIL("tooltip") )
			gdk_draw_rectangle(window, style->black_gc, FALSE,
					   x, y, width - 1, height - 1);

		if (area) {
			gdk_gc_set_clip_rectangle(gc1, NULL);
		}
	} else
		gtk_style_apply_default_pixmap(style, window, state_type, area, 
				x, y, width, height);
}


void
draw_option(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, gchar * detail, gint x, gint y, gint width, gint height)
{
	gtk_paint_diamond(style, window, state_type, shadow_type, area, widget,
			  detail, x, y, width, height);
}



void
draw_tab(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, gchar * detail, gint x, gint y, gint width, gint height)
{
	g_return_if_fail(style != NULL);
	g_return_if_fail(window != NULL);
	
	gtk_paint_box(style, window, state_type, shadow_type, area, widget, detail,
		      x, y, width, height);
}


void
draw_extension(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, gchar * detail, gint x, gint y, gint width, gint height, GtkPositionType gap_side)
{
	GdkRectangle    rect;

	g_return_if_fail(style != NULL);
	g_return_if_fail(window != NULL);

	gtk_paint_box(style, window, state_type, shadow_type, area, widget, detail,
		      x, y, width, height);

	switch (gap_side) {
	case GTK_POS_TOP:
		rect.x = x + style->klass->xthickness;
		rect.y = y;
		rect.width = width - style->klass->xthickness * 2;
		rect.height = style->klass->ythickness;
		break;
	case GTK_POS_BOTTOM:
		rect.x = x + style->klass->xthickness;
		rect.y = y + height - style->klass->ythickness;
		rect.width = width - style->klass->xthickness * 2;
		rect.height = style->klass->ythickness;
		break;
	case GTK_POS_LEFT:
		rect.x = x;
		rect.y = y + style->klass->ythickness;
		rect.width = style->klass->xthickness;
		rect.height = height - style->klass->ythickness * 2;
		break;
	case GTK_POS_RIGHT:
		rect.x = x + width - style->klass->xthickness;
		rect.y = y + style->klass->ythickness;
		rect.width = style->klass->xthickness;
		rect.height = height - style->klass->ythickness * 2;
		break;
	}

	gtk_style_apply_default_pixmap(style, window, state_type, area,
				   rect.x, rect.y, rect.width, rect.height);
}

void
draw_focus(GtkStyle * style, GdkWindow * window, GdkRectangle * area, GtkWidget * widget, gchar * detail, gint x, gint y, gint width, gint height)
{
	g_return_if_fail(style != NULL);
	g_return_if_fail(window != NULL);

	if ((width == -1) && (height == -1)) {
		gdk_window_get_size(window, &width, &height);
		width -= 1;
		height -= 1;
	} else if (width == -1) {
		gdk_window_get_size(window, &width, NULL);
		width -= 1;
	} else if (height == -1) {
		gdk_window_get_size(window, NULL, &height);
		height -= 1;
	}
	if (area) {
		gdk_gc_set_clip_rectangle(style->black_gc, area);
	}

	gdk_draw_rectangle(window, style->black_gc, FALSE, x, y, width, height);

	if (area) {
		gdk_gc_set_clip_rectangle(style->black_gc, NULL);
	}
}

void
draw_slider(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, gchar * detail, gint x, gint y, gint width, gint height, GtkOrientation orientation)
{
	g_return_if_fail(style != NULL);
	g_return_if_fail(window != NULL);

	if ((width == -1) && (height == -1))
		gdk_window_get_size(window, &width, &height);
	else if (width == -1)
		gdk_window_get_size(window, &width, NULL);
	else if (height == -1)
		gdk_window_get_size(window, NULL, &height);

	gtk_draw_box(style, window, state_type, shadow_type, x, y,
		     width, height);

	if (orientation == GTK_ORIENTATION_HORIZONTAL)
		draw_vline(style, window, state_type, area, widget, detail,
			   style->klass->ythickness,
			   height - style->klass->ythickness - 1, width / 2);
	else
		draw_hline(style, window, state_type, area, widget, detail,
			   style->klass->xthickness,
			   width - style->klass->xthickness - 1, height / 2);

}

void
draw_handle(GtkStyle * style, GdkWindow * window, GtkStateType state_type, GtkShadowType shadow_type, GdkRectangle * area, GtkWidget * widget, gchar * detail, gint x, gint y, gint width, gint height, GtkOrientation orientation)
{
	gint            xx, yy;
	gint            xthick, ythick;
	GdkGC          *light_gc, *dark_gc;
	GdkRectangle    dest;

	g_return_if_fail(style != NULL);
	g_return_if_fail(window != NULL);

	if ((width == -1) && (height == -1))
		gdk_window_get_size(window, &width, &height);
	else if (width == -1)
		gdk_window_get_size(window, &width, NULL);
	else if (height == -1)
		gdk_window_get_size(window, NULL, &height);

	gtk_paint_box(style, window, state_type, shadow_type, area, widget,
		      detail, x, y, width, height);

	light_gc = style->light_gc[state_type];
	dark_gc = style->dark_gc[state_type];

	xthick = style->klass->xthickness;
	ythick = style->klass->ythickness;

	dest.x = x + xthick;
	dest.y = y + ythick;
	dest.width = width - (xthick * 2);
	dest.height = height - (ythick * 2);

	gdk_gc_set_clip_rectangle(light_gc, &dest);
	gdk_gc_set_clip_rectangle(dark_gc, &dest);

	for (yy = y + ythick; yy < (y + height - ythick); yy += 3)
		for (xx = x + xthick; xx < (x + width - xthick); xx += 6) {
			gdk_draw_point(window, light_gc, xx, yy);
			gdk_draw_point(window, dark_gc, xx + 1, yy + 1);

			gdk_draw_point(window, light_gc, xx + 3, yy + 1);
			gdk_draw_point(window, dark_gc, xx + 4, yy + 2);
		}
	gdk_gc_set_clip_rectangle(light_gc, NULL);
	gdk_gc_set_clip_rectangle(dark_gc, NULL);
}


