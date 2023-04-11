/*
 * Atividade 15 - Computacao Grafica
 * Codigo OpenGL responsavel por salvar a area de desenho em um arquivo
 * Autor: Enzo Eduardo Cassiano Ibiapina
 * Data de apresentacao: 28/02/2023
 * Ultima modificacao: 11/04/2023
*/

#ifndef salvaimagem_h
#define salvaimagem_h


/*
 * Funcao que salva a area de pintura como um .ppm ASCII
 */
void exportaP3PPM(float areaDesenho[], int qntValoresCor)
{
	FILE *arq = fopen("tmpP3.ppm", "wb");
	fprintf(arq, "P3\n# Comentario do header\n%d %d\n255\n", width, height-50);

	for (int i = 0; i < qntValoresCor; i++)
	{
		// Escreve o valor de cada canal de cor de cada pixel no arquivo
		fprintf(arq, "%d\n", (int) round(255 * areaDesenho[i]));
	}

	fclose(arq);
	printf("ESCRITA TERMINADA!\n");
}


/*
 * Funcao que salva a area de pintura como um .ppm em binario
 */
void exportaP6PPM(float areaDesenho[], int qntValoresCor)
{
	FILE *arq = fopen("tmpP6.ppm", "wb");
	fprintf(arq, "P6\n# Comentario do header\n%d %d\n255\n", width, height-50);

	for (int i = 0; i < qntValoresCor; i++)
	{
		// A cada 3 valores RGB (ou a cada pixel)
		if (i % 3 == 0)
		{
			unsigned char corPixelByte[3];
			corPixelByte[0] = (int) round(255 * areaDesenho[i]);
			corPixelByte[1] = (int) round(255 * areaDesenho[i+1]);
			corPixelByte[2] = (int) round(255 * areaDesenho[i+2]);

			// Escreve os bytes equivalentes a cor do pixel no arquivo 
			fwrite(corPixelByte, 1, 3, arq);
		}
	}

	fclose(arq);
	printf("ESCRITA TERMINADA!\n");
}

#endif