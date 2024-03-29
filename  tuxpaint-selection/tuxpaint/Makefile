
# $Id: Makefile,v 1.175 2008/04/03 16:48:50 wkendrick Exp $

# Tux Paint - A simple drawing program for children.

# Copyright (c) 2002-2008 by Bill Kendrick and others
# bill@newbreedsoftware.com
# http://www.tuxpaint.org/

# June 14, 2002 - April 3, 2008


# The version number, for release:

VER_VERSION=0.9.20
VER_DATE=`date +"%Y-%m-%d"`

MAGIC_API_VERSION=0x00000001
SO_TYPE=so

# Where to install things:

PREFIX=/usr/local


# Root directory to place files when creating packages.

PKG_ROOT=


# Program:

BIN_PREFIX=$(PKG_ROOT)$(PREFIX)/bin
EXE_EXT=


# Data:

DATA_PREFIX=$(PKG_ROOT)$(PREFIX)/share/tuxpaint


# Magic Tool plug-ins

INCLUDE_PREFIX=$(PKG_ROOT)$(PREFIX)/include
MAGIC_PREFIX=$(PKG_ROOT)$(PREFIX)/lib/tuxpaint/plugins


# Docs and man page:

DOC_PREFIX=$(PKG_ROOT)$(PREFIX)/share/doc/tuxpaint
DEVDOC_PREFIX=$(PKG_ROOT)$(PREFIX)/share/doc/tuxpaint-dev
MAN_PREFIX=$(PKG_ROOT)$(PREFIX)/share/man
DEVMAN_PREFIX=$(PKG_ROOT)$(PREFIX)/share/man


# 'System-wide' Config file:

ifeq ($(PREFIX),/usr)
  CONFDIR=$(PKG_ROOT)/etc/tuxpaint
else
  CONFDIR=$(PKG_ROOT)$(PREFIX)/etc/tuxpaint
endif


# Commands useful to other arch's (e.g., BeOS)

RSRC_CMD=echo -n
MIMESET_CMD=echo -n


# Icons and launchers:

ICON_PREFIX=$(PKG_ROOT)$(PREFIX)/share/pixmaps
X11_ICON_PREFIX=$(PKG_ROOT)$(PREFIX)/X11R6/include/X11/pixmaps
GNOME_PREFIX=`gnome-config --prefix 2> /dev/null`
KDE_PREFIX=`kde-config --install apps --expandvars 2> /dev/null`
KDE_ICON_PREFIX=`kde-config --install icon --expandvars 2> /dev/null`


# Built with sound by default  (override with "make nosound")

NOSOUNDFLAG=__SOUND


# Built with SVG support (via Cairo) by default  (override with "make nosvg")

NOSVGFLAG=__SVG


# Built with SDL Pango support by default  (override with "make nopango")

NOPANGOFLAG=___SDLPANGO


# Built with libcairo2 support by default  (use libcairo1 with "make oldsvg")

OLDSVGFLAG=__SVG


# Maemo flag

MAEMOFLAG=NO_MAEMOFLAG


# Where to find cursor shape XBMs

MOUSEDIR=mouse
CURSOR_SHAPES=LARGE

# MOUSEDIR=mouse/16x16
# CURSOR_SHAPES=SMALL


# Libraries, paths, and flags:

SDL_LIBS=$(shell sdl-config --libs) -lSDL_image -lSDL_ttf $(SDL_MIXER_LIB) $(SDL_PANGO_LIB)
SDL_MIXER_LIB=-lSDL_mixer
SDL_PANGO_LIB=-lSDL_Pango
SDL_CFLAGS=$(shell sdl-config --cflags) $(SVG_CFLAGS)


SVG_LIB=-lrsvg-2 -lcairo
SVG_CFLAGS=-I/usr/include/librsvg-2/librsvg \
	-I/usr/include/gtk-2.0 \
	-I/usr/include/glib-2.0 \
	-I/usr/lib/glib-2.0/include \
	-I/usr/include/cairo

PAPER_LIB=-lpaper
ARCH_LIBS=obj/postscript_print.o


# The entire set of CFLAGS:

#-ffast-math
OPTFLAGS=-O2
CFLAGS=$(OPTFLAGS) -W -Wall -fno-common -ffloat-store \
	-Wcast-align -Wredundant-decls \
	-Wbad-function-cast -Wwrite-strings \
	-Waggregate-return \
	-Wstrict-prototypes -Wmissing-prototypes \
	`src/test-option.sh -Wdeclaration-after-statement`

DEFS=-DDATA_PREFIX=\"$(DATA_PREFIX)/\" \
	-D$(NOSOUNDFLAG) -D$(NOSVGFLAG) -D$(OLDSVGFLAG) \
	-D$(NOPANGOFLAG) \
	-DDOC_PREFIX=\"$(DOC_PREFIX)/\" \
	-DLOCALEDIR=\"$(LOCALE_PREFIX)/\" -DIMDIR=\"$(IM_PREFIX)/\" \
	-DCONFDIR=\"$(CONFDIR)/\" \
	-DMAGIC_PREFIX=\"$(MAGIC_PREFIX)/\" \
	-DVER_VERSION=\"$(VER_VERSION)\" \
	-DVER_DATE=\"$(VER_DATE)\" \
	-D$(MAEMOFLAG)

DEBUG_FLAGS=
#DEBUG_FLAGS=-g

MOUSE_CFLAGS=-Isrc/$(MOUSEDIR) -D$(CURSOR_SHAPES)_CURSOR_SHAPES


# "make" with no arguments builds the program and man page from sources:

all:	tuxpaint translations magic-plugins tp-magic-config
	@echo
	@echo "--------------------------------------------------------------"
	@echo
	@echo "Done compiling."
	@echo
	@echo "Now run 'make install' with any options you ran 'make' with."
	@echo "to install Tux Paint."
	@echo
	@echo "You may need superuser ('root') privileges, depending on"
	@echo "where you're installing."
	@echo "(Depending on your system, you either need to 'su' first,"
	@echo "or run 'sudo make install'.)"
	@echo

releaseclean:
	@echo
	@echo "Cleaning release directory"
	@echo
	@rm -rf "build/tuxpaint-$(VER_VERSION)" "build/tuxpaint-$(VER_VERSION).tar.gz"
	@-if [ -d build ] ; then rmdir build ; fi

releasedir: build/tuxpaint-$(VER_VERSION)

