import glob
import os

# create build environment
env = Environment()

if os.name != "nt":
# determine compiler and linker flags for SDL
  env.ParseConfig('sdl-config --cflags')
  env.ParseConfig('sdl-config --libs')
  env.ParseConfig('gsl-config --cflags')
  env.ParseConfig('gsl-config --libs')
else:
# these settings are for mingw32 and my specific system,
# at the very least CPPPATH will need to be changed
  env.Append(LIBS = ['mingw32', 'SDLmain', 'SDL', 'gsl', 'gslcblas'])
  env.Append(CPPPATH = ['C:\Program Files\CodeBlocks\MinGW\include\SDL'])

# gather a list of source files
SOURCES = glob.glob('*.c') + glob.glob('**/*.c') + glob.glob('**/**/*.c')

# add additional compiler flags
env.Append(CFLAGS = ['-Wall', '-Werror', '--std=c99', '-g'])
# add additional libraries to link against
env.Append(LIBS = ['SDL_gfx', 'SDL_ttf'])
# add this directory to include path
env.Append(CPPPATH = '#')

# enable this to see nav_mesh + a* paths
# env.Append(CPPDEFINES='NAV_DEBUG')

# build target
# output executable will be "skirmish"
env.Program(target = 'skirmish', source = SOURCES)
