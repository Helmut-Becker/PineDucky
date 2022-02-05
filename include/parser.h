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
 *  @var slices = Holds slices of strings
 *
 */
typedef struct SplitLine{
  u_int8_t quantity;
  char ** slices;
}SplitLine;
