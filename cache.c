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
    (*cache)=malloc(sizeof(struct sce212cache)*(sizeof(int)*3+sizeof(struct cache_set)));  
   (*cache)->n_data_bits = index_bits(block_size);
    //offset의 비트 수 
    (*cache)->n_set_bits = index_bits(capacity / block_size/n_ways);
    // index의 비트 수
    (*cache)->n_ways = n_ways;
    (*cache)->sets = malloc(sizeof(struct cache_set) * (capacity/block_size/n_ways));
    for(int i=0;i<capacity / block_size/n_ways;i++){
      (*cache)->sets[i].lines=malloc(sizeof(struct cache_line)*(*cache)->n_ways);
      for(int j=0;j<(*cache)->n_ways;j++){
        (*cache)->sets[i].lines[j].age=n_ways;
        (*cache)->sets[i].lines[j].valid=false;
        (*cache)->sets[i].lines[j].dirty=false;
        (*cache)->sets[i].lines[j].tag=0;
      }
    }   
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
  int sh=cache->n_data_bits+cache->n_set_bits;
  int index=(addr>>cache->n_data_bits)&((1<<cache->n_set_bits)-1);
  int i,j;
  int max=0,max_num=0;

  for(i=0;i<cache->n_ways;i++){ 
    if(cache->sets[index].lines[i].valid==true){
      if(cache->sets[index].lines[i].tag==addr>>sh){
        (*hit)++;
        cache->sets[index].lines[i].age=0;
        if(op==1){
            cache->sets[index].lines[i].dirty=true;
          // 이때도 데이터를 쓰는 건데..  
        }
        break;
      }
      if(cache->sets[index].lines[i].age>max){
        max=cache->sets[index].lines[i].age;
        max_num=i;
      }
    }
    else{
        (*miss)++;
        cache->sets[index].lines[i].tag=addr>>sh;
        cache->sets[index].lines[i].valid=true;
        cache->sets[index].lines[i].age=0;
        // 같은 태그이지만 데이터 내용을 바꿀때도 있잖아 그럴때도 write back 아닌가..
        if(op==1){
          cache->sets[index].lines[i].dirty=true;
        }
        break;
      }
  }
// 싹 다 트루일때.. 나가기 전 여기서 마무리 지어야지 write back을..
  if(i==cache->n_ways){
    (*miss)++;
    i=max_num;       
    cache->sets[index].lines[i].tag=addr>>sh;
    cache->sets[index].lines[i].valid=true;
    cache->sets[index].lines[i].age=0;
    if(cache->sets[index].lines[i].dirty==true){
      (*wb)++;
    }
    if(op==1){
      cache->sets[index].lines[i].dirty=true;
    }
    else{
      cache->sets[index].lines[i].dirty=false;
    }
    
  }

  for(j=0;j<cache->n_ways;j++){
      if(cache->sets[index].lines[j].valid==true&& i!=j){
        cache->sets[index].lines[j].age++;
      }
  } 
}
