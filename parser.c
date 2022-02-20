#include "parser.h"
#include "hashmap.h"

node * _arr [TABLE_SIZE];
Script * _script;
Delay * _delay;
int _default_delay = 0;

/*
 *  Function allocate
 *
 *  Allocating memory for @global _arr
 *    and @global _script
 *
 */
void allocate(){
  initHashTable(_arr);

  _script = malloc(sizeof(Script));
  _script->quantity = 0;
  _script->sequences = malloc(sizeof(char *));
  if(DEBUG) printf("\n%s\n", "Allocating Memory for Script.");
  if(DEBUG) printf("\t%s %p\n", "Script address: ", _script);
  if(DEBUG) printf("\t%s %d\n", "Script->quantity =");

  _delay = malloc(sizeof(Delay));
  _delay->quantity = 0;
  //_delay->entries = malloc(sizeof(Time *));
  if(DEBUG) printf("\n%s\n", "Allocating Memory for Delay.");
  if(DEBUG) printf("\t%s %p\n", "Delay address: ", _script);
  if(DEBUG) printf("\t%s %d\n", "Delay->quantity =");
}

void sendLastLine(Script * _sc, u_int8_t ** tmp_sequence, u_int8_t * mask){
  insertIntoScript(_sc, tmp_sequence, mask); // sending last line
  if(_default_delay){
    insertIntoDelay(_delay, _script->quantity-1, _default_delay);
  }
}

/*
 *  Function evaluateSplitLine
 *
 *  Evaluates each @var _sl->slices and calls
 *    functions accordingly
 *
 *
 */
void evaluateSplitLine(SplitLine * _sl){

  u_int8_t ** tmp_sequence; tmp_sequence = malloc(sizeof(u_int8_t *));
  *tmp_sequence = calloc(8, sizeof(u_int8_t));
  u_int8_t * tmp_mask; tmp_mask = calloc(1, sizeof(u_int8_t));
  *tmp_mask = 0b01000000;

  if(strcmp(_sl->slices[0], "REM") == 0) return; // Check for comment
  for (int i = 0; i < _sl->quantity; i++){

    Entry * _tmp = searchHashmap(_arr, _sl->slices[i]) != NULL ?
                   searchHashmap(_arr, _sl->slices[i])->entry : NULL;

    if(_tmp != NULL && _tmp->type == Keyword){
      if(strcmp(_tmp->keyword, "DELAY") == 0)
      insertIntoDelay(_delay, _script->quantity, atoi(_sl->slices[1]));

      if(strcmp(_sl->slices[0], "DEFAULT_DELAY") == 0 ||
      strcmp(_sl->slices[0], "DEFAULTDELAY") == 0)
        _default_delay = atoi(_sl->slices[1]);

      if(strcmp(_sl->slices[0], "STRING") == 0){
        for (size_t k = 1; k < _sl->quantity; k++) {
          char * c; c = malloc(2); c[1] = '\0';
          for (size_t j = 0; j < strnlen(_sl->slices[k], MAX_KEYWORD_LENGTH); j++) {
            c[0] = _sl->slices[k][j];
            insertIntoSequence(_script, tmp_mask, tmp_sequence,
                                      searchHashmap(_arr, c)->entry->value);
          } if(k < _sl->quantity-1) insertIntoSequence(_script, tmp_mask, tmp_sequence, 0x2c00);
        }
      }
      sendLastLine(_script, tmp_sequence, tmp_mask);
      return;
    }

    if(_tmp != NULL && (_tmp->type == Key || _tmp->type == Modifier)){
      insertIntoSequence(_script, tmp_mask, tmp_sequence,
                        _tmp->value);
    }
  }
  sendLastLine(_script, tmp_sequence, tmp_mask);
}

/*
 *  Function setupKeysAndKewords
 *
 *  Reads file defines/keys. Fills @var _dictionary with keys and Keywords
 *
 *  @param _dictionary = Dictionary inserted into
 *
 */
int setupKeysAndKewords(){

  FILE * fp; fp = openFile("defines/keys-de", "r");
  char * line = NULL;
  size_t len = 0;
  ssize_t read;
  u_int8_t pos;

  while ((read = getline(&line, &len, fp)) != -1) {
    if(line[0] == '<' && line[1] == '#') continue;

    SplitLine * _sl; _sl = splitLine(line, read, ' ');
    if (line[0] == '*' && line[1] == '*'){
      if(strcmp(_sl->slices[1], "KEYS") == 0) pos = 0;
      else if(strcmp(_sl->slices[1], "MODIFIERS") == 0) pos = 1;
      else if(strcmp(_sl->slices[1], "KEYWORDS") == 0) pos = 2;
      else if(strcmp(_sl->slices[1], "CUSTOM") == 0) pos = 3;
    }else{
      Entry * _tmp_entry = setupEntry(_tmp_entry, _sl->slices[0],
                                      _sl->slices[1], pos);

      insertIntoHashmap(_arr, _tmp_entry);
      if (_sl) freeSplitLine(_sl);
    }
  }
  closeFile(fp, line);
  return 0;
}

void parseScript(char ** argv){
  allocate();
  if(setupKeysAndKewords()){
    printf("%s\n", "Something went wrong while populating the hashmap, exiting...");
    exit(EXIT_FAILURE);
  }
  printTable(_arr);
  FILE * fp = openFile(argv[1], "r");
  char * line = NULL;
  size_t len = 0;
  ssize_t read;

  while ((read = getline(&line, &len, fp)) != -1) {
    SplitLine * sl; sl = splitLine(line, read, ' ');
    evaluateSplitLine(sl);
  }

  insertIntoScript(_script, NULL, NULL); // sending line
  // Write a function to send last sequence

  closeFile(fp, line);

  fp = openFile(argv[2], "w");
  line = NULL;
  len = 0;
  writeSequences(_script, _delay, fp);
  closeFile(fp, line);

  // executing outfile
  char *command = malloc(sizeof(char) * (strlen(argv[2])+5));
  sprintf(command, "%s %s", "bash", argv[2]);
  //system(command);

}































//
