#include "parser.h"

u_int8_t DEBUG = 1;

/*
 *  Function splitLine
 *
 *  Splits line into smaller pieces by delimiter
 *
 *  @return struct SplitLine see headerfile for more information
 *  @var tmp = Holds temporary sequence of chars, before
 *    they get inserted into struct @var result
 *
 */
static SplitLine * splitLine(char * line, u_int16_t len, const char delim){
  //number of entries in the first pointer
  SplitLine * result;
  result = malloc(sizeof(SplitLine));
  result->quantity = 0;
  result->slices = (char **)malloc(sizeof(char *));
  char * tmp; tmp = malloc(200);

  u_int16_t offset = 0;
  for(int i = 0; i < len; i++){
    if(line[i] == delim || i == len-1){
      if(DEBUG) printf("result address: %p\n", result);
      if(DEBUG) printf("result->quantity address: %p\n", &result->quantity);
      if(DEBUG) printf("result->slices address: %p\n", result->slices);
      if(DEBUG) printf("result->quantity value: %d\n", result->quantity);
      if(DEBUG) printf("offset value: %d\n", i-offset);
      result->slices[result->quantity] = malloc(i-offset);
      if(DEBUG) printf("result->slices[result->quantity] address: %p\n", result->slices[result->quantity]);
      result->slices[result->quantity][0] = 'c';
      if(DEBUG) printf("result->slices[result->quantity][0] value: %c\n", result->slices[result->quantity][0]);
      for(int j = 0; j < i-offset-1; j++){
        if(DEBUG) printf("insert: %c at ", tmp[j]);
        if(DEBUG) printf("j: %d\n", j);
        result->slices[result->quantity][j] = tmp[j];
      }
      if(DEBUG) printf("%s: %d\n", "Done with inserting i",i);
      offset = (u_int16_t)i;
      tmp = malloc(200);
      result->quantity += 1;
    }else{
      printf("| %c |", line[i]);
      printf("i: %d\n", i);
      tmp[i-offset-1] = line[i];
    }
  }
  free(tmp);
  return result;
}


void parseScript(char ** argv){
  FILE * fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;

  fp = fopen(argv[1], "r");
  if (fp == NULL){
    printf("%s\n", "File not found, exiting...");
    exit(EXIT_FAILURE);
  }

  while ((read = getline(&line, &len, fp)) != -1) {
    if(line[0] == '#') return;
    SplitLine * sl; sl = splitLine(line, read, ' ');
    // printf("returned quantity: %d\n", sl->quantity);
    // printf("returned slice [1]: %c\n", sl->slices[1][1]);
  }

  fclose(fp);
  if (line)
      free(line);
  exit(EXIT_SUCCESS);
}
