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
  u_int8_t ** sequences;
}Script;

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

/*
 *  Function insertIntoDictionary
 *
 *  Creates and inserts a new Entry at the end of @param _dictionary
 *
 *  @param _dictionary = Pointer to Dictionary into which shall be inserted
 *  @param keyword_length = Length of @param keyword
 *  @param value = Char array in wich the hex value of keystroke is stored
 *  @param type = Enum Type to identify Entry
 *
 */
static void insertIntoDictionary(Dictionary * _dictionary,
                                  u_int8_t keyword_length,
                                  char * keyword,
                                  char * value,
                                  Type type
                                )
{
  // Make space for another entry
  if(DEBUG) printf("%s\n", "Allocating memory for new entry");
  _dictionary->entries = (Entry *)realloc(_dictionary->entries, sizeof(Entry) * (_dictionary->quantity+1));
  if(DEBUG) printf("\t%s: %d\n", "Keyword_length", keyword_length);
  _dictionary->entries[_dictionary->quantity].keyword = malloc(keyword_length+1); // +1 for the '\0'
  _dictionary->entries[_dictionary->quantity].keyword_length = keyword_length;
  strcpy(_dictionary->entries[_dictionary->quantity].keyword, keyword);
  _dictionary->entries[_dictionary->quantity].value = (u_int16_t) strtol(value, NULL, 16);
  _dictionary->entries[_dictionary->quantity].type = type;
  if(DEBUG) printf("\t%s: %p\n", "Adress of Entry", &_dictionary->entries[_dictionary->quantity]);
  if(DEBUG) printf("\t%s: %p\n", "Adress of Entry.keyword", _dictionary->entries[_dictionary->quantity].keyword);
  if(DEBUG) printf("\t%s: %d\n", "Allocated Bytes", keyword_length+1);
  if(DEBUG) printf("\t%s: %d\n", "Entry.type", _dictionary->entries[_dictionary->quantity].type);
  if(DEBUG) printf("\t%s: %d\n", "Entry.keyword_length", _dictionary->entries[_dictionary->quantity].keyword_length);
  if(DEBUG) printf("\t%s: %s\n", "Keyword", _dictionary->entries[_dictionary->quantity].keyword);
  if(DEBUG) printf("\t%s: %x\n", "Value", _dictionary->entries[_dictionary->quantity].value);
  _dictionary->quantity += 1;
  if(DEBUG) printf("\t%s: %d\n", "Dictionary->quantity", _dictionary->quantity);
  if(DEBUG) printf("%s\n\n", "Done");
}

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
  // printf("%s: %d %s: %d\n", "Sizeof SplitLine", sizeof(SplitLine), "Sizeof Splitline* ", sizeof(SplitLine *));
  result = (SplitLine *)calloc(1, sizeof(struct SplitLine));
  result->quantity = 0;
  result->length = malloc(sizeof(u_int8_t));
  result->slices = malloc(sizeof(char *)*3); // If I change this to 3 = errors häää
  char * tmp; tmp = malloc(200 * sizeof(char));
  //
  // result->slices   p--->   char *  p---> char *
  //
  //
  u_int16_t offset = 0;
  for(int i = 0; i < len; i++){
    if(line[i] == delim || i == len-1){
      result->length = (u_int8_t *)realloc(result->length, (result->quantity)+1);
      if(DEBUG) printf("result address: %p\n", result);
      if(DEBUG) printf("result->quantity address: %p\n", &result->quantity);
      if(DEBUG) printf("result->slices address: %p\n", result->slices);
      if(DEBUG) printf("result->quantity value: %d\n", result->quantity);
      if(DEBUG) printf("offset value: %d\n", i-offset);
      result->slices[result->quantity] = malloc(i-offset+1 * sizeof(char)); // +1 because of the '\0' following
      if(DEBUG) printf("result->slices[result->quantity] malloced size: %d\n", i-offset+1);

      if(DEBUG) printf("result->slices[result->quantity] address: %p\n", &result->slices[result->quantity]);
      int j = 0;
      while (j < i-offset){
        // if(DEBUG) printf("insert: %c at ", tmp[j]);
        // if(DEBUG) printf("j: %d\n", j);
        result->slices[result->quantity][j] = tmp[j];
        j++;
      }
      if(DEBUG) printf("insert: %s at ", "\\0");
      if(DEBUG) printf("j: %d\n", j);
      result->slices[result->quantity][j] = '\0';
      if(DEBUG) printf("%s: %d\n", "Done with inserting i",i);
      offset = (u_int16_t)i+1;
      result->length[result->quantity] = j;
      if(DEBUG) printf("%s %d %s%d%s\n", "inserting", j, "into result->length[", result->quantity, "]");
      free(tmp);
      tmp = malloc(200  * sizeof(char));
      result->quantity += 1;
      if(DEBUG) printf("%s\n", "----------------------------------------------------------");
    }else{
      if(DEBUG) printf("| %c |", line[i]);
      if(DEBUG) printf("%s%d%s\n", "tmp[", i-offset, "]");
      tmp[i-offset] = line[i];
    }
  }
  free(tmp);
  return result;
}

