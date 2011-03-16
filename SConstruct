import glob
import os
import commands

def get_ruby_config(key):
  return commands.getoutput('ruby -rrbconfig -e "puts Config::CONFIG[\'%s\']"' % key)

# create build environment
env = Environment()

have_ruby = commands.getstatusoutput('ruby -v')[0]
if have_ruby == 0:
  hdr_dir = get_ruby_config('rubyhdrdir')
  if hdr_dir == 'nil': # 1.8
    env.Append(CPPPATH = get_ruby_config('archdir'))
    env.Append(CPPDEFINES = ['RUBY_1_8'])
  else:
    env.Append(CPPPATH = [hdr_dir, "%s/%s" % (hdr_dir, get_ruby_config('arch'))])
    env.Append(CPPDEFINES = ['RUBY_1_9'])

  env.Append(LIBPATH = get_ruby_config('libdir'))
  env.Append(LIBS = get_ruby_config('LIBRUBYARG_SHARED'))
  env.Append(CPPDEFINES = ['HAVE_RUBY'])
else:
  print "Ruby is not installed, some features will not be enabled."


if os.name != "nt":
# determine compiler and linker flags for SDL
  env.ParseConfig('sdl-config --cflags')
  env.ParseConfig('sdl-config --libs')
  env.ParseConfig('gsl-config --cflags')
  env.ParseConfig('gsl-config --libs')

  sdl_exists = commands.getstatusoutput('pkg-config --exists SDL_gfx')[0]
  if sdl_exists == 0:
    env.ParseConfig('pkg-config SDL_gfx --cflags')
    env.ParseConfig('pkg-config SDL_gfx --libs')
  else:
    env.Append(LIBS = 'SDL_gfx')

  sdl_exists = commands.getstatusoutput('pkg-config --exists SDL_ttf')[0]
  if sdl_exists == 0:
    env.ParseConfig('pkg-config SDL_ttf --cflags')
    env.ParseConfig('pkg-config SDL_ttf --libs')
  else:
    env.Append(LIBS = 'SDL_ttf')
else:
# these settings are for mingw32 and my specific system,
# at the very least CPPPATH will need to be changed
  env.Append(LIBS = ['mingw32', 'SDLmain', 'SDL', 'gsl', 'gslcblas', 'SDL_ttf', 'SDL_gfx'])
  env.Append(CPPPATH = ['C:\Program Files\CodeBlocks\MinGW\include\SDL'])

# gather a list of source files
SOURCES = glob.glob('*.c') + glob.glob('**/*.c') + glob.glob('**/**/*.c')

# add additional compiler flags
env.Append(CFLAGS = ['-Wall', '-Werror', '--std=c99', '-g'])
# add this directory to include path
env.Append(CPPPATH = '#')

# enable this to see nav_mesh + a* paths
# env.Append(CPPDEFINES = ['NAV_DEBUG'])

# build target
# output executable will be "skirmish"
env.Program(target = 'skirmish', source = SOURCES)
