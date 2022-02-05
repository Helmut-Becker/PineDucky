#include <stdio.h>
#include <string.h>

void help(){
  printf("%s", "Usage:");
  printf("%s", " pinephone-tools [args] < [script]\n");
  printf("%s", "[args]:\n\thelp");
  printf("%s", "\n");
}


void evaluateArgs(char **argv){

  if (argv[1] == NULL) return;
  else if (strcmp(argv[1], "help") == 0) help();

}
