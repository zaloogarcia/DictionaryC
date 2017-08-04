#include "ignored.h"

// Ignored set structure
struct ign_t_ {
  char *word;
  ign_t left;
  ign_t right;
};

//Create new ignore set
ign_t new_ign(void) {
  ign_t ign = malloc(sizeof(struct ign_t_));
  ign->word = NULL;
  ign->left = NULL;
  ign->right = NULL;
  return ign;
}

// Destroy given ignored set
void ign_destroy(ign_t ign) { 
  ign_t left;
  ign_t right;
 
  if (ign != NULL) {
    left = ign->left;
    right = ign->right;

    free(ign->word);
    ign->word = NULL;

    ign->left = NULL;
    ign->right = NULL;
    free(ign);
    ign = NULL;

    ign_destroy(left);
    ign_destroy(right);
  }
}

// Add given word to given ignored set
void ign_add_word(ign_t ign, char *word) {
  if (ign == NULL) {
    ign = new_ign();
    ign->word = word;
  }
  else if (ign->word == NULL) {
    ign->word = word;
  }
  else {
    unsigned int comparison = strcmp(word, ign->word);
    if (comparison < 0) {
      if (ign->left == NULL)
        ign->left = new_ign();
      ign_add_word(ign->left, word);
    }
    else if (comparison > 0) {
      if (ign->right == NULL)
        ign->right = new_ign();
      ign_add_word(ign->right, word);
    }
    else {
      free(word);
      word = NULL;
    }
  }
}

// Remove given word to given ignored set
void ign_remove_word(ign_t ign, char *word) {
  if (ign != NULL) {
    unsigned int aux_cmp = strcmp(word, ign->word);
    ign_t auxl = ign->left;
    ign_t auxr = ign->right;
    ign_t aux = auxr;

    if (aux_cmp == 0) {
      auxl = ign->left;
      auxr = ign->right;
      free(ign->word);
      ign->word = NULL;
      ign->left = NULL;
      ign->right = NULL;
      free(ign);

      if (auxr == NULL)
	ign = auxl;
      else {
	if (auxl != NULL) {
	  while (aux->left != NULL) {
	    aux = aux->left;
	  }
	  aux->left = auxl;
	}
	ign = auxr;
      }
    }
    else if(aux_cmp < 0)
      ign_remove_word(auxl, word);
    else
      ign_remove_word(auxr, word);
  }
}

// Returns 'true' if given words is in the ignored set given
bool ign_word_exists(ign_t ign, char *word) {
  bool k = false;
  if (ign != NULL && ign->word != NULL) {
    unsigned int aux = strcmp(word, ign->word);
    if (aux == 0) k = true;
    else if (aux < 0) k = ign_word_exists(ign->left, word);
    else k = ign_word_exists(ign->right, word);
  }
  return k;
}

//Puts the given ignored set in given file
void ign_to_file(ign_t ign, FILE *file) {
  if (ign != NULL) {
    if (ign->word != NULL) {
      fputs(ign->word, file);
      fputs("\n", file);
    }
    ign_to_file(ign->left, file);
    ign_to_file(ign->right, file);
  }
}

// Obtains an ignored set from file
ign_t ign_from_file(FILE *fd) {
  char *word;
  char buff[127];
  char word_buff[127];
  memset(buff, '\0', 127);

  ign_t ign = new_ign();
  while (fgets(buff, 127, fd) != NULL) {
    sscanf(buff, "%s", word_buff);
    if (strlen(buff) > 0) {
      word = calloc(strlen(word_buff) + 1, sizeof(char));
      strcpy(word, word_buff);
      ign_add_word(ign, word);
    }
    memset(buff, '\0', 127);
    memset(word_buff, '\0', 127);
  }
  return ign;
}
