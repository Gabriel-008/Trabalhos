#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct IndiceEndereco{
    char cep[8];
    unsigned int posicao;
}IndiceEndereco;

typedef struct Endereco{
	char logradouro[72];
	char bairro[72];
	char cidade[72];
	char uf[72];
	char sigla[2];
	char cep[8];
	char lixo[2];
}Endereco;


int compara (const void *a, const void *b){
    return strncmp(((IndiceEndereco*)a)->cep,((IndiceEndereco*)b)->cep,8);
}

int busca (IndiceEndereco *v, int num, char *cep){
    long inicio = 0;
    long fim = num -1;
    
    while(inicio <= fim){
        long meio = (inicio+fim)/2;
        int comp = strncmp(cep,v[meio].cep, 8);

        if(comp==0)
            return v[meio].posicao;
        if(comp>0)
            inicio = meio + 1;
        else
             fim = meio + 1;
    }
    return -1;
}

int main (int argc, char *argv[]){
    FILE *f, *fi;
    int i = 0;
    Endereco e;

    f = fopen("cep.dat", "rb");
    fi = fopen("indice.dat", "wb");

    if(argc!=2){
        fprintf(stderr, "Quantidade de argumentos invalida!");
        return 1;
    }

    if(f==NULL||fi==NULL){
        fprintf(stderr, "\nErro ao abrir arquivos");
        return 1;
    }

    fseek(f,0,SEEK_END);
    long tam = ftell(f);
    printf("Tamanho do arquivo: %ld\n", tam);
    rewind(f);

    long qtd = (tam/sizeof(Endereco));
    IndiceEndereco *a = (IndiceEndereco*)malloc(qtd*sizeof(IndiceEndereco));
    int leu = fread(&e,sizeof(Endereco), 1, f);

    printf("Primeiro CEP bruto: ");
    fwrite(e.cep, 1, 8, stdout);
    printf("\n");

    while(leu>0){
        strncpy(a[i].cep, e.cep,8);
        a[i].posicao = i;
        leu = fread(&e,sizeof(Endereco), 1, f);
        i++;
    }
    qsort(a, i, sizeof(IndiceEndereco), compara);
    fwrite(a, sizeof(IndiceEndereco), i, fi);
    
    fclose(fi);

    int posicao = busca(a,i,argv[1]);
    if(posicao == -1)
        printf("\nCEP nao encontrado");
    else{
        printf("\nCEP encontrado");
        fseek(f,posicao*sizeof(Endereco), SEEK_SET);
        fread(&e, sizeof(Endereco), 1, f);

        printf("CEP: %.8s\n", e.cep);
        printf("Logradouro: %.72s\n", e.logradouro);
        printf("Bairro: %.72s\n", e.bairro);
        printf("Cidade: %.72s\n", e.cidade);
        printf("UF: %.72s\n", e.uf);
        printf("Sigla: %.2s\n", e.sigla);
    }

    fclose(f);
    return 0;
}
    
