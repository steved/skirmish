require 'interface/state_interface'

class Menu < StateInterface
  def initialize(font)
    super(font)

    @elements << CenteredBox.new(WIDTH / 2, HEIGHT / 2, WIDTH / 2, HEIGHT / 2, true, SDL::Color.new([0xff, 0xff, 0xff, 0]), 40)
    @elements << CenteredBox.new(WIDTH / 2, HEIGHT / 2, WIDTH / 2, HEIGHT / 2, false, SDL::Color.new([0xff, 0, 0, 0]))

    unless font.nil?
      @font_height = SDL::TTF.FontHeight(font)
      @selected = 0
      @max = HEIGHT / 2 / @font_height

      @menu_items = []
      add_menu_item("Start Game") do
      end
      add_menu_item("Hello")
      add_menu_item("Hello")
      add_menu_item("Hello")
      add_menu_item("Hello")
    end
  end

  def render(surface)
    super(surface)

    @menu_items.each do |menu_item|
      menu_item.draw_to(surface)
    end
  end

  def add_menu_item(text, &callback)
    unless @font.nil? || @menu_items.length >= @max 
      index = @menu_items.length
      start_x, start_y = WIDTH / 2 - WIDTH / 4, HEIGHT / 2 - HEIGHT / 4

      menu_item = MenuItem.new(@font, start_x, start_y + (index * @font_height), WIDTH / 2, @font_height, text)
      menu_item.selected = true if index == 0

      @menu_items << menu_item
    end
  end

  def event(event)
    if !@font.nil? && event[:type] == SDL::KEYDOWN
      sym = event.keysym.sym
      @menu_items[@selected].selected = false 
      if sym == SDL::K_DOWN
        @selected += 1 if @selected < @menu_items.length - 1
      elsif sym == SDL::K_UP
        @selected -= 1 if @selected > 0
      end
      @menu_items[@selected].selected = true
    end
  end
end

