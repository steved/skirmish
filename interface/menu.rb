require 'interface/state_interface'

class Menu < StateInterface
  def initialize
    super

    @elements << Box.new(120, 120, 20, 20, true)
    @elements << Box.new(420, 250, 20, 20)
  end

  def event(event)
  end
end
