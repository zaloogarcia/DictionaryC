#include "dict.h"

/* Dictionary structure*/
struct dict_t_ {
  char *word;
  char *trans;
  dict_t left;
  dict_t right;
};

/*Creates a new dictionary*/
dict_t new_dict(void) {
  dict_t dict = malloc(sizeof(struct dict_t_));
  dict->word = NULL;
  dict->trans = NULL;
  dict->right = NULL;
  dict->left = NULL;
  return dict;
}

/*Destroys given dictionary*/
void dict_destroy(dict_t dict) { 
  dict_t left;
  dict_t right;
 
  if (dict != NULL) {
    left = dict->left;
    right = dict->right;

    free(dict->word);
    dict->word = NULL;

    free(dict->trans);
    dict->trans = NULL;

    dict->left = NULL;
    dict->right = NULL;
    free(dict);
    dict = NULL;

    dict_destroy(left);
    dict_destroy(right);
  }
}

/*Adds a new word with its translation*/
void dict_add_word(dict_t dict, char *word, char *trans) {
  if (dict->word == NULL) {
    dict->word = word;
    dict->trans = trans;
  }
  else {
    unsigned int comparison = strcmp(word, dict->word);
    if (comparison < 0) {
      if (dict->left == NULL)
        dict->left = new_dict();
      dict_add_word(dict->left, word, trans);
    }
    else if (comparison > 0) {
      if (dict->right == NULL)
        dict->right = new_dict();
      dict_add_word(dict->right, word, trans);
    }
    else {
      free(dict->trans);
      dict->trans = calloc(1, strlen(trans));
      strcpy(dict->trans, trans);
    }
  }
}

/*Remove given word from given dictionary, only works if the word is in it*/
void dict_remove_word(dict_t dict, char *word) {
  if (dict != NULL) {
    unsigned int aux_cmp = strcmp(word, dict->word);
    dict_t auxl = dict->left;
    dict_t auxr = dict->right;
    dict_t aux = auxr;

    if (aux_cmp == 0) {
      auxl = dict->left;
      auxr = dict->right;
      free(dict->word);
      free(dict->trans);
      dict->trans = NULL;
      dict->word = NULL;
      dict->left = NULL;
      dict->right = NULL;
      free(dict);

      if (auxr == NULL)
        dict = auxl;
      else {
        if (auxl != NULL) {
          while (aux->left != NULL) {
            aux = aux->left;
          }
          aux->left = auxl;
        }
        dict = auxr;
      }
    }
    else if(aux_cmp < 0)
      dict_remove_word(auxl, word);
    else
      dict_remove_word(auxr, word);
  }
}

/*Searchs the translation of given word from given dictionary*/
char *dict_find_def(dict_t dict, char *word) {
  unsigned int aux = strcmp(word, dict->word);
  if (aux == 0)
    return dict->trans;
  else if (aux < 0)
    return dict_find_def(dict->left, word);
  else
    return dict_find_def(dict->right, word);
}

/*Returns 'true' if given word is in given dictionary, else 'false'*/
bool dict_word_exists(dict_t dict, char *word) {
  bool k = false;
  if (dict != NULL && dict->word != NULL) {
    unsigned int aux = strcmp(word, dict->word);
    if (aux == 0) k = true;
    else if (aux < 0) k = dict_word_exists(dict->left, word);
    else k = dict_word_exists(dict->right, word);
  }
  return k;
}

/*Puts given dictionary into file*/
void dict_to_file(dict_t dict, FILE *file, bool rev) {
  if (dict != NULL) {
    if (dict->word != NULL) {
      if (rev) {
        fputs(dict->trans, file);
        fputs(",", file);
        fputs(dict->word, file);
        fputs("\n", file);
      }
      else {
        fputs(dict->word, file);
        fputs(",", file);
        fputs(dict->trans, file);
        fputs("\n", file);
      }
      dict_to_file(dict->left, file, rev);
      dict_to_file(dict->right, file, rev);
    }
  }
}

/*Displays on a dictionary structure a given dictionary file*/
dict_t dict_from_file(FILE *fd, bool rev) {
  char *word;
  char *trans;
  char buff[255];
  char word_buff[252];
  char trans_buff[252];

  memset(buff, '\0', 255);
  memset(word_buff, '\0', 252);
  memset(trans_buff, '\0', 252);

  dict_t dict = new_dict();
  while (fgets(buff, 255, fd) != NULL) {
    sscanf(buff, "%252[^,],%s", word_buff, trans_buff);
    if (strlen(word_buff) > 0 && strlen(trans_buff) > 0) {
      word = calloc(strlen(word_buff) + 1, sizeof(char));
      trans = calloc(strlen(trans_buff) + 1, sizeof(char));
      strcpy(word, word_buff);
      strcpy(trans, trans_buff);
      dict_add_word(dict, word, trans);
      memset(word_buff, '\0', 252);
      memset(trans_buff, '\0', 252);
    }
    memset(buff, '\0', 255);
  }
  return dict;
}
