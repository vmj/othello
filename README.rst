othello -- OpenGL Othello game
******************************

Othello is a board game for two players.

Objective of the game is to conquer the board with your color. Players
place disks on the board in turn such that they trap at least one of
the opponents' disks between their own disks. Those trapped disks are
turned upside down so that they change color. If the player can't
place a disk on the board, shift is transfered to the opponent. If
neither player can play, game is over.  Player with more disks is the
winner.

| Homepage: http://linuxbox.fi/~vmj/othello/
| Download: http://linuxbox.fi/~vmj/othello/archive/
| Source code: http://github.com/vmj/othello

.. contents::


Basic usage
===========

You can launch the game from the command line:

    $ othello

See `othello(1)`_ for more information on usage and configuration.

.. _othello(1): http://www.linuxbox.fi/~vmj/othello/othello.1.html

.. image:: http://linuxbox.fi/~vmj/othello/img/othello.png


Requirements
============

The 'make' command, a C compiler, standard C libraries and headers,
and OpenGL libraries and headers, including the GLU and GLUT.

On Debian and derivates, you can get all of those by issuing command
'sudo apt-get install libc6-dev freeglut3-dev'.


Installation
============

Type 'make install' to install the othello into '/usr/local/bin' and
manual page to '/usr/local/man'.

You can also define PREFIX to install othello in a different prefix:
'make PREFIX=/usr install' for example.  Similarly, there's DESTDIR,
BINDIR, MANDIR, and MAN1DIR for those who need them.


Authors
=======

Original author and current maintainer is Mikko Värri
(vmj@linuxbox.fi).


License
=======

othello is Free Software, licensed under GNU General Public License
(GPL), version 3 or later.  See LICENSE.txt file for details.
