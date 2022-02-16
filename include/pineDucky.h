#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void help(){
  printf("%s\n", "Usage:");
  printf("\t%s\n\n", "./pineDucky [script-file] [outfile]");
  printf("\t%s\n", "[script-file]: DuckyScript Language is read from.");
  printf("\t%s\n\n", "[outfile]: Sequences are written into and will be executed");
  printf("\t%s\n", "Note: To turn DEBUG on, change DEBUG to 1 in parser.h and recompile");
}

void evaluateArgs(char **argv){
  if (argv[1] == NULL || argv[2] == NULL) exit(-1);
  else if (strcmp(argv[1], "help") == 0){
    help();
    exit(-1);
  }
}
void parseScript(char ** argv);
