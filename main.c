/*Organizao de Arquivos (SCC021501) - Profa. Dra. Cristina Dutra de Aguiar Ciferri
Trabalho 1 -
Integrantes:
Giovanni Paolo Meloni - Nusp: 10295603
Ana Carolina Fainelo de Oliveira - Nusp: 10284542
Lui Franco Rocha - Nusp: 10295558
Bruno Del Monde - Nusp: 10262818
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arqInfo.h"
#include "btree.h"

int main(int argc, char* argv[]){

    if(argc < 2){
        printf("Faltam argumentos\n");
        return 1;
    }

    //Declaracoes
    int RRN,func;
    char* arq_saida = "dados.dat"; // nome do arquivo de saida

    sscanf(argv[1],"%d",&func);

    if(func > 14 || func < 1){
        printf("Funcao nao existe\n");
        return 1;
    }
    BufferInit();
    //--------------------------------------------------------------------------------
    if(func == 1 || func == 10){ // le o arquivo CSV e salva no arquivo de saida
        if(argc < 3){
            printf("Faltam argumentos");
            return 1;
        }

        char arq_entrada[100];
        strcpy(arq_entrada,argv[2]); // o arquivo csv vem dos argumentos
        leituraArq(arq_entrada, arq_saida); // gera o arquivo de saida
    }

    else if(func == 2){ // Recupera todos os registros do arquivo de saida
        recuperaArq(arq_saida);
    }

	//--------------------------------------------------------------------------------
    else if(func == 3){ // Recupera todos os dados que satisfacao um criterio
        if(argc < 4){
            printf("Faltam argumentos");
            return 1;
        }
		//buscando
        int i, array_size;
		tRegistro** reg = search(arq_saida, argv[2], argv[3], &array_size);

        if(reg == NULL)  printf("Registro inexistente.\n");
        else if(reg == (void*)1){
            printf("Falha no processamento do arquivo.\n");
        }
        else{
            for(i = 0; i < array_size; i++){
                printf("%d %.10s %.10s %d %s %d %s %d %s\n", (reg[i])->codEscola, (reg[i])->dataInicio, (reg[i])->dataFinal,
                                               (int)strlen(reg[i]->nomeEscola), (reg[i])->nomeEscola, (int)strlen(reg[i]->municipio), (reg[i])->municipio, (int)strlen(reg[i]->endereco) ,(reg[i])->endereco );
            }
        }
    }
	//-------------------------------------------------------------------------------
    else if(func == 4){ // Recupera os registros segundo um RNN
        if(argc < 3){
            printf("Faltam argumentos");
            return 1;
        }
        //buscando
		tRegistro* reg = searchRRN(arq_saida, argv[2]);

		if(reg == NULL)  printf("Registro inexistente.\n");
        else if(reg == (void*)1){
            printf("Falha no processamento do arquivo.\n");
        }
        else{
			printf("%d %.10s %.10s %d %s %d %s %d %s\n", reg->codEscola, reg->dataInicio, reg->dataFinal, (int)strlen(reg->nomeEscola), reg->nomeEscola, (int)strlen(reg->municipio), reg->municipio, (int)strlen(reg->endereco), reg->endereco );

        }

    }
	//-------------------------------------------------------------------------------

    else if(func == 5){ // Remove um resgitro dado o RNN
        if(argc < 3){
            printf("Faltam argumentos");
            return 1;
        }
        RRN = atoi(argv[2]);// o RRN vem dos argumentos;
        removeReg(arq_saida, RRN);
    }

    //--------------------------------------------------------------------------------
    else if(func == 6 || func == 11){ // Insere um novo resgistro
        if(argc < 7){
            printf("Faltam argumentos");
            return 1;
        }
        tRegistro reg;

        reg.nomeEscola = malloc(128);
        reg.municipio = malloc(128);
        reg.endereco = malloc(128);

        sscanf(argv[2],"%d",&reg.codEscola); // prepara o registrador que sera usado na insercao
        strcpy(reg.dataInicio , argv[3]);
        strcpy(reg.dataFinal , argv[4]);
        strcpy(reg.nomeEscola , argv[5]);
        strcpy(reg.municipio , argv[6]);
        strcpy(reg.endereco , argv[7]);

        addReg(arq_saida,reg);

        free(reg.nomeEscola);
        free(reg.municipio);
        free(reg.endereco);
    }

    //--------------------------------------------------------------------------------
    else if(func == 7){ // Atualiza um registro
        if(argc < 9){
            printf("Faltam argumentos");
            return 1;
        }

        RRN = atoi(argv[2]);// o RRN vem dos argumentos;

        tRegistro reg;
        reg.nomeEscola = malloc(128);
        reg.municipio = malloc(128);
        reg.endereco = malloc(128);

        sscanf(argv[3],"%d",&reg.codEscola); // prepara o registrador que sera usado na insercao
        strcpy(reg.dataInicio , argv[4]);
        strcpy(reg.dataFinal , argv[5]);
        strcpy(reg.nomeEscola , argv[6]);
        strcpy(reg.municipio , argv[7]);
        strcpy(reg.endereco , argv[8]);

        atualizaReg(arq_saida, RRN, reg);

        free(reg.nomeEscola);
        free(reg.municipio);
        free(reg.endereco);
    }

    //--------------------------------------------------------------------------------
    else if(func == 8){ // Compacta o arquivo

        desfragmentarArq(arq_saida);

    }

    //--------------------------------------------------------------------------------
    else if(func == 9){ // Retorna os RNN dos registros removidos
        
        showStack(arq_saida);

    }

    //---------------------------------------------------------------------------------
    else if(func == 12){ // Busca por um registro
        if(argc < 3){
            printf("Faltam argumentos");
            return 1;
        }
        int RRN,index,father,codEscola,ad_rrn,ret;
        sscanf(argv[2],"%d",&codEscola);
        if((ret = searchBTree(codEscola,&RRN,&father,&index,&ad_rrn)) == 1){
            //buscando
            tRegistro* reg = searchRRNint(arq_saida, ad_rrn);

            if(reg == NULL)  printf("Registro inexistente.\n");
            else if(reg == (void*)1){
                printf("Falha no processamento do arquivo.\n");
            }
            else{
                printf("%d %.10s %.10s %d %s %d %s %d %s\n", reg->codEscola, reg->dataInicio, reg->dataFinal, (int)strlen(reg->nomeEscola), reg->nomeEscola, (int)strlen(reg->municipio), reg->municipio, (int)strlen(reg->endereco), reg->endereco );

            }
        }
        else if(ret == -1){
            printf("Falha no processamento do arquivo.\n");
        }
        else{
            printf("Registro inexistente.\n");
        }
    }

    //---------------------------------------------------------------------------------
    else if(func == 13){ // Remocao de um registro
        int codEscola;
        if(argc < 3){
            printf("Faltam argumentos");
            return 1;
        }
        int lixo,ad_rrn;
        sscanf(argv[2],"%d", &codEscola);

        if(searchBTree(codEscola,&lixo,&lixo,&lixo,&ad_rrn) == -1){
            printf("Falaha no processamento do arquivo.\n");
        }else{
            if (removeBTree(codEscola) == 0){
                removeReg(arq_saida, ad_rrn);
                printf("Registro removido com sucesso.\n");
            }
            else printf("Registro inexistente\n");
        }
    }

    //---------------------------------------------------------------------------------
    else if(func == 14){ // Atualizaçao de um registro
        if(argc < 10){
            printf("Faltam argumentos");
            return 1;
        }
        int RRN,index,father,codEscola,ad_rrn,ret;
        sscanf(argv[2],"%d",&codEscola);
        if((ret = searchBTree(codEscola,&RRN,&father,&index,&ad_rrn)) == 1){
            if (argv[2] != argv[3]){
                //atualizando no arquivo de dados
                tRegistro reg;
                reg.nomeEscola = malloc(128);
                reg.municipio = malloc(128);
                reg.endereco = malloc(128);

                sscanf(argv[3],"%d",&reg.codEscola); // prepara o registrador que sera usado na insercao
                strcpy(reg.dataInicio , argv[4]);
                strcpy(reg.dataFinal , argv[5]);
                strcpy(reg.nomeEscola , argv[6]);
                strcpy(reg.municipio , argv[7]);
                strcpy(reg.endereco , argv[8]);

                atualizaReg(arq_saida, ad_rrn, reg);
                //atualizando a Btree
                removeBTree(codEscola);
                insertBTree(reg.codEscola, ad_rrn);

                free(reg.nomeEscola);
                free(reg.municipio);
                free(reg.endereco);
                printf("Registro alterado com sucesso.");
            }
        }
        else if(ret == -1){
            printf("Falha no processamento do arquivo.\n");
        }
        else{
            printf("Registro inexistente.\n");
        }
    }

    return 0;
}
