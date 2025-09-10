CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGETS = rm mkdir echo cat cp ls mv pwd rmdir clear head tail wc whoami
ALL_TARGETS = $(TARGETS) cls wam

all: $(TARGETS)

rm: rm.c
	$(CC) $(CFLAGS) -o $@ $<

mkdir: mkdir.c
	$(CC) $(CFLAGS) -o $@ $<

echo: echo.c
	$(CC) $(CFLAGS) -o $@ $<

cat: cat.c
	$(CC) $(CFLAGS) -o $@ $<

cp: cp.c
	$(CC) $(CFLAGS) -o $@ $<

ls: ls.c
	$(CC) $(CFLAGS) -o $@ $<

mv: mv.c
	$(CC) $(CFLAGS) -o $@ $<

pwd: pwd.c
	$(CC) $(CFLAGS) -o $@ $<

rmdir: rmdir.c
	$(CC) $(CFLAGS) -o $@ $<

clear: cls.c
	$(CC) $(CFLAGS) -o $@ $<

cls: cls.c
	$(CC) $(CFLAGS) -o $@ $<

head: head.c
	$(CC) $(CFLAGS) -o $@ $<

tail: tail.c
	$(CC) $(CFLAGS) -o $@ $<

wc: wc.c
	$(CC) $(CFLAGS) -o $@ $<

whoami: wam.c
	$(CC) $(CFLAGS) -o $@ $<

wam: wam.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(ALL_TARGETS)

install: all
	cp $(TARGETS) /usr/local/bin/

.PHONY: all clean install
