#include <stdio.h>
#include <string.h>
#include "bpp.h"
#include "sol.h"
#include "solver.h"

int read_instance(char * path, bpp * instance){
	FILE* file; fopen_s(&file, path, "r");
	if (file == NULL) {
		printf("Arquivo nao existente:\npath: %s\n", path);
		return -1;
	}
	fscanf(file, "%zu%zu", &instance->n, &instance->C);
	if (instance == NULL) instance_alloc_ptr(instance);
	instance_alloc(*instance);

	instance->w_sum = 0;
	for (int i = 0; i < instance->n; ++i) {
		if( !fscanf(file, "%d", &(instance->w)[i]) ) return -2;
		instance->w_sum += (instance->w)[i];
	}

	fclose(file);

	return 0;
}

static inline int lower_bound(const bpp instance){
	return (instance.w_sum % instance.C) == 0 ?		\
			instance.w_sum/instance.C : instance.w_sum/instance.C + 1;
}
char * bpptostr(const bpp instance,char ** dest){
	size_t size = 256;
	size_t length = 0;
	char * str = (char *) malloc( size*sizeof(char) );
	length+= snprintf(str, size,"C: %zu\n" \
			"w_sum: %zu\n{ ", \
			instance.C,instance.w_sum);
	for (int i = 0; i < instance.n; ++i) {
		if(size-length < 30){
			size = 3*size/2;
			str = (char *) realloc(str, size * sizeof(char));
		}
		length+= snprintf(str + length, size - length,"%d%s", \
				instance.w[i], i < instance.n-1 ? " , " : " ");
	}
	length += snprintf(str + length,size-length,"}\nLB : %d\n", lower_bound(instance));
	if (dest){
		free(*dest);
		*dest = str;
	}
	return str;
}


void benchmark(char * path /*, solver solvers[] */){
	bpp * inst; char * buffer;
	LIST_HEAD(instances);

	DIR * dir = opendir(path);
	struct dirent * entry;
	while ( (entry = readdir(dir)) ) {
		if(entry->d_name[0] == '.') continue;
		instance_alloc_ptr(inst);
		instance_init0(*inst);
		char relative_path[strlen(entry->d_name)+strlen(path)+1];
		strcpy(relative_path, path);
		if (relative_path[strlen(relative_path)-1]!='/')
		strcat(relative_path,"/");
		printf("%s\n", strcat(relative_path, entry->d_name));
		// strcat(relative_path, entry->d_name);
		read_instance(relative_path, inst);
		list_add_tail(&inst->list, &instances);

		inst = NULL;
	}
	struct list_head * iter;
	list_for_each(iter, &instances){
		inst = list_entry(iter, bpp, list);
		printf("%s\n", bpptostr(*inst, &buffer));
	}

	free(inst); free(buffer);
	closedir(dir);
}

int main(int argc, char *argv[]){
	char * path = argv[1];
	char * buffer;
	if (path==NULL) {
		path = "instances/Hard28/";
	}

	/* benchmark(path); */
	bpp inst;
	instance_init0(inst);
	read_instance("instances/Scholl/Scholl_1/N1C1W1_A.txt", &inst);
	printf("%s\n\n", bpptostr(inst,&buffer));



	/* bin b; */
	/* bin_alloc(b); */
	/* printf("b:%zu,%zu, l:%d\n",b.n, b._max_size,b.load); */
	/* for (int i = 0; i < 4; ++i) { */
	/*     printf(" %zu{%d} ", b.itens[i], inst.w[i]); */
	/* } */
	/* printf("\n"); */
	/* for (int i = 0; i < inst.n; ++i) { */
		/* bin_add(&b,i,inst.w[8]); */
		/* printf("b:%zu,%zu, l:%d\n",b.n, b._max_size,b.load); */
	/* } */

	/* printf("b:%zu,%zu, l:%d\n",b.n, b._max_size,b.load); */
	/* for (int i = 0; i < 10; ++i) { */
	/*     printf(" %zu{%d} ", b.itens[i], inst.w[i]); */
	/* } */
	/* printf("\n"); */

	/* printf("L:%d { item0:%zu, item1:%zu } \n", b.load, b.itens[0], b.itens[1]); */
	/* printf("%s\n", bintostr(b,&buffer,inst.w)); */

	/* for (int i = 2; i < inst.n; i+=1) { */
	/*     bin_remove(&b,i,inst.w[8]); */
	/*     printf("b:%zu,%zu, l:%d\n",b.n, b._max_size,b.load); */
	/* } */

	/* sol * s = sol_trivial(inst); */
	sol s;
	sol_alloc(s, inst);
	sol_add_new_bin(&s);
	printf("%s\n", bintostr(s.bins[0], &buffer, inst.w));

	sol_add_i_j(&s,20,0);
	sol_add_i_j(&s,30,0);

	for (int i = 0; i < s.n_bins; ++i) {
		printf("%s\n", bintostr(s.bins[i], &buffer, inst.w));
	}
	/* for (int i = 0; i < s.inst_ptr->n; ++i) { */
	/*     if (s.bin_of[i]) */
	/*         printf("item %d: bin%s\n", i,bintostr(*s.bin_of[i], &buffer, inst.w)); */
	/* } */
	sol_remove_item(&s,20);

	for (int i = 0; i < s.n_bins; ++i) {
		printf("%s\n", bintostr(s.bins[i], &buffer, inst.w));
	}

	printf("%s\n",soltostr(s,&buffer));

	/* sol trivial = sol_trivial(inst); */
	sol trivial;
	sol_alloc(trivial,inst);
	sol_trivial(&trivial,inst);
	/* sol_alloc(trivial,inst); */
	/* trivial = sol_trivial(inst); */
	printf("%s\n",soltostr(trivial,&buffer));
	/* printf("%s\n", bintostr(trivial->bins[0],&buffer,inst.w)); */
	free(buffer);
	return 0;
}
