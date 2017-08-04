#include "files.h"

struct file_array {
  char **words;
  int length;
  int first;
};

file_t new_file(void) {
  file_t file = malloc(sizeof(struct file_array));
  file->words = NULL;
  file->length = 0;
  file->first = 0;
  
  return file;
}

char *file_next_word(file_t file) {
  /* Return the next word in a file in memory, or null if the file is empty
     or there are no more words. Does not remove the word from file */

  char *word = NULL;
  int first = file->first;
  int length = file->length;
  char **file_array = file->words;

  if (length != 0 && first < length) {
    word = file_array[first];
    (file->first)++;
  }

  file_array = NULL;
  return word;
}

void destroy_file(file_t file) {
  if (file != NULL) {
    if (file->words != NULL) {
      for (int i = 0; i < file->length; i++)
        free(file->words[i]);
    }
    free(file->words);
    file->words = NULL;
    free(file);
    file = NULL;
  }
}

int ch_num(char c) {
  int masked = c & 0xFF;
  if (masked < 0x80)
    return 1;
  else if (masked < 0xE0)
    return 2;
  else if (masked < 0xF0)
    return 3;
  else if (masked < 0xF8)
    return 4;
  else
    return 0;
}

file_t file_from_file(FILE *fd) {
  /*
    Saves the entirety of the file pointed to by fd in a file_array.
    Each char* in file array will contain one word of the file, a 
    space or a symbol. Returns a struct file_array with
    file_array.array = file_array and file_array.length = number of
    elements in array.

    If the file is empty or the space could not be allocated
    the returned array will be null and the length will be 0.

    fd must be pointing to an open file.
  */
  file_t file = new_file();
  char buff[255];
  char ch[5];
  regex_t reg;
  int count = 0;
  int wcount = 0;
  int charnum;
  memset(buff, '\0', 255);
  regcomp(&reg, "[^[:space:][:punct:]¿¡]", 0);

  while (fgets(ch, 2, fd) != NULL) {
    if ((charnum = ch_num(ch[0])) != 1)
      fgets(ch + 1, charnum, fd);

    if (regexec(&reg, ch, 0, NULL, 0) == 0) {
      count = 1;
    }
    else {
      if (count != 0) {
        count = 0;
        wcount++;
      }
      wcount++;
    }
  }

  if (count != 0)
    wcount++;

  file->words = calloc(wcount, sizeof(char *));
  count = 0;
  wcount = 0;
  rewind(fd);
  while (fgets(ch, 2, fd) != NULL) {
    if ((charnum = ch_num(ch[0])) != 1) {
      fgets(ch + 1, charnum, fd);
    }

    if (regexec(&reg, ch, 0, NULL, 0) == 0) {
      strcpy(buff + count, ch);
      count += charnum;
    }
    else {
      if (count > 0) {
        file->words[wcount] = calloc(count + 1, sizeof(char));
        strcpy(file->words[wcount], buff);
        memset(buff, '\0', 255);
        count = 0;
        wcount++;
      }
      file->words[wcount] = calloc(strlen(ch) + 1, sizeof(char));
      strcpy(file->words[wcount], ch);
      wcount++;
    }
    memset(ch, '\0', 5);
  }

  if (count != 0) {
    file->words[wcount] = calloc(count + 1, sizeof(char));
    strcpy(file->words[wcount], buff);
    memset(buff, '\0', 255);
    count = 0;
    wcount++;
  }

  regfree(&reg);
  file->length = wcount;
  file->first = 0;
  return file;
}

void file_to_file(file_t file, FILE *fd) {
  /* file must not be NULL */
  if (file != NULL && file->words != NULL) {
    for (int i = 0; i < file->length; i++)
      fprintf(fd, "%s", file->words[i]);
  }
}
