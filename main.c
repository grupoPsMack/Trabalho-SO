#include <stdio.h>
#include <stdlib.h>

//0-FIFO, 1-LRU, 2-CLOCK, 3-PRÓPIO

typedef struct{
    int num_pagina;
    int presente;
    //int modificado;
    //int referenciado;  a ideia é boa de usar esse, mas o que complica é qunaod teria que verificar novamente para resetar
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
}Frame;

typedef struct{
    int tamanho;
    int num_frames;
    Frame *frames;
    int frames_ocupados;
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
        if(simulador.memoria.num_frames > simulador.memoria.frames_ocupados){
            //achar o elemento vazio
            simulador.memoria.frames_ocupados += 1;
        }
        //substituição de acordo com o algoritmo implementado
        simulador.pageFaults += 1;
    }

    processo.tabelaPaginas[paginas].ultimo_acesso = simulador.tempo_atual;

    return processo.tabelaPaginas[paginas].frame * simulador.tam_pagina + deslocamento;
}


void pageFaultFIFO(Pagina pagina, memoriaFisica memoria, int pid, Simulador simulador){
    int max_time = 0;
    int frame = 0;
    for(int i=0; i<memoria.num_frames; i++){
        if(simulador.processos[memoria.frames[i].pid -1].tabelaPaginas[memoria.frames[i].pagina].tempo_carga > max_time){
            max_time = simulador.processos[memoria.frames[i].pid -1].tabelaPaginas[memoria.frames[i].pagina].tempo_carga;
            frame = i;
        }
    }


    int auxPid = memoria.frames[frame].pid;
    int aux_numPagina = memoria.frames[frame].pagina;
    memoria.frames[frame].pid = pid;
    memoria.frames[frame].pagina = pagina.num_pagina;
    pagina.frame = frame;
    pagina.presente = 1;
    pagina.tempo_carga = simulador.tempo_atual;
    pagina.ultimo_acesso = simulador.tempo_atual;

    simulador.processos[auxPid-1].tabelaPaginas[aux_numPagina].frame = -1;
    simulador.processos[auxPid-1].tabelaPaginas[aux_numPagina].presente = 0;
}

void pageFaultClock(){
    //necessario ter um "ponteiro" no simulador, ou algo que indique o index a ser trocado
    //trocar essa pagina pela acessada
    //atualizar os bits de monitoramento de ambas as paginas
    //atualizar os frames de ambas as paginas
    //passar o ponteiro para a próxima posição
}

void pageFaultLRU(Pagina pagina, memoriaFisica memoria, int pid, Simulador simulador){
    int max_time = 0;
    int frame = 0;
    for(int i=0; i<memoria.num_frames; i++){
        if(simulador.processos[memoria.frames[i].pid -1].tabelaPaginas[memoria.frames[i].pagina].ultimo_acesso > max_time){
            max_time = simulador.processos[memoria.frames[i].pid -1].tabelaPaginas[memoria.frames[i].pagina].ultimo_acesso;
            frame = i;
        }
    }


    int auxPid = memoria.frames[frame].pid;
    int aux_numPagina = memoria.frames[frame].pagina;
    memoria.frames[frame].pid = pid;
    memoria.frames[frame].pagina = pagina.num_pagina;
    pagina.frame = frame;
    pagina.presente = 1;
    pagina.tempo_carga = simulador.tempo_atual;
    pagina.ultimo_acesso = simulador.tempo_atual;

    simulador.processos[auxPid-1].tabelaPaginas[aux_numPagina].frame = NULL;
    simulador.processos[auxPid-1].tabelaPaginas[aux_numPagina].presente = 0;
}


//Função de algoritimo própio


//Função de imprimir a  memoria


//Implementação da cache TLB



int main(){
    Simulador s;
    return 0;
}


//Carregamento de informações por txt