/*
 *  Function openFile
 *
 *  Opens file and @return filepointer
 *
 */
static FILE * openFile(char * path){
  FILE * fp;
  fp = fopen(path, "r");
  if (fp == NULL){
    printf("%s\n", "File not found, exiting...");
    exit(EXIT_FAILURE);
  }
  return fp;
}

/*
 *  Function closeFile
 *
 *  Closes file @param fp and frees @param line
 *
 */
static void closeFile(FILE * fp, char * line){
  fclose(fp);
  if (line)
      free(line);
}

static void freeSplitLine(SplitLine * _sl){
  for (size_t i = 0; i < _sl->quantity; i++) {
    if(_sl->slices[i]){
      // printf("slice %p gets freed\n", _sl->slices[i]);
      free(_sl->slices[i]);
    }
  }
  if(_sl->slices) free(_sl->slices);
  if(_sl->length) free(_sl->length);
  if(_sl) free(_sl);
}

// static u_int8_t * copy_uint8_Array(u_int8_t * dest, u_int8_t * src, u_int16_t len){
//   u_int8_t * p; p = malloc(sizeof(u_int8_t) * len);
//   for (size_t i = 0; i < len; i++) {
//     dest[i] = src[i];
//   }
//   return p;
// }

static char * strrev(char * str){
      char *p1, *p2;

      if (! str || ! *str)
            return str;
      for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
      {
            *p1 ^= *p2;
            *p2 ^= *p1;
            *p1 ^= *p2;
      }
      return str;
}

static char * printIntToBinary(int n){
  char p[9]; int pos = 0;
  do{
    if(n % 2 == 0) p[pos++] = '0';
    else p[pos++] = '1';
    n = n / 2;
  }while(n > 0);
  for (size_t i = pos; i < 8; i++) { p[pos++] = '0'; }
  p[pos] = '\0';
  return strrev(p);
}

static u_int8_t * insertIntoSequence(u_int8_t mask, u_int8_t * tmp_sequence, char * value){
  printf("%s: %s\n", "Mask", printIntToBinary(mask));
  printf("\n%s: \n", "Sequence");
  for (size_t i = 0; i < 8; i++) {
    printf("%c ", tmp_sequence[i]);
  }
  printf("\nValue: %s\n", value);
  return tmp_sequence;
  // need to take value apart
  // return the sequence, because if this function has to make a new sequence, because it is full or the same letter got inserted, the new sequence can be returned
  // checking if its full
  // checking if the same value was inserted before, if jes make a new sequence and
 //look out for netx spot in mask and then write a 1 in this spot, if key value was not 0x00. In this case it was a modifier
}
//
// static void insertIntoScript(){
//
// }
























//
