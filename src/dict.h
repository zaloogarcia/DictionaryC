#ifndef DICT_H
#define DICT_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/* Pointer to a dictionary */
typedef struct dict_t_ *dict_t;

/* Creates a new dictionary */
dict_t new_dict(void);

/* Destroys given dictionary */
void dict_destroy(dict_t dict);

/* Functions remove_word and find_def must only be used after
 * verifying the word exists using word_exists
 */

/* Adds a new word with its translation */
void dict_add_word(dict_t dict, char *word, char *def);

/* Remove given word from given dictionary, only works if the word is in it */
void dict_remove_word(dict_t dict, char *word);

/* Searchs the translation of given word from given dictionary */
char *dict_find_def(dict_t dict, char *word);

/* Returns 'true' if given word is in given dictionary, else 'false' */
bool dict_word_exists(dict_t dict, char *word);

/* Puts given dictionary into file */
void dict_to_file(dict_t dict, FILE *file, bool rev);

/* Loads a file into a dictionary */
dict_t dict_from_file(FILE *file, bool rev);

#endif
