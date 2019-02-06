Termshot
========

Screenshot your terminal!

This is a simple ANSI-C application to save vtty screen memory as text file. Basically you can screenshot the terminal contents.

Only works for real vpty terminals (non-graphic mode), so xterm, gnome-terminal and stuff does not work!

x-term workaround
-----------------

In case you really want to screenshot a graphic terminal there is a workaround:

* Open a real vtty terminal (usually: CTRL+ALT+F1..F6)

* Start a terminal multiplexer (screen, tmux, etc...)

* Attach your xterm, gnome-terminal, st, or other terminal to the session

* Write stuff in your graphics terminal environment

* Whenever you need a "termshot", just give in the vtty you started tmux/screen from

But I have no idea why anyone would want to do this...

Known issues
------------

* Graphics terminals are not working

* Non-ascii characters - especially those non in the basic-multilingual-plane

Usage
-----

Build and install:

	make
	sudo make install

Simple usage:

	termshot 1        # to termshow vty1
	termshot          # to termshow current vty
	termshot --help   # to show help
