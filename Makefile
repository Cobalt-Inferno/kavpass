CC := gcc
FILE := kavpass.c
CFLAGS := -march=native -Os -Wall


install:
	$(CC) $(CFLAGS) $(FILE) -o kavpass
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f kavpass ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/kavpass

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/kavpass
