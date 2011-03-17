require 'interface/state_interface'

class Menu < StateInterface
  def initialize
    super

    @elements << Box.new(rand(WIDTH), rand(HEIGHT), 20, 20, true)
    @elements << Box.new(rand(WIDTH), rand(HEIGHT), 20, 20)
    @elements << Box.new(WIDTH / 2, HEIGHT / 2, 20, 20, true, SDL::Color.new([0, 0xff, 0, 0xff]))
    @elements << Box.new(300, 40, 20, 20, false, SDL::Color.new([0, 0, 0xff, 0xff]))
    @elements << CenteredBox.new(WIDTH / 2, HEIGHT / 2, WIDTH / 2, HEIGHT / 2)
  end

  def event(event)
  end
end
