#ifndef PHYSICS_H
#define PHYSICS_H

#include "../include/linmath.h"
#include "../basic utilities/dynamic_array.h"
#include "../types.h"

typedef struct {
	vec2 position;
	vec2 half_size;
} AABB;

typedef struct {
	AABB aabb;
	vec2 velocity;
	vec2 acceleration;
} Body;

void init_physics(d_array * objects);
void create_physics_body(d_array * objects,vec2 pos,vec2 size);
void update_physics(d_array * objects,f32 delta);
Body * get_physics_body(d_array * objects,size_t index);
#endif
