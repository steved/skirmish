#include "text.h"

int init_ttf() {
  if(TTF_Init() == -1) {
    return -1;
  }

  font = TTF_OpenFont("vera.ttf", 12);
  if(font == NULL) {
    printf("Couldn't open font vera.ttf\n");
    return -1;
  }

  return 0;
}

void close_ttf() {
  if(font != NULL) {
    TTF_CloseFont(font);
    font = NULL;
  }
}

SDL_Surface *draw_text(char *text) {
  SDL_Color color = {255, 255, 255};
  SDL_Surface *txt = TTF_RenderText_Blended(font, text, color);
  return txt;
}

int font_height() {
  return TTF_FontHeight(font);
}
