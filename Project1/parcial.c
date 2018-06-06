//#include <stdio.h>
//#include <stdlib.h>
//
///*---------------------------------------------------------------------*/
//#define N 100
//#pragma pack(1)
//
///*---------------------------------------------------------------------*/
//struct cabecalho 
//{
//	unsigned short tipo;
//	unsigned int tamanho_arquivo;
//	unsigned short reservado1;
//	unsigned short reservado2;
//	unsigned int offset;
//	unsigned int tamanho_image_header;
//	int largura;
//	int altura;
//	unsigned short planos;
//	unsigned short bits_por_pixel;
//	unsigned int compressao;
//	unsigned int tamanho_imagem;
//	int largura_resolucao;
//	int altura_resolucao;
//	unsigned int numero_cores;
//	unsigned int cores_importantes;
//}; 
//typedef struct cabecalho CABECALHO;
//
//struct pixel
//{
//	unsigned char B;
//	unsigned char G;
//	unsigned char R;
//	
//};
//typedef struct pixel PIXEL;
//
///*---------------------------------------------------------------------*/
//CABECALHO le_cabecalho_arquivo(char entrada[N])
//{
//
//	CABECALHO cabecalho;
//
//	FILE *f = fopen(entrada, "rb");
//
//	if ( f == NULL ){
//		printf("Erro ao abrir o arquivo %s\n", entrada);
//		exit(0);
//	}  
//
//	fread(&cabecalho, sizeof(CABECALHO), 1, f);
//
//	printf("Tamanho da imagem: %u\n", cabecalho.tamanho_arquivo);
//	printf("Largura: %d\n", cabecalho.largura);
//	printf("Altura: %d\n", cabecalho.altura);
//	printf("Bits por pixel: %d\n", cabecalho.bits_por_pixel);
//
//	fclose(f);
//
//    return cabecalho;
//}
//
///* * * * inicio: funcoes marcadas no  "to do" * * * * */
//PIXEL** alocaMatriz(int coluna, int linha)
//{
//    PIXEL** mat;
//    int i;
//
//    mat = (PIXEL **)malloc(coluna*sizeof(PIXEL*));
//    for (i=0; i<linha; i++)
//    {
//        mat[i] = (PIXEL *)malloc(coluna*sizeof(int));
//    }
//
//    return mat;
//}
//
//void lePixels(char entrada[N], CABECALHO header, PIXEL** mat)
//{
//    int i, j;
//    FILE* arq = NULL;
//    PIXEL p;
//
//    arq = fopen(entrada, "rb");
//    if (arq == NULL)
//    {
//        printf(" !! Nao foi possível abrir o arquivo !!");
//        exit(0);
//    }
//
//    fseek(arq, sizeof(header), SEEK_SET);
//    for (i=0; i<header.altura; i++)
//    {
//        for (j=0; j<header.largura; j++)
//        {
//            fread(&p, sizeof(PIXEL), 1, arq);
//            mat[i][j] = p;
//        }
//    }
//    fclose(arq);
//}
//
//void converteTonsCinza(PIXEL** mat, int altura, int largura)
//{
//    unsigned int media;
//    int i, j;
//    for (i=0; i<altura; i++)
//    {
//        for (j=0; j<largura; j++)
//        {
//            media = (mat[i][j].R + mat[i][j].G + mat[i][j].B)/3;
//            mat[i][j].R = media;
//            mat[i][j].G = media;
//            mat[i][j].B = media;
//        }
//    }
//}
//
//void gravaArquivoBMP(CABECALHO cab, PIXEL** mat, char destino[N])
//{
//    int i,j;
//    FILE* output = fopen(destino, "wb");
//    fwrite(&cab, sizeof(CABECALHO), 1, output);
//    for (i=0; i<cab.altura; i++)
//    {
//        for (j=0; j<cab.largura; j++)
//        {
//            fwrite(&mat[i][j], sizeof(PIXEL), 1, output);
//        }
//    }
//    fclose(output);
//}
//
////esta função não funciona adequadamente.
//void desalocaMatriz (PIXEL** mat, int linha)
//{
//    int i;
//    for (i=0; i<linha; i++)
//    {
//        free(mat[i]);
//    }
//    free(mat);
//}
///* * * * fim: funcoes marcadas no  "to do" * * * * */
//
///*---------------------------------------------------------------------*/
//int main(int argc, char **argv ){
//    PIXEL** matriz;
//    CABECALHO cabecalho;
//
//	if ( argc != 3 ){
//		printf("%s <arquivo_de_entrada> <arquivo_de_Saida>\n", argv[0]);
//		exit(0);
//	}
//
//    /* código adicionado em 17/05: */
//    cabecalho = le_cabecalho_arquivo(argv[1]);
//    matriz = alocaMatriz(cabecalho.altura, cabecalho.largura);
//    lePixels(argv[1], cabecalho, matriz);
//    converteTonsCinza(matriz, cabecalho.altura, cabecalho.largura);
//    //binarizarImagem <- implementar (Otsu ou K-Means)
//    gravaArquivoBMP(cabecalho, matriz, argv[2]);
//    desalocaMatriz(matriz, cabecalho.altura); //não funciona adequadamente
//    /* fim código adicionado em 17/05. */
//
//    //status:
//    //ler cabecalho............ok, modificado do prof
//    //alocar matriz............ok
//    //ler pixels...............ok
//    //converter tons cinza.....ok
//    //binarizar imagem.........to do      
//    //gravar BMP...............ok
//    //desalocar matriz.........to do
//	
//
//}
///*---------------------------------------------------------------------*/
