.PHONY: all clean delobj copy

CFLAGS=-g -Wall
CC=arm-none-linux-gnueabi-gcc
AR=arm-none-linux-gnueabi-ar
RANLIB=arm-none-linux-gnueabi-ranlib
LIBS=-L/usr/local/sqlite3.08-arm/lib -L./ -lcgic -lsqlite3
DEPS=libcgic.a

all: login.cgi query.cgi setup.cgi refresh.cgi relay.cgi password.cgi log.cgi


login.cgi: login.o $(DEPS)
	$(CC) $(CFLAGS) login.o -o $@ $(LIBS)

query.cgi: query.o $(DEPS)
	$(CC) $(CFLAGS) query.o -o $@ $(LIBS)

setup.cgi: setup.o $(DEPS)
	$(CC) $(CFLAGS) setup.o -o $@ $(LIBS)

refresh.cgi:refresh.o $(DEPS)
	$(CC) $(CFLAGS) refresh.o -o $@ $(LIBS)

relay.cgi: relay.o $(DEPS)
	$(CC) $(CFLAGS) relay.o -o $@ $(LIBS)

password.cgi: password.o $(DEPS)
	$(CC) $(CFLAGS) password.o -o $@ $(LIBS)

log.cgi: log.o $(DEPS)
	$(CC) $(CFLAGS) log.o -o $@ $(LIBS)

clean:
	rm -f *.o *.cgi

delobj:
	rm -f *.o

copy:
	cp -f *.cgi /mnt/hgfs/share\ files/cgi-bin/
