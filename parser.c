#include "parser.h"

Dictionary * _dictionary;
Script * _script;
Delay * _delay;
int _default_delay = 0;

/*
 *  Function allocate
 *
 *  Allocating memory for @global _dictionary
 *    and @global _script
 *
 */
void allocate(){
  _dictionary = malloc(sizeof(Dictionary));
  _dictionary->quantity = 0;
  if(DEBUG) printf("%s\n", "Allocating Memory for Dictionary.");
  if(DEBUG) printf("\t%s %p\n", "Dictionary address: ", _dictionary);
  if(DEBUG) printf("\t%s %d\n", "Dictionary->quantity =");

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

/*
 *  Function evalString
 *
 *  Called if keyword 'STRING' is evalueated in @FILE script
 *  Calling @function insertIntoSequence to insert everyting
 *    in @param _sl into @global _script
 *
 */
void evalString(SplitLine * _sl){

  u_int8_t ** tmp_sequence; tmp_sequence = malloc(sizeof(u_int8_t *));
  *tmp_sequence = calloc(8, sizeof(u_int8_t));
  u_int8_t * tmp_mask; tmp_mask = calloc(1, sizeof(u_int8_t));
  *tmp_mask = 0b01000000;

  for (size_t i = 1; i < _sl->quantity; i++) {
    for (size_t j = 0; j < _sl->length[i]; j++) {
      insertIntoSequence(_dictionary, _script, tmp_mask, tmp_sequence, &_sl->slices[i][j], 1);
    }
  }
  insertIntoScript(_script, tmp_sequence, tmp_mask); // sending line
}

/*
 *  Function evalKeyword
 *
 *  Evaluates Keyword (will later expand this
 *    with 'DELAY' and 'DEFAULT_DELAY')
 *
 */
void evalKeyword(SplitLine * _sl){
  for (size_t i = 0; i < _sl->quantity; i++) {
    if(strcmp(_sl->slices[0], "STRING") == 0){
      evalString(_sl);
      return;
    }
    if(strcmp(_sl->slices[0], "DELAY") == 0){
      insertIntoDelay(_delay, _script->quantity, atoi(_sl->slices[1]));
      if(DEBUG) printf("%s: %d %s: %d\n", "Insert DELAY", atoi(_sl->slices[1]), "at position", _script->quantity);
      return;
    }
    if(strcmp(_sl->slices[0], "DEFAULT_DELAY") == 0){
      _default_delay = atoi(_sl->slices[1]);
      if(DEBUG) printf("%s: %d", "Set DEFAULT_DELAY to", _default_delay);
      return;
    }
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
    // Key, Modifier, Keyword, Custom
    for (size_t j = 0; j < _dictionary->quantity; j++) {
      if (strcmp(_sl->slices[i], _dictionary->entries[j].keyword) == 0){
        if(_dictionary->entries[j].type == Key){
          insertIntoSequence(_dictionary, _script, tmp_mask, tmp_sequence,
                            _dictionary->entries[j].keyword,
                            _dictionary->entries[j].keyword_length);
          if(DEBUG) printf("FOUND KEY: %s\n", _dictionary->entries[j].keyword);
        }
        if(_dictionary->entries[j].type == Modifier){
          insertIntoSequence(_dictionary, _script, tmp_mask, tmp_sequence,
                            _dictionary->entries[j].keyword,
                            _dictionary->entries[j].keyword_length);
          if(DEBUG) printf("FOUND MODIFIER: %s\n", _dictionary->entries[j].keyword);
          continue;
        }
        if(_dictionary->entries[j].type == Keyword){
          evalKeyword(_sl);
          if(DEBUG) printf("FOUND KEYWORD: %s\n", _dictionary->entries[j].keyword);
          continue;
        }
        if(_dictionary->entries[j].type == Custom) printf("FOUND CUSTOM\n");
      }
    }
  }
  insertIntoScript(_script, tmp_sequence, tmp_mask); // sending line
  // Check default delay
  if(_default_delay){
    insertIntoDelay(_delay, _script->quantity-1, _default_delay);
  }
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

  FILE * fp; fp = openFile("defines/keys", "r");
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
      insertIntoDictionary(_dictionary, _sl->length[0], _sl->slices[0], _sl->slices[1], pos);
      if (_sl) freeSplitLine(_sl);
    }
  }
  closeFile(fp, line);
  return 0;
}

void parseScript(char ** argv){
  allocate();
  if(setupKeysAndKewords()){
    printf("%s\n", "Something went wront while populating the dictionary, exiting...");
    exit(EXIT_FAILURE);
  }
  FILE * fp = openFile(argv[1], "r");
  char * line = NULL;
  size_t len = 0;
  ssize_t read;

  while ((read = getline(&line, &len, fp)) != -1) {
    SplitLine * sl; sl = splitLine(line, read, ' ');
    evaluateSplitLine(sl);


  }
  // Write a function to send last sequence

  closeFile(fp, line);

  fp = openFile(argv[2], "w");
  line = NULL;
  len = 0;
  writeSequences(_script, _delay, fp);
  closeFile(fp, line);

  // executing outfile
  char *command = malloc(sizeof(char) * (strlen(argv[2])+3));
  sprintf(command, "%s %s", "sh", argv[2]);
  system(command);
}































//
