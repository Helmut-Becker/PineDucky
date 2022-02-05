#include "pineDucky.h"
#include "parser.h"

int main(int argc, char **argv){

  evaluateArgs(argv);

  parseScript(argv);

  return 0;
}
