#include <stdio.h>
#include <string.h>

typedef struct IndiceEndereco{
    char CEP[8];
    unsigned int posicao;
}IndiceEndereco;

typedef struct _Endereco Endereco;

struct _Endereco
{
	char logradouro[72];
	char bairro[72];
	char cidade[72];
	char uf[72];
	char sigla[2];
	char cep[8];
	char lixo[2];
};

int main (int argc, char** argv[]){
    FILE *f;

    f = fopen("cep.dat", "rb");
    if(f==NULL){
        fprintf(stderr, "Erro ao abrir o arquivo");
        return 1;
    }
    fseek(f,0,SEEK_END);

    long tam = ftell(f);
    rewind(f);
    long qtd = (tam/sizeof(Endereco));
    IndiceEndereco a[] = (IndiceEndereco*)malloc(qtd*sizeof(IndiceEndereco));

    while(fread(&a,sizeof(IndiceEndereco),1,f) > 0){
        printf("%s %d\n", a.CEP, a.posicao);


    }

  





    fclose(f);

}