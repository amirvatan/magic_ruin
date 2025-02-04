#include "time.h"
#include <SDL2/SDL_timer.h>
void time_init(f32 frame_rate, f32 *frame_delay) {
  *frame_delay = (1000.0 / frame_rate);
}
void time_update(f32 *time_last, f32 *delta) {

  f32 time_now = SDL_GetTicks();
  *delta = (time_now - *time_last) / 1000.0;
  *time_last = time_now;
}

void time_update_late(f32 *frame_delay, f32 *time_last) {
  f32 frame_time = SDL_GetTicks() - *time_last;
  if (*frame_delay > frame_time) {
    SDL_Delay(*frame_delay - frame_time);
  }
}
