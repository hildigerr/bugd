
CC = gcc
CFLAGS = -g -Wall
LFLAGS = -l sqlite3
CONFIG = `pkg-config --cflags --libs gtk+-2.0`

SRCS = menu.c diagui.c datab.c bugd.c
# INSTALL_DIR = ~/PROGRAMING/bin/

bugd : ${SRCS}
	${CC} ${CFLAGS} ${SRCS} -o ${@} ${LFLAGS} ${CONFIG}

clean:
	rm bugd

# install: bugd
# 	cp bugd ${INSTALL_DIR}

