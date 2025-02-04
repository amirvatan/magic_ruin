#include "dynamic_array.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

d_array init_array(size_t size, size_t d_size) {
  d_array array = {
      .d_size = d_size,
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
    array->data = realloc(array->data, array->size * array->d_size);
    if (array->data == NULL) {
      printf("Add new element to array : Array realloc failed.\n");
      return;
    }
  }
  memcpy((unsigned int *)array->data + index * array->d_size, value,
         array->d_size);
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
    push_array(array, value);
    return;
  }
  memcpy((unsigned int *)array->data + idx * array->d_size, value,
         array->d_size);
}

d_array copy_array(d_array *array) {
  d_array cp_array = init_array(array->size, array->d_size);

  memcpy((unsigned int *)cp_array.data, (unsigned int *)array->data,
         array->size * array->d_size);

  return cp_array;
}

void *head_array(d_array *array) {
  int index = (*array).index;
  return (unsigned int *)array->data + (array->index - 1) * array->d_size;
}

void *pop_array(d_array *array) {
  int index = (*array).index;
  array->index = index - 1;
  return (unsigned int *)array->data + index * array->d_size;
}
