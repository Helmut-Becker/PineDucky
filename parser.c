#include "parser.h"

Dictionary * _dictionary;

void allocate(){
  _dictionary = malloc(sizeof(Dictionary *));
  _dictionary->quantity = 0;
  if(DEBUG) printf("%s\n", "Allocating Memory for Dictionary.");
  if(DEBUG) printf("\t%s %p\n", "Dictionary address: ", _dictionary);
  if(DEBUG) printf("\t%s %d\n", "Dictionary->quantity =");
}



// static Script *

// void evaluateSplitLine(SplitLine * splitLine, ){
//   // KEY OR Keyword
//
//
//   // See if Key
//   // See if modifier
//   // see if Keyword
//   // see if delay
//   // See if it is a Keyword if yes call evalKeyword
//   // if not
// }



static void setupKeysAndKewords(Dictionary * _dictionary){

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
    }
  }
  closeFile(fp, line);
}


void parseScript(char ** argv){
  allocate();
  setupKeysAndKewords(_dictionary);
  FILE * fp = openFile(argv[1]);
  char * line = NULL;
  size_t len = 0;
  ssize_t read;

  // while ((read = getline(&line, &len, fp)) != -1) {
  //   if(line[0] == '#') return;
  //   SplitLine * sl; sl = splitLine(line, read, ' ');
  //   printSplitLine(sl);
  //
  // }

  closeFile(fp, line);
}































//
