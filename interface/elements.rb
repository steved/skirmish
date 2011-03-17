class Box
  def initialize(x, y, w, h, filled = false, color = SDL::Color.new([0xff, 0, 0, 0xff]))
    @position = SDL::Rect.new([x, y, w, h])
    @color = color
    @filled = filled
    @method = SDL::Gfx.method(filled ? :boxRGBA : :rectangleRGBA)
  end

  def draw_to(surface)
    @method.call(surface, @position.x, @position.y,
                 @position.x + @position.w, @position.y + @position.h, 
                 @color.r, @color.g, @color.b, 0xff)
  end
end

class CenteredBox < Box
  def initialize(x, y, w, h, filled = false, color = SDL::Color.new([0xff, 0, 0, 0xff]))
    super(x - (w / 2), y - (h / 2), w, h, filled, color)
  end
end
