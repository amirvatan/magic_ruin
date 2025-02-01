#include "dynamic_array.h"

size_t array_size(d_DataType type) {
  size_t d_size;
  switch (type) {
  case D_INT:
    d_size = sizeof(int);
    break;
  case D_FLOAT:
    d_size = sizeof(float);
    break;
  default:
    d_size = 0;
  }
  return d_size;
}

d_array init_array(size_t size, d_DataType type) {
  size_t d_size = array_size(type);
  d_array array = {
      .type = type,
      .size = size,
      .index = 0,
      .data = malloc(size * d_size),
  };
  return array;
}

void push_array(d_array *array, void *value) {
  size_t size = array->size;
  size_t index = array->index;
  if (size == index) {
    array->size = size * 2;
    array->data = realloc(array->data, array->size * array_size(array->type));
    if (array->data == NULL) {
      printf("Add new element to array : Array realloc failed.\n");
      return;
    }
  }
  if (array->type == D_INT) {
    int *intptr = (int *)value;
    ((int *)array->data)[index] = *intptr;
  } else {
    float *fltptr = (float *)value;
    ((float *)array->data)[index] = *fltptr;
  }
  array->index = ++index;
}
void insert_array(d_array *array, void *value, size_t idx) {
  size_t size = array->size;
  size_t index = array->index;
  if (size < idx) {
    printf("Add new element to array : specified index is more than size.\n");
    return;
  }
  if (size == idx) {
    array->size = size * 2;
    array->data = realloc(array->data, array->size * array_size(array->type));
    if (array->data == NULL) {
      printf("Add new element to array : Array realloc failed.\n");
      return;
    }
  }
  if (array->type == D_INT) {
    int *intptr = (int *)value;
    ((int *)array->data)[idx] = *intptr;
  } else {
    float *fltptr = (float *)value;
    ((float *)array->data)[idx] = *fltptr;
  }
  if (idx == index) {

    array->index = ++index;
  }
}

d_array copy_array(d_array *array) {
  size_t d_size = array_size(array->size);

  d_array cp_array = init_array(array->size, array->type);

  memcpy(cp_array.data, array->data, array->size * d_size);

  return cp_array;
}

void *head_array(d_array *array) {
  int index = (*array).index;
  if (array->type == D_INT) {
    return &((int *)array->data)[index - 1];
  } else {
    return &((float *)array->data)[index - 1];
  }
}

void *pop_array(d_array *array) {
  int index = (*array).index;
  array->index = index - 1;
  if (array->type == D_INT) {
    return &((int *)array->data)[index - 1];
  } else {
    return &((float *)array->data)[index - 1];
  }
}
