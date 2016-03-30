#include <stdio.h>
#include <stdlib.h>
#include "trace_item.h"
#include "skeleton.h"

#define TRACE_BUFSIZE 1024*1024

static FILE *trace_fd;
static int trace_buf_ptr;
static int trace_buf_end;
static struct trace_item *trace_buf;

// to keep statistics
unsigned int accesses = 0;
unsigned int read_accesses = 0;
unsigned int write_accesses = 0;
unsigned int hits = 0;
unsigned int misses = 0;
unsigned int misses_with_writeback = 0;

void trace_init()
{
  trace_buf = malloc(sizeof(struct trace_item) * TRACE_BUFSIZE);

  if (!trace_buf) {
    fprintf(stdout, "** trace_buf not allocated\n");
    exit(-1);
  }

  trace_buf_ptr = 0;
  trace_buf_end = 0;
}

void trace_uninit()
{
  free(trace_buf);
  fclose(trace_fd);
}

int trace_get_item(struct trace_item **item)
{
  int n_items;

  if (trace_buf_ptr == trace_buf_end) {
    // get new data
    n_items = fread(trace_buf, sizeof(struct trace_item), TRACE_BUFSIZE, trace_fd);
    if (!n_items) return 0;

    trace_buf_ptr = 0;
    trace_buf_end = n_items;
  }

  *item = &trace_buf[trace_buf_ptr];
  trace_buf_ptr++;

  return 1;
}

int main(int argc, char **argv)
{
  struct trace_item *tr_entry;
  size_t size;
  char *trace_file_name;
  //variables to extract from args
  int trace_view_on, cache_size, block_size, cache_sets, replacement_policy;
  unsigned long long counter = 0;

  //conition for invalid number of arguments
  //exits the program
  if(argc != 7)
  {
    printf("USAGE: cache <trace_file> <trace_view_on> <cache size (power of 2)> <block size (power of 2) <cache associtivity (power of 2)> <replacement policy 0 => LRU 1 => FIFO\n");
    exit(0);
  }

  //extracts necessary command line arguments
  trace_file_name = argv[1];
  trace_view_on = atoi(argv[2]);
  cache_size = atoi(argv[3]);
  block_size = atoi(argv[4]);
  cache_sets = atoi(argv[5]);
  replacement_policy = atoi(argv[6]);

  fprintf(stdout, "\n ** opening file %s\n", trace_file_name);

  trace_fd = fopen(trace_file_name, "rb");

  if (!trace_fd) {
    fprintf(stdout, "\ntrace file %s not opened.\n\n", trace_file_name);
    exit(0);
  }

  trace_init();
  // here should call cache_create(cache_size, block_size, associativity, replacement_policy)

  struct cache_t *my_cache = (struct cache_t*)cache_create(cache_size, block_size, cache_sets, replacement_policy);

  while(1) {
    size = trace_get_item(&tr_entry);

    if (!size) {       /* no more instructions to simulate */
	     printf("+ number of accesses : %d \n", accesses);
      printf("+ number of reads : %d \n", read_accesses);
      printf("+ number of writes : %d \n", write_accesses);
      printf("+ number of hits : %d \n", hits);
      printf("+ number of misses : %d \n", misses);
	  break;
    }
    else{              /* process only loads and stores */;
	  if (tr_entry->type == ti_LOAD) {
			if (trace_view_on) printf("LOAD %x \n",tr_entry->Addr) ;
			accesses ++;
			read_accesses++ ;
			// call cache_access(struct cache_t *cp, tr_entry->Addr, access_type)

      int this_access = cache_access(my_cache, tr_entry->Addr, 0 , counter);
      //case for cache hit
      if(this_access == 0)
      {
        hits++;
      }
      //case for a miss
      else if(this_access == 1)
      {
        misses++;
      }
      else if(this_access == 2)
      {
        misses_with_writeback;
      }
	  }
	  if (tr_entry->type == ti_STORE) {
    		  if (trace_view_on) printf("STORE %x \n",tr_entry->Addr) ;
			accesses ++;
			write_accesses++ ;
			// call cache_access(struct cache_t *cp, tr_entry->Addr, access_type)
      int this_access = cache_access(my_cache, tr_entry->Addr, 1 , counter);
      //case for cache hit
      if(this_access == 0)
      {
        hits++;
      }
      //case for a miss
      else if(this_access == 1)
      {
        misses++;
      }
	  }
	  // based on the value returned, update the statisctics for hits, misses and misses_with_writeback
    }
    counter++;
  }

  trace_uninit();

  exit(0);
}
