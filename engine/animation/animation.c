#include "animation.h"
#include <assert.h>

d_array *animation_definition_storage;
d_array *animation_storage;

void animation_init(void) {
  animation_definition_storage = init_array(20, sizeof(Animation_Definition));
  animation_storage = init_array(20, sizeof(Animation));
}

usize animation_definition_create(Sprite_Sheet *sprite_sheet, f32 *durations,
                                  u8 *rows, u8 *columns, u8 frame_count) {
  assert(frame_count <= MAX_FRAMES);

  Animation_Definition def = {0};

  def.sprite_sheet = sprite_sheet;
  def.frame_count = frame_count;

  for (u8 i = 0; i < frame_count; ++i) {
    def.frames[i] = (Animation_Frame){
        .column = columns[i],
        .row = rows[i],
        .duration = durations[i],
    };
  }
  push_array(animation_definition_storage, &def);
  return animation_definition_storage->index;
}

usize animation_create(usize animation_definition_id, bool does_loop) {
  usize id = animation_storage->index;
  Animation_Definition *adef =
      get_element(animation_definition_storage, animation_definition_id);
  if (adef == NULL) {
    printf("Animation Definition with id %zu not found.",
           animation_definition_id);
  }

  // Try to find free slot first.
  for (usize i = 0; i < animation_storage->index; ++i) {
    Animation *animation = get_element(animation_storage, i);
    if (!animation->is_active) {
      id = i;
      break;
    }
  }

  if (id == animation_storage->index) {
    push_array(animation_storage, &(Animation){0});
  }

  Animation *animation = get_element(animation_storage, id);

  // Other fields default to 0 when using field dot syntax.
  *animation = (Animation){
      .definition = adef,
      .does_loop = does_loop,
      .is_active = true,
  };

  return id;
}

void animation_destroy(usize id) {
  Animation *animation = get_element(animation_storage, id);
  animation->is_active = false;
}

Animation *animation_get(usize id) {
  return get_element(animation_storage, id);
}

void animation_update(f32 dt) {
  for (usize i = 0; i < animation_storage->index; ++i) {
    Animation *animation = get_element(animation_storage, i);
    Animation_Definition *adef = animation->definition;
    animation->current_frame_time -= dt;

    if (animation->current_frame_time <= 0) {
      animation->current_frame_index += 1;

      // Loop or stay on last frame.
      if (animation->current_frame_index ==
          animation->definition->frame_count) {
        if (animation->does_loop) {
          animation->current_frame_index = 0;
        } else {
          animation->current_frame_index -= 1;
        }
      }

      animation->current_frame_time =
          adef->frames[animation->current_frame_index].duration;
    }
  }
}
