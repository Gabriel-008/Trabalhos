#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Endereco{
	char logradouro[72];
	char bairro[72];
	char cidade[72];
	char uf[72];
	char sigla[2];
	char cep[8];
	char lixo[2];
}Endereco;

int compara(const void *a, const void *b){
    return strncmp(((Endereco*)a)->cep,((Endereco*)b)->cep,8);
}

int main(){

    FILE *f;
    f = fopen("cep.dat", "rb");

    if(f==NULL){
        fprintf(stderr, "\nErro ao abrir o arquivo!");
        return 1;
    }

    fseek(f, 0, SEEK_END);
    long tamArq = ftell(f);
    long qtdReg = tamArq/sizeof(Endereco);
    rewind(f);

    Endereco *v = (Endereco*)malloc(qtdReg*sizeof(Endereco));
    fread(v, sizeof(Endereco), qtdReg, f);

    fclose(f);

    long meio = qtdReg/2; 

    qsort(v, meio, sizeof(Endereco), compara);
    qsort(v, qtdReg-meio, sizeof(Endereco), compara);

    for(long i = 0; i < meio; i++){
        printf("%.8s", v[i].cep);
    }

    for(long i = meio; i < qtdReg; i++){
        printf("%.8s", v[i].cep);
    }

    return 0;
    }
