#include <stdio.h>
#include <stdlib.h>

int read_instance(char * path, int * C, int * w, int * wSum){
	FILE* file;
	fopen_s(&file, path, "r");
	if (file == NULL) {
		printf("Arquivo nao existente:\npath: %s\n", path);
		return -1;
	}
	int n;
	fscanf_s(file, "%d%d", &n, C);
	w = (int *) malloc(n*sizeof(int));

	*wSum = 0;
	for (int i = 0; i < n; ++i) {
		fscanf_s(file, "%d", &w[i]);
		*wSum += w[i];
	}
	fclose(file);

	printf("C: %d\n"\
			"wSum: %d\n",*C,*wSum);
	printf("[ ");
	for (int i = 0; i < n; ++i) {
		printf("%d ", w[i]);
	}
	printf("]");
	return 1;

}

int main(int argc, char *argv[]){
	int C, *w, wSum;
	char path[] = "D:\\\\Codigos\\metaheuristics\\instances\\Hard28\\Hard28_BPP14.txt";
	
	read_instance(path,&C,w,&wSum);

	return 0;
}
