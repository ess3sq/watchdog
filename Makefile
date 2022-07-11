CC=gcc
.PHONY: clean install

watchdog: watchdog.c
	$(CC) watchdog.c -o watchdog

clean:
	rm watchdog

install: watchdog
	chmod +x watchdog
	mv watchdog /usr/bin/watchdog

