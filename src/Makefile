CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lm
BINDIR = bin

TARGETS = rm mkdir echo cat cp ls mv touch pwd rmdir head tail wc whoami clear
ALL_TARGETS = $(addprefix $(BINDIR)/, $(TARGETS) cls wam)

all: $(BINDIR) $(ALL_TARGETS)

$(BINDIR):
	mkdir -p $(BINDIR)

$(BINDIR)/rm: rm.c | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(BINDIR)/mkdir: mkdir.c | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(BINDIR)/echo: echo.c | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(BINDIR)/cat: cat.c | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(BINDIR)/cp: cp.c | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(BINDIR)/ls: ls.c | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(BINDIR)/mv: mv.c | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(BINDIR)/touch: touch.c | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(BINDIR)/pwd: pwd.c | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(BINDIR)/rmdir: rmdir.c | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(BINDIR)/head: head.c | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(BINDIR)/tail: tail.c | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(BINDIR)/wc: wc.c | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(BINDIR)/whoami: wam.c | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(BINDIR)/wam: wam.c | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(BINDIR)/clear: cls.c | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $<

$(BINDIR)/cls: cls.c | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -rf $(BINDIR)

install: all
	sudo cp $(BINDIR)/* /bin/

.PHONY: all clean install