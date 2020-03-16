#include <stdio.h>
#include <stdbool.h>
#include "hc.h"

static bool first_imp(hc * sl, sol * dest);
static bool first_imp2(hc * sl, sol * dest);
static bool first_imp3(hc * sl, sol * dest);

void hc_solve(hc * sl, sol * dest){
	if (sl->t != t_hc){
		fprintf(stderr, "ParameterType Error.\n");
		return;
	}
	sl->inst_ptr = dest->inst_ptr;
	bool imp;
	do {
		imp = first_imp3(sl,dest);
	} while (imp);
}


static bool first_imp3(hc * sl, sol * dest){
	double mean = 1.0* sl->inst_ptr->w_sum / dest->n_bins;
	size_t n = sl->inst_ptr->n, c = dest->inst_ptr->C;
	bin * bins = dest->bins;
	int * w = sl->inst_ptr->w;
	static int l0i,l0j,lfi,lfj,bi;
	static double d0, df;

	for (size_t i = 0; i < n; ++i) {
		bi = dest->bin_of[i]-1;
		for (size_t j = 0; j < dest->n_bins; ++j) {
			if (j != bi && bins[j].load + w[i] <= c ) {
				l0i = sol_get_bin_of(*dest,i)->load;
				lfi = l0i - w[i];
				l0j = bins[j].load;
				lfj = l0j + w[i];
				d0 = (l0i - mean) * (l0i - mean) + (l0j - mean) * (l0j - mean);
				df = (lfi - mean) * (lfi - mean) + (lfj - mean) * (lfj - mean);
				if (df-d0 > MIN_IMPROV) {
					sol_remove_item(dest,i);
					sol_add_item(dest,i,j);

					if (dest->bins[bi].load == 0) sol_remove_bin(dest,bi);
					return true;
				}
			}
		}
	}
	return false;
}

