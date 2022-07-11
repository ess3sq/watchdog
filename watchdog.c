#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

#include <sys/inotify.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

int fd;
int watch_mask = IN_CLOSE_WRITE |
	IN_CREATE | IN_DELETE | IN_DELETE_SELF |
	IN_MOVE_SELF | IN_MOVED_FROM | IN_MOVED_TO;

bool ready = true;
char *command = ":";
bool verbose = false;

void
print_help(FILE *stream)
{
	fprintf(stream,
			"watchdog -- watch files for changes\n\n"
			"    usage:\n"
			"        watchdog [OPTIONS [ARGS...] ...] <FILE> [FILES...]\n"
			"    options:\n"
			"        --help             display help and exit\n"
			"        --command -c [arg] run command on file change\n"
			"        --verbose -v       Announce file changes\n");
}

void run_command(void)
{
	errno = 0;
	int status = system(command);
	if (errno && status < 0) {
		fprintf(stderr,
				"warning: could not execute '%s' - %s.\n",
				command,
				strerror(errno));
	}
}

void
end_watch(int signal)
{
	fprintf(stderr, "Shutting down watchdog...\n");
	close(fd);
	exit(0);
}

void
start_watch(void)
{
	struct inotify_event ev;
	
	do {
		if (ready && read(fd, &ev, sizeof(ev)) > 0) {
			if (ev.mask) {
				if (verbose) {
					printf("Detected change\n");
				}
				
				ready = false;
				run_command();
				ready = true;
			} 
		}
	} while (1);
}

int
main(int argc, char **argv)
{
	int nfiles = 0;

	bool awaiting_cmd = false;

	if ((fd = inotify_init()) < 0) {
		fprintf(stderr, "failed to call inotify_init()\n");
		exit(1);
	}
	
	for (int i = 1; i < argc; ++i) {
		if (awaiting_cmd) {
			command = argv[i];
			awaiting_cmd = false;
			continue;
		}
		else if (*argv[i] == '-') {
			if (!strcmp(argv[i], "--help")) {
				print_help(stderr);
				exit(2);
			}
			else if (!strcmp(argv[i], "--verbose") || !strcmp(argv[i], "-v")) {
				verbose = true;
			}
			else if (!strcmp(argv[i], "--command") || !strcmp(argv[i], "-c")) {
				awaiting_cmd = true;
			}
			continue;
		}

		if (inotify_add_watch(fd, argv[i], watch_mask) < 0) {
			fprintf(stderr,
					"Failed to add '%s' to the watchlist - %s.",
					argv[i],
					strerror(errno));
			exit(1);
		}
		nfiles++;
	}

	if (nfiles ==  0) {
		fprintf(stderr, "Failed to specify any files to watch.\n");
		exit(1);
	}

	signal(SIGTERM, &end_watch);
	signal(SIGINT, &end_watch);
	signal(SIGABRT, &end_watch);
	start_watch();
	end_watch(0);
}
