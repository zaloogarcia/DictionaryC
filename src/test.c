#include "dict.h"
#include "ignored.h"
#include "files.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  FILE *fd;
  dict_t dict;

  fd = fopen("perma_dict.txt", "r");
  if (fd != NULL) {
    dict = dict_from_file(fd, false);
    fclose(fd);
  }

  fd = fopen("userdict", "w");
  dict_to_file(dict, fd, false);
  fclose(fd);
  dict_destroy(dict);
  return 0;
}
