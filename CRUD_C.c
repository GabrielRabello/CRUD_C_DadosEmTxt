/*
CRUD em C com manipulacao de arquivo de texto
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define VERDADEIRO 1

struct funcionario
{
	char cadastro[26],
		nome[31],
		email[29],
		dataAdmissao[11],
		salario[10];
};

int cadastrar();
void consultar();
int procurar();
int procurar_cadastro();
int procurar_letra();
void alterar();
void excluir();
void clrscr(); // Função para limpar a tela do terminal

int main()
{
	FILE *p1;
    struct funcionario info; // declaracao
    struct funcionario *p;
    int opcao = 0, cont;
    int tam;    //n. de bytes da estrutura
    char c;

    p = &info; // Este ponteiro será passado para as funções. Assim não se copia a estrutura inteira a cada passagem e os dados são alterados diretamente.
    tam = sizeof(info); // Pega o nº de bytes da estrutura

    for(int i = 0; i < tam; i++) // Reseta a Estrutura de Dados
    {
		info.cadastro[i]=0;
    }
    
    while(VERDADEIRO) {
        clrscr();
        printf("\t\t\tNAP 2_CRUD em C com manipulacao de arquivo de texto\n");
    	printf("Discente: Gabriel Shimada Rabello\t|\t Disciplina: Tecnicas de Programacao I 2020.1\n");
    	printf("Matricula: 2020037280\t\t\t|\t Docente: Edvar da Luz Oliveira\n");
        printf("\n----------------------------------------------");
        printf("------------------------------------------------");
        printf("\n\tCADASTRO DE PESSOAS\n");
        printf("\t01. CADASTRAR\n");
        printf("\t02. CONSULTAR\n");
        printf("\t03. ALTERAR\n");
        printf("\t04. EXCLUIR\n");
        printf("\t05. ---SAIR---\n");

        p1 = fopen("contador.txt", "r");
        if(p1 != NULL) // Na primeira abertura do programa, esse valor será NULL
        {
            fscanf(p1,"%d",&cont);
            fclose(p1);
        }
        
        printf("\n   Escolha o numero da opcao que deseja.\n-->");
        scanf("%d", &opcao);
        getchar(); // Pega o \n remanescente do buffer causado pelo scanf() acima.
        

        switch(opcao)
        {
            case 1:
                cadastrar(p, tam);
                break;
            case 2:
                consultar(p, tam);
                break;
            case 3:
                alterar(p, tam);
                break;
            case 4:
                excluir(p, tam);
                break;
            case 5:
            	printf("Confirmar saida? (s ou n)\n"); 
				scanf("%c", &c);
				if (c == 's') return 0; // Sai do while, encerrando o programa.
				else break;
            default:
                printf("\nEscolha uma opcao valida.\n");
        }
	}
}

int cadastrar(struct funcionario *ps, int tam)
{
    FILE *p, *p1;
    int cont = 0;
    char cadastro[26], q, y;

    /*--------------------------------------*/

	// Verifica e designa a quantidade de cadastros.
    p1 = fopen("contador.txt", "r");
    fscanf(p1,"%d",&cont);
    fclose(p1);
    
    for(int i=0;i<tam;i++){ // Zera os dados da estrutura
        ps->cadastro[i] = 0;
    }
    /* 
	Abrindo com a+ os registros não estão sendo lidos nem salvos corretamente por alguma razão.
	Porém é preciso abrir como "r" para ler o arquivo com fread(), a fim de verificar se o cadastro já existe no arquivo.
	Então na primeira abertura sempre haverá o erro abaixo.
	*/
    p = fopen("arquivo.txt", "r"); 
    if(p == NULL)				  
    {
    	p = fopen("arquivo.txt", "a"); 
        puts("\nERRO NA ABERTURA DO ARQUIVO.\nCRIANDO ARQUIVO...");
        puts("Pressione qualquer tecla para continuar.");
        getchar(); // Nota: A cada getchar() para input do usuario sera usado um fflush(stdin) para evitar poluir o buffer.
        fflush(stdin);
        return 1;
    }
    

    printf("\n\tCADASTRAR USUARIO:\n");
    printf("Cadastro do usuario %d:\n", (cont + 1));
    printf("\n---> Numero de cadastro do funcionario: ");
    gets(cadastro);
    //fgets(ps->cadastro, sizeof(ps->cadastro), stdin); /*fgets() seria o ideal, porém a formatação ficou complicada pro uso de fwrite() e fread();*/
    //fflush(stdin);
    
    // Verifica se o cadastro bate com algum ja existente.
    for(int i = 0; i < cont; i++) 
    {
        fread(ps,tam,1,p);
        for(y = 0; cadastro[y] != '\0';y++){
        	if(cadastro[y] != ps->cadastro[y]){ // Se for identificada alguma diferença entre os numeros, a estrutura recebe o valor inserido e o loop é encerrado
            	strcpy(ps->cadastro, cadastro); 
                break;
			}
		}
    
        if(cadastro[y] == '\0' && ps->cadastro[y] == '\0'){ // Se chegar no identificador de fim em ambos os cadastros, significa que são iguais.
        	puts("Usuario ja existente.");
        	getchar(); 
        	fflush(stdin);
        	return 0;
		}   	
    }
    fclose(p); // Depois que as leituras são finalizadas, fecha o arquivo.
	p = fopen("arquivo.txt", "a"); // Abre o arquivo no modo append para salvar as informações.
    printf("\n---> Nome (max 30 digitos): ");
    gets(ps->nome);
    printf("\n---> E-mail: ");
    gets(ps->email);
    printf("\n---> Data de admissao (DD/MM/AA): ");
    gets(ps->dataAdmissao);
    printf("\n---> Salário: ");
    gets(ps->salario);
    printf("\nConfirme os seguintes dados:\nCadastro: %s\n\tNome: %s\n\tEmail: %s\n\tData de Admissao: %s\n\tSalario: R$%s\n\n", 
										ps->cadastro, ps->nome, ps->email, ps->dataAdmissao, ps->salario);
    printf("Os dados estao corretos? (s ou n)|| ");
    if ((q = getchar()) == 's'){
    	fflush(stdin);
    	fwrite(ps,tam,1,p); // Insere as informações acima no arquivo
	}
	else return 0;
    fclose(p);

    // Atualiza a quantidade de pessoas no cadastro
    cont++;
    p1 = fopen("contador.txt", "w");
    fprintf(p1,"%d",cont);
    fclose(p1);
    puts("Pressione qualquer tecla para continuar.");
    getchar();
    fflush(stdin);
    return 0;
}

