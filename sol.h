#ifndef SOL_H_JGUSTNUO
#define SOL_H_JGUSTNUO

typedef struct bin_t bin;
typedef struct sol_t sol;

#include <stdlib.h>
#include "bpp.h"


/*! \struct sol_t
 *  \brief Represents a solution to a instance of BPP
 *
 * 	The objective function of the problem is n_bins (minimize number of bins)
 *  inst_ptr is a pointer to the relative instance of the solution,
 */
struct sol_t{
	bin * bins; /*!< list of bins, list of bins of itens */
	size_t * bin_of;
	bpp * inst_ptr;
	size_t n_bins, _max_size;
};

/**
* @brief: Add item i to bin b
*
* @param: sol s
* 		: size_t i
*       : size_t b
*
* @return: int
*/
int sol_add_item(sol * s, size_t i, size_t b);


/**
* @brief: Remove item from partial solution
*
* @param: sol * s
* 		: size_t i
*
* @return: int
*/
int sol_remove_item(sol * s, size_t i);

/**
* @brief: Remove empty bin from partial solution
*
* @param: sol s
* 		: size_t b
*
* @return: int
*/
int sol_remove_bin(sol * s, size_t b);

/**
* @brief: Copy [src] into [dest]
*
* @param: sol * dest
*       : const sol src
*
* @return: void
*/
void sol_copy(sol * dest, const sol src);

/**
* @brief: Create and add new empty bin
*
* @param: sol s
*
* @return: void
*/
void sol_add_new_bin(sol * s);

/**
* @brief: Create a string that represents the content of solution
*
* @param: const sol b
*       : char * dest
*
* @return: char *
*/
char * soltostr(const sol b, char ** dest);

/**
* @brief: Initiate trivial solution (each bin with only one item)
*
* @param: sol * s
*
* @return: void
*/
void sol_trivial(sol * s);

/**
* @brief: Initiate solution with first fit method
*
* @param: sol * s
* 		: size_t order[n_itens]
*
* @return: void
*/
void sol_firstfit(sol * s, size_t order[s->inst_ptr->n]);

#define sol_alloc(s, inst) {(s).bins = (bin *) malloc((inst).n*sizeof(bin));	\
		(s).bin_of = (size_t *) calloc((inst).n, sizeof(size_t));		\
		(s).n_bins = 0; (s)._max_size = (inst).n; (s).inst_ptr = &(inst);}
#define sol_realloc(s) { (s)._max_size *= 2;		\
		(s).bins = (bin *) realloc((s).bins,( (s)._max_size )*sizeof(bin));}
#define sol_alloc_ptr(s,inst) {s = (struct sol_t *) malloc(sizeof(struct sol_t));	\
		sol_alloc(*(s),inst);}
#define sol_decrease_size(s) { if ( (s)._max_size/2 >  ((s).inst_ptr)->w_sum/((s).inst_ptr->C +1)  ) { \
	(s)._max_size = (s)._max_size/2;		\
	(s).bins = (bin *) realloc((s).bins, ((s)._max_size)*sizeof(bin));} }
#define sol_destroy(s) {while((s).n_bins>0){sol_remove_bin(&(s),(s).n_bins-1);}		\
		if ((s).bins) free( (s).bins ); (s).bins=NULL; \
		if ((s).bin_of) free( (s).bin_of ); (s).bin_of=NULL;}
#define sol_reset(s) {while((s).n_bins>0){sol_remove_bin(&(s),(s).n_bins-1);}}

#define sol_w_of(s,i) (s).inst_ptr->w[i]
#define sol_get_bin(sol,j) (sol).bins + j
#define sol_get_bin_of(sol,i) ((sol).bins + (sol).bin_of[i]-1)


/*! \struct bin_t
 *  \brief Bins
 *
 *  structure to save the bins of the solution.
 */
struct bin_t{
	size_t n, _max_size; /*!< load and bin itens) */
	int load;
	size_t * itens;
};

/**
* @brief: Add item i to bin b
*
* @param: bin * b
*       : size_t i
*       : int w
*
* @return: void
*/
void bin_add(bin * b, size_t i, int w);

/**
* @brief: Remove item i to bin b
*
* @param: bin * b
*       : size_t i
*       : int w
*
* @return: void
*/
void bin_remove(bin * b, size_t i, int w);

/**
* @brief: Create a string that represents the content of bin
*
* @param: const bin b
*       : char * dest
*       : const int w[]
*
* @return: char **
*/
char * bintostr(const bin b, char ** dest, const int * w);

#define bin_alloc(b) {(b).itens = (size_t *) malloc(4*sizeof(size_t));	\
		(b).n = 0; (b).load = 0; (b)._max_size=4;}
#define bin_realloc(b) {(b)._max_size = 3*(b)._max_size/2 + 1;		\
		(b).itens = (size_t *) realloc((b).itens,((b)._max_size)*sizeof(size_t));}
#define bin_alloc_ptr(b) {b = (struct bin_t *) malloc(sizeof(struct bpp_t));	\
		bin_alloc(*(b));}
#define bin_decrease_size(b) {(b)._max_size = 4 > (b)._max_size/2 + 1 ? 4 : (b)._max_size/2 + 1; \
		(b).itens = (size_t *) realloc((b).itens, ((b)._max_size)*sizeof(size_t));}
#define bin_destroy(b) {free((b).itens); (b).itens=NULL; (b)._max_size = 0; (b).n = 0; (b).load = 0;}

#endif /* end of include guard: SOL_H_JGUSTNUO */
