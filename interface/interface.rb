require 'rubygems'
Gem.activate('ruby-sdl-ffi')
require 'ruby-sdl-ffi'

# returns the address of the SDL_Surface struct
def draw
  overlay = SDL.CreateRGBSurface(SDL::SRCALPHA, WIDTH, HEIGHT, BPP, 0, 0, 0, 0)
  SDL.SetColorKey(overlay, SDL::SRCCOLORKEY | SDL::RLEACCEL, 0xffffff)
  # Create a surface with a transparent black colorkey
  SDL.FillRect(overlay, nil, 0xffffff)
  SDL::Gfx.filledCircleColor(overlay, WIDTH / 2, HEIGHT / 2, 5, 0xff0000ff)

  overlay.to_ptr.address
end
