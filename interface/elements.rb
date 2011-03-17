class Box
  def initialize(x, y, w, h, filled = false, color = SDL::Color.new([0xff, 0, 0, 0xff]))
    @filled = filled
    @color = color
    @rect = SDL::Rect.new([x, y, w, h])
    @method = SDL::Gfx.method(filled ? :boxRGBA : :rectangleRGBA)
  end

  def draw_to(surface)
    @method.call(surface, @rect.x, @rect.y,
                 @rect.x + @rect.w, @rect.y + @rect.h, 
                 @color.r, @color.g, @color.b, 0xff)
  end
end

class CenteredBox < Box
  def initialize(x, y, w, h, filled = false, color = SDL::Color.new([0xff, 0, 0, 0xff]))
    super(x - (w / 2), y - (h / 2), w, h, filled, color)
  end
end
