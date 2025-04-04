#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../types.h"
typedef struct {
  size_t d_size;
  size_t size;
  size_t index;
  void *data;
} d_array;

d_array *init_array(int size,size_t d_size);
void push_array(d_array * array,void * value);
d_array *copy_array(d_array * array);
void * pop_array(d_array * array);
void * head_array(d_array * array);
void insert_array(d_array * array,void * value,size_t idx);
void *get_element(d_array *array, size_t index); 
