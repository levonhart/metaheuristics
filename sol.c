#include <stdio.h>
#include "sol.h"

int sol_add_item(sol * s, size_t i, size_t b){
	if (s->bin_of[i]) return -1;
	if (s->bins[b].load + sol_w_of(*s,i) > s->inst_ptr->C) return -2;
	bin * bin_ptr = sol_get_bin(*s,b);

	bin_add(bin_ptr,i,sol_w_of(*s,i));
	s->bin_of[i] = b+1;
	return 0;
}


int sol_remove_item(sol * s, size_t i){
	if (!s->bin_of[i]) return -1;
	bin * b = sol_get_bin_of(*s,i);
	bin_remove(b,i,sol_w_of(*s,i));
	s->bin_of[i] = 0;
	return 0;
}

int sol_remove_bin(sol * s, size_t b){
	bin * bins = s->bins;
	size_t i;
	bin_destroy(bins[b]);
	for (i = b; i < s->n_bins-1; ++i) {
		bins[i] = bins[i+1];
	}
	for (i = 0; i < s->inst_ptr->n; ++i) {
		if(s->bin_of[i] == b) s->bin_of[i]=0;
		if(s->bin_of[i] > b) s->bin_of[i]--;
	}

	s->n_bins--;
	if (s->_max_size > 2*s->n_bins) sol_decrease_size(*s);
	return 0;
}

void sol_copy(sol * dest, const sol src){
	sol_reset(*dest);
	dest->inst_ptr = src.inst_ptr;
	for (int b = 0; b < src.n_bins; ++b) {
		if (b == dest->n_bins) sol_add_new_bin(dest);
		for (int i = 0; i < src.bins[b].n; ++i) {
			sol_add_item(dest,src.bins[b].itens[i],b);
		}
	}
}

void sol_add_new_bin(sol * s){
	if(s->_max_size == s->n_bins) sol_realloc(*s);
	bin * b = &(s->bins[s->n_bins]);
	bin_alloc(*b);
	(s->n_bins)++;
}

void sol_trivial(sol * s){
	/* if (!s) { sol_alloc_ptr(s, *s->inst_ptr); } */
	if (!s) { fprintf(stderr, "NULL Pointer Error\n"); return;}
	/* else{ */
	/*     sol_destroy(*s); */
	/*     sol_alloc(*s, *s->inst_ptr); */
	/* } */
	sol_reset(*s);
	for (size_t i = 0; i < s->inst_ptr->n; ++i) {
		sol_add_new_bin(s);
		sol_add_item(s,i,i);
	}
}

void sol_firstfit(sol * s, size_t order[s->inst_ptr->n]){
	sol_reset(*s);
	size_t item;
	for (size_t i = 0; i < s->inst_ptr->n; ++i) {
		item = order[i];
		for (size_t j = 0; j < s->n_bins && s->bin_of[item] == 0; ++j) {
			sol_add_item(s,item,j);
		}
		if (!s->bin_of[item]) {
			sol_add_new_bin(s);
			sol_add_item(s,item,s->n_bins-1);
		}
	}
}

char * soltostr(const sol s, char ** dest){
	size_t size = 256;
	size_t length = 0, lbuffer = 0;
	char * str = (char *) malloc( size*sizeof(char) );
	char * buffer = NULL;
	length+= snprintf(str, size,"N:%zu{%c", s.n_bins, !s.n_bins ? '}' : ' ');
	for (size_t i = 0; i < s.n_bins; ++i) {
		bintostr(s.bins[i], &buffer, s.inst_ptr->w);
		lbuffer = strlen(buffer);
		if(size <  length + lbuffer + 1 + 3 /* 3 = strlen(" , ") */){
			size = 2*(length+lbuffer+1);
			str = (char *) realloc(str, size * sizeof(char));
		}
		length+= snprintf(str + length, size - length,"%s%s", \
				buffer, i < s.n_bins-1 ? " , " : " }");
	}
	if (dest){
		free(*dest);
		*dest = str;
	}
	free(buffer);
	return str;
}

void bin_add(bin * b, size_t i, int w){
	if(b->_max_size == b->n){ bin_realloc(*b);}
	b->itens[b->n++] = i;
	b->load+=w;
}

void bin_remove(bin * b, size_t i, int w){
	for (size_t j = 0; j < b->n; ++j) {
		if (b->itens[j] == i){
			b->itens[j] = b->itens[--b->n];
			break;
		}
	}
	if (b->_max_size > 3*b->n) bin_decrease_size(*b);
	b->load-=w;
}

char * bintostr(const bin b, char ** dest, const int * w){
	size_t size = 128;
	size_t length = 0;
	char * str = (char *) malloc( size*sizeof(char) );
	length+= snprintf(str, size,"L:%d[%c", b.load, !b.n ? ']' : ' ');
	for (size_t i = 0; i < b.n; ++i) {
		if(size-length < 30){
			size = 3*size/2;
			str = (char *) realloc(str, size * sizeof(char));
		}
		length+= snprintf(str + length, size - length,"%d%s", \
				w[b.itens[i]], i < b.n-1 ? " , " : " ]");
	}
	if (dest){
		free(*dest);
		*dest = str;
	}
	return str;
}