void consultar(struct funcionario *ps, int tam)
{
	FILE *p, *p1;
    struct funcionario info;
    int i, pesq;
    int cont,comp, del = 0;

    p1 = fopen("contador.txt", "r");
    if(p1 == NULL)
    {
        printf("\n\tERRO EM ABERTURA DE ARQUIVO. RETORNADO AO MENU PRINCIPAL.\n");
        getchar();
        return;
    }
	fscanf(p1, "%d", &cont);
    fclose(p1);

    p = fopen("arquivo.txt", "r");
    if(p == NULL)
    {
        printf("\n\tERRO EM ABERTURA DE ARQUIVO. RETORNANDO AO MENU PRINCIPAL.\n");
        getchar();
        fflush(stdin);
        return;
    }

    do
    {
        printf("\n\tREGISTRO COMPLETO DOS USUÁRIOS CADASTRADOS:\n\n");
        printf("\n--------------------------------------------------------------------\n");
        printf("\n COD -  Nome\n");
        del = 0; // Reseta o del para consultas consecutivas nao resultarem em diminuição errada de cont, quando há cadastros excluidos . 
        for(i = 0; i < cont; i++)
        {
			comp = i * tam;
            // MULTIPLICA O INDICE DO for PELO tam DADO COMO PARAMETRO NA FUNÇAO.
            // O tam É O TAMANHO DA ESTRUTURA DE DADOS, E AO MULTIPLICAR VOCE PEGA
            // O NUMERO QUE REPRESENTA O ENDEREÇO EXATO DA INFORMAÇÃO.

            fseek(p, comp, 0);
            // POSICIONA O PONTEIRO p NA LOCALIZAÇÃO comp NO ARQUIVO.

            fread(ps,sizeof(info),1,p);
            // LÊ O REGISTRO E GUARDA NA VARIAVEL ps UMA INFORMAÇÃO COM O TAMANHO
            // DO sizeof(info).


            if(ps->cadastro[0] != '*'){ // VERIFICA SE O ARQUIVO ESTÁ APAGADO.                      
                printf("\n%2d   - %s\n", (i + 1), ps->nome); // SE NÃO ESTIVER, SEGUE PRINTAGEM:
            }
            else{
                del++;
            }
                
        }
        printf("\n--------------------------------------------------------------------\n");
        printf("\n\n\tREGISTRO TERMINADO. %d PESSOAS CADASTRADAS E ATIVAS.\n\n\n", cont - del);

        /*--------------------------------------*/

        for(i=0;i<tam;i++){ // Zera os dados da estrutura
            ps->cadastro[i]=0;
        }


        printf("\nDIGITE O COD DE USUARIO PARA DETALHAR ALGUEM OU DIGITE  0  PRA SAIR:");
        scanf("%d", &pesq);
        getchar();
        if(pesq != 0)
        {
            for(i = 0; i < cont; i++)
                if((pesq - 1) == i)
                {
                    if(ps->cadastro[0] != '*')
                    {
                        comp = i * tam;
                        fseek(p, comp, 0);
                        fread(ps,sizeof(info),1,p);
                        printf("\n--------------------------------------\n");
                        printf("\nUSUARIO NUMERO %d:", pesq);
                        printf("\nCadastro: %s", ps->cadastro);
                        printf("\nNome: %s", ps->nome);
                        printf("\nEmail: %s", ps->email);
                        printf("\nData de Admissao: %s", ps->dataAdmissao);
                        printf("\nSalario: %s", ps->salario);
                        printf("\n--------------------------------------\n");
                        puts("Pressione qualquer tecla para continuar.");
                        getchar();
                        fflush(stdin);
                    }
                    else
                    {
                        printf("\nUSUARIO APAGADO.\n");
                        getchar();
                        fflush(stdin);
                    }

                }
        }
    } while(pesq != 0);

    fclose(p);
    puts("");
    puts("Aperte qualquer tecla para sair");
    getchar();
    fflush(stdin);
}

