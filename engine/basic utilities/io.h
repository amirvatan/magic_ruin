#ifndef IO_H
#define IO_H 
#include <stdlib.h>
#include <stdio.h>

char * read_file(char * path);
int write_file(void * data,size_t size,char * path);

#endif
