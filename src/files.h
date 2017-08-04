#ifndef FILEPARSER_H
#define FILEPARSER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <regex.h>

typedef struct file_array *file_t;

file_t new_file(void);

char *file_next_word(file_t file);

void destroy_file(file_t file);

file_t file_from_file(FILE *file);

void file_to_file(file_t file, FILE *filestream);

#endif
