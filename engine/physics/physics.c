#include "physics.h"

void aabb_min_max(vec2 min, vec2 max, AABB aabb) {
  vec2_sub(min, aabb.position, aabb.half_size);
  vec2_add(max, aabb.position, aabb.half_size);
}
AABB aabb_minkowski_difference(AABB a, AABB b) {
  AABB result;
  vec2_sub(result.position, a.position, b.position);
  vec2_add(result.half_size, a.half_size, b.half_size);

  return result;
}

bool physics_aabb_intersect_aabb(AABB a, AABB b) {
  vec2 min, max;
  aabb_min_max(min, max, aabb_minkowski_difference(a, b));

  return (min[0] <= 0 && max[0] >= 0 && min[1] <= 0 && max[1] >= 0);
}


void aabb_penetration_vector(vec2 r, AABB aabb) {
	vec2 min, max;
	aabb_min_max(min, max, aabb);

	f32 min_dist = fabsf(min[0]);
	r[0] = min[0];
	r[1] = 0;

	if (fabsf(max[0]) < min_dist) {
		min_dist = fabsf(max[0]);
		r[0] = max[0];
	}

	if (fabsf(min[1]) < min_dist) {
		min_dist = fabsf(min[1]);
		r[0] = 0;
		r[1] = min[1];
	}

	if (fabsf(max[1]) < min_dist) {
		r[0] = 0;
		r[1] = max[1];
	}
}

Hit ray_intersect_aabb(vec2 pos, vec2 magnitude, AABB aabb) {
	Hit hit = {0};
	vec2 min, max;
	aabb_min_max(min, max, aabb);

	f32 last_entry = -INFINITY;
	f32 first_exit = INFINITY;

for (u8 i = 0; i < 2; ++i) {
		if (magnitude[i] != 0) {
			f32 t1 = (min[i] - pos[i]) / magnitude[i];
			f32 t2 = (max[i] - pos[i]) / magnitude[i];

			last_entry = fmaxf(last_entry, fminf(t1, t2));
			first_exit = fminf(first_exit, fmaxf(t1, t2));
		} else if (pos[i] <= min[i] || pos[i] >= max[i]) {
			return hit;
		}
	}

	if (first_exit > last_entry && first_exit > 0 && last_entry < 1) {
		hit.position[0] = pos[0] + magnitude[0] * last_entry;
		hit.position[1] = pos[1] + magnitude[1] * last_entry;

		hit.is_hit = true;
		hit.time = last_entry;

		f32 dx = hit.position[0] - aabb.position[0];
		f32 dy = hit.position[1] - aabb.position[1];
		f32 px = aabb.half_size[0] - fabsf(dx);
		f32 py = aabb.half_size[1] - fabsf(dy);

		if (px < py) {
			hit.normal[0] = (dx > 0) - (dx < 0);
		} else {
			hit.normal[1] = (dy > 0) - (dy < 0);
		}
	}

	return hit;
}
void init_physics(d_array *objects) {
  *objects = *init_array(40, sizeof(Body));
}

void create_physics_body(d_array *objects, vec2 pos, vec2 size) {
  Body body = {.aabb =
                   {
                       .position = {pos[0], pos[1]},
                       .half_size = {size[0] * 0.5, size[1] * 0.5},
                   },
               .velocity = {0, 0}};
  push_array(objects, &body);
}
Body *get_physics_body(d_array *objects, size_t index) {

  return get_element(objects, index);
}
void update_physics(d_array *objects, f32 delta) {

  Body *body;

  for (size_t i = 0; i < objects->size; i++) {
    body = get_physics_body(objects, i);
    body->velocity[0] += body->acceleration[0] * delta;
    body->velocity[1] += body->acceleration[1] * delta;
    body->aabb.position[0] += body->velocity[0] * delta;
    body->aabb.position[1] += body->velocity[1] * delta;
  }
}
