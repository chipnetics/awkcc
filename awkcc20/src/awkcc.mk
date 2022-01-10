# This make file was specially created for the R&D Department (Dept. 45262).
# This make file will be called the following way the the system integrators
# and hence this file will call the original make file with
# apporpriate arguments. That is the only reason why it is here.
# The system integration will execute for EACH PACKAGE the command:
# cd <arg>/src; make -f *.mk ARCH=<arg> OBJROOT=<arg> PKGDIR=<arg>

# EXEROOT and OBJROOT can be independantly specified on the make command line.
# NOTE: -r option of pkgmk likes only full path names. OBJROOT better be proper.

ROOT = 
EXEROOT = $(ROOT)
OBJROOT = $(ROOT)
TESTDIR = .

#ARCH = # Default architecture must be specified on the command line.
# Default place to create packages. Can also be specified on the command line.
# Added for packaging the awkcc.
PKGDIR = /var/spool/pkg
PKGOPT = 
PKGNAME = awkcc

CC =     $(EXEROOT)/opt/SUNWspro/bin/cc		#SunPro Compiler (Solaris)
MAKE =   $(EXEROOT)/usr/ccs/bin/make
INC =    $(EXEROOT)/usr/include
INCSYS = $(EXEROOT)/usr/include/sys
INS =    $(EXEROOT)/usr/sbin/install
#SHELL = $(EXEROOT)/sbin/sh

INSDIR	= /usr/bin
INSMAN	= /usr/share/man/man1

CFLAGS=-O -Xt

CMD = install test clean clobber prepkg makepkg installpkg rmpkg infopkg chkpkg

$(CMD):
	cd ..; $(MAKE) $@ "CC=$(CC)" CFLAGS="$(CFLAGS)" "ARCH=$(ARCH)" "EXEROOT=$(EXEROOT)" "OBJROOT=$(OBJROOT)" "PKGDIR=$(PKGDIR)" TARGBIN=/opt/awkcc/bin TARGSH=/opt/awkcc/bin TARGLIB=/opt/awkcc/bin TARGHDR=/opt/awkcc/include 
