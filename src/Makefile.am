
AM_CPPFLAGS = @CPPFLAGS@ -Wall -Wextra -pedantic -D_FORTIFY_SOURCE=2 -ggdb

bin_PROGRAMS = global-trainer-gps

INCLUDES = -I$(top_srcdir)/include $(LIBUSB_CFLAGS)
global_trainer_gps_LDADD = $(LIBUSB_LIBS)

global_trainer_gps_SOURCES = main.cpp \
							 global-trainer-gps.cpp


