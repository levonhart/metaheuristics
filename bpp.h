#ifndef BPP_H_JCGHVQGV
#define BPP_H_JCGHVQGV

#include <stdlib.h>
#include <stddef.h>
#include "include/list.h"

#if !defined(__GNUC__) && !defined(__MINGW32__) && !defined(__MINGW64__)
	#ifdef _WIN32
		#include "include/dirent.h"
	#endif
#else
	#include <dirent.h>
#endif


/*! \struct bpp_t
 *  \brief Instance of Bin Packing Problem
 *
 *  Guarda uma instancia do BBP, ou seja, a capaciade máxima do pacote, os pesos dos itens,
 *  o numero de itens e a soma total de pesos.
 */
typedef struct bpp_t {
	int C, w_sum;
	int * w;
	size_t n;
	struct list_head list;
} bpp;

/**
* @brief: Read a formated file of a BPP instance. The file must be one integer per line, structured as follows:
*
* n
* C
* w0
* w1
* ...
*
* where C is the capacity, n is the number of itens and wX are the weight of the itens
*
* @param: char * path Path to the file
* 		: bpp * dest Pointer to which the instance will be saved 
*
* @return: int
*/
int read_instance(char * path, bpp * dest);

/**
* @brief: Calculates a lowerbound for the given instance, calculated as
* { SUM_WEIGHTS / C }
*
* @param: bpp instance
*
* @return: int
*/
static inline int lower_bound(const bpp instance);

/**
* @brief: Procura todos os arquivos em path e tenta ler como instancias do BPP,
* e em seguida, aplica as heuristicas contidas no vetor solvers
*
* @param: char * path
*       : solver solvers[]
*
* @return: void
*/
void benchmark(char * path /*, solver solvers[]*/);

#define instance_alloc(inst) \
		(inst).w = ((inst).n != 0) ?  (int *) malloc((inst).n*sizeof(int)) : NULL;
#define instance_alloc_ptr(inst) inst = malloc(sizeof(struct bpp_t));

#define instance_init0(inst) (inst).C = (inst).w_sum = (inst).n = 0; (inst).w = NULL;
#define instance_init(inst,c,nitens) inst.C = c; inst.w_sum = 0; inst.n=nitens;\
		instance_alloc(inst);

#endif /* end of include guard: BPP_H_JCGHVQGV */
