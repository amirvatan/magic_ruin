#include "dynamic_array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

d_array *init_array(int size, size_t d_size) {
  d_array *array = malloc(sizeof(d_array));
  array->d_size = d_size;
  array->size = size;
  array->index = 0;
  array->data = malloc(size * d_size);
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
  memcpy((u8 *)array->data + (index * array->d_size), value, array->d_size);
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
  memcpy((u8 *)array->data + idx * array->d_size, value, array->d_size);
}

d_array *copy_array(d_array *array) {
  d_array *cp_array = init_array(array->size, array->d_size);

  memcpy((u8 *)cp_array->data, (unsigned int *)array->data,
         array->size * array->d_size);

  return cp_array;
}

void *head_array(d_array *array) {
  int index = (*array).index;
  return (u8 *)array->data + (array->index - 1) * array->d_size;
}
void *get_element(d_array *array, size_t index) {
  return (u8 *)array->data + (index)*array->d_size;
}

void *pop_array(d_array *array) {
  int index = (*array).index;
  array->index = index - 1;
  return (u8 *)array->data + index * array->d_size;
}
