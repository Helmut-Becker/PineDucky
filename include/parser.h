#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>


/*
 *  Struct Sequence
 *
 *  @var modifier = Holds the modifier this line will be send with
 *    i.e SHIFT or ALT in hex
 *  @var sequence = Holds the sequence of keystrokes that will be send
 *    with this line
 *
 */
typedef struct Sequence{
  u_int8_t modifier;
  u_int8_t sequence[6];
}Sequence;


/*
 *  Struct SplitLine
 *
 *  This struct is used to split a line out of the DuckyScript into
 *    smaller pieces, to evaluate them all one by one.
 *
 *  @var quantity = Holds number of entries in slices
 *  @var length = Holds pointers to length of slices
 *  @var slices = Holds slices of strings
 *
 */
typedef struct SplitLine{
  u_int8_t quantity;
  u_int8_t * length;
  char ** slices;
}SplitLine;


/*
 *  Struct Script
 *
 *  Struct to hold Bytesequences of 8 bytes each
 *
 *  @var quantity = Holds number of entries in sequences
 *  @var sequences = Holds Bytesequences
 */
typedef struct Script{
  u_int8_t quantity;
  char ** sequences;
}Script;


/*
 *  Function printSplitLine
 *
 *  Prints all contents of split line onto STDOUT
 *
 */
static void printSplitLine(SplitLine * sl){
  printf("Quantity: %d\n", sl->quantity);
  for (size_t i = 0; i < sl->quantity; i++){
    printf("%s: %d\n", "Slice", i);
    printf("\t%s: %d\n\t", "length", sl->length[i]);
    for (size_t j = 0; j < sl->length[i]; j++) {
      printf("%c", sl->slices[i][j]);
    }
    printf("\n%s\n", "-----------------------------");
  }
}
