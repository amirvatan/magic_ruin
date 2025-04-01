#ifndef PHYSICS_H
#define PHYSICS_H

#include "../include/linmath.h"
#include "../basic utilities/dynamic_array.h"
#include "../types.h"
#include <stdbool.h>

typedef struct {
	vec2 position;
	vec2 half_size;
} AABB;

typedef struct {
	AABB aabb;
	vec2 velocity;
	vec2 acceleration;
} Body;
typedef struct {
	AABB aabb;
} Static_Body;


typedef  struct {
	size_t other_id;
	f32 time;
	vec2 position;
	vec2 normal;
	bool is_hit;
} Hit;
void init_physics(d_array * objects,d_array * static_objects);
void create_physics_body(d_array * objects,vec2 pos,vec2 size);
void create_physics_static_body(d_array * static_objects,vec2 pos,vec2 size);
void update_physics(d_array * objects,d_array * static_objects,f32 delta);
Body * get_physics_body(d_array * objects,size_t index);
Static_Body *get_physics_static_body(d_array *static_objects, size_t index); 
#endif
