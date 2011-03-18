class Element
  def initialize(x, y, w, h, color = SDL::Color.new([0xff, 0xff, 0xff, 0]), alpha = 0xff)
    @position = SDL::Rect.new([x, y, w, h])
    @color = color
    @alpha = alpha
  end
end

class Box < Element
  def initialize(x, y, w, h, filled = false, color = SDL::Color.new([0xff, 0, 0, 0xff]), alpha = 0xff)
    super(x, y, w, h, color, alpha)

    @filled = filled
    @method = SDL::Gfx.method(filled ? :boxRGBA : :rectangleRGBA)
  end

  def draw_to(surface)
    @method.call(surface, @position.x, @position.y,
                 @position.x + @position.w, @position.y + @position.h, 
                 @color.r, @color.g, @color.b, @alpha)
  end
end

class CenteredBox < Box
  def initialize(x, y, w, h, filled = false, color = SDL::Color.new([0xff, 0, 0, 0xff]), alpha = 0xff)
    super(x - (w / 2), y - (h / 2), w, h, filled, color, alpha)
  end
end

# Renders text centered inside the box defined by x, y, w, h
class CenteredText < Element
  def initialize(font, x, y, w, h, text, color = SDL::Color.new([0xff, 0xff, 0xff, 0]))
    @font, @text = font, text
    width, height = SDL::TTF.SizeText(font, text)

    super(x + w / 2 - width / 2, y + h / 2 - height / 2, 0, 0, color)
  end

  def draw_to(surface)
    SDL.BlitSurface(@text_surface, nil, surface, @position) 
  end

  def color=(color)
    @color = color
    @text_surface = SDL::TTF.RenderText_Blended(@font, @text, @color)
  end
end

class MenuItem < Box
  attr_accessor :selected

  def initialize(font, x, y, w, h, text, color = SDL::Color.new([0xff, 0xff, 0xff, 0]))
    super(x, y, w, h, true, color)

    @text = CenteredText.new(font, x, y, w, h, text)
    @selected = false
  end

  def draw_to(surface)
    if @selected
      super(surface)
      @text.color = SDL::Color.new([0, 0, 0, 0])
    else
      @text.color = SDL::Color.new([0xff, 0xff, 0xff, 0])
    end
    @text.draw_to(surface)
  end
end
