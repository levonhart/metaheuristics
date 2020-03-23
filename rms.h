#ifndef RMS_H_XLZQHNGS
#define RMS_H_XLZQHNGS

#include "solver.h"

/*! \struct rms_t
 *  \brief Brief struct description
 *
 *  Detailed description
 */
typedef struct rms_t{
	solver_type t; /*!< Description */
	bpp * inst_ptr;
	unsigned int ite;
} rms;

/**
* @brief: Solve [dest] with random multi-start method
*
* @param: rms * sl
*       : sol * dest
*
* @return: void
*/
void rms_solve(rms * sl, sol * dest);

#define rms_alloc(ptr) {ptr = (rms *) malloc(sizeof(rms));}
#define rms_init(s, i) {(s).t = t_rms; (s).ite = i;}

#endif /* end of include guard: RMS_H_XLZQHNGS */
