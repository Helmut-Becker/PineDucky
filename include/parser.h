#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>

#define DEBUG 0

typedef enum Type{Key, Modifier, Keyword, Custom}Type;

/*
*  Struct Entry
*
*  Holds information about an Entry
*
*  @var keyword_length = length of the keyword the @struct Keys
*    will be searched upon
*  @var keyword = Holds keyword as STRING so I can do strcmp
*  @var value = Holds two u_int8_t shifted into one u_int16_t
*    - First 8 bits (read from right to left) = Modifier
*    - Second 8 bits (read from right to left) = Key value
*
*/
typedef struct Entry{
  Type type;
  u_int8_t keyword_length;
  char * keyword;
  u_int16_t value;
}Entry;

/*
 *  Struct Dictionary
 *
 *  Holds a number of @struct Entry
 *
 *  @var quantity = Holds the number of entries in @var entries
 *  @var entries = Holds pointers to @struct Entry
 *
 */
typedef struct Dictionary{
  u_int16_t quantity;
  Entry * entries;
}Dictionary;


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


static void insertIntoDictionary(Dictionary * _dictionary,
                                  u_int8_t keyword_length,
                                  char * keyword,
                                  char * value,
                                  Type type
                                )
{
  // Make space for another entry
  if(DEBUG) printf("%s\n", "Allocating memory for new entry");
  _dictionary->entries = realloc(_dictionary->entries, sizeof(Entry) * (_dictionary->quantity+1));
  if(DEBUG) printf("\t%s: %d\n", "Keyword_length", keyword_length);
  _dictionary->entries[_dictionary->quantity].keyword = malloc(keyword_length);
  _dictionary->entries[_dictionary->quantity].keyword_length = keyword_length;
  // strcpy(_dictionary->entries[_dictionary->quantity].keyword, keyword);
  // _dictionary->entries[_dictionary->quantity].value = (u_int16_t)*value;
  // _dictionary->entries[_dictionary->quantity].type = type;
  if(DEBUG) printf("\t%s: %p\n", "Adress of Entry", &_dictionary->entries[_dictionary->quantity]);
  if(DEBUG) printf("\t%s: %p\n", "Adress of Entry.keyword", _dictionary->entries[_dictionary->quantity].keyword);
  if(DEBUG) printf("\t%s: %d\n", "Entry.keyword_length", _dictionary->entries[_dictionary->quantity].keyword_length);
  _dictionary->quantity += 1;
  if(DEBUG) printf("\t%s: %d\n", "Dictionary->quantity", _dictionary->quantity);
  if(DEBUG) printf("%s\n\n", "Done");
}

































//