int procurar(struct funcionario *ps, int tam) //-----------------------------------------------------------------PROCURAR
{
    FILE *a;
    int opcao = 0;
	int y, cont;
	
    a = fopen("contador.txt", "r");
    fscanf(a,"%d",&cont);
    fclose(a);

    /*--------------------------------------*/

    printf("VOCE DESEJA PROCURAR:\n");
    printf("1 - Pelo Cadastro,\n");
    printf("2 - Pela Primeira Letra do Nome,\n");
    printf("ESCOLHA UMA OPCAO:\n-->");

    scanf("%d", &opcao);
    fflush(stdin);

    switch(opcao)
    {
        case 1:
            y = procurar_cadastro(ps, tam);
            break;
        case 2:
            y = procurar_letra(ps, tam);
            break;
    }

    if(y == -1)
    {
        printf("\nREGISTRO NAO ENCONTRADO...\n");
        getchar();
        fflush(stdin);
        return -1;
    }

    for(int i=0; i < cont; i++)
        if(y == i)
        {
            if(ps->cadastro[0] != '*')
            {
                printf("\n--------------------------------------\n");
                printf("\nUSUARIO NUMERO %d:", (y + 1));
                printf("\nCadastro: %s", ps->cadastro);
                printf("\nNome: %s", ps->nome);
                printf("\nE-mail: %s", ps->email);
                printf("\nData de Admissao: %s", ps->dataAdmissao);
                printf("\nSalario: %s", ps->salario);
                printf("\n--------------------------------------\n");
                getchar();
                fflush(stdin);
                return y;
            }
            else return -1;
		}
    return 0;
}

int procurar_cadastro(struct funcionario *ps, int tam)
{
    FILE *p;
    FILE *a;
    int cont, x, y;
    char cadastro[26];

    p = fopen("arquivo.txt", "r");
    a = fopen("contador.txt", "r");
    fscanf(a,"%d",&cont);
    fclose(a);

    printf("Informe um cadastro para pesquisa: ");
    scanf("%s", cadastro);

    for(y = 0; y < cont; y++)
    {
        fread(ps,tam,1,p);
        for(x = 0; cadastro[x] != '\0';x++)
            if(cadastro[x] != ps->cadastro[x])
                break;

        if(cadastro[x] == '\0' && ps->cadastro[x] == '\0')
            return y;
    }
    return -1;
}

int procurar_letra(struct funcionario *ps, int tam)
{
    FILE *p;
    FILE *a;
    int cont, y;
    char letra[2];
    char afirm[2];

    p = fopen("arquivo.txt", "r");
    a = fopen("contador.txt", "r");
    fscanf(a,"%d",&cont);
    fclose(a);

    printf("Informe uma letra para pesquisa: ");
    scanf("%s", letra);

    for(y = 0; y < cont; y++)
    {
        fread(ps,tam,1,p);
        if(letra[0] == ps->nome[0])
        {
            printf("\n--------------------------------------\n");
            printf("\nUSUARIO NUMERO %d:", (y + 1));
            printf("\nNome: %s", ps->nome);
            printf("\n--------------------------------------\n");
            printf("ESSE E O USUARIO QUE PROCURA? (digite 's' pra confirmar ou 'n' pra continuar busca) \n-->");
            scanf("%s", &afirm[0]);
            getchar();
            if(afirm[0] == 's')
                return y;
        }
    }
    getchar();
    fflush(stdin);
    return -1;
}

