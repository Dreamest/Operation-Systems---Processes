EXTENSION = c
COMPILER  = gcc
PACKAGES  =
EXTRAINCL =
EXTRALIBS =

MAINS   = v1.c v2.c check.c
CFLAGS  = -W -Wall -pedantic $(ALL_HDRS)
LDFLAGS = $(ALL_LIBS)

ALL_HDRS = $(foreach pack, $(PACKAGES), $(shell echo `pkg-config --cflags $(pack)`)) $(EXTRAINCL)
ALL_LIBS = $(foreach pack, $(PACKAGES), `pkg-config --libs $(pack)`) $(EXTRALIBS)

EXT     = $(EXTENSION)
CC      = $(COMPILER)
ALLSRC  = $(wildcard *.$(EXT))
SRC     = $(filter-out $(MAINS),$(ALLSRC))
OBJ     = $(SRC:.$(EXT)=.o)

.SECONDEXPANSION:
.PHONY: all clean mrproper list pkgs-avail pkgs-version help

all: pkgs-avail $(MAINS:.$(EXT)=)

$(MAINS:.$(EXT)=): $(OBJ) $$(@).o
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.$(EXT)
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf $(OBJ) $(MAINS:.$(EXT)=.o)

mrproper: clean
	rm -rf $(MAINS:.$(EXT)=)

list:
	@echo -e "Sources $(SRC)\nObjets  $(OBJ)\nMains   $(MAINS)"

pkgs-version:
	@for p in $(PACKAGES); do \
		echo -n "$$p : "; \
		pkg-config --modversion $$p; \
	done

pkgs-avail:
	@for p in $(PACKAGES); do \
		pkg-config --exists $$p; \
		if [ $$? -ne 0 ]; then \
			echo -en "\033[31m" > /dev/stderr; \
			echo -en "The package $$p does not exist" > /dev/stderr; \
			echo -e  "\033[0m" > /dev/stderr; \
		fi; \
	done

help:
	@echo "Maky version $(MAKYVERSION)"; \
	echo "Generic Makefile for all your projects (C, C++ and so...)"; \
	echo "Even multi-main ones."; \
	echo ""; \
	echo "COMPILATION TARGETS :"; \
	echo " * all          : check for packages availability and make all"; \
	echo "                  executables"; \
	echo " * EXECUTABLE   : name your main file without extension to"; \
	echo "                  compile only this executable"; \
	echo "INFORMATION TARGETS :"; \
	echo " * clean        : delete objects"; \
	echo " * mrproper     : delete objects and executables"; \
	echo " * list         : list all recognized files"; \
	echo " * pkgs-version : list all the packages along with their"; \
	echo "                  version number"; \
	echo " * pkgs-avail   : Check for pakages availability through"; \
	echo "                  pkg-config. No output if all of the packages exist."; \
	echo ""; \
	echo "SETTINGS :"; \
	echo "   To set Maky for your project, edit it, and fill the"; \
	echo "   BASIC CUSTOMIZATION section."; \
	echo "   You can go further in editing the PLUMBING CUSTOMIZATION"; \
	echo "   section. For instance by adding a file containing a main in"; \
	echo "   the MAINS list.";

