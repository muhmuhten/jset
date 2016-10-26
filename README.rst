jset, jat, jrm - scriptable interfaces to jails
===============================================

Usage
-----

::

	jset [-CuAd] params... ["" prog...]
	jat jail prog...
	jrm [-f] jail...

Description
-----------

jset
~~~~

The ``jset`` utility is a wrapper for jail_set(2), using libjail's jailparam_*
interfaces to parse arguments from the command line.

``params...`` should consist of an even number of arguments alternating between
names and values, which are parsed by jailparam_init(3) and jailparam_import(3)
respectively, then passed together to jailparam_set(3) with flags dependent on
the following options:

* ``-c/-C``: add (default)/omit JAIL_CREATE.
* ``-u/-U``: add/omit (default) JAIL_UPDATE.
* ``-A/-A``: add (default)/omit JAIL_ATTACH.
* ``-d/-D``: add/omit (default) JAIL_DYING.

The default is ``-cUaD``, so normally only ``-CuAd`` are useful options. See
jail(8) for information on parameters and jail_set(2) for information on flags.

If ``prog...`` is specified, ``jset`` then chain-loads it using execvp(3).

Optionally, each argument in ``params...`` may be preceded by a single space,
which is ignored. (This allows the parameters to be in an execline_ block.)

.. _execline: http://skarnet.org/software/execline/

jat
~~~

The ``jat`` utility is a wrapper for jail_attach(2) using jail_getid(3). It is
generally redundant with jexec(8), except that it has much less functionality;
it doesn't bother shoddily implementing a subset of su(1)'s functionality...

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

No wrapper for jail_get(2) is provided; just use jls(8). Unlike for the other
three calls, there doesn't seem to be a single obvious interface for it. jls(8)
may be complex, but the complexity is mostly not superfluous.

why this
--------

jail(8) is unsuitable under process supervision. It *assumes double-forking*,
so it forks and expects the program it runs in the jail to exit successfully,
and doesn't forward signals or anything for a long-running process. It also
includes a lot of functionality which isn't needed when we have a run script.
It also isn't very scriptable, as its behaviour is determined in large part a
configuration file intended for humans, not programs, to edit.

``jset`` supersedes ``gaogao``, doing less ad-hoc parsing and requiring less
work to support new types of jail parameters as long as libjail is supported.

Caveats
-------

It works for me; it may or may not work for you. These programs are probably
for all intents and purposes final products: they will not need updating for
new features in the foreseeable future (parsing is handled by libjail), and
there is so little code that they are unlikely to contain bugs short of
fundamental design errors.

As of the time of this writing, jexec(8) was last updated 15 months ago, and
before that hadn't been touched in the past six years.

License
-------

ISC License, see COPYING.
