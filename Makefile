CC = gcc

CFLAGS = -Os -fdata-sections -ffunction-sections -Wall `sdl-config --cflags` -Isrc/tinyjpegdecoder -Isrc -Isrc/sdl -Isrc/ok_jpg -Isrc/lodepng -DNDEBUG
FILEBROWSER = 1

ifeq ($(FILEBROWSER),0)
CFLAGS += -DNO_FILEBROWSER 
SOURCES =
else ifeq ($(FILEBROWSER),1)
SOURCES = ./src/main.c 
endif

LDFLAGS = `sdl-config --libs` -lSDLmain -lSDL -Wl,--as-needed -Wl,--gc-sections -flto -s
OUTPUT = simpleimgviewer.elf

SOURCES += ./src/imgview.c
SOURCES += ./src/sdl/n2DLib.c ./src/sdl/control.c
SOURCES += ./src/ok_jpg/ok_jpg.c ./src/lodepng/lodepng.c
#SOURCES += ./src/tinyjpegdecoder/jidctflt.c ./src/tinyjpegdecoder/tinyjpeg.c ./src/tinyjpegdecoder/loadjpg.c
OBJS = ${SOURCES:.c=.o}

${OUTPUT}:${OBJS}
	${CC} -o ${OUTPUT} ${OBJS} ${CFLAGS} ${LDFLAGS} 
	
clean:
	rm ./src/*.o ./src/tinyjpegdecoder/*.o ./src/lodepng/*.o ./src/ok_jpg/*.o ./src/nsp/*.o /src/sdl/*.o
	rm ${OUTPUT} ${OUTPUT}.tns

