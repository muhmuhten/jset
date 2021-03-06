jset, jat, jrm - scriptable interfaces to jails
===============================================

Usage
-----

::

	jset [-cuad] params... ["" prog...]
	jat jail prog...
	jrm [-f] jail...

Description
-----------

jset
~~~~

The ``jset`` utility is a wrapper for jail_set(2), using libjail's jailparam_*
interfaces to parse arguments from the command line. ``jset`` *does not* call
chdir(2), even a path parameter is present; it behaves more like the chroot(2)
system call than the utility of the same name.

``params...`` should consist of an even number of arguments alternating between
names and values, which are parsed by jailparam_init(3) and jailparam_import(3)
respectively, then passed together to jailparam_set(3) with flags set according
to the following options:

* ``-c``: add JAIL_CREATE.
* ``-u``: add JAIL_UPDATE.
* ``-a``: add JAIL_ATTACH.
* ``-d``: add JAIL_DYING.

By default, no flags are set. Typical use to create a new jail and immediately
enter it requires ``-ca``. See jail(8) for more information on parameters and
jail_set(2) for more information on these flags.

If ``prog...`` is specified, ``jset`` then chain-loads it using execvp(3).

Optionally, each argument in ``params...`` may be preceded by a single space,
which is ignored. (This allows the parameters to be in an execline_ block.)

.. _execline: http://skarnet.org/software/execline/

jat
~~~

The ``jat`` utility is a wrapper for jail_attach(2) using jail_getid(3). It is
generally redundant with jexec(8), except that it has much less functionality;
it doesn't bother shoddily implementing a subset of su(1)'s functionality...

``jat`` *does not* call chdir(2), which may create security vulnerabilities.

Generally, ``jset -ua`` is also sufficient to accomplish anything which could
be done with ``jat``, but can also modify the jail's parameters.

jrm
~~~

The ``jrm`` utility is a wrapper for jail_remove(2) using jail_getid(3). It is
generally redundant with jail(8), which does much the same thing when called as
``jail -R ...``, except that it has much less functionality. (This is probably
a good thing, because jail(8) does more work than it needs to, maybe, depending
on configuration.) It tries to remove each jail specified on the command line
in order. Its behaviour on failure is determined by its options:

* ``-f``: ``jrm`` will exit immediately on failure.
* ``-F``: ``jrm`` will emit a warning, but continue attempting to remove any
  other jails specified on the command line, then exit with a status indicating
  the error. This is the default.

jail_get(2)
~~~~~~~~~~~

No wrapper for jail_get(2) is provided; I have not been able to design a clean,
obvious interface to it. Just use jls(8).

why this
--------

jail(8) is unsuitable under process supervision. It *assumes double-forking*,
so it forks and expects the program it runs in the jail to exit successfully,
and doesn't forward signals or anything for a long-running process. It also
includes a lot of functionality which isn't needed when we have a run script.
It also isn't very scriptable, as its behaviour is determined in large part a
configuration file intended for humans, not programs, to edit.

``jset`` supersedes ``gaogao``, reducing ad-hoc parsing by delegating that to
libjail. As such, it should also require less work to support jail parameters
with new types, as libjail will usually be updated alongside the host kernel.

Caveats
-------

It suffices for me; it may or may not work for you.

These programs may be treated for all intents and purposes final products, as
they are designed to avoid needing updates for new features in the foreseeable
future (parsing is handled by libjail), and there is so little code that they
are unlikely to contain bugs short of fundamental design errors.

It should be noted that jail_attach(2) and jail_set(2), like chroot(2), have no
effect on the process's current directory. For best results, always enter jails
from the inside. (It's not safe to invoke the jail's own binaries to chdir(2)
after entering the jail if the contents are untrusted, since those binaries may
be doctored to do evil outside the jail path.) This is not "fixable", as a jail
need not have the path parameter, and it would be tedious to change to the root
directory after, say, creating a jail just to disable network access, but also
aesthetically intrusive to inspect the command for specific parameters.

License
-------

ISC License, see COPYING.
