#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "cache.h"

static int index_bits(int n){
  int cnt = 0;

  while(n) {
    cnt++;
    n = n >> 1;
  }

  return cnt-1;
}

/***************************************************************/
/*                                                             */
/* Procedure : build_cache                                     */
/*                                                             */
/* Purpose   : Initialize cache structure                      */
/*                                                             */
/***************************************************************/
void build_cache(struct sce212cache** cache, int block_size, int n_ways, int capacity)
{
  /* Implement this function */
}

/***************************************************************/
/*                                                             */
/* Procedure : access_cache                                    */
/*                                                             */
/* Purpose   : Update cache stat and content                   */
/*                                                             */
/***************************************************************/
void access_cache(struct sce212cache* cache, int op, uint32_t addr, int* hit, int* miss, int* wb)
{
  /* Implement this function */
}
