#include <stdio.h>
#include <string.h>

typedef struct _Endereco Endereco;

struct _Endereco
{
	char logradouro[72];
	char bairro[72];
	char cidade[72];
	char uf[72];
	char sigla[2];
	char cep[8];
	char lixo[2]; // Ao Espaço no final da linha + quebra de linha
};

int main(int argc, char**argv)
{
	FILE *f;
	Endereco e;
	int qt;
	int c;

	if(argc != 2)
	{
		fprintf(stderr, "USO: %s [CEP]", argv[0]);
		return 1;
	}

	c = 0;
	printf("Tamanho da Estrutura: %ld\n\n", sizeof(Endereco));//Informa o tamanho da struct em bytes
	f = fopen("cep.dat","rb");//Retorna um ponteiro para o arquivo
	fseek(f,0,SEEK_END); //Move o cursor de leitura/escrita para o final do arquivo

	long tamanhoBytes = ftell(f); //Retorna o tamanho do arquivo em bytes, já que o cursor está no final
	long tamanhoRegistros = tamanhoBytes/sizeof(Endereco);//Informa a quantidade de registros no arquivo
	long inicio = 0; //Posição do primeiro registro do arquivo
	long fim = tamanhoRegistros-1;
	
	//qt = fread(&e,sizeof(Endereco),1,f);	

	while(inicio <= fim)
	{
		long meio = (inicio+fim)/2;
		fseek(f,meio*sizeof(Endereco),SEEK_SET);
		fread(&e,sizeof(Endereco),1,f);
		int resultado = strncmp(argv[1],e.cep,8);

		// argv[1] < e.cep  => strcmp(argv[1],e.cep) < 0
		// argv[1] > e.cep  => strcmp(argv[1],e.cep) > 0
		// argv[1] == e.cep  => strcmp(argv[1],e.cep) == 0
		// pode usar o strstr

		if(resultado==0)
		{
			printf("Endereco encontrado:\n\n");
			printf("%.72s\n%.72s\n%.72s\n%.72s\n%.2s\n%.8s\n",e.logradouro,e.bairro,e.cidade,e.uf,e.sigla,e.cep);
			break;
		}
	else if(resultado<0){
		fim = meio-1;
	}
	else{
		inicio = meio+1;
	}

	}
	printf("Total Lido: %d\n", c);
	fclose(f);
}

