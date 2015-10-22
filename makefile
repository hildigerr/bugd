
CC = gcc
CFLAGS = -g -Wall
LFLAGS = -l sqlite3
CONFIG = `pkg-config --cflags --libs gtk+-2.0`

SRCS = src/menu.c src/diagui.c src/datab.c src/bugd.c
INSTALL_DIR = ${DESTDIR}/usr/bin/
MAN_INSTALL_DIR = ${DESTDIR}/usr/share/man/man1/
ICON_INSTALL_DIR = ${DESTDIR}/usr/share/icons/
MENU_INSTALL_DIR = ${DESTDIR}/usr/share/applications/

bugd : ${SRCS}
	${CC} ${SRCS} -o ${@} ${LFLAGS} ${CONFIG}

debug : ${SRCS}
	${CC} ${CFLAGS} ${SRCS} ${LFLAGS} ${CONFIG}

man:
	gzip -c doc/bugd.1 > bugd.1.gz

install: bugd man
	@ mv bugd ${INSTALL_DIR}
	@ mv bugd.1.gz ${MAN_INSTALL_DIR}
	@ cp icons/bugd-icon.png ${ICON_INSTALL_DIR}bugd.png
	@ cp BugD.desktop ${MENU_INSTALL_DIR}


${SRCS} : src/bugd.h
