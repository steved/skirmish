
[SCons]: http://www.scons.org/ "SCons build system"
[GSL]: http://www.gnu.org/software/gsl/ "GNU Scientific Library"
[SDL]: http://www.libsdl.org/ "SDL: Simple Direct Media Layer"
[MingW32]: http://www.mingw.org/ "Minimalist GNU for Windows"
[SDL_gfx]: http://www.ferzkopp.net/joomla/software-mainmenu-14/4-ferzkopps-linux-software/19-sdlgfx
[GnuWin32 GSL]: http://gnuwin32.sourceforge.net/packages/gsl.htm "GNU Scientific Library for Windows"
[SDL_ttf]: http://www.libsdl.org/projects/SDL_ttf/
[Python2]: http://python.org/download/

Install Dependencies
====================

Quick Install With Homebrew on Mac OS X 10.6
---

Build with [SCons][]

    $ brew install scons

Get scientific with [GSL][]

    $ brew install gsl

Get simple and direct with [SDL][]

    $ brew install sdl
    $ brew install sdl_gfx
    $ brew install sdl_ttf

Installing under Windows using MingW32
---

Install [MingW32][], following the installer.

Install [Python2][] as well as [SCons][]. (note: you may have to add  the Python Scripts directory to your Path variable)

Download the [GnuWin32 GSL][] binaries and developer files and place them appropriately under the MingW32 directory

    *.dll to /bin
    *.a to /lib
    *.h to /include

Download and install the [SDL][] mingw32 archive.

Compile the [SDL_gfx][] library by downloading the source, moving Makefile.mingw to Makefile and changing the $prefix variable inside.
Install the .dll, .a, and .h files under the MingW32 directory.

Download the [SDL_ttf][] VC development zip, rename .lib to .a and place them in the MingW32 directory.

Build
=====

    $ cd skirmish
    $ scons

When everything is done you should see:
    scons: done building targets.

Then start the executable:
    $ main

Control
=======

The arrows keys and mouse can be used to move around the map.

Page up / Scrollwheel up - Zoom in

Page down / Scrollwheel down - Zoom out

P - Pause

Left mouse click - (Un)Select unit / division

Right mouse click - (setup/game) Move selected unit(s) and (game) attack unit

(setup) R - regenerate map
