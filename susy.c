/* * * * * * * * *
 * Susy
 * Rafael Barretos
 * Versao 1.5
 * 27/03/2014
 * * * * * * * * */

#include <stdio.h>
#include <stdlib.h> /* Lib para o comando system(). */


#define MAIN_DIRECTORY     "~/.zanoninho"
#define ARQUIVO_SAIDA_TEMP ".output"
#define DIFF_LOG           "diff_log"
#define GITHUB_RAW_LINK    "https://github.com/rafaelb4rreto/MC102/raw/master"


int compilou (void);

int main(int argc, char ** argv){

    /* Primeiro parametro: arquivo-fonte. Exemplo 1: lab05b.c;  Exemplo 2: /home/usuario/mc102/lab07a/source_07a.c;
     * Segundo  parametro: nome do lab.   Exemplo: 06a;
     * Terceiro parametro: numero de testes (1~10). Exemplo: 8.
     */		 

    int i, n_testes;
    char comando[9][500], entrada_teste[50], saida_teste[50], nome_programa[20];
    
    if (argc != 4){		/* Caso nao houver 3 parametros de execucao, 
                         * o Zanoninho termina retornando 1. */
                         
        printf("\nUso: <arquivo-fonte> <nome do lab> <numero de testes (1~10)>\n\n");
        return 1;
    }
    
    sprintf(nome_programa, "%s.exe", argv[2]);  /* O nome do programa compilado sera o mesmo do nome do lab + ".exe".
                                                 * Ex.: "03b.exe". */												
    sprintf(entrada_teste, "arq00.in");
    sprintf(saida_teste,   "arq00.res");
    
    n_testes = atoi(argv[3]);	
    

    
    sprintf(comando[0], "rm %s", DIFF_LOG);	
    
    sprintf(comando[1],
    "gcc -ansi -pedantic -Wall -Werror -lm -o %s %s &> .gcc_output; "
    "cat .gcc_output", nome_programa, argv[1]);
    
    
    /* REMOVE O ARQUIVO DIFF_LOG CASO ELE JA EXISTA. */
    system(comando[0]);
    
    /* GCC! Armazena o output do gcc no arquivo ".gcc_output" para depois sabermos se compilou ou nao. */
    system(comando[1]);
    
    
    
    if (compilou()){
        
        printf("\n\nCOMPILADO! \n\n");
        
        sprintf(comando[2],
        "mkdir %s", MAIN_DIRECTORY);
        
        sprintf(comando[3], 
        "(yes | unzip %s/%s.zip -d %s) || "
        "((wget \"%s/%s.zip\" -P %s) && (yes | unzip %s/%s.zip -d %s))",
        MAIN_DIRECTORY, argv[2], MAIN_DIRECTORY,
        GITHUB_RAW_LINK ,argv[2], MAIN_DIRECTORY, MAIN_DIRECTORY, argv[2], MAIN_DIRECTORY);
        
        
        sprintf(comando[7],
        "cat %s", DIFF_LOG);		
        
        
        /* CRIA OS DIRETORIOS DO ZANONINHO. */
        system(comando[2]);
        
        /* EXTRAI O ZIP DO TESTE CASO JA EXISTA. CASO CONTRARIO, BAIXA E DEPOIS EXTRAI. */
        system(comando[3]); 
        
        
        for (i = 1; i <= n_testes; i++){
            
            if (i < 10){
                entrada_teste[4] = i+48;	
                saida_teste  [4] = i+48;
            }else if (i == 10){
                entrada_teste[3] = '1';
                entrada_teste[4] = '0';
                
                saida_teste  [3] = '1';
                saida_teste  [4] = '0';
            }			

            sprintf(comando[4],
            "printf \"\\n----- TESTE %d ------\\n\" >> %s", i, DIFF_LOG);

            sprintf(comando[5],
            "((./%s < %s/%s > %s) && "
            "(diff -sby --suppress-common-lines %s/%s %s >> %s))",
            nome_programa, MAIN_DIRECTORY, entrada_teste, ARQUIVO_SAIDA_TEMP,
            MAIN_DIRECTORY, saida_teste, ARQUIVO_SAIDA_TEMP, DIFF_LOG);
            
            sprintf(comando[6],
            "printf \"\\n\" >> %s", DIFF_LOG);
            
            
            
            /* PRINTA NO ARQUIVO DIFF_LOG UMA DIVISORIA REFERENTE AO TESTE. */
            system(comando[4]);
            
    
            /* FAZ A ENTRADA DOS VALORES, GUARDA A SAIDA E DEPOIS FAZ A DIFERENCA ENTRE A SAIDA E A SAIDA-TESTE.
             * A DIFERENCA EH TAMBEM ARMAZENADA NO ARQUIVO DIFF_LOG. */
            system(comando[5]); 
            
            
            /* PRINTA UMA NOVA LINHA NO ARQUIVO DIFF_LOG. */
            system(comando[6]);
            
            
        }
        
        printf("\n\nESQUERDA: OUTPUT-TESTE (jeito certo)  |  DIREITA: O SEU OUTPUT (jeito incorreto) \n");
        
        /* MOSTRA NA TELA O CONTEUDO DO DIFF_LOG. */
        system(comando[7]);
        
        
    }else{
        printf("\n\nERRO AO COMPILAR O PROGRAMA DO ARQUIVO-FONTE %s!\n", argv[1]);
        return -1;
    }
        
    return 0;

}

int compilou(){
    
    FILE *gcc_output;
    
    int tamanho_gcc_output;
    
    /* CHECAGEM PARA VER SE O PROGRAMA COMPILOU. */
    
    gcc_output = fopen(".gcc_output", "r");
    
    fseek(gcc_output, 0, SEEK_END);
    tamanho_gcc_output = ftell(gcc_output);	

    fclose(gcc_output);
    system("rm .gcc_output");
    
    
    /* Caso o arquivo que armazena o output do gcc for vazio, o programa foi compilado. */
        
    if (tamanho_gcc_output == 0)
        return 1;	/* Compilou. */	
    else
        return 0;   /* Nao compilou. */
    
}


