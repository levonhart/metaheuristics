#ifndef SOLVER_H_3PQ6AKOY
#define SOLVER_H_3PQ6AKOY

#include "bpp.h"
#include "sol.h"

#define BPP_HC 0x1
#define BPP_VND 0x10
#define BPP_RMS 0x100
#define BPP_ILS 0x1000
/*! \enum solver_type
 *
 *  Types of implemented solvers. Used to identify which solver function to use.
 */
enum solver_type { 
	hill_climb,
	var_nbh_descend,
	random_multistart,
	iterated_loc_search
};

/*! \struct solver_t
 *  \brief Brief struct description
 *
 *  Detailed description
 */
typedef struct solver_t{
	enum solver_type t; /*!< Solver type (0 : unset ) */
} solver;


#endif /* end of include guard: SOLVER_H_3PQ6AKOY */