void alterar(struct funcionario *ps, int tam)
{
    clrscr();
    printf("ALTERACAO DE USUARIO:\n");
    FILE *p;
    int n_reg;
    int n_bytes;
    char c;

    printf("\nPRIMEIRO, INFORME O USUARIO A SER ALTERADO:\n");
    n_reg = procurar(ps, tam);     // pesquisa o registro no arquivo
    if(n_reg == -1)
    {
        printf("\nUSUARIO NAO ENCONTRADO PARA ALTERACAO\n");
        getchar();
        fflush(stdin);
        return;
    }

    printf("\nENCONTRADO USUARIO N: %d\n",(n_reg + 1));
    n_bytes = tam * n_reg;

    p = fopen("arquivo.txt", "r+");

    fseek(p,n_bytes,0);            //posiciona o ponteiro do arquivo no registro a ser alterado
    fread(ps, tam, 1, p );         //le registro do arquivo
    clrscr();
    printf("\nALTERAR INFORMACOES:\n-----------------------------------------------------------------------------\n");
    printf("\n---> Cadastro do funcionario: %s (Nao editavel)\n", ps->cadastro);
    printf("Alterar nome? (s ou n) || ");
    fflush(stdin);
    if((c = getchar()) == 's'){
    	printf("---> Novo nome: ");
    	fflush(stdin);
    	gets(ps->nome);
	}
	printf("\nAlterar e-mail? (s ou n) || ");
	fflush(stdin);
    if((c = getchar()) == 's'){
    	printf("---> Novo e-mail: ");
    	fflush(stdin);
    	gets(ps->email);
	}
	printf("\nAlterar data de admissao? (s ou n) || ");
	fflush(stdin);
	if((c = getchar()) == 's'){
    	printf("---> Nova data de admissao: ");
    	fflush(stdin);
    	gets(ps->dataAdmissao);
	}
	printf("\nAlterar salario? (s ou n) || ");
	fflush(stdin);
	if((c = getchar()) == 's'){
    	printf("---> Salario: ");
    	fflush(stdin);
    	gets(ps->salario);
	}
    printf("\n-----------------------------------------------------------------------------\n");

    fseek(p,n_bytes,0);            //posiciona o ponteiro do arquivo no inicio do registro a ser alterado
    fwrite(ps, tam,1,p);           //escreve o registro no arquivo

    fclose(p);
    puts("Aperte qualquer tecla para sair.");
    getchar();
    fflush(stdin);
    return;
}
void excluir(struct funcionario *ps, int tam)
{
    clrscr();
    printf("\nDELETAR REGISTRO:\n");
    FILE *p, *a;
    p = fopen("arquivo.txt", "r+");
    int n_reg, cont = 0;
    int n_bytes;
   
    a = fopen("contador.txt", "r");
    fscanf(a,"%d",&cont);
    fclose(a);
    
    printf("\nPRIMEIRO, INFORME O USUARIO A SER DELETADO:\n");
    n_reg = procurar(ps, tam); // Usa a procura para buscar o usuario a excluir.
    if(n_reg == -1)
    {
        printf("\nUSUARIO NAO ENCONTRADO PARA DELETAR\n");
        getchar();
        fflush(stdin);
        return;
    }

    // Calcula a posição do usuario e usa o Fseek().

    printf("\nENCONTRADO USUARIO N: %d\n",(n_reg + 1));
    n_bytes = tam * n_reg;

    fseek(p, n_bytes, 0);
    fread(ps, tam, 1, p);

    printf("REGISTRO PARA APAGAR: %s.  <--- DELETADO!\n\n",ps -> cadastro);
    ps->cadastro[0] = '*';

    // Registra com um asteristico para que nao seja lido como leitura valida nas outras funções.

	fseek(p,n_bytes,0);
    fwrite(ps,tam,1,p);

    fclose(p);
    puts("Aperte qualquer tecla para sair.");
    getchar();
    fflush(stdin);
}

void clrscr()
{
    system("cls||clear");
}