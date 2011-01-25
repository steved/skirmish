import glob

# create build environment
env = Environment()

# determine compiler and linker flags for SDL
env.ParseConfig('sdl-config --cflags')
env.ParseConfig('sdl-config --libs')
env.ParseConfig('gsl-config --cflags')
env.ParseConfig('gsl-config --libs')

# gather a list of source files
SOURCES = glob.glob('*.c')

# add additional compiler flags
env.Append(CFLAGS = ['-Wall', '--std=c99', "-g"])
# add additional libraries to link against
env.Append(LIBS = ['SDL_gfx', 'SDL_ttf'])

# build target
# output executable will be "game"
env.Program(target = 'main', source = SOURCES)
