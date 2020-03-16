#ifndef SOLVER_H_3PQ6AKOY
#define SOLVER_H_3PQ6AKOY

#include "sol.h"

/*! \enum solver_type
 *
 *  Types of implemented solvers. Used to identify which solver function to use.
 */
enum solver_type { 
	t_hc,
	t_vnd,
	t_rms,
	t_ils
};

/*! \struct solver_t
 *  \brief Brief struct description
 *
 *  Detailed description
 */
typedef struct solver_t{
	enum solver_type t; /*!< Solver type (0 : unset ) */
} solver;


/**
* @brief: Start the solver
*
* @param: solver * sl
* 		: sol * dest
*
* @return: void
*/
void solver_start(solver * sl, sol * dest);

#endif /* end of include guard: SOLVER_H_3PQ6AKOY */
