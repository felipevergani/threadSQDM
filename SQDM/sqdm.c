/*
Felipe Brun Vergani
Algoritmo que executa a soma das diferenças dos quadrados de 2 matrizes A e B. (SQDM)

		gcc -fopenmp sqdm.c -o sqdm.x -lm 
		./sqdm.x 5 > tabela.csv     
		executavel (n_de_threads)(largura_da_matriz) > nome.csv
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "omp.h"
#define quadrado 2

void printALL(double **a, int largura);

int main(int argc, char *argv[]){
	
	FILE *opn;
	opn = fopen("tabela.csv", "a+");
	
    int numThreads;
	
	double start_time, run_timeALOC, run_timeINIC, run_timePROCESS;
	int largura;
	largura = atoi(argv[2]);
	char X;
	
	//condicaoo para pegar sempre o maximo se nao tiver entrada no argv
    if (argc > 1) {
        numThreads = atoi(argv[1]);
    }else{
        numThreads = omp_get_max_threads();
    }
	
	
	
	
	if(opn == NULL){
		printf("Erro na abertura do arquivo");
		return 1;
	}
	
	
	//NUMEROS DE THREAD
	omp_set_num_threads(numThreads);
   //printf("Threads = %d\n", numThreads );
	

	start_time = omp_get_wtime();
	
	//ALOCACAO
	double **mA = (double **) malloc(largura * sizeof(double*));
    for(int i = 0; i < largura; i++){
        mA[i] = (double *) malloc(largura * sizeof(double));
    }
	
	double **mB = (double **) malloc(largura * sizeof(double*));
    for(int i = 0; i < largura; i++){
        mB[i] = (double *) malloc(largura * sizeof(double));
    }
	
	double **mX = (double **) malloc(largura * sizeof(double*));
    for(int i = 0; i < largura; i++){
        mX[i] = (double *) malloc(largura * sizeof(double));
    }
	
	double Resultado;
	
	
	
    
	
	//TEMPO DE ALOCACAO
	run_timeALOC = omp_get_wtime() - start_time;
   // printf("Tempo de alocação: %.20lf s\n", run_timeALOC);
	start_time = omp_get_wtime();
	
  
	
	int i, j;
	//inicializar as matrizes com valores random.
	#pragma omp parallel shared(i, j, mA, mB)
	{
	
		unsigned int threadId = omp_get_thread_num();
		//Randomizar valores da matriz.
		#pragma omp for
		for(i = 0; i < largura; i++){
			for(j = 0; j < largura; j++){
				mA[i][j] = rand_r(&threadId)%100;
				mB[i][j] = rand_r(&threadId)%100;
			}
		}
	}
	
	run_timeINIC = omp_get_wtime() - start_time;
    //printf("Tempo de inicialização: %.20lf s\n", run_timeINIC);

    start_time = omp_get_wtime();
	
	//printALL(mA, largura);
	//printALL(mB, largura);
	
	//Realiza o quadrado de cada elemento nas matrizes.
	#pragma omp parallel for private(j) shared(mA, mB, i)
	for(i = 0; i < largura; i++){
		for(j = 0; j < largura; j++){
			mA[i][j] = pow(mA[i][j], quadrado);
			mB[i][j] = pow(mB[i][j], quadrado);
		}
	}
	
	
	//(X = A^2 - B^2);
	#pragma omp parallel for private(j) shared(mX, i)
	for(i = 0; i < largura; i++){
		for(j = 0; j < largura; j++){
			mX[i][j] = (mA[i][j] - mB[i][j]);
		}
	}
	
	
	//Somar todos valores da matriz X
	for(i = 0; i < largura; i++){
		for(j = 0; j < largura; j++){
			Resultado += mX[i][j];
		}
	}
	
	
	run_timePROCESS = omp_get_wtime() - start_time;
    //printf("Tempo de processamento: %.20lf s\n", run_timePROCESS);

	//printALL(mA, largura);
	//printALL(mB, largura);
	//printALL(mX, largura);
	
	
	
	//printf("\nResultado: %lf\n", Resultado);
	//printf("Tempo\n");
	fprintf(opn,"%.20lf\n", run_timeALOC+run_timeINIC+run_timePROCESS);
	fclose(opn);
	return 0;
}

void printALL(double **a, int largura){
    int i, j;
    for(i = 0; i < largura; i++){
        for(j = 0; j < largura; j++){
			printf("%lf; ;", a[i][j]);
		}
		printf("\n");
    }	
	printf("\n");
    return;
}