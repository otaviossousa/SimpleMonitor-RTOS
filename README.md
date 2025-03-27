# SimpleMonitor-RTOS

Este projeto implementa um **Sistema de Monitoramento Simples** utilizando o **FreeRTOS** em um microcontrolador Raspberry Pi Pico W (RP2040). O sistema simula o monitoramento de um botão e o controle de um LED, com três tarefas cooperando para realizar as funções.

## Descrição do Sistema

O sistema é composto por três tarefas principais:

1. **Tarefa 1: Leitura do Botão**  
   - Simula a leitura do estado de um botão.
   - Alterna o estado do botão (pressionado ou não pressionado) a cada 100ms.
   - Envia o estado do botão para uma fila.

2. **Tarefa 2: Processamento do Botão**  
   - Recebe o estado do botão da fila.
   - Caso o botão esteja pressionado, aciona a próxima tarefa (controle do LED) utilizando um semáforo.

3. **Tarefa 3: Controle do LED**  
   - Aguarda o semáforo liberado pela Tarefa 2.
   - Alterna o estado do LED (aceso ou apagado).
   - Exibe o estado do LED.

## Código

- **`SimpleMonitor.c`**: Código principal do sistema, contendo as três tarefas e a lógica de comunicação entre elas.

## Estrutura do Código
1. `vTaskButtonRead`:
    - Simula a leitura do botão e envia o estado para a fila.
2. `vTaskButtonProcess`:
    - Processa o estado do botão e libera o semáforo para a tarefa do LED.
3. `vTaskLEDControl`:
    - Alterna o estado do LED e exibe o estado no Serial Monitor.

## Comunicação entre Tarefas

- Fila `QueueHandle_t`:
    - Utilizada para compartilhar o estado do botão entre as tarefas de leitura e processamento.
- Semáforo `SemaphoreHandle_t`:
    - Utilizado para sincronizar as tarefas de processamento e controle do LED.
    