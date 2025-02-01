#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  D_INT,
  D_FLOAT,
} d_DataType;

typedef struct {
  d_DataType type;
  size_t size;
  size_t index;
  void *data;
} d_array;

d_array init_array(size_t size,d_DataType type);
void push_array(d_array * array,void * value);
d_array copy_array(d_array * array);
size_t array_size(d_DataType type);
void * pop_array(d_array * array);
void * head_array(d_array * array);
void insert_array(d_array * array,void * value,size_t idx);
