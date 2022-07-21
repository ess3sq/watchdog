CC=gcc
.PHONY: clean install uninstall help

watchdog: watchdog.c
	$(CC) watchdog.c -o watchdog

help:
	@echo -e "Makefile for watchdog.\nValid targets: clean install uninstall help"
	
clean:
	rm -f watchdog

install: watchdog
	chmod +x watchdog
	mv watchdog /usr/bin/watchdog

uninstall:
	rm -f /usr/bin/watchdog

