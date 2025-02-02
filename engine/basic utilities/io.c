#include "io.h"

char *read_file(char *path) {
  FILE *fp = fopen(path, "r");
  if (!fp) {
    printf("Failed to read file %s\n", path);
    exit(1);
  }
  fseek(fp, 0, SEEK_END);

  int length = ftell(fp);

  if (length == -1) {
    printf("Failed to read file length %s\n", path);
    exit(1);
  }
  fseek(fp, 0, SEEK_SET);
  char *buffer = malloc((length + 1) * sizeof(char));

  if (!buffer) {
    printf("Failed to allocate buffer");
    exit(1);
  }
  fread(buffer, sizeof(char), length, fp);
  buffer[length] = 0;

  fclose(fp);
  printf("File loaded %s\n", path);
  return buffer;
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
