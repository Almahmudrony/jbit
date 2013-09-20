JBit
====

If you are looking for the mobile (J2ME) version of JBit, you can find
it here:

<http://jbit.sourceforge.net> (XHTML site, optimized for feature phones)

or here:

<http://jbit.sourceforge.net/m> (WAP site, for old phones / very low bandwidth)

This version of JBit targets desktops and browsers.

Compiling
---------

The following should work (tested mostly on debian, but occasionally on
NetBSD):

	cd native
	make

If you are missing ncurses (or have curses), comment out the offending
lines in the Makefile (or edit them). The (n)curses library is needed
for the microio device, and, unless you are coming from the mobile
version of JBit, you are unlikely to need it.

A precompiled EXE for Windows is also available (see the wiki below).

Testing
-------

The resulting binary (jbit) is self-contained, so you should be able to
run it from anywhere.  From now on, I assume that you have it in your
path.

Switch to the sample directory and type:

	jbit hello.asm

You should get back the
[usual message](http://en.wikipedia.org/wiki/Hello_world_program):

	Hello, World!

JBit supports a binary format, if you really need it:

	jbit -c jb hello.asm >hello.jb
	jbit hello.jb

Getting Started
---------------

You can find plenty of information on the
[6502](http://en.wikipedia.org/wiki/MOS_Technology_6502) on the
Internet.  There is a also a
[Beginner's Tutorial](http://jbit.sourceforge.net//doc/tutorial_en.html)
on how to start 6502 programming with JBit (J2ME), but here I assume you
are familiar with 6502 assembly and new to JBit.

The CPU in JBit is pretty similar to a regular 6502. The main difference
is that it lacks BCD mode and interrupts. Code is loaded starting from
page 3 and a "IO chip" is mapped on page 2.

Type and run:

	$a9 'A' $8d $00 $02

Register $0200 is PUTCHAR and when you write a character into it,
that character is sent to stdout.

Since its roots in feature phones, the prefered notation in JBit is
*decimal*:

	169 'A' 141 0 2

To work around clean numbers ($C000) becoming magic numbers (49152),
JBit uses the non-standard notation *page:offset* for decimal words:

	169 'A' 141 2:0
	
You don't need to remember the location of the IO registers. If you
select a device, you get some constants defined:

	.device "stdout"
	169 'A' 141 PUTCHAR

You can of course write in assembly instead:

	.device "stdout"
	lda #'A'
	sta PUTCHAR

stdout
------

The stdout device is very simple and is the only device also available
on Windows.

To see what symbols are defined for a device, run:

	jbit -s stdout

You should get:

	PUTCHAR 512
	FRMFPS 529
	FRMDRAW 530
	PUTUINT8 534

You know PUTCHAR. PUTUINT8 prints a decimal number (0-255). Writing
anything into FRMDRAW calls fflush(stdout). If FRMFPS is 0 (the
default), FRMDRAW doesn't wait. If FRMFPS is != 0, FRMDRAW waits to keep
a "refresh rate" of FRMFPS / 4 frames per seconds. For example, this
program:

	.device "stdout"
	      lda #4
	      sta FRMFPS
	      lda #'X'
	loop: sta PUTCHAR
	      sta FRMDRAW
	      jmp loop

keeps printing one X every second, until you press Ctrl-C.

More information about the assembly format is in the samples/format
directory.

xv65
----

The device xv65 maps a subset of the traditonal Unix V6 API (fork, exec,
pipe, etc...).  It was inspired by the beautiful
[xv6](http://pdos.csail.mit.edu/6.828/2012/xv6.html).

microio
-------

A couple of jb files for the microio device are included. Since the jb
format does not include device information, you need tell jbit to use
the microio device when you start it:

	jbit -d microio charset.jb

JavaScript
----------

The JavaScript version uses
[emscripten](https://github.com/kripken/emscripten), but it's not
working at the moment. You can see an older version running here:

<http://jbit.sourceforge.net/gallery/list.php?tag=microio>

Browse the gallery and click on *sim* to run the sample on your browser
and *jb* to download it to run it on the native simulator.

More info
---------

<https://github.com/efornara/jbit/wiki>
