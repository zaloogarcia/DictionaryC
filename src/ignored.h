#ifndef IGN_H
#define IGN_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// Ignored set structure
typedef struct ign_t_ *ign_t;

//Create new ignore set
ign_t new_ign(void);

// Destroy given ignored set
void ign_destroy(ign_t ign);

/* Function remove_word must only be used after
 * verifying the word exists using word_exists
 */

// Add given word to given ignored set
void ign_add_word(ign_t ign, char *word);

// Remove given word to given ignored set
void ign_remove_word(ign_t ign, char *word);

// Returns 'true' if given words is in the ignored set given
bool ign_word_exists(ign_t ign, char *word);

//Puts the given ignored set in given file
void ign_to_file(ign_t ign, FILE *file);

// Obtains an ignored set from file
ign_t ign_from_file(FILE *file);

#endif
