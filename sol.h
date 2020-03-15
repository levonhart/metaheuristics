#ifndef SOL_H_JGUSTNUO
#define SOL_H_JGUSTNUO

#include <stdlib.h>
#include "bpp.h"

typedef struct bin_t bin;

/*! \struct sol_t
 *  \brief Represents a solution to a instance of BPP
 *
 * 	The objective function of the problem is n_bins (minimize number of bins)
 *  inst_ptr is a pointer to the relative instance of the solution,
 */
typedef struct sol_t{
	bin * bins; /*!< list of bins, list of bins of itens */
	bin ** bin_of;
	bpp * inst_ptr;
	size_t n_bins, _max_size;
} sol;

/**
* @brief: Add item i to bin b
*
* @param: sol s
* 		: size_t i
*       : bin b
*
* @return: int
*/
int sol_add_item(sol * s, size_t i, bin * b);


/**
* @brief: Remove empty bin from partial solution
*
* @param:  i
*
* @return: int
*/
int sol_remove_item(sol * s, size_t i);

/**
* @brief: Remove item from partial solution
*
* @param: sol s
* 		: size_t i
*
* @return: int
*/
int sol_remove_bin(sol * s, bin * b);

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
* @param: bpp instance
*
* @return: void
*/
void sol_trivial(sol * s, bpp instance);

#define sol_add_i_j(s,i,j) sol_add_item((s),i,&((s)->bins[j]))
#define sol_alloc(s, inst) {(s).bins = (bin *) malloc((inst).n*sizeof(bin));	\
		(s).bin_of = (bin **) calloc((inst).n, sizeof(bin*));		\
		(s).n_bins = 0; (s)._max_size = (inst).n; (s).inst_ptr = &(inst);}
#define sol_realloc(s) { (s)._max_size *= 2;		\
		(s).bins = (bin *) realloc((s).bins,( (s)._max_size )*sizeof(bin));}
#define sol_alloc_ptr(s,inst) {s = (struct sol_t *) malloc(sizeof(struct sol_t));	\
		sol_alloc(*(s),inst);}
#define sol_decrease_size(s) {(s).bins = (bin *) realloc((s).bins, (1+(s)._max_size/2)*sizeof(bin));}
#define sol_destroy(s) {while((s).n_bins) sol_remove_bin(&(s),&(s).bins[(s).n_bins-1]);		\
		free( (s).bins ); (s).bins=NULL; free( (s).bin_of ); (s).bin_of=NULL;}

#define sol_w_of(s,i) (s).inst_ptr->w[i]


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
#define bin_decrease_size(b) {(b)._max_size = (b)._max_size/2 + 1; \
		(b).itens = (size_t *) realloc((b).itens, ((b)._max_size)*sizeof(size_t));}
#define bin_destroy(b) {free((b).itens); (b).itens=NULL; (b).n = 0; (b).load = 0;}

#endif /* end of include guard: SOL_H_JGUSTNUO */
