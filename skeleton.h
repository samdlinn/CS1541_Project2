#ifndef __SKELETON_H__
#define __SKELETON_H__

///////////////////////////////////////////////////////////////////////////////
//
// CS 1541 Introduction to Computer Architecture
// You may use this skeleton code to create a cache instance and implement cache operations.
// Feel free to add new variables in the structure if needed.
//
///////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>

struct cache_blk_t {
  unsigned long tag;
  char valid;
  char dirty;
  //added time stamp for lru
  unsigned long long last_access;
  //added time stamp for fifo
  unsigned long long first_access;


/* Add either a pointer (to construct FIFO or LRU lists)
   or a time stamp to implement the replacement polity */
};



enum cache_policy {
  LRU,
  FIFO
};

struct cache_t {
  int nsets;		// # sets
  int bsize;		// block size
  int assoc;		// associativity

  enum cache_policy policy;       // cache replacement policy
  struct cache_blk_t **blocks;    // cache blocks in the cache
};

struct cache_t *
cache_create(int size, int blocksize, int assoc, enum cache_policy policy)
{
// The cache is represented by a 2-D array of blocks.
// The first dimension of the 2D array is "nsets" which is the number of sets (entries)
// The second dimension is "assoc", which is the number of blocks in each set.

	int i;

  //this block assignes the appropriate variabes nblocks and nsets

  int nblocks = (size*1024) / blocksize; //converts size in kb to byte, gets # of blocks
  int nsets = nblocks /assoc; //the number of sets is the number of blocks/ associativity

  //



  struct cache_t *C = (struct cache_t *)calloc(1, sizeof(struct cache_t));

  C->nsets = nsets;
  C->bsize = blocksize;
  C->assoc = assoc;
  C->policy = policy;

  C->blocks= (struct cache_blk_t **)calloc(nsets, sizeof(struct cache_blk_t));

	for(i = 0; i < nsets; i++) {
		C->blocks[i] = (struct cache_blk_t *)calloc(assoc, sizeof(struct cache_blk_t));
	}

  return C;
}

//function that returns the index of a memory access
unsigned long get_index(unsigned long address, int nsets, int blocksize)
{
  return (address / blocksize) % nsets;
}

//function returns the tag for an memory access
unsigned long get_tag(unsigned long address, int nsets, int blocksize)
{
  return (address / blocksize) / nsets;
}

//function that returns if cached access is a hit
int is_hit(struct cache_t *cp, unsigned long access_tag, long access_index,
char access_type, unsigned long long now)
{
  int i;
  //loop for associativity
  for(i = 0; i < cp->assoc; i++)
  {
    //case for a hit in the cache
    if(cp->blocks[access_index][i].tag == access_tag && cp->blocks[access_index][i].valid == 1)
    {
      //case for lru time stamp
      if(cp->policy == 0)
      {
        cp->blocks[access_index][i].last_access = now;
      }
      //case for a write, must set dirty bit
      if(access_type == 1)
      {
        cp->blocks[access_index][i].dirty = 1;
      }
      return 1; //cache hit
    }

    return 0;
  }
}

//returns the index of the least recently used
int calc_LRU(struct cache_t *cp, long access_index){
  int LRU; //used for index of least recently used
  unsigned long long temp_time;
  int i;

  temp_time = cp->blocks[access_index][0].last_access; //gets the first blocks last access time
  LRU = 0;

  //finds least recently used in the associative block
  for(i = 0; i < cp->assoc; i++){
    if(cp->blocks[access_index][i].last_access < temp_time){
      temp_time = cp->blocks[access_index][i].last_access;
      LRU = i;
    }
  }
  return LRU;
}

//calculates the index of the first in
int calc_FIFO(struct cache_t *cp, long access_index){
  int FIFO;
  unsigned long long temp_time;
  int i;

  temp_time = cp->blocks[access_index][0].first_access; //gets the first blocks first access time
  FIFO = 0;

  //finds the first in for fifo associative block
  for(i = 0; i < cp->assoc; i++){
    if(cp->blocks[access_index][i].first_access < temp_time){
      temp_time = cp->blocks[access_index][i].first_access;
      FIFO = i;
    }
  }
  return FIFO;
}

//returns 1 for miss with no write back and 2 for writeback
int is_miss(struct cache_t *cp, unsigned long access_tag, long access_index,
                char access_type, unsigned long long now){
  int i;
  int new_index; //used for replacement
  int return_value;

  //for each associative set
  for(i = 0; i < cp->assoc; i++){
    //if the index isnt valid, can place without removing entry
    if(cp->blocks[access_index][i].valid == 0){
      cp->blocks[access_index][i].tag = access_tag;
      cp->blocks[access_index][i].last_access = now;
      cp->blocks[access_index][i].first_access = now;
      cp->blocks[access_index][i].valid = 1;
      cp->blocks[access_index][i].dirty = 1;
      return 1; //miss with no writeback
    }
  }

  //case for need of replacement
  if(cp->policy == 0) //LRU replacement
    new_index = calc_LRU(cp, access_index);
  else //FIFO replacement
    new_index = calc_FIFO(cp, access_index);

  //case for miss with no writeback
  if(cp->blocks[access_index][new_index].dirty == 0){
    cp->blocks[access_index][new_index].tag = access_tag;
    cp->blocks[access_index][new_index].last_access = now;
    cp->blocks[access_index][new_index].first_access = now;
    cp->blocks[access_index][new_index].valid = 1;
    cp->blocks[access_index][new_index].dirty = 1;
    return 1;
  }
  //case for miss with writeback
  else if(cp->blocks[access_index][new_index].dirty == 1){
    cp->blocks[access_index][new_index].tag = access_tag;
    cp->blocks[access_index][new_index].last_access = now;
    cp->blocks[access_index][new_index].first_access = now;
    cp->blocks[access_index][new_index].valid = 1;
    cp->blocks[access_index][new_index].dirty = 1;
    return 2;
  }
  return 0; //error occured
}

int
cache_access(struct cache_t *cp, unsigned long address,
             char access_type, unsigned long long now)
{
	//////////////////////////////////////////////////////////////////////
  //
  // Your job:
  // based on address determine the set to access in cp
  // examine blocks in the set to check hit/miss
  // if miss, determine the victim in the set to replace
  // if update the block list based on the replacement policy
  // return 0 if a hit, 1 if a miss or 2 if a miss_with_write_back
  //
	//////////////////////////////////////////////////////////////////////

  //variables needed
  int i;
  int miss_wb;
  unsigned long access_index;
  unsigned long access_tag;
  struct cache_blk_t temp;

  //gets the index trying to access
  access_index = get_index(address, cp-> nsets, cp->bsize);

  //gets the tag trying to access
  access_tag = get_tag(address, cp->nsets, cp->bsize);

  //block will dectect a hit or not
  if(is_hit(cp, access_tag, access_index, access_type, now) == 1)
  {
    return 0; //returns 0 if hit
  }

  miss_wb = is_miss(cp, access_tag, access_index, access_type, now);

  if(miss_wb == 0)
  {
    printf("There has been an error\n");
  }

  return miss_wb; //returns 1 for miss without wb 2 for wb


}

#endif
