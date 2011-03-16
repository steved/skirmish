require 'rubygems'
Gem.activate('ruby-sdl-ffi')
require 'ruby-sdl-ffi'

class StateInterface
  def initialize(*args)
  end

  def render
  end

  def event(event)
  end

  def cleanup
  end
end

class Setup < StateInterface
  def initialize(*args)
    # Create a surface with a transparent black colorkey
    @overlay = SDL.CreateRGBSurface(SDL::SRCALPHA, WIDTH, HEIGHT, BPP, 0, 0, 0, 0)
    SDL.SetColorKey(@overlay, SDL::SRCCOLORKEY | SDL::RLEACCEL, 0xffffff)
  end

  def render
    SDL.FillRect(@overlay, nil, 0xffffff)
    SDL::Gfx.filledCircleColor(@overlay, WIDTH / 2, HEIGHT / 2, 5, 0xff0000ff)

    @overlay.to_ptr.address
  end

  def cleanup
  end
end