build/tuxpaint-$(VER_VERSION):
	@echo
	@echo "Creating release directory"
	@echo
	@mkdir -p build/tuxpaint-$(VER_VERSION)
	@find . -follow \
	     \( -wholename '*/CVS' -o -name .cvsignore -o -name 'build' -o -name '.#*' \) \
	     -prune -o -type f -exec cp --parents -vdp \{\} build/tuxpaint-$(VER_VERSION)/ \;

release: releasedir
	@echo
	@echo "Creating release tarball"
	@echo
	@cd build ; \
	    tar -czvf tuxpaint-$(VER_VERSION).tar.gz tuxpaint-$(VER_VERSION)


# "make nosound" builds the program with sound disabled:

nosound:
	@echo
	@echo "Building with sound DISABLED"
	@echo
	make SDL_MIXER_LIB= NOSOUNDFLAG=NOSOUND


# "make nosvg" builds the program with SVG (Cairo2) support disabled:

nosvg:
	@echo
	@echo "Building with SVG DISABLED"
	@echo
	make SVG_LIB= SVG_CFLAGS= NOSVGFLAG=NOSVG


# "make nopango" builds the program with Pango support disabled:

nopango:
	@echo
	@echo "Building with Pango DISABLED"
	@echo
	make NOPANGOFLAG=NO_SDLPANGO SDL_PANGO_LIB=


# "make oldsvg" builds the program using older SVG (Cairo1) libraries:

oldsvg:
	@echo
	@echo "Building with CAIRO1 SVG SUPPORT"
	@echo
	make SVG_LIB="-lcairo -lsvg -lsvg-cairo" OLDSVGFLAG=OLD_SVG


# "make olpc" builds the program for an OLPC XO:

olpc:
	@echo
	@echo "Building for an OLPC XO"
	@echo
	make PREFIX=. SVG_LIB= SVG_CFLAGS= NOSVGFLAG=NOSVG OPTFLAGS='-O2 -fno-tree-pre -march=athlon -mtune=generic -mpreferred-stack-boundary=2 -mmmx -m3dnow -fomit-frame-pointer -falign-functions=0 -falign-jumps=0 -DOLPC_XO -DSUGAR'


# "make beos" builds the program for BeOS

beos:
	make \
		PREFIX=./ \
		BIN_PREFIX=./ \
		DATA_PREFIX=./data \
		DOC_PREFIX=./docs \
		MAN_PREFIX=./share/man \
		CONFDIR=./share/conf \
		ICON_PREFIX=. \
		X11_ICON_PREFIX=. \
		LOCALE_PREFIX=./share/locale \
		NOSVGFLAG=NOSVG \
		NOPANGOFLAG=NO_SDLPANGO \
		SDL_PANGO_LIB= \
		SVG_LIB= \
		PAPER_LIB= \
		IM_PREFIX=./src \
		SDL_CFLAGS="$(shell sdl-config --cflags)" \
		SDL_LIBS="$(shell sdl-config --libs) -lSDL -lSDL_image -lSDL_ttf $(SDL_MIXER_LIB)" \
		SDL_MIXER_LIB=-lSDL_mixer \
		CFLAGS="-O1 -funroll-loops -fomit-frame-pointer -pipe -Wall" \
		RSRC_CMD="xres -o tuxpaint src/tuxpaint.rsrc" \
		MIMESET_CMD="mimeset -f tuxpaint" \
		ARCH_LINKS="-lintl -lpng -lz -lbe -liconv" \
		ARCH_HEADERS="src/BeOS_print.h" \
		ARCH_LIBS="obj/BeOS_print.o" \
		MAGIC_PREFIX=./lib/tuxpaint/plugins \
		TARGET_PASSTHRU=beos

# "make win32" builds the program for Windows 2K/XP/Vista using MinGW/MSYS.
# The DATA_, DOC_ and LOCALE_ prefixes are absolute paths.
# Suitable for development/testing in the MinGW/MSYS environment.
win32:
	make \
		PREFIX=/usr/local \
		BIN_PREFIX=$(PREFIX)/bin \
		EXE_EXT=.exe \
		SO_TYPE=dll \
		DATA_PREFIX=$(PREFIX)/share/tuxpaint \
		DOC_PREFIX=$(PREFIX)/share/doc/tuxpaint \
		MAN_PREFIX=$(PREFIX)/share/man \
		DEVDOC_PREFIX=$(PREFIX)/share/doc/tuxpaint-dev \
		DEVMAN_PREFIX=$(PREFIX)/share/man \
		ICON_PREFIX=. \
		X11_ICON_PREFIX=. \
		LOCALE_PREFIX=$(PREFIX)/share/locale \
		IM_PREFIX=$(PREFIX)/share/tuxpaint/im \
		CONFDIR=$(PREFIX)/etc/tuxpaint \
		ARCH_LINKS="-lintl -lpng12 -lwinspool -lshlwapi" \
		ARCH_HEADERS="src/win32_print.h" \
		ARCH_LIBS="obj/win32_print.o obj/resource.o" \
		SVG_LIB="-lrsvg-2 -lcairo -lgobject-2.0" \
		SVG_CFLAGS="-I/usr/local/include/librsvg-2/librsvg -I/usr/local/include/gtk-2.0 -I/usr/local/include/glib-2.0 -I/usr/local/lib/glib-2.0/include -I/usr/local/include/cairo" \
		INCLUDE_PREFIX=$(PREFIX)/include \
		MAGIC_PREFIX=$(PREFIX)/lib/tuxpaint/plugins \
		TARGET_PASSTHRU=win32 \
		PAPER_LIB=

