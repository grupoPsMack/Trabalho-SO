#include <stdio.h>
#include <stdlib.h>

//Dicionário de algoritimos


typedef struct{
    int presente;
    int modificado;
    int referenciado;
    int frame;
    int tempo_carga;
    int ultimo_acesso;
}Pagina;

typedef struct{
    int pid;
    Pagina *tabelaPaginas;
    int num_paginas;
    int tamanho;
}Processo;

typedef struct{
    int pid;
    int pagina;
    int index_memoria;
}Frame;

typedef struct{
    int tamanho;
    int num_frames;
    Frame *frames;
    int *tempo_carregamento;
}memoriaFisica;

typedef struct{
    Processo *processos;
    memoriaFisica memoria;
    int algoritimo;
    int pageFaults;
    int totalAcessos;
    int tempo_atual;
    int tam_pagina;
}Simulador;


//Função inicializadora, fazendo as contas de quantos frames devem ter, quantas paginas por processo, 


//Tradução de endereços
int traducaoEnderecos(int endereco, Processo processo, Simulador simulador){
    int paginas;
    int deslocamento;
    if(endereco % processo.tamanho == 0){
        paginas = endereco / processo.tamanho;
        deslocamento = 0;
    }else{
        paginas = endereco/processo.tamanho + 1;
        deslocamento = endereco % processo.tamanho;
    }

    if(paginas > processo.num_paginas){
        perror("Segmentation Fault");
        //PERGUNTAR SE O PAGE FAULT DO SEGMENTATION FAULT DEVE SER CONTABILIZADO;
        return 1;
    }

    if(processo.tabelaPaginas[paginas].presente == 0){
        printf("hi");
        //verificar se alguma pagina esta presente 
        //substituição de acordo com o algoriymo implementado
        //atualização do frame e bits de monitoração da pagina que saiu e da colocada
        simulador.pageFaults += 1;
    }

    /*PARA O LRU, caso uma página a ser acessada já esteja na memória e tenha passa do pelas verificações
    é necessário resetar o seu tempo*/

    return processo.tabelaPaginas[paginas].frame * simulador.tam_pagina + deslocamento;
}


void pageFaultFIFO(){
    //verificar qual pagina esta a mais tempo na memoria
    //trocar essa pagina pela acessada
    //atualizar os bits de monitoramento de ambas as paginas
    //atualizar os frames de ambas as paginas
}

void pageFaultClock(){
    //necessario ter um "ponteiro" no simulador, ou algo que indique o index a ser trocado
    //trocar essa pagina pela acessada
    //atualizar os bits de monitoramento de ambas as paginas
    //atualizar os frames de ambas as paginas
    //passar o ponteiro para a próxima posição
}

void pageFaultLRU(){
    //verificar qual pagina esta a mais tempo na memoria
    //trocar essa pagina pela acessada
    //atualizar os bits de monitoramento de ambas as paginas
    //atualizar os frames de ambas as paginas
}


//Função de algoritimo própio


//Função de imprimir a  memoria


//Implementação da cache TLB






//Carregamento de informações por txt

