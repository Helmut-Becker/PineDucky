#include "parser.h"

Dictionary * _dictionary;

void allocate(){
  _dictionary = malloc(sizeof(Dictionary));
  _dictionary->quantity = 0;
  if(DEBUG) printf("%s\n", "Allocating Memory for Dictionary.");
  if(DEBUG) printf("\t%s %p\n", "Dictionary address: ", _dictionary);
  if(DEBUG) printf("\t%s %d\n", "Dictionary->quantity =");
}



// static Script *

void evaluateSplitLine(SplitLine * _sl){

  for (int i = 0; i < _sl->quantity; i++){
    // Key, Modifier, Keyword, Custom
    printf("_sl->slices[i]: %s\n", _sl->slices[i]);
    for (size_t j = 0; j < _dictionary->quantity; j++) {
      printf("_dictionary->entries[j].keyword: %s\n", _dictionary->entries[j].keyword);
      if (strcmp(_sl->slices[i], _dictionary->entries[j].keyword) == 0) printf("%s", _dictionary->entries[j].keyword);
    }
  }
  // KEY OR Keyword


  // See if Key
  // See if modifier
  // see if Keyword
  // see if delay
  // See if it is a Keyword if yes call evalKeyword
  // if not
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

  FILE * fp; fp = openFile("defines/keys");
  char * line = NULL;
  size_t len = 0;
  ssize_t read;
  u_int8_t pos;

  while ((read = getline(&line, &len, fp)) != -1) {
    if(line[0] == '#') continue;

    SplitLine * _sl; _sl = splitLine(line, read, ' ');
    if (line[0] == '*'){
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
  FILE * fp = openFile(argv[1]);
  char * line = NULL;
  size_t len = 0;
  ssize_t read;

  while ((read = getline(&line, &len, fp)) != -1) {
    // if(line[0] == '#') break;
    printf("Hewoo");
    SplitLine * sl; sl = splitLine(line, read, ' ');
    // evaluateSplitLine(sl);

  }

  closeFile(fp, line);
}































//