# "make win9x" builds the program for Windows 9x/ME using MinGW/MSYS.
# The DATA_, DOC_ and LOCALE_ prefixes are absolute paths.
# Suitable for development/testing in the MinGW/MSYS environment.
win9x:
	make \
		PREFIX=/usr/local \
		BIN_PREFIX=$(PREFIX)/bin \
		EXE_EXT=.exe \
		SO_TYPE=dll \
		DATA_PREFIX=$(PREFIX)/share/tuxpaint \
		DOC_PREFIX=$(PREFIX)/share/doc/tuxpaint \
		MAN_PREFIX=$(PREFIX)/share/man \
		DEVDOC_PREFIX=$(PREFIX)/share/doc/tuxpaint-dev \
		DEVMAN_PREFIX=$(PREFIX)/share/man \
		ICON_PREFIX=. \
		X11_ICON_PREFIX=. \
		LOCALE_PREFIX=$(PREFIX)/share/locale \
		IM_PREFIX=$(PREFIX)/share/tuxpaint/im \
		CONFDIR=$(PREFIX)/etc/tuxpaint \
		ARCH_LINKS="-lintl -lpng12 -lwinspool -lshlwapi" \
		ARCH_HEADERS="src/win32_print.h" \
		ARCH_LIBS="obj/win32_print.o obj/resource.o" \
		SVG_CFLAGS=-I/usr/local/include/cairo \
		SVG_LIB="-lcairo -lsvg -lsvg-cairo" \
		OLDSVGFLAG=OLD_SVG \
		NOPANGOFLAG=NO_SDLPANGO SDL_PANGO_LIB= \
		INCLUDE_PREFIX=$(PREFIX)/include \
		MAGIC_PREFIX=$(PREFIX)/lib/tuxpaint/plugins \
		TARGET_PASSTHRU=win32 \
		PAPER_LIB=

# "make nokia770" builds the program for the Nokia 770.

nokia770:
	make \
		DATA_PREFIX=/usr/share/tuxpaint \
		SVG_LIB= SVG_CFLAGS= NOSVGFLAG=NOSVG \
		MAEMOFLAG=NOKIA_770 \
		LOCALE_PREFIX=$(PREFIX)/share/locale \
		CONFDIR=/etc/tuxpaint


# Include the i18n stuff (moved out of main Makefile -bjk 2007.05.02)
#
include Makefile-i18n


# "make install" installs all of the various parts
# (depending on the *PREFIX variables at the top, you probably need
# to do this as superuser ("root"))

install:	install-bin install-data install-man install-doc \
		install-magic-plugins \
		install-magic-plugin-dev \
		install-icon install-gettext install-im install-importscript \
		install-default-config install-example-stamps \
		install-example-starters \
		install-gnome install-kde install-kde-icons
	@echo
	@echo "--------------------------------------------------------------"
	@echo
	@echo "All done! Now (preferably NOT as 'root' superuser),"
	@echo "you can type the command 'tuxpaint' to run the program!!!"
	@echo
	@echo "For more information, see the 'tuxpaint' man page,"
	@echo "run 'tuxpaint --usage' or see $(DOC_PREFIX)/README.txt"
	@echo
	@echo "Visit Tux Paint's home page for more information, updates"
	@echo "and to learn how you can help out!"
	@echo
	@echo "  http://www.tuxpaint.org/"
	@echo
	@echo "Enjoy!"
	@echo


