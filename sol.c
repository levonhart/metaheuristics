#include <stdio.h>
#include "sol.h"

int sol_add_item(sol * s, size_t i, bin * b){
	if (s->bin_of[i]) return -1;
	if (b->load + sol_w_of(*s,i) > s->inst_ptr->C) return -2;

	bin_add(b,i,sol_w_of(*s,i));
	s->bin_of[i] = b;
	return 0;
}


int sol_remove_item(sol * s, size_t i){
	bin * b = s->bin_of[i];
	if (!b) return -1;
	bin_remove(b,i,sol_w_of(*s,i));
	s->bin_of[i] = NULL;
	if (b->n == 0) sol_remove_bin(s,b);
	return 0;
}

int sol_remove_bin(sol * s, bin * b){
	for (int i = 0; i < s->n_bins; ++i) {
		if (s->bins[i].itens == b->itens) {
			s->bins[i] = s->bins[--s->n_bins];
			break;
		}
	}
	bin_destroy(*b);
	if (s->_max_size > 2*s->n_bins) sol_decrease_size(*s);
	return 0;
}

void sol_add_new_bin(sol * s){
	if(s->_max_size == s->n_bins) sol_realloc(*s);
	bin * b = &(s->bins[s->n_bins]);
	bin_alloc(*b);
	(s->n_bins)++;
}

void sol_trivial(sol * s, bpp instance){
	sol * trivial = s;
	if (!trivial) { sol_alloc_ptr(trivial, instance); }
	else{
		sol_destroy(*trivial);
		sol_alloc(*trivial, instance);
	}
	for (int i = 0; i < instance.n; ++i) {
		sol_add_new_bin(trivial);
		sol_add_i_j(trivial,i,i);
	}
}

char * soltostr(const sol s, char ** dest){
	size_t size = 256;
	size_t length = 0, lbuffer = 0;
	char * str = (char *) malloc( size*sizeof(char) );
	char * buffer = NULL;
	length+= snprintf(str, size,"N:%zu{%c", s.n_bins, !s.n_bins ? '}' : ' ');
	for (int i = 0; i < s.n_bins; ++i) {
		bintostr(s.bins[i], &buffer, s.inst_ptr->w);
		lbuffer = strlen(buffer);
		if(size-length <= lbuffer){
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
	for (int j = 0; j < b->n; ++j) {
		if (b->itens[j] == i){
			b->itens[j] = b->itens[--b->n];
			break;
		}
	}
	if (b->_max_size > 3*b->n && b->n>8) bin_decrease_size(*b);
	b->load-=w;
}

char * bintostr(const bin b, char ** dest, const int * w){
	size_t size = 128;
	size_t length = 0;
	char * str = (char *) malloc( size*sizeof(char) );
	length+= snprintf(str, size,"L:%d[%c", b.load, !b.n ? ']' : ' ');
	for (int i = 0; i < b.n; ++i) {
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


