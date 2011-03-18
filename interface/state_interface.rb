require 'interface/elements'

class StateInterface
  def initialize(font)
    @elements = []
    @font = font
  end

  def render(surface)
    @elements.each do |element|
      element.draw_to(surface)
    end
  end

  def event(event)
  end

  def cleanup
  end
end
