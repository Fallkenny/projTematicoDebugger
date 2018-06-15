#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> 

#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS

#define N 300
#pragma pack(1)

// #pragma region Archives Struct

struct archives {
	char nome[N];
};
typedef struct archives ARQUIVOS;

// #pragma endregion

// #pragma region Cabecalho Struct


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

// #pragma endregion

// #pragma region Pixel Struct

struct pixel
{
	unsigned char B;
	unsigned char G;
	unsigned char R;

};
typedef struct pixel PIXEL;

// #pragma endregion

// #pragma region Leitura de arquivo texto

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

// #pragma endregion

// #pragma region Leitura de Arquivo bmp

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

// #pragma endregion

// #pragma region Processamento BMP

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
		printf(" !! Nao foi poss�vel abrir o arquivo !!");
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

/* EXPERIMENTAL */
//gerarHistograma: gera um histograma dos tons de cinza utilizados.
//é chamada na função binarizarImagem.
//recebe como parâmetros a matriz a ser avaliada, um ponteiro para vetor
//do tipo Float de 256 posições, e a altura e largura da matriz. 
void gerarHistograma (PIXEL** matriz, float *histograma, int h, int l)
{
	int ocorrencias[256];
	int i, x, y, totalPixels=0, cor;

	for (i=0; i<256; i++)
	{
		histograma[i] = 0;
		ocorrencias[i] = 0;
	}

	for (y=0; y<h; y++)
	{
		for (x=0; x<l; x++)
		{
			cor = matriz[y][x].R;
			ocorrencias[cor]++;
			totalPixels++;
		}
	}

	for (i=0; i<256; i++)
	{
		histograma[i] =  (float) ocorrencias[i] / (float) totalPixels;
	}

	//return histograma;
}

//método de Otsu: recebe o histograma e o total de pixels, e retorna o limiar.
//é chamado na função binarizarImagem. fonte:
// https://stackoverflow.com/questions/33693423/otsu-thresholding
int metodoOtsu(float *histogram, int total_pixels) {
    double probability[256], mean[256];
    double max_between, between[256];
    int threshold;


    /*
    probability = class probability
    mean = class mean
    between = between class variance
    */

    for(int i = 0; i < 256; i++) {
        probability[i] = 0.0;
        mean[i] = 0.0;
        between[i] = 0.0;
    }

    probability[0] = histogram[0];

    for(int i = 1; i < 256; i++) {
        probability[i] = probability[i - 1] + histogram[i];
        mean[i] = mean[i - 1] + i * histogram[i];
    }

    threshold = 0;
    max_between = 0.0;

    for(int i = 0; i < 255; i++) {
        if(probability[i] != 0.0 && probability[i] != 1.0)
            between[i] = pow(mean[255] * probability[i] - mean[i], 2) / (probability[i] * (1.0 - probability[i]));
		else
			between[i] = 0.0;
			if(between[i] > max_between) {
				max_between = between[i];
				threshold = i;
			}
    }

    return threshold;
}

void binarizarImagem(PIXEL** matriz, int h, int l)
{
	PIXEL black, white;
	int x, y, limiar;
	float histograma[256];
	black.R = black.G = black.B = 0;
	white.R = white.G = white.B = 255;

	gerarHistograma(matriz, histograma, h, l);
	limiar = metodoOtsu(histograma, h*l);
	for (x=0; x<l; x++)
	{
		for (y=0; y<h; y++)
		{
			if (matriz[x][y].R > limiar)
				matriz[x][y] = white;
			else
				matriz[x][y] = black;
		}
	}
}


//expandirPixels: faz a expansão de um pixel preto nos 8 pixels adjacentes, tornando-os pretos.
//é chamada em processaListaBMP. 
//Recebe como parâmetros a matriz binarizada, a nova matriz, e o tamanho. 
void expandirPixels (PIXEL** original, PIXEL** nova, int h, int l)
{
	PIXEL black;
	PIXEL eval;
	black.R = black.G = black.B = 0;

	int x, y;
	for (y=1; y<h-1; y++)
	{
		for (x=1; x<l-1; x++)
		{
			eval = original[x][y];
			nova[x][y]=original[x][y];
			if (eval.R == 0 && eval.G == 0 && eval.B == 0)
			{
				nova[x-1][y-1] = black;
				nova[x-1][y]   = black;
				nova[x-1][y+1] = black;

				nova[x][y-1] = black;
				nova[x][y]   = black;
				nova[x][y+1] = black;

				nova[x+1][y-1] = black;
				nova[x+1][y]   = black;
				nova[x+1][y+1] = black;
			}
		}
	}
}

//experimental: transladar a matriz.
// typedef struct coordenada
// {
// 	int x;
// 	int y;
// } COORDENADA;
// void centralizaMatriz (PIXEL** original, PIXEL** centralizada, int h, int l)
// {
// 	COORDENADA maisAlto, maisBaixo, origem;
// 	origem.x = l/2;
// 	origem.y = h/2;


// }
/* FIM: EXPERIMENTAL */


void processaListaBMP(ARQUIVOS* lista, int tamLista)
{
	int i = 0;
	int tam = tamLista;

	for (i = 0; i < tam; i++)
	{
		PIXEL** matriz;
		PIXEL** matrizExpandida;
		CABECALHO cabecalho;
		cabecalho = le_cabecalho_arquivo(lista[i].nome);
		matriz = alocaMatriz(cabecalho.altura, cabecalho.largura);
		lePixels(lista[i].nome, cabecalho, matriz);
		converteTonsCinza(matriz, cabecalho.altura, cabecalho.largura);

		/* EXPERIMENTAL: binarização e expansão */
		// if (i==4)
		// {
			matrizExpandida = alocaMatriz(cabecalho.altura, cabecalho.largura);

			binarizarImagem(matriz, cabecalho.altura, cabecalho.largura);


			expandirPixels(matriz, matrizExpandida, cabecalho.altura, cabecalho.largura);
			//gravaArquivoBMP(cabecalho, matrizExpandida, "/Users/rafael/ucs/projetotematico/projTematicoDebugger/Project1/testeBuracoExp.bmp");
			//gravaArquivoBMP(cabecalho, matriz, "/Users/rafael/ucs/projetotematico/projTematicoDebugger/Project1/testeBuraco.bmp");

			gravaArquivoBMP(cabecalho, matrizExpandida, lista[i].nome);

			desalocaMatriz(matrizExpandida, cabecalho.altura);
		// }
		/* FIM: EXPERIMENTAL */

		// gravaArquivoBMP(cabecalho, matriz, lista[i].nome);
		desalocaMatriz(matriz, cabecalho.altura);
	}
}


// #pragma endregion


// #pragma region Fun��es Diversas


// #pragma endregion


// #pragma region Main

int main(int argc, char **argv)
{
	//wtf menino Fisher
	/*
	if (!_DEBUG)
	{
		argc--;
		argv += sizeof(char);
	}
	*/


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

	printf("Finalizado!\n");

	free(listaBMP);


}

// #pragma endregion
