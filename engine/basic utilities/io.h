#ifndef IO_H
#define IO_H 
#include <stdlib.h>
#include <stdio.h>

char * read_file(const char * path);
int write_file(void * data,size_t size,char * path);

#endif
