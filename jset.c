/* Copyright (c) 2016, Michael Zuo. ISC License. */

#include <unistd.h>

#include <err.h>

#include <sys/param.h>
#include <sys/jail.h>
#include <jail.h>

int main(int argc, char **argv) {
	int ch, flags = JAIL_CREATE|JAIL_ATTACH;
	unsigned njp = 0;
	struct jailparam jp[argc/2]; /* maximum used if all args are params */

	if (argc < 1)
		errx(2, "not enough arguments");

	while ((ch = getopt(argc, argv, "cCuUaAdD")) != -1) {
		switch (ch) {
		case 'c': flags |= JAIL_CREATE;  break;
		case 'u': flags |= JAIL_UPDATE;  break;
		case 'a': flags |= JAIL_ATTACH;  break;
		case 'd': flags |= JAIL_DYING;   break;
		case 'C': flags &= ~JAIL_CREATE; break;
		case 'U': flags &= ~JAIL_UPDATE; break;
		case 'A': flags &= ~JAIL_ATTACH; break;
		case 'D': flags &= ~JAIL_DYING;  break;
		default: errx(2, "usage: %s [-CUad] params... '' prog...", *argv);
		}
	}
	argc -= optind;
	argv += optind;

	while (*argv && **argv) {
		if (!argv[1] || !*argv[1])
			errx(2, "unpaired parameter: %s", argv[0]);

		if (argv[0][0] == ' ')
			argv[0]++;
		if (jailparam_init(jp+njp, argv[0]) == -1)
			errx(2, "%s", jail_errmsg);

		if (argv[1][0] == ' ')
			argv[1]++;
		if (jailparam_import(jp+njp, argv[1]) == -1)
			errx(2, "%s", jail_errmsg);

		njp++;
		argv += 2;
	}

	if (jailparam_set(jp, njp, flags) == -1)
		errx(1, "%s", jail_errmsg);
	/* no need to deallocate */

	if (!*argv || !*++argv) return 0;
	execvp(*argv, argv);
	err(1, "%s", *argv);
}
