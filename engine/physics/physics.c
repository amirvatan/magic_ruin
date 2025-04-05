#include "physics.h"

static u32 iterations = 2;
static f32 tick_rate;

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

static Hit sweep_static_bodies(AABB aabb, vec2 velocity,
                               d_array *static_objects) {
  Hit result = {.time = 0xBEEF}; // Any large number *should* work fine.

  for (u32 i = 0; i < static_objects->index; ++i) {
    Static_Body *static_body = get_physics_static_body(static_objects, i);

    AABB sum_aabb = static_body->aabb;
    vec2_add(sum_aabb.half_size, sum_aabb.half_size, aabb.half_size);

    Hit hit = ray_intersect_aabb(aabb.position, velocity, sum_aabb);
    if (!hit.is_hit) {
      continue;
    }

    if (hit.time < result.time) {
      result = hit;
    } else if (hit.time == result.time) {
      if (fabsf(velocity[0]) > fabsf(velocity[1]) && hit.normal[0] != 0) {
        result = hit;
      } else if (fabsf(velocity[1]) > fabsf(velocity[0]) &&
                 hit.normal[1] != 0) {
        result = hit;
      }
    }
  }

  return result;
}

static void sweep_response(Body *body, vec2 velocity, d_array *static_objects) {
  Hit hit = sweep_static_bodies(body->aabb, velocity, static_objects);

  if (hit.is_hit) {
    body->aabb.position[0] = hit.position[0];
    body->aabb.position[1] = hit.position[1];

    if (hit.normal[0] != 0) {
      body->aabb.position[1] += velocity[1];
      body->velocity[0] = 0;
    } else if (hit.normal[1] != 0) {
      body->aabb.position[0] += velocity[0];
      body->velocity[1] = 0;
    }
  } else {
    body->aabb.position[0] += velocity[0];
    body->aabb.position[1] += velocity[1];
  }
}

static void stationary_response(Body *body, d_array *static_objects) {
  for (u32 i = 0; i < static_objects->index; ++i) {
    Static_Body *static_body = get_physics_static_body(static_objects, i);

    AABB aabb = aabb_minkowski_difference(static_body->aabb, body->aabb);
    vec2 min, max;
    aabb_min_max(min, max, aabb);

    if (min[0] <= 0 && max[0] >= 0 && min[1] <= 0 && max[1] >= 0) {
      vec2 penetration_vector;
      aabb_penetration_vector(penetration_vector, aabb);

      body->aabb.position[0] += penetration_vector[0];
      body->aabb.position[1] += penetration_vector[1];
    }
  }
}
void init_physics(d_array *objects, d_array *static_objects) {
  *objects = *init_array(40, sizeof(Body));
  *static_objects = *init_array(40, sizeof(Static_Body));
  tick_rate = 1.f / iterations;
}

usize create_physics_body(d_array *objects, vec2 pos, vec2 size,
                          vec2 velocity) {
  Body body = {.aabb =
                   {
                       .position = {pos[0], pos[1]},
                       .half_size = {size[0] * 0.5, size[1] * 0.5},
                   },
               .velocity = *velocity};
  push_array(objects, &body);
  return objects->index;
}
void create_physics_static_body(d_array *static_objects, vec2 pos, vec2 size) {
  Static_Body static_body = {.aabb = {.position = {pos[0], pos[1]},
                                      .half_size = {
                                          size[0] * 0.5,
                                          size[1] * 0.5,
                                      }}};
  push_array(static_objects, &static_body);
}
Body *get_physics_body(d_array *objects, size_t index) {

  return get_element(objects, index);
}
Static_Body *get_physics_static_body(d_array *static_objects, size_t index) {

  return get_element(static_objects, index);
}
void update_physics(d_array *objects, d_array *static_objects, f32 delta) {
  Body *body;

  for (u32 i = 0; i < objects->index; i++) {
    body = get_element(objects, i);

    body->velocity[0] += body->acceleration[0];
    body->velocity[1] += body->acceleration[1];

    vec2 scaled_velocity;
    vec2_scale(scaled_velocity, body->velocity, delta * tick_rate);

    for (u32 j = 0; j < iterations; j++) {
      sweep_response(body, scaled_velocity, static_objects);
      stationary_response(body, static_objects);
    }

    body->velocity[0] = 0;
    body->velocity[1] = 0;
  }
}
