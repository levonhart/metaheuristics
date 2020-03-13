#include <stdio.h>
#include <stdlib.h>

/*! \struct bpp_t
 *  \brief Instance of Bin Packing Problem
 *
 *  Guarda uma instancia do BBP, ou seja, a capaciade máxima do pacote, os pesos dos itens,
 *  o numero de itens e a soma total de pesos.
 */
typedef struct bpp_t {
	int C, w_sum;
	int * w;
	size_t n;
} bpp;

int read_instance(char * path, bpp * instance){
	FILE* file;
	fopen_s(&file, path, "r");
	if (file == NULL) {
		printf("Arquivo nao existente:\npath: %s\n", path);
		return -1;
	}
	fscanf_s(file, "%d%d", &instance->n, &instance->C);
	instance->w = (int *) malloc(instance->n*sizeof(int));

	instance->w_sum = 0;
	for (int i = 0; i < instance->n; ++i) {
		fscanf_s(file, "%d", &(instance->w)[i]);
		instance->w_sum += (instance->w)[i];
	}
	fclose(file);

	return 1;

}

int main(int argc, char *argv[]){
	// int C, *w, wSum;
	bpp inst;
	char path[] = "D:\\\\Codigos\\metaheuristics\\instances\\Hard28\\Hard28_BPP14.txt";
	
	read_instance(path, &inst);

	printf("C: %d\n"\
			"wSum: %d\n",inst.C,inst.w_sum);
	printf("[ ");
	for (int i = 0; i < inst.n; ++i) {
		printf("%d ", inst.w[i]);
	}
	printf("]");
	return 0;
}
