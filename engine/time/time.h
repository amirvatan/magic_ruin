#ifndef TIME_H
#define TIME_H
#include "../types.h"
#include <SDL2/SDL.h>

void time_init(f32 frame_rate, f32 *frame_delay); 
void time_update(f32 *time_last, f32 *delta); 
void time_update_late(f32 *frame_delay, f32 *time_last); 
#endif