install-magic-plugins:
	@echo
	@echo "...Installing Magic Tool plug-ins..."
	@install -d $(MAGIC_PREFIX)
	@cp magic/*.$(SO_TYPE) $(MAGIC_PREFIX)
	@chmod a+r,g-w,o-w $(MAGIC_PREFIX)/*.$(SO_TYPE)
	@install -d $(DATA_PREFIX)/images/magic
	@cp magic/icons/*.png $(DATA_PREFIX)/images/magic
	@chmod a+r,g-w,o-w $(DATA_PREFIX)/images/magic/*.png
	@install -d $(DATA_PREFIX)/sounds/magic
	@cp magic/sounds/*.wav magic/sounds/*.ogg $(DATA_PREFIX)/sounds/magic
	@chmod a+r,g-w,o-w $(DATA_PREFIX)/sounds/magic/*.wav \
			$(DATA_PREFIX)/sounds/magic/*.ogg

install-magic-plugin-dev:	src/tp_magic_api.h
	@echo
	@echo "...Installing Magic Tool plug-in development files and docs..."
	@cp tp-magic-config $(BIN_PREFIX)
	@chmod a+rx,g-w,o-w $(BIN_PREFIX)/tp-magic-config
	@install -d $(INCLUDE_PREFIX)/tuxpaint
	@cp src/tp_magic_api.h $(INCLUDE_PREFIX)/tuxpaint
	@chmod a+r,g-w,o-w $(INCLUDE_PREFIX)/tuxpaint/tp_magic_api.h
	@install -d $(DEVDOC_PREFIX)
	@cp -R magic/docs/* $(DEVDOC_PREFIX)
	@chmod a=rX,g=rX,u=rwX $(DEVDOC_PREFIX)

# Installs the various parts for the MinGW/MSYS development/testing environment.


install-private-win32:	install-bin install-data install-man install-doc \
		install-magic-plugins \
		install-magic-plugin-dev \
		install-gettext install-im install-importscript \
		install-default-config install-example-stamps \
		install-example-starters
	@echo
	@echo "--------------------------------------------------------------"
	@echo
	@echo "All done!"
	@echo "Now you can type the command 'tuxpaint' to run the program!!!"
	@echo
	@echo "For more information, see the 'tuxpaint' man page,"
	@echo "run 'tuxpaint --usage' or see $(DOC_PREFIX)/README.txt"
	@echo
	@echo "Visit Tux Paint's home page for more information, updates"
	@echo "and to learn how you can help out!"
	@echo
	@echo "  http://www.tuxpaint.org/"
	@echo
	@echo "Enjoy!"
	@echo


# Installs the various parts for the MinGW/MSYS development/testing environment.

bdist-private-win32:	install-bin install-data install-doc \
		install-magic-plugins \
		install-gettext install-im install-dlls\
		install-example-stamps install-example-starters
	@echo
	@echo "--------------------------------------------------------------"
	@echo
	@echo "A binary distribution suitable for running on Windows systems"
	@echo "other than this one is now available in $(PREFIX)."
	@echo
	@echo "Try compiling the InnoSetup installer script 'tuxpaint.iss'."
	@echo


# "make install-beos" installs Tux Paint, but using BeOS settings

install-beos:
	make install \
		PREFIX=/boot/develop/tools/gnupro \
		BIN_PREFIX=./ \
		DATA_PREFIX=./data \
		DOC_PREFIX=./docs \
		MAN_PREFIX=./src \
		CONFDIR=./src/ \
		ICON_PREFIX=. \
		X11_ICON_PREFIX=. \
		LOCALE_PREFIX=./build/share/locale \
		IM_PREFIX=./src \
		SVG_LIB= SVG_CFLAGS= NOSVGFLAG=NOSVG \
		NOPANGOFLAG=NO_SDLPANGO SDL_PANGO_LIB= \
		CFLAGS="-O1 -funroll-loops -fomit-frame-pointer -pipe -Wall" \
		RSRC_CMD="xres -o tuxpaint tuxpaint.rsrc" \
		MIMESET_CMD="mimeset -f tuxpaint" \
		ARCH_LINKS="-lintl -lpng -lz -lbe" \
		ARCH_HEADERS="src/BeOS_print.h" \
		ARCH_LIBS="obj/BeOS_print.o" \
		MAGIC_PREFIX=./build/lib/tuxpaint/plugins

# "make install-win32" installs Tux Paint, but using MinGW/MSYS settings
# Suitable for development/testing in the MinGW/MSYS environment.
install-win32:
	@strip -s tuxpaint.exe
	@make install-private-win32 \
		PREFIX=/usr/local \
		BIN_PREFIX=$(PREFIX)/bin \
		EXE_EXT=.exe \
		SO_TYPE=dll \
		DATA_PREFIX=$(PREFIX)/share/tuxpaint \
		DOC_PREFIX=$(PREFIX)/share/doc/tuxpaint \
		MAN_PREFIX=$(PREFIX)/share/man \
		ICON_PREFIX=. \
		X11_ICON_PREFIX=. \
		LOCALE_PREFIX=$(PREFIX)/share/locale \
		IM_PREFIX=$(PREFIX)/share/tuxpaint/im \
		CONFDIR=$(PREFIX)/etc/tuxpaint \

# "make bdist-win32" recompiles Tux Paint to work with executable-relative
# data, docs and locale directories. Also copies all files, including DLLs,
# into a 'bdist' output directory ready for processing by an installer script.
bdist-win32:
	@-rm -f tuxpaint.exe
	@-rm -f obj/*.o
	make \
		PREFIX=./win32/bdist \
		BIN_PREFIX=$(PREFIX)/bin \
		EXE_EXT=.exe \
		SO_TYPE=dll \
		DATA_PREFIX=data \
		DOC_PREFIX=docs \
		LOCALE_PREFIX=locale \
		IM_PREFIX=im \
		CONFDIR=. \
		ARCH_LINKS="-lintl -lpng12 -lwinspool -lshlwapi" \
		ARCH_HEADERS="src/win32_print.h" \
		ARCH_LIBS="obj/win32_print.o obj/resource.o" \
		SVG_LIB="-lrsvg-2 -lcairo -lgobject-2.0" \
		SVG_CFLAGS="-I/usr/local/include/librsvg-2/librsvg -I/usr/local/include/gtk-2.0 -I/usr/local/include/glib-2.0 -I/usr/local/lib/glib-2.0/include -I/usr/local/include/cairo" \
		INCLUDE_PREFIX=plugins/include \
		MAGIC_PREFIX=plugins \
		TARGET_PASSTHRU=win32 \
		PAPER_LIB= 
	strip -s tuxpaint.exe
	make bdist-private-win32 \
		PREFIX=./win32/bdist \
		BIN_PREFIX=./win32/bdist \
		EXE_EXT=.exe \
		SO_TYPE=dll \
		DATA_PREFIX=./win32/bdist/data \
		DOC_PREFIX=./win32/bdist/docs \
		LOCALE_PREFIX=./win32/bdist/locale \
		IM_PREFIX=./win32/bdist/im \
		CONFDIR=./win32/bdist \
		INCLUDE_PREFIX=./win32/bdist/plugins/include \
		MAGIC_PREFIX=./win32/bdist/plugins \
		TARGET_PASSTHRU=win32

# "make bdist-clean" deletes the 'bdist' directory
bdist-clean:
	@echo
	@echo "Cleaning up the 'bdist' directory! ($(PWD))"
	@-rm -rf ./win32/bdist
	@echo

# "make clean" deletes the program, the compiled objects and the
# built man page (returns to factory archive, pretty much...)

clean:
	@echo
	@echo "Cleaning up the build directory! ($(PWD))"
	@-rm -f tuxpaint
	@-rm -f obj/*.o
	@#if [ -d obj ]; then rmdir obj; fi
	@-rm -f trans/*.mo
	@-rm -f src/tp_magic_api.h
	@-rm -f tp-magic-config
	@if [ -d trans ]; then rmdir trans; fi
	@cd magic && make clean$(TARGET_PASSTHRU)
	@echo

clean-win32:
	@make clean\
		TARGET_PASSTHRU=win32

# "make uninstall" should remove the various parts from their
# installation locations.  BE SURE the *PREFIX variables at the top
# are the same as they were when you installed, of course!!!

uninstall:	uninstall-i18n
	-if [ "x$(GNOME_PREFIX)" != "x" ]; then \
	  rm $(GNOME_PREFIX)/share/applications/tuxpaint.desktop; \
	  rm $(GNOME_PREFIX)/share/pixmaps/tuxpaint.png; \
	else \
	  rm /usr/share/applications/tuxpaint.desktop; \
	  rm /usr/share/pixmaps/tuxpaint.png; \
	fi
	-if [ "x$(KDE_PREFIX)" != "x" ]; then \
	  rm $(KDE_PREFIX)/Graphics/tuxpaint.desktop; \
	fi
	-rm $(ICON_PREFIX)/tuxpaint.png
	-rm $(X11_ICON_PREFIX)/tuxpaint.xpm
	-if [ "x$(KDE_ICON_PREFIX)" != "x" ]; then \
	  rm $(KDE_ICON_PREFIX)/hicolor/scalable/apps/tuxpaint.svg; \
	  rm $(KDE_ICON_PREFIX)/hicolor/192x192/apps/tuxpaint.png; \
	  rm $(KDE_ICON_PREFIX)/hicolor/128x128/apps/tuxpaint.png; \
	  rm $(KDE_ICON_PREFIX)/hicolor/96x96/apps/tuxpaint.png; \
	  rm $(KDE_ICON_PREFIX)/hicolor/64x64/apps/tuxpaint.png; \
	  rm $(KDE_ICON_PREFIX)/hicolor/48x48/apps/tuxpaint.png; \
	  rm $(KDE_ICON_PREFIX)/hicolor/32x32/apps/tuxpaint.png; \
	  rm $(KDE_ICON_PREFIX)/hicolor/22x22/apps/tuxpaint.png; \
	  rm $(KDE_ICON_PREFIX)/hicolor/16x16/apps/tuxpaint.png; \
	fi
	-rm $(BIN_PREFIX)/tuxpaint
	-rm $(BIN_PREFIX)/tuxpaint-import
	-rm -r $(DATA_PREFIX)
	-rm -r $(DOC_PREFIX)
	-rm $(MAN_PREFIX)/man1/tuxpaint.1.gz
	-rm $(MAN_PREFIX)/pl/man1/tuxpaint.1.gz
	-rm $(MAN_PREFIX)/man1/tuxpaint-import.1.gz
	-rm $(MAN_PREFIX)/man1/tp-magic-config.1.gz
	-rm -f -r $(CONFDIR)
	-rm -r $(MAGIC_PREFIX)
	-rm -r $(INCLUDE_PREFIX)/tuxpaint
	-rm $(BIN_PREFIX)/tp-magic-config
	-rm -r $(DEVDOC_PREFIX)


# Install default config file:

install-default-config:
	@echo
	@echo "...Installing default config file..."
	@install -d $(CONFDIR)
	@cp src/tuxpaint.conf $(CONFDIR)
	@chmod 644 $(CONFDIR)/tuxpaint.conf


# Install example stamps

install-example-stamps:
	@echo
	@echo "...Installing example stamps..."
	@install -d $(DATA_PREFIX)/stamps
	@cp -R stamps/* $(DATA_PREFIX)/stamps
	@chmod -R a+rX,g-w,o-w $(DATA_PREFIX)/stamps


# Install example starters

install-example-starters:
	@echo
	@echo "...Installing example starter images..."
	@install -d $(DATA_PREFIX)/starters
	@cp -R starters/* $(DATA_PREFIX)/starters
	@chmod -R a+rX,g-w,o-w $(DATA_PREFIX)/starters


# Install a launcher icon in the Gnome menu

install-gnome:
	@echo
	@echo "...Installing launcher icon into GNOME..."
	@if [ "x$(GNOME_PREFIX)" != "x" ]; then \
	 install -d $(PKG_ROOT)$(GNOME_PREFIX)/share/pixmaps; \
	 cp data/images/icon.png $(PKG_ROOT)/$(GNOME_PREFIX)/share/pixmaps/tuxpaint.png; \
	 chmod 644 $(PKG_ROOT)$(GNOME_PREFIX)/share/pixmaps/tuxpaint.png; \
	 install -d $(PKG_ROOT)$(GNOME_PREFIX)/share/applications; \
	 cp src/tuxpaint.desktop $(PKG_ROOT)$(GNOME_PREFIX)/share/applications/; \
	 chmod 644 $(PKG_ROOT)$(GNOME_PREFIX)/share/applications/tuxpaint.desktop; \
	fi


# Install a launcher icon for the Nokia 770.

install-nokia770:
	@echo
	@echo "...Installing launcher icon into the Nokia 770..."
	@if [ "x$(NOKIA770_PREFIX)" != "x" ]; then \
	 install -d $(PKG_ROOT)$(NOKIA770_PREFIX)/share/pixmaps; \
	 cp data/images/icon.png $(PKG_ROOT)$(NOKIA770_PREFIX)/share/pixmaps/tuxpaint.png; \
	 chmod 644 $(PKG_ROOT)$(NOKIA770_PREFIX)/share/pixmaps/tuxpaint.png; \
	 cp hildon/tuxpaint.xpm $(PKG_ROOT)/$(NOKIA770_PREFIX)/share/pixmaps/tuxpaint.xpm; \
	 chmod 644 $(PKG_ROOT)$(NOKIA770_PREFIX)/share/pixmaps/tuxpaint.xpm; \
	 install -d $(PKG_ROOT)$(NOKIA770_PREFIX)/share/applications/hildon; \
	 cp hildon/tuxpaint.desktop $(PKG_ROOT)$(NOKIA770_PREFIX)/share/applications/hildon/; \
	 chmod 644 $(PKG_ROOT)$(NOKIA770_PREFIX)/share/applications/hildon/tuxpaint.desktop; \
	 install -d $(PKG_ROOT)/etc/tuxpaint; \
	 cp hildon/tuxpaint.conf $(PKG_ROOT)/etc/tuxpaint; \
	 chmod 644 $(PKG_ROOT)/etc/tuxpaint/tuxpaint.conf; \
	 rm -rf $(PKG_ROOT)$(NOKIA770_PREFIX)/X11R6; \
	 rm -rf $(PKG_ROOT)$(NOKIA770_PREFIX)/share/doc; \
	 rm -rf $(PKG_ROOT)$(NOKIA770_PREFIX)/share/man; \
	fi
	@-find $(PKG_ROOT)$(NOKIA770_PREFIX) -name CVS -type d -exec rm -rf \{\} \;



# Install a launcher icon in the KDE menu...

install-kde:
	@echo
	@echo "...Installing launcher icon into KDE..."
	@if [ "x$(KDE_PREFIX)" != "x" ]; then \
	  install -d $(PKG_ROOT)$(KDE_PREFIX)/Graphics; \
	  cp src/tuxpaint.desktop $(PKG_ROOT)$(KDE_PREFIX)/Graphics/; \
	  chmod 644 $(PKG_ROOT)$(KDE_PREFIX)/Graphics/tuxpaint.desktop; \
	fi


install-kde-icons:
	@echo "...Installing launcher icon graphics into KDE..."
	@if [ "x$(KDE_ICON_PREFIX)" != "x" ]; then \
	  install -d $(PKG_ROOT)$(KDE_ICON_PREFIX)/hicolor/scalable/apps/; \
	  install -d $(PKG_ROOT)$(KDE_ICON_PREFIX)/hicolor/192x192/apps/; \
	  install -d $(PKG_ROOT)$(KDE_ICON_PREFIX)/hicolor/128x128/apps/; \
	  install -d $(PKG_ROOT)$(KDE_ICON_PREFIX)/hicolor/96x96/apps/; \
	  install -d $(PKG_ROOT)$(KDE_ICON_PREFIX)/hicolor/64x64/apps/; \
	  install -d $(PKG_ROOT)$(KDE_ICON_PREFIX)/hicolor/48x48/apps/; \
	  install -d $(PKG_ROOT)$(KDE_ICON_PREFIX)/hicolor/32x32/apps/; \
	  install -d $(PKG_ROOT)$(KDE_ICON_PREFIX)/hicolor/22x22/apps/; \
	  install -d $(PKG_ROOT)$(KDE_ICON_PREFIX)/hicolor/16x16/apps/; \
	  cp data/images/tuxpaint-icon.svg \
		$(PKG_ROOT)$(KDE_ICON_PREFIX)/hicolor/scalable/apps/tuxpaint.svg; \
          chmod 644 $(PKG_ROOT)$(KDE_ICON_PREFIX)/hicolor/scalable/apps/tuxpaint.svg; \
	  cp data/images/icon192x192.png \
		$(PKG_ROOT)$(KDE_ICON_PREFIX)/hicolor/192x192/apps/tuxpaint.png; \
          chmod 644 $(PKG_ROOT)$(KDE_ICON_PREFIX)/hicolor/192x192/apps/tuxpaint.png; \
	  cp data/images/icon128x128.png \
		$(PKG_ROOT)$(KDE_ICON_PREFIX)/hicolor/128x128/apps/tuxpaint.png; \
          chmod 644 $(PKG_ROOT)$(KDE_ICON_PREFIX)/hicolor/128x128/apps/tuxpaint.png; \
	  cp data/images/icon96x96.png \
		$(PKG_ROOT)$(KDE_ICON_PREFIX)/hicolor/96x96/apps/tuxpaint.png; \
          chmod 644 $(PKG_ROOT)$(KDE_ICON_PREFIX)/hicolor/96x96/apps/tuxpaint.png; \
	  cp data/images/icon64x64.png \
		$(PKG_ROOT)$(KDE_ICON_PREFIX)/hicolor/64x64/apps/tuxpaint.png; \
          chmod 644 $(PKG_ROOT)$(KDE_ICON_PREFIX)/hicolor/64x64/apps/tuxpaint.png; \
	  cp data/images/icon48x48.png \
		$(PKG_ROOT)$(KDE_ICON_PREFIX)/hicolor/48x48/apps/tuxpaint.png; \
          chmod 644 $(PKG_ROOT)$(KDE_ICON_PREFIX)/hicolor/48x48/apps/tuxpaint.png; \
	  cp data/images/icon32x32.png \
		$(PKG_ROOT)$(KDE_ICON_PREFIX)/hicolor/32x32/apps/tuxpaint.png; \
          chmod 644 $(PKG_ROOT)$(KDE_ICON_PREFIX)/hicolor/32x32/apps/tuxpaint.png; \
	  cp data/images/icon22x22.png \
		$(PKG_ROOT)$(KDE_ICON_PREFIX)/hicolor/22x22/apps/tuxpaint.png; \
          chmod 644 $(PKG_ROOT)$(KDE_ICON_PREFIX)/hicolor/22x22/apps/tuxpaint.png; \
	  cp data/images/icon16x16.png \
		$(PKG_ROOT)$(KDE_ICON_PREFIX)/hicolor/16x16/apps/tuxpaint.png; \
          chmod 644 $(PKG_ROOT)$(KDE_ICON_PREFIX)/hicolor/16x16/apps/tuxpaint.png; \
	fi


# Install the PNG icon (for GNOME, KDE, etc.)
# and the 24-color 32x32 XPM (for other Window managers):

# FIXME: Should this also use $(PKG_ROOT)?

install-icon:
	@echo
	@echo "...Installing launcher icon graphics..."
	@install -d $(ICON_PREFIX)
	@cp data/images/icon.png $(ICON_PREFIX)/tuxpaint.png
	@chmod 644 $(ICON_PREFIX)/tuxpaint.png
	@install -d $(X11_ICON_PREFIX)
	@cp data/images/icon32x32.xpm $(X11_ICON_PREFIX)/tuxpaint.xpm
	@chmod 644 $(X11_ICON_PREFIX)/tuxpaint.xpm


# Install the program:

install-bin:
	@echo
	@echo "...Installing program itself..."
	@install -d $(BIN_PREFIX)
	@cp tuxpaint $(BIN_PREFIX)
	@chmod a+rx,g-w,o-w $(BIN_PREFIX)/tuxpaint$(EXE_EXT)

# Install the required Windows DLLs into the 'bdist' directory

install-dlls:
	@echo
	@echo "...Installing Windows DLLs..."
	@install -d $(BIN_PREFIX)
	@cp `which tuxpaint-config.exe` $(BIN_PREFIX)
	@cp `which libintl-8.dll` $(BIN_PREFIX)
	@cp `which libiconv-2.dll` $(BIN_PREFIX)
	@cp `which libpng12.dll` $(BIN_PREFIX)
	@cp `which SDL.dll` $(BIN_PREFIX)
	@cp `which SDL_image.dll` $(BIN_PREFIX)
	@cp `which SDL_mixer.dll` $(BIN_PREFIX)
	@cp `which SDL_ttf.dll` $(BIN_PREFIX)
	@cp `which libfreetype-6.dll` $(BIN_PREFIX)
	@cp `which zlib1.dll` $(BIN_PREFIX)
	@cp `which libogg-0.dll` $(BIN_PREFIX)
	@cp `which libvorbis-0.dll` $(BIN_PREFIX)
	@cp `which libvorbisfile-3.dll` $(BIN_PREFIX)
	@cp `which jpeg.dll` $(BIN_PREFIX)
	@cp `which libxml2-2.dll` $(BIN_PREFIX)
	@cp `which libcairo-2.dll` $(BIN_PREFIX)
	@cp `which libfontconfig-1.dll` $(BIN_PREFIX)
	@cp `which libSDL_Pango-1.dll` $(BIN_PREFIX)
	@cp `which libgobject-2.0-0.dll` $(BIN_PREFIX)
	@cp `which librsvg-2-2.dll` $(BIN_PREFIX)
	@cp `which libcroco-0.6-3.dll` $(BIN_PREFIX)
	@cp `which libgdk_pixbuf-2.0-0.dll` $(BIN_PREFIX)
	@cp `which libglib-2.0-0.dll` $(BIN_PREFIX)
	@cp `which libgsf-1-114.dll` $(BIN_PREFIX)
	@cp `which libpango-1.0-0.dll` $(BIN_PREFIX)
	@cp `which libpangocairo-1.0-0.dll` $(BIN_PREFIX)
	@cp `which libpangoft2-1.0-0.dll` $(BIN_PREFIX)
	@cp `which libgmodule-2.0-0.dll` $(BIN_PREFIX)
	@cp `which libpangowin32-1.0-0.dll` $(BIN_PREFIX)
	@strip -s $(BIN_PREFIX)/*.dll
	@echo
	@echo "...Installing Configuration Files..."
	@cp -r win32/etc/ $(BIN_PREFIX)
	@echo
	@echo "...Installing Library Modules..."
	@mkdir -p $(BIN_PREFIX)/lib/gtk-2.0/2.10.0/engines
	@cp /usr/local/lib/gtk-2.0/2.10.0/engines/*.dll $(BIN_PREFIX)/lib/gtk-2.0/2.10.0/engines
	@strip -s $(BIN_PREFIX)/lib/gtk-2.0/2.10.0/engines/*.dll
	@mkdir -p $(BIN_PREFIX)/lib/gtk-2.0/2.10.0/immodules
	@cp /usr/local/lib/gtk-2.0/2.10.0/immodules/*.dll $(BIN_PREFIX)/lib/gtk-2.0/2.10.0/immodules
	@strip -s $(BIN_PREFIX)/lib/gtk-2.0/2.10.0/immodules/*.dll
	@mkdir -p $(BIN_PREFIX)/lib/gtk-2.0/2.10.0/loaders
	@cp /usr/local/lib/gtk-2.0/2.10.0/loaders/*.dll $(BIN_PREFIX)/lib/gtk-2.0/2.10.0/loaders
	@strip -s $(BIN_PREFIX)/lib/gtk-2.0/2.10.0/loaders/*.dll
	@mkdir -p $(BIN_PREFIX)/lib/pango/1.6.0/modules
	@cp /usr/local/lib/pango/1.6.0/modules/*.dll $(BIN_PREFIX)/lib/pango/1.6.0/modules
	@strip -s $(BIN_PREFIX)/lib/pango/1.6.0/modules/*.dll
	
# Install the import script:

install-importscript:
	@echo
	@echo "...Installing 'tuxpaint-import' script..."
	@cp src/tuxpaint-import.sh $(BIN_PREFIX)/tuxpaint-import
	@chmod a+rx,g-w,o-w $(BIN_PREFIX)/tuxpaint-import


# Install the data (sound, graphics, fonts):

install-data:
	@echo
	@echo "...Installing data files..."
	@install -d $(DATA_PREFIX)
	@cp -R data/* $(DATA_PREFIX)
	@chmod -R a+rX,g-w,o-w $(DATA_PREFIX)
	@echo
	@echo "...Installing fonts..."
	@install -d $(DATA_PREFIX)/fonts/locale
	@cp -R fonts/locale/* $(DATA_PREFIX)/fonts/locale
	@chmod -R a+rX,g-w,o-w $(DATA_PREFIX)/fonts/locale



# Install the text documentation:

install-doc:
	@echo
	@echo "...Installing documentation..."
	@install -d $(DOC_PREFIX)
	@cp -R docs/* $(DOC_PREFIX)
	@cp -R magic/magic-docs $(DOC_PREFIX)
	@chmod a=rX,g=rX,u=rwX $(DOC_PREFIX)


# Install the man page:

install-man:
	@echo
	@echo "...Installing man pages..."
	@# man1 directory...
	@install -d $(MAN_PREFIX)/man1
	@# tuxpaint.1
	@cp src/manpage/tuxpaint.1 $(MAN_PREFIX)/man1
	@gzip -f $(MAN_PREFIX)/man1/tuxpaint.1
	@chmod a+rx,g-w,o-w $(MAN_PREFIX)/man1/tuxpaint.1.gz
	@# pl/man1 directory...
	@install -d $(MAN_PREFIX)/pl/man1/
	@# tuxpaint-pl.1
	@cp src/manpage/tuxpaint-pl.1 $(MAN_PREFIX)/pl/man1/tuxpaint.1
	@gzip -f $(MAN_PREFIX)/pl/man1/tuxpaint.1
	@chmod a+rx,g-w,o-w $(MAN_PREFIX)/pl/man1/tuxpaint.1.gz
	@# tuxpaint-import.1
	@cp src/manpage/tuxpaint-import.1 $(MAN_PREFIX)/man1/
	@gzip -f $(MAN_PREFIX)/man1/tuxpaint-import.1
	@chmod a+rx,g-w,o-w $(MAN_PREFIX)/man1/tuxpaint-import.1.gz
	@# tp-magic-config.1
	@cp src/manpage/tp-magic-config.1 $(MAN_PREFIX)/man1/
	@gzip -f $(MAN_PREFIX)/man1/tp-magic-config.1
	@chmod a+rx,g-w,o-w $(MAN_PREFIX)/man1/tp-magic-config.1.gz



# Build the program!

tuxpaint:	obj/tuxpaint.o obj/i18n.o obj/im.o obj/cursor.o obj/pixels.o \
		obj/rgblinear.o obj/playsound.o obj/fonts.o \
		obj/progressbar.o obj/dirwalk.o obj/get_fname.o \
		obj/selection.o \
		$(HQXX_O) $(ARCH_LIBS)
	@echo
	@echo "...Linking Tux Paint..."
	@$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(SDL_CFLAGS) $(PAPER_CFLAGS) \
		$(DEFS) \
		-o tuxpaint \
		$^ \
		$(HQXX_O) \
		$(SDL_LIBS) \
		$(SVG_LIB) \
		$(PAPER_LIB) \
		$(ARCH_LINKS)
	@$(RSRC_CMD)
	@$(MIMESET_CMD)


# Build the object for the program!

obj/tuxpaint.o:	src/tuxpaint.c \
		src/i18n.h src/im.h src/cursor.h src/pixels.h \
		src/rgblinear.h src/playsound.h src/fonts.h \
		src/progressbar.h src/dirwalk.h src/get_fname.h \
		src/compiler.h src/debug.h src/selection.h \
		src/tools.h src/titles.h src/colors.h src/shapes.h \
		src/sounds.h src/tip_tux.h src/great.h \
		src/tp_magic_api.h \
		$(HQXX_H) \
		src/$(MOUSEDIR)/arrow.xbm src/$(MOUSEDIR)/arrow-mask.xbm \
		src/$(MOUSEDIR)/hand.xbm src/$(MOUSEDIR)/hand-mask.xbm \
		src/$(MOUSEDIR)/insertion.xbm \
		src/$(MOUSEDIR)/insertion-mask.xbm \
		src/$(MOUSEDIR)/wand.xbm src/$(MOUSEDIR)/wand-mask.xbm \
		src/$(MOUSEDIR)/brush.xbm src/$(MOUSEDIR)/brush-mask.xbm \
		src/$(MOUSEDIR)/crosshair.xbm \
		src/$(MOUSEDIR)/crosshair-mask.xbm \
		src/$(MOUSEDIR)/rotate.xbm src/$(MOUSEDIR)/rotate-mask.xbm \
		src/$(MOUSEDIR)/tiny.xbm src/$(MOUSEDIR)/tiny-mask.xbm \
		src/$(MOUSEDIR)/watch.xbm src/$(MOUSEDIR)/watch-mask.xbm \
		src/$(MOUSEDIR)/up.xbm src/$(MOUSEDIR)/up-mask.xbm \
		src/$(MOUSEDIR)/down.xbm src/$(MOUSEDIR)/down-mask.xbm \
		$(ARCH_HEADERS) \
		Makefile
	@echo
	@echo "...Compiling Tux Paint from source..."
	@$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(SDL_CFLAGS) $(MOUSE_CFLAGS) $(DEFS) \
		-c src/tuxpaint.c -o obj/tuxpaint.o

obj/i18n.o:	src/i18n.c src/i18n.h src/debug.h
	@echo
	@echo "...Compiling i18n support..."
	@$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(DEFS) \
		-c src/i18n.c -o obj/i18n.o

obj/im.o:	src/im.c src/im.h src/debug.h
	@echo
	@echo "...Compiling IM support..."
	@$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(SDL_CFLAGS) $(DEFS) \
		-c src/im.c -o obj/im.o

obj/get_fname.o:	src/get_fname.c src/get_fname.h src/debug.h
	@echo
	@echo "...Compiling filename support..."
	@$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(DEFS) \
		-c src/get_fname.c -o obj/get_fname.o

obj/fonts.o:	src/fonts.c src/fonts.h src/dirwalk.h src/progressbar.h \
		src/get_fname.h src/debug.h
	@echo
	@echo "...Compiling font support..."
	@$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(SDL_CFLAGS) $(DEFS) \
		-c src/fonts.c -o obj/fonts.o

obj/dirwalk.o:	src/dirwalk.c src/dirwalk.h src/progressbar.h src/fonts.h \
		src/debug.h
	@echo
	@echo "...Compiling directory-walking support..."
	@$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(SDL_CFLAGS) $(DEFS) \
		-c src/dirwalk.c -o obj/dirwalk.o

obj/cursor.o:	src/cursor.c src/cursor.h src/debug.h
	@echo
	@echo "...Compiling cursor support..."
	@$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(SDL_CFLAGS) $(MOUSE_CFLAGS) $(DEFS) \
		-c src/cursor.c -o obj/cursor.o

obj/pixels.o:	src/pixels.c src/pixels.h src/compiler.h src/debug.h
	@echo
	@echo "...Compiling pixel functions..."
	@$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(SDL_CFLAGS) $(DEFS) \
		-c src/pixels.c -o obj/pixels.o

obj/playsound.o:	src/playsound.c src/playsound.h \
			src/compiler.h src/debug.h
	@echo
	@echo "...Compiling sound playback functions..."
	@$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(SDL_CFLAGS) $(DEFS) \
		-c src/playsound.c -o obj/playsound.o

obj/progressbar.o:	src/progressbar.c src/progressbar.h \
			src/compiler.h src/debug.h
	@echo
	@echo "...Compiling progress bar functions..."
	@$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(SDL_CFLAGS) $(DEFS) \
		-c src/progressbar.c -o obj/progressbar.o

obj/rgblinear.o:	src/rgblinear.c src/rgblinear.h \
			src/compiler.h src/debug.h
	@echo
	@echo "...Compiling RGB to Linear functions..."
	@$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(SDL_CFLAGS) $(DEFS) \
		-c src/rgblinear.c -o obj/rgblinear.o

obj/selection.o:	src/selection.c src/selection.h
	@echo
	@echo "...Compiling selection functions..."
	@$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(SDL_CFLAGS) $(DEFS) \
		-c src/selection.c -o obj/selection.o

obj/BeOS_print.o:	src/BeOS_print.cpp src/BeOS_print.h
	@echo
	@echo "...Compiling BeOS print support..."
	@$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(SDL_CFLAGS) $(DEFS) \
		-c src/BeOS_print.cpp -o obj/BeOS_print.o

obj/win32_print.o:	src/win32_print.c src/win32_print.h src/debug.h
	@echo
	@echo "...Compiling win32 print support..."
	@$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(SDL_CFLAGS) $(DEFS) \
		-c src/win32_print.c -o obj/win32_print.o

obj/postscript_print.o:	src/postscript_print.c Makefile \
			src/postscript_print.h src/debug.h
	@echo
	@echo "...Compiling PostScript print support..."
	@$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(SDL_CFLAGS) $(DEFS) \
		-c src/postscript_print.c -o obj/postscript_print.o

obj/resource.o:	win32/resources.rc win32/resource.h
	@echo
	@echo "...Compiling win32 resources..."
	@windres -i win32/resources.rc -o obj/resource.o


# Go into 'magic' subdirectory and buld magic plug-ins

magic-plugins:	src/tp_magic_api.h
	@cd magic && make $(TARGET_PASSTHRU)


src/tp_magic_api.h:	src/tp_magic_api.h.in
	@echo
	@echo "...Generating 'Magic' tool API development header file..."
	@cat src/tp_magic_api.h.in | sed -e s/__APIVERSION__/$(MAGIC_API_VERSION)/ > src/tp_magic_api.h


tp-magic-config:	src/tp-magic-config.sh.in Makefile
	@echo
	@echo "...Generating 'Magic' tool API configuration script..."
	@sed src/tp-magic-config.sh.in -e s/__VERSION__/$(VER_VERSION)/ \
		-e s/__APIVERSION__/$(MAGIC_API_VERSION)/ \
		-e s=__INCLUDE__=$(INCLUDE_PREFIX)/tuxpaint= \
		-e s=__DATAPREFIX__=$(DATA_PREFIX)= \
		-e s=__PLUGINPREFIX__=$(MAGIC_PREFIX)= \
		-e s=__PLUGINDOCPREFIX__=$(DOC_PREFIX)/magic-docs= \
		> tp-magic-config

# Make the "obj" directory to throw the object(s) into:
# (not necessary any more; bjk 2006.02.20)

obj:
	@mkdir obj

.PHONY: win32

