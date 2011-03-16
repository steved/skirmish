class Box
  def initialize(x, y, w, h, filled = false)
    @filled = filled
    @rect = SDL::Rect.new([x, y, w, h])
    @method = SDL::Gfx.method(filled ? :boxColor : :rectangleColor)
  end

  def draw_to(surface)
    @method.call(surface, @rect.x, @rect.y,
                 @rect.x + @rect.w, @rect.y + @rect.h, 0xff0000ff)
  end
end
