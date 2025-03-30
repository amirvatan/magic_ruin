#include "physics.h"
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
