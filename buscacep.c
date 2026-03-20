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

int main(int argc, char *argv[]){
	FILE *f;
	Endereco e;
	int c;

	if(argc != 2)//Informa a quantidade de argumentos recebidos pelo terminal
	{
		fprintf(stderr, "USO: %s [CEP]", argv[0]);//Se for menor que 2, imprime uma mensagem de erro e o programa termina
		return 1;
	}

	c = 0;
	printf("Tamanho da Estrutura: %ld\n\n", sizeof(Endereco));//Informa o tamanho da struct em bytes
	f = fopen("cep.dat","rb");//Retorna um ponteiro para o arquivo

	if(f==NULL){
		fprintf(stderr, "\nErro ao abrir o arquivo");
		return 0;
	}

	fseek(f,0,SEEK_END); //Move o cursor de leitura/escrita para o final do arquivo

	long tamanhoBytes = ftell(f); //Retorna o tamanho do arquivo em bytes, já que o cursor está no final
	long tamanhoRegistros = tamanhoBytes/sizeof(Endereco);//Informa a quantidade de registros no arquivo
	long inicio = 0; //Posição do primeiro registro do arquivo
	long fim = tamanhoRegistros-1; //Posição do último registro do arquivo
	
	fseek(f,0,SEEK_SET); //Move o cursor de volta para o início do arquivo	

	while(inicio <= fim)
	{
		c++;
		long meio = (inicio+fim)/2;
		fseek(f,meio*sizeof(Endereco),SEEK_SET); //Move o cursor de leitura para o meio do arquivo
		fread(&e,sizeof(Endereco),1,f); //Lê um registro, agora no meio, e salva no endereço de memória de e
		
		int comp = strncmp(argv[1],e.cep,8); //Compara o cep fornecido pelo usuário e o cep lido no arquivo
		if(comp==0) //Se a comparação for igual a 0, achou o cpf buscado
		{
			printf("Endereco encontrado:\n\n");
			printf("%.72s\n%.72s\n%.72s\n%.72s\n%.2s\n%.8s\n",e.logradouro,e.bairro,e.cidade,e.uf,e.sigla,e.cep);
			break;
		}
	 	if(comp<0){ //Se a comparação der menor que 0 então o cpf buscado está antes do meio
		fim = meio-1;
		}
		else{ //Caso contrário, o cpf buscado está depois do meio
			inicio = meio+1;
		}

	}
	printf("Total Lido: %d\n", c);
	fclose(f); //Fecha o arquivo

	return 0;
}

