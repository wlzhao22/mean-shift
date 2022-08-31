# A simple makefile generator by KiSoft, 2008. mailto: kisoft@rambler.ru
# version: 0.3.12.3

# Project Variables start
CPP=g++
CC=gcc
LD=g++
LIB=ar
WINDRES=
# Project Variables end

# Target: Release

OBJS_RELEASE=obj/Release/abstractimage.o obj/Release/cimage.o obj/Release/image.o obj/Release/ioimage.o obj/Release/main.o obj/Release/segm.o obj/Release/tool.o obj/Release/vmath.o

Release: bin/Release/meanshift

bin/Release/meanshift: $(OBJS_RELEASE)
	@echo Building console executable bin/Release/meanshift
	@g++  -o bin/Release/meanshift $(OBJS_RELEASE)  -s  lib/libjpeg.a 

obj/Release/abstractimage.o: abstractimage.cpp
	@echo Compiling: abstractimage.cpp
	@g++ -Wall -fexceptions  -O2     -c abstractimage.cpp -o obj/Release/abstractimage.o

obj/Release/cimage.o: cimage.cpp
	@echo Compiling: cimage.cpp
	@g++ -Wall -fexceptions  -O2     -c cimage.cpp -o obj/Release/cimage.o

obj/Release/image.o: image.cpp
	@echo Compiling: image.cpp
	@g++ -Wall -fexceptions  -O2     -c image.cpp -o obj/Release/image.o

obj/Release/ioimage.o: ioimage.cpp
	@echo Compiling: ioimage.cpp
	@g++ -Wall -fexceptions  -O2     -c ioimage.cpp -o obj/Release/ioimage.o

obj/Release/main.o: main.cpp
	@echo Compiling: main.cpp
	@g++ -Wall -fexceptions  -O2     -c main.cpp -o obj/Release/main.o

obj/Release/segm.o: segm.cpp
	@echo Compiling: segm.cpp
	@g++ -Wall -fexceptions  -O2     -c segm.cpp -o obj/Release/segm.o

obj/Release/tool.o: tool.cpp
	@echo Compiling: tool.cpp
	@g++ -Wall -fexceptions  -O2     -c tool.cpp -o obj/Release/tool.o

obj/Release/vmath.o: vmath.cpp
	@echo Compiling: vmath.cpp
	@g++ -Wall -fexceptions  -O2     -c vmath.cpp -o obj/Release/vmath.o

abstractimage.cpp: abstractimage.h tool.h ioimage.h

ioimage.h: ./include/jpeglib.h

include/jpeglib.h: ./include/jconfig.h ./include/jmorecfg.h ./include/jpegint.h ./include/jerror.h

cimage.cpp: cimage.h ioimage.h tool.h vmath.h

cimage.h: abstractimage.h image.h

image.h: abstractimage.h

image.cpp: ioimage.h image.h tool.h

ioimage.cpp: ioimage.h cimage.h bmp.h vmath.h

main.cpp: segm.h

segm.h: bmp.h

segm.cpp: cimage.h segm.h

tool.cpp: tool.h

vmath.cpp: vmath.h

.PHONY: clean_Release

clean_Release: 
	@echo Delete $(OBJS_RELEASE) bin/Release/meanshift
	-@rm -f $(OBJS_RELEASE) bin/Release/meanshift



