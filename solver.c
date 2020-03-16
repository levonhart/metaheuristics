#include <stdio.h>
#include "solver.h"
#include "hc.h"

void solver_start(solver * sl, sol * dest){
	switch (sl->t) {
		/* case t_rms: */
		/*     rms_solve((rms *) sl, dest); */
		/*     break; */
		/* case t_ils: */
		/*     ils_solve((ils *) sl, dest); */
		/*     break; */
		case t_hc:
			hc_solve((hc *) sl, dest);
			break;
		/* case t_vnd: */
		/*     vnd_solve((vnd *) sl, dest); */
		/*     break; */
		default:
			fprintf(stderr, "Solver Type Error\n");
	}
}
