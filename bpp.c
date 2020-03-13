#include <stdio.h>
#include <string.h>
#include "bpp.h"

int read_instance(char * path, bpp * instance){
	FILE* file; fopen_s(&file, path, "r");
	if (file == NULL) {
		printf("Arquivo nao existente:\npath: %s\n", path);
		return 1;
	}
	fscanf(file, "%zd%d", &instance->n, &instance->C);
	if (instance == NULL) instance_alloc_ptr(instance);
	instance_alloc(*instance);

	instance->w_sum = 0;
	for (int i = 0; i < instance->n; ++i) {
		fscanf(file, "%d", &(instance->w)[i]);
		instance->w_sum += (instance->w)[i];
	}

	fclose(file);

	return 1;
}

static inline int lower_bound(const bpp instance){
	return (instance.w_sum % instance.C) == 0 ?		\
			instance.w_sum/instance.C : instance.w_sum/instance.C + 1;
}


void benchmark(char * path /*, solver solvers[] */){
	bpp * inst;
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

		printf("C: %d\n"\
				"w_sum: %d\n",inst->C,inst->w_sum);
		printf("[ ");
		for (int i = 0; i < inst->n; ++i) {
			printf("%d ", inst->w[i]);
		}
		printf("]\n");
		printf("LB : %d\n", lower_bound(*inst));
	}

	closedir(dir);
}

int main(int argc, char *argv[]){
	char * path = argv[1];
	if (path==NULL) {
		path = "instances/Hard28/";
	}
	
	benchmark(path);

	return 0;
}
