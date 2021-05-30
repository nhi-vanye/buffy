/*
 * $Id: interface.c,v 1.1.1.1 1999/06/01 21:35:54 offer Exp $
 * 
 * Copyright (c) 1999 Richard Offer. All rights reserved. 
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
 * 
 */

/*
 * This engine was originally suggested by Richard Hess, who nagged me for
 * six months until I did it....
 * 
 */

#include <gtk/gtk.h>
#include <gmodule.h>

#include "buffy.h"

/* Theme functions to export */
void            theme_init(GtkThemeEngine * engine);
void            theme_exit(void);

/* Exported vtable from th_draw */

extern GtkStyleClass buffy_default_class;

/* internals */

static struct {
	gchar          *name;
	gint            token;
} themeSymbols[] = {

	{ "checkColor", TOKEN_CHECK_COLOR },
	{ "radioColor", TOKEN_RADIO_COLOR },
};

#define NUMBER(a) (sizeof((a))/sizeof( (a)[0] ))

static          guint
parseCheckColor(GScanner * scanner, BuffyThemeRc * theme)
{

	guint           token;
	GdkColor        col;

	token = g_scanner_get_next_token(scanner);
	if (token != TOKEN_CHECK_COLOR) {
		return TOKEN_CHECK_COLOR;
	}
	token = g_scanner_get_next_token(scanner);
	if (token != G_TOKEN_EQUAL_SIGN) {
		return G_TOKEN_EQUAL_SIGN;
	}
	return gtk_rc_parse_color(scanner, &theme->check_color);

}
static          guint
parseRadioColor(GScanner * scanner, BuffyThemeRc * theme)
{

	guint           token;
	GdkColor        col;

	token = g_scanner_get_next_token(scanner);
	if (token != TOKEN_RADIO_COLOR)
		return TOKEN_RADIO_COLOR;

	token = g_scanner_get_next_token(scanner);
	if (token != G_TOKEN_EQUAL_SIGN)
		return G_TOKEN_EQUAL_SIGN;

	gtk_rc_parse_color(scanner, &theme->radio_color);

	return G_TOKEN_NONE;

}

static          guint
theme_parse_rc_style(GScanner * scanner, GtkRcStyle * rc_style)
{

	guint           token;
	static GQuark   scope_id = 0;
	GQuark          old_scope;
	int             i;
	BuffyThemeRc   *themeRc;


	/* init'ize the additions to the symbol table */

	if (scope_id == 0)
		scope_id = g_quark_from_string("theme_engine");

	old_scope = g_scanner_set_scope(scanner, scope_id);

	if (!g_scanner_lookup_symbol(scanner, themeSymbols[0].name)) {

		g_scanner_freeze_symbol_table(scanner);

		for (i = 0; i < NUMBER(themeSymbols); i++) {

			g_scanner_scope_add_symbol(scanner, scope_id, themeSymbols[i].name,
				    GINT_TO_POINTER(themeSymbols[i].token));

		}

		g_scanner_thaw_symbol_table(scanner);

	}
	themeRc = g_new0(BuffyThemeRc, 1);
	themeRc->scrollbar_thickness = DEFAULT_SCROLLBARTHICKNESS;

	token = g_scanner_peek_next_token(scanner);
	while (token != G_TOKEN_RIGHT_CURLY) {
		switch (token) {

		case TOKEN_CHECK_COLOR:
			token = parseCheckColor(scanner, themeRc);
			break;

		case TOKEN_RADIO_COLOR:
			token = parseRadioColor(scanner, themeRc);
			break;

		default:
			g_scanner_get_next_token(scanner);
			token = G_TOKEN_RIGHT_CURLY;
			break;

		}

		if (token != G_TOKEN_NONE)
			return token;

		token = g_scanner_peek_next_token(scanner);

	}

	g_scanner_get_next_token(scanner);

	rc_style->engine_data = themeRc;

	g_scanner_set_scope(scanner, old_scope);

	return G_TOKEN_NONE;
}

