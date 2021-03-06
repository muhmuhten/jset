/* Copyright (c) 2016, Michael Zuo. ISC License. */

#include <unistd.h>

#include <err.h>

#include <sys/param.h>
#include <sys/jail.h>
#include <jail.h>

int main(int argc, char **argv) {
	int jid;

	if (argc < 2)
		errx(2, "usage: %s jid|name prog...", *argv);

	jid = jail_getid(argv[1]);
	if (jid == -1)
		errx(1, "%s", jail_errmsg);

	if (jail_attach(jid) == -1)
		err(1, "jail_attach");

	if (!argv[2]) return 0;
	execvp(argv[2], argv+2);
	err(1, "%s", argv[2]);
}
