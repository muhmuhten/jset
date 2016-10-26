/* Copyright (c) 2016, Michael Zuo. ISC License. */

#include <unistd.h>

#include <err.h>

#include <sys/param.h>
#include <sys/jail.h>
#include <jail.h>

#define ERRMSG_GETID	"%s: %s", *argv, jail_errmsg
#define ERRMSG_REMOVE	"%s: jail_remove", *argv

int main(int argc, char **argv) {
	int ch, jid;
	int status = 0;
	char fail = 0;

	if (argc < 1)
		errx(2, "not enough arguments");

	while ((ch = getopt(argc, argv, "fF")) != -1) {
		switch (ch) {
		case 'f': fail = 1; break;
		case 'F': fail = 0; break;
		default: errx(2, "usage: %s [-f] jail...", *argv);
		}
	}
	argc -= optind;
	argv += optind;

	while (*argv) {
		jid = jail_getid(*argv);
		if (jid == -1) {
			if (fail)
				errx(2, ERRMSG_GETID);
			else {
				status |= 2;
				warnx(ERRMSG_GETID);
			}
		}
		else if (jail_remove(jid) == -1) {
			if (fail)
				err(1, ERRMSG_REMOVE);
			else {
				status |= 1;
				warn(ERRMSG_REMOVE);
			}
		}

		argv++;
	}

	return status;
}
