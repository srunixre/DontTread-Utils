CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lm

TARGETS = rm mkdir echo cat cp ls mv touch pwd rmdir head tail wc whoami clear
ALL_TARGETS = $(TARGETS) cls wam

all: $(ALL_TARGETS)

rm: rm.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

mkdir: mkdir.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

echo: echo.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

cat: cat.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

cp: cp.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

ls: ls.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

mv: mv.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

touch: touch.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

pwd: pwd.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

rmdir: rmdir.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

head: head.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

tail: tail.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

wc: wc.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

whoami: wam.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

wam: wam.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clear: cls.c
	$(CC) $(CFLAGS) -o $@ $<

cls: cls.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(ALL_TARGETS)

install: all
	cp $(TARGETS) /usr/local/bin/
	cp cls /usr/local/bin/
	cp wam /usr/local/bin/

.PHONY: all clean install
