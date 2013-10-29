BINDIR=bin
OBJDIR=obj
INCLUDEDIR=include
SRCDIR=src
DOCDIR=doc
CFLAGS=-std=c99 -Wall -Os -c -I$(INCLUDEDIR) -I/usr/include/ -I/usr/include/libusb-1.0/ -D_BSD_SOURCE
CP=cp
RM=rm -f
RMDIR=rm -rf
MKDIR=mkdir -p
CC=gcc
CHMOD=chmod -R 777
NAME=host

.PHONY: install clean uninstall

$(BINDIR)/$(NAME): $(OBJDIR)/$(NAME).o
	$(MKDIR) $(BINDIR)
	$(CC) -o $@ $^ -lusb-1.0

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(MKDIR) $(OBJDIR)
	$(CC) $(CFLAGS) -o $@ $<

clean:
	$(RMDIR) $(BINDIR)
	$(RMDIR) $(OBJDIR)
	$(RMDIR) $(DOCDIR)
