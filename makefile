
CC = gcc
CFLAGS = -g -Wall
LFLAGS = -l sqlite3
CONFIG = `pkg-config --cflags --libs gtk+-2.0 libconfuse`

SRCS = src/menu.c src/diagui.c src/datab.c src/bugd.c src/config.c
INSTALL_DIR = ${DESTDIR}/usr/bin/
MAN_INSTALL_DIR = ${DESTDIR}/usr/share/man/man1/
ICON_INSTALL_DIR = ${DESTDIR}/usr/share/icons/
MENU_INSTALL_DIR = ${DESTDIR}/usr/share/applications/
CONF_INSTALL_DIR = ${DESTDIR}/etc/

bugd : ${SRCS}
	${CC} ${SRCS} -o ${@} ${LFLAGS} ${CONFIG}

debug : ${SRCS}
	${CC} ${CFLAGS} ${SRCS} ${LFLAGS} ${CONFIG}

man:
	gzip -c doc/bugd.1 > bugd.1.gz

install: bugd man
	@ mkdir -p ${INSTALL_DIR}
	@ mv bugd ${INSTALL_DIR}
	@ mkdir -p ${MAN_INSTALL_DIR}
	@ mv bugd.1.gz ${MAN_INSTALL_DIR}
	@ mkdir -p ${ICON_INSTALL_DIR}
	@ cp icons/bugd-icon.png ${ICON_INSTALL_DIR}bugd.png
	@ mkdir -p ${MENU_INSTALL_DIR}
	@ cp BugD.desktop ${MENU_INSTALL_DIR}
	@ mkdir -p ${CONF_INSTALL_DIR}
	@ cp doc/bugd.cfg ${CONF_INSTALL_DIR}

clean:
	rm -f bugd bugd.1.gz a.out core

${SRCS} : src/bugd.h
src/config.c : src/config.h
