[SCons]: http://www.scons.org/ "SCons build system"
[GSL]: http://www.gnu.org/software/gsl/ "GNU Scientific Library"
[SDL]: http://www.libsdl.org/ "SDL: Simple Direct Media Layer"


Install Dependencies
====================

Quick Install With Homebrew on Mac OS X 10.6

Build with [SCons][]

    $ brew install scons

Get scientific with [GSL][]

    $ brew install gsl

Get simple and direct with [SDL][]

    $ brew install sdl
    $ brew install sdl_gfx
    $ brew install sdl_ttf


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

Left mouse click - (Un)Select unit
