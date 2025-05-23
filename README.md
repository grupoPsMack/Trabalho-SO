# Simulador de Paginação de Memória

Este projeto é um simulador de gerenciamento de memória que implementa os algoritmos de substituição de páginas **FIFO** e **LRU**. Ele simula a tradução de endereços virtuais para físicos, contabiliza page faults e exibe o estado da memória física ao longo do tempo.

## 👤 Integrantes do grupo

* Pietro Caffettani: 10425628  
* Guilherme Haddad Borro: 10427699  
* Edmilson Li Quansang: 10425593  
* Gabriel Pereira Faravola: 10427189  
* João Paulo Berto Massabki: 10425514

## 🛠 Tecnologias Utilizadas

* Linguagem C
* Terminal/Console (para entrada e saída)
* Compilador GCC (recomendado)

## 📦 Dependências Necessárias

Para compilar e executar este projeto, você precisa ter:

* Um compilador C (como `gcc`)
* Ambiente de terminal (Linux, macOS ou WSL no Windows)
* Make (opcional, se quiser criar um Makefile)

## ▶️ Como Rodar o Projeto

1. Clone ou baixe os arquivos do projeto.

2. Compile o código com o `gcc`:

   ```bash
   gcc simulador.c -o simulador
   ```

3. Execute o programa:

   ```bash
   ./simulador
   ```

4. Escolha o modo de inicialização:

   * Digite `1` para inserir os dados manualmente.
   * Digite `2` para usar uma configuração padrão.

## 📁 Estrutura do Projeto

```
main.c           # Código-fonte principal com a lógica do simulador
README.md             # Este arquivo
```

### Principais Componentes:

* **Structs**

  * `Pagina`: Representa uma página virtual.
  * `Processo`: Representa um processo com uma tabela de páginas.
  * `Frame`: Representa um quadro da memória física.
  * `memoriaFisica`: Estrutura da memória real com frames.
  * `Simulador`: Contexto geral do simulador, controlando tempo, algoritmos e estatísticas.

* **Funções**

  * `traducaoEnderecos`: Tradução de endereços virtuais para físicos.
  * `pageFaultFIFO` / `pageFaultLRU`: Algoritmos de substituição de páginas.
  * `estatisticas`: Imprime as estatísticas da simulação.
  * `imprimirMemoria`: Exibe o estado atual da memória física.
  * `inicializacaoInput` / `inicializacaoPadrao`: Inicializam os dados de entrada.
  * `liberarMallocs`: Libera a memória alocada dinamicamente.
