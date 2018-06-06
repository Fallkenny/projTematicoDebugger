#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct archives {
	char nome[50];
};
typedef struct archives ARQUIVOS;

#pragma region Leitura de arquivos

ARQUIVOS* leArquivo(char *nomeArq, int* totalFunc) {

	FILE *arq = NULL;
	ARQUIVOS *archives = NULL;
	int i;

	FILE *file = NULL;
	file = fopen(nomeArq, "r");

	if (file == NULL) {
		printf("Erro na abertura do arquivo de entrada\n");
		exit(0);
	}

	char lin[50];
	while (fgets(lin, 50, file))
		*totalFunc += 1;

	fclose(file);


	arq = fopen(nomeArq, "r");

	fscanf(arq, "%d\n", totalFunc);
	archives = (ARQUIVOS*)malloc((*totalFunc) * sizeof(ARQUIVOS));

	for (i = 0; i < (*totalFunc); i++) {
		fgets(archives[i].nome, 50, arq);
		archives[i].nome[strlen(archives[i].nome) - 1] = '\0';

	}

	fclose(arq);

	return archives;
}

#pragma endregion

#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS