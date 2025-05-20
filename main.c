#include <stdio.h>
#include <stdlib.h>

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
    int num_processos;
    int ponterio_clock;
}Simulador;


void traducaoEnderecos(int endereco, Processo *processo, Simulador *simulador);
void estatisticas(Simulador simulador);
void imprimirMemoria(memoriaFisica memoria);

int pageFaultFIFO(Pagina *pagina, memoriaFisica *memoria, int pid, Simulador *simulador);
int pageFaultLRU(Pagina *pagina, memoriaFisica *memoria, int pid, Simulador *simulador);
//int pageFaultClock(Pagina *pagina, memoriaFisica *memoria, int pid, Simulador *simulador);

void inicializacaoInput(Simulador *s);
void inicializacaoPadrao(Simulador *s);
void liberarMallocs(Simulador *simulador);


//Fórmula está errada no print do endereço físico

//clock
//aleatório
//tlb
//própio

int main(){
    Simulador s;
    int escolha;
    printf("Digite 1 para digitar os dados ou 2 para usar a configuracao padrao adotada: ");
    scanf("%d", &escolha);

    switch (escolha){
        case 1:
            inicializacaoInput(&s);
            break;
        
        case 2:
            inicializacaoPadrao(&s);
            break;

        default:
            printf("\nOpção inválida\n");
            return 1;
    }
   
    

    int matriz_teste[15][2] = {
        {1, 0},       // página 0
        {2, 4096},    // página 1
        {3, 8192},    // página 2
        {1, 1024},    // página 0 (repetido)
        {2, 5120},    // página 1 (repetido)
        {3, 9216},    // página 2 (repetido)
        {1, 4096},    // página 1 (novo)
        {2, 8192},    // página 2 (novo)
        {3, 0},       // página 0 (novo)
        {1, 2048},    // página 0 (repetido)
        {1, 6144},    // página 1 (repetido)
        {3, 10240},   // página 2 (pode ter sido substituída)
        {2, 7168},    // página 1 (possivelmente substituída)
        {3, 12288},   // página 3 (novo)
        {2, 9216}     // página 2 (possível repetição)
    };

    printf("\n\n\n\n");
    for(int i=0; i<15; i++){
        traducaoEnderecos(matriz_teste[i][1], &s.processos[matriz_teste[i][0]-1], &s);
        s.totalAcessos+=1;
        s.tempo_atual += 1;
    }

    estatisticas(s);
    liberarMallocs(&s);
    return 0;
}


void imprimirMemoria(memoriaFisica memoria){
    for(int i=0; i<memoria.num_frames; i++){
        if(memoria.frames[i].pid == -1){
            printf("Frame %d: Esse frame esta desocupado\n", i);
        }
        else{
            printf("Frame %d: processo %d; pagina %d\n", i, memoria.frames[i].pid, memoria.frames[i].pagina);
        }
    }
}

void traducaoEnderecos(int endereco, Processo *processo, Simulador *simulador){
    int paginas;
    int deslocamento;
    int frame_alocado;
    int tempo;
    if( endereco<simulador->tam_pagina){
        paginas = 0;
        deslocamento = endereco%simulador->tam_pagina;
    }else{
        paginas = (endereco/simulador->tam_pagina);
        deslocamento = endereco % processo->tamanho;
    }

    if(paginas > processo->num_paginas){
        perror("Segmentation Fault");
        //PERGUNTAR SE O PAGE FAULT DO SEGMENTATION FAULT DEVE SER CONTABILIZADO;
        return;
    }

    if(processo->tabelaPaginas[paginas].presente == 0){
        printf("\nTempo t=%d: [PAGE FAULT] Pagina %d do processo %d nao esta na memoria fisica!\n", simulador->tempo_atual, paginas, processo->pid);
        if(simulador->memoria.num_frames > simulador->memoria.frames_ocupados){
            simulador->memoria.frames_ocupados += 1;
            for(int i=0; i<simulador->memoria.num_frames; i++){

                if(simulador->memoria.frames[i].pid == -1){

                    simulador->memoria.frames[i].pid = processo->pid;
                    simulador->memoria.frames[i].pagina = paginas;
                    processo->tabelaPaginas[paginas].frame = i;
                    processo->tabelaPaginas[paginas].presente = 1;

                    tempo = simulador->tempo_atual;
                    processo->tabelaPaginas[paginas].tempo_carga = tempo;
                    processo->tabelaPaginas[paginas].ultimo_acesso = simulador->tempo_atual;
                    printf("Tempo t=%d: Carregando a pagina %d do processo %d no Frame %d\n\n", simulador->tempo_atual, paginas, processo->pid, i);
                    frame_alocado = i;
                    break;
                }
            }
        }
        else{
            switch (simulador->algoritimo){
                case 0:
                    frame_alocado = pageFaultFIFO(&processo->tabelaPaginas[paginas], &simulador->memoria, processo->pid, simulador);
                    break;
                
                /*case 1:
                    frame_alocado = pageFaultClock(&processo->tabelaPaginas[paginas], &simulador->memoria, processo->pid, simulador);*/
                
                default:
                    frame_alocado = pageFaultLRU(&processo->tabelaPaginas[paginas], &simulador->memoria, processo->pid, simulador);
                    break;
            }
        }
        simulador->pageFaults += 1;
    }
    else{
        frame_alocado = processo->tabelaPaginas[paginas].frame;

        tempo =simulador->tempo_atual;
        processo->tabelaPaginas[paginas].ultimo_acesso = tempo;
        printf("Tempo t=%d: Pagina %d do processo %d esta na memoria fisica no frame %d!\n\n", simulador->tempo_atual, paginas, processo->pid, frame_alocado);
    }
    imprimirMemoria(simulador->memoria);
    printf("Tempo t=%d: Endereco Virtual P%d: (%d) -> Pagina %d -> Frame %d -> Endereco fisico: (%d)\n\n[...continua...]\n\n", simulador->tempo_atual, processo->pid, endereco, paginas, frame_alocado, (frame_alocado*simulador->tam_pagina)+deslocamento);
}

void estatisticas(Simulador simulador){
    printf("\n\n---Estatisticas---\n");
    printf("Total de Acessos: %d\n", simulador.totalAcessos);
    printf("Page Faults: %d\n", simulador.pageFaults);
    printf("Total de Acessos: %d por cento", (simulador.pageFaults*100)/simulador.totalAcessos);
}

int pageFaultFIFO(Pagina *pagina, memoriaFisica *memoria, int pid, Simulador *simulador){
    int max_time = 0;
    int frame = 0;
    for(int i=0; i<memoria->num_frames; i++){
        if(simulador->tempo_atual - simulador->processos[memoria->frames[i].pid -1].tabelaPaginas[memoria->frames[i].pagina].tempo_carga > max_time){
            max_time = simulador->tempo_atual - simulador->processos[memoria->frames[i].pid -1].tabelaPaginas[memoria->frames[i].pagina].tempo_carga;
            frame = i;
        }
    }

    int auxPid = memoria->frames[frame].pid;
    int aux_numPagina = memoria->frames[frame].pagina;
    memoria->frames[frame].pid = pid;
    memoria->frames[frame].pagina = pagina->num_pagina;
    pagina->frame = frame;
    pagina->presente = 1;
    
    int tempo =simulador->tempo_atual;
    pagina->tempo_carga = tempo;
    pagina->ultimo_acesso = tempo;

    simulador->processos[auxPid-1].tabelaPaginas[aux_numPagina].frame = -1;
    simulador->processos[auxPid-1].tabelaPaginas[aux_numPagina].presente = 0;
    printf("\nTempo t=%d: Substituido a pagina %d do processo %d no Frame %d, Pela Pagina %d do processo %d\n", simulador->tempo_atual,aux_numPagina, auxPid, frame, pagina->num_pagina, pid);
    return frame;
}

/*int pageFaultClock(Pagina *pagina, memoriaFisica *memoria, int pid, Simulador *simulador){
    //necessario ter um "ponteiro" no simulador, ou algo que indique o index a ser trocado
    //trocar essa pagina pela acessada
    //atualizar os bits de monitoramento de ambas as paginas
    //atualizar os frames de ambas as paginas
    //passar o ponteiro para a próxima posição
    printf("A fazer");
    return 0;
}*/

int pageFaultLRU(Pagina *pagina, memoriaFisica *memoria, int pid, Simulador *simulador){
    int max_time = 0;
    int frame = 0;
    for(int i=0; i<memoria->num_frames; i++){
        if(simulador->processos[memoria->frames[i].pid -1].tabelaPaginas[memoria->frames[i].pagina].ultimo_acesso > max_time){
            max_time = simulador->processos[memoria->frames[i].pid -1].tabelaPaginas[memoria->frames[i].pagina].ultimo_acesso;
            frame = i;
        }
    }


    int auxPid = memoria->frames[frame].pid;
    int aux_numPagina = memoria->frames[frame].pagina;
    memoria->frames[frame].pid = pid;
    memoria->frames[frame].pagina = pagina->num_pagina;
    pagina->frame = frame;
    pagina->presente = 1;
    
    int tempo =simulador->tempo_atual;
    pagina->tempo_carga = tempo;
    pagina->ultimo_acesso = tempo;

    simulador->processos[auxPid-1].tabelaPaginas[aux_numPagina].frame = -1;
    simulador->processos[auxPid-1].tabelaPaginas[aux_numPagina].presente = 0;
    printf("\nTempo t=%d: Substituido a pagina %d do processo %d no Frame %d, Pela Pagina %d do processo %d\n", simulador->tempo_atual,aux_numPagina, auxPid, frame, pagina->num_pagina, pid);
    return frame;
}





void inicializacaoPadrao(Simulador *s){
    int tamanho_pagina = 4096;
    int tamanho_memoria = 16384;
    int algoritimo = 0;
    int total_processos = 3;
    int tamanho_processo = 16384;

    memoriaFisica memoria;
    memoria.tamanho = tamanho_memoria;
    memoria.num_frames = tamanho_memoria/tamanho_pagina;
    memoria.frames_ocupados = 0;
    memoria.frames = (Frame*) malloc(memoria.num_frames*sizeof(Frame));
    for(int i=0; i<memoria.num_frames; i++){
        memoria.frames[i].pid = -1;
        memoria.frames[i].pagina = -1;
    }


    s->memoria = memoria;
    s->tam_pagina = tamanho_pagina;
    s->totalAcessos = 0;
    s->pageFaults = 0;
    s->tempo_atual = 0;
    s->algoritimo = algoritimo;
    s->num_processos = total_processos;

    s->processos = (Processo*) malloc(total_processos*sizeof(Processo));
    for(int i=0; i<total_processos;i++){
        s->processos[i].pid = i+1;
        s->processos[i].tamanho = tamanho_processo;
        s->processos[i].num_paginas = s->processos[i].tamanho/s->tam_pagina;
        s->processos[i].tabelaPaginas = (Pagina*) malloc(s->processos[i].num_paginas*sizeof(Pagina));
        for(int j=0; j<s->processos[i].num_paginas; j++){
            s->processos[i].tabelaPaginas[j].num_pagina = j;
            s->processos[i].tabelaPaginas[j].presente = 0;
            s->processos[i].tabelaPaginas[j].frame  = -1;
        }
    }
}

void inicializacaoInput(Simulador *s){
    int tamanho_pagina, tamanho_memoria, algoritimo, total_processos;
    printf("Qual sera o tamanho da pagina? ");
    scanf("%d", &tamanho_pagina);
    printf("Qual sera o tamanho da memoria? ");
    scanf("%d", &tamanho_memoria);
    printf("Qual sera o algoritimo? 0-FIFO, 1-LRU, 2-CLOCK, 3-ALEATÓRIO, 4-PRÓPIO: ");
    scanf("%d", &algoritimo);

    memoriaFisica memoria;
    memoria.tamanho = tamanho_memoria;
    memoria.num_frames = tamanho_memoria/tamanho_pagina;
    memoria.frames_ocupados = 0;
    memoria.frames = (Frame*) malloc(memoria.num_frames*sizeof(Frame));
    for(int i=0; i<memoria.num_frames; i++){
        memoria.frames[i].pid = -1;
        memoria.frames[i].pagina = -1;
    }


    s->memoria = memoria;
    s->tam_pagina = tamanho_pagina;
    s->totalAcessos = 0;
    s->pageFaults = 0;
    s->tempo_atual = 0;
    s->algoritimo = algoritimo;

    printf("Quantos processos teremos? ");
    scanf("%d", &total_processos);
    s->num_processos = total_processos;
    s->processos = (Processo*) malloc(total_processos*sizeof(Processo));
    for(int i=0; i<total_processos;i++){
        s->processos[i].pid = i+1;
        printf("Qual sera o tamanho do processo? ");
        scanf("%d", &s->processos[i].tamanho);
        s->processos[i].num_paginas = s->processos[i].tamanho/s->tam_pagina;
        s->processos[i].tabelaPaginas = (Pagina*) malloc(s->processos[i].num_paginas*sizeof(Pagina));
        for(int j=0; j<s->processos[i].num_paginas; j++){
            s->processos[i].tabelaPaginas[j].num_pagina = j;
            s->processos[i].tabelaPaginas[j].presente = 0;
            s->processos[i].tabelaPaginas[j].frame  = -1;
        }
    }
}

void liberarMallocs(Simulador *simulador){
    free(simulador->memoria.frames);
    for(int i=0;i<simulador->num_processos; i++){
        free(simulador->processos[i].tabelaPaginas);
    }
    free(simulador->processos);
}