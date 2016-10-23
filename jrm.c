#include <unistd.h>

#include <err.h>

#include <sys/param.h>
#include <sys/jail.h>
#include <jail.h>

void failed(char fail, int status, const char *name, const char *msg,
		void (*warnf)(const char *fmt, ...),
		void (*errf)(int, const char *fmt, ...)) {
	if (fail) {
		errf(status, "%s: %s", name, msg);
	}
	else {
		warnf("%s: %s", name, msg);
	}
}

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
		if (jid == -1)
			failed(fail, status |= 2, *argv, jail_errmsg, warnx, errx);
		else if (jail_remove(jid) == -1)
			failed(fail, status |= 1, *argv, "jail_remove", warn, err);

		argv++;
	}

	return status;
}
