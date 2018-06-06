//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//
//#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS
//
//struct archives {
//	char nome[50];
//};
//typedef struct archives ARQUIVOS;
//
//ARQUIVOS* leArquivo(char *nomeArq, int* totalFunc)
//{
//
//	FILE *arq = NULL;
//	ARQUIVOS *archives = NULL;
//	int i;
//
//	FILE *file = NULL;
//	file = fopen(nomeArq, "r");
//
//	if (file == NULL) {
//		printf("Erro na abertura do arquivo de entrada\n");
//		exit(0);
//	}
//
//	char lin[50];
//	while (fgets(lin, 50, file))
//		*totalFunc += 1;
//
//	fclose(file);
//
//
//	arq = fopen(nomeArq, "r");
//
//	fscanf(arq, "%d\n", totalFunc);
//	archives = (ARQUIVOS*)malloc((*totalFunc) * sizeof(ARQUIVOS));
//
//	for (i = 0; i < (*totalFunc); i++) {
//		fgets(archives[i].nome, 50, arq);
//		archives[i].nome[strlen(archives[i].nome) - 1] = '\0';
//
//	}
//
//	fclose(arq);
//
//	return archives;
//}
//
////---------------------------------------------------------------------------------------------
//
//int main(int argc, char **argv)
//{
//	if (_DEBUG)
//	{
//		argc--;
//		argv += sizeof(char);
//	}
//
//
//	ARQUIVOS *f = NULL;
//	int i, total = 0;
//	if (argc != 1)
//	{
//		printf("Numeros de parametros invalido\n");
//	}
//	else {
//		f = leArquivo(argv[1], &total);
//
//		for (i = 0; i < total; i++)
//			printf("%s\n", f[i].nome);
//		free(f);
//	}
//
//}
//
