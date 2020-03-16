#ifndef SOLVER_H_3PQ6AKOY
#define SOLVER_H_3PQ6AKOY


/*! \enum solver_type
 *
 *  Types of implemented solvers. Used to identify which solver function to use.
 */
typedef enum solver_type_t {
	t_hc,
	t_vnd,
	t_rms,
	t_ils
} solver_type;

/*! \struct solver_t
 *  \brief Brief struct description
 *
 *  Detailed description
 */
typedef struct solver_t{
	solver_type t; /*!< Solver type (0 : unset ) */
} solver;

#include "sol.h"

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
