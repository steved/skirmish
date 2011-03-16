require 'interface/elements'

class StateInterface
  def initialize
    # Create a surface with a transparent black colorkey
    @overlay = SDL.CreateRGBSurface(SDL::SRCALPHA, WIDTH, HEIGHT, BPP, 0, 0, 0, 0)
    SDL.SetColorKey(@overlay, SDL::SRCCOLORKEY | SDL::RLEACCEL, 0xffffff)

    @elements = []
  end

  def render
    SDL.FillRect(@overlay, nil, 0xffffff)

    @elements.each do |element|
      element.draw_to(@overlay)
    end

    @overlay.to_ptr.address
  end

  def event(event)
  end

  def cleanup
  end
end
