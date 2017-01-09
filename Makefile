CC = gcc

CFLAGS = -O0 -g -Wall `sdl-config --cflags` -Isrc/tinyjpegdecoder -Isrc -Isrc/sdl
FILEBROWSER = 0

ifeq ($(FILEBROWSER),0)
CFLAGS += -DNO_FILEBROWSER 
SOURCES =
else ifeq ($(FILEBROWSER),1)
SOURCES = ./src/main.c 
endif

LDFLAGS = `sdl-config --libs` -lSDLmain -lSDL
OUTPUT = simpleimgviewer.elf

SOURCES += ./src/imgview.c 
SOURCES += ./src/sdl/n2DLib.c ./src/lodepng.c ./src/sdl/control.c
SOURCES += ./src/tinyjpegdecoder/jidctflt.c ./src/tinyjpegdecoder/tinyjpeg.c ./src/tinyjpegdecoder/loadjpg.c
OBJS = ${SOURCES:.c=.o}

${OUTPUT}:${OBJS}
	${CC} -o ${OUTPUT} ${OBJS} ${CFLAGS} ${LDFLAGS} 
	
clean:
	rm ./src/*.o
	rm ./src/tinyjpegdecoder/*.o
	rm ./src/sdl/*.o
	rm ${OUTPUT}
