#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include <stdbool.h>

// Variáveis globais
QueueHandle_t buttonStateQueue;        // Fila para compartilhar o estado do botão
SemaphoreHandle_t ledControlSemaphore; // Semáforo para controlar o LED
bool LEDState = false;                 // Estado do LED 

// Prototipação das tarefas
void vTaskButtonRead(void *pvParameters);
void vTaskButtonProcess(void *pvParameters);
void vTaskLEDControl(void *pvParameters);

int main() {
    // Inicialização 
    stdio_init_all();
    buttonStateQueue = xQueueCreate(5, sizeof(bool)); // Fila para armazenar o estado do botão
    ledControlSemaphore = xSemaphoreCreateBinary();   // Semáforo para controle do LED

    if (buttonStateQueue != NULL && ledControlSemaphore != NULL) {
        // Criação das tarefas
        xTaskCreate(vTaskButtonRead, "ButtonRead", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
        xTaskCreate(vTaskButtonProcess, "ButtonProcess", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
        xTaskCreate(vTaskLEDControl, "LEDControl", configMINIMAL_STACK_SIZE, NULL, 3, NULL);

        // Inicia o agendador do FreeRTOS
        vTaskStartScheduler();
    }

    // Caso o sistema chegue aqui, houve falha na inicialização
    for (;;);
    return 0;
}

// Tarefa 1: Leitura do botão (simulado)
void vTaskButtonRead(void *pvParameters) {
    (void)pvParameters;
    bool buttonState = false;

    for (;;) {
        // Simula a leitura do botão 
        buttonState = !buttonState;

        // Envia o estado do botão para a fila
        xQueueSend(buttonStateQueue, &buttonState, portMAX_DELAY);

        // Aguarda 100ms antes de executar novamente
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

// Tarefa 2: Processamento do botão
void vTaskButtonProcess(void *pvParameters) {
    (void)pvParameters;
    bool buttonState;

    for (;;) {
        // Recebe o estado do botão da fila
        if (xQueueReceive(buttonStateQueue, &buttonState, portMAX_DELAY)) {
            // Se o botão estiver pressionado, dá o semáforo para a tarefa do LED
            if (buttonState) {
                xSemaphoreGive(ledControlSemaphore);
            }
        }
    }
}

// Tarefa 3: Controle do LED
void vTaskLEDControl(void *pvParameters) {
    (void)pvParameters;

    for (;;) {
        // Aguarda o semáforo ser liberado pela Tarefa 2
        if (xSemaphoreTake(ledControlSemaphore, portMAX_DELAY)) {
            // Alterna o estado do LED
            LEDState = !LEDState;

            // Simula o controle do LED 
            if (LEDState) {
                printf("LED ACESO\n");
            } else {
                printf("LED APAGADO\n");
            }
        }
    }
}