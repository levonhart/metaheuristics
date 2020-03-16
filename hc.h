#ifndef HILLCLIMB_H_AED3PJBQ
#define HILLCLIMB_H_AED3PJBQ

#include "solver.h"

#define MIN_IMPROV 0.001

/*! \struct hillclimb_t
 *  \brief hillclimb method
 *
 *  Detailed description
 */
typedef struct hc_t{
	solver_type t; /*!< Solver type */
	bpp * inst_ptr;
} hc;

/**
* @brief: Solve [dest] with hillclimb method
*
* @param: hc * sl
*       : sol * dest
*
* @return: void
*/
void hc_solve(hc * sl, sol * dest);

#define hc_alloc(ptr) {ptr = (hc*) malloc(sizeof(hc));}
#define hc_init(s) {(s).t = t_hc;}

#endif /* end of include guard: HILLCLIMB_H_AED3PJBQ */
