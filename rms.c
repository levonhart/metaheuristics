#include <stdbool.h>
#include <time.h>
#include "rms.h"
#include "hc.h"

static void swap(size_t * a, size_t * b){
	size_t temp = *a;
	*a = *b;
	*b = temp;
}

static void shuffle(size_t v[], size_t n){
	srand(time(NULL));
	for (size_t i = 0; i < n; ++i) {
		size_t j = rand() % n;	
		swap(v+i,v+j);
	}
}

void rms_solve(rms * sl, sol * dest){
	if (sl->t != t_rms){
		fprintf(stderr, "ParameterType Error.\n");
		return;
	}
	
	sl->inst_ptr = dest->inst_ptr;
	size_t n = sl->inst_ptr->n;

	hc hillclimb;
	hc_init(hillclimb);

	sol curr;
	sol_alloc(curr,*(dest->inst_ptr));

	size_t order[n], i;
	for (i = 0; i < n; ++i)
		order[i] = i;		

	for (i = 0; i < sl->ite; ++i) {
		shuffle(order,n);
		sol_firstfit(&curr,order);
		hc_solve(&hillclimb,&curr);
		if (curr.n_bins < dest->n_bins) {
			sol_copy(dest,curr);
			/* printf("\n%zu HC %zu\n",i+1,dest->n_bins); */
		}
	}

}
