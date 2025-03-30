#include "io.h"
#include <stdio.h>
#define IO_READ_CHUNK_SIZE 2097152
char *read_file(const char *path) {
  FILE *fp = fopen(path, "rb");
  if (!fp) {
    printf("Failed to open file %s\n", path);
  }
  char *data = NULL;
  char *tmp;
  size_t used = 0;
  size_t size = 0;
  size_t n;

  while (1) {
    if (used + IO_READ_CHUNK_SIZE + 1 > size) {
      size = used + IO_READ_CHUNK_SIZE + 1;

      if (size <= used) {
        free(data);
      }

      tmp = realloc(data, size);
      if (!tmp) {
        free(data);
      }
      data = tmp;
    }

    n = fread(data + used, 1, IO_READ_CHUNK_SIZE, fp);
    if (n == 0)
      break;

    used += n;
  }

  if (ferror(fp)) {
    free(data);
  }

  tmp = realloc(data, used + 1);
  if (!tmp) {
    free(data);
  }
  data = tmp;
  data[used] = 0;

  return data;
}

int write_file(void *data, size_t size, char *path) {
  FILE *fp = fopen(path, "w");
  if (!fp) {
    printf("Failed to create file %s\n", path);
  }
  size_t written_data = fwrite(data, size, 1, fp);

  fclose(fp);

  if (written_data != 1) {
    printf("Failed to write in file %s\n", path);
    exit(1);
  }
  printf("File saved %s\n", path);

  return 0;
}

