
CC = gcc
CFLAGS = -g -Wall
CONFIG = -l sqlite3 `pkg-config --cflags --libs gtk+-2.0`

SRCS = bugd.c
INSTALL_DIR = ~/PROGRAMING/bin/

bugd : ${SRCS}
	${CC} ${SRCS} -o ${@} ${CONFIG}

clean:
	rm bugd

install: bugd
	cp bugd ${INSTALL_DIR}

