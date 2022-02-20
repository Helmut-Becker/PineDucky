#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 125

/*
 * Struct node
 *
 *  Is a link an a linked list
 *
 *  @var entry = pointer to @struct entry
 *  @var next = holds pointer to next node;
 *    == NULL, if it is the last node in the chain
 */
typedef struct node{
  Entry * entry;
  struct node * next;
}node;

/*
 *  Function initHashTable
 *
 *  Initializes table entries with NULL
 *
 */
static void initHashTable(node ** arr){
  for (size_t i = 0; i < TABLE_SIZE; i++) {
    arr[i] = NULL;
  }
}

/*
 *  Function hash
 *
 *  Provides unique hash for @param keyword
 *    and mapps it to @var _arr
 *
 */
u_int16_t hash(char * keyword){
  u_int16_t length =  strnlen(keyword, MAX_KEYWORD_LENGTH);
  u_int32_t hash = 0;
  for (size_t i = 0; i < length; i++) {
    hash += keyword[i]; hash *= keyword[i];
  }
  return hash % TABLE_SIZE;
}

/*
 *  Function insertIntoHashmap
 *
 *  Inserts @param Entry into hashtable
 *    Does this with chaining
 *
 */
static void insertIntoHashmap(node ** arr, Entry * entry){
  if(arr[hash(entry->keyword)] == NULL){
    arr[hash(entry->keyword)] = malloc(sizeof(node *));
    arr[hash(entry->keyword)]->entry = entry;
  }
  else{
    node * _tmp = arr[hash(entry->keyword)]; //starting node
    node * new = malloc(sizeof(node)); new->entry = entry;
    while (_tmp->next != NULL){
      if(strcmp(_tmp->entry->keyword, entry->keyword) == 0) return;
      _tmp = _tmp->next;
    }
    _tmp->next = new;
  }
}

/*
 *  Function printTable
 *
 *  Well, ... prints entries in @param arr
 *
 */
static void printTable(node ** arr){
  printf("\n\n\n");
  for (size_t i = 0; i < TABLE_SIZE; i++) {
    if(arr[i] != NULL) {
      node * _tmp = arr[i];
      while (_tmp->next != NULL){
        printf("Keyword: %s ", _tmp->entry->keyword);
        printf("Address: %p --> ", _tmp);
        _tmp = _tmp->next;
      }
      printf("Keyword: %s ", _tmp->entry->keyword);
      printf("Address: %p\n", _tmp);
      printf("\n");
    }
    else printf("%d\t%s\n", i, "-----");
  }
}

/*
 *  Function searchHashmap
 *
 *  Searches @param arr for @param keyword
 *  @return @struct node where @struct Entry with keyword is located
 *
 */
node * searchHashmap(node ** arr, char * keyword){
  if(arr[hash(keyword)] != NULL){
    node * _tmp = arr[hash(keyword)];
    do{
      if(strcmp(_tmp->entry->keyword, keyword) == 0)
        return _tmp;
      _tmp = _tmp->next;
    }while(_tmp != NULL);
  }
  else {
    printf("%s NOT found!!!\n", keyword);
    return NULL;
  }
}

/*
 *  Function delete
 *
 *  Deletes a @struct node based on @param keyword
 *
 */
void delete(node ** arr, char * keyword){
  if(arr[hash(keyword)] != NULL){
    node * _tmp = arr[hash(keyword)];
    node * _next_node = NULL;
    while(_tmp->next != NULL){
      if(strcmp(_tmp->next->entry->keyword, keyword) == 0){
        if(_tmp->next->next != NULL)
          _next_node = _tmp->next->next;
        _tmp->next = _next_node;

      }else _tmp = _tmp->next;
    }
  }
}
