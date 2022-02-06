#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>


/*
*  Struct Key
*
*  Holds information about a Key
*
*  @var keyword_length = length of the keyword the @struct Keys
*    will be searched upon
*  @var keyword = Holds keyword as STRING so I can do strcmp
*  @var value = Holds two u_int8_t shifted into one u_int16_t
*    - First 8 bits (read from right to left) = Modifier
*    - Second 8 bits (read from right to left) = Key value
*
*/
typedef struct Key{
  u_int8_t keyword_length;
  char * keyword;
  u_int16_t value;
}Key;

/*
 *  Struct Keywords
 *
 *  Holds Keywords like DELAY, STRING, etc...
 *
 *  @var keyword_length = length of @var keyword
 *  @var entries = Holds keyword as STRING so I can do strcmp
 *
 */
typedef struct Keywords{
  u_int8_t quantity;
  Key * entries;
}Keywords;


/*
 *  Struct Keys
 *
 *  Holds a number of @struct Key
 *
 *  @var quantity = Holds the number of entries in @var keys
 *  @var entries = Holds pointers to @struct Key
 *
 */
typedef struct Keys{
  u_int16_t quantity;
  Key * entries;
}Keys;


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
 *
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
