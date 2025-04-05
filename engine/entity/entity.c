#include "entity.h"

static d_array *entity_list;

void entity_init(void) { entity_list = init_array(20, sizeof(Entity)); }

usize entity_create(vec2 position, vec2 size, vec2 velocity) {
  Entity entity = {
      .body_id = create_physics_body(position, size, velocity),
      .animation_id = (usize)-1,
      .is_active = true,
  };
  push_array(entity_list, &entity);
  return entity_list->index;
}

Entity *entity_get(usize id) { return get_element(entity_list, id); }
usize entity_count() { return entity_list->index; }
