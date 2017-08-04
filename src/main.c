#include <stdio.h>
#include <argp.h>
#include <stdlib.h>
#include <stdbool.h>
#include <regex.h>
#include "files.h"
#include "dict.h"
#include "ignored.h"

#define IGNORE 'i'
#define IGNALL 'h'
#define TRANSL 't'
#define TALWAYS 's'

struct arguments {
  char *infile;
  char *dictfile;
  char *outfile;
  char *ignoredfile;
  bool reverse;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
  struct arguments *args = state->input;

  switch(key)
    {
    case 'r':
      args->reverse = true;
      break;
    case 'i':
      args->infile = arg;
      break;
    case 'o':
      args->outfile = arg;
      break;
    case 'd':
      args->dictfile = arg;
      break;
    case 'g':
      args->ignoredfile = arg;
      break;
    default:
      return ARGP_ERR_UNKNOWN;
    }

  return(0);
}

struct argp_option options[] =
  {
    {"input",'i',"FILE",0,"Choose input file."},
    {"dictionary",'d',"FILE",0,"Choose user dictionary."},
    {"ignored",'g',"FILE",0,"Choose dictionary of ignored words."},
    {"output",'o',"FILE",0,"Choose output file."},
    {"reverse",'r',0,0,"Use to translate from english to spanish."},
    {0}
  }; 

static const char args_doc[] = "Input file";

static const char doc[] =
  "Translator -- translates an input file from Spanish to English, or "
  "from english to spanish if the --reverse option is used.";

struct argp argp = {options,parse_opt,args_doc,doc};


char print_options(char *word) {
  char result = '\0';
  char discard = '\0';
  int scanf_result = 0;

  printf( "\n\t\t No hay traducción para la palabra: %s \n"
          "\t\t Ignorar (i) - Ignorar Todas (h)\n\t\t Traducir como (t)" 
          "- Traducir siempre como (s)\n", word);            
  printf("Opcion: ");

  scanf_result = scanf("%c", &result);
  if (scanf_result != 1) {
    result = '\0';
  }
  /* Consume everything left in the stdin buffer */
  while (discard != '\n')
    scanf_result = scanf("%c", &discard);

  return (result);
}

bool is_valid_option(char option) {
  bool result = false;

  result = (option == IGNORE || option == IGNALL
            || option == TRANSL || option == TALWAYS);

  return (result);
}
 
void show_options(char *word, ign_t ignored, dict_t personal, FILE *outfile) {
  char choice = '\0';
  char discard = '\0';
  char *new_word = NULL;
  char *new_trans = NULL;
  char buff[255];
  memset(buff, '\0', 255);
  do {
    choice = print_options(word);
    switch(choice)
      {
      case IGNORE:
        fputs(word, outfile);       
        break;
      case IGNALL:
        new_word = calloc(strlen(word) + 1, sizeof(char));
        strcpy(new_word, word);
        ign_add_word(ignored, new_word);
        fputs(word, outfile);
        break;
      case TRANSL:
        printf("\nTraducir %s como:\n",word);
        scanf("%s", buff);

        while (discard != '\n')
          scanf("%c", &discard);

        fputs(buff, outfile);
        memset(buff, '\0', 255);
        break;
      case TALWAYS:
        printf("\nTraducir %s como:\n", word);
        scanf("%s", buff);

        while (discard != '\n')
          scanf("%c", &discard);

        new_word = calloc(strlen(word) + 1, sizeof(char));
        new_trans = calloc(strlen(buff) + 1, sizeof(char));
        strcpy(new_word, word);
        strcpy(new_trans, buff);
        dict_add_word(personal, new_word, new_trans);
        fputs(new_trans, outfile);
        break;
      default:
        printf("\n Opcion Invalida!, intente nuevamente.\n");
      }
  } while (!is_valid_option(choice));
}

void translate(file_t translation_text ,dict_t dictionary, ign_t ignored, 
          dict_t personal, FILE *outfile)
{   
  char *current_word = file_next_word(translation_text);
  char *translated_word;
  regex_t reg;
  regcomp(&reg, "[^[:space:][:punct:]]", 0);

  while (current_word != NULL) {
    if (strcmp(current_word, "¿") != 0 && strcmp(current_word, "¡") != 0) {
      if (!ign_word_exists(ignored, current_word)
          && (regexec(&reg, current_word, 0, NULL, 0) == 0)) {

        if(dict_word_exists(personal, current_word)) {
          translated_word = dict_find_def(personal, current_word);
          fputs(translated_word, outfile);
        }
        else if(dict_word_exists(dictionary, current_word)) {
          translated_word = dict_find_def(dictionary, current_word);
          fputs(translated_word, outfile);
        }
        else {
          show_options(current_word, ignored, personal, outfile);
        }
      }

      else {
        fputs(current_word, outfile);
      }
    }
    current_word = file_next_word(translation_text);
  }
  regfree(&reg);
}

int main(int argc, char **argv) {
  struct arguments arguments;

  file_t input_text;
  dict_t dictionary;
  ign_t ignored;
  dict_t personal;

  FILE *outfile;

  FILE *instream;
  FILE *dictstream;
  FILE *ignoredstream;

  /* Set argument defaults. */
  arguments.infile = NULL;
  arguments.outfile = NULL;
  arguments.dictfile = NULL;
  arguments.ignoredfile = NULL;
  arguments.reverse = false;
  
  argp_parse(&argp,argc,argv,0,0,&arguments);

  if (!arguments.infile) {
    printf("Debe especificar un archivo de entrada (opcion -i)\n");
    return 0;
  }

  dictstream = fopen("personal_dict.txt", "r");
  if (dictstream != NULL) {
    personal = dict_from_file(dictstream, arguments.reverse);
    fclose(dictstream);
  }
  else {
    personal = new_dict();
  }

  if (arguments.outfile)
    outfile = fopen(arguments.outfile, "w");
  else
    outfile = fopen("translated_text.txt", "w");

  if (arguments.dictfile) {
    dictstream = fopen(arguments.dictfile, "r");
    dictionary = dict_from_file(dictstream, arguments.reverse);
    fclose(dictstream);
  }
  else {
    dictionary = new_dict();
  }

  if (arguments.ignoredfile) {
    ignoredstream = fopen(arguments.ignoredfile, "r");
    ignored = ign_from_file(ignoredstream);
    fclose(ignoredstream);
  }
  else {
    ignored = new_ign();
    arguments.ignoredfile = "def_ignored";
  }

  instream = fopen(arguments.infile, "r");
  input_text = file_from_file(instream);
  fclose(instream);

  translate(input_text, dictionary, ignored, personal, outfile);

  fclose(outfile);

  dictstream = fopen("personal_dict.txt", "w");
  dict_to_file(personal, dictstream, arguments.reverse);
  fclose(dictstream);
  dict_destroy(personal);

  ignoredstream = fopen(arguments.ignoredfile, "w");
  ign_to_file(ignored, ignoredstream);
  fclose(ignoredstream);
  ign_destroy(ignored);

  dict_destroy(dictionary);

  destroy_file(input_text);

  return 0;
}
