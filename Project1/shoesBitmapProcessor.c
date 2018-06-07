#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS


#define N 300
#pragma pack(1)

#pragma region Archives Struct

struct archives {
	char nome[N];
};
typedef struct archives ARQUIVOS;

#pragma endregion

#pragma region Cabecalho Struct


struct cabecalho
{
	unsigned short tipo;
	unsigned int tamanho_arquivo;
	unsigned short reservado1;
	unsigned short reservado2;
	unsigned int offset;
	unsigned int tamanho_image_header;
	int largura;
	int altura;
	unsigned short planos;
	unsigned short bits_por_pixel;
	unsigned int compressao;
	unsigned int tamanho_imagem;
	int largura_resolucao;
	int altura_resolucao;
	unsigned int numero_cores;
	unsigned int cores_importantes;
};
typedef struct cabecalho CABECALHO;

#pragma endregion

#pragma region Pixel Struct

struct pixel
{
	unsigned char B;
	unsigned char G;
	unsigned char R;

};
typedef struct pixel PIXEL;

#pragma endregion

#pragma region Leitura de arquivo texto

ARQUIVOS* leArquivo(char *nomeArq, int* totalFunc)
{

	FILE *arq = NULL;
	ARQUIVOS *archives = NULL;
	int i;

	FILE *file = NULL;
	file = fopen(nomeArq, "r");

	if (file == NULL) {
		printf("Erro na abertura do arquivo de entrada\n");
		exit(0);
	}

	char lin[N];
	while (fgets(lin, N, file))
		*totalFunc += 1;

	fclose(file);


	arq = fopen(nomeArq, "r");

	fscanf(arq, "%d\n", totalFunc);
	archives = (ARQUIVOS*)malloc((*totalFunc) * sizeof(ARQUIVOS));

	for (i = 0; i < (*totalFunc); i++) {
		fgets(archives[i].nome, N, arq);
		archives[i].nome[strlen(archives[i].nome) - 1] = '\0';

	}

	fclose(arq);

	return archives;
}

#pragma endregion

#pragma region Leitura de Arquivo bmp

CABECALHO le_cabecalho_arquivo(char entrada[N])
{
	CABECALHO cabecalho;

	FILE *f = fopen(entrada, "rb");

	if (f == NULL) {
		printf("Erro ao abrir o arquivo %s\n", entrada);
		exit(0);
	}

	fread(&cabecalho, sizeof(CABECALHO), 1, f);

	printf("Tamanho da imagem: %u\n", cabecalho.tamanho_arquivo);
	printf("Largura: %d\n", cabecalho.largura);
	printf("Altura: %d\n", cabecalho.altura);
	printf("Bits por pixel: %d\n", cabecalho.bits_por_pixel);

	fclose(f);

	return cabecalho;
}

#pragma endregion

#pragma region Processamento BMP

void desalocaMatriz (PIXEL** mat, int linha)
{
    int i;
    for (i=0; i<linha; i++)
    {
        free(mat[i]);
    }
    free(mat);
}

void gravaArquivoBMP(CABECALHO cab, PIXEL** mat, char destino[N])
{
	int i, j;
	FILE* output = fopen(destino, "wb");
	fwrite(&cab, sizeof(CABECALHO), 1, output);
	for (i = 0; i<cab.altura; i++)
	{
		for (j = 0; j<cab.largura; j++)
		{
			fwrite(&mat[i][j], sizeof(PIXEL), 1, output);
		}
	}
	fclose(output);
}

void converteTonsCinza(PIXEL** mat, int altura, int largura)
{
    unsigned int media;
    int i, j;
    for (i=0; i<altura; i++)
    {
        for (j=0; j<largura; j++)
        {
            media = (mat[i][j].R + mat[i][j].G + mat[i][j].B)/3;
            mat[i][j].R = media;
            mat[i][j].G = media;
            mat[i][j].B = media;
        }
    }
}


PIXEL** alocaMatriz(int coluna, int linha)
{
	PIXEL** mat;
	int i;

	mat = (PIXEL **)malloc(coluna * sizeof(PIXEL*));
	for (i = 0; i<linha; i++)
	{
		mat[i] = (PIXEL *)malloc(coluna * sizeof(int));
	}

	return mat;
}

void lePixels(char entrada[N], CABECALHO header, PIXEL** mat)
{
	int i, j;
	FILE* arq = NULL;
	PIXEL p;

	arq = fopen(entrada, "rb");
	if (arq == NULL)
	{
		printf(" !! Nao foi possível abrir o arquivo !!");
		exit(0);
	}

	fseek(arq, sizeof(header), SEEK_SET);
	for (i = 0; i<header.altura; i++)
	{
		for (j = 0; j<header.largura; j++)
		{
			fread(&p, sizeof(PIXEL), 1, arq);
			mat[i][j] = p;
		}
	}
	fclose(arq);
}

void processaListaBMP(ARQUIVOS* lista, int tamLista)
{
	int i = 0;
	int tam = tamLista;
	
	for (i = 0; i < tam; i++)
	{
		PIXEL** matriz;
		CABECALHO cabecalho;
		cabecalho = le_cabecalho_arquivo(lista[i].nome);
		matriz = alocaMatriz(cabecalho.altura, cabecalho.largura);
		lePixels(lista[i].nome, cabecalho, matriz);
		converteTonsCinza(matriz, cabecalho.altura, cabecalho.largura);
		////binarizarImagem <- implementar (Otsu ou K-Means)
		gravaArquivoBMP(cabecalho, matriz, lista[i].nome);
		desalocaMatriz(matriz, cabecalho.altura); //não funciona adequadamente
	}

}

#pragma endregion


#pragma region Funções Diversas


#pragma endregion


#pragma region Main

int main(int argc, char **argv)
{
	if (!_DEBUG)
	{
		argc--;
		argv += sizeof(char);
	}


	ARQUIVOS *listaBMP = NULL;
	int i, total = 0;
	if (argc != 2)
	{
		printf("Numeros de parametros invalido\n");
		exit(0);
	}
	
	listaBMP = leArquivo(argv[1], &total);

	for (i = 0; i < total; i++)
		printf("%s\n", listaBMP[i].nome);

	processaListaBMP(listaBMP, total);

	printf("Finalizado!");

	free(listaBMP);


}

#pragma endregion
