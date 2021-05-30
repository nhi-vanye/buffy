AUTOMAKE_OPTIONS = no-dependencies
INCLUDES = $(GTK_CFLAGS)
enginedir = $(libdir)/gtk/themes/engines

engine_LTLIBRARIES = libbuffy.la
libbuffy_la_SOURCES = draw.c interface.c notif.c
noinst_HEADERS = buffy.h
libbuffy_la_LDFLAGS = --module -avoid-version