static void
theme_merge_rc_style(GtkRcStyle * dest,
		     GtkRcStyle * src)
{

	BuffyThemeRc   *src_data = src->engine_data;
	BuffyThemeRc   *dest_data = dest->engine_data;

	if (src_data == NULL) {
		src_data = g_new0(BuffyThemeRc, 1);

		src->engine_data = src_data;

	}
	if (dest_data == NULL) {

		dest_data = g_new0(BuffyThemeRc, 1);

		dest->engine_data = dest_data;
	}
	dest_data->scrollbar_thickness = src_data->scrollbar_thickness;
	dest_data->check_color = src_data->check_color;
	dest_data->radio_color = src_data->radio_color;

}

static void
theme_rc_style_to_style(GtkStyle * style,
			GtkRcStyle * rc_style)
{

	BuffyThemeRc   *rcData = (BuffyThemeRc *) rc_style->engine_data;

	BuffyThemeData *styleData = g_new0(BuffyThemeData, 1);

	styleData->scrollbarThickness = rcData->scrollbar_thickness;
	styleData->checkColor = rcData->check_color;
	styleData->radioColor = rcData->radio_color;

	style->klass = &buffy_default_class;

	style->engine_data = styleData;

}

static void
theme_duplicate_style(GtkStyle * dest,
		      GtkStyle * src)
{
}

static void
theme_realize_style(GtkStyle * style)
{
}

static void
theme_unrealize_style(GtkStyle * style)
{
}

static void
theme_destroy_rc_style(GtkRcStyle * rc_style)
{
	if (rc_style->engine_data)
		g_free(rc_style->engine_data);
}

static void
theme_destroy_style(GtkStyle * style)
{
	if (style->engine_data)
		g_free(style->engine_data);
}

void
theme_init(GtkThemeEngine * engine)
{

	GtkRangeClass  *rangeClass;
	GtkCheckButtonClass *checkClass;
	GtkRadioButtonClass *radioClass;
	GtkCheckMenuItemClass *checkMenuItemClass;

	engine->parse_rc_style = theme_parse_rc_style;
	engine->merge_rc_style = theme_merge_rc_style;
	engine->rc_style_to_style = theme_rc_style_to_style;
	engine->duplicate_style = theme_duplicate_style;
	engine->realize_style = theme_realize_style;
	engine->unrealize_style = theme_unrealize_style;
	engine->destroy_rc_style = theme_destroy_rc_style;
	engine->destroy_style = theme_destroy_style;
	engine->set_background = NULL;

	rangeClass = (GtkRangeClass *) gtk_type_class(gtk_range_get_type());
	rangeClass->slider_width = DEFAULT_SCROLLBARTHICKNESS;
	rangeClass->stepper_size = DEFAULT_SCROLLBARTHICKNESS;
	rangeClass->min_slider_size = DEFAULT_SCROLLBARTHICKNESS;

	checkClass = (GtkCheckButtonClass *) gtk_type_class(gtk_check_button_get_type());
	checkClass->indicator_size = DEFAULT_CHECK_SIZE;

	radioClass = (GtkRadioButtonClass *) gtk_type_class(gtk_radio_button_get_type());
	radioClass->parent_class.indicator_size = DEFAULT_RADIO_SIZE;
	checkMenuItemClass = (GtkCheckMenuItemClass *) gtk_type_class(gtk_check_menu_item_get_type());
	checkMenuItemClass->parent_class.toggle_size = DEFAULT_CHECK_SIZE;
}

void
theme_exit(void)
{
}

/*
 * The following function will be called by GTK+ when the module is loaded
 * and checks to see if we are compatible with the version of GTK+ that loads
 * us.
 */
G_MODULE_EXPORT const gchar *g_module_check_init(GModule * module);
const gchar    *
g_module_check_init(GModule * module)
{
	return gtk_check_version(GTK_MAJOR_VERSION,
				 GTK_MINOR_VERSION,
				 GTK_MICRO_VERSION - GTK_INTERFACE_AGE);
}
