#include "constants.h"

// variable init
uint32_t last_time = 0;
SDL_Window* w;
SDL_Renderer* r;
bool running = true;
float win_x = INITIAL_X;
float win_y = INITIAL_Y;
SDL_DisplayMode display;
int cur_x_direction = 1, cur_y_direction = 1;

void init(){
  if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    fprintf(stderr, "E: Couldn't initialize SDL\n");

  w = SDL_CreateWindow(
      TITLE,
      win_x,
      win_y,
      SCR_W, SCR_H,
      SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS
      );

  if(w == 0)
    fprintf(stderr, "E: Couldn't initialize SDL window\n");

  r = SDL_CreateRenderer(w, -1, 0);

  SDL_GetCurrentDisplayMode(0, &display);

  IMG_Init(IMG_INIT_JPG);
}

void keydown(SDL_Event e){
  switch(e.key.keysym.sym){
    case SDLK_ESCAPE:
      running = false;
      break;
  }
}

void process_input(){
  SDL_Event e;

  while(SDL_PollEvent(&e)){
    switch(e.type){
      case SDL_KEYDOWN:
        keydown(e);
        break;
      case SDL_QUIT:
        running = false;
        return;
      default:
        break;
    }
  }
}

void update(){
  uint32_t now = SDL_GetTicks();
  float deltaT = (now - last_time) / 1000.0f;
  last_time = now;

  win_x += cur_x_direction * SPEED * deltaT;
  win_y += cur_y_direction * SPEED * deltaT;

  if(win_x <= 0){
    win_x = 1;
    cur_x_direction = 1;
  }

  if(win_y <= 0){
    win_y = 1;
    cur_y_direction = 1;
  }

  if(win_x + SCR_W >= display.w){
    win_x = display.w - (SCR_W + 1);
    cur_x_direction = -1;
  }

  if(win_y + SCR_H >= display.h){
    win_y = display.h - (SCR_H + 1);
    cur_y_direction = -1;
  }

  SDL_SetWindowPosition(w, win_x, win_y);
}

void getSpriteTexture(SDL_Renderer* r, const char* filepath, SDL_Rect rect){
  SDL_Surface* surface = IMG_Load(filepath);
  SDL_Texture* texture = SDL_CreateTextureFromSurface(r, surface);
  SDL_FreeSurface(surface);
  SDL_RenderCopy(r, texture, NULL, &rect);
  SDL_DestroyTexture(texture);
}

void render(){
  SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
  SDL_RenderClear(r);

  SDL_Rect dvd = {0, 0, SCR_W, SCR_W};
  getSpriteTexture(r, "./img/dvd.jpeg", dvd);

  SDL_RenderPresent(r);
}

void quit(){
  SDL_DestroyRenderer(r);
  SDL_DestroyWindow(w);
  SDL_Quit();
}

int main(void){
  // window and renderer init
  init();

  while(running){
    process_input();
    update();
    render();
  }

  quit();

  return 0;
}
