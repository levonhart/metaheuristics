#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include "bpp.h"
#include "sol.h"
#include "solvers.h"

#define BUFFSIZE 1024

int read_instance(char * path, bpp * instance){
	FILE* file; fopen_s(&file, path, "r");
	if (file == NULL) { perror("File Opening Error"); return -1; }

	if ( !fscanf(file, "%zu%zu", &instance->n, &instance->C) ) return -2;
	if (instance->n == 0 || instance->C == 0){ perror("Wrong Formating"); return -2; }

	if (instance == NULL) instance_alloc_ptr(instance);
	instance_alloc(*instance);

	instance->w_sum = 0;
	for (size_t i = 0; i < instance->n; ++i) {
		if( fscanf(file, "%d", &(instance->w)[i]) == EOF ){ perror("Wrong Formating"); return -2; }
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
	for (size_t i = 0; i < instance.n; ++i) {
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


void benchmark(char * path , solver ** solvers, int n_solvers){
	if (!path) return;
	bpp * inst; char * buffer = NULL, buff[BUFFSIZE];
	sol curr; size_t sum_bins; int n_instances = 0;
	FILE * logfile = fopen("log.txt","a");
	clock_t t0, t1;
	LIST_HEAD(instances);

	DIR * dir = opendir(path);
	struct dirent * entry;
	entry = readdir(dir);
	while ( (entry = readdir(dir)) ) {
		if(entry->d_name[0] == '.') continue;
		instance_alloc_ptr(inst);
		instance_init0(*inst);
		char relative_path[strlen(entry->d_name)+strlen(path)+1];
		strcpy(relative_path, path);
		if (relative_path[strlen(relative_path)-1]!='/')
		strcat(relative_path,"/");
		strcat(relative_path, entry->d_name);

		if(read_instance(relative_path, inst) == 0){
			printf("Instance found: %s\n", relative_path);
			list_add_tail(&inst->list, &instances);
			n_instances++;
		}
		inst = NULL;
	}
	if (n_instances == 0){
		printf("No instance Found\n");
		return;
	}
	struct list_head * iter;
	for (int i = 0; i < n_solvers; ++i) {
		int j=0;
		if (solvers[i]) {
			sum_bins = 0;
			t0 = clock();
			list_for_each(iter, &instances){
				inst = list_entry(iter, bpp, list);
				sol_alloc(curr,*inst);
				sol_trivial(&curr);
				solver_start(solvers[i],&curr);
				/* printf(" ==============================================\n" */
				/*         "sol %d : %s\n" */
				/*         "----------------------------------------------\n",i,soltostr(curr,&buffer)); */
				printf("\rSolving... %3d%% completed",++j*100/n_instances);
				fflush(stdout);
				sum_bins += curr.n_bins;
				sol_destroy(curr);
			}
			t1 = clock();
			snprintf(buff,BUFFSIZE,"solver %d t(%s) : %lf\t\t execution time: % .5lf secs\n",i,
					solvers[i]->t == t_hc ? "HC" :
					solvers[i]->t == t_vnd ? "VND" :
					solvers[i]->t == t_rms ? "RMD" :
					solvers[i]->t == t_ils ? "RMD" :
					"ERROR",
					sum_bins/(1.0*n_instances),
					(double) (t1 - t0) / CLOCKS_PER_SEC);
			printf("\n%s",buff);
			fputs(buff,logfile);
		}
	}

	free(inst); free(buffer);
	closedir(dir);
}

int main(int argc, char *argv[]){
	char * path = NULL;
	char * buffer = NULL;
	int n_sl = 2;
	if (argc>1) {
		path = argv[1];
	} else {
		printf("usage: bbp <path>\n\n   <path>\t\tPath to directory with instances of the bin packing problem\n");
		exit(-1);
	}

	solver ** solvers = (solver**) calloc(n_sl, sizeof(solver*));
	hc * HC;
	hc_alloc(HC);
	hc_init(*HC);
	solvers[0] = (solver*) HC;

	rms * RMS;
	rms_alloc(RMS);
	rms_init(*RMS,5);
	solvers[1] = (solver*) RMS;
	printf("%u\n", RMS->ite);


	benchmark(path, solvers, n_sl);

	free(buffer);
	return 0;
}
