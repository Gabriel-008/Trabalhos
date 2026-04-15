#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100 

int compara(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

int blocos(FILE *entrada, int k, int tamanho_bloco) {
    int buffer[MAX];
    int i, leu;
    char nome[20];

    for (i = 0; i < k; i++) {
        leu = fread(buffer, sizeof(int), tamanho_bloco, entrada);
        if (leu == 0) 
            break;

        qsort(buffer, leu, sizeof(int), compara);

        sprintf(nome, "bloco_%d.dat", i);
        FILE *saida = fopen(nome, "wb");
        fwrite(buffer, sizeof(int), leu, saida);
        fclose(saida);
    }

    return i;
}

void intercala(char *arq1, char *arq2, char *saida) {
    FILE *f1 = fopen(arq1, "rb");
    FILE *f2 = fopen(arq2, "rb");
    FILE *fout = fopen(saida, "wb");

    int a, b;
    int leu1 = fread(&a, sizeof(int), 1, f1);
    int leu2 = fread(&b, sizeof(int), 1, f2);

    while (leu1 ==1 && leu2 ==1) {
        if (a <= b) {
            fwrite(&a, sizeof(int), 1, fout);
            leu1 = fread(&a, sizeof(int), 1, f1);
        } else {
            fwrite(&b, sizeof(int), 1, fout);
            leu2 = fread(&b, sizeof(int), 1, f2);
        }
    }

    while (leu1 ==1) {
        fwrite(&a, sizeof(int), 1, fout);
        leu1 = fread(&a, sizeof(int), 1, f1);
    }

    while (leu2 ==1) {
        fwrite(&b, sizeof(int), 1, fout);
        leu2 = fread(&b, sizeof(int), 1, f2);
    }

    fclose(f1);
    fclose(f2);
    fclose(fout);
}

void merge(int numBlocos) {
    int passo = 0;
    char nome1[20], nome2[20], saida[20];

    while (numBlocos > 1) {
        int i, novoTot = 0;

        for (i = 0; i < numBlocos; i += 2) {
            sprintf(nome1, "bloco_%d.dat", i);

            if (i + 1 < numBlocos) {
                sprintf(nome2, "bloco_%d.dat", i + 1);
                sprintf(saida, "temp_%d_%d.dat", passo, novoTot);

                intercala(nome1, nome2, saida);
            } else {
                sprintf(saida, "temp_%d_%d.dat", passo, novoTot);
                rename(nome1, saida);
            }

            novoTot++;
        }

        for (int i = 0; i < novoTot; i++) {
            sprintf(nome1, "temp_%d_%d.dat", passo, i);
            sprintf(nome2, "bloco_%d.dat", i);
            rename(nome1, nome2);
        }

        numBlocos = novoTot;
        passo++;
    }

    printf("\nArquivo final ordenado: bloco_0.dat");
}

int main() {
    FILE *entrada = fopen("Arquivo.dat", "rb");

    if (entrada==NULL) {
        fprintf(stderr, "\nErro ao abrir arquivo!");
        return 1;
    }

    int k = 4;
    int tamBloco = 100;

    int numBlocos = blocos(entrada, k, tamBloco);
    fclose(entrada);

    merge(numBlocos);

    return 0;
}
