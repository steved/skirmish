$: << '/usr/lib/ruby/gems/1.9.1/gems/rubysdl-2.1.1/lib/'
require 'sdl'

def draw(surf)
  surf.draw_circle(WIDTH / 2, HEIGHT / 2, 1, [255, 0, 0], true)
end
